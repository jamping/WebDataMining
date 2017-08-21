// WebDataMining.h : main header file for the WEBDATAMINING application
//

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "ExtractSourceTree.h"
#include "ExtractRuleTree.h"
#include "ScheduleTree.h"
#include "DBServerTree.h"
#include "OptionsData.h"
#include "thread_pool.h"
//#include "thread_pool.hpp"
#include "Topic.h"
#include "ListViewEx.h"
#include "HLSegFunc.h"
/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningApp:
// See WebDataMining.cpp for the implementation of this class
//

class CWebDataMiningApp : public CWinAppEx
{
public:
	enum DBServerType { localServer=0,remoteServer,wdmServer,wdmAccess};
    enum UserLevelType { unknowManager=-1,commonManager,systemManager,superManager};

	CWebDataMiningApp();

	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	// Override
	virtual void PreLoadState ();
	virtual void LoadCustomState ();
	virtual void SaveCustomState ();
	//
    CString GetUserDescName(UserLevelType type=commonManager);
    int  GetUserLevel() { return (int)m_nCurUserLevel; }
    CString GetCurUserName() { return m_strUserName; }
    CString GetCurUserDesc() { return m_strUserDesc; }

	void SetSourceTree(CWnd *pWnd) {m_pwndSourceTree=(CExtractSourceTree*)pWnd;}
	CExtractSourceTree* GetSourceTree(){return m_pwndSourceTree;}
	void SetRuleTree(CWnd* pWnd) { m_pwndRuleTree=(CExtractRuleTree*)pWnd;}
	CExtractRuleTree* GetRuleTree() { return m_pwndRuleTree; }
	void SetScheduleTree(CWnd *pWnd) { m_pwndScheduleTree=(CScheduleTree*)pWnd;}
	CScheduleTree* GetScheduleTree() { return m_pwndScheduleTree; }	
    void SetDBServerTree(CWnd* pWnd) { m_pwndDBServerTree=(CDBServerTree*)pWnd; }
    CDBServerTree* GetDBServerTree() { return m_pwndDBServerTree; }
    void SetShowSelfView(CWnd* pWnd) { m_pwndShowSelfView=(CListViewEx*)pWnd; }
    CListViewEx* GetShowSelfView() { return m_pwndShowSelfView; }
    void SetShowChildView(CWnd* pWnd) { m_pwndShowChildView=(CListViewEx*)pWnd; }
    CListViewEx* GetShowChildView() { return m_pwndShowChildView; }

	BOOL DBIsOpen(DBServerType nType=localServer);
	CADODatabase* GetDB(DBServerType nType=localServer) { return m_pConn[nType]; };
    BOOL ConnectDB(DBServerType nType=localServer);
	BOOL ConnectDB(const COptionsData::OptionsData::DBServerData& data,DBServerType nType=localServer);
	void CloseDB(DBServerType nType=localServer);
	
	BOOL IsVisitedUrl(const CString& strURL);
	void SaveVisitedUrlMD5(const CString& strURL);
	void DeleteVisitedUrlMD5(const CString& strURL);
	void SaveVisitedUrl();

    int& GetCurMissionCount() {  m_nCurMissionCount =thread_pool::instance().get_current_working_count(); return m_nCurMissionCount; }
	//Get config parameters
   	BOOL IsAutoRun() { return m_data.m_data.m_bAutoRun; }
    BOOL IsSingleton() { return m_data.m_data.m_bOne; }
	BOOL IsMiniToTray() { return m_data.m_data.m_bMiniToTray; }
	BOOL IsCloseToTray() { return m_data.m_data.m_bCloseToTray; }
	int  GetNetworkPortNum () { return m_data.m_data.m_nPortNum; }
	CString GetBroadcastIP() { return m_data.m_data.m_strBroadcastIP; }
	CString GetMysqlDrive() { return m_data.m_data.m_strMysqlDrive; }
	//0 is out network,1 is inner network
	int GetNetworkType() {return m_data.m_data.m_nOuterNetwork;}
	CString GetFileSaveDir(DBServerType nType=localServer);
	COptionsData::OptionsData::DBServerData& GetDBServerData(DBServerType nType=localServer);
    int GetTopicSaveDays() { return m_data.m_data.m_nTopicSaveDays; }
    int GetPictureWidth() { return m_data.m_data.m_nPictureWidth; }
	int GetPictureHeight() { return m_data.m_data.m_nPictureHeight; }
	int GetPictureInterpolation() { return m_data.m_data.m_nInterpolation; }
	int GetPictureMaxSize() { return m_data.m_data.m_nPictureMaxSize; }
	int GetTopicTopTime() { return m_data.m_data.m_nTopicTopTime; }
	int GetTopicKeyWordCount() { return m_data.m_data.m_nTopicKeyWord; }
	int GetTopicTopKeyWordCount() { return m_data.m_data.m_nTopicTopKeyWord; }
    int GetLocalDatabaseType() { return m_data.m_data.m_localDatabase.m_nType; }
	//CString GetPictureDir() { return m_data.m_data.m_strPictureDir; }
	CString GetTopicViewPath() { return m_data.m_data.m_strTopicViewPath; }
	CString GetTopicModifyPath() { return m_data.m_data.m_strTopicModifyPath; }
	CString& ExportDir() { return m_strExportDir; } 	
	
    CBaseObject* GetServerColumnRoot() { return m_pServerColumnRoot; }
    void InitServerColumn(CBaseObject* pParent=NULL);
	
	void SetLoadTopicDatetime(COleDateTime time) { m_timeLoadTopic=time; }
	COleDateTime GetLoadTopicDatetime() { return m_timeLoadTopic ;}
    void DeleteOldTopics();
	
	CTypedPtrArray<CPtrArray,CTopic*>& GetDownloadTopicsList() { return m_listDownloadTopics; }
	CTypedPtrArray<CPtrArray,CTopic*>& GetAutoPublishTopicList() { return m_listAutoPublishTopics; }
	CStringArray& GetShowTopColumnList() { return m_arrayShowTopColumn; }
	BOOL IsAutoSendTopic() { return m_bAutoSendTopic; }
	void  SendTopicToServer(BOOL bAuto =TRUE);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebDataMiningApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWebDataMiningApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileConnectDb();
	afx_msg void OnUpdateFileConnectDb(CCmdUI* pCmdUI);
	afx_msg void OnToolOptions();
	afx_msg void OnFileShowRecentTopic();
	afx_msg void OnFileSendtoServer();
	afx_msg void OnUpdateFileSendtoServer(CCmdUI* pCmdUI);
	afx_msg void OnFileDelOldtopics();
	afx_msg void OnFileClearOldurl();
	afx_msg void OnToolViewUsers();
	afx_msg void OnUpdateToolViewUsers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolViewLogs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileClearOldurl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDelOldtopics(CCmdUI* pCmdUI);
	afx_msg void OnFileLogin();
	afx_msg void OnUpdateFileLogin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolOptions(CCmdUI* pCmdUI);
	afx_msg void OnFileAutoSend();
	afx_msg void OnUpdateFileAutoSend(CCmdUI* pCmdUI);
	afx_msg void OnFileShowTopcolumn();
	afx_msg void OnFileDownloadManual();
	afx_msg void OnToolViewLogs();
	afx_msg void OnToolSearch();
	afx_msg void OnUpdateFileDownloadManual(CCmdUI* pCmdUI);
	afx_msg void OnToolImportServerColumn();
	afx_msg void OnToolBackupServerColumns();
	afx_msg void OnToolRebackupServerColumn();
	//}}AFX_MSG
	afx_msg void OnUpdateDatabaseStatus(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePictureSaveDir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNetworkStatus(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkThreadNum(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUserStatus(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	
private:
	
	CMultiDocTemplate*		m_pDocTemplateWeb;
    HINSTANCE               m_hInstBCGCBPRes;
//******************程序运行参数************************************
	COptionsData            m_data;
    //保存用户信息
	CString                 m_strUserName;
	CString                 m_strUserDesc;
	CString                 m_strUserPasswd;
    UserLevelType           m_nUserLevel;
	BOOL                    m_bSaveUserPasswd;
    BOOL                    m_bAutoSendTopic;
    BOOL                    m_bIsExpanded;
	CString                 m_strExportDir;

	UserLevelType           m_nCurUserLevel;                     
    //Database params
	//0 为本地连接，1为服务器连接,2为WDM服务器
	CADODatabase*           m_pConn[3];
    BOOL                    m_bDBConnect[3];
    
	//重载以下函数实现初始化参数操作
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
	CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL );
	
	BOOL LoadProfile();
	BOOL WriteProfile();
	BOOL Login();
	void SetAutoRun(BOOL bAutoRun=TRUE);
	void GetVisitedUrlMD5();
//******************集中管理各相关窗口************************************
	CExtractSourceTree	*m_pwndSourceTree;
	CExtractRuleTree    *m_pwndRuleTree;
	CScheduleTree       *m_pwndScheduleTree;
	CDBServerTree       *m_pwndDBServerTree;
	CListViewEx         *m_pwndShowSelfView;
	CListViewEx         *m_pwndShowChildView;
//////////////////////////////////////////////////////////////////////////
	// visited url MD5 file handle
	std::set<std::string> m_setVisitedUrlMD5; 
	ofstream m_ofsVisitedUrlMD5File;    
	//current missions
	int   m_nCurMissionCount;   

	COleDateTime m_timeLoadTopic;       //Load topics after the date
	//////////////////////////////////////////////////////////////////////////
	CTypedPtrArray<CPtrArray,CTopic*>    m_listDownloadTopics;
	CTypedPtrArray<CPtrArray,CTopic*>    m_listAutoPublishTopics;
    CStringArray                         m_arrayShowTopColumn;
	
    CBaseObject*     m_pServerColumnRoot;
	
	BOOL      m_bIsRun;
	BOOL      m_bExceptionExit;

	//查找缓存
	CStringArray     m_listSearchText;
};


extern CWebDataMiningApp theApp;