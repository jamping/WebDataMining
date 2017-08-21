// EveryMonthDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEveryMonthDlg dialog

class CEveryMonthDlg : public CDialogEx
{
// Construction
public:
	CEveryMonthDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEveryMonthDlg)
	enum { IDD = IDD_EVERYMONTH_DLG };
	CSpinButtonCtrl	m_spinMonth2;
	CSpinButtonCtrl	m_spinMonth;
	CSpinButtonCtrl	m_spinDay;
	int		m_nEvery;
	int		m_nDay;
	int		m_nMonth;
	int		m_nMonth2;
	int		m_nComboDay;
	int		m_nComboNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEveryMonthDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEveryMonthDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEvery();
	afx_msg void OnEvery2();
	//}}AFX_MSG
	afx_msg void OnUpdateMonth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMonth2(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};