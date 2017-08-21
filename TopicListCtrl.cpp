// TopicListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "TopicListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopicListCtrl

CTopicListCtrl::CTopicListCtrl()
{
}

CTopicListCtrl::~CTopicListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTopicListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CTopicListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopicListCtrl message handlers
void CTopicListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,LVS_REPORT | LVS_EDITLABELS);
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES 
		            /*|LVS_EX_ONECLICKACTIVATE */|LVS_EX_CHECKBOXES);

	CSortListCtrl::PreSubclassWindow();
}

