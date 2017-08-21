// CustomTree.cpp : implementation file
//

#include "stdafx.h"

#include "WebDataMining.h"
#include "BaseObject.h"
#include "CustomTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CCustomTree
IMPLEMENT_DYNAMIC(CCustomTree,CCJTreeCtrl)

CCustomTree::CCustomTree()
{
	m_nMenuID=0;
	m_nSubMenuPos=0;
	m_bDragging=FALSE;
	m_pDragImg=NULL;
}

CCustomTree::~CCustomTree()
{
	DeleteObject(m_hCursor);
}


BEGIN_MESSAGE_MAP(CCustomTree, CCJTreeCtrl)
	//{{AFX_MSG_MAP(CCustomTree)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustomTree message handlers

void CCustomTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_pDragImg=CreateDragImage(pNMTreeView->itemNew.hItem);
	if(m_pDragImg == NULL)
		return;

	int n=m_pDragImg->GetImageCount();

	if(m_pDragImg != NULL)
	{
		m_bDragging=m_pDragImg->BeginDrag(0,CPoint(0,0));
		m_pDragImg->DragEnter(this,pNMTreeView->ptDrag);
		m_hDraggedItem=pNMTreeView->itemNew.hItem;
	}

	if(!m_bDragging)
	{
		delete m_pDragImg;
	}

	*pResult = 0;
}

void CCustomTree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bDragging)
	{
		m_pDragImg->EndDrag();
		delete m_pDragImg;
		Select(GetDropHilightItem(),TVGN_CARET);
		m_bDragging=FALSE;

		HTREEITEM hItemTarget=GetDropHilightItem();

		if(m_hDraggedItem != hItemTarget &&				//不能拖放到自身
		   hItemTarget != NULL &&						//目标不能为空
		   GetParentItem(m_hDraggedItem) != hItemTarget)//不能拖放到父节点上
		{
			HTREEITEM hTmpItem=hItemTarget;				//不能由父节点拖放到子节点
			while((hTmpItem=GetParentItem(hTmpItem)) != NULL)
			{
				if(hTmpItem ==  m_hDraggedItem)
					break;
			}
			if(hTmpItem == NULL)
			{
				DoDrop(m_hDraggedItem,hItemTarget,0);
				SelectDropTarget(NULL);
			}
		}

		RedrawWindow();
	}

	CCJTreeCtrl::OnLButtonUp(nFlags, point);
}

void CCustomTree::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM hItem;
	UINT nFlag;

	if(m_bDragging)
	{
		hItem=HitTest(point,&nFlag);
		if(hItem != NULL)
		{
			if((nFlag & TVHT_ONITEM) && hItem != GetDropHilightItem())
			{
				SelectDropTarget(hItem);
			}
			if(nFlag & TVHT_ONITEMBUTTON)
			{
				CCJTreeCtrl::Expand(hItem,TVE_EXPAND);
			}
		}
		m_pDragImg->DragMove(point);
	}
	
	CCJTreeCtrl::OnMouseMove(nFlags, point);
}

void CCustomTree::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CCustomTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CBaseObject *pObOld=NULL,*pObNew=NULL;

	if(pNMTreeView->itemOld.hItem != NULL)
	{
		
		pObOld=(CBaseObject *)GetItemData(pNMTreeView->itemOld.hItem);
		if(pObOld != NULL)
		{
			
		}
	}

	if(pNMTreeView->itemNew.hItem != NULL)
	{
		pObNew=(CBaseObject *)GetItemData(pNMTreeView->itemNew.hItem);
		if(pObNew != NULL)
		{
			if(theApp.GetShowSelfView()!=NULL)
				theApp.GetShowSelfView()->ShowObject(pObNew);
			if(theApp.GetShowChildView()!=NULL )
				theApp.GetShowChildView()->ShowObject(pObNew,FALSE);
		}
	}

	*pResult = 0;
}

void CCustomTree::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,/*TVS_EDITLABELS | */TVS_HASLINES |TVS_LINESATROOT | TVS_HASBUTTONS);

	CCJTreeCtrl::PreSubclassWindow();
}

BOOL CCustomTree::PreDrop(HTREEITEM hItemDragged,HTREEITEM hItemTarget,int nAction)
{
	return TRUE;
}

BOOL CCustomTree::DoDrop(HTREEITEM hItemDragged,HTREEITEM hItemTarget,int nAction)
{
	if(PreDrop(hItemDragged,hItemTarget,nAction))
	{
		CBaseObject *pOBD=(CBaseObject *)GetItemData(hItemDragged);
		CBaseObject *pOBT=(CBaseObject *)GetItemData(hItemTarget);
		if(pOBT->CanAccept(pOBD,this))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CCustomTree::DeleteSelection(BOOL bDelSelf/* =TRUE */)
{
	CPtrList list;
	if(IsMultiSelect())
	{
		GetMultiSelectedObjects(list);
	}
	else
		list.AddTail(GetSelectedObject());

	CBaseObject* pOB,*pParent;
	POSITION pos=list.GetHeadPosition();
	while (pos)
	{
		pOB=(CBaseObject *)list.GetNext(pos);
		if (!bDelSelf)            
		{
			DeleteAllChildItem(pOB->GetRefItem());
		}
		else if(pOB != NULL && pOB->CanDelete())
		{
			pParent=pOB->GetParent();
			if(pParent != NULL)
			{
				pOB->RelatingDelete();  
				pOB->LeaveFrom(pParent);
			}
			if(pOB->GetRefItem()!=NULL) 
				DeleteItem(pOB->GetRefItem());
			delete pOB;
		}
	}

	return TRUE;
}


void CCustomTree::DeleteAllChildItem(HTREEITEM hItem)
{
	CBaseObject* pChild,*pParent;
	if (ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetChildItem(hItem);
		// Delete all of the children of hItem.
		while (hChildItem != NULL )
		{
			DeleteAllChildItem(hChildItem);

			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);	

			pChild=(CBaseObject *)GetItemData(hChildItem);	
			if(pChild != NULL && pChild->CanDelete())
			{
				pParent=pChild->GetParent();
				if(pParent!=NULL)
				{
					pChild->LeaveFrom(pParent);
				}
				if(pChild->GetRefItem()!=NULL)
                   DeleteItem(hChildItem);
				delete pChild;
			}						
			hChildItem = hNextItem;
		}
	}

}

BOOL CCustomTree::Expand(HTREEITEM hItem,UINT nCode/* =TVE_EXPAND */,BOOL bChild/* =FALSE */)
{
	if ( bChild && ItemHasChildren(hItem) )
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetChildItem(hItem);
		// Expand all of the children of hItem.
		while (hChildItem != NULL )
		{
			Expand(hChildItem,nCode,bChild);

			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);	

			CCJTreeCtrl::Expand(hChildItem,nCode);
			
			hChildItem = hNextItem;
		}
	}
	CCJTreeCtrl::Expand(hItem,nCode);

	return TRUE;
}
HTREEITEM CCustomTree::InsertItem(CBaseObject *pOb)
{
	CBaseObject *pParent=pOb->GetParent();
	HTREEITEM hParent=pParent != NULL ? pParent->GetRefItem() : NULL;
	if(hParent == NULL)		hParent=TVI_ROOT;

	HTREEITEM hItem=CCJTreeCtrl::InsertItem(pOb->GetDesc(),pOb->GetNormImg(),pOb->GetSelImg(),hParent);
	pOb->SetRefItem(hItem);
	if(pOb->IsSpecShow())
		SetItemSpecShow(hItem,TRUE,FALSE);
		
	SetItemData(hItem,(DWORD)pOb);
	if(hParent == TVI_ROOT)
		CTreeCtrl::Expand(hParent,TVE_EXPAND);

	return hItem;
}

BOOL CCustomTree::DeleteItem(HTREEITEM hItem)
{
	CBaseObject *pOB=(CBaseObject *)GetItemData(hItem);
    if(pOB!=NULL)
	{
		if(pOB->IsSpecShow())
	        pOB->SetSpecShowFlag(FALSE);

		pOB->InvalidateItemHandle();
	}

	return CCJTreeCtrl::DeleteItem(hItem);
}

BOOL CCustomTree::SetImageList(UINT nId,int cx,int nGrow,COLORREF crMask)
{
	BOOL bRet=m_ImgList.Create(nId,cx,nGrow,crMask);
	if(bRet)
		CCJTreeCtrl::SetImageList(&m_ImgList,TVSIL_NORMAL);
	return bRet;
}

void CCustomTree::SetPromptMenu(UINT nID, int nSubPos)
{
	m_nMenuID=nID;
	m_nSubMenuPos=nSubPos;
}

int CCustomTree::SelectObject(CBaseObject *pOb)
{
	if(pOb!=NULL && pOb->GetRefItem()!=NULL)
	{
 		SelectItem(pOb->GetRefItem());  
	}
	return 0;
}

CBaseObject *CCustomTree::GetSelectedObject()
{	
	HTREEITEM hItem=GetSelectedItem();
	if(hItem != NULL)
		return (CBaseObject *)GetItemData(hItem);
	else
		return NULL;
}
void CCustomTree::GetMultiSelectedObjects(CPtrList& listOb)
{
	ASSERT(m_bMultiSelect==TRUE);
	listOb.RemoveAll();

	for(HTREEITEM hItem=GetFirstSelectedItem();hItem;hItem=GetNextSelectedItem(hItem))
		  listOb.AddTail((CBaseObject *)GetItemData(hItem));
		
}
BOOL CCustomTree::IsMultiSelect()
{
	return m_bMultiSelect;
}
void CCustomTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/* =FALSE */)
{
}

void CCustomTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CCJTreeCtrl::OnRButtonDown(nFlags, point);

	HTREEITEM hItem=HitTest(point);
	if(hItem != NULL)
	{
		SelectItem(hItem);
		CBaseObject *pOb=GetSelectedObject();
		UINT nMenuID=pOb->PromptMenuID();
		int nMenuPos=pOb->PromptMenuPos();

		if(nMenuID != 0 && nMenuPos != -1)
		{
			CMenu *pMenu,menu;
			menu.LoadMenu(nMenuID);	
			pMenu=menu.GetSubMenu(nMenuPos);
    	
			if(pMenu!=NULL)
			{
				//更新菜单状态
				pOb->UpdateMenu(pMenu);
				CPoint pt;
				GetCursorPos(&pt);
				pMenu->TrackPopupMenu(TPM_LEFTBUTTON,pt.x,pt.y,this);
			}
		}
	}
}


void CCustomTree::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM hItem=HitTest(point);
	if(hItem != NULL)
	{
		SelectItem(hItem);
		CBaseObject *pOb=GetSelectedObject();
		if(pOb->EditObject(this,theApp.GetUserLevel()<1)==IDOK &&
			pOb->IsModified() && 
			MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			pOb->SaveToDB(*theApp.GetDB(),FALSE);			
		}
		pOb->RefreshLabel(this);	
	}
}

void CCustomTree::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem=pTVDispInfo->item.hItem;
	LPCSTR lpszText=pTVDispInfo->item.pszText;
	CBaseObject *pOb=(CBaseObject *)GetItemData(hItem);
	if(pOb != NULL && lpszText != NULL)
	{
		pOb->ChangeDesc(lpszText);
		SetItemText(hItem,pOb->GetDesc());
	}

	*pResult = 0;
}


BOOL CCustomTree::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
//	SetCursor(m_hCursor);

	return CCJTreeCtrl::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CCustomTree::SetItemSpecShow(HTREEITEM hItem,BOOL bSpec/*=TRUE*/,BOOL bInvalidate/* =TRUE */)
{
	if(hItem!=NULL)
	{		
		SetItemBold(hItem,bSpec);		
		SetItemColor(hItem,bSpec?RGB(255,0,0):(::GetSysColor(COLOR_WINDOWTEXT)));
		
		if( bInvalidate )
		    Invalidate();

		return TRUE;
	}
	return FALSE;
	
}

