//ScheduleBar.h
#pragma once

#include "ScheduleTree.h"
/////////////////////////////////////////////////////////////////////////////
// CScheduleBar window

class CScheduleBar : public CDockablePane
{
// Construction
public:
	CScheduleBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduleBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScheduleBar();

	// Generated message map functions
protected:
	CScheduleTree	m_wndTree;
	//{{AFX_MSG(CScheduleBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};