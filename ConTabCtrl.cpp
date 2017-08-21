// ConTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ConTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConTabCtrl

CConTabCtrl::CConTabCtrl()
{
}

CConTabCtrl::~CConTabCtrl()
{
	PageNode *pNode;
	int n=m_listPages.GetSize();
	for(int i=0;i<n;i++)
	{
		pNode=(PageNode *)m_listPages.GetAt(i);
		TRACE("Page removed:Caption=%s,Page=%x\n",pNode->m_strCaption,pNode->m_pPage);
		delete pNode;
	}
}


BEGIN_MESSAGE_MAP(CConTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CConTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConTabCtrl message handlers





BOOL CConTabCtrl::AddPage(LPCSTR strCaption,CDialog *pPage,int nPos)
{
	OnSelchanging(NULL,NULL);

	int n=GetPageCount();
	if(nPos == -1)
		nPos=n;
	else if(nPos >= GetPageCount())
		nPos=n;

	SetCurSel(InsertItem(nPos,strCaption,nPos));

	PageNode *pNode=new PageNode;

	pNode->m_strCaption=strCaption;
	pNode->m_pPage=pPage;
	if(nPos == -1)
		m_listPages.Add(pNode);
	else
		m_listPages.InsertAt(nPos,pNode);

	pNode->m_lStyle=::GetWindowLong(pPage->m_hWnd,GWL_STYLE);
	pPage->ModifyStyle(WS_OVERLAPPEDWINDOW,WS_CHILDWINDOW,SWP_FRAMECHANGED);
	pPage->SetParent(this);

	CRect rect;
	GetClientRect(&rect);
	AdjustRect(FALSE,&rect);
	pPage->MoveWindow(&rect);
	
	TRACE("Page added:Caption=%s,Page=%x\n",strCaption,pPage);

	OnSelchange(NULL,NULL);

	return TRUE;
}

CDialog *CConTabCtrl::AddPage(LPCSTR strCaption, CRuntimeClass *pPageCls, UINT nTemplate,int nPos)
{
	//The class should be derived from class CDialog
	OnSelchanging(NULL,NULL);

	ASSERT(pPageCls != NULL ||
		pPageCls->IsDerivedFrom(RUNTIME_CLASS(CDialog)));

	CDialog *pPage=(CDialog *)pPageCls->CreateObject();
	ASSERT(pPage != NULL);
	pPage->Create(nTemplate,this);
	ASSERT(pPage->m_hWnd != NULL);

	AddPage(strCaption,pPage,nPos);

	TRACE("Page added:Caption=%s,Page=%x\n",strCaption,pPage);

	OnSelchange(NULL,NULL);

	return pPage;
}

CDockWindow *CConTabCtrl::DockPage(int n)
{
	CDockWindow *pWnd;
	CDialog *pPage=RemovePage(n);
	CString strCaption;
	CRect rect;

	pPage->GetWindowRect(&rect);
	pPage->GetWindowText(strCaption);

	pWnd=CDockWindow::Create(strCaption,this);
	pWnd->MoveWindow(&rect);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->SetPage(pPage,this,n);

	return pWnd;
}

void CConTabCtrl::DockPage(CDialog *pPage,int nPos)
{
	CString strCaption;

	pPage->GetWindowText(strCaption);
	AddPage(strCaption,pPage,nPos);
}

CDialog *CConTabCtrl::RemovePage(int n)
{
	OnSelchanging(NULL,NULL);

	PageNode *pNode=RemoveNode(n);
	CDialog *pPage=pNode->m_pPage;

	DeleteItem(n);

	int sel=n;
	if(sel >= GetItemCount())
		sel--;
	if(sel >= 0)
		SetCurSel(sel);

	::SetWindowLong(pPage->m_hWnd,GWL_STYLE,pNode->m_lStyle);
	pPage->SetWindowText(pNode->m_strCaption);
	pPage->SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

	delete pNode;
	OnSelchange(NULL,NULL);

	return pPage;
}

void CConTabCtrl::ActivePage(int n)
{
	if(n >= 0 && n < GetItemCount())
	{
		OnSelchanging(NULL,NULL);
		SetCurSel(n);
		OnSelchange(NULL,NULL);
	}
}

CConTabCtrl::PageNode *CConTabCtrl::RemoveNode(int n)
{
	PageNode *pNode=GetNode(n);

	if(pNode != NULL)
		m_listPages.RemoveAt(n);

	return pNode;
}

CConTabCtrl::PageNode *CConTabCtrl::GetNode(int n)
{
	if(n >= 0 && n < m_listPages.GetSize())
	{
		PageNode *pNode=(PageNode *)m_listPages.GetAt(n);

		return pNode;
	}

	return NULL;
}

int CConTabCtrl::GetPageCount()
{
	return m_listPages.GetSize();
}

CDialog * CConTabCtrl::GetPage(int n)
{
	PageNode *pNode=GetNode(n);
	if(pNode == NULL)
		return NULL;

	return pNode->m_pPage;
}

LPCSTR CConTabCtrl::GetPageTitle(int n)
{
	PageNode *pNode=GetNode(n);
	if(pNode == NULL)
		return NULL;

	return pNode->m_strCaption;
}
CDialog* CConTabCtrl::GetCurPage()
{
	int nTab=GetCurSel();
    CDialog *pWnd=NULL;

	if(nTab != -1)
	{
		pWnd=GetPage(nTab);
	}
	return pWnd;
}
void CConTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nTab=GetCurSel();

	if(nTab != -1)
	{
		CDialog *pWnd=GetPage(nTab);
		ASSERT(pWnd != NULL);
		if(pWnd != NULL)
		    pWnd->ShowWindow(SW_SHOW);
	}

	if(pResult != NULL)
		*pResult = 0;
}

void CConTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nTab=GetCurSel();

	if(nTab != -1)
	{
		CDialog *pWnd=GetPage(nTab);
		ASSERT(pWnd != NULL);
		pWnd->ShowWindow(SW_HIDE);
	}

	if(pResult != NULL)
		*pResult = 0;
}

void CConTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	PageNode *pNode;
	CRect rect(5,5,cx-5,cy-30);
	int n=m_listPages.GetSize();

	for(int i=0;i<n;i++)
	{
		pNode=(PageNode *)m_listPages.GetAt(i);
		if(pNode != NULL && pNode->m_pPage != NULL)
			pNode->m_pPage->MoveWindow(&rect);
	}
}

void CConTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
// 	TCHITTESTINFO hi;
// 
// 	hi.pt=point;
// 	hi.flags=0;
// 
// 	int n;
// 	if((n =HitTest(&hi)) != -1)
// 		DockPage(n);
// 
// 	CTabCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CConTabCtrl::SetImageList(UINT nID,int cx,int nGrow,COLORREF crMask)
{
	BOOL bRet=m_ImgList.Create(nID,cx,nGrow,crMask);
	if(bRet)
		CTabCtrl::SetImageList(&m_ImgList);
	return bRet;

}
///////////////////////////////////////////////////////
int CDockWindow::g_nWindows=0;
LPCTSTR CDockWindow::g_lpszClassName=NULL;

BEGIN_MESSAGE_MAP(CDockWindow, CWnd)
	//{{AFX_MSG_MAP(CDlg)
	ON_WM_NCDESTROY()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDockWindow *CDockWindow::Create(LPCTSTR strCaption,CWnd *pParent)
{
	if(g_nWindows == 0)
	{
		g_lpszClassName=AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
											::LoadCursor(NULL, IDC_ARROW),
											(HBRUSH) ::GetStockObject(GRAY_BRUSH),
											::LoadIcon(NULL, IDI_APPLICATION));
	}

	CDockWindow *pWnd=new CDockWindow;
	pWnd->CreateEx(WS_EX_TOOLWINDOW,
				   g_lpszClassName,
				   strCaption,
				   WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME ,
				   CRect(0,0,100,100),
				   pParent,
				   0,
				   NULL);

	g_nWindows++;

	return pWnd;
}

void CDockWindow::SetPage(CDialog *pPage,CConTabCtrl *pTab,int nTabPos)
{
	CRect rect;
	int w,h;

	pPage->GetWindowRect(&rect);
	CalcWindowRect(&rect);
	w=rect.Width();		h=rect.Height();

	SetWindowPos(NULL,0,0,w,h,SWP_NOMOVE | SWP_NOZORDER);
	pPage->SetParent(this);
	pPage->MoveWindow(0,0,w,h);
	pPage->ShowWindow(SW_SHOW);
	
	m_pPage=pPage;
	m_pTab=pTab;
	m_nTabPos=nTabPos;
}

CDialog *CDockWindow::GetPage()
{
	return m_pPage;
}


CDockWindow::CDockWindow()
{
	m_pTab=NULL;
	m_pPage=NULL;
	m_nTabPos=-1;
}

CDockWindow::~CDockWindow()
{
	g_nWindows--;
	if(g_nWindows == 0)
	{
		UnregisterClass(g_lpszClassName,AfxGetInstanceHandle());
	}
}

void CDockWindow::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	
	// TODO: Add your message handler code here
	delete this;
}

void CDockWindow::OnSize(UINT nType, int cx, int cy)
{
	if(m_pPage != NULL && m_pPage->m_hWnd != NULL)
		m_pPage->MoveWindow(2,2,cx-4,cy-4);
}

void CDockWindow::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch(nID)
	{
	case SC_CLOSE:
		m_pTab->DockPage(m_pPage,m_nTabPos);
		break;
	}

	CWnd::OnSysCommand(nID,lParam);
}


