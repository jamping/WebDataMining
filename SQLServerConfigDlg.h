#if !defined(AFX_SQLSERVERCONFIGDLG_H__9AAE0782_F6D9_4111_B8C0_968816E98912__INCLUDED_)
#define AFX_SQLSERVERCONFIGDLG_H__9AAE0782_F6D9_4111_B8C0_968816E98912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SQLServerConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSQLServerConfigDlg dialog

class CSQLServerConfigDlg : public CDialog
{
// Construction
public:
	CSQLServerConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSQLServerConfigDlg)
	enum { IDD = IDD_SQLSERVER_CONFIG };
	CComboBox	m_ctlDBServer;
	CComboBox	m_ctlDatabase;
	CString	m_strPass;
	BOOL	m_bSavePasswd;
	CString	m_strUser;
	CString	m_strDatabase;
	CString	m_strServer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSQLServerConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSQLServerConfigDlg)
	afx_msg void OnDropdownDbserver();
	afx_msg void OnDropdownDatabase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQLSERVERCONFIGDLG_H__9AAE0782_F6D9_4111_B8C0_968816E98912__INCLUDED_)
