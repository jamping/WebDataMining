// ColumnClassInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ColumnClassInfoDlg.h"
#include "ServerColumnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColumnClassInfoDlg dialog


CColumnClassInfoDlg::CColumnClassInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColumnClassInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnClassInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CColumnClassInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnClassInfoDlg)
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnClassInfoDlg, CDialogEx)
	//{{AFX_MSG_MAP(CColumnClassInfoDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
 	ON_UPDATE_COMMAND_UI(IDC_APPLY,OnUpdateOnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumnClassInfoDlg message handlers

BOOL CColumnClassInfoDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
 	m_tree.SetImageList(IDB_TREEIMGLIST,16,4,RGB(255,255,255));
    theApp.GetServerColumnRoot()->ShowInTreeView(&m_tree,TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColumnClassInfoDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	CBaseObject* pOb=theApp.GetSourceTree()->GetSelectedObject();
// 	if(pOb->GetType()!=OBJECT_TYPE_ENTRY)
// 	{
// 		MessageBox(_T("选择的不是条目!"),NULL,MB_OK|MB_ICONWARNING);
// 		return;
// 	}
	CBaseObject* pSelOb=m_tree.GetSelectedObject();
// 	if(pSelOb==NULL || pSelOb->GetChildCount()>0 )
// 	{
//         MessageBox(_T("没有选择具体条目或选择的不是条目!"),NULL,MB_OK|MB_ICONWARNING);
// 		return;
// 	}
	BOOL bRet=TRUE;
	CEntry* pEntry=(CEntry*)pOb;

	if(MessageBox(pSelOb->GetDesc()+_T("===>")+pEntry->m_data.m_strEntryName+
		_T("\n栏目ID：")+pSelOb->GetID(),("请确认"),MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		pEntry->m_data.m_strServerColumnID=pSelOb->GetID();
		pEntry->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
		bRet=FALSE;
	
	MessageBox(bRet?_T("设置服务器对应栏目ID成功"):_T("设置服务器对应栏目ID失败"));
}



void CColumnClassInfoDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialogEx::PostNcDestroy();
}


BOOL CColumnClassInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CColumnClassInfoDlg::OnUpdateOnApply(CCmdUI* pComUUI)
{
	CBaseObject* pOb=theApp.GetSourceTree()->GetSelectedObject();
	CBaseObject* pSelOb=m_tree.GetSelectedObject();

     pComUUI->Enable( (pOb!=NULL) 
		           && (pOb->GetType() == OBJECT_TYPE_ENTRY) 
				   && (pSelOb!=NULL) 
				   && (pSelOb->GetChildCount()==0) );
}
