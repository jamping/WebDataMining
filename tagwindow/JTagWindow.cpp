#include "..\StdAfx.h"
#include "JTagWindow.h"

CJTagWindow::CJTagWindow(TidyDoc doc,TidyNode node):m_doc(doc),m_node(node)
{
	m_strText = GetInnerText(m_doc,node,FALSE);
	m_strAnchorText = GetAnchorText();

	if( tidyNodeIsElement(node) )
		m_nNumInfoNodes = GetNumInfoNode(node);
}


CJTagWindow::~CJTagWindow(void)
{
}
/**
* 平滑函数，用来平滑我们的算法中的一些值。目前为了效率，使用非常粗糙的解题函数
* @param x 参数
* @return 平滑后的参数
*/
double CJTagWindow::fn(double x) 
{
	if (x > 0.8f) 
	{
		return 0.8f;
	}
	return x;
}
/**
* 通过所给的HTML全文参数，计算该链接在该HTML文档中的权重。
* @param nTotalT HTML文档的总字数。
* @param nTotalA HTML文档的总超链接字数。
* @param nTotalNumInfoNodes HTML文档的<a href="#">infoNode</a>个数。
* @return 该标签在所给参数的HTML文档当中的权重
*/
double CJTagWindow::Weight(int nTotalT,int nTotalA,int nTotalNumInfoNodes)
{
	double weight = 0;
	if ( tidyNodeIsElement(m_node) ) 
	{
		weight += CJUtility::IsLargeNode(m_node) ? .1 : 0;
		weight += CJUtility::IsTableNode(m_node) ? .1 : 0;
		
		weight += 0.2 * fn(m_nNumInfoNodes / (double) (nTotalNumInfoNodes));
		weight -= CJUtility::IsContainsInput(m_node) ? .5 : 0;
	}

	if (CJUtility::IsContainsNoise(m_strText)) 
	{
		weight -= 0.5;
	}

	weight += 1.0 - AnchorDensity();
	weight += Share(nTotalA, nTotalT);

	return weight;
}
    /**
     * 定义：锚文本和整体文本的比值
     * @note 特别的：如果text或者anchorText为空，那么这个anchorDensity为0
     * @return
     */
double CJTagWindow::AnchorDensity() 
{
	int anchorLen = m_strAnchorText.GetLength();
	int textLen = m_strText.GetLength();

	if (anchorLen == 0 || textLen == 0) 
	{
		return 0;
	}
	return anchorLen / (double) textLen;
}

    /**
     * 通过所给的HTML参数计算该标记在该HTML文档中所站的份额比例。
     * 计算方法是，alpha*文字所占的比例-beta超链接所占比例，公式参数详见函数内
     * @param nTotalA 该HTML文档包含的超链接文本的字数。
     * @param nTotalT 该HTML文档包含的文本字数。
     * @return 这个标记所占的比例。
     */
double CJTagWindow::Share(int nTotalA,int nTotalT)
{
	if (nTotalA == 0)
	{
		return 1.6 * fn((double) m_strText.GetLength() / nTotalT);
	}
	return 1.6 * fn((double) m_strText.GetLength() / nTotalT) - .8 * m_strAnchorText.GetLength() / nTotalA;
}

    /**
     *  获取这个标签包含的文本
     * @param bViewMode 是否依照浏览器显示的方式进行优化（将加入额外的空格和换行）？
     * @return 这个标签所包含的实际文本。
     */
CString CJTagWindow::GetInnerText(BOOL bViewMode)
{
	if (bViewMode) 
	{
		return GetInnerText(m_doc,m_node, bViewMode);
	} 
	else 
	{
		return m_strText;
	}
}
 /**
     *  获取这个标签原始文本
     * @return 这个标签所包含的原始文本。
     */
CString CJTagWindow::GetOuterText()
{
	return CJUtility::GetNodeText( m_doc,m_node,FALSE );
}
    /**
     * 这个标签所包含的锚文本字符
     * @return 这个标签所包含的锚文本字符
     */
CString CJTagWindow::GetAnchorText() 
{
	if ( tidyNodeIsElement(m_node)) 
	{
		return GetAnchorText(m_doc,m_node);
	}
	return _T("");
}
    /**
     * 获取指定标签内所包含的有效文字
     * @param node 所指定的标签
     * @param bViewMode 是否依照浏览器显示的方式进行优化（将加入额外的空格和换行）？
     * @return 获取指定标签内所包含的有效文字
     */
 CString CJTagWindow::GetInnerText(TidyDoc doc,TidyNode node,BOOL bViewMode)
 {	 
	 if ( tidyNodeIsText(node) )
	 {
		 return CJUtility::GetNodeText(doc,node);
//		 return /*CJUtility::Filter*/(CJUtility::GetNodeText(node));
	 }

	 if( tidyNodeIsElement(node))
	 {
		 if ( CJUtility::IsInvalidElement(node)) 
			 return _T("");

		 CString nodeText,str;
		 //replace the line break with space,
		 //beacause inappropriate line break may cause the paragraph corrupt.
		 if (bViewMode && tidyNodeIsBR(node))
			 nodeText += _T(" ");

		 //let the appearance tags stay
		 if (bViewMode && CJUtility::IsImgNode(node))
		 {
			 str.Format("<%s src=\"%s\" border=0>\r\n",tidyNodeGetName(node),tidyAttrValue(tidyAttrGetById(node,TidyAttr_SRC)));
			 nodeText += str;
		 }

		 for(TidyNode child = tidyGetChild(node);child;child = tidyGetNext(child))
		 {
			 str = GetInnerText(doc,child,bViewMode);

			 if (bViewMode && CJUtility::IsNeedSpace(node)) 
			 {
				 str += _T(" ");
			 }
			 nodeText += str;		 
		 }
		 //break the line, if the element is a REAL BIG tag, such as DIV,TABLE
		 if (bViewMode && CJUtility::IsNeedWarp(node) &&
			 nodeText.Trim().GetLength() != 0) 
		 {
			 nodeText += _T("\r\n");
		 }

		 return  CJUtility::ReplaceAll(nodeText,_T("[\r\n]+"), _T("\r\n"));
	 }
	 return _T("");
 }

    /**
     * 这个标签所包含的锚文本字符
     * @param node 所指定的w3cHTML元素
     * @return 这个标签所包含的锚文本字符
     */
CString CJTagWindow::GetAnchorText(TidyDoc doc,TidyNode node)
{
	CString strAnchor;
	// get anchor text
	CList<TidyNode,TidyNode> list;
	CJUtility::GetElementsByTagName(node,_T("A"),list,TRUE);
	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		TidyNode anchorNode = list.GetNext(pos);

		strAnchor += GetInnerText(doc,anchorNode,FALSE);
	}	
	return strAnchor;
}

    /**
     * 当前这个节点下包含多少个InfoNode?
     * @param node 所给定的w3c元素
     * @return 当前这个节点下包含多少个InfoNode?
     */
 int CJTagWindow::GetNumInfoNode(TidyNode node) 
 {
	 int num = CJUtility::IsInfoNode(node) ? 1 : 0;
	 
	 for(TidyNode child = tidyGetChild(node);child;child = tidyGetNext(child))
	 {
		 if( tidyNodeIsElement(child))
			 num += GetNumInfoNode(child);		 
	 }
	 
	 return num;
 }
 