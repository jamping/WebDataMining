// ModifyUserPassDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CModifyUserPassDlg dialog

class CModifyUserPassDlg : public CDialogEx
{
// Construction
public:
	CModifyUserPassDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModifyUserPassDlg)
	enum { IDD = IDD_MODIFY_USERPASS };
	CString	m_strNewPass;
	CString	m_strOldPass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyUserPassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyUserPassDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};