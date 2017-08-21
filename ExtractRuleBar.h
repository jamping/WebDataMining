// ExtractRuleBar.h : interface of the CExtractRuleBar class

#pragma once

#include "ExtractRuleTree.h"


class CExtractRuleBar : public CDockablePane
{
public:
	CExtractRuleBar();

// Attributes
protected:
	CExtractRuleTree	m_wndTree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtractRuleBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtractRuleBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CExtractRuleBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
