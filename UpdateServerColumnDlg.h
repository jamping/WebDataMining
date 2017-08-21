// UpdateServerColumnDlg.h : header file
//
#pragma once
#include "customtree.h"
#include "sortlistctrl.h"
#include "GradientProgressCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CUpdateServerColumnDlg dialog

class CUpdateServerColumnDlg : public CDialogEx
{
// Construction
public:
	CUpdateServerColumnDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUpdateServerColumnDlg)
	enum { IDD = IDD_IMPORT_SERVER_COLUMN };
	CGradientProgressCtrl	m_progress;
	CSortListCtrl	m_listContent;
	CListBox	m_listSelectingField;
	CListBox	m_listSelectedField;
	CSortListCtrl	m_listFieldMap;
	CComboBox	m_comboTable;
	CComboBox	m_comboDB;
	CString     m_strRootValue;
	CCustomTree	m_tree;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateServerColumnDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void Init();
	void InitMapList();
	CString GetColumnString();
    void ShowContent();
    BOOL IsUpdate();

	CWDMDatabase*  m_pDB;
	CString        m_strTableName;
	// Generated message map functions
	//{{AFX_MSG(CUpdateServerColumnDlg)
	afx_msg void OnUpdate();
	afx_msg void OnDel();
	afx_msg void OnAdd();
	afx_msg void OnSelchangeComboDatabase();
	afx_msg void OnSelchangeComboTable();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListSelectingField();
	afx_msg void OnDblclkListSelectedField();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnLoadComboboxData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUpdate(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};