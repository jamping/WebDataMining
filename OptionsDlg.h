// OptionsDlg.h : header file
//
#pragma once

#include "OptionsData.h"
#include "OptionsDatabase.h"
#include "OptionsGeneral.h"
#include "OptionsWDMConfig.h"
#include "OptionsPubConfigDlg.h"
/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

class COptionsDlg : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(COptionsDlg)

// Construction
public:
	COptionsDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    void SetValue(COptionsData &data);
	void GetValue(COptionsData &data);
// Attributes
public:
     COptionsGeneral         m_pageGeneral;
	 COptionsDatabase        m_pageDatabaseLocal;
	 COptionsDatabase        m_pageDatabaseWDMServer;
	 COptionsWDMConfig       m_pageWDMConfig;
//	 COptionsPubConfigDlg    m_pagePubConfig;
// Operations
public:
// Implementation
public:
	virtual ~COptionsDlg();

	// Generated message map functions
protected:
// 	CStringArray    m_arrayLabel;
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};