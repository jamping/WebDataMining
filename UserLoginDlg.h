// UserLoginDlg.h : header file
//
#pragma once

#include "ExpandingDialog.h"
#include "TopicListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog

class CUserLoginDlg : public CExpandingDialog
{
// Construction
public:
	CUserLoginDlg(BOOL bAutoClose=FALSE,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserLoginDlg)
	enum { IDD = IDD_USER_LOGIN };
	CTopicListCtrl	m_listTopColumn;
	int		m_nUserLevel;
	BOOL	m_bSavePasswd;
	CString	m_strUserName;
	CString	m_strUserPasswd;
	BOOL	m_bIsAutoSendTopic;
	//}}AFX_DATA
    BOOL    m_bIsExpanded;
	CString m_strUserDesc;
// Overrides
	virtual BOOL OnDialogExpanding(BOOL bExpanded);
	virtual void OnDialogExpanded(BOOL bExpanded);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    BOOL m_bAutoClose;
    int  m_nDelay;
    BOOL m_bIsSelAll;

	void ShowContent();
	void ClearMemory();
	BOOL IsSelAll();
	// Generated message map functions
	//{{AFX_MSG(CUserLoginDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnAdvanced();
	afx_msg void OnSelall();
	afx_msg void OnConfig();
	//}}AFX_MSG
	afx_msg void OnUpdateSelall(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRepair();
};