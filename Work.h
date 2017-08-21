// Work.h: interface for the CWork class.
//
#pragma once

#include "thread_pool.h"
//#include "thread_pool.hpp"
#include "Entry.h"
#include "DownloadHttpFile.h"
#include "Rule.h"
#include "WDMEngine.h"

class CWork  
{
public:
	CWork(CEntry* pEntry,CRule* pRule);
	virtual ~CWork();

protected:	
	void UpdateOutputInfo(CString strInfo);

	CTopic* DownloadTopic(CString strTopicURL,BOOL bMore =TRUE);
	void RetriveKeyWord(CTopic* pTopic);
	int DownloadResource(CTopic* pTopic);
	void RetriveTopic(CString strFilePath);

	CEntry *           m_pEntry;          //������Ŀ
    HWND               m_hWnd;            //�����Ϣ���ھ��
    CDownloadHttpFile  m_http;            //����
	CWDMEngine*        m_pWDM;            //��ȡ����
};

