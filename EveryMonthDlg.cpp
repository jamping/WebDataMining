// EveryMonthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "EveryMonthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEveryMonthDlg dialog


CEveryMonthDlg::CEveryMonthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEveryMonthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEveryMonthDlg)
	m_nEvery = 0;
	m_nDay = 1;
	m_nMonth = 1;
	m_nMonth2 = 1;
	m_nComboDay = 0;
	m_nComboNum = 0;
	//}}AFX_DATA_INIT
}


void CEveryMonthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEveryMonthDlg)
	DDX_Control(pDX, IDC_SPIN_MONTH2, m_spinMonth2);
	DDX_Control(pDX, IDC_SPIN_MONTH, m_spinMonth);
	DDX_Control(pDX, IDC_SPIN_DAY, m_spinDay);
	DDX_Radio(pDX, IDC_EVERY, m_nEvery);
	DDX_Text(pDX, IDC_DAY, m_nDay);
	DDV_MinMaxInt(pDX, m_nDay, 1, 31);
	DDX_Text(pDX, IDC_MONTH, m_nMonth);
	DDX_Text(pDX, IDC_MONTH2, m_nMonth2);
	DDX_CBIndex(pDX, IDC_COMBO_DAY, m_nComboDay);
	DDX_CBIndex(pDX, IDC_COMBO_NUM, m_nComboNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEveryMonthDlg, CDialogEx)
	//{{AFX_MSG_MAP(CEveryMonthDlg)
	ON_BN_CLICKED(IDC_EVERY, OnEvery)
	ON_BN_CLICKED(IDC_EVERY2, OnEvery2)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_MONTH,OnUpdateMonth)
	ON_UPDATE_COMMAND_UI(IDC_DAY,OnUpdateMonth)
	ON_UPDATE_COMMAND_UI(IDC_MONTH2,OnUpdateMonth2)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_DAY,OnUpdateMonth2)
    ON_UPDATE_COMMAND_UI(IDC_COMBO_NUM,OnUpdateMonth2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEveryMonthDlg message handlers

BOOL CEveryMonthDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CEveryMonthDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_spinDay.SetRange(1,31);
	m_spinMonth.SetRange(1,99);
	m_spinMonth2.SetRange(1,99);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEveryMonthDlg::OnEvery() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CEveryMonthDlg::OnEvery2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
void CEveryMonthDlg::OnUpdateMonth(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nEvery==0);
}
void CEveryMonthDlg::OnUpdateMonth2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nEvery==1);
}
