// OptionsDatabase.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "OptionsDatabase.h"
#include "SQLInfoEnumerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDatabase property page

IMPLEMENT_DYNCREATE(COptionsDatabase, CMFCPropertyPage)

COptionsDatabase::COptionsDatabase(BOOL m_bShowSqlConfig/* =FALSE */)
     : CMFCPropertyPage(COptionsDatabase::IDD),m_bShowSqlConfig(m_bShowSqlConfig)
{
	//{{AFX_DATA_INIT(COptionsDatabase)
	m_strFileSaveDir = _T("");
	m_nDatabaseType = 0;
	m_strDatabase = _T("");
	m_strServer = _T("");
	m_bSavePasswd = FALSE;
	m_strUser = _T("");
	m_strPass = _T("");
	//}}AFX_DATA_INIT
}

COptionsDatabase::~COptionsDatabase()
{
}

void COptionsDatabase::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDatabase)
	DDX_Control(pDX, IDC_DBSERVER, m_ctlDBServer);
	DDX_Control(pDX, IDC_DATABASE, m_ctlDatabase);
	DDX_Text(pDX, IDC_FILE_SAVE_DIR, m_strFileSaveDir);
	DDX_Radio(pDX, IDC_SQLSERVER, m_nDatabaseType);
	DDX_CBString(pDX, IDC_DATABASE, m_strDatabase);
	DDX_CBString(pDX, IDC_DBSERVER, m_strServer);
	DDX_Check(pDX, IDC_SAVEPASSWORD, m_bSavePasswd);
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_Text(pDX, IDC_PASS, m_strPass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDatabase, CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionsDatabase)
	ON_BN_CLICKED(IDC_BUTTON_FILESAVE, OnButtonFilesave)
	ON_CBN_DROPDOWN(IDC_DATABASE, OnDropdownDatabase)
	ON_CBN_DROPDOWN(IDC_DBSERVER, OnDropdownDbserver)
	ON_BN_CLICKED(IDC_SQLSERVER, OnSqlserver)
	ON_BN_CLICKED(IDC_ACCESS, OnAccess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDatabase message handlers

BOOL COptionsDatabase::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	Init();
	ShowSQLConfig(m_nDatabaseType);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionsDatabase::OnButtonFilesave() 
{
	// TODO: Add your control notification handler code here
	if(theApp.GetShellManager()->BrowseForFolder(m_strFileSaveDir,this,m_strFileSaveDir,_T("Ñ¡ÔñÎÄ¼þ¼Ð")))
	{
        UpdateData(FALSE);
	}
}
void COptionsDatabase::Init()
{
	if(m_bShowSqlConfig)
	{
		GetDlgItem(IDC_SQLSERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ACCESS)->EnableWindow(FALSE);
	}
}
void COptionsDatabase::ShowSQLConfig(int nDBType)
{
	BOOL bShow=FALSE;
	if(nDBType==0)
		bShow=TRUE;

	GetDlgItem(IDC_DBSERVER)->EnableWindow(bShow);
	GetDlgItem(IDC_DATABASE)->EnableWindow(bShow);
	GetDlgItem(IDC_USER)->EnableWindow(bShow);
	GetDlgItem(IDC_PASS)->EnableWindow(bShow);
	GetDlgItem(IDC_SAVEPASSWORD)->EnableWindow(bShow);
}

void COptionsDatabase::OnDropdownDatabase() 
{
	// TODO: Add your control notification handler code here
	//Fill List of Databases
	CWaitCursor wait;
	m_ctlDatabase.ResetContent();
	UpdateData();
	CSQLInfoEnumerator obSQLEnumerate;

	if(obSQLEnumerate.EnumerateDatabase(m_strServer,m_strUser,m_strPass))
	{
		int iCount = obSQLEnumerate.m_szSQLServerDatabaseArray.GetSize();
		for(int i = 0; i < iCount; i++)
		{
			m_ctlDatabase.AddString(obSQLEnumerate.m_szSQLServerDatabaseArray[i]);
		}
	}	
}

void COptionsDatabase::OnDropdownDbserver() 
{
	// TODO: Add your control notification handler code here
	//Fill List of SQL Servers
	CWaitCursor wait;
	m_ctlDBServer.ResetContent();
	CSQLInfoEnumerator obSQLEnumerate;
	if(obSQLEnumerate.EnumerateSQLServers())
	{
		int iCount = obSQLEnumerate.m_szSQLServersArray.GetSize();
		for(int i = 0; i < iCount; i++)
		{
			m_ctlDBServer.AddString(obSQLEnumerate.m_szSQLServersArray[i]);
		}
	}
}

void COptionsDatabase::OnSqlserver() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowSQLConfig(m_nDatabaseType);
}

void COptionsDatabase::OnAccess() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowSQLConfig(m_nDatabaseType);
}
