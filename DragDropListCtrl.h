// DragDropListCtrl.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDragDropListCtrl window

class CDragDropListCtrl : public CListCtrl
{
protected:
	CDWordArray			m_anDragIndexes;
	int					m_nDropIndex;
	CImageList*			m_pDragImage;
	int					m_nPrevDropIndex;
	UINT				m_uPrevDropState;
	DWORD				m_dwStyle;

	enum EScrollDirection
	{
		scrollNull,
		scrollUp,
		scrollDown
	};
	EScrollDirection	m_ScrollDirection;
	UINT				m_uScrollTimer;

// Construction
public:
	CDragDropListCtrl();
	bool IsDragging() const { return m_pDragImage != NULL; }

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragDropListCtrl();

protected:
	void DropItem();
	void RestorePrevDropItemState();
	void UpdateSelection(int nDropIndex);
	void SetScrollTimer(EScrollDirection ScrollDirection);
	void KillScrollTimer();
	CImageList* CreateDragImageEx(LPPOINT lpPoint);
	
// Generated message map functions
protected:
	//{{AFX_MSG(CDragDropListCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};