#include "stdafx.h"
#include "UniversalWDM.h"
#include "tagwindow/JTextExtractor.h"
#include "Regex.h"

CUniversalWDM::CUniversalWDM(void)
{		
}


CUniversalWDM::~CUniversalWDM(void)
{
}
CString CUniversalWDM::ParseTitle(const CString& strBuf)
{	
	CString strTitle = CRegex::Search(strBuf,_T("<\\s*title\\s*>(.*?)(?:-|_|—|\\||</\\s*title\\s*>)"));
	strTitle.Trim();

	return strTitle;
}

CString CUniversalWDM::ParseTime(const CString& strBuf)
{
	CString strTime;
	CStringArray list;
	COleDateTime t;
	BOOL bValid = FALSE;

	//CRegex::Search(strBuf,_T("(\\d{4}(?:年|-|\\.|\\/)\\d{1,2}(?:月|-|\\.|\\/)\\d{1,2}(日)?(\\s*\\d{1,2}:\\d{1,2}(:\\d{1,2})?)?)"),list);	
	CRegex::Search(strBuf,_T("((?:(\\d{4}年\\d{1,2}月\\d{1,2}日)|(\\d{4}-\\d{1,2}-\\d{1,2})|(\\d{4}.\\d{1,2}.\\d{1,2})|(\\d{4}\\/\\d{1,2}\\/\\d{1,2}))(\\s*\\d{1,2}:\\d{1,2}(:\\d{1,2})?)?)"),list);
	
	for( int i =0;i<list.GetSize();i++ )
	{
		strTime = list.GetAt(i);
		strTime.Replace('.','-');
		if(t.ParseDateTime(strTime))
		{
			bValid = TRUE;
			break;
		}
	}
	if( bValid )
	{
		strTime.Trim();
		return strTime;
	}
	else
		return _T("");
}

BOOL CUniversalWDM::FetchTopic(const CString& strBuf,CTopic** pTopic,CString& strResultHtml)
{
	if(strBuf.IsEmpty())
		return FALSE;	

	CString strContent(strBuf);
	// 删除无用的html元素
	CJUtility::FilterInvalidElements(strContent);

	CJTextExtractor extractor;

	(*pTopic)->m_data.m_strContent = extractor.Extract(strContent);
	strResultHtml = extractor.GetHtmlText();	

	(*pTopic)->m_data.m_strTitle = ParseTitle(strContent);
	(*pTopic)->m_data.m_strTime = ParseTime(strContent);	

	return TRUE;
}