// LogsDlg.h : header file
//
#pragma once

#include "SortListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CLogsDlg dialog
const int NUM_PER_PAGE=25;

class CLogsDlg : public CDialogEx
{
// Construction
public:
	CLogsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogsDlg)
	enum { IDD = IDD_LOGS };
	CSortListCtrl	m_listLogs;
	CString	m_strPageNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void ShowContent();

	int m_nTotalCounts;
	int m_nTotalPages;
	int m_nCurPage;

	// Generated message map functions
	//{{AFX_MSG(CLogsDlg)
	afx_msg void OnClearLogs();
	virtual BOOL OnInitDialog();
	afx_msg void OnFirstPage();
	afx_msg void OnPrevPage();
	afx_msg void OnNextPage();
	afx_msg void OnLastPage();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnUpdateClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFirstPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePrevPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNextPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLastPage(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};