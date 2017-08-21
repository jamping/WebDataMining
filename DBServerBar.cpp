// DBServerBar.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "DBServerBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBServerBar
const int nBorderSize = 1;

CDBServerBar::CDBServerBar()
{
}

CDBServerBar::~CDBServerBar()
{
}


BEGIN_MESSAGE_MAP(CDBServerBar, CDockablePane)
	//{{AFX_MSG_MAP(CDBServerBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBServerBar message handlers

int CDBServerBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup trees content:
	m_wndTree.SetImageList(IDB_TREEIMGLIST,16,4,RGB(255,255,255));
	m_wndTree.InitUpdate(TRUE);
    theApp.SetDBServerTree(&m_wndTree);

	return 0;
}

void CDBServerBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

void CDBServerBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
	
}

void CDBServerBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndTree;
	ASSERT_VALID (pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_TOOLBAR, point.x, point.y, this, TRUE);
}


void CDBServerBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_wndTree.SetFocus();
}
