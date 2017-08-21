#pragma once

/////////////////////////////////////////////////////////////////////////////
// ExpandingDialog.h : header file
//
// MFC Expanding/Contracting Dialog header file
//
// Written by Daniel G. Hyams
//		dhyams@ebicom.net or dhyams@altavista.net
//
// Copyright (c) 1998.
//
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any  commercial application then a statement along the lines of 
// "Portions copyright (c) Daniel G. Hyams, 1998" must be included in 
// the startup banner, "About" box or printed documentation. An email 
// letting me know that you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CExpandingDialog dialog

class CExpandingDialog : public CDialogEx
{
// Construction and destruction
public:
	CExpandingDialog(
		UINT nIDTemplate,
		CWnd* pParent,
		int nIDFrame,
		int nIDButton,
		LPCTSTR strExpand   = _T("Advanced >>"),
		LPCTSTR strContract = _T("Advanced <<"),
		BOOL bAllowContract = TRUE
		);
		
	virtual ~CExpandingDialog();

public:
	// a public function so that the dialog can query whether or not
	// we are in "advanced" mode.
	BOOL IsExpanded() const {return m_bExpanded;}
	
	// allow the user to expand or contract the dialog whenever they
	// please.  This is only needed in extreme circumstances, when the
	// dialog should expand or contract based on something else besides
	// the "Advanced" button.
	BOOL Expand(BOOL bExpand);

	// a virtual notification function so that the expanding or contracting
	// can be aborted if need be.  The function should return TRUE if the
	// expanding should happen, or FALSE if it should not.
	virtual BOOL OnDialogExpanding(BOOL bExpanded);

	// a virtual notification function that is called after the expansion
	// takes place
	virtual void OnDialogExpanded(BOOL bExpanded);

// Dialog Data
	//{{AFX_DATA(CExpandingDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpandingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	



// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpandingDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

private:
	// property data
	CString m_strExpand;	// string to put on the expand/contract button, when
							// the dialog is currently contracted.
	CString m_strContract;	// string to pu on the expand/contract button, when
							// the dialog is currently expanded.
	int m_nIDFrame;			// resource ID of the frame that defines the contracted
							// dimensions of the dialog.
	int m_nIDButton;		// resource ID of the expand/contract button on the 
							// dialog
	BOOL m_bAllowContract;	// whether or not to allow the user to contract the dialog
							// after expanding it for the first time.

	// state data
	CSize * m_pSize;		// records the original size of the dialog before 
							// contraction
	BOOL m_bExpanded;		// records whether the dialog is currently expanded or not.

private:
	void ExpandBox(BOOL fExpand);
	void OnClickAdvanced();

};