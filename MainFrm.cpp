// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WebDataMining.h"

#include "MainFrm.h"
#include "RegexWDM.h"

#include "ScheduleDlg.h"
#include "DownloadHttpFile.h"
//////////////////////////////////////////////////////////////////////////
#include "TopicPublishDlg.h"
#include "ListViewEx.h"
//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_TOPIC, OnHelpTopic)
	ON_COMMAND(ID_SENDTOPIC_SERVER, OnSendtopicServer)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
// 	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
// 	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)
	ON_COMMAND(ID_VIEW_EXTRACT_SOURCE, OnViewExtractSource)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXTRACT_SOURCE, OnUpdateViewExtractSource)
	ON_COMMAND(ID_VIEW_EXTRACT_RULE, OnViewExtractRule)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXTRACT_RULE, OnUpdateViewExtractRule)
	ON_COMMAND(ID_VIEW_SCHEDULE, OnViewSchedule)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCHEDULE, OnUpdateViewSchedule)
	ON_COMMAND(ID_VIEW_DBSERVER, OnViewDBServer)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DBSERVER, OnUpdateViewDBServer)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)// 
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_MESSAGE(TRAY_ICON_NOTIFY,OnTrayIconNotify)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_USER_STATUS,
	ID_INDICATOR_PICTURE_DIR,
	ID_INDICATOR_WORK_THREAD_NUM,
	ID_INDICATOR_NETWORK_TYPE,
	ID_INDICATOR_DB_STATUS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	::SetProp(m_hWnd, AfxGetApp()->m_pszAppName, (HANDLE)1);

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

// 	CMDITabInfo mdiTabParams;
// 	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
// 	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
// 	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
// 	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
// 	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
// 	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

// 	if (!m_wndStatusBar.Create(this))
// 	{
// 		TRACE0("未能创建状态栏\n");
// 		return -1;      // 未能创建
// 	}

// 	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_INDICATOR_MESSAGE, _T(""), TRUE), _T(""));
// 	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_INDICATOR_USER_STATUS, _T("用户"), TRUE), _T("用户"));
// 	m_wndStatusBar.AddSeparator();
//     m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_INDICATOR_NETWORK_TYPE, _T("网络"), TRUE), _T("网络"));
// 	m_wndStatusBar.AddSeparator();
//     m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_INDICATOR_DB_STATUS, _T("数据库"), TRUE), _T("数据库"));
// 	m_wndStatusBar.AddSeparator();
//     m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_INDICATOR_WORK_THREAD_NUM, _T("工作线程"), TRUE), _T("工作线程"));
// 	m_wndStatusBar.SetInformation(NULL);
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndExtractSource.EnableDocking(CBRS_ALIGN_ANY);
	m_wndExtractRule.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSchedule.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDBServer.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndExtractSource);
	CDockablePane* pTabbedBar = NULL;
	m_wndExtractRule.AttachToTabWnd(&m_wndExtractSource, DM_SHOW, TRUE, &pTabbedBar);
    m_wndSchedule.AttachToTabWnd(&m_wndExtractSource, DM_SHOW, TRUE, &pTabbedBar);
    m_wndDBServer.AttachToTabWnd(&m_wndExtractSource, DM_SHOW, TRUE, &pTabbedBar);
	
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	m_trayIcon.Create(this,TRAY_ICON_NOTIFY,_T("新闻采集系统3.0"),
		LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1)),IDR_POPMEMU);

	// 启用增强的窗口管理对话框
//	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);	

	return 0;

	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

// 	CList<UINT, UINT>	lstBasicCommands;
// 
// 	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
// 	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
// 		
// 	lstBasicCommands.AddTail (ID_FILE_CONNECT_DB);
// 	lstBasicCommands.AddTail (ID_FILE_LOGIN);
// 	lstBasicCommands.AddTail (ID_FILE_SHOW_TOPCOLUMN);	
// 	lstBasicCommands.AddTail (ID_FILE_SHOW_RECENT_TOPIC);
// 	lstBasicCommands.AddTail (ID_FILE_CLEAR_OLDURL);
// 	lstBasicCommands.AddTail (ID_FILE_DEL_OLDTOPICS);
// 	lstBasicCommands.AddTail (ID_TOOL_VIEW_USERS);
// 	lstBasicCommands.AddTail (ID_TOOL_VIEW_LOGS);
// 	lstBasicCommands.AddTail (ID_TOOL_OPTIONS);
// 	lstBasicCommands.AddTail (ID_FILE_SENDTO_SERVER);
// 	lstBasicCommands.AddTail (ID_FILE_AUTO_SEND);
// 	lstBasicCommands.AddTail (ID_SENDTOPIC_SERVER);
// 	lstBasicCommands.AddTail (ID_FILE_DOWNLOAD_MANUAL);
// 	lstBasicCommands.AddTail (ID_APP_ABOUT);
// 
// 	CMFCToolBar::SetBasicCommands (lstBasicCommands);
// 
// 	if (!m_wndMenuBar.Create (this,dwDefaultToolbarStyle,IDR_MAINFRAME))
// 	{
// 		TRACE0("Failed to create menubar\n");
// 		return -1;      // fail to create
// 	}
// 
// 	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);
// 
// 	// Detect color depth. 256 color toolbars can be used in the
// 	// high or true color modes only (bits per pixel is > 8):
// 	CClientDC dc (this);
// 	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;
// 
// 	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;
// 	UINT uiToolbarColdID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;
// 
// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
// 		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, uiToolbarColdID, 0, FALSE, 0, 0, uiToolbarHotID))
// 	{
// 		TRACE0("Failed to create toolbar\n");
// 		return -1;      // fail to create
// 	}
// 	if (!m_wndStatusBar.Create(this) ||
// 		!m_wndStatusBar.SetIndicators(indicators,
// 		  sizeof(indicators)/sizeof(UINT)))
// 	{
// 		TRACE0("Failed to create status bar\n");
// 		return -1;      // fail to create
// 	}

// 	CImageList imagesWorkspace;
// 	imagesWorkspace.Create (IDB_WORKSPACE, 16, 0, RGB (255, 0, 255));
// 	
// 	if (!m_wndWorkSpace.Create (_T("采集源"), this, CRect (0, 0, 200, 200),
// 		TRUE, ID_VIEW_WORKSPACE,
// 		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 	{
// 		TRACE0("Failed to create Workspace bar\n");
// 		return FALSE;      // fail to create
// 	}
// 
// 	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);
// 
// 	if (!m_wndWorkSpace2.Create (_T("采集规则"), this, CRect (0, 0, 200, 200),
// 		TRUE, ID_VIEW_WORKSPACE2,
// 		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 	{
// 		TRACE0("Failed to create Workspace bar 2\n");
// 		return FALSE;      // fail to create
// 	}
// 
// 	m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);
// 
// 	if (!m_wndWorkSpace3.Create (_T("调度计划"), this, CRect (0, 0, 200, 200),
// 		TRUE, ID_VIEW_WORKSPACE3,
// 		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 	{
// 		TRACE0("Failed to create Workspace bar 3\n");
// 		return FALSE;      // fail to create
// 	}
// 
// 	m_wndWorkSpace3.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);
// 	
// 	if (!m_wndWorkSpace4.Create (_T("服务器"), this, CRect (0, 0, 200, 200),
// 		TRUE, ID_VIEW_WORKSPACE4,
// 		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 	{
// 		TRACE0("Failed to create Workspace bar 4\n");
// 		return FALSE;      // fail to create
// 	}
// 
// 	m_wndWorkSpace4.SetIcon (imagesWorkspace.ExtractIcon (3), FALSE);
// 
// 	if (!m_wndOutput.Create (_T("输出信息"), this, CRect (0, 0, 150, 150),
// 			TRUE /* Has gripper */, ID_VIEW_OUTPUT,
// 			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM))		
// 	{
// 		TRACE0("Failed to create output bar\n");
// 		return -1;      // fail to create		
// 	}
// 
// 	CString strMainToolbarTitle;
// 	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
// 	m_wndToolBar.SetWindowText (strMainToolbarTitle);
//     m_wndToolBar.EnableTextLabels();
// 	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("定制..."));
// 
// 	// TODO: Delete these three lines if you don't want the toolbar to
// 	//  be dockable
// 	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndWorkSpace3.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndWorkSpace4.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
// 
// 	EnableDocking(CBRS_ALIGN_ANY);
// 
// 	if (!m_wndCaptionBar.Create (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this,
// 		ID_VIEW_CAPTIONBAR, -1))
// 	{
// 		TRACE0("Failed to create caption bar\n");
// 		return -1;      // fail to create
// 	}
// 
// 	m_wndCaptionBar.SetFlatBorder ();
// 	m_wndCaptionBar.SetText (_T("<Put your caption here>"));
// 	m_bmpCaption.LoadBitmap (IDB_CAPTION);
// 	m_wndCaptionBar.SetBitmap (m_bmpCaption, RGB (255, 0, 255));
// 
// 	EnableAutoHideBars(CBRS_ALIGN_ANY);
// 	DockControlBar(&m_wndMenuBar);
// 	DockControlBar(&m_wndToolBar);
// 	DockControlBar (&m_wndWorkSpace);
// 
// 	CBCGPDockingControlBar* pTabbedBar = NULL; 
// 	m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_SHOW, TRUE, &pTabbedBar);
// 	m_wndWorkSpace3.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_SHOW, TRUE, &pTabbedBar);
// 	m_wndWorkSpace4.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_SHOW, TRUE, &pTabbedBar);
//  	DockControlBar(&m_wndOutput);
// 
// 	// Allow user-defined toolbars operations:
// 	InitUserToobars (NULL,
// 					uiFirstUserToolBarId,
// 					uiLastUserToolBarId);
 
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{       
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
// 	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
// 		TRUE /* Automatic menus scaning */
// // 		 ,(	BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
// // 			BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS,BCGCUSTOMIZE_SELECT_SKINS)
//          );
// 
// 	pDlgCust->EnableUserDefinedToolbars ();
// 	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
// 	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));
// 
// 	CMenu menu;
// 	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));
// 
// 	CMenu* pPopup = menu.GetSubMenu(0);
// 	ASSERT(pPopup != NULL);
// 
// 	SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
// 
// 	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
// //	pPopupMenu->SetAutoDestroy (FALSE);
// 	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

void CMainFrame::OnToolsViewUserToolbar (UINT uiId)
{
// 	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
// 	if (pUserToolBar == NULL)
// 	{
// 		ASSERT (FALSE);
// 		return;
// 	}
// 
// 	ShowControlBar (pUserToolBar, !(pUserToolBar->IsVisible ()), FALSE, TRUE);
// 	RecalcLayout ();
}

void CMainFrame::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
// 	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
// 	if (pUserToolBar == NULL)
// 	{
// 		pCmdUI->Enable (FALSE);
// 		return;
// 	}
// 
// 	pCmdUI->Enable ();
// 	pCmdUI->SetCheck (pUserToolBar->IsVisible ());
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

// BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
// {
// 	//---------------------------------------------------------
// 	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
// 	//---------------------------------------------------------
//      CMDIFrameWndEx::OnShowPopupMenu (pMenuPopup);
// 
//     if (pMenuPopup != NULL &&
// 		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
//     {
// 		if (CBCGPToolBar::IsCustomizeMode ())
// 		{
// 			//----------------------------------------------------
// 			// Don't show toolbars list in the cuztomization mode!
// 			//----------------------------------------------------
// 			return FALSE;
// 		}
// 
// 		pMenuPopup->RemoveAllItems ();
// 
// 		CMenu menu;
// 		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));
// 
// 		CMenu* pPopup = menu.GetSubMenu(0);
// 		ASSERT(pPopup != NULL);
// 
// 		SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
// 		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
//     }
// 
// 	return TRUE;
// }

void CMainFrame::OnViewExtractSource() 
{
	ShowPane(&m_wndExtractSource,
					!(m_wndExtractSource.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewExtractSource(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndExtractSource.IsVisible ());
}

void CMainFrame::OnViewExtractRule() 
{
	ShowPane (&m_wndExtractRule,
					!(m_wndExtractRule.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewExtractRule(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndExtractRule.IsVisible ());
}
void CMainFrame::OnViewSchedule() 
{
	ShowPane (&m_wndSchedule,
					!(m_wndSchedule.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewSchedule(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndSchedule.IsVisible ());
}
void CMainFrame::OnViewDBServer() 
{
	ShowPane (&m_wndDBServer,
					!(m_wndDBServer.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewDBServer(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndDBServer.IsVisible ());
}
void CMainFrame::OnViewOutput() 
{
	ShowPane (&m_wndOutput,
					!(m_wndOutput.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
}


void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if (pWnd->GetSafeHwnd () == m_wndClientArea.GetSafeHwnd ())
	{
		OnToolbarContextMenu (0, MAKELPARAM(point.x, point.y));
	}
	else if (pWnd->GetSafeHwnd () == m_wndClientArea.GetMDITabs ().GetSafeHwnd ())
	{
		CMFCTabCtrl& wndTab = (CMFCTabCtrl&) m_wndClientArea.GetMDITabs ();

		CRect rectTabs;
		wndTab.GetTabsRect (rectTabs);

		CPoint ptTab = point;
		wndTab.ScreenToClient (&ptTab);

		const int nTab = wndTab.GetTabFromPoint (ptTab);

		if (nTab >= 0)
		{
			wndTab.SetActiveTab (nTab);
			theApp.ShowPopupMenu (IDR_POPUP_MDITABS, point, this);
		}
	}
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// Add some tools for example....
	CUserToolsManager* pUserToolsManager = theApp.GetUserToolsManager ();
	if (pUserToolsManager != NULL &&
		pUserToolsManager->GetUserTools ().IsEmpty ())
	{
		CUserTool* pTool1 = pUserToolsManager->CreateNewTool ();
		pTool1->m_strLabel = _T("Notepad");
		pTool1->SetCommand (_T("notepad.exe"));

		CUserTool* pTool2 = pUserToolsManager->CreateNewTool ();
		pTool2->m_strLabel = _T("Paint Brush");
		pTool2->SetCommand (_T("mspaint.exe"));

		CUserTool* pTool3 = pUserToolsManager->CreateNewTool ();
		pTool3->m_strLabel = _T("Windows Explorer");
		pTool3->SetCommand (_T("explorer.exe"));

		CUserTool* pTool4 = pUserToolsManager->CreateNewTool ();
		pTool4->m_strLabel = _T("海军政工网在线");
		pTool4->SetCommand (_T("http://www.zzb.hj"));
	}

	//----------------------------------------------------
	// Enable customization button fore all user toolbars:
	//----------------------------------------------------
	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("定制..."));
		}
	}


	return TRUE;
}


LRESULT CMainFrame::OnTrayIconNotify(WPARAM wParam,LPARAM lParam)
{
 	return m_trayIcon.OnTrayNotification(wParam,lParam);
}
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nID)
	{
    case SC_MINIMIZE: 
		if(theApp.IsMiniToTray())
		{
           ShowWindow(SW_HIDE);
		   return;
		}
		break;
	case SC_CLOSE:
		if(theApp.IsCloseToTray())
		{
		    ShowWindow(SW_HIDE);
		    return;
		}
    	break;	
    }
	CMDIFrameWndEx::OnSysCommand(nID, lParam);
}


void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(MessageBox("退出新闻采集系统吗?","请确认",MB_YESNO |MB_ICONWARNING)==IDNO)
		return;
	
	CMDIFrameWndEx::OnClose();
}
HWND CMainFrame::GetOutPutWindowHWND( )
{
	return m_wndOutput.GetSafeHwnd();
}
void CMainFrame::ShowOutputWindow(BOOL bShow/* =TRUE */)
{
	ShowPane(&m_wndOutput,bShow,FALSE, TRUE);
	RecalcLayout ();
}
void CMainFrame::UpdateOutputInfo(CString strTitle,CString strContent/* =_T */)
{
	m_wndOutput.UpdateTextInfo(strTitle,strContent);
}

void CMainFrame::OnHelpTopic() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL,_T("open"),CCommonFunction::GetFileFullPath(_T("wdm.chm")),NULL,NULL,SW_SHOWNORMAL);
}

void CMainFrame::OnSendtopicServer() 
{
	// TODO: Add your command handler code here
	CTopicPublishDlg dlg;
  	dlg.DoModal();	
}
void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
}
BOOL CMainFrame::CreateDockingWindows()
{
	//创建采集源视图	
	if (!m_wndExtractSource.Create(_T("采集源"),this,CRect(0, 0, 200, 200), TRUE, ID_VIEW_EXTRACT_SOURCE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“采集源视图”窗口\n");
		return FALSE; //未能创建
	}

	//创建采集规则视图	
	if (!m_wndExtractRule.Create(_T("采集规则"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_EXTRACT_RULE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“采集规则视图”窗口\n");
		return FALSE; //未能创建
	}
	//创建调度计划视图	
	if (!m_wndSchedule.Create(_T("调度计划"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SCHEDULE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“调度计划视图”窗口\n");
		return FALSE; // 未能创建
	}
	// 创建服务器视图	
	if (!m_wndDBServer.Create(_T("服务器"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_DBSERVER,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“服务器视图”窗口\n");
		return FALSE; // 未能创建
	}
	// 创建输出窗口
	
	if (!m_wndOutput.Create(_T("输出"), this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}	

	SetDockingWindowIcons(theApp.m_bHiColorIcons);

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hExtractSourcrIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_EXTRACT_SOURCE_VIEW_HC : IDI_EXTRACT_SOURCE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndExtractSource.SetIcon(hExtractSourcrIcon, FALSE);

	HICON hExtractRuleIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_EXTRACT_RULE_VIEW_HC : IDI_EXTRACT_RULE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndExtractRule.SetIcon(hExtractRuleIcon, FALSE);

	HICON hScheduleIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_SCHEDULE_VIEW_HC : IDI_SCHEDULE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSchedule.SetIcon(hScheduleIcon, FALSE);

	HICON hDBServerIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_DBSERVER_VIEW_HC : IDI_DBSERVER_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndDBServer.SetIcon(hDBServerIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);	

	UpdateMDITabbedBarsIcons();
}