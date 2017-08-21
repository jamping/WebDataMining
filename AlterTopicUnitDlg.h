// AlterTopicUnitDlg.h : header file
//
#pragma once
/////////////////////////////////////////////////////////////////////////////
// CAlterTopicUnitDlg dialog

class CAlterTopicUnitDlg : public CDialogEx
{
// Construction
public:
	CAlterTopicUnitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlterTopicUnitDlg)
	enum { IDD = IDD_ALTER_TOPIC };
	CComboBox m_ctlTopicUnit;
	CComboBox m_ctlProcessType;
	int	m_nTopicUnit;
	int	m_nProcessType;
	CString	m_strValue;
	//}}AFX_DATA  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlterTopicUnitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlterTopicUnitDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};