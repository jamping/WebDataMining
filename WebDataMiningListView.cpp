// WebDataMiningListView.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "WebDataMiningListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningListView

IMPLEMENT_DYNCREATE(CWebDataMiningListView, CListView)

CWebDataMiningListView::CWebDataMiningListView()
{
}

CWebDataMiningListView::~CWebDataMiningListView()
{
}


BEGIN_MESSAGE_MAP(CWebDataMiningListView, CListView)
	//{{AFX_MSG_MAP(CWebDataMiningListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningListView drawing

void CWebDataMiningListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningListView diagnostics

#ifdef _DEBUG
void CWebDataMiningListView::AssertValid() const
{
	CListView::AssertValid();
}

void CWebDataMiningListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningListView message handlers
