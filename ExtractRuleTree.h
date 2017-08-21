// ExtractRuleTree.h : header file
//
#pragma once

#include "customtree.h"
#include "HelperObject.h"
/////////////////////////////////////////////////////////////////////////////
// CExtractRuleTree window

class CExtractRuleTree : public CCustomTree
{
// Construction
public:
	CExtractRuleTree();

	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//ÏìÓ¦É¾³ý¼üÏûÏ¢
	virtual void InitUpdate(BOOL bReadOfDB=TRUE,BOOL bShowInfo=FALSE);

	CHelperObject m_objectHelper;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtractRuleTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtractRuleTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtractRuleTree)
	afx_msg void OnRootNew();
	afx_msg void OnRuleNew();
	afx_msg void OnRootSave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRootDelete();
	afx_msg void OnRootFresh();
	afx_msg void OnRuleDelete();
	afx_msg void OnRuleEdit();
	afx_msg void OnRuleSave();
	afx_msg void OnRuleTest();
	afx_msg void OnExport();
	afx_msg void OnImport();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
};