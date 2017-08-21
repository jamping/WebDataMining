// OutputBar.cpp : implementation of the COutputBar class
//

#include "stdafx.h"
#include "WebDataMining.h"
#include "OutputBar.h"
#include "DownloadHttpFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// COutputBar

BEGIN_MESSAGE_MAP(COutputBar, CDockablePane)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROCESS_INFO,OnProcessOutputInfo)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar construction/destruction

COutputBar::COutputBar()
{
	// TODO: add one-time construction code here

}

COutputBar::~COutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create list window.
	// TODO: create your own window here:
	const DWORD dwStyle =	
		ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_edit.Create (dwStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}
	
	return 0;
}

void COutputBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	m_edit.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectEdit;
	m_edit.GetWindowRect (rectEdit);
	ScreenToClient (rectEdit);

	rectEdit.InflateRect (1, 1);
	dc.Draw3dRect (rectEdit,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}
void COutputBar::UpdateTextInfo(CString strTitle,CString strContent/*=_T("")*/)
{
	CString str;
	m_edit.GetWindowText (str);
	if(!str.IsEmpty ())
	{
		if(str.GetLength ()>18000)
		{
			str=strTitle+" "+strContent;
		}
		else
		{
			str=str+"\r\n"+strTitle+" "+strContent;
		}
	}
	else
	{
		str=strTitle+" "+strContent;
	}
	m_edit.SetWindowText (str);
	m_edit.LineScroll(m_edit.GetLineCount());
}

void COutputBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
//	theApp.ShowPopupMenu (IDR_CONTEXT_MENU1, point, &m_edit);
}

BOOL COutputBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDockablePane::PreTranslateMessage(pMsg);
}
LRESULT COutputBar::OnProcessOutputInfo(WPARAM wParam,LPARAM lParam)
{
    UpdateTextInfo((LPCTSTR)lParam);
	return 0;
}

void COutputBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_edit.SetFocus();
}
