// ListViewEx.h : header file
//
#pragma once

#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CListViewEx form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CListViewEx : public CFormView
{
protected:
	CListViewEx();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CListViewEx)

// Form Data
public:
	//{{AFX_DATA(CListViewEx)
	enum { IDD = IDD_LIST_VIEW };
	CListCtrlEx	m_list;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void ShowObject(CBaseObject* pOb,BOOL bSelf=TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListViewEx)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CListViewEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CListViewEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};