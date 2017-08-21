// ListCtrlEx.h : header file
//
#pragma once

#include "SortListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CSortListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};