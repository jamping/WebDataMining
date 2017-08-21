// CustomTree.h : header file
//
#pragma once

#include <afxole.h>
#include <oleidl.h>

/////////////////////////////////////////////////////////////////////////////
// CCustomTree window

class CBaseObject;

/************************************************************************/
/* �������࣬�����ṩ����ͼ�ԡ���Ŀ����Ŀ�����µȹ���Ĺ�������
/* ����CBaseObject���๹��������ϵ  
/* 
/*   CCustomTree Item <--> CBaseObject
/* 
/*  ����CJTreeCtrl����Ϊ���࣬��ҪΪ���������ڵ�������ʾ�Ͷ�ڵ�ѡ��
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
	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//��Ӧɾ������Ϣ

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