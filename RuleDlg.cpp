// RuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "RuleDlg.h"
#include "RegexWDM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg dialog


CRuleDlg::CRuleDlg(BOOL bReadOnly,CWnd* pParent /*=NULL*/)
	: CSetReadOnlyDialog(bReadOnly,CRuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleDlg)
	m_strRuleName = _T("");
	m_strMatchRule = _T("");
	m_strProcessType = _T("");
	m_strSaveType = _T("");
	//}}AFX_DATA_INIT
}

void CRuleDlg::SetValue(CRuleTree::RuleTreeData& data)
{
	m_strRuleName=data.m_strRuleTreeName;
	m_strMatchRule=data.m_strMatchRule;
	m_nProcessType=data.m_nProcessType;
	m_nSaveType=data.m_nSaveType;
}
BOOL CRuleDlg::GetValue(CRuleTree::RuleTreeData& data)
{
	BOOL bModify=FALSE;
	if(!CCommonFunction::IsSameString(data.m_strRuleTreeName,m_strRuleName))
	{
		data.m_strRuleTreeName=m_strRuleName;
		bModify |= TRUE;
	}
    if(!CCommonFunction::IsSameString(data.m_strMatchRule,m_strMatchRule))
	{
		data.m_strMatchRule=m_strMatchRule;
		bModify |= TRUE;
	}
	if(data.m_nProcessType!=m_nProcessType)
	{
		data.m_nProcessType=m_nProcessType;
		bModify |= TRUE;
	}
	if(data.m_nSaveType!=m_nSaveType)
	{
		data.m_nSaveType=m_nSaveType;
		bModify |= TRUE;
	}
	return bModify;
}
void CRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CSetReadOnlyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleDlg)
	DDX_Control(pDX, IDC_SAVE_TYPE, m_comboSaveType);
	DDX_Control(pDX, IDC_PROCESS_TYPE, m_comboProcessType);
	DDX_Text(pDX, IDC_RULE_NAME, m_strRuleName);
	DDX_Text(pDX, IDC_MATCH_RULE, m_strMatchRule);
	DDV_MaxChars(pDX, m_strMatchRule, 300);
	DDX_CBString(pDX, IDC_PROCESS_TYPE, m_strProcessType);
	DDX_CBString(pDX, IDC_SAVE_TYPE, m_strSaveType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuleDlg, CSetReadOnlyDialog)
	//{{AFX_MSG_MAP(CRuleDlg)
	ON_CBN_SELCHANGE(IDC_PROCESS_TYPE, OnSelchangeProcessType)
//	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_PROCESS_TYPE,OnUpdateProcessType)
	ON_UPDATE_COMMAND_UI(IDC_SAVE_TYPE,OnUpdateSaveType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg message handlers

void CRuleDlg::OnOK() 
{
	// TODO: Add extra validation here
    m_nProcessType=m_comboProcessType.GetCurSel();
	m_nSaveType=m_comboSaveType.GetCurSel();

	CSetReadOnlyDialog::OnOK();
}

BOOL CRuleDlg::OnInitDialog() 
{
	CSetReadOnlyDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboProcessType.SetCurSel(m_nProcessType);
	m_comboSaveType.SetCurSel(m_nSaveType);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRuleDlg::OnSelchangeProcessType() 
{
	// TODO: Add your control notification handler code here
    m_nProcessType=m_comboProcessType.GetCurSel();
}

BOOL CRuleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CSetReadOnlyDialog::PreTranslateMessage(pMsg);
}
void CRuleDlg::OnUpdateSaveType(CCmdUI* pCmdUI)
{
	UNREFERENCED_PARAMETER(pCmdUI);
	pCmdUI->Enable(m_nProcessType==0);
}
void CRuleDlg::OnUpdateProcessType(CCmdUI* pCmdUI)
{
	UNREFERENCED_PARAMETER(pCmdUI);
}

