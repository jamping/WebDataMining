// OutputBar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

class COutputBar : public CDockablePane
{
public:
	COutputBar();

	void UpdateTextInfo(CString strTitle,CString strContent=_T(""));

// Attributes
protected:
	CEdit	m_edit;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnProcessOutputInfo(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
