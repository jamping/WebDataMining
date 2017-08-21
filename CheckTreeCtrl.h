//#######################################################################################
//## CheckTreeCtrl.h : header file
//## [Magerusan G. Cosmin] 20-apr-2002
//#######################################################################################
#pragma once

//## ====================================================================================
#define IMG_OPEN	0L
#define IMG_CLOSE	1L
#define IMG_ITEM	2L
//#######################################################################################
#define NOTIFY_TREECOMBO_CHECK WM_USER+200

class CCheckTreeCtrl : public CTreeCtrl
{
//## CONSTRUCTOR
public:
	CCheckTreeCtrl();
	virtual ~CCheckTreeCtrl();

	void EnableSendMsgToParent(BOOL bEnable=TRUE) { m_bSendMsgToParent=bEnable; }
//## STATE
private:
	CWnd *m_pwndParentCombo;
	BOOL m_bUpdateNeeded;
    BOOL m_bSendMsgToParent;
//## MESSAGES
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

//## UPDATE methods
private:
	void UpdateItemImage(HTREEITEM hItem);
	void RecursiveUpdateFromState(HTREEITEM hParentItem);
	void UpdateParentWnd();
public:
	void UpdateToState();
	void UpdateFromState();

//## METHODS
public:
	void Populate();
	void SetParentCombo(CWnd *pwnd){ m_pwndParentCombo = pwnd; };

//## TREE Specific
private:
	BOOL GetItemExpanded(HTREEITEM hItem);
};