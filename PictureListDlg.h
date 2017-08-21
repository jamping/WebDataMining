// PictureListDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPictureListDlg dialog
class CTopic;

class CPictureListDlg : public CDialogEx
{
// Construction
public:
	CPictureListDlg(CTopic* pTopic,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPictureListDlg)
	enum { IDD = IDD_PICTURE_LIST_DLG };
	CMFCListCtrl	m_listPictures;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureListDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTopic*  m_pTopic;

	void ShowContent();
	// Generated message map functions
	//{{AFX_MSG(CPictureListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkListPicture(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelete();
	afx_msg void OnAdd();
	afx_msg void OnFilter();
	//}}AFX_MSG
	afx_msg void OnUpdateDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilter(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};