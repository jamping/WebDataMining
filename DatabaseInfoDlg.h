// DatabaseInfoDlg.h : header file
//
#pragma once

#include "dbservertree.h"
/////////////////////////////////////////////////////////////////////////////
// CDatabaseInfoDlg dialog

class CDatabaseInfoDlg : public CDialogEx
{
// Construction
public:
	CDatabaseInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDatabaseInfoDlg)
	enum { IDD = IDD_DATABASE_INFO };
	
	//}}AFX_DATA
    CString GetDatabaseID() { return m_strDatabaseID; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatabaseInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBServerTree	m_tree;
    CString         m_strDatabaseID;   

	// Generated message map functions
	//{{AFX_MSG(CDatabaseInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};