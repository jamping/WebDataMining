// SelContentDlg.h : header file
//
#pragma once

#include "BaseObject.h"
/////////////////////////////////////////////////////////////////////////////
// CSelContentDlg dialog

class CSelContentDlg : public CDialogEx
{
// Construction
public:
	CSelContentDlg(CBaseObject*   pObject,CWnd* pParent = NULL);   // standard constructor
    
// Dialog Data
	//{{AFX_DATA(CSelContentDlg)
	enum { IDD = IDD_SEL_CONTENT };
	CComboBox	m_ctlContent;
	CString	m_strPrompt;
	CString	m_strSelContent;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelContentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CBaseObject*    m_pOb;
	// Generated message map functions
	//{{AFX_MSG(CSelContentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownContent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};