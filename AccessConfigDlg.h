#if !defined(AFX_ACCESSCONFIGDLG_H__F120A59A_E4B8_4DDA_A164_0776094E1AD9__INCLUDED_)
#define AFX_ACCESSCONFIGDLG_H__F120A59A_E4B8_4DDA_A164_0776094E1AD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccessConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccessConfigDlg dialog

class CAccessConfigDlg : public CDialog
{
// Construction
public:
	CAccessConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccessConfigDlg)
	enum { IDD = IDD_ACCESS_CONFIG };
	CString	m_strDatabase;
	CString	m_strPass;
	BOOL	m_bSavePasswd;
	CString	m_strUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccessConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccessConfigDlg)
	afx_msg void OnSelDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCESSCONFIGDLG_H__F120A59A_E4B8_4DDA_A164_0776094E1AD9__INCLUDED_)
