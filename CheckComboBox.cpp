//#######################################################################################
//## CheckComboBox.cpp : implementation file
//#######################################################################################
#include "stdafx.h"
#include "CheckComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//## ====================================================================================
WNDPROC CCheckComboBox::m_parentWndProc = NULL;
CWnd* CCheckComboBox::m_pwndActiveDropDown = NULL;
CCheckComboBox* CCheckComboBox::m_pwndActiveCheckComboBox = NULL;
CRect CCheckComboBox::m_rcParentRect = CRect(0, 0, 0, 0);
//#######################################################################################
IMPLEMENT_DYNAMIC(CCheckComboBox, CButton)
//## ====================================================================================
CCheckComboBox::CCheckComboBox()
{
	//## INITIALIZE
	m_pwndTree = NULL;
	m_parentWndProc = NULL;
	m_pwndActiveCheckComboBox = NULL;
	m_nDroppedWidth = DROPPED_WIDTH_NOT_SET;
	m_bSendMsgToParent=TRUE;
}
//## ====================================================================================
CCheckComboBox::~CCheckComboBox()
{
	//## HIDE ActiveDropDown
	HideActiveDropDown();

	//## UNINTERCEPT
	UnInterceptParentWndProc();

	//## DESTROY Tree
	if (m_pwndTree){
		m_pwndTree->ShowWindow( SW_HIDE );
		delete m_pwndTree;
		m_pwndTree = NULL;
	}
}
//## ====================================================================================
void CCheckComboBox::OnDestroy() 
{
	//## CALL Parent
	CButton::OnDestroy();
	
	//## HIDE ActiveDropDown
	HideActiveDropDown();

	//## UNINTERCEPT
	UnInterceptParentWndProc();
}
//#######################################################################################
BEGIN_MESSAGE_MAP(CCheckComboBox, CButton)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETDLGCODE()

	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
//#######################################################################################
void CCheckComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	//## CALL Parent
	CButton::OnLButtonDown(nFlags, point);

	//## SHOW Tree
	ShowDropWnd();
}
//## ====================================================================================
void CCheckComboBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//## CALL Parent
	CButton::OnLButtonDown(nFlags, point);

	//## SHOW Tree
	ShowDropWnd();
}
//## ====================================================================================
LRESULT CCheckComboBox::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	//## CATCH DropDown Keys
	switch (message){
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			switch ((int) wParam){
				case VK_F4:
					ShowDropWnd();
					return NULL;	//## INTERCEPT
				case VK_DOWN:
				case VK_UP:
					if (message == WM_SYSKEYDOWN)
						ShowDropWnd();
					return NULL;	//## INTERCEPT
				case VK_SPACE:
					return NULL;	//## INTERCEPT
			}
	}		

	//## CALL Parent
	return CButton::WindowProc(message, wParam, lParam);
}
//## ====================================================================================
BOOL CCheckComboBox::PreTranslateMessage(MSG* pMsg)
{
	//## INTERCEPT Keys => send them to tree
	if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		if(m_pwndActiveDropDown){
			m_pwndActiveDropDown->SendMessage( pMsg->message, pMsg->wParam, pMsg->lParam);
			((CCheckTreeCtrl*)m_pwndActiveDropDown)->UpdateToState();
			return FALSE;
		}

	//## PROCESS ToolTip
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}
//#######################################################################################
void CCheckComboBox::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	//## GET CDC
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//## FRAME
	CRect rc = lpDrawItemStruct->rcItem;
	pDC->DrawEdge( rc, EDGE_SUNKEN, BF_TOP | BF_LEFT | BF_BOTTOM | BF_RIGHT );

	//## DRAW white BK
	rc.DeflateRect(2, 2);
	static CBrush brWindow(GetSysColor(COLOR_WINDOW));
	static CBrush brBtnFace(GetSysColor(COLOR_BTNFACE));
	static CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
	if (IsWindowEnabled())
			pDC->FillRect(&rc, &brWindow);
	else	pDC->FillRect(&rc, &brBtnFace);

	//## COMPUTE Button Rect
	CRect rcButton = rc;
	rcButton.left = rcButton.right - DROP_BUTTON_WIDTH;
	if (rcButton.left < rc.left) rcButton.left = rc.left;

	//## COMPUTE Caption Rect
	CRect rcCaption = rc;
	rcCaption.right = rcButton.left - 1;
	if (rcCaption.right < rcCaption.left) rcCaption.right = rcCaption.left;

	//## FOCUS
	rcCaption.DeflateRect(1, 1);
	if (lpDrawItemStruct->itemState & ODS_FOCUS) pDC->DrawFocusRect(rcCaption);
	rcCaption.DeflateRect(2, 1);

	//## GET Caption
	CString strText;
	GetWindowText(strText);

	//## DRAW Caption
	pDC->SetBkColor( (IsWindowEnabled()) ? GetSysColor(COLOR_WINDOW) : GetSysColor(COLOR_BTNFACE) );
	COLORREF crOldColor = SetTextColor(lpDrawItemStruct->hDC, RGB(0, 0, 0));
	DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &rcCaption, DT_SINGLELINE | DT_VCENTER );
	SetTextColor(lpDrawItemStruct->hDC, crOldColor);

	//## GET Button Style
	UINT uStyle = DFCS_BUTTONPUSH;
	if (lpDrawItemStruct->itemState & ODS_SELECTED) uStyle |= DFCS_PUSHED;

	//## DRAW Button
	if (lpDrawItemStruct->itemState & ODS_SELECTED){
		pDC->FrameRect(rcButton, &brBtnShadow);
		rcButton.DeflateRect(1, 1);
		pDC->FillRect(&rcButton, &brBtnFace);
		rcButton.DeflateRect(1, 3, 0, 0);
	}else{
		pDC->FrameRect(rcButton, &brBtnFace);
		rcButton.DeflateRect(1, 1, 0, 0);
		DrawFrameControl(lpDrawItemStruct->hDC, &rcButton, DFC_BUTTON, uStyle);
	}

	//## DRAW Arrow
	static CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	static CPen penBtnShadow(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
	CPen* ppenOld;
	if (IsWindowEnabled())
			ppenOld = pDC->SelectObject(&penBlack);
	else	ppenOld = pDC->SelectObject(&penBtnShadow);
	for(long i=0; i<4; i++){
		pDC->MoveTo( rcButton.left + 3 + i, rcButton.top + rcButton.Height()/2 - 2 + i);
		pDC->LineTo( rcButton.left + 3 + 7 - i, rcButton.top + rcButton.Height()/2 - 2 + i);
		if (!IsWindowEnabled())
			pDC->SetPixel(rcButton.left + 3 + 7 - i, rcButton.top + rcButton.Height()/2 - 2 + i + 1, 0xFFFFFF);
	}
	pDC->SelectObject(ppenOld);
}
//#######################################################################################
void CCheckComboBox::AddString(LPCTSTR lpszString, CString strID, long nLevel)
{
	//## IF is the first item => add root
	if (m_Data.GetSize() == 0)
		m_Data.AddString(lpszString, INVALID_ID, ROOT_LEVEL);

	//## ADD
	m_Data.AddString(lpszString, strID, nLevel);
}
//## ====================================================================================
void CCheckComboBox::CheckAll(BOOL bCheck)
{
	m_Data.CheckAll(bCheck);
	UpdateCaption();
}
//## ====================================================================================
void CCheckComboBox::SetChecks(CString strIDs, BOOL bCheck)
{
	m_Data.SetChecks(strIDs, bCheck);
	UpdateCaption();
}
//## ====================================================================================
BOOL CCheckComboBox::GetCheck(CString strID)
{
	return m_Data.GetCheck(strID);
}
//## ====================================================================================
int CCheckComboBox::GetCount()
{
	return m_Data.GetSize();
}
//## ====================================================================================
CString CCheckComboBox::GetCheckedIDs()
{
	return m_Data.GetCheckedIDs();
}
CString	CCheckComboBox::GetCheckedTexts()
{
	return m_Data.GetCheckedTexts();
}
//## ====================================================================================
void CCheckComboBox::UpdateCaption()
{
	//## CAPTION
	CString str = m_Data.GetCheckedTexts();
	SetWindowText(str);

	//## ASSERT
	if (!::IsWindow(m_ToolTip.m_hWnd)) InitToolTip();
	if (!::IsWindow(m_ToolTip.m_hWnd)) return;

	//## TOOLTIP
	m_ToolTip.SetMaxTipWidth( TOOLTIP_MAX_WIDTH );
	if (str.GetLength() > TOOLTIP_MAX_CHARACTERS)
		str = str.Mid(0, (TOOLTIP_MAX_CHARACTERS-3)) + "...";
	SetToolTipText( str );
}
//#######################################################################################
void CCheckComboBox::SetImageList(CImageList *pimgList)
{
	//## SET ImageList
	m_imgList.Create( pimgList );
}
//## ====================================================================================
CImageList* CCheckComboBox::GetImageList()
{
	//## ASSERT
	if ((HIMAGELIST)m_imgList) return &m_imgList;
	return NULL;
}
//#######################################################################################
void CCheckComboBox::ShowDropWnd()
{
	//## ASSERT
	if (!m_pwndTree) CreateDropWnd();
	if (!m_pwndTree) return;

	//## PLACE Tree
	PlaceDropWnd();

	//## DROP
	Drop( !IsDropped() );
}
//## ====================================================================================
void CCheckComboBox::CreateDropWnd()
{
	//## ASSERT
	if (m_pwndTree) return;

	//## CREATE Tree
	m_pwndTree = new CCheckTreeCtrl;
// 	m_pwndTree->CreateEx(	WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOOLWINDOW,
// 							WC_TREEVIEW,
// 							NULL,
// 							WS_CHILD | WS_BORDER |
// 							WS_CLIPSIBLINGS | WS_OVERLAPPED |
// 							TVS_HASBUTTONS | TVS_HASLINES | TVS_CHECKBOXES | TVS_DISABLEDRAGDROP |
// 							TVS_SHOWSELALWAYS | TVS_FULLROWSELECT,
// 							CRect(0, 0, 0, 0),
// 							GetDesktopWindow(),
// 							0x3E8,
// 							NULL);

	m_pwndTree->CreateEx(	WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOOLWINDOW,
		WS_CHILD | WS_BORDER |
		WS_CLIPSIBLINGS | WS_OVERLAPPED |
		TVS_HASBUTTONS | TVS_HASLINES | TVS_CHECKBOXES | TVS_DISABLEDRAGDROP |
		TVS_SHOWSELALWAYS | TVS_FULLROWSELECT,
		CRect(0, 0, 0, 0),
		GetDesktopWindow(),
		0x3E8);
	//## MOVE on top
	m_pwndTree->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	//## SET Parent
	m_pwndTree->SetParentCombo( this );
    m_pwndTree->EnableSendMsgToParent(m_bSendMsgToParent);
	//## ATTACH ImageList
// 	if (GetImageList())
// 		m_pwndTree->SetImageList( GetImageList(), TVSIL_NORMAL );

	//## POPULATE
	m_pwndTree->Populate();
}
//## ====================================================================================
void CCheckComboBox::PlaceDropWnd()
{
	//## ASSERT
	if (!m_pwndTree) CreateDropWnd();
	if (!m_pwndTree) return;

	//## GET ComboRect
	CRect rc;
	GetWindowRect(&rc);

	//## MOVE Tree
	m_pwndTree->MoveWindow(rc.left, rc.bottom, GetDroppedWidth(), DROPDOWN_HEIGHT);
}
//## ====================================================================================
BOOL CCheckComboBox::IsDropped()
{
	//## ASSERT
	if (!m_pwndTree) return FALSE;

	//## RETURN
	return ( m_pwndTree->IsWindowVisible() );
}
//## ====================================================================================
void CCheckComboBox::Drop(BOOL bDrop)
{
	//## ASSERT
	if (!m_pwndTree) return;

	//## DROP/UNDROP
	if (bDrop){
		//## ASSERT
		HideActiveDropDown();

		//## INTERCEPT parent messages
//		if (!m_parentWndProc) InterceptParentWndProc();

		//## STRANGE (I do not know why... but without an image list, the following lines are needed)
		if (!(HIMAGELIST)m_imgList){
			m_pwndTree->ShowWindow( SW_SHOW );
			m_pwndTree->UpdateFromState();
		}

		//## UPDATE
		m_pwndTree->UpdateFromState();

		//## SHOW Window
		m_pwndTree->ShowWindow( SW_SHOW );
		m_pwndTree->SetFocus();

		//## SET Active DropDown
		m_pwndActiveDropDown = m_pwndTree;
		m_pwndActiveCheckComboBox = this;

		//## TOOLTIP
		SetToolTipText( "" );

		//## TO BE SURE (if our combo is in a FormView)
		if (this->GetParent())
			this->GetParent()->GetWindowRect( &m_rcParentRect );
	}else{
		//## ASSERT
		HideActiveDropDown();

		//## HIDE Window
		m_pwndTree->ShowWindow( SW_HIDE );

		//## CAPTION
		UpdateCaption();
	}
}
//## ====================================================================================
void CCheckComboBox::HideActiveDropDown()
{
	//## ASSERT
	if (!m_pwndActiveDropDown) return;

	//## HIDE Window
	m_pwndActiveDropDown->ShowWindow( SW_HIDE );
	m_pwndActiveDropDown = NULL;

	//## CAPTION
	if (m_pwndActiveCheckComboBox){
		m_pwndActiveCheckComboBox->UpdateCaption();
		m_pwndActiveCheckComboBox = NULL;
	}
}
//#######################################################################################
void CCheckComboBox::InterceptParentWndProc()
{
	//## ASSERT
	if (m_parentWndProc) return;

	//## ASSERT
	CWnd *pwndParent = GetParent();
	if (!pwndParent) return;
	if (!pwndParent->GetSafeHwnd()) return;

	//## GET Parent WinProc & SET our function
	m_parentWndProc = (WNDPROC)::SetWindowLong(pwndParent->GetSafeHwnd(), GWL_WNDPROC, (long)(WNDPROC)ParentWindowProc);
}
//## ====================================================================================
void CCheckComboBox::UnInterceptParentWndProc()
{
	//## ASSERT
	if (!m_parentWndProc) return;

	//## ASSERT
	CWnd *pwndParent = GetParent();
	if (!pwndParent) return;
	if (!pwndParent->GetSafeHwnd()) return;

	//## SET Parent WinProc = UNINTERCEPT
	(WNDPROC)::SetWindowLong(pwndParent->GetSafeHwnd(), GWL_WNDPROC, (long)(WNDPROC)m_parentWndProc);
	m_parentWndProc = NULL;
}
//## ====================================================================================
LRESULT CALLBACK CCheckComboBox::ParentWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//## CHECK Message
	switch (nMsg){
		case WM_COMMAND:
		case WM_SYSCOMMAND:
		case WM_SYSKEYDOWN:
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_WINDOWPOSCHANGING:
		case WM_WINDOWPOSCHANGED:
			//## FILTER Messages
			if (IsMsgOK(hWnd, nMsg,/* wParam,*/ lParam)) break;

			//## HIDE Active DropDown
			HideActiveDropDown();
			break;
		case WM_PAINT:
			//## TO BE SURE (if our combo is in a FormView)
			if (m_pwndActiveCheckComboBox)
				if (m_pwndActiveCheckComboBox->GetParent()){
					CRect rcParent;
					m_pwndActiveCheckComboBox->GetParent()->GetWindowRect( &rcParent );
					if ((rcParent.top != m_rcParentRect.top) || (rcParent.left != m_rcParentRect.left))
						HideActiveDropDown();
				}
			break;
	}

	//## CALL Parent
	if(!m_parentWndProc) return NULL;
	return CallWindowProc( m_parentWndProc, hWnd, nMsg, wParam, lParam );
}
//## ====================================================================================
BOOL CCheckComboBox::IsMsgOK(HWND hWnd, UINT nMsg, /*WPARAM wParam,*/ LPARAM lParam)
{
	//## ASSERT
	if (!hWnd) return FALSE;
	if (nMsg != WM_COMMAND) return FALSE;
	if (!m_pwndActiveDropDown) return FALSE;
	if ((HWND)lParam != m_pwndActiveCheckComboBox->GetSafeHwnd()) return FALSE;

	//## GET DropDownRect
	CRect rc;
	m_pwndActiveCheckComboBox->GetWindowRect( rc );

	//## GET MousePos
	CPoint pt;
	::GetCursorPos( &pt );

	//## CHECK is in rect
	if (pt.x < rc.left) return FALSE;
	if (pt.x > rc.right) return FALSE;
	if (pt.y < rc.top) return FALSE;
	if (pt.y > rc.bottom) return FALSE;

	//## FINALLY
	return TRUE;
}
//## ====================================================================================
UINT CCheckComboBox::OnGetDlgCode()
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS | CButton::OnGetDlgCode();
}
//#######################################################################################
void CCheckComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	//## CALL Parent
	CButton::OnKillFocus(pNewWnd);

	//## HIDE Active DropDown
	HideActiveDropDown();
}
//#######################################################################################
void CCheckComboBox::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL){
		try{
			m_ToolTip.Create(this);
			m_ToolTip.Activate(FALSE);
		}catch(...){};
	}
}
//## ====================================================================================
void CCheckComboBox::SetToolTipText(int nId, BOOL bActivate)
{
	//## LOAD String
	CString strText;
	strText.LoadString(nId);

	//## SET Tooltip
	if (strText.IsEmpty() == FALSE) SetToolTipText(strText, bActivate);
}
//## ====================================================================================
void CCheckComboBox::SetToolTipText(CString strText, BOOL bActivate)
{
	//## INITIALIZE ToolTip
	InitToolTip();

	//## IF there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)strText, rectBtn, 1);
	}

	//## SET text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)strText, this, 1);
	m_ToolTip.Activate(bActivate);
}
//#######################################################################################
long CCheckComboBox::GetDroppedWidth()
{
	//## ASSERT
	if (m_nDroppedWidth != DROPPED_WIDTH_NOT_SET) return m_nDroppedWidth;
	if (!m_hWnd) return DROPPED_WIDTH_NOT_SET;

	//## GET ComboRect
	CRect rc;
	GetWindowRect(&rc);

	//## SET, RETURN DroppedWidth
	m_nDroppedWidth = rc.Width();
	return m_nDroppedWidth;
}
//## ====================================================================================
void CCheckComboBox::SetDroppedWidth(long nWidth)
{
	m_nDroppedWidth = nWidth;
}
//#######################################################################################
void CCheckComboBox::EnableSendMsgToParent(BOOL bEnable/* =TRUE */)
{
	m_bSendMsgToParent=bEnable;
}