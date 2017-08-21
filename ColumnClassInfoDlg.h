// ColumnClassInfoDlg.h : header file
//
#pragma once

#include "customtree.h"
/////////////////////////////////////////////////////////////////////////////
// CColumnClassInfoDlg dialog

class CColumnClassInfoDlg : public CDialogEx
{
// Construction
public:
	CColumnClassInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColumnClassInfoDlg)
	enum { IDD = IDD_COLUMNCLASS_INFO };
	CCustomTree	m_tree;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnClassInfoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:	
	// Generated message map functions
	//{{AFX_MSG(CColumnClassInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	//}}AFX_MSG
	afx_msg void OnUpdateOnApply(CCmdUI* pComUUI);

	DECLARE_MESSAGE_MAP()
};