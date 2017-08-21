// DBServerDlg.h : header file
//
#pragma once

#include "DBServer.h"
#include "SetReadOnlyDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDBServerDlg dialog

class CDBServerDlg : public CSetReadOnlyDialog
{
// Construction
public:
	CDBServerDlg(BOOL bReadOnly=FALSE,CWnd* pParent = NULL);   // standard constructor

	void SetValue(CDBServer::DBServerData& data);
	BOOL GetValue(CDBServer::DBServerData& data);
// Dialog Data
	//{{AFX_DATA(CDBServerDlg)
	enum { IDD = IDD_DB_SERVER_DLG };
	CComboBox	m_ctlDBServer;
	CString	m_strDBServer;
	CString	m_strResourceDir;
	CString	m_strPass;
	CString	m_strUser;
	int		m_nDataBaseType;
	CString m_strResourcePath;
	int     m_nServerPort;
	CString m_strServerCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBServerDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDBServerDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnButtonFilesave();
	afx_msg void OnDropdownDbserver();
	//}}AFX_MSG
	afx_msg void OnUpdateServerPort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServerCode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonTest(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDirPath(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileServerPort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThumbName(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeFileSaveDir();
	afx_msg void OnBnClickedDatabaseType();
	CString m_strFileServerName;
	CString m_strFileServerPassWord;
	int m_nFileServerPort;
	CString m_strFileServerUserName;
	int m_nFileSendType;
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedCheckAnonymous();
	BOOL m_bAnonymous;
	BOOL m_bThumb;
	CString m_strThumbName;
	afx_msg void OnClickedCheckThumb();
};