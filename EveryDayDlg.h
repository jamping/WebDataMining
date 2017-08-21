// EveryDayDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEveryDayDlg dialog

class CEveryDayDlg : public CDialogEx
{
// Construction
public:
	CEveryDayDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEveryDayDlg)
	enum { IDD = IDD_EVERYDAY_DLG };
	CSpinButtonCtrl	m_spinDay;
	int		m_nDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEveryDayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEveryDayDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};