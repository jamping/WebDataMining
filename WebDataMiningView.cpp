// WebDataMiningView.cpp : implementation of the CWebDataMiningView class
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WebDataMining.h"
#endif

#include "WebDataMiningDoc.h"
#include "WebDataMiningView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningView

IMPLEMENT_DYNCREATE(CWebDataMiningView, CView)

BEGIN_MESSAGE_MAP(CWebDataMiningView, CView)
	//{{AFX_MSG_MAP(CWebDataMiningView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningView construction/destruction

CWebDataMiningView::CWebDataMiningView()
{
	// TODO: add construction code here

}

CWebDataMiningView::~CWebDataMiningView()
{
}

BOOL CWebDataMiningView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningView drawing

void CWebDataMiningView::OnDraw(CDC* pDC)
{
	CWebDataMiningDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningView printing

void CWebDataMiningView::OnFilePrintPreview() 
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWebDataMiningView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWebDataMiningView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWebDataMiningView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningView diagnostics

#ifdef _DEBUG
void CWebDataMiningView::AssertValid() const
{
	CView::AssertValid();
}

void CWebDataMiningView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWebDataMiningDoc* CWebDataMiningView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWebDataMiningDoc)));
	return (CWebDataMiningDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebDataMiningView message handlers

void CWebDataMiningView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	theApp.OnViewDoubleClick (this, IDR_MAINFRAME);
}

void CWebDataMiningView::OnContextMenu(CWnd*, CPoint point)
{
// 	theApp.ShowPopupMenu (IDR_CONTEXT_MENU1, point, this);
}

int CWebDataMiningView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here


	return 0;
}



BOOL CWebDataMiningView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	m_list.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0), pParentWnd, 1);
// 
// 	m_list.SetHeadings(_T("���,50;����,250;��Ŀ,80;��Ŀ,100;����1,100;����2,100;ר��,100;����,160;ͼƬ����,100")); 
// 
// 	for(int i=0;i<50;i++)
// 	{
// 		CString str;
// 		str.Format("%d",i+1);
// 	    m_list.AddItem(str,"�й������ž�","����","�ۺϱ���","","","","","");
// 	}

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
