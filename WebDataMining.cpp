// WebDataMining.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include "WebDataMining.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "WebDataMiningDoc.h"
#include "WebDataMiningView.h"
#include "LogDlg.h"
#include "OptionsDlg.h"
#include "RecentDownloadTopicDateDlg.h"
#include "UserLoginDlg.h"
#include "UserManageDlg.h"
#include "LogsDlg.h"
#include "Des.h"
#include "MySplashThread.h"
#include "UrlDlg.h"
#include "WorkFile.h"
#include "WorkUrl.h"
#include "ExportDlg.h"
#include "SearchDlg.h"
#include "Md5.h"
#include "UpdateServerColumnDlg.h"
#include "ServerColumn.h"
#include "progressbar.h"
#include "DataBase.h"
#include "AccessDataBase.h"
#include "SQLDataBase.h"
#include "OracleDatabase.h"
#include "MySQLDataBase.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
extern void g_CleanupMemory();
const std::string URL_MD5_FILE(_T("wdm.visitedurl"));
/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningApp

BEGIN_MESSAGE_MAP(CWebDataMiningApp, CWinAppEx)
	//{{AFX_MSG_MAP(CWebDataMiningApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_CONNECT_DB, OnFileConnectDb)
	ON_UPDATE_COMMAND_UI(ID_FILE_CONNECT_DB, OnUpdateFileConnectDb)
	ON_COMMAND(ID_TOOL_OPTIONS, OnToolOptions)
	ON_COMMAND(ID_FILE_SHOW_RECENT_TOPIC, OnFileShowRecentTopic)
	ON_COMMAND(ID_FILE_SENDTO_SERVER, OnFileSendtoServer)
	ON_UPDATE_COMMAND_UI(ID_FILE_SENDTO_SERVER, OnUpdateFileSendtoServer)
	ON_COMMAND(ID_FILE_DEL_OLDTOPICS, OnFileDelOldtopics)
	ON_COMMAND(ID_FILE_CLEAR_OLDURL, OnFileClearOldurl)
	ON_COMMAND(ID_TOOL_VIEW_USERS, OnToolViewUsers)
	ON_UPDATE_COMMAND_UI(ID_TOOL_VIEW_USERS, OnUpdateToolViewUsers)
	ON_UPDATE_COMMAND_UI(ID_TOOL_VIEW_LOGS, OnUpdateToolViewLogs)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEAR_OLDURL, OnUpdateFileClearOldurl)
	ON_UPDATE_COMMAND_UI(ID_FILE_DEL_OLDTOPICS, OnUpdateFileDelOldtopics)
	ON_COMMAND(ID_FILE_LOGIN, OnFileLogin)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOGIN, OnUpdateFileLogin)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPTIONS, OnUpdateToolOptions)
	ON_COMMAND(ID_FILE_AUTO_SEND, OnFileAutoSend)
	ON_UPDATE_COMMAND_UI(ID_FILE_AUTO_SEND, OnUpdateFileAutoSend)
	ON_COMMAND(ID_FILE_SHOW_TOPCOLUMN, OnFileShowTopcolumn)
	ON_COMMAND(ID_FILE_DOWNLOAD_MANUAL, OnFileDownloadManual)
	ON_COMMAND(ID_TOOL_VIEW_LOGS, OnToolViewLogs)
	ON_COMMAND(ID_TOOL_SEARCH, OnToolSearch)
	ON_UPDATE_COMMAND_UI(ID_FILE_DOWNLOAD_MANUAL, OnUpdateFileDownloadManual)
	ON_COMMAND(ID_TOOL_IMPORT_SERVER_COLUMN, OnToolImportServerColumn)
	ON_COMMAND(ID_TOOL_BACKUP_SERVER_COLUMNS, OnToolBackupServerColumns)
	ON_COMMAND(ID_TOOL_REBACKUP_SERVER_COLUMN, OnToolRebackupServerColumn)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DB_STATUS,OnUpdateDatabaseStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_PICTURE_DIR,OnUpdatePictureSaveDir)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NETWORK_TYPE,OnUpdateNetworkStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_WORK_THREAD_NUM,OnUpdateWorkThreadNum)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_USER_STATUS,OnUpdateUserStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningApp construction

CWebDataMiningApp::CWebDataMiningApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("wdm.2010.1.0"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pDocTemplateWeb = NULL;
	m_bDBConnect[0]=FALSE;
	m_bDBConnect[1]=FALSE;
    m_nCurMissionCount=0;
	m_nUserLevel=unknowManager;
	m_nCurUserLevel=unknowManager;
	m_bAutoSendTopic=TRUE;
	m_listAutoPublishTopics.RemoveAll();
	m_listDownloadTopics.RemoveAll();
	m_bIsRun=FALSE;
	m_bExceptionExit=FALSE;
	m_pwndSourceTree= NULL;
	m_pwndRuleTree= NULL;
	m_pwndScheduleTree= NULL;
	m_pwndDBServerTree= NULL;
	m_pwndShowSelfView= NULL;
	m_pwndShowChildView= NULL;
	m_arrayShowTopColumn.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWebDataMiningApp object

CWebDataMiningApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningApp initialization

BOOL CWebDataMiningApp::InitInstance()
{	
	//only one instance is allowed except -m
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//检查是否已经运行此程序
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, m_pszAppName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)		
		m_bIsRun = TRUE;
	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	SetRegistryKey(_T("WDM_NEW"));
	LoadProfile();

	if(m_bIsRun && IsSingleton())
	{
		//only one instance
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
		while (::IsWindow(hWndPrevious))
		{
			if (::GetProp(hWndPrevious, m_pszAppName))
			{
				if (::IsIconic(hWndPrevious))
					::ShowWindow(hWndPrevious,SW_RESTORE);
				else if(::IsWindowVisible(hWndPrevious)==FALSE)
					::ShowWindow(hWndPrevious,SW_MAXIMIZE);

				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));

				m_bExceptionExit=TRUE;
				return FALSE;
			}

			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
		}	
	}
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化socket库
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);
	
	// Read url cache
	GetVisitedUrlMD5();

	//Connect the database 
	if( !ConnectDB() )
	{
		do 
		{
			MessageBox(NULL,_T("数据库连接失败，请检查网络连接和数据库配置参数"),_T("错误"),MB_OK|MB_ICONWARNING);

			if(MessageBox(NULL,_T("是否检查数据库配置参数？"),_T("请选择"),MB_YESNO|MB_ICONQUESTION)==IDNO )
				return FALSE;

			COptionsDlg dlg(_T("基本参数配置"), m_pMainWnd);
			dlg.SetValue(m_data);

			if(dlg.DoModal()==IDOK)
			{
				dlg.GetValue(m_data);
				//Set the autorun flag
				SetAutoRun(m_data.m_data.m_bAutoRun);
				//Set the max mission
				thread_pool::instance().initialize(m_data.m_data.m_nMaxMission/2,m_data.m_data.m_nMaxMission);
				//Save the params to regedit
				WriteProfile();
			}
		} 
		while(!ConnectDB());		
	}

	//用户登陆 
	//允许登陆三次
	for(int i=0;i<3;i++)
	{
		if(!Login())
		{
			if(MessageBox(NULL,_T("登陆失败，是否重新登陆？"),_T("请选择"),MB_YESNO|MB_ICONQUESTION)==IDNO )
				return FALSE;			
		}
		else
			break;
	} 
	//删除历史文章
	DeleteOldTopics();

	///初始化分词
	if(!HLSplitInit())
	{
		AfxMessageBox("分词初始化失败!");
		return FALSE ;
	}
	//加载用户自定义词典
	if(!HLOpenUsrDict (CCommonFunction::GetFileFullPath(_T("wdm_dict.txt"))))
	{
		AfxMessageBox(_T("加载用户自定义词典失败！"));
		//		return FALSE;
	}
	// 	///初始化分词
	// 	if(!ICTCLAS_Init())
	// 	{
	// 		AfxMessageBox("分词初始化失败!");
	// 		return FALSE ;
	// 	}

	if(GetNetworkType()==0)
	{
		CCommonFunction::OpenOutNet();
	}

	//初始化线程池和显示文章的日期
	thread_pool::instance().initialize(m_data.m_data.m_nMaxMission/2,m_data.m_data.m_nMaxMission);
	SetLoadTopicDatetime(COleDateTime::GetCurrentTime());
	//初始化服务器栏目信息
	InitServerColumn();
	
	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	// Enable user-defined tools. If you want allow more than 10 tools,
	// add tools entry to resources (ID_USER_TOOL11, ID_USER_TOOL12,...)
// 	EnableUserTools (ID_TOOLS_ENTRY, ID_USER_TOOL1, ID_USER_TOOL10,
// 		RUNTIME_CLASS (CUserTool), IDR_MENU_ARGS, IDR_MENU_DIRS);
// 
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	//	CSingleDocTemplate* pDocTemplate;
	m_pDocTemplateWeb = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWebDataMiningDoc),
		RUNTIME_CLASS(CChildFrame),       
		RUNTIME_CLASS(CWebDataMiningView));
	AddDocTemplate(m_pDocTemplateWeb);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;

	//Create the splash-thread!
	//
	CString	vMain(_T("3.0")),
		vSeri(_T("3,0,0,0")),
		vData(_T("2016.10.26"));

	CSplashThreadDemo	*pThread = new CSplashThreadDemo();
	// you may put these static Texts to the CSplashThreadDemo()-Constructor
	pThread->AddStringHeaderMain(_T("新闻采集系统3.0"));
	pThread->AddStringHeaderSub(_T("Web Data Mining system is automatic data processing"));
	pThread->AddStringCustom(_T("海军政治工作部信息网络中心"));
	pThread->AddStringCustom(_T("Copyright 2010-2016"));

	pThread->SetTextCustomized(_T("Registered for: xxzx"));
	pThread->AddStringVersion(_T("Main"));
	pThread->AddStringVersion(_T("Serialize"));
	pThread->AddStringVersion(_T("Data"));
	pThread->AddStringVersionRight(vMain);
	pThread->AddStringVersionRight(vSeri);
	pThread->AddStringVersionRight(vData);

	pThread->SetParent(pMainFrame);
	pThread->CreateThread();
	//Load main frame
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// Parse command line for standard shell commands, DDE, file open

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		m_pDocTemplateWeb->OpenDocumentFile (NULL);
	}
	else
	{
		// Dispatch commands specified on the command line
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;
	}
	// The one and only window has been initialized, so show and update it.
	// 	m_pMainWnd->SetWindowText(_T("新闻采集3.0-海军政治部办公室"));
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();	

	CLogs::WriteLogs(_T("登陆系统成功"),CLogs::log_login);

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningApp message handlers

int CWebDataMiningApp::ExitInstance() 
{
	AfxOleTerm(FALSE);
	if( !m_bExceptionExit )
    {
		//卸载分词
	    HLFreeSplit();
		//保存链接缓存
		m_ofsVisitedUrlMD5File.close();	
// 		SaveVisitedUrl();
		//保存系统配置信息
		WriteProfile();   

		CLogs::WriteLogs(_T("安全退出系统"),CLogs::log_logout);
		
		for(int i=0;i<3;i++)
			CloseDB((DBServerType)i);
	}
	//清理内存
    g_CleanupMemory();	

	return CWinAppEx::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CMFCLinkCtrl	m_btnURL;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWebDataMiningApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CWebDataMiningApp::PreLoadState ()
{

	GetMouseManager()->AddView (IDR_MAINFRAME, _T("My view"), IDR_MAINFRAME);

	// TODO: add another views and windows were mouse double click
	// customization is required

	GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU1);
	GetContextMenuManager()->AddMenu (_T("MDI Tabs menu"), IDR_POPUP_MDITABS);
// 	GetContextMenuManager()->AddMenu (_T("Tree menu"), IDR_TREE_MENU);

	// TODO: add another context menus here
}
void CWebDataMiningApp::LoadCustomState ()
{
}

void CWebDataMiningApp::SaveCustomState ()
{
}
UINT CWebDataMiningApp::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
// 	TCHAR szPath[MAX_PATH];
// 
// 	::GetModuleFileName(NULL,szPath,MAX_PATH);	
// 	CString strIniFilePath(szPath);
//     strIniFilePath=strIniFilePath.Left(strIniFilePath.ReverseFind('\\')+1)+_T("wdm.ini");
// 	
// 	 return ::GetPrivateProfileInt(lpszSection,lpszEntry,nDefault,strIniFilePath);

	return CWinAppEx::GetProfileInt(lpszSection,lpszEntry,nDefault);
}
BOOL CWebDataMiningApp::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
//     CString strValue;
// 	strValue.Format("%d",nValue);
// 	
// 	return WriteProfileString(lpszSection,lpszEntry,strValue);

	return CWinAppEx::WriteProfileInt(lpszSection,lpszEntry,nValue);
}
CString CWebDataMiningApp::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault /* = NULL  */)
{
// 	CString strRet=_T("");
// 	TCHAR szPath[MAX_PATH];
// 
// 	::GetModuleFileName(NULL,szPath,MAX_PATH);	
// 	CString strIniFilePath(szPath);
//     strIniFilePath=strIniFilePath.Left(strIniFilePath.ReverseFind('\\')+1)+_T("wdm.ini");
// 	
// 	::GetPrivateProfileString(lpszSection,lpszEntry,lpszDefault,szPath,MAX_PATH,strIniFilePath);
// 	strRet=szPath;
// 
// 	return strRet;

	return CWinAppEx::GetProfileString(lpszSection,lpszEntry,lpszDefault);
}
BOOL CWebDataMiningApp::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
// 	TCHAR szPath[MAX_PATH];
// 
// 	::GetModuleFileName(NULL,szPath,MAX_PATH);	
// 	CString strIniFilePath(szPath);
//     strIniFilePath=strIniFilePath.Left(strIniFilePath.ReverseFind('\\')+1)+_T("wdm.ini");
// 	
// 	return ::WritePrivateProfileString(lpszSection,lpszEntry,lpszValue,strIniFilePath);

	return CWinAppEx::WriteProfileString(lpszSection,lpszEntry,lpszValue);
}
BOOL CWebDataMiningApp::LoadProfile()
{
	//密码采用des算法加密
	CDes des;
    //General params
    m_data.m_data.m_bAutoRun=(BOOL)GetProfileInt(_T("WDM"),_T("AutoRun"),0);
	m_data.m_data.m_bOne=(BOOL)GetProfileInt(_T("WDM"),_T("One"),1);
	m_data.m_data.m_bMiniToTray=(BOOL)GetProfileInt(_T("WDM"),_T("MiniToTray"),0);
	m_data.m_data.m_bCloseToTray=(BOOL)GetProfileInt(_T("WDM"),_T("CloseToTray"),0);
	m_data.m_data.m_nOuterNetwork=GetProfileInt(_T("WDM"),_T("OuterNet"),1);
	m_data.m_data.m_nMaxMission=GetProfileInt(_T("WDM"),_T("MaxMission"),10);
    m_data.m_data.m_nPortNum=GetProfileInt(_T("WDM"),_T("PortNum"),3550);
    m_data.m_data.m_strBroadcastIP=GetProfileString(_T("WDM"),_T("BroadcastIP"),_T("21.114.11.255"));
    m_data.m_data.m_strMysqlDrive=GetProfileString(_T("WDM"),_T("MysqlDrive"),_T("MySQL ODBC 5.1 Driver"));
	//Local params
	m_data.m_data.m_localDatabase.m_nType=GetProfileInt(_T("WDM"),_T("LocalDatabaseType"),1);
    m_data.m_data.m_localDatabase.m_strServer=GetProfileString(_T("WDM"),_T("LocalDatabaseServer"),_T("xxzx-news"));
	m_data.m_data.m_localDatabase.m_strDatabase=GetProfileString(_T("WDM"),_T("LocalDatabase"),_T("wdm"));
	m_data.m_data.m_localDatabase.m_strUser=GetProfileString(_T("WDM"),_T("LocalDatabaseUser"),_T("sa"));
	m_data.m_data.m_localDatabase.m_strPass=des.DesCrypteString(_T("auqfvfhpgu"),GetProfileString(_T("WDM"),_T("LocalDatabasePass"),_T("8CF37E20C40F9F12")),0);
	m_data.m_data.m_localDatabase.m_bSavePass=(BOOL)GetProfileInt(_T("WDM"),_T("LocalDatabaseSavePass"),1);

    m_data.m_data.m_localDatabase.m_strResourceDir=GetProfileString(_T("WDM"),_T("LocalFileSaveDir"),_T("D:\\下载工作目录"));
//	m_data.m_data.m_localDatabase.m_strRootValue=GetProfileString(_T("WDM"),_T("RootValue"),_T("0"));
    //Wdm params
    m_data.m_data.m_wdmServerDatabase.m_strServer=GetProfileString(_T("WDM"),_T("WDMDatabaseServer"),_T("xxzx-news"));
	m_data.m_data.m_wdmServerDatabase.m_strDatabase=GetProfileString(_T("WDM"),_T("WDMDatabase"),_T("wdm"));
	m_data.m_data.m_wdmServerDatabase.m_strUser=GetProfileString(_T("WDM"),_T("WDMDatabaseUser"),_T("sa"));
	m_data.m_data.m_wdmServerDatabase.m_strPass=des.DesCrypteString(_T("auqfvfhpgu"),GetProfileString(_T("WDM"),_T("WDMDatabasePass"),_T("8CF37E20C40F9F12")),0);
	m_data.m_data.m_wdmServerDatabase.m_bSavePass=(BOOL)GetProfileInt(_T("WDM"),_T("WDMDatabaseSavePass"),1);

    m_data.m_data.m_wdmServerDatabase.m_strResourceDir=GetProfileString(_T("WDM"),_T("WDMFileSaveDir"),_T("Y:\\下载工作目录"));
    //Topic info
	m_data.m_data.m_nTopicSaveDays=GetProfileInt(_T("WDM"),_T("TopicSaveDays"),7);
	m_data.m_data.m_nPictureWidth=GetProfileInt(_T("WDM"),_T("PictureWidth"),800);
	m_data.m_data.m_nPictureHeight=GetProfileInt(_T("WDM"),_T("PictureHeight"),600);
	m_data.m_data.m_nInterpolation=GetProfileInt(_T("WDM"),_T("PictureInterpolation"),4);
	m_data.m_data.m_nPictureMaxSize=GetProfileInt(_T("WDM"),_T("PictureMaxSize"),8);
    m_data.m_data.m_nTopicTopTime=GetProfileInt(_T("WDM"),_T("TopicTopTime"),2);
    m_data.m_data.m_nTopicKeyWord=GetProfileInt(_T("WDM"),_T("TopicKeyWordCount"),2);
	m_data.m_data.m_nTopicTopKeyWord=GetProfileInt(_T("WDM"),_T("TopicTopKeyWordCount"),40);
	//m_data.m_data.m_strPictureDir=GetProfileString(_T("WDM"),_T("PictureDir"),_T("http://21.114.11.9/files"));
	m_data.m_data.m_strTopicModifyPath=GetProfileString(_T("WDM"),_T("TopicModifyPath"),_T("http://21.114.11.9/manager/EditArticle.aspx?id="));
    m_data.m_data.m_strTopicViewPath=GetProfileString(_T("WDM"),_T("TopicViewPath"),_T("http://21.114.11.9/ShowArticle.aspx?id="));
    //User info
	m_strUserName=GetProfileString(_T("WDM"),_T("UserName"),_T(""));
	m_strUserPasswd=des.DesCrypteString(_T("auqfvfhpgu"),GetProfileString(_T("WDM"),_T("UserPasswd"),_T("")),0);
	m_nUserLevel=(UserLevelType)GetProfileInt(_T("WDM"),_T("UserLevel"),0);
    m_bSaveUserPasswd=(BOOL)GetProfileInt(_T("WDM"),_T("SaveUserPasswd"),1);
    m_bAutoSendTopic=(BOOL)GetProfileInt(_T("WDM"),_T("AutoSendTopic"),1);
	m_bIsExpanded=(BOOL)GetProfileInt(_T("WDM"),_T("IsExpanded"),1);
    //Top column info
	int nCount=GetProfileInt(_T("WDM"),_T("TopicColumnNum"),0);
	CString str;
	for(int i=0;i<nCount;i++)
	{
		str.Format("TopicColumnName%02d",i+1);
        GetShowTopColumnList().Add(GetProfileString(_T("WDM"),str,_T("")));
	}
	m_strExportDir=GetProfileString(_T("WDM"),_T("ExportDir"),_T("D:\\导出")); 

	return TRUE;
}
BOOL CWebDataMiningApp::WriteProfile()
{
	//密码采用des算法加密
	CDes des;
    //General params
    WriteProfileInt(_T("WDM"),_T("AutoRun"),(int)m_data.m_data.m_bAutoRun);
	WriteProfileInt(_T("WDM"),_T("One"),(int)m_data.m_data.m_bOne);
	WriteProfileInt(_T("WDM"),_T("MiniToTray"),(int)m_data.m_data.m_bMiniToTray);
	WriteProfileInt(_T("WDM"),_T("CloseToTray"),(int)m_data.m_data.m_bCloseToTray);
	WriteProfileInt(_T("WDM"),_T("OuterNet"),m_data.m_data.m_nOuterNetwork);
	WriteProfileInt(_T("WDM"),_T("MaxMission"),m_data.m_data.m_nMaxMission);
	WriteProfileInt(_T("WDM"),_T("PortNum"),m_data.m_data.m_nPortNum);
	WriteProfileString(_T("WDM"),_T("BroadcastIP"),m_data.m_data.m_strBroadcastIP);
	WriteProfileString(_T("WDM"),_T("MysqlDrive"),m_data.m_data.m_strMysqlDrive);
    //Loacal database 
	WriteProfileInt(_T("WDM"),_T("LocalDatabaseType"),(int)m_data.m_data.m_localDatabase.m_nType);
    WriteProfileString(_T("WDM"),_T("LocalDatabaseServer"),m_data.m_data.m_localDatabase.m_strServer);
	WriteProfileString(_T("WDM"),_T("LocalDatabase"),m_data.m_data.m_localDatabase.m_strDatabase);
	WriteProfileString(_T("WDM"),_T("LocalDatabaseUser"),m_data.m_data.m_localDatabase.m_strUser);
	WriteProfileString(_T("WDM"),_T("LocalDatabasePass"),des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_data.m_localDatabase.m_strPass,1));
	WriteProfileInt(_T("WDM"),_T("LocalDatabaseSavePass"),(int)m_data.m_data.m_localDatabase.m_bSavePass);

   	WriteProfileString(_T("WDM"),_T("LocalFileSaveDir"),m_data.m_data.m_localDatabase.m_strResourceDir);
//	WriteProfileString(_T("WDM"),_T("RootValue"),m_data.m_data.m_localDatabase.m_strRootValue);
    //wdm database
    WriteProfileString(_T("WDM"),_T("WDMDatabaseServer"),m_data.m_data.m_wdmServerDatabase.m_strServer);
	WriteProfileString(_T("WDM"),_T("WDMDatabase"),m_data.m_data.m_wdmServerDatabase.m_strDatabase);
	WriteProfileString(_T("WDM"),_T("WDMDatabaseUser"),m_data.m_data.m_wdmServerDatabase.m_strUser);
	WriteProfileString(_T("WDM"),_T("WDMDatabasePass"),des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_data.m_wdmServerDatabase.m_strPass,1));
	WriteProfileInt(_T("WDM"),_T("WDMDatabaseSavePass"),(int)m_data.m_data.m_wdmServerDatabase.m_bSavePass);   

	WriteProfileString(_T("WDM"),_T("WDMFileSaveDir"),m_data.m_data.m_wdmServerDatabase.m_strResourceDir);
    //topic info
	WriteProfileInt(_T("WDM"),_T("TopicSaveDays"),m_data.m_data.m_nTopicSaveDays);
	WriteProfileInt(_T("WDM"),_T("PictureWidth"),m_data.m_data.m_nPictureWidth);
	WriteProfileInt(_T("WDM"),_T("PictureHeight"),m_data.m_data.m_nPictureHeight);
	WriteProfileInt(_T("WDM"),_T("PictureInterpolation"),m_data.m_data.m_nInterpolation);
	WriteProfileInt(_T("WDM"),_T("PictureMaxSize"),m_data.m_data.m_nPictureMaxSize);
	WriteProfileInt(_T("WDM"),_T("TopicTopTime"),m_data.m_data.m_nTopicTopTime);
    WriteProfileInt(_T("WDM"),_T("TopicKeyWordCount"),m_data.m_data.m_nTopicKeyWord);
	WriteProfileInt(_T("WDM"),_T("TopicTopKeyWordCount"),m_data.m_data.m_nTopicTopKeyWord);
	//WriteProfileString(_T("WDM"),_T("PictureDir"),m_data.m_data.m_strPictureDir);
	WriteProfileString(_T("WDM"),_T("TopicModifyPath"),m_data.m_data.m_strTopicModifyPath);
	WriteProfileString(_T("WDM"),_T("TopicViewPath"),m_data.m_data.m_strTopicViewPath);
    //user info
	WriteProfileString(_T("WDM"),_T("UserName"),m_strUserName);
	WriteProfileString(_T("WDM"),_T("UserPasswd"),des.DesCrypteString(_T("auqfvfhpgu"),m_strUserPasswd,1));
	WriteProfileInt(_T("WDM"),_T("UserLevel"),(int)m_nUserLevel);
    WriteProfileInt(_T("WDM"),_T("SaveUserPasswd"),(int)m_bSaveUserPasswd);
	WriteProfileInt(_T("WDM"),_T("AutoSendTopic"),(int)m_bAutoSendTopic);
	WriteProfileInt(_T("WDM"),_T("IsExpanded"),(int)m_bIsExpanded);
    //top column info 
	int nCount=GetShowTopColumnList().GetSize();
	WriteProfileInt(_T("WDM"),_T("TopicColumnNum"),nCount);
	CString str;
	for(int i=0;i<nCount;i++)
	{
		str.Format("TopicColumnName%02d",i+1);
        WriteProfileString(_T("WDM"),str,GetShowTopColumnList().GetAt(i));
	}
    WriteProfileString(_T("WDM"),_T("ExportDir"),m_strExportDir);

	return TRUE;
}
BOOL CWebDataMiningApp::Login()
{
	BOOL bRet=FALSE;
	CUserLoginDlg dlgLog(m_data.m_data.m_bAutoRun);

	dlgLog.m_strUserName=m_strUserName;	
	dlgLog.m_nUserLevel=(int)m_nUserLevel;
	dlgLog.m_bSavePasswd=m_bSaveUserPasswd;
    dlgLog.m_bIsAutoSendTopic=m_bAutoSendTopic;
    dlgLog.m_bIsExpanded=m_bIsExpanded;

	if(dlgLog.m_bSavePasswd)
         dlgLog.m_strUserPasswd=m_strUserPasswd;
	else
		dlgLog.m_strUserPasswd=_T("");

	if(dlgLog.DoModal() == IDOK)
	{
		m_strUserName=dlgLog.m_strUserName;
		m_strUserDesc=dlgLog.m_strUserDesc;
		m_strUserPasswd=dlgLog.m_strUserPasswd;
		m_nUserLevel=(UserLevelType)dlgLog.m_nUserLevel;
		m_bSaveUserPasswd=dlgLog.m_bSavePasswd;	
		m_bAutoSendTopic=dlgLog.m_bIsAutoSendTopic;
		m_nCurUserLevel=m_nUserLevel;
		m_bIsExpanded=dlgLog.m_bIsExpanded;
		
		bRet=TRUE;
	}
//   MessageBox(NULL,bRet?_T("合法用户，登陆成功"):_T("非法用户，登陆失败"),_T("请注意"),MB_OK);

	return bRet;
}
BOOL CWebDataMiningApp::DBIsOpen(DBServerType nType/* =localServer */)
{
	if(m_pConn[nType]!=NULL)
		return m_pConn[nType]->IsOpen();

    return FALSE;
}
BOOL CWebDataMiningApp::ConnectDB(const COptionsData::OptionsData::DBServerData& data,DBServerType nType/* =localServer */)
{
  	switch(nType)
	{
	case localServer:    //本地
		m_data.m_data.m_localDatabase.m_nType=data.m_nType;
		m_data.m_data.m_localDatabase.m_strServer=data.m_strServer;
		m_data.m_data.m_localDatabase.m_strDatabase=data.m_strDatabase;
		m_data.m_data.m_localDatabase.m_strUser=data.m_strUser;
		m_data.m_data.m_localDatabase.m_strPass=data.m_strPass;
		m_data.m_data.m_localDatabase.m_strResourceDir=data.m_strResourceDir;
		break;
	case remoteServer:  //服务器
		m_data.m_data.m_ServerDatabase.m_strServer=data.m_strServer;
		m_data.m_data.m_ServerDatabase.m_strDatabase=data.m_strDatabase;
		m_data.m_data.m_ServerDatabase.m_strUser=data.m_strUser;
		m_data.m_data.m_ServerDatabase.m_strPass=data.m_strPass;
		m_data.m_data.m_ServerDatabase.m_strResourceDir=data.m_strResourceDir;
		m_data.m_data.m_ServerDatabase.m_strResourcePath=data.m_strResourcePath;
		m_data.m_data.m_ServerDatabase.m_nType=data.m_nType;
		m_data.m_data.m_ServerDatabase.m_nPort=data.m_nPort;
		m_data.m_data.m_ServerDatabase.m_strCode=data.m_strCode;
		break;
	case wdmServer:  //WDM服务器
		m_data.m_data.m_wdmServerDatabase.m_strServer=data.m_strServer;
		m_data.m_data.m_wdmServerDatabase.m_strDatabase=data.m_strDatabase;
		m_data.m_data.m_wdmServerDatabase.m_strUser=data.m_strUser;
		m_data.m_data.m_wdmServerDatabase.m_strPass=data.m_strPass;
		m_data.m_data.m_wdmServerDatabase.m_strResourceDir=data.m_strResourceDir;
		
		break;
	default:
		ASSERT(FALSE);
	}
	return ConnectDB(nType);
}
BOOL CWebDataMiningApp::ConnectDB(DBServerType nType/* =localServer */)
{
	if(DBIsOpen(nType))
        return TRUE;
			 
	CString connStr,strLogs;
	CDataBase* pDatabase = NULL;

	switch(nType)
	{
	case localServer:    //本地 
		if(GetLocalDatabaseType()==0)
		{
			// SQL server
			pDatabase = new CSQLDataBase(m_data.m_data.m_localDatabase.m_strUser,
				                         m_data.m_data.m_localDatabase.m_strPass,
		                                 m_data.m_data.m_localDatabase.m_strServer,
					                     m_data.m_data.m_localDatabase.m_strDatabase);
		}
		else
		{
			// Access 
            pDatabase = new CAccessDataBase(CCommonFunction::GetFileFullPath(_T("wdm.mdb")));
//			connStr.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s",CCommonFunction::GetFileFullPath(_T("wdm.mdb")));
		}
 		strLogs=_T("连接本地数据库");
		break;
	case remoteServer:  //服务器
		{
			switch (m_data.m_data.m_ServerDatabase.m_nType)
			{
			case 0:   //SQL server
				pDatabase = new CSQLDataBase(m_data.m_data.m_ServerDatabase.m_strUser,		
				              				 m_data.m_data.m_ServerDatabase.m_strPass,
				                             m_data.m_data.m_ServerDatabase.m_strServer,
											 m_data.m_data.m_ServerDatabase.m_strDatabase);
				break;
			case 1:   //Oracel server 
                pDatabase = new COracleDatabase(m_data.m_data.m_ServerDatabase.m_strUser,
					                            m_data.m_data.m_ServerDatabase.m_strPass,
				                                m_data.m_data.m_ServerDatabase.m_strServer,
												m_data.m_data.m_ServerDatabase.m_nPort);
				break;
			case 2:   //MySQL  server
				pDatabase = new CMySQLDataBase(m_data.m_data.m_ServerDatabase.m_strServer,
					                           m_data.m_data.m_ServerDatabase.m_strDatabase,
					                           m_data.m_data.m_ServerDatabase.m_strUser,
					                           m_data.m_data.m_ServerDatabase.m_strPass,
											   m_data.m_data.m_ServerDatabase.m_nPort,
											   m_data.m_data.m_ServerDatabase.m_strCode);
				break;
			case 3:   //Access
				break;
			}
		}
		strLogs=_T("连接远程数据库");
		break;
	case wdmServer:  //WDM服务器
		pDatabase = new CSQLDataBase(m_data.m_data.m_wdmServerDatabase.m_strUser,m_data.m_data.m_wdmServerDatabase.m_strPass,
		     m_data.m_data.m_wdmServerDatabase.m_strServer,m_data.m_data.m_wdmServerDatabase.m_strDatabase);
		strLogs=_T("连接挖掘数据库");
		break;
	default:
		ASSERT(FALSE);
	}
		
	connStr = pDatabase->GetDataBaseConnString();
	delete pDatabase;
	pDatabase = NULL;

	if(m_pConn[nType]==NULL)
	{
		m_pConn[nType]=new CADODatabase;
	}
	if(m_pConn[nType]->Open(connStr))
	{
		m_bDBConnect[nType]=TRUE;
		strLogs+=_T("成功");
	}
	else
	{
		delete m_pConn[nType];
		m_pConn[nType]=NULL;
		strLogs+=_T("失败");
	}
		
// 	CLogs::WriteLogs(strLogs,CLogs::log_open_db);

	return m_bDBConnect[nType];
}
void CWebDataMiningApp::CloseDB(DBServerType nType/* =localServer */)
{
	if(DBIsOpen(nType))
	{
		CString strLogs;
		switch(nType)
		{
		case localServer:    //本地
			strLogs=_T("断开本地数据库");
			break;
		case remoteServer:  //服务器
			strLogs=_T("断开远程数据库");
			break;
		case wdmServer:  //WDM服务器
			strLogs=_T("断开挖掘数据库");
			break;
		default:
			ASSERT(FALSE);
		}
// 		CLogs::WriteLogs(strLogs,CLogs::log_close_db);

		m_pConn[nType]->Close();
		delete m_pConn[nType];
		m_pConn[nType]=NULL;
		m_bDBConnect[nType]=FALSE;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningApp message handlers
void CWebDataMiningApp::OnUpdateDatabaseStatus(CCmdUI* pCmdUI)
{	
	pCmdUI->SetText(DBIsOpen()?_T("数据库已连接"):_T("数据库已断开"));
	pCmdUI->Enable(TRUE);
}
void CWebDataMiningApp::OnUpdatePictureSaveDir(CCmdUI* pCmdUI)
{	
	pCmdUI->SetText(_T("图片保存目录 ")+GetFileSaveDir());
	pCmdUI->Enable(TRUE);
}
void CWebDataMiningApp::OnUpdateNetworkStatus(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(GetNetworkType()==0?_T("外网"):_T("内网"));
	pCmdUI->Enable(TRUE);
}
void CWebDataMiningApp::OnUpdateWorkThreadNum(CCmdUI* pCmdUI)
{
	CString str;
	str.Format("当前工作线程数%d",thread_pool::instance().get_current_working_count());
	pCmdUI->SetText(str);
	pCmdUI->Enable(TRUE);
}
void CWebDataMiningApp::OnFileConnectDb() 
{
	// TODO: Add your command handler code here
	if(DBIsOpen())   
	{	
		if(MessageBox(NULL,_T("关闭数据库吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
		{
			//关闭原来连接
			for(int i=0;i<3;i++)
			    CloseDB((DBServerType)i);
			
			GetSourceTree()->InitUpdate();
			GetRuleTree()->InitUpdate();
			GetScheduleTree()->InitUpdate();
			GetDBServerTree()->InitUpdate();
			
			MessageBox(NULL,_T("数据库连接已关闭"),_T("新闻采集系统"),MB_OK);
		}	
	}
	else
	{
		//准备数据库连接
		ConnectDB();
		GetSourceTree()->InitUpdate(TRUE,TRUE);
		GetRuleTree()->InitUpdate(TRUE,TRUE);
		GetScheduleTree()->InitUpdate(TRUE,TRUE);
		GetDBServerTree()->InitUpdate(TRUE,TRUE);

		InitServerColumn();
		
		MessageBox(NULL,m_bDBConnect?_T("数据库连接成功":"数据库连接失败"),_T("新闻采集系统"),MB_OK);

	}
}

void CWebDataMiningApp::OnUpdateFileConnectDb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(DBIsOpen()?_T("断开"):_T("连接"));
}

void CWebDataMiningApp::OnToolOptions() 
{
	// TODO: Add your command handler code here
	CLogs::WriteLogs(_T("管理用户配置信息"),CLogs::log_admin);

	COptionsDlg dlg(_T("基本参数配置"), m_pMainWnd);
	dlg.SetValue(m_data);

	if(dlg.DoModal()==IDOK)
	{
		dlg.GetValue(m_data);
		//Set the autorun flag
		SetAutoRun(m_data.m_data.m_bAutoRun);
		//Set the max mission
		thread_pool::instance().initialize(m_data.m_data.m_nMaxMission/2,m_data.m_data.m_nMaxMission);
		//Save the params to regedit
		WriteProfile();
		//Connect the database repeatedly
		CloseDB();
		if(!ConnectDB())
		{
			MessageBox(NULL,_T("连接数据库失败"),_T("错误"),MB_OK|MB_ICONWARNING);			
		}
	}
}
void CWebDataMiningApp::SetAutoRun(BOOL bAutoRun/*=TRUE*/)
{
	char path[256];
	::GetModuleFileName(NULL,path,256);
	
	CString strPath=(CString)path;

	LPSTR szSubKey=_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	LPSTR szAutoRun=_T("WDM");
	CSettingsStore  registry(TRUE,FALSE);
    CString strValue;

	if(registry.Open(szSubKey))
	{
		if(bAutoRun)
		{
			registry.Write(szAutoRun,strPath);
		}
		else 
		{			
			registry.DeleteValue(szAutoRun);
		}		
	}
}

COptionsData::OptionsData::DBServerData& CWebDataMiningApp::GetDBServerData(DBServerType nType/* =localServer */)
{
	COptionsData::OptionsData::DBServerData& data=m_data.m_data.m_localDatabase;
	switch(nType)
	{
	case localServer:
		data=m_data.m_data.m_localDatabase;
		break;
	case remoteServer:
		data= m_data.m_data.m_ServerDatabase;
		break;
	case wdmServer:
		data= m_data.m_data.m_wdmServerDatabase;
		break;
	default:
		ASSERT(FALSE);
	}
	return data;
}
/**********************************************************************************
 *  Function name: IsVisitedUrl
 *  Input argv:
 *  	-- url: url
 *  Output argv:
 *  	-- 
 *  Return:
   	true: is visited
   	false: not visited
 *  Function Description: test the url visited by the MD5
 *  Be careful:
**********************************************************************************/
BOOL CWebDataMiningApp::IsVisitedUrl(const CString& strURL)
{
	if( strURL.IsEmpty() ){
		return TRUE; // if be null, we think it have been visited
	}

	CMD5 iMD5;
	iMD5.GenerateMD5( (unsigned char*)(const char*)strURL, strURL.GetLength() );

	if( m_setVisitedUrlMD5.find(iMD5.ToString()) != m_setVisitedUrlMD5.end() )
	{
		return TRUE;
	} 
	else 
	{
		return FALSE;
	}
    return TRUE;
}
void CWebDataMiningApp::GetVisitedUrlMD5()
{
	CStdioFile file;
	if(file.Open(CCommonFunction::GetFileFullPath(URL_MD5_FILE.c_str()),CFile::modeRead))
	{
		CString str;
		while (file.ReadString(str))
		{
			std::string s(str);
			m_setVisitedUrlMD5.insert(s);
		}
		file.Close();
	}
	m_ofsVisitedUrlMD5File.open((const char*)CCommonFunction::GetFileFullPath(URL_MD5_FILE.c_str()), ios::out|ios::app|ios::binary);	
	if( !m_ofsVisitedUrlMD5File )
	{
	     AfxMessageBox(_T("打开链接地址缓存文件错误"));	
	}
}

void CWebDataMiningApp::SaveVisitedUrlMD5(const CString& strURL)
{
	if(strURL.IsEmpty())
		return;

	CMD5 md5;
	md5.GenerateMD5((unsigned char*)(const char*)strURL,strURL.GetLength());

	m_setVisitedUrlMD5.insert(md5.ToString());
	if( m_ofsVisitedUrlMD5File )
	{
		m_ofsVisitedUrlMD5File << md5.ToString().c_str() << endl;
	}
}
void CWebDataMiningApp::DeleteVisitedUrlMD5(const CString& strURL)
{
	if(strURL.IsEmpty())
		return;

	CMD5 md5;
	md5.GenerateMD5((unsigned char*)(const char*)strURL,strURL.GetLength());

	m_setVisitedUrlMD5.erase(md5.ToString());
}
void CWebDataMiningApp::SaveVisitedUrl()
{
	ofstream ofsVisitedUrlMD5File;
	ofsVisitedUrlMD5File.open((const char*)CCommonFunction::GetFileFullPath(URL_MD5_FILE.c_str()), ios::out|ios::app|ios::binary|ios::trunc);	
	if( !ofsVisitedUrlMD5File )
	{
		AfxMessageBox(_T("打开链接地址缓存文件错误败"));
		return ;
	}
	std::set<std::string>::iterator s,e;
	for(s=m_setVisitedUrlMD5.begin(),e=m_setVisitedUrlMD5.end();s!=e;s++)
	{
		ofsVisitedUrlMD5File << (*s).c_str() << endl;
	}
	ofsVisitedUrlMD5File.close();
}
void CWebDataMiningApp::OnFileShowRecentTopic() 
{
	// TODO: Add your command handler code here
	CRecentDownloadTopicDateDlg dlg;
    dlg.m_time=m_timeLoadTopic;
	
	if(dlg.DoModal()==IDOK)
	{
		SetLoadTopicDatetime(dlg.m_time);
		GetSourceTree()->InitUpdate(TRUE,TRUE);
	}
}

void CWebDataMiningApp::OnFileSendtoServer() 
{
	// TODO: Add your command handler code here
    SendTopicToServer(FALSE);
}

void CWebDataMiningApp::OnUpdateFileSendtoServer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetNetworkType()==0 && m_listDownloadTopics.GetSize()>0);
}
CString CWebDataMiningApp::GetFileSaveDir(DBServerType nType/* =localServer */)
{
	CString str;
	switch(nType)
	{
	case localServer:
		str=m_data.m_data.m_localDatabase.m_strResourceDir;
		break;
	case remoteServer:
		str=m_data.m_data.m_ServerDatabase.m_strResourceDir;
		break;
	case wdmServer:
	    str=m_data.m_data.m_wdmServerDatabase.m_strResourceDir;
		break;
	default:
		ASSERT(FALSE);
	}
    
	if(str.GetLength()>0 && str[str.GetLength()-1]=='\\' )
		str=str.Left(str.GetLength()-1);

	return str;
}

void CWebDataMiningApp::OnFileDelOldtopics() 
{
	// TODO: Add your command handler code here
	if(MessageBox(NULL,_T("删除所有文章吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
	{
		CWaitCursor wait;

		if(!DBIsOpen())
			ConnectDB();

		LPCSTR strSqls[] =
		{
			_T("delete from wdm_picture"),
			_T("delete from wdm_topic"),
		};
		for(int i=0;i<sizeof(strSqls)/sizeof(LPCSTR);i++)
		    GetDB()->Execute(strSqls[i]);

		CLogs::WriteLogs(_T("删除所有历史文章"),CLogs::log_system);
		//刷新
		GetSourceTree()->InitUpdate();
	}
}

void CWebDataMiningApp::OnFileClearOldurl() 
{
	// TODO: Add your command handler code here
	if(MessageBox(NULL,_T("清空URL历史记录吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
	{
		m_setVisitedUrlMD5.clear();
        
		if(m_ofsVisitedUrlMD5File)
			m_ofsVisitedUrlMD5File.close();

		DeleteFile(CCommonFunction::GetFileFullPath(URL_MD5_FILE.c_str()));
		CLogs::WriteLogs(_T("清空URL缓存"),CLogs::log_system);

		m_ofsVisitedUrlMD5File.open(CCommonFunction::GetFileFullPath(URL_MD5_FILE.c_str()), ios::out|ios::app|ios::binary);	
		if( !m_ofsVisitedUrlMD5File )
		{
			TRACE1("cannot open %s for output\n",URL_MD5_FILE.c_str() );
		}
	}
}

void CWebDataMiningApp::OnToolViewLogs() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;

    CLogsDlg dlg;
	dlg.DoModal();
}

void CWebDataMiningApp::OnToolViewUsers() 
{
	// TODO: Add your command handler code here
	CUserManageDlg dlg;
	dlg.DoModal();
}

void CWebDataMiningApp::OnUpdateToolViewUsers(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetUserLevel()==2);
}

void CWebDataMiningApp::OnUpdateToolViewLogs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetUserLevel()>0);
}

void CWebDataMiningApp::OnUpdateFileClearOldurl(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetUserLevel()>0);
}

void CWebDataMiningApp::OnUpdateFileDelOldtopics(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetUserLevel()>0);
}
CString CWebDataMiningApp::GetUserDescName(UserLevelType type/* =commonManager */)
{
	CString str=_T("");
	switch(type)
	{
	case superManager:
		str=_T("超级管理员");
		break;
	case systemManager:
		str=_T("系统管理员");
		break;
	case commonManager:
		str=_T("普通操作员");
		break;
	case unknowManager:
	    str=_T("未知用户");
		break;
	default:
		ASSERT(FALSE);
	}
	return str;
}

void CWebDataMiningApp::OnFileLogin() 
{
	// TODO: Add your command handler code here
	if(GetUserLevel()==-1)
		Login();
	else
	{
		if(MessageBox(NULL,_T("注销用户吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
		{
		   m_nCurUserLevel=unknowManager;
		   m_strUserName=_T("");
		   m_strUserPasswd=_T("");
		}
	}

}

void CWebDataMiningApp::OnUpdateFileLogin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(GetUserLevel()!=-1?_T("注销"):_T("登陆"));
}

void CWebDataMiningApp::OnUpdateToolOptions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetUserLevel()>=0 );
}
void CWebDataMiningApp::SendTopicToServer(BOOL bAuto /* =TRUE */)
{
	if(bAuto || MessageBox(NULL,_T("发送下载的文章到内网服务器吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
	{
		m_bAutoSendTopic=FALSE;

		CMainFrame* pMainFrm=(CMainFrame*)GetMainWnd();
        // 外网
		if(GetNetworkType()==0)
		{
			pMainFrm->UpdateOutputInfo(_T("断开外网连接，正在连接内网服务器..."));

			CCommonFunction::OpenInNet();
			//wait for changing to the Inner net
			do
			{
				Sleep(1000);
			}
			while( _access( theApp.GetFileSaveDir(CWebDataMiningApp::wdmServer),0 ) == -1 );
			// publish topic automaticly
			//					
			if(m_listAutoPublishTopics.GetSize()>0)
			{
				pMainFrm->UpdateOutputInfo(_T("正在自动发布文章..."));
				for(int i=0;i<m_listAutoPublishTopics.GetSize();i++)
				{
					CTopic* pTopic=m_listAutoPublishTopics.GetAt(i);
					pMainFrm->UpdateOutputInfo(_T("自动发布：")+pTopic->GetDesc());					
					pTopic->m_data.m_bIsPublish=pTopic->PublishToServer();
				}
				m_listAutoPublishTopics.RemoveAll();
				pMainFrm->UpdateOutputInfo(_T("自动发布文章完毕!"));
			}
		    //  Sending topic to wdm server
			pMainFrm->UpdateOutputInfo(_T("正在连接内网数据库..."));
			if(ConnectDB(wdmServer))
			{
				pMainFrm->UpdateOutputInfo(_T("连接内网数据库成功!"));
				pMainFrm->UpdateOutputInfo(_T("正在传送下载文章..."));
				for(int i=0;i<m_listDownloadTopics.GetSize();i++)
				{
					CTopic* pTopic=m_listDownloadTopics.GetAt(i);
					pMainFrm->UpdateOutputInfo(_T("正在传送：")+pTopic->GetDesc());
					pTopic->SaveToDB(*GetDB(wdmServer));
					//Set flag
					pTopic->m_data.m_bIsSendto=TRUE;
					pTopic->SaveToDB(*GetDB(),FALSE);
				}
				m_listDownloadTopics.RemoveAll();
				
				pMainFrm->UpdateOutputInfo(_T("传送下载文章成功!"));
				pMainFrm->UpdateOutputInfo(_T("关闭内网数据库连接"));
				CloseDB(wdmServer);	
				
				pMainFrm->UpdateOutputInfo(_T("断开内网服务器连接,连接外网"));
				CCommonFunction::OpenOutNet();					
			}
			
		}
		else   //内网
		{
		    // publish topic automaticly					
			if(bAuto && m_listAutoPublishTopics.GetSize()>0)
			{
				pMainFrm->UpdateOutputInfo(_T("正在自动发布文章..."));
				for(int i=0;i<m_listAutoPublishTopics.GetSize();i++)
				{
					CTopic* pTopic=m_listAutoPublishTopics.GetAt(i);
					pMainFrm->UpdateOutputInfo(_T("自动发布：")+pTopic->GetDesc());
					pTopic->m_data.m_bIsPublish=pTopic->PublishToServer();
					pTopic->SaveToDB(*theApp.GetDB(),FALSE);
				}
				m_listAutoPublishTopics.RemoveAll();
				pMainFrm->UpdateOutputInfo(_T("自动发布文章完毕!"));
			}
		}
		m_bAutoSendTopic=TRUE;
	}
}

void CWebDataMiningApp::OnFileAutoSend() 
{
	// TODO: Add your command handler code here
	m_bAutoSendTopic=!m_bAutoSendTopic;
}

void CWebDataMiningApp::OnUpdateFileAutoSend(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bAutoSendTopic?1:0);
	pCmdUI->Enable(GetNetworkType()==0);
}

void CWebDataMiningApp::OnFileShowTopcolumn() 
{
	// TODO: Add your command handler code here
	if(Login())
		GetSourceTree()->InitUpdate(TRUE,TRUE);
}

void CWebDataMiningApp::OnFileDownloadManual() 
{
	// TODO: Add your command handler code here
	CUrlDlg dlg;
	if(dlg.DoModal()==IDOK )
	{
		CStringArray arrayUrls;
		dlg.GetUrls(arrayUrls);	
 	
		if(arrayUrls.GetSize()>0)
		{	
		    thread_pool::instance().queue_request(new CWorkUrl(dlg.GetEntry(),dlg.GetRule(),arrayUrls));
 			//request_ptr request = request_ptr( std::auto_ptr<CWorkUrl>(new CWorkUrl(dlg.GetEntry(),dlg.GetRule(),arrayUrls)));
 			//thread_pool::instance().add_request(request);

		}	
	}
}

void CWebDataMiningApp::OnUpdateUserStatus(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(m_strUserName.IsEmpty()?_T("当前用户：未登陆"):_T("当前用户：")+m_strUserName);
    pCmdUI->Enable(TRUE);
}

void CWebDataMiningApp::OnToolSearch() 
{
	// TODO: Add your command handler code here
	CSearchDlg* pDlg=new CSearchDlg(m_listSearchText);
	pDlg->Create(IDD_SEARCH);
	pDlg->CenterWindow();
	pDlg->ShowWindow(SW_NORMAL);
}

void CWebDataMiningApp::InitServerColumn(CBaseObject* pParent/* =NULL */)
{
 	CString strSql;
	
	if( pParent )
	{		
		strSql.Format("select * from wdm_k_type where parent_id='%s'",pParent->GetID());
	}
	else
	{
		if( m_pServerColumnRoot != NULL )
		{
			delete m_pServerColumnRoot;
			m_pServerColumnRoot = NULL;
		}
		m_pServerColumnRoot=new CBaseObject(OBJECT_TYPE_SERVER_COLUMN);
		m_pServerColumnRoot->SetID(NULL_ID,_T("所有服务器栏目"));

		strSql.Format("select * from wdm_k_type where parent_id='%s'",NULL_ID);
	}
	
	CServerColumn* pServerColumn=NULL;
	CADORecordset rs(theApp.GetDB()); 
	
	if(rs.Open(strSql,CADORecordset::openQuery))
	{		
		while (!rs.IsEOF())
		{
			pServerColumn = new CServerColumn;
			pServerColumn->LoadInfo(rs);
            pServerColumn->AddTo(pParent?pParent:m_pServerColumnRoot);

			pServerColumn->LoadFromDB(*theApp.GetDB());

			if(pServerColumn->GetChildCount()>0)
				pServerColumn->SetImg(2,3);

			rs.MoveNext();
		}		
		rs.Close();
	}

}

void CWebDataMiningApp::OnUpdateFileDownloadManual(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CBaseObject* pEntry=theApp.GetSourceTree()->GetSelectedObject();
	pCmdUI->Enable(pEntry!=NULL && pEntry->GetType()==OBJECT_TYPE_ENTRY);
}

// void CWebDataMiningApp::OnStopAllDownload() 
// {
// 	// TODO: Add your command handler code here
// 	if(MessageBox(NULL,_T("停止所有下载任务吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
// 	{
// 		GetThreadPool().shutdown();
// 		GetThreadPool().initialize(50,100);
// 	}
// }
// 
// void CWebDataMiningApp::OnUpdateStopAllDownload(CCmdUI* pCmdUI) 
// {
// 	// TODO: Add your command update UI handler code here
// 	pCmdUI->Enable(GetThreadPool().get_current_working_count()!=0);
//}

void CWebDataMiningApp::OnToolImportServerColumn() 
{
	// TODO: Add your command handler code here
	CUpdateServerColumnDlg dlg;
	dlg.DoModal();
}
void CWebDataMiningApp::DeleteOldTopics()
{
	if (DBIsOpen())
	{
		CString strSql;
		COleDateTime curTime=COleDateTime::GetCurrentTime();
		COleDateTimeSpan spanTime(GetTopicSaveDays(),0,0,0);
		
		if(GetLocalDatabaseType()==0)
		{
		    strSql.Format("delete from wdm_picture where topic_id in \
			    (select topic_id from wdm_topic where download_date <'%s')",(curTime-spanTime).Format("%Y-%m-%d"));
		}
		else
		{
			strSql.Format("delete from wdm_picture where topic_id in \
			   (select topic_id from wdm_topic where download_date < datevalue('%s'))",(curTime-spanTime).Format("%Y-%m-%d"));
		}		
		
		GetDB()->Execute(strSql);
		
		if(GetLocalDatabaseType()==0)
		{
			strSql.Format("delete from wdm_topic where download_date <'%s'",(curTime-spanTime).Format("%Y-%m-%d"));
		}
		else
		{
			strSql.Format("delete from wdm_topic where download_date <datevalue('%s')",(curTime-spanTime).Format("%Y-%m-%d"));
		}
		
		GetDB()->Execute(strSql);
	}
}

void CWebDataMiningApp::OnToolBackupServerColumns() 
{
	// TODO: Add your command handler code here
	BOOL bRet=FALSE;
	CExportDlg dlg(EXPORT_XML);
	dlg.m_strFileDir=theApp.ExportDir();
	if(dlg.DoModal()==IDOK)
	{   
		CWaitCursor wait;
		CString strDir=dlg.m_strFileDir;
		theApp.ExportDir()=strDir;

		CProgressBar bar(_T("备份服务器栏目所有内容"),60,5000);
		TiXmlDocument doc;
		TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
		
		doc.LinkEndChild(pDecl);
		TiXmlNode* pNode=(TiXmlNode*)&doc;
		   
		TiXmlElement * pRoot= new TiXmlElement(_T("wdm_server_root"));
	    pRoot->SetAttribute(_T("name"),GetServerColumnRoot()->GetDesc());
	    pRoot->SetAttribute(_T("type"),GetServerColumnRoot()->GetType());
		
	    pNode->LinkEndChild(pRoot);

		GetServerColumnRoot()->StreamOut(pRoot,TRUE,&bar);

		CString strFile;
		strFile.Format("%s\\服务器栏目.xml",strDir);
		CCommonFunction::CreateDir(strFile);
		bRet=doc.SaveFile(strFile);	
		
		bar.Finish(_T("备份服务器栏目数据完毕"));
	}
	AfxMessageBox(bRet?_T("备份服务器栏目数据成功"):_T("备份服务器栏目数据失败"));
}

void CWebDataMiningApp::OnToolRebackupServerColumn() 
{
	// TODO: Add your command handler code here
	char szFilters[]="Xml Files (*.xml)|*.xml||";
	CString strFileName;
    BOOL bRet=TRUE;

	CFileDialog fileDlg(TRUE,"xml","服务器栏目.xml",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters);
	if( fileDlg.DoModal ()==IDOK )
	{
		CWaitCursor wait;
		strFileName= fileDlg.GetPathName();
		TiXmlDocument doc(strFileName);
		bRet&=(BOOL)doc.LoadFile();

		if(bRet)
		{
            TiXmlHandle hDoc(&doc);		    
            TiXmlElement* pRoot=NULL;

			CProgressBar bar(_T("恢复服务器栏目数据"),60,5000);
			pRoot=hDoc.FirstChildElement(_T("wdm_server_root")).ToElement();
		
			if(pRoot)
			{
				 int nType=-1;	
                 pRoot->QueryIntAttribute(_T("type"),&nType);
				 if(nType==OBJECT_TYPE_SERVER_COLUMN)
				 {					 
//					 GetServerColumnRoot()->DeleteFromDB(*GetDB(),TRUE,&bar);
					 GetDB()->Execute(_T("delete from wdm_k_type"));
					 InitServerColumn();

					 TiXmlElement* pElement = NULL;
					 CString strID;
					 for(pElement=pRoot->FirstChildElement(_T("wdm_server_column"));pElement;pElement=pElement->NextSiblingElement())
					 {
						 strID=pElement->Attribute(_T("type_id"));
						 if(!strID.IsEmpty())
						 {
							 CServerColumn* pServerColumn = new CServerColumn;
							 pServerColumn->m_data.m_strID=strID; 	
							 pServerColumn->m_data.m_strParentID=pElement->Attribute(_T("parent_id"));
							 pServerColumn->m_data.m_strColumnName=pElement->Attribute(_T("column_name"));
							 
							 pServerColumn->SetID(strID,pServerColumn->m_data.m_strColumnName);

							 pServerColumn->AddTo(GetServerColumnRoot());				
							 pServerColumn->SaveToDB(*GetDB(),FALSE,&bar);
							 
							 pServerColumn->StreamIn(pElement,TRUE,&bar);
							 
							 if(pServerColumn->GetChildCount()>0)
								 pServerColumn->SetImg(2,3);
						 }
					 }			         
				 }
				 else
				 {
					AfxMessageBox(_T("数据格式不匹配"));
					bRet=FALSE;
				 }			
			}
			bar.Finish(_T("恢复服务器栏目数据完毕"));
		}
	}
	else
	{
		bRet=FALSE;
	}

	AfxMessageBox(bRet?_T("恢复服务器栏目数据成功"):_T("恢复服务器栏目数据失败"));
}
