// AlterPictureDlg.h : header file
//
#pragma once
/////////////////////////////////////////////////////////////////////////////
// CAlterPictureDlg dialog

class CAlterPictureDlg : public CDialogEx
{
// Construction
public:
	CAlterPictureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlterPictureDlg)
	enum { IDD = IDD_ALTER_PICTURE };
	CComboBox	m_ctlProcessType;
	int m_nProcessType;
	CString	m_strValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlterPictureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlterPictureDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};