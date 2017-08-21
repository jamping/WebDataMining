// ViewDataTableDlg.h : header file
//
#pragma once

#include "sortlistctrl.h"
#include "Entry.h"
/////////////////////////////////////////////////////////////////////////////
// CViewDataTableDlg dialog

class CViewDataTableDlg : public CDialogEx
{
// Construction
public:
	CViewDataTableDlg(CString strEntryID,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewDataTableDlg)
	enum { IDD = IDD_VIEW_DATATABLE };
	CListBox	m_listSelectingFields;
	CListBox	m_lisSelectedFields;
	CSortListCtrl	m_listTableContent;
	CComboBox	m_ctlDataTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDataTableDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
    CEntry*   m_pEntry;

	CString GetColumnString();
	CWDMDatabase* GetDatabase();
    void ShowContent();
	// Generated message map functions
	//{{AFX_MSG(CViewDataTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnDropdownComboTable();
	afx_msg void OnSelchangeComboTable();
	afx_msg void OnDblclkListSelectedField();
	afx_msg void OnDblclkListSelectingField();
	//}}AFX_MSG
	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDel(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};