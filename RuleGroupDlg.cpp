// RuleGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "RuleGroupDlg.h"
#include "HelperObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectGroupDlg dialog


CObjectGroupDlg::CObjectGroupDlg(HELPER_TYPE type,CWnd* pParent /* = NULL */)
	: CDialogEx(CObjectGroupDlg::IDD, pParent),m_type(type),m_pSelObject(NULL)
{
	//{{AFX_DATA_INIT(CObjectGroupDlg)
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
}

void CObjectGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectGroupDlg)
	DDX_Control(pDX, IDC_LIST_RULE, m_listGroup);
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectGroupDlg, CDialogEx)
	//{{AFX_MSG_MAP(CObjectGroupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectGroupDlg message handlers

BOOL CObjectGroupDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBaseObject *pOb=NULL;
	CBaseObject *pRoot=NULL;
    CString str;
	switch(m_type)
	{
	case HELPER_RULE:
		pRoot=&theApp.GetRuleTree()->m_objectHelper;
		str=_T("提取规则组");
		break;
	case HELPER_DBSERVER:
		pRoot=&theApp.GetDBServerTree()->m_objectHelper;
		str=_T("数据库服务器组");
		break;
	default:
		ASSERT(FALSE);
	}
    SetWindowText(str);
	m_strTitle=_T("已有")+str;

	POSITION pos=pRoot->GetFirstChildPosition();
	while (pos!=NULL)
	{
        pOb=pRoot->GetNextChild(pos);
        if( pOb!=NULL)
		{
			m_listGroup.SetItemData(m_listGroup.AddString(pOb->GetDesc()),(DWORD)pOb);
		}
	}
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	int idx=m_listGroup.GetCurSel();
	if( idx !=LB_ERR)
	{
		m_pSelObject=((CBaseObject*)m_listGroup.GetItemData(idx));
	}
    else
	{
		m_pSelObject=NULL;
	}
	CDialogEx::OnOK();
}
