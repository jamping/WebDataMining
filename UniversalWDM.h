#pragma once
#include "wdmengine.h"

class CUniversalWDM :public CWDMEngine
{
public:
	CUniversalWDM(void);
	~CUniversalWDM(void);

	// жиди
	WDM_Engine_Type GetWDMEngineType() { return UNIVERSAL_WMD_ENGINE; }
	BOOL FetchTopic(const CString& strBuf,CTopic** pTopic,CString& strResultHtml);

private:
	CString ParseTitle(const CString& strBuf);
	CString ParseTime(const CString& strBuf);
};

