//#######################################################################################
//## CheckComboBox.h : header file
//## [Magerusan G. Cosmin] 20-apr-2002
//#######################################################################################

#pragma once

#include "CheckTreeCtrl.h"
#include "CheckTreeData.h"
//## ====================================================================================
#define DROP_BUTTON_WIDTH 16
#define DROPDOWN_HEIGHT 220
#define TOOLTIP_MAX_WIDTH 350
#define TOOLTIP_MAX_CHARACTERS 50*20
#define DROPPED_WIDTH_NOT_SET -1
//#######################################################################################
class CCheckComboBox : public CButton
{
//## CONSTRUCTOR
public:
	CCheckComboBox();
	virtual ~CCheckComboBox();
	DECLARE_DYNAMIC(CCheckComboBox)

//## STATE
private:
	CCheckTreeCtrl*	m_pwndTree;
	CCheckTreeData	m_Data;
	CImageList		m_imgList;
	static WNDPROC	m_parentWndProc;
	static CWnd*	m_pwndActiveDropDown;
	static CCheckComboBox* m_pwndActiveCheckComboBox;
	static CRect	m_rcParentRect;
	CToolTipCtrl	m_ToolTip;
	long			m_nDroppedWidth;
    BOOL            m_bSendMsgToParent;
//## OVERRIDE
protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

//## MESSAGES
protected:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg UINT OnGetDlgCode();

	static LRESULT CALLBACK ParentWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	static BOOL IsMsgOK(HWND hWnd, UINT nMsg,/* WPARAM wParam,*/ LPARAM lParam);
	DECLARE_MESSAGE_MAP()

//## INTERCEPT Methods
private:
	void InterceptParentWndProc();
	void UnInterceptParentWndProc();

//## PROPERTIES
public:
	CCheckTreeData* GetData(){ return &m_Data; };
	void EnableSendMsgToParent(BOOL bEnable=TRUE);

//## DROP Methods
private:
	void CreateDropWnd();
	void ShowDropWnd();
	void PlaceDropWnd();
	BOOL IsDropped();
	void Drop(BOOL bDrop);
	static void HideActiveDropDown();

//## TREE Methods
public:
	void AddString(LPCTSTR lpszString, CString strID = INVALID_ID, long nLevel = ROOT_LEVEL + 1);
	void CheckAll(BOOL bCheck);

//## GET/SET Methods
public:
	int			GetCount();
	BOOL		GetCheck(CString strID);
	CString		GetCheckedIDs();
	CString		GetCheckedTexts();
	CImageList* GetImageList();
	long		GetDroppedWidth();

	void		SetChecks(CString strIDs, BOOL bCheck);
	void		SetImageList(CImageList *pimgList);
	void		SetDroppedWidth(long nWidth);

//## TOOLTIP & CAPTION Methods
private:
	void InitToolTip();
	void SetToolTipText(CString strText, BOOL bActivate = TRUE);
	void SetToolTipText(int nId, BOOL bActivate = TRUE);
	void UpdateCaption();
};
