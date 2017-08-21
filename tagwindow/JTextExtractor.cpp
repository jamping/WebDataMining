#include "..\StdAfx.h"
#include "JTextExtractor.h"
#include "JTagWindow.h"
#include "../StopWatch.h"

CJTextExtractor::CJTextExtractor(void)
{
	m_doc = tidyCreate();

	tidySetCharEncoding(m_doc,"raw");
	tidyOptSetBool( m_doc, TidyXhtmlOut, yes );	

	m_pMaxTagWindow = NULL;
}


CJTextExtractor::~CJTextExtractor(void)
{
	POSITION pos = m_windowsList.GetHeadPosition();
	while(pos)
		delete m_windowsList.GetNext(pos);

	m_pMaxTagWindow = NULL;

	tidyRelease(m_doc);
}
/**
* 删除文档中的一些显然不会包含主题信息的节点，例如script,style,等等，它们将影响我们的文本抽取器的分析。
* @param node 所需要清楚地w3c节点
*/
void CJTextExtractor::Cleanup(TidyDoc doc,TidyNode node) 
{
	TidyNode child = tidyGetChild(node);
	while(child)
	{
		if( tidyNodeIsElement(child) )
		{
			if( CJUtility::IsInvalidElement(child) )
			{
				// 删除子节点,返回下一节点
				child = CJUtility::DiscardChild(doc,child);
				continue;
			}
			else
			{
				Cleanup(doc,child);					
			}
		}
		child = tidyGetNext(child);
	}	
}

/**
     * 抽取HTML文本信息，并且分段，为每一段文本的主题相关性打分。
     * @return 所抽取出的主题信息
     */
CString CJTextExtractor::Extract(const CString& strBuf)
{			
	CStopWatch s;
	CString strBodyText = _T("");

	tidyParseString( m_doc, strBuf );           // Parse the input
	tidyCleanAndRepair( m_doc );                // Tidy it up!

	TidyNode body = tidyGetBody(m_doc);
	if( !body )
		return _T("");
	//cleanup, remove the invalid tags,
	Cleanup(m_doc,body);

	m_nTotalTextLen = CJTagWindow::GetInnerText(m_doc,body, false).GetLength();
	// get anchor text length
	m_nTotalAnchorTextLen = CJTagWindow::GetAnchorText(m_doc,body).GetLength();

	m_nTotalNumInfoNodes = CJTagWindow::GetNumInfoNode(body);
	
	double t1 = s.Now();
	TRACE("标记窗准备时间%.6f秒\n",t1);

	s.Start();
	ExtractWindows(m_doc,body);	
	double t2 = s.Now();
	TRACE("生成标记窗时间%.6f秒\n",t2);
    
	s.Start();

	if(m_windowsList.IsEmpty())
		return _T("");

	//get the max score	
	m_pMaxTagWindow = NULL;
	POSITION pos = m_windowsList.GetHeadPosition();
	m_pMaxTagWindow = m_windowsList.GetNext(pos);
	double dMaxTagWindowWeight = m_pMaxTagWindow->Weight(m_nTotalTextLen, m_nTotalAnchorTextLen, m_nTotalNumInfoNodes);
	double dNextTagWindowWeitht = 0;
	//get the max score	
	while(pos)
	{
		CJTagWindow* pNext = m_windowsList.GetNext(pos);
		dNextTagWindowWeitht = pNext->Weight(m_nTotalTextLen, m_nTotalAnchorTextLen, m_nTotalNumInfoNodes);
		if(dMaxTagWindowWeight <dNextTagWindowWeitht)
		{
		   m_pMaxTagWindow = pNext;
		   dMaxTagWindowWeight = dNextTagWindowWeitht;
		}
	}
	strBodyText = m_pMaxTagWindow->GetInnerText(TRUE);

	double t3 = s.Now();
	TRACE("计算最大标记窗时间%.6f秒\n",t3); 
	TRACE("标记窗算法总时间%.6f秒\n",t1+t2+t3); 

	return strBodyText;
}
CString CJTextExtractor::GetHtmlText()
{
	if( m_pMaxTagWindow )
		return m_pMaxTagWindow->GetOuterText();
	else
		return _T("");
}
    /**
     * 遍历每个Node对象，把每个Node都存储到taglist当中。
     * @param node 所需要遍历的w3cNode对象
     */
void CJTextExtractor::ExtractWindows(TidyDoc doc,TidyNode node) 
{
	if ( tidyNodeIsText(node) )
		return;

	if( tidyNodeIsElement(node))
	{
		if( CJUtility::IsInvalidElement(node))
			return;

		if( !CJUtility::GetNodeText(doc,node).Trim().IsEmpty() )  //add the tags
		{
			m_windowsList.AddTail(new CJTagWindow(doc,node));
		}
		for(TidyNode child = tidyGetChild(node);child;child = tidyGetNext(child))
			ExtractWindows(doc,child);
	}
}
