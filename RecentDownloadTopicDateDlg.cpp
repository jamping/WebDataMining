// RecentDownloadTopicDateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "RecentDownloadTopicDateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecentDownloadTopicDateDlg dialog


CRecentDownloadTopicDateDlg::CRecentDownloadTopicDateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecentDownloadTopicDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecentDownloadTopicDateDlg)
	//}}AFX_DATA_INIT
	m_time = COleDateTime::GetCurrentTime();
}


void CRecentDownloadTopicDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecentDownloadTopicDateDlg)
	//}}AFX_DATA_MAP
	DDX_DateTimeCtrl(pDX, IDC_DATETIME, m_time);
}


BEGIN_MESSAGE_MAP(CRecentDownloadTopicDateDlg, CDialogEx)
	//{{AFX_MSG_MAP(CRecentDownloadTopicDateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecentDownloadTopicDateDlg message handlers

BOOL CRecentDownloadTopicDateDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecentDownloadTopicDateDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialogEx::OnOK();
}
