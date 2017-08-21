// UrlDlg.h : header file
// 
#pragma once

#include "Entry.h"
#include "Rule.h"
/////////////////////////////////////////////////////////////////////////////
// CUrlDlg dialog

class CUrlDlg : public CDialogEx
{
// Construction
public:
	CUrlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUrlDlg)
	enum { IDD = IDD_URL_DRAGLIST };
	CComboBox	m_comboRule;
	CListCtrl	m_listUrls;
//	CString	m_strFileDir;
	CString	m_strURL;
	//}}AFX_DATA

    int GetUrls(CStringArray& arrayUrl);
	CEntry* GetEntry() { return m_pEntry; }
	CRule* GetRule() { return m_pRule; }
//	CString GetFileDir() { return m_strFileDir; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CMap<CString,LPCTSTR,CString,LPCTSTR>  m_mapURL;
	CString     m_strSelColumn;
	CString     m_strSelEntry;

	CEntry*     m_pEntry;
	CRule*      m_pRule;
	// Generated message map functions
	//{{AFX_MSG(CUrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearUrl();
	afx_msg void OnDelUrl();
	afx_msg void OnPasteUrlClipboard();
	virtual void OnOK();
//	afx_msg void OnButtonBrowse();
	afx_msg void OnAddUrl();
	//}}AFX_MSG
	afx_msg void OnUpdateClearUrl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelUrl(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePasteUrlClipboard(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddURL(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};