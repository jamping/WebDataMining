// WDMEngine.h: interface for the CWDMEngine class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>
#include "Engine.h"
#include "Topic.h"

enum WDM_Engine_Type
{
	REGEX_WDM_ENGINE,
	UNIVERSAL_WMD_ENGINE
};
struct HREF
{
	CString    m_strLink;
	CString    m_strContent;
	HREF(CString strLink,CString strContent)
	{
		m_strLink=strLink;
		m_strContent=strContent;
	}
};

class CWDMEngine : public CEngine  
{
public:
	CWDMEngine();
	virtual ~CWDMEngine();
    
	Engine_Type GetEngineType() { return WDM_ENGINE; }

	virtual WDM_Engine_Type GetWDMEngineType() =0;
    virtual BOOL FetchTopic(const CString& strBuf,CTopic** pTopic,CString& strResultHtml)=0;
};