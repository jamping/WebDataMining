// AlterTopicUnitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "AlterTopicUnitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlterTopicUnitDlg dialog


CAlterTopicUnitDlg::CAlterTopicUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlterTopicUnitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlterTopicUnitDlg)
	m_strValue = _T("");
	//}}AFX_DATA_INIT
}


void CAlterTopicUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlterTopicUnitDlg)
	DDX_Control(pDX, IDC_TOPIC_UNIT, m_ctlTopicUnit);
	DDX_Control(pDX, IDC_PROCESS_TYPE, m_ctlProcessType);
	DDX_CBIndex(pDX, IDC_TOPIC_UNIT, m_nTopicUnit);
	DDX_CBIndex(pDX, IDC_PROCESS_TYPE, m_nProcessType);
	DDX_Text(pDX, IDC_VALUE, m_strValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlterTopicUnitDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAlterTopicUnitDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlterTopicUnitDlg message handlers

void CAlterTopicUnitDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	m_strValue.TrimLeft();
	m_strValue.TrimRight();

	if( m_strValue.IsEmpty() && m_ctlProcessType.GetCurSel() != 3 )
	{
		MessageBox(_T("内容不能为空！"));
		return;
	}

	CDialogEx::OnOK();
}

BOOL CAlterTopicUnitDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctlTopicUnit.SetCurSel(9);
	m_ctlProcessType.SetCurSel(2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
