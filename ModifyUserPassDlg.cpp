// ModifyUserPassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ModifyUserPassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyUserPassDlg dialog


CModifyUserPassDlg::CModifyUserPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyUserPassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyUserPassDlg)
	m_strNewPass = _T("");
	m_strOldPass = _T("");
	//}}AFX_DATA_INIT
}


void CModifyUserPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyUserPassDlg)
	DDX_Text(pDX, IDC_NEWPASS, m_strNewPass);
	DDX_Text(pDX, IDC_OLDPASS, m_strOldPass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyUserPassDlg, CDialogEx)
	//{{AFX_MSG_MAP(CModifyUserPassDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyUserPassDlg message handlers
