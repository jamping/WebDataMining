// LogDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

class CLogDlg : public CDialogEx
{
// Construction
public:
	CLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_LOGDLG };
	CString	m_strDBServer;
	CString	m_strPass;
	BOOL	m_bSavePass;
	CString	m_strUser;
	CString	m_strDatabase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};