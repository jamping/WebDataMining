// WorkTopic.cpp: implementation of the CWorkTopic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WorkTopic.h"
#include "MainFrm.h"
#include "CheckComboBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkTopic::CWorkTopic(CTopic* pTopic,BOOL bPublish/* =TRUE */)
:m_pTopic(pTopic),m_bPublish(bPublish)
{

}
CWorkTopic::~CWorkTopic()
{

}

void CWorkTopic::process()
{	
	BOOL bRet=FALSE;

	if(m_bPublish)
		bRet=m_pTopic->PublishToServer();
	else
		bRet=m_pTopic->DeleteFromServer();
	
	CMainFrame * pMainFrm=static_cast<CMainFrame*>(theApp.GetMainWnd());
	if(pMainFrm!=NULL)
	{
		CString str;
		str.Format("%s%s====>%s",m_bPublish?_T("�������£�"):_T("ɾ��������"),
			m_pTopic->GetDesc(),bRet?_T("�ɹ�"):_T("ʧ��"));
		pMainFrm->UpdateOutputInfo(str);
	}	
}