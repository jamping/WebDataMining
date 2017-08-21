// InfoDlg.h : header file
//
#pragma once
/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog
#include "SetReadOnlyDialog.h"

class CInfoDlg : public CSetReadOnlyDialog
{
// Construction
public:
	CInfoDlg(BOOL bReadOnly=FALSE,CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CInfoDlg)
	enum { IDD = IDD_INFO_DLG };
	CString	m_strName;
	CString	m_strTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};