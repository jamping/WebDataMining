// EveryWeekDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEveryWeekDlg dialog

class CEveryWeekDlg : public CDialogEx
{
// Construction
public:
	CEveryWeekDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEveryWeekDlg)
	enum { IDD = IDD_EVERYWEEK_DLG };
	CSpinButtonCtrl	m_spinWeek;
	int		m_nWeeks;
	BOOL	m_bFriday;
	BOOL	m_bMonday;
	BOOL	m_bSaturday;
	BOOL	m_bSunday;
	BOOL	m_bThursday;
	BOOL	m_bTuesday;
	BOOL	m_bWednsday;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEveryWeekDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEveryWeekDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
