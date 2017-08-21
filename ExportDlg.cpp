// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog


CExportDlg::CExportDlg(EXPORT_FILE_TYPE nSingleType/* =EXPORT_ALL */,CWnd* pParent /* = NULL */)
	: CDialogEx(CExportDlg::IDD, pParent),m_nSingleType(nSingleType)
{
	//{{AFX_DATA_INIT(CExportDlg)
	m_strFileDir = _T("");
	m_nType = 0;
	//}}AFX_DATA_INIT
}


void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDlg)
	DDX_Text(pDX, IDC_FILE_DIR, m_strFileDir);
	DDX_Radio(pDX, IDC_XML, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialogEx)
	//{{AFX_MSG_MAP(CExportDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

void CExportDlg::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(theApp.GetShellManager()->BrowseForFolder(m_strFileDir,this,m_strFileDir,_T("Ñ¡ÔñÎÄ¼þ¼Ð")))
	{
		UpdateData(FALSE);	
	}
}

BOOL CExportDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_nSingleType)
	{
	case EXPORT_XML:  //XML
		GetDlgItem(IDC_TXT)->EnableWindow(FALSE);
	    GetDlgItem(IDC_LANWAN)->EnableWindow(FALSE);
		break;
	case EXPORT_TXT:  //TXT
		GetDlgItem(IDC_XML)->EnableWindow(FALSE);
	    GetDlgItem(IDC_LANWAN)->EnableWindow(FALSE);
		break;
	case EXPORT_LANWAN: //LANWAN
        GetDlgItem(IDC_XML)->EnableWindow(FALSE);
	    GetDlgItem(IDC_TXT)->EnableWindow(FALSE);
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
