// OptionsGeneral.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneral dialog

class COptionsGeneral : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionsGeneral)

// Construction
public:
	COptionsGeneral();
	~COptionsGeneral();

// Dialog Data
	//{{AFX_DATA(COptionsGeneral)
	enum { IDD = IDD_OPTIONS_GENERAL };
	CIPAddressCtrl	m_ctlBroadcastIP;
	BOOL	m_bAutoRun;
	BOOL	m_bOne;
	int		m_nOuter;
	BOOL	m_bCloseToTray;
	BOOL	m_bMiniToTray;
	int		m_nPortNum;
	CString	m_strBroadcastIP;
	int		m_nMaxMission;
	CString m_strMysqlDrive;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};