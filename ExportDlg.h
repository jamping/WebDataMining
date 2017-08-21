// ExportDlg.h : header file
//
#pragma once

enum EXPORT_FILE_TYPE{ EXPORT_ALL=-1,EXPORT_XML,EXPORT_TXT,EXPORT_LANWAN};
/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog

class CExportDlg : public CDialogEx
{
// Construction
public:
	CExportDlg(EXPORT_FILE_TYPE nSingleType=EXPORT_ALL,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportDlg)
	enum { IDD = IDD_EXPORT };
	CString	m_strFileDir;
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    EXPORT_FILE_TYPE m_nSingleType;
	// Generated message map functions
	//{{AFX_MSG(CExportDlg)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};