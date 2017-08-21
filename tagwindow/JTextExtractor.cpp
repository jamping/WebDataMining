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
* ɾ���ĵ��е�һЩ��Ȼ�������������Ϣ�Ľڵ㣬����script,style,�ȵȣ����ǽ�Ӱ�����ǵ��ı���ȡ���ķ�����
* @param node ����Ҫ�����w3c�ڵ�
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
				// ɾ���ӽڵ�,������һ�ڵ�
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
     * ��ȡHTML�ı���Ϣ�����ҷֶΣ�Ϊÿһ���ı�����������Դ�֡�
     * @return ����ȡ����������Ϣ
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
	TRACE("��Ǵ�׼��ʱ��%.6f��\n",t1);

	s.Start();
	ExtractWindows(m_doc,body);	
	double t2 = s.Now();
	TRACE("���ɱ�Ǵ�ʱ��%.6f��\n",t2);
    
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
	TRACE("��������Ǵ�ʱ��%.6f��\n",t3); 
	TRACE("��Ǵ��㷨��ʱ��%.6f��\n",t1+t2+t3); 

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
     * ����ÿ��Node���󣬰�ÿ��Node���洢��taglist���С�
     * @param node ����Ҫ������w3cNode����
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
