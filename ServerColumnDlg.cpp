// ServerColumnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ServerColumnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerColumnDlg dialog


CServerColumnDlg::CServerColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerColumnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerColumnDlg)
	m_strColumnID = _T("");
	m_strColumnName = _T("");
	//}}AFX_DATA_INIT
}


void CServerColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerColumnDlg)
	DDX_Text(pDX, IDC_COLUMN_ID, m_strColumnID);
	DDX_Text(pDX, IDC_COLUMN_NAME, m_strColumnName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerColumnDlg, CDialogEx)
	//{{AFX_MSG_MAP(CServerColumnDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerColumnDlg message handlers
