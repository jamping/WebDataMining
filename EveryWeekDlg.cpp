// EveryWeekDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "EveryWeekDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEveryWeekDlg dialog


CEveryWeekDlg::CEveryWeekDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEveryWeekDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEveryWeekDlg)
	m_nWeeks = 1;
	m_bFriday = FALSE;
	m_bMonday = TRUE;
	m_bSaturday = FALSE;
	m_bSunday = FALSE;
	m_bThursday = FALSE;
	m_bTuesday = FALSE;
	m_bWednsday = FALSE;
	//}}AFX_DATA_INIT
}


void CEveryWeekDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEveryWeekDlg)
	DDX_Control(pDX, IDC_SPIN1, m_spinWeek);
	DDX_Text(pDX, IDC_WEEK, m_nWeeks);
	DDX_Check(pDX, IDC_FRIDAY, m_bFriday);
	DDX_Check(pDX, IDC_MONDAY, m_bMonday);
	DDX_Check(pDX, IDC_SATURDAY, m_bSaturday);
	DDX_Check(pDX, IDC_SUNDAY, m_bSunday);
	DDX_Check(pDX, IDC_THURSDAY, m_bThursday);
	DDX_Check(pDX, IDC_TUESDAY, m_bTuesday);
	DDX_Check(pDX, IDC_WEDNSDAY, m_bWednsday);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEveryWeekDlg, CDialogEx)
	//{{AFX_MSG_MAP(CEveryWeekDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEveryWeekDlg message handlers

BOOL CEveryWeekDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_spinWeek.SetRange(1,52);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
