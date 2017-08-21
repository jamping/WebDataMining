// SearchDlg.h : header file
//
#pragma once

#include <queue>
/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog

class CSearchDlg : public CDialogEx
{
// Construction
public:
	CSearchDlg(CStringArray& listTarget,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSearchDlg)
	enum { IDD = IDD_SEARCH };
	CComboBox	m_cbTarget;
	BOOL	m_bAll;
	BOOL	m_bContent;
	BOOL	m_bTitle;
	CString	m_strTarget;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
    void GetTopics(CBaseObject* pOb);

	std::queue<CTopic *>  m_qTopics;
	CStringArray&         m_listTarget;
	// Generated message map functions
	//{{AFX_MSG(CSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearchNext();
	//}}AFX_MSG
	afx_msg void OnUpdateSearchNext(CCmdUI * pCmdUI);
	DECLARE_MESSAGE_MAP()
};