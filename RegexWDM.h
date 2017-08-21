// RegexWDM.h: interface for the CRegexWDM class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "WDMEngine.h"
#include "Rule.h"

class CRegexWDM : public CWDMEngine  
{
public:
	enum FILTER_TYPE{FRONT,BACK,SELF};

	CRegexWDM();
	CRegexWDM(CRule* pRule);

	virtual ~CRegexWDM();

	void SetRule(CRule* pRule) { m_pRule=pRule; }
	CRule* GetRule() { return m_pRule; }
	// 重载
	WDM_Engine_Type GetWDMEngineType() { return REGEX_WDM_ENGINE; }
    BOOL FetchTopic(const CString& strBuf,CTopic** pTopic,CString& strResultHtml);
	// 常用处理
	static CString TimeFormatStrConvert(const CString& str);
	static CString GetWebPageBaseURL(const CString& strBuf);
	static int FetchPictures(const CString& strBuf,CTopic** pTopic,const CString& strBaseUrl);
	static int FetchMorePages(const CString& strBuf,CArray<HREF*,HREF*>& morePageHrefs,const CString& strBaseUrl);
	static int FetchHrefs(const CString& strBuf,CArray<HREF*,HREF*>& arHrefs,const CString& strBaseUrl);
private:
    CRule*  m_pRule;
	int     m_nStartPos;  //有效范围
	int     m_nEndPos;

	static BOOL IsValidMorePage(const HREF* pHref);
	CString FetchContentByRule(const CString& strBuf,CRuleTree* pRuleTree);	
};