// EveryDayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "EveryDayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEveryDayDlg dialog


CEveryDayDlg::CEveryDayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEveryDayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEveryDayDlg)
	m_nDays = 1;
	//}}AFX_DATA_INIT
}


void CEveryDayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEveryDayDlg)
	DDX_Control(pDX, IDC_SPIN1, m_spinDay);
	DDX_Text(pDX, IDC_DAY, m_nDays);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEveryDayDlg, CDialogEx)
	//{{AFX_MSG_MAP(CEveryDayDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEveryDayDlg message handlers

BOOL CEveryDayDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
 	m_spinDay.SetRange(1,366);
		
		return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
