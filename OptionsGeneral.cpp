// OptionsGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "OptionsGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneral property page

IMPLEMENT_DYNCREATE(COptionsGeneral, CMFCPropertyPage)

COptionsGeneral::COptionsGeneral() : CMFCPropertyPage(COptionsGeneral::IDD)
{
	//{{AFX_DATA_INIT(COptionsGeneral)
	m_bAutoRun = FALSE;
	m_bOne = FALSE;
	m_nOuter = -1;
	m_bCloseToTray = FALSE;
	m_bMiniToTray = FALSE;
	m_nPortNum = 0;
	m_strBroadcastIP = _T("");
	m_nMaxMission = 10;
	m_strMysqlDrive = _T("");
	//}}AFX_DATA_INIT
}

COptionsGeneral::~COptionsGeneral()
{
}

void COptionsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsGeneral)
	DDX_Control(pDX, IDC_IPADDRESS_BROADCAST, m_ctlBroadcastIP);
	DDX_Check(pDX, IDC_AUTORUN, m_bAutoRun);
	DDX_Check(pDX, IDC_ONE, m_bOne);
	DDX_Radio(pDX, IDC_OUTER, m_nOuter);
	DDX_Check(pDX, IDC_CLOSE_TO_TRAY, m_bCloseToTray);
	DDX_Check(pDX, IDC_MIMI_TO_TRAY, m_bMiniToTray);
	DDX_Text(pDX, IDC_PORTNUM, m_nPortNum);
	DDX_Text(pDX, IDC_IPADDRESS_BROADCAST, m_strBroadcastIP);
	DDX_Text(pDX, IDC_MAX_MISSION, m_nMaxMission);
	DDV_MinMaxInt(pDX, m_nMaxMission, 5, 100);
	DDX_Text(pDX, IDC_EDIT_MYSQL_DRIVE, m_strMysqlDrive);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsGeneral, CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionsGeneral)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneral message handlers

BOOL COptionsGeneral::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
