// EntryDlg.h : header file
//
#pragma once

#include "Entry.h"
#include "SetReadOnlyDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CEntryDlg dialog

class CEntryDlg : public CSetReadOnlyDialog
{
// Construction
public:
	CEntryDlg(BOOL bReadOnly=FALSE,CWnd* pParent = NULL);   // standard constructor

	void SetValue(CEntry::EntryData& data);
	BOOL GetValue(CEntry::EntryData& data);
// Dialog Data
	//{{AFX_DATA(CEntryDlg)
	enum { IDD = IDD_ENTRY_DLG };
	CComboBox	m_comboRule;
	BOOL	m_bAutoPublish;
	CString	m_strEntryName;
	CString	m_strRule;
	int		m_nTopicMax;
	int		m_nTryTimes;
	CString	m_strURL;
	CString	m_strWebName;
	CString	m_strServerColumnID;
	//}}AFX_DATA
    CString m_strRuleID;
	CString m_strEntryID;
	CString m_strScheduleID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntryDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntryDlg)
	afx_msg void OnButtonTest();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};