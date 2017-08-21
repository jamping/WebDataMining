// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "WebDataMining.h"

#include "ChildFrm.h"
#include "ListViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(WS_SYSMENU | FWS_ADDTOTITLE);
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_MAXIMIZE;
	CMDIChildWndEx::ActivateFrame(nCmdShow);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class	
	if (!m_wndSplitter.CreateStatic(this,2, 1))
	{
		TRACE0("Failed to create splitter bar ");
		return FALSE;    // failed to create
	}
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CListViewEx),CSize(0,80),pContext);
	m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CListViewEx),CSize(0,80),pContext);

	theApp.SetShowSelfView(m_wndSplitter.GetPane(0,0));
    theApp.SetShowChildView(m_wndSplitter.GetPane(1,0));

    return TRUE;
//	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}
