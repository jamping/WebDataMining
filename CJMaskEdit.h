// CJMaskEdit.h : header file
//
// Based on the article "A masked edit control (2)" by Jeremy Davis [jmd@jvf.co.uk]
// http://www.codeguru.com/editctrl/masked_edit2.shtml
//
// Copyright ?1998-99 Kirk W. Stowell
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This source code may be used in compiled form in any way you desire.
// Source file(s) may be redistributed unmodified by any means PROVIDING 
// they ARE NOT sold for profit without the authors expressed written 
// consent, and providing that this notice and the authors name and all 
// copyright notices remain intact. This software is by no means to be 
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit. 
// 
// If the source code is used in any commercial applications then a statement 
// along the lines of:
// 
// "Portions Copyright ?1998-99 Kirk Stowell" must be included in the startup 
// banner, "About" box or printed documentation. This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author(s) accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 7/25/99 10:00p $
 * $Revision: 4 $
 * $Archive: /CodeJockey/Include/CJMaskEdit.h $
 *
 * $History: CJMaskEdit.h $
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/Include
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 7/19/99    Time: 12:35a
 * Created in $/CodeJockey/Include
 * Initial creation and addition to library.
 * 
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#pragma once

// CCJMaskEdit is a CEdit derived class which allows text masking to be 
// applied to the control to format it for special editing restrictions.
class CCJMaskEdit : public CEdit
{
	DECLARE_DYNAMIC(CCJMaskEdit)

// Construction
public:
	CCJMaskEdit();

// Attributes
public:

	bool		m_bUseMask;				// true to use edit mask.
	bool		m_bMaskKeyInProgress;	// true when 
	CString		m_strWindowText;		// buffer that holds the actual edit text.
	CString		m_strMask;				// buffer that holds the actual edit mask value.
	CString		m_strLiteral;
	CString		m_strValid;
	CString		m_strMaskLiteral;
	CFont		m_Font;					// font used by masked edit control.

// Operations
public:

	void SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszWindowText);
	void SendChar(UINT nChar);
	bool CheckChar(UINT nChar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJMaskEdit)
	//}}AFX_VIRTUAL

	virtual bool ProcessMask(UINT nChar, int nEndPos);

// Implementation
public:
	void UpdateFont();
	virtual ~CCJMaskEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCJMaskEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// CCJDateEdit is a CCJMaskEdit derived class which is specifically 
// geared toward editing date fields.
class  CCJDateEdit : public CCJMaskEdit
{
	DECLARE_DYNAMIC(CCJDateEdit)

// Construction
public:
	CCJDateEdit();

	void SetDateTime(COleDateTime& dt);
	void SetDateTime(CString strDate);
	COleDateTime GetDateTime();
	CString GetWindowDateTime();
	COleDateTime ReadOleDateTime(LPCTSTR lpszData);

	virtual bool ProcessMask(UINT nChar, int nEndPos);
	virtual void FormatOleDateTime(CString& strData, COleDateTime dt);
};

// CCJTimeEdit is a CCJDateEdit derived class which is specifically geared 
// toward editing time fields.
class  CCJTimeEdit : public CCJDateEdit
{
	DECLARE_DYNAMIC(CCJTimeEdit)

// Construction
public:
	CCJTimeEdit();

	CString		m_strHours;
	CString		m_strMins;
	CString     m_strSecs;

	void SetHours(int nHours);
	void SetMins(int nMins);
	void SetSecs(int nSecs);

	virtual bool ProcessMask(UINT nChar, int nEndPos);
	virtual void FormatOleDateTime(CString& strData, COleDateTime dt);
};

/////////////////////////////////////////////////////////////////////////////
// DDX for date mask control

void AFXAPI DDX_OleDateTime(CDataExchange* pDX, int nIDC, CCJDateEdit& rControl, COleDateTime& rDateTime);

/////////////////////////////////////////////////////////////////////////////
