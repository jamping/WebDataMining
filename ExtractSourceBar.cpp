//ExtractSourceBar.cpp : implementation of the CExtractSourceBar class
//

#include "stdafx.h"
#include "WebDataMining.h"
#include "ExtractSourceBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CExtractSourceBar

BEGIN_MESSAGE_MAP(CExtractSourceBar, CDockablePane)
	//{{AFX_MSG_MAP(CExtractSourceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtractSourceBar construction/destruction

CExtractSourceBar::CExtractSourceBar()
{
	// TODO: add one-time construction code here

}

CExtractSourceBar::~CExtractSourceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CExtractSourceBar message handlers

int CExtractSourceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	m_wndTree.SetImageList(IDB_TREEIMGLIST,16,4,RGB(255,255,255));
	m_wndTree.InitUpdate(TRUE);
	theApp.SetSourceTree(&m_wndTree);

	return 0;
}

void CExtractSourceBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CExtractSourceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
		::GetSysColor (COLOR_3DSHADOW));
}

void CExtractSourceBar::OnContextMenu(CWnd* pWnd, CPoint point) 
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


BOOL CExtractSourceBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDockablePane::PreTranslateMessage(pMsg);
}


void CExtractSourceBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_wndTree.SetFocus();
}
