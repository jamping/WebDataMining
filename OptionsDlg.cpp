// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

IMPLEMENT_DYNAMIC(COptionsDlg, CMFCPropertySheet)

COptionsDlg::COptionsDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage),m_pageDatabaseWDMServer(TRUE)
{
	SetLook (CMFCPropertySheet::PropSheetLook_Tree, 170 /* Tree control width */);
 	SetIconsList (IDB_OPTIONSIMAGES, 16 /* Image width */);

	CMFCPropertySheetCategoryInfo* pCat1 = AddTreeCategory (_T("系统配置"), 0, 1);

	AddPageToTree (pCat1, &m_pageGeneral, -1, 2);

	CMFCPropertySheetCategoryInfo* pCat2 = AddTreeCategory (_T("本地配置"), 0, 1);
 
	AddPageToTree (pCat2, &m_pageDatabaseLocal, -1, 2);

	CMFCPropertySheetCategoryInfo* pCat3 = AddTreeCategory (_T("挖掘服务器配置"), 0, 1);	

    AddPageToTree (pCat3, &m_pageDatabaseWDMServer, -1, 2);
	AddPageToTree (pCat3, &m_pageWDMConfig, -1, 2);

}

COptionsDlg::~COptionsDlg()
{
}
void COptionsDlg::SetValue(COptionsData &data)
{
	//General params
     m_pageGeneral.m_bAutoRun              =data.m_data.m_bAutoRun;
	 m_pageGeneral.m_bOne                  =data.m_data.m_bOne;
	 m_pageGeneral.m_bMiniToTray           =data.m_data.m_bMiniToTray;
	 m_pageGeneral.m_bCloseToTray          =data.m_data.m_bCloseToTray;
	 m_pageGeneral.m_nOuter                =data.m_data.m_nOuterNetwork;
	 m_pageGeneral.m_nMaxMission           =data.m_data.m_nMaxMission;
     m_pageGeneral.m_nPortNum              =data.m_data.m_nPortNum;
	 m_pageGeneral.m_strBroadcastIP        =data.m_data.m_strBroadcastIP;
	 m_pageGeneral.m_strMysqlDrive         =data.m_data.m_strMysqlDrive;
     //local database params
	 m_pageDatabaseLocal.m_nDatabaseType   =data.m_data.m_localDatabase.m_nType;
	 
	 m_pageDatabaseLocal.m_strServer       =data.m_data.m_localDatabase.m_strServer;
	 m_pageDatabaseLocal.m_strDatabase     =data.m_data.m_localDatabase.m_strDatabase;
	 m_pageDatabaseLocal.m_strUser		   =data.m_data.m_localDatabase.m_strUser;
	 m_pageDatabaseLocal.m_strPass         =data.m_data.m_localDatabase.m_strPass;
	 m_pageDatabaseLocal.m_bSavePasswd     =data.m_data.m_localDatabase.m_bSavePass;
	
     m_pageDatabaseLocal.m_strFileSaveDir  =data.m_data.m_localDatabase.m_strResourceDir;
     //wdm database params
	 m_pageDatabaseWDMServer.m_nDatabaseType=0;
	 m_pageDatabaseWDMServer.m_strServer       =data.m_data.m_wdmServerDatabase.m_strServer;
	 m_pageDatabaseWDMServer.m_strDatabase     =data.m_data.m_wdmServerDatabase.m_strDatabase;
	 m_pageDatabaseWDMServer.m_strUser		   =data.m_data.m_wdmServerDatabase.m_strUser;
	 m_pageDatabaseWDMServer.m_strPass         =data.m_data.m_wdmServerDatabase.m_strPass;
	 m_pageDatabaseWDMServer.m_bSavePasswd     =data.m_data.m_wdmServerDatabase.m_bSavePass;

     m_pageDatabaseWDMServer.m_strFileSaveDir  =data.m_data.m_wdmServerDatabase.m_strResourceDir;
     //topic params
	 m_pageWDMConfig.m_nTopicSaveDays      =data.m_data.m_nTopicSaveDays;
     m_pageWDMConfig.m_nPictureWidth       =data.m_data.m_nPictureWidth;
	 m_pageWDMConfig.m_nPictureHeight      =data.m_data.m_nPictureHeight;
	 m_pageWDMConfig.m_nInterpolation      =data.m_data.m_nInterpolation;
     m_pageWDMConfig.m_nPicMaxSize         =data.m_data.m_nPictureMaxSize;
	 m_pageWDMConfig.m_nTopicTopTime       =data.m_data.m_nTopicTopTime;
     m_pageWDMConfig.m_nTopicKeyWord       =data.m_data.m_nTopicKeyWord;
	 m_pageWDMConfig.m_nTopicTopKeyWord    =data.m_data.m_nTopicTopKeyWord;
	 m_pageWDMConfig.m_strTopicModifyPath  =data.m_data.m_strTopicModifyPath;
	 m_pageWDMConfig.m_strTopicViewPath    =data.m_data.m_strTopicViewPath;
}
void COptionsDlg::GetValue(COptionsData &data)
{
	//general params
	data.m_data.m_bAutoRun=m_pageGeneral.m_bAutoRun;
	data.m_data.m_bOne=m_pageGeneral.m_bOne;
	data.m_data.m_bMiniToTray=m_pageGeneral.m_bMiniToTray;
	data.m_data.m_bCloseToTray=m_pageGeneral.m_bCloseToTray;
	data.m_data.m_nOuterNetwork=m_pageGeneral.m_nOuter;
	data.m_data.m_nMaxMission=m_pageGeneral.m_nMaxMission;
	data.m_data.m_nPortNum=m_pageGeneral.m_nPortNum;
	data.m_data.m_strBroadcastIP=m_pageGeneral.m_strBroadcastIP;
	data.m_data.m_strMysqlDrive=m_pageGeneral.m_strMysqlDrive;
    //local databse params
	data.m_data.m_localDatabase.m_nType=m_pageDatabaseLocal.m_nDatabaseType;

	data.m_data.m_localDatabase.m_strServer=m_pageDatabaseLocal.m_strServer;
	data.m_data.m_localDatabase.m_strDatabase=m_pageDatabaseLocal.m_strDatabase;
	data.m_data.m_localDatabase.m_strUser=m_pageDatabaseLocal.m_strUser;
	data.m_data.m_localDatabase.m_strPass=m_pageDatabaseLocal.m_strPass;
	data.m_data.m_localDatabase.m_bSavePass=m_pageDatabaseLocal.m_bSavePasswd;	

	data.m_data.m_localDatabase.m_strResourceDir=m_pageDatabaseLocal.m_strFileSaveDir;	
    //wdm database params
	data.m_data.m_wdmServerDatabase.m_strServer=m_pageDatabaseWDMServer.m_strServer;
	data.m_data.m_wdmServerDatabase.m_strDatabase=m_pageDatabaseWDMServer.m_strDatabase;
	data.m_data.m_wdmServerDatabase.m_strUser=m_pageDatabaseWDMServer.m_strUser;
	data.m_data.m_wdmServerDatabase.m_strPass=m_pageDatabaseWDMServer.m_strPass;
	data.m_data.m_wdmServerDatabase.m_bSavePass=m_pageDatabaseWDMServer.m_bSavePasswd;

	data.m_data.m_wdmServerDatabase.m_strResourceDir=m_pageDatabaseWDMServer.m_strFileSaveDir;
	//topic params
	data.m_data.m_nTopicSaveDays=m_pageWDMConfig.m_nTopicSaveDays;
	data.m_data.m_nPictureWidth=m_pageWDMConfig.m_nPictureWidth;
	data.m_data.m_nPictureHeight=m_pageWDMConfig.m_nPictureHeight;
	data.m_data.m_nInterpolation=m_pageWDMConfig.m_nInterpolation;
	data.m_data.m_nPictureMaxSize=m_pageWDMConfig.m_nPicMaxSize;
    data.m_data.m_nTopicTopTime=m_pageWDMConfig.m_nTopicTopTime;
    data.m_data.m_nTopicKeyWord=m_pageWDMConfig.m_nTopicKeyWord;
	data.m_data.m_nTopicTopKeyWord=m_pageWDMConfig.m_nTopicTopKeyWord;
	data.m_data.m_strTopicViewPath=m_pageWDMConfig.m_strTopicViewPath;
	data.m_data.m_strTopicModifyPath=m_pageWDMConfig.m_strTopicModifyPath;
}
BEGIN_MESSAGE_MAP(COptionsDlg, CMFCPropertySheet)
	//{{AFX_MSG_MAP(COptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CMFCPropertySheet::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
