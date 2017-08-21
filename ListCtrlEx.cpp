// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CSortListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,LVS_REPORT | LVS_EDITLABELS);
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CSortListCtrl::PreSubclassWindow();
}

void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CSortListCtrl::OnRButtonDown(nFlags, point);

// 	UINT uFlags;
// 	int nItem = HitTest(point, &uFlags);
// 	
// 	if (uFlags & LVHT_ONITEMLABEL)
// 	{
// 		SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 	LVIS_SELECTED, 0);
// 
// 		CBaseObject *pOb=(CBaseObject*)GetItemData(nItem);
// 		if(pOb!=NULL)
// 		{
// 			UINT nMenuID=pOb->PromptMenuID();
// 			int nMenuPos=pOb->PromptMenuPos();
// 			
// 			if(nMenuID != 0 && nMenuPos != -1)
// 			{
// 				CMenu *pMenu,menu;
// 				menu.LoadMenu(nMenuID);	
// 				pMenu=menu.GetSubMenu(nMenuPos);
// 				
// 				if(pMenu!=NULL)
// 				{
// 					//¸üÐÂ²Ëµ¥×´Ì¬
// 					pOb->UpdateMenu(pMenu);
// 					CPoint pt;
// 					GetCursorPos(&pt);
// 					pMenu->TrackPopupMenu(TPM_LEFTBUTTON,pt.x,pt.y,this);
// 				}
// 			}
// 		}
// 	}
}
