// Filename:      MyRichEditCtrl.h
// Project:       MyRtf
// Module:        MyRichEditCtrl
// Programmer:    Frunza Adriana
// Version:       1.00
// Revision Date: 19-09-2001
//----------------------------------------------------------------------------
// Description:   Declaration of class CMyRichEditCtrl
//----------------------------------------------------------------------------
// Acknowledgements: function SetRtf , SetParagraphBulleted ,
//					 SetFontName and SetColour are taken from 
//					 Jeremy Iverson's CAutoRichEditCtrl - automate 
//					 rich edit formatting and RTF handling. - 
//					 www.codeproject.com
//----------------------------------------------------------------------------
// Revision history:
// 19-09-2001 - First implementation
//----------------------------------------------------------------------------
// Bugs: ........
//----------------------------------------------------------------------------
// @doc	--- codeproject.com, codeguru.com
//----------------------------------------------------------------------------
// Bugs fix:.....

#pragma once
// MyRichEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyRichEditCtrl window

//----------------------------------------------------------------------------
// @class         CMyRichEditCtrl |
//                dialog box to write text in
// @base          public | CRichEditCtrl
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// 
// 
// @revs 
// 19-09-2001 - First implementation
//----------------------------------------------------------------------------
// @ex Here's how to use this control in a dialog |
// 1. Include CMyRichEditCtrl.cpp and CMyRichEditCtrl.h in your project
// 2. In the resource editor create a rich edit control with the 
//    following styles: Multiline, Vertical scroll, AutoVScroll , No 
//    hide selection, Want return , Border 	 
// 3. Create a control member variable for this rich edit control in 
//    VC classwizard
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------


class CMyRichEditCtrl : public CRichEditCtrl
{
// Construction
// @access Public Member Functions and Variables
public:
	// @cmember
	// constructor
	CMyRichEditCtrl();

// Attributes
public:
	// @cmember
	// added on Friday 21 September 2001
	BOOL bold;
	BOOL italic;
	BOOL underlined;

	// @cmember
	// added on Saturday 22 September 2001
	CHARFORMAT cf;
	PARAFORMAT pf;

// Operations
public:
	// function added on 20 september 2001
    // @cmember function
    // Calling this will insert the file content. 
    void SetRtf(CFile* pInputFile );
	
	// function added on Friday 21 september 2001
	// @cmember , mfunc
    // set the selection bold
	void SetSelectionBold(BOOL bBold=TRUE);
	
	// function added on Friday 21 september 2001
	// @cmember,mfunc
    // set the selection italic
	void SetSelectionItalic();
	
	// function added on Friday 21 september 2001
	// @cmember,mfunc
    // set the selection underlined
	void SetSelectionUnderline();
	
	// function added on Saturday 22 september 2001
	// @cmember,mfunc
    // displays a color selection dialog box and sets the selection 
	// to the chosen color (if OK is pressed).  
	void SetColour();
	// this is for CFontDialog color box
	void SetColor(COLORREF clr);
	
	// function added on Saturday 22 september 2001
	// @cmember,mfunc
    // sets the current paragraph to be left justified
	void SetParagraphLeft();
	
	// function added on Monday 24 september 2001
	// @cmember
    // sets the current paragraph to be right justified
	void SetParagraphRight();
	
	// function added on Monday 24 september 2001
	// @cmember
    // sets the current paragraph to be centered
	void SetParagraphCenter();
	
	// function added on Wednesday(Miercuri) 26 september 2001
	// @cmember
    // sets the current paragraph to be bulleted
	void SetParagraphBulleted();

	// function added on Friday 28 september 2001
	// @cmember,mfunc
    // sets the font name
	void SetFontName(CString sFontName);

	// function added on Tuesday 2 Octomber 2001
	// @cmember,mfunc
    // sets the font size
	void SetFontSize(int nFontSize);

	//Set start indent
	//Added by ocean in 2003.11.12
	void SetParagraphStartIndent(int nStartIndent=400);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRichEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	// @cmember,mfunc
	// destructor
	virtual ~CMyRichEditCtrl();
// @access Private Member Variables
private:
	
	// @cmember
	// My callback procedure that writes the file content to the
	// rich edit control
	// function added on 20 september 2001 from VC help(MSDN)
	static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie,LPBYTE pbBuff, 
		                              LONG cb, LONG *pcb);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyRichEditCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};