// AlterPictureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "AlterPictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlterPictureDlg dialog


CAlterPictureDlg::CAlterPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlterPictureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlterPictureDlg)
	m_strValue = _T("");
	//}}AFX_DATA_INIT
}


void CAlterPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlterPictureDlg)
	DDX_Control(pDX, IDC_PROCESS_TYPE, m_ctlProcessType);
	DDX_CBIndex(pDX, IDC_PROCESS_TYPE, m_nProcessType);
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlterPictureDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAlterPictureDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlterPictureDlg message handlers

BOOL CAlterPictureDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctlProcessType.SetCurSel(2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
