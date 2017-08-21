// WebDataMiningView.h : interface of the CWebDataMiningView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SortListCtrl.h"

class CWebDataMiningView : public CView
{
protected: // create from serialization only
	CWebDataMiningView();
	DECLARE_DYNCREATE(CWebDataMiningView)

// Attributes
public:
	CWebDataMiningDoc* GetDocument() const;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebDataMiningView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWebDataMiningView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CSortListCtrl m_list;
// Generated message map functions
protected:
	//{{AFX_MSG(CWebDataMiningView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WebDataMiningView.cpp
inline CWebDataMiningDoc* CWebDataMiningView::GetDocument() const
{ return reinterpret_cast<CWebDataMiningDoc*>(m_pDocument); }
#endif
