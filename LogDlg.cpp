// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDlg)
	m_strDBServer = _T("");
	m_strPass = _T("");
	m_bSavePass = TRUE;
	m_strUser = _T("");
	m_strDatabase = _T("");
	//}}AFX_DATA_INIT
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	DDX_Text(pDX, IDC_DBSERVER, m_strDBServer);
	DDX_Text(pDX, IDC_PASS, m_strPass);
	DDX_Check(pDX, IDC_SAVEPASSWORD, m_bSavePass);
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_Text(pDX, IDC_DATABASE, m_strDatabase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialogEx)
	//{{AFX_MSG_MAP(CLogDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers
