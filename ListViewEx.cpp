// ListViewEx.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ListViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListViewEx

IMPLEMENT_DYNCREATE(CListViewEx, CFormView)

CListViewEx::CListViewEx()
	: CFormView(CListViewEx::IDD)
{
	//{{AFX_DATA_INIT(CListViewEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CListViewEx::~CListViewEx()
{
}

void CListViewEx::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListViewEx)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListViewEx, CFormView)
	//{{AFX_MSG_MAP(CListViewEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListViewEx diagnostics

#ifdef _DEBUG
void CListViewEx::AssertValid() const
{
	CFormView::AssertValid();
}

void CListViewEx::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListViewEx message handlers

void CListViewEx::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
}

void CListViewEx::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_list.GetSafeHwnd())
		m_list.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE|SWP_NOZORDER);
}

void CListViewEx::ShowObject(CBaseObject *pOb, BOOL bSelf)
{
     pOb->FillListCtrl(&m_list,bSelf);
}

void CListViewEx::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem=pNMListView->iItem;
	
	if(nItem!=-1)
	{		
		CBaseObject* pOb=(CBaseObject*)m_list.GetItemData(nItem);

		if(pOb->EditObject(this,theApp.GetUserLevel()<1)==IDOK && pOb->IsModified() && 
		   MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			pOb->SaveToDB(*theApp.GetDB(),FALSE);			
		}
	}
	
	*pResult = 0;
}
