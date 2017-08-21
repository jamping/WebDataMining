// OptionsWDMConfig.h : header file
//
#pragma once
/////////////////////////////////////////////////////////////////////////////
// COptionsWDMConfig dialog

class COptionsWDMConfig : public CMFCPropertyPage
{
	DECLARE_DYNCREATE(COptionsWDMConfig)

// Construction
public:
	COptionsWDMConfig();
	~COptionsWDMConfig();

// Dialog Data
	//{{AFX_DATA(COptionsWDMConfig)
	enum { IDD = IDD_OPTIONS_WDMCONFIG };
	int		m_nTopicSaveDays;
	int		m_nPictureHeight;
	int		m_nPictureWidth;
	int		m_nTopicTopTime;
	int		m_nTopicKeyWord;
	CString	m_strTopicModifyPath;
	CString	m_strTopicViewPath;
	int		m_nTopicTopKeyWord;
	int		m_nPicMaxSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsWDMConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsWDMConfig)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nInterpolation;
};