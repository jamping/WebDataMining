// PictureDlg.h : header file
//
#pragma once

#include "NewsPicture.h"

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg dialog

class CPictureDlg : public CDialogEx
{
// Construction
public:
	CPictureDlg(CNewsPicture* pPicture,CWnd* pParent = NULL);   // standard constructor
    ~CPictureDlg();

	void SetValue(CNewsPicture::NewsPictureData& data);
	BOOL GetValue(CNewsPicture::NewsPictureData& data);
// Dialog Data
	//{{AFX_DATA(CPictureDlg)
	enum { IDD = IDD_PICTURE_DLG };
	CStatic	m_Preview;
	CString	m_strExplain;
	CString	m_strURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CNewsPicture* m_pPicture;	
//	FCImage pic;
	CxImage pic;
	BOOL        m_bAnotherPicture;
	CString     m_strOldURL;
//  HBRUSH		m_BkBrush ;
	// Generated message map functions
	//{{AFX_MSG(CPictureDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};