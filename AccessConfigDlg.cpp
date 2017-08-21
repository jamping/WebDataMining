// AccessConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "AccessConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessConfigDlg dialog


CAccessConfigDlg::CAccessConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessConfigDlg)
	m_strDatabase = _T("");
	m_strPass = _T("");
	m_bSavePasswd = FALSE;
	m_strUser = _T("");
	//}}AFX_DATA_INIT
}


void CAccessConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessConfigDlg)
	DDX_Text(pDX, IDC_DATABASE, m_strDatabase);
	DDX_Text(pDX, IDC_PASS, m_strPass);
	DDX_Check(pDX, IDC_SAVEPASSWORD, m_bSavePasswd);
	DDX_Text(pDX, IDC_USER, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessConfigDlg)
	ON_BN_CLICKED(IDC_SEL_DIR, OnSelDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessConfigDlg message handlers

void CAccessConfigDlg::OnSelDir() 
{
	// TODO: Add your control notification handler code here
	char szFilters[]="Access Files (*.mdb)|*.mdb||";
	
	CFileDialog fileDlg(TRUE,"","",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		m_strDatabase= fileDlg.GetPathName();
	}
}
