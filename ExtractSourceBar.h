// ExtractSourceBar.h : interface of the CExtractSourceBar class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ExtractSourceTree.h"


class CExtractSourceBar : public CDockablePane
{
public:
	CExtractSourceBar();

// Attributes
protected:
	CExtractSourceTree	m_wndTree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtractSourceBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtractSourceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CExtractSourceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};