//#######################################################################################
//## CheckTreeCtrl.cpp : implementation file
//#######################################################################################
#include "stdafx.h"

#include "CheckTreeCtrl.h"
#include "CheckComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//#######################################################################################
CCheckTreeCtrl::CCheckTreeCtrl() : CTreeCtrl()
{
	//## INITIALIZE
	m_pwndParentCombo = NULL;
	m_bUpdateNeeded = FALSE;
	m_bSendMsgToParent=TRUE;
}
//## ====================================================================================
CCheckTreeCtrl::~CCheckTreeCtrl()
{
}
//#######################################################################################
BEGIN_MESSAGE_MAP(CCheckTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//#######################################################################################
void CCheckTreeCtrl::Populate()
{
	//## ASSERT
	if (!m_hWnd) return;

	//## GET Data
	CCheckTreeData* pTreeData = ((CCheckComboBox*)m_pwndParentCombo)->GetData();
	BOOL bImageList = (GetImageList(TVSIL_NORMAL) != NULL);

	//## INSERT ROOT
	HTREEITEM hRoot;
	if (bImageList)
			hRoot = InsertItem(ROOT_CAPTION, IMG_OPEN, IMG_OPEN);
	else	hRoot = InsertItem(ROOT_CAPTION);
	SetItemData(hRoot, ROOT_INDEX);

	//## ADD rest of the items
	HTREEITEM hParents[TREE_MAX_LEVELS];
	hParents[ROOT_LEVEL] = hRoot;
	long nLevel; BOOL bIsLeaf,bChecked;
	for(long i=0+1; i<pTreeData->GetSize(); i++)
	{
		nLevel = pTreeData->ElementAt(i).nLevel;
		bIsLeaf = pTreeData->ElementAt(i).bIsLeaf;
		bChecked = pTreeData->ElementAt(i).bChecked;
		if (bImageList)
				hParents[ nLevel ] = InsertItem( pTreeData->ElementAt(i).strCaption, (bIsLeaf) ? IMG_ITEM : IMG_CLOSE, (bIsLeaf) ? IMG_ITEM : IMG_CLOSE, hParents[ nLevel - 1 ] );
		else	hParents[ nLevel ] = InsertItem( pTreeData->ElementAt(i).strCaption, hParents[ nLevel - 1 ] );
		SetItemData(hParents[ nLevel ], i);
		if(bChecked)
		{
			EnsureVisible(hParents[ nLevel ]);
		}
	}

	//## EXPAND Root
	this->Expand(hRoot, TVE_EXPAND);
}
//## ====================================================================================
void CCheckTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	//## CALL Parent
	CTreeCtrl::OnLButtonDown(nFlags, point);

	//## UPDATE Needed
	m_bUpdateNeeded = TRUE;

	//## UPDATE to state
	UpdateToState();
}
//## ====================================================================================
void CCheckTreeCtrl::OnPaint()
{
	//## CALL Parent
	CTreeCtrl::OnPaint();

	//## UPDATE to state
	if (m_bUpdateNeeded){
		m_bUpdateNeeded = FALSE;
		UpdateToState();
	}
}
//#######################################################################################
void CCheckTreeCtrl::UpdateToState()
{
	//## ASSERT
	if (!m_pwndParentCombo) return;

	//## DECLARE
	long nIndex; BOOL bChecked;

	//## GET Data
	CCheckTreeData* pTreeData = ((CCheckComboBox*)m_pwndParentCombo)->GetData();
	if (!pTreeData) return;
	if (!pTreeData->GetSize()) return;

	//## SCAN TREE
	HTREEITEM hItem = GetNextItem(TVI_ROOT, TVGN_FIRSTVISIBLE);
	while(hItem){
		//## GET ItemData = Index
		nIndex = GetItemData(hItem);

		//## UPDATE Image if needed
		if ((nIndex >= 0) && (!pTreeData->ElementAt(nIndex).bIsLeaf))
			UpdateItemImage(hItem);

		//## GET Checked. Modification => update & exit
		bChecked = GetCheck(hItem);
		if ((nIndex >= 0) && (pTreeData->ElementAt(nIndex).bChecked != bChecked)){
			pTreeData->ElementAt(nIndex).bChecked = bChecked;
			pTreeData->UpdateChecks(nIndex);
			UpdateFromState();
			UpdateParentWnd();
			Invalidate();
			return;
		}
	
		//## GO ON
		hItem = GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}
//#######################################################################################
BOOL CCheckTreeCtrl::GetItemExpanded(HTREEITEM hItem)
{
	//## ASSERT
	if (!hItem) return FALSE;

	//## RETURN Expanded
	return ((GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) != 0L);
}
//## ====================================================================================
void CCheckTreeCtrl::UpdateItemImage(HTREEITEM hItem)
{
	//## ASSERT
	if (!hItem) return;

	//## DECLARE
	BOOL bExpanded;
	int nImage, nSelectedImage;

	//## GET Expanded + Image
	bExpanded = GetItemExpanded(hItem);
	GetItemImage(hItem, nImage, nSelectedImage);

	//## UPDTE Image if necessary
	if (bExpanded && (nImage == IMG_CLOSE))
		SetItemImage(hItem, IMG_OPEN, IMG_OPEN);
	if (!bExpanded && (nImage == IMG_OPEN))
		SetItemImage(hItem, IMG_CLOSE, IMG_CLOSE);
}
//#######################################################################################
void CCheckTreeCtrl::UpdateFromState()
{
	//## ASSERT
	if (!m_pwndParentCombo) return;

	//## GET Data
	CCheckTreeData* pTreeData = ((CCheckComboBox*)m_pwndParentCombo)->GetData();
	if (pTreeData->GetSize() <= 0) return;

	//## GET Root & RootIndex
	HTREEITEM hItem = GetRootItem();
	long nIndex = GetItemData(hItem);

	//## SET Check
	SetCheck( hItem, pTreeData->ElementAt(nIndex).bChecked );

	//## SELECT
	HTREEITEM hItemToSelect = GetNextItem(TVI_ROOT, TVGN_CARET);
	if (!hItemToSelect) hItemToSelect = hItem;
	SelectItem( hItemToSelect );
	
	//## RECURSIVE
	RecursiveUpdateFromState(hItem);
}
//## ====================================================================================
void CCheckTreeCtrl::RecursiveUpdateFromState(HTREEITEM hParentItem)
{
	//## ASSERT
	if (!ItemHasChildren(hParentItem)) return;

	//## GET Data
	CCheckTreeData* pTreeData = ((CCheckComboBox*)m_pwndParentCombo)->GetData();

	//## SCAN
	long nIndex = 0L;
	HTREEITEM hItem = GetChildItem(hParentItem);
	while(hItem){
		//## GET ItemData = Index
		nIndex = GetItemData(hItem);

		//## SET Check
		SetCheck( hItem, pTreeData->ElementAt(nIndex).bChecked );

		//## RECURSIVE
		RecursiveUpdateFromState(hItem);

		//## GO ON
		hItem = GetNextItem(hItem, TVGN_NEXT);
	}	
}
//## ====================================================================================
void CCheckTreeCtrl::UpdateParentWnd()
{
	//## ASSERT
	if (!m_pwndParentCombo) return;

	//## GET Data
	CCheckTreeData* pTreeData = ((CCheckComboBox*)m_pwndParentCombo)->GetData();
	if (!pTreeData) return;

	//## SET Caption
	m_pwndParentCombo->SetWindowText( pTreeData->GetCheckedTexts() );
	//Send message to parent window of the combo tree
	CWnd* pParent=m_pwndParentCombo->GetParent();
	if ( pParent!=NULL && pParent->GetSafeHwnd () && m_bSendMsgToParent )
	{
		pParent->SendMessage (NOTIFY_TREECOMBO_CHECK);
	}
}
//#######################################################################################
