// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebDataMining.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog


CInfoDlg::CInfoDlg(BOOL bReadOnly,CWnd* pParent /*=NULL*/)
	: CSetReadOnlyDialog(bReadOnly,CInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoDlg)
	m_strName = _T("");
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
}


void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSetReadOnlyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoDlg)
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoDlg, CSetReadOnlyDialog)
	//{{AFX_MSG_MAP(CInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg message handlers
