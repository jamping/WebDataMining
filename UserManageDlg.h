// UserManageDlg.h : header file
//
#pragma once

#include "SortListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CUserManageDlg dialog

class CUserManageDlg : public CDialogEx
{
// Construction
public:
	CUserManageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserManageDlg)
	enum { IDD = IDD_USER_MANAGE };
	CComboBox	m_comboUserLevel;
	CSortListCtrl	m_listUsers;
	CString	m_strUserDesc;
	CString	m_strUserName;
	CString	m_strUserPasswd;
	int		m_nUserLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserManageDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ClearCtrlText();
    BOOL IsModified();
	
    int m_nCurSelectedItem;
	CStringList m_listUserName;
	// Generated message map functions
	//{{AFX_MSG(CUserManageDlg)
	afx_msg void OnAddUser();
	afx_msg void OnClearUsers();
	afx_msg void OnDelUser();
	afx_msg void OnModifyUser();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedListUsers(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnUpdateAddUser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearUser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelUser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModifyUser(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};