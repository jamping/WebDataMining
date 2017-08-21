// OptionsDatabase.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COptionsDatabase dialog

class COptionsDatabase : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionsDatabase)

// Construction
public:
	COptionsDatabase(BOOL bShowSqlConfig=FALSE);
	~COptionsDatabase();

// Dialog Data
	//{{AFX_DATA(COptionsDatabase)
	enum { IDD = IDD_OPTIONS_DATABASE };
	CComboBox	m_ctlDBServer;
	CComboBox	m_ctlDatabase;
	CString	m_strFileSaveDir;
	int		m_nDatabaseType;
	CString	m_strDatabase;
	CString	m_strServer;
	BOOL	m_bSavePasswd;
	CString	m_strUser;
	CString	m_strPass;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsDatabase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL   m_bShowSqlConfig;

	void Init();
	void ShowSQLConfig(int nDBType);
	// Generated message map functions
	//{{AFX_MSG(COptionsDatabase)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFilesave();
	afx_msg void OnDropdownDatabase();
	afx_msg void OnDropdownDbserver();
	afx_msg void OnSqlserver();
	afx_msg void OnAccess();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};