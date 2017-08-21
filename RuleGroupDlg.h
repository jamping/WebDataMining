// RuleGroupDlg.h : header file
//
#pragma once

class CHelperObject;
/////////////////////////////////////////////////////////////////////////////
// CObjectGroupDlg dialog

class CObjectGroupDlg : public CDialogEx
{
// Construction
public:
	CObjectGroupDlg(HELPER_TYPE type,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectGroupDlg)
	enum { IDD = IDD_RULEGROUP_DLG };
	CListBox	m_listGroup;
	CString	m_strTitle;
	//}}AFX_DATA
    CBaseObject* GetSelObject() { return m_pSelObject; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HELPER_TYPE  m_type;
	CBaseObject*  m_pSelObject;
	// Generated message map functions
	//{{AFX_MSG(CObjectGroupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};