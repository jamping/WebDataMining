// ServerTableDlg.h : header file
//
#pragma once

#include "WDMTable.h"
/////////////////////////////////////////////////////////////////////////////
// CServerTableDlg dialog

class CServerTableDlg : public CDialogEx
{
// Construction
public:
	CServerTableDlg(CBaseObject*   pObject,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerTableDlg)
	enum { IDD = IDD_SERVER_TABLE };
	CComboBox	m_ctlServerTable;
	int		m_nType;
	CString	m_strServerTable;
	int     m_nProcessOrder;
	BOOL    m_bPublishRepeat;
	//}}AFX_DATA
    BOOL GetValue(CWDMTable::WDMTableData& data);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CBaseObject*    m_pOb;
	// Generated message map functions
	//{{AFX_MSG(CServerTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownContent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
};