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
* ƽ������������ƽ�����ǵ��㷨�е�һЩֵ��ĿǰΪ��Ч�ʣ�ʹ�÷ǳ��ֲڵĽ��⺯��
* @param x ����
* @return ƽ����Ĳ���
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
* ͨ��������HTMLȫ�Ĳ���������������ڸ�HTML�ĵ��е�Ȩ�ء�
* @param nTotalT HTML�ĵ�����������
* @param nTotalA HTML�ĵ����ܳ�����������
* @param nTotalNumInfoNodes HTML�ĵ���<a href="#">infoNode</a>������
* @return �ñ�ǩ������������HTML�ĵ����е�Ȩ��
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
     * ���壺ê�ı��������ı��ı�ֵ
     * @note �ر�ģ����text����anchorTextΪ�գ���ô���anchorDensityΪ0
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
     * ͨ��������HTML��������ñ���ڸ�HTML�ĵ�����վ�ķݶ������
     * ���㷽���ǣ�alpha*������ռ�ı���-beta��������ռ��������ʽ�������������
     * @param nTotalA ��HTML�ĵ������ĳ������ı���������
     * @param nTotalT ��HTML�ĵ��������ı�������
     * @return ��������ռ�ı�����
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
     *  ��ȡ�����ǩ�������ı�
     * @param bViewMode �Ƿ������������ʾ�ķ�ʽ�����Ż������������Ŀո�ͻ��У���
     * @return �����ǩ��������ʵ���ı���
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
     *  ��ȡ�����ǩԭʼ�ı�
     * @return �����ǩ��������ԭʼ�ı���
     */
CString CJTagWindow::GetOuterText()
{
	return CJUtility::GetNodeText( m_doc,m_node,FALSE );
}
    /**
     * �����ǩ��������ê�ı��ַ�
     * @return �����ǩ��������ê�ı��ַ�
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
     * ��ȡָ����ǩ������������Ч����
     * @param node ��ָ���ı�ǩ
     * @param bViewMode �Ƿ������������ʾ�ķ�ʽ�����Ż������������Ŀո�ͻ��У���
     * @return ��ȡָ����ǩ������������Ч����
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
     * �����ǩ��������ê�ı��ַ�
     * @param node ��ָ����w3cHTMLԪ��
     * @return �����ǩ��������ê�ı��ַ�
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
     * ��ǰ����ڵ��°������ٸ�InfoNode?
     * @param node ��������w3cԪ��
     * @return ��ǰ����ڵ��°������ٸ�InfoNode?
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
 