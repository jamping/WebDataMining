// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ExtractSourceBar.h"
#include "ExtractRuleBar.h"
#include "ScheduleBar.h"
#include "dbserverbar.h"
#include "OutputBar.h"
#include "TrayIcon1.h"

#define TRAY_ICON_NOTIFY WM_USER+500

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
// protected: // create from serialization only
// 	CMainFrame();
// 	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void UpdateOutputInfo(CString strTitle,CString strContent=_T(""));

    HWND GetOutPutWindowHWND();
	void ShowOutputWindow(BOOL bShow=TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCStatusBar  m_wndStatusBar;

	CExtractSourceBar	m_wndExtractSource;
	CExtractRuleBar	m_wndExtractRule;
	CScheduleBar  m_wndSchedule;
	CDBServerBar  m_wndDBServer;
	COutputBar	    m_wndOutput;

	CTrayIcon    m_trayIcon;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnHelpTopic();
	afx_msg void OnSendtopicServer();
	//}}AFX_MSG
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewExtractSource();
	afx_msg void OnUpdateViewExtractSource(CCmdUI* pCmdUI);
	afx_msg void OnViewExtractRule();
	afx_msg void OnUpdateViewExtractRule(CCmdUI* pCmdUI);
	afx_msg void OnViewSchedule();
	afx_msg void OnUpdateViewSchedule(CCmdUI* pCmdUI);
	afx_msg void OnViewDBServer();
	afx_msg void OnUpdateViewDBServer(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
    afx_msg LRESULT OnTrayIconNotify(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};
