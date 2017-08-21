// ScheduleTree.h : header file
//
#pragma once

#include "customtree.h"
/////////////////////////////////////////////////////////////////////////////
// CScheduleTree window

class CScheduleTree : public CCustomTree
{
// Construction
public:
	CScheduleTree();

	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//ÏìÓ¦É¾³ý¼üÏûÏ¢
	virtual void InitUpdate(BOOL bReadOfDB=TRUE,BOOL bShowInfo=FALSE);

	CHelperObject m_objectHelper;
	UINT          m_nTimerID;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduleTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScheduleTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScheduleTree)
	afx_msg void OnRootSave();
	afx_msg void OnRootFresh();
	afx_msg void OnRootDelete();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnScheduleDelete();
	afx_msg void OnScheduleEdit();
	afx_msg void OnScheduleSave();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnExport();
	afx_msg void OnImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};