//DBServerBar.h
#pragma once

#include "DBServerTree.h"
/////////////////////////////////////////////////////////////////////////////
// CDBServerBar window

class CDBServerBar : public CDockablePane
{
// Construction
public:
	CDBServerBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBServerBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDBServerBar();

	// Generated message map functions
protected:
	CDBServerTree m_wndTree;
	//{{AFX_MSG(CDBServerBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};