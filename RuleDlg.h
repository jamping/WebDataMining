// RuleDlg.h : header file
//
#pragma once

#include "RuleTree.h"
#include "SetReadOnlyDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CRuleDlg dialog


class CRuleDlg : public CSetReadOnlyDialog
{
// Construction
public:
	CRuleDlg(BOOL bReadOnly=FALSE,CWnd* pParent = NULL);   // standard constructor

	void SetValue(CRuleTree::RuleTreeData& data);
	BOOL GetValue(CRuleTree::RuleTreeData& data);
// Dialog Data
	//{{AFX_DATA(CRuleDlg)
	enum { IDD = IDD_RULE_DLG };
	CComboBox	m_comboSaveType;
	CComboBox	m_comboProcessType;
	CString	m_strRuleName;
	CString	m_strMatchRule;
	CString	m_strProcessType;
	CString	m_strSaveType;
	//}}AFX_DATA
    int     m_nProcessType;
	int     m_nSaveType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeProcessType();
//	afx_msg void OnTest();
	//}}AFX_MSG
	afx_msg void OnUpdateProcessType(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveType(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};