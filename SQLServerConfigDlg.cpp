// SQLServerConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "SQLServerConfigDlg.h"
#include "SQLInfoEnumerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSQLServerConfigDlg dialog


CSQLServerConfigDlg::CSQLServerConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSQLServerConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSQLServerConfigDlg)
	m_strPass = _T("");
	m_bSavePasswd = FALSE;
	m_strUser = _T("");
	m_strDatabase = _T("");
	m_strServer = _T("");
	//}}AFX_DATA_INIT
}


void CSQLServerConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSQLServerConfigDlg)
	DDX_Control(pDX, IDC_DBSERVER, m_ctlDBServer);
	DDX_Control(pDX, IDC_DATABASE, m_ctlDatabase);
	DDX_Text(pDX, IDC_PASS, m_strPass);
	DDX_Check(pDX, IDC_SAVEPASSWORD, m_bSavePasswd);
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_CBString(pDX, IDC_DATABASE, m_strDatabase);
	DDX_CBString(pDX, IDC_DBSERVER, m_strServer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSQLServerConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CSQLServerConfigDlg)
	ON_CBN_DROPDOWN(IDC_DBSERVER, OnDropdownDbserver)
	ON_CBN_DROPDOWN(IDC_DATABASE, OnDropdownDatabase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSQLServerConfigDlg message handlers

void CSQLServerConfigDlg::OnDropdownDbserver() 
{
	// TODO: Add your control notification handler code here

}

void CSQLServerConfigDlg::OnDropdownDatabase() 
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
