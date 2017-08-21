// ServerColumnDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CServerColumnDlg dialog

class CServerColumnDlg : public CDialogEx
{
// Construction
public:
	CServerColumnDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerColumnDlg)
	enum { IDD = IDD_SERVER_COLUMN };
	CString	m_strColumnID;
	CString	m_strColumnName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerColumnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerColumnDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};