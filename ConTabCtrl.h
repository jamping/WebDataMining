// ConTabCtrl.h : header file
//
#pragma once

class CDockWindow;
/////////////////////////////////////////////////////////////////////////////
//The bitmap of tab button is added by ocean in 2002.9.6

/////////////////////////////////////////////////////////////////////////////
// CConTabCtrl window

class CConTabCtrl : public CTabCtrl
{
// Construction
	struct PageNode
	{
		CString m_strCaption;
		CDialog *m_pPage;
		LONG m_lStyle;
	};
public:
	CConTabCtrl();

// Attributes
public:
	CPtrArray m_listPages;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConTabCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL SetImageList(UINT nID,int cx,int nGrow,COLORREF crMask);
	int GetPageCount();
	LPCSTR GetPageTitle(int n);
	PageNode *RemoveNode(int n);
	PageNode *GetNode(int n);
	CDialog * GetPage(int n);
	BOOL AddPage(LPCSTR strCaption,CDialog *pPage,int nPos=-1);
	CDialog *AddPage(LPCSTR strCaption,CRuntimeClass *pPageCls,UINT nTemplate,int nPos=-1);
	CDockWindow *DockPage(int n);
	void DockPage(CDialog *pPage,int nPos=-1);
	CDialog *RemovePage(int n);
	void ActivePage(int n);
    CDialog* GetCurPage();

	virtual ~CConTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CImageList m_ImgList;
};

class CDockWindow : public CWnd
{
public:
	static CDockWindow *Create(LPCTSTR strCaption,CWnd *pParent=NULL);

protected:
	static int g_nWindows;
	static LPCTSTR g_lpszClassName;

	CConTabCtrl *m_pTab;
	int m_nTabPos;

	CDockWindow();

public:
	CDialog *m_pPage;

	~CDockWindow();	
	void SetPage(CDialog *pPage,CConTabCtrl *pTab,int nTabPos = -1);
	CDialog *GetPage();

protected:
	//{{AFX_MSG(CDlg)
	afx_msg void OnNcDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};