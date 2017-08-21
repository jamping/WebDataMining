// SetReadOnlyDialog.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSetReadOnlyDialog dialog

class CSetReadOnlyDialog : public CDialogEx
{
// Construction
public:
	CSetReadOnlyDialog(BOOL bReadOnly,UINT nIDTemplate,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetReadOnlyDialog)
	enum { IDD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetReadOnlyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    BOOL    m_bReadOnly;
	// Generated message map functions
	//{{AFX_MSG(CSetReadOnlyDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};