// RuleInfoDlg.h : header file
//
#pragma once

#include "Rule.h"
/////////////////////////////////////////////////////////////////////////////
// CRuleInfoDlg dialog

class CRuleInfoDlg : public CDialogEx
{
// Construction
public:
	CRuleInfoDlg(CWnd* pParent = NULL);   // standard constructor
	
	void SetValue(CRule::RuleData& data);
	BOOL GetValue(CRule::RuleData& data);
// Dialog Data
	//{{AFX_DATA(CRuleInfoDlg)
	enum { IDD = IDD_RULE };
	CString	m_strRuleName;
	int		m_nRuleEncoding;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuleInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuleInfoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};