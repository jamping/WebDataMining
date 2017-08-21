// DatabaseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "DatabaseInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatabaseInfoDlg dialog


CDatabaseInfoDlg::CDatabaseInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDatabaseInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDatabaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatabaseInfoDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDatabaseInfoDlg, CDialogEx)
	//{{AFX_MSG_MAP(CDatabaseInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatabaseInfoDlg message handlers

BOOL CDatabaseInfoDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tree.SubclassDlgItem(IDC_TREE,this);
	m_tree.SetImageList(IDB_TREEIMGLIST,16,4,RGB(255,255,255));
	m_tree.InitUpdate(TRUE,FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDatabaseInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	CBaseObject* pOb=m_tree.GetSelectedObject();
    if(pOb->GetType()!=OBJECT_TYPE_DATABASE)
	{
		MessageBox(_T("选择的不是数据库，请重新选择!"),_T("警告"),MB_OK | MB_ICONWARNING);
		return;
	}
	m_strDatabaseID=pOb->GetID();

	CDialogEx::OnOK();
}
