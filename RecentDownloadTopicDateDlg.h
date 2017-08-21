// RecentDownloadTopicDateDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRecentDownloadTopicDateDlg dialog

class CRecentDownloadTopicDateDlg : public CDialogEx
{
// Construction
public:
	CRecentDownloadTopicDateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecentDownloadTopicDateDlg)
	enum { IDD = IDD_SEL_RECENT_TOPIC };
	//}}AFX_DATA
 // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecentDownloadTopicDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	
	// Generated message map functions
	//{{AFX_MSG(CRecentDownloadTopicDateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_time;
};