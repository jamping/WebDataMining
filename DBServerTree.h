// DBServerTree.h : header file
//
#pragma once

#include "customtree.h"
/////////////////////////////////////////////////////////////////////////////
// CDBServerTree window

class CDBServerTree : public CCustomTree
{
// Construction
public:
	CDBServerTree();

	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//ÏìÓ¦É¾³ý¼üÏûÏ¢
	virtual void InitUpdate(BOOL bReadOfDB=TRUE,BOOL bShowInfo=FALSE);

// Attributes
public:
	CHelperObject m_objectHelper;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBServerTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDBServerTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDBServerTree)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRootDelete();
	afx_msg void OnRootFresh();
	afx_msg void OnRootSave();
	afx_msg void OnDbserverDel();
	afx_msg void OnDbserverEdit();
	afx_msg void OnDbserverSave();
	afx_msg void OnDbserverNewDb();
	afx_msg void OnTableDel();
	afx_msg void OnTableEdit();
	afx_msg void OnTableNewOperator();
	afx_msg void OnTableSave();
	afx_msg void OnDatabaseDel();
	afx_msg void OnDatabaseEdit();
	afx_msg void OnDatabaseNewTable();
	afx_msg void OnDatabaseSave();
	afx_msg void OnOperatorDel();
	afx_msg void OnOperatorEdit();
	afx_msg void OnOperatorNewUnit();
	afx_msg void OnOperatorSave();
	afx_msg void OnUnitDel();
	afx_msg void OnUnitEdit();
	afx_msg void OnUnitSave();
	afx_msg void OnRootNew();
	afx_msg void OnExport();
	afx_msg void OnImport();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};