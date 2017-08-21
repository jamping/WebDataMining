// TopicListCtrl.h : header file
//
#pragma once

#include "SortListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTopicListCtrl window
//#define CListCtrl CSortListCtrl

class CTopicListCtrl : public CSortListCtrl
{
// Construction
public:
	CTopicListCtrl();

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopicListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTopicListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTopicListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};