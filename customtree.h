// CustomTree.h : header file
//
#pragma once

#include <afxole.h>
#include <oleidl.h>

/////////////////////////////////////////////////////////////////////////////
// CCustomTree window

class CBaseObject;

/************************************************************************/
/* 公共基类，用以提供树视图对、栏目、条目和文章等管理的公共界面
/* 它与CBaseObject基类构成下述关系  
/* 
/*   CCustomTree Item <--> CBaseObject
/* 
/*  引入CJTreeCtrl类作为基类，主要为了增加树节点特殊显示和多节点选择
/*                                                              */
/************************************************************************/
#include "CJTreeCtrl.h"

class CCustomTree : public CCJTreeCtrl
{
protected:
	DECLARE_DYNAMIC(CCustomTree)
// Construction
public:
	CCustomTree();

// Attributes
public:
	HCURSOR m_hCursor;

// Operations
public:
	HTREEITEM InsertItem(CBaseObject *pOb);		//Insert a object to tree view, if it has been shown, just expand it
												//the item is added under it's parent's item,
												//if it has no parent or it's parent is not shown, the item add under the root item
	BOOL DeleteItem(HTREEITEM hItem);			//Overload this function to do some more process while deleting an item
    void DeleteAllChildItem(HTREEITEM hItem);   //Delete all of the children of Item
    BOOL SetItemSpecShow(HTREEITEM hItem,       //Specially show
		BOOL bSpec=TRUE,BOOL bInvalidate=TRUE);

	BOOL Expand(HTREEITEM hItem,UINT nCode=TVE_EXPAND,BOOL bChild=FALSE);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomTree)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_nMenuID;								// ****more consider needed****
	int m_nSubMenuPos;
	void SetPromptMenu(UINT nID,int nSubPos);
	virtual BOOL SetImageList(UINT nId,int cx,int nGrow,COLORREF crMask);	//Give me the imglist id and info 

	CImageList m_ImgList,*m_pDragImg;			//Image lists of the tree
	BOOL m_bDragging;							//is in dragging
	HTREEITEM m_hDraggedItem;					//which item is being dragged
	virtual ~CCustomTree();

	virtual BOOL PreDrop(HTREEITEM hItemDragged,HTREEITEM hItemTarget,int nAction);	//Called before actrual drop occured
	virtual BOOL DoDrop(HTREEITEM hItemDragged,HTREEITEM hItemTarget,int nAction);	//Do drop
	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//响应删除键消息

	int SelectObject(CBaseObject *pOb);			//
	CBaseObject *GetSelectedObject();			   //select one
    void GetMultiSelectedObjects(CPtrList& listOb);//multi select
	BOOL IsMultiSelect();
	
	virtual void InitUpdate(BOOL bReadOfDB=TRUE,BOOL bShowInfo=FALSE);


	//Implimentations of IDropTaget
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomTree)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};