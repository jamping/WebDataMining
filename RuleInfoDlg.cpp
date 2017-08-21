// RuleInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "RuleInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuleInfoDlg dialog


CRuleInfoDlg::CRuleInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRuleInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleInfoDlg)
	m_strRuleName = _T("");
	m_nRuleEncoding = 0;
	//}}AFX_DATA_INIT
}


void CRuleInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleInfoDlg)
	DDX_Text(pDX, IDC_RULE_NAME, m_strRuleName);
	DDX_Radio(pDX, IDC_ENCODING_TYPE, m_nRuleEncoding);
	//}}AFX_DATA_MAP
}
BOOL CRuleInfoDlg::GetValue(CRule::RuleData& data)
{
	BOOL bModify=FALSE;
	if( !CCommonFunction::IsSameString(data.m_strRuleName,m_strRuleName ))
	{
		data.m_strRuleName=m_strRuleName;
		bModify |=TRUE;
	}
	if(data.m_nRuleEncoding=m_nRuleEncoding)
	{
		data.m_nRuleEncoding=m_nRuleEncoding;
		bModify |= TRUE;
	}
	return bModify;
}

void CRuleInfoDlg::SetValue(CRule::RuleData& data)
{
	m_strRuleName=data.m_strRuleName;
	m_nRuleEncoding=data.m_nRuleEncoding;
}
BEGIN_MESSAGE_MAP(CRuleInfoDlg, CDialogEx)
	//{{AFX_MSG_MAP(CRuleInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleInfoDlg message handlers
