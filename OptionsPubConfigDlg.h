// OptionsPubConfigDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COptionsPubConfigDlg dialog

class COptionsPubConfigDlg : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionsPubConfigDlg)

// Construction
public:
	COptionsPubConfigDlg();
	~COptionsPubConfigDlg();

// Dialog Data
	//{{AFX_DATA(COptionsPubConfigDlg)
	enum { IDD = IDD_OPTIONS_PUBCONFIG };
	CListBox	m_listTopicUnit;
	CListBox	m_listTableUnit;
	CComboBox	m_ctrTables;
	CString	m_strSql;
	//}}AFX_DATA
    DWORD  m_dwTopicUnits;
    CMap<DWORD,DWORD,CString,LPCTSTR>  m_mapTopicUnits;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPubConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPubConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAssoc();
	afx_msg void OnSelchangeComboTables();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};