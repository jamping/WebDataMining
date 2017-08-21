// MyRichEditCtrl.cpp : implementation file
//
// Filename:      MyRichEditCtrl.cpp
// Project:       MyRtf
// Module:        MyRtf
// Programmer:    Frunza Adriana
// Version:       1.00
// Revision Date: 19-09-2001
//----------------------------------------------------------------------------
// Description:   Definition of class CMyRichEditCtrl
//                See CMyRichEditCtrl.h
//----------------------------------------------------------------------------
// Revision history:
// 19-09-2001 - First implementation
//----------------------------------------------------------------------------
// Bugs: ........1. D:\MyProjects\MyRtf\MyRtfDlg.cpp(200) : error C2440:
//                  '=' : cannot convert from 'unsigned long (__stdcall 
//				    CMyRtfDlg::*)(unsigned long,unsigned char *,
//					long,long *)' to 'unsigned long (__stdcall *)
//					(unsigned long,unsigned char *,long,long *)'
//				 2. D:\MyProjects\MyRtf\MyRichEditCtrl.cpp(88) : error
//                  C2724: 'MyStreamInCallback' : 'static' should not 
//                  be used on member functions defined at file scope
//				 3. Line: SetBackgroundColor(FALSE, RGB(255,0, 0));	has
//                  no effect 
//----------------------------------------------------------------------------
// @doc
//----------------------------------------------------------------------------
// Bugs fix:.....
// 1. function below taken from MyRtfDlg.cpp and put in CMyRichEditCtrl.cpp 
//    and make it static
// 2. delete keyword static from MyStreamInCallback from 
//    MyRichEditCtrl.cpp  	
// 3. Line is moved in class CMyRtfDlg::OnInitDialog() and has effect
//----------------------------------------------------------------------------


#include "stdafx.h"
#include "MyRichEditCtrl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRichEditCtrl



//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::CMyRichEditCtrl
// @mfunc   constructor
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 19-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

CMyRichEditCtrl::CMyRichEditCtrl()
{
	bold = TRUE;
	italic = TRUE;
	underlined = TRUE;

}


//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::~CMyRichEditCtrl
// @mfunc   destructor
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 19-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

CMyRichEditCtrl::~CMyRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CMyRichEditCtrl)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRichEditCtrl message handlers


// My callback procedure that writes a file content
// to the rich edit control.
// function added on 20 september 2001 from VC help(MSDN)
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::MyStreamInCallback
// @func    writes the file content to the rich edit control
// @parm    DWORD  | dwCookie | handle to an open file
// @parm    LPBYTE | pbBuff   | Pointer to a buffer to read from to.
//                              For a stream-in (read) operation, 
//								the callback function fills this 
//								buffer with data to transfer into the
//								rich edit control
// @parm    LONG   | cb       | Number of bytes to read 
// @parm    LONG   | *pcb     | Pointer to a variable that the 
//								callback function sets to the number 
//								of bytes actually read  
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 20-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

DWORD CALLBACK CMyRichEditCtrl ::MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*) dwCookie;
   ASSERT_KINDOF(CFile,pFile);
   *pcb = pFile->Read(pbBuff, cb);
   return 0;
}

// function added on 20 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetRtf
// @func    Calling this will insert the file content.
// @parm    CFile*  | pInputFile | the file to be opened
//----------------------------------------------------------------------------
// @prog 
// Jeremy Iverson
// @revs 
// 20-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetRtf(CFile* pInputFile )
{
	// Read the text in
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = MyStreamInCallback;
	es.dwCookie = (DWORD)pInputFile;
	StreamIn(SF_RTF, es);	// Do it.
}

// function added on Friday 21 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetSelectionBold
// @func    sets the current selection to be bold (Note: this will 
//			actually act like pressing the Bold toolbar button in 
//			Wordpad - if the text is not bold, it will become bold; 
//			if the text is already bold, it will remove the bold 
//			formatting; if part of the selection is bold, the entire 
//			selection will become bold; if there is not anything 
//			selected, the format is changed to bold so when the user 
//			starts typing, it will be in bold). 
// @parm    no parameters
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 21-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetSelectionBold(BOOL bBold/* =TRUE */)
{
//	cf.dwMask = CFM_BOLD;
//	if ( bold == TRUE )	
//		 cf.dwEffects |= CFE_BOLD;
//	else 
//		 cf.dwEffects &= ~CFE_BOLD;
//	SetSelectionCharFormat(cf);
//	bold = !bold;
	cf.dwMask = CFM_BOLD;
	if(bBold==TRUE)
		cf.dwEffects |= CFE_BOLD;
	else 
		cf.dwEffects &= ~CFE_BOLD;
	SetSelectionCharFormat(cf);

}
// function added on Friday 21 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetSelectionItalic
// @func    sets the current selection to be italic (Note: this will 
//			actually act like pressing the Italic toolbar button in 
//			Wordpad - if the text is not Italic, it will become Italic; 
//			if the text is already Italic, it will remove the Italic 
//			formatting; if part of the selection is Italic, the entire 
//			selection will become Italic; if there is not anything 
//			selected, the format is changed to Italic so when the user 
//			starts typing, it will be in Italic). 
// @parm    no parameters
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 21-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetSelectionItalic()
{
	cf.dwMask = CFM_ITALIC;
	if ( italic == TRUE )	
		 cf.dwEffects |= CFE_ITALIC;
	else 
		 cf.dwEffects &= ~CFE_ITALIC;
	SetSelectionCharFormat(cf);
	italic = !italic;
}


// function added on Friday 21 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetSelectionUnderline
// @func    sets the current selection to be Underlined (Note: this 
//			will actually act like pressing the Underlined toolbar 
//			button in Wordpad - if the text is not Underlined, it will 
//			become Underlined; if the text is already Underlined, it 
//			will remove the Underlined formatting; if part of the 
//			selection is Underlined, the entire selection will become 
//			Underlined; if there is not anything selected, the format
//			is changed to Underlined so when the user starts typing,
//			it will be in Underlined). 
// @parm    no parameters
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 21-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetSelectionUnderline()
{
	cf.dwMask = CFM_UNDERLINE;
	if ( underlined == TRUE )	
		 cf.dwEffects |= CFE_UNDERLINE;
	else 
		 cf.dwEffects &= ~CFE_UNDERLINE;
	SetSelectionCharFormat(cf);
	underlined = !underlined;
}
// function added on Saturday 22 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetColour
// @func    displays a color selection dialog box and sets the 
//			selection to the chosen color (if OK is pressed). 
// @parm    no parameters
// @xref    <c CMyRichEditCtrl>
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 22-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetColour()
{
	CColorDialog dlg;
	
	if (cf.dwEffects & CFE_AUTOCOLOR) 
		cf.dwEffects -= CFE_AUTOCOLOR;
	
	// Get a color from the common color dialog box
	if ( dlg.DoModal() == IDOK )
	{
		cf.crTextColor = dlg.GetColor();
	}
	cf.dwMask = CFM_COLOR;	

	SetSelectionCharFormat(cf);
}

// function added on Saturday 22 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetParagraphLeft
// @func    sets the current paragraph to be left 
// @parm    no parameters
// @xref    <c CMyRichEditCtrl>
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 22-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CMyRichEditCtrl::SetParagraphLeft()
{

	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;    
	pf.wAlignment = PFA_LEFT;
	
	SetParaFormat(pf);
}


// function added on Monday 24 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetParagraphRight
// @func    sets the current paragraph to be right 
// @parm    no parameters
// @xref    <c CMyRichEditCtrl>
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 24-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CMyRichEditCtrl::SetParagraphRight()
{

	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;    
	pf.wAlignment = PFA_RIGHT;
	
	SetParaFormat(pf);
}


// function added on Monday 24 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetParagraphCenter
// @func    Modify the paragraph format so that the text is centered. 
// @parm    no parameters
// @xref    <c CMyRichEditCtrl>
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
// 24-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CMyRichEditCtrl::SetParagraphCenter()
{

	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;    
	pf.wAlignment = PFA_CENTER;
	
	SetParaFormat(pf);
}


// function added on Wednesday 26 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetParagraphBulleted
// @func    Modify the paragraph format so that the text is bulleted. 
// @parm    no parameters
// @xref    <c CMyRichEditCtrl>
//----------------------------------------------------------------------------
// taken from www.codeproject.com 
// Jeremy Iverson
// @revs 
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------
void CMyRichEditCtrl::SetParagraphBulleted()
{
	GetParaFormat(pf);

	if ( (pf.dwMask & PFM_NUMBERING) && (pf.wNumbering == PFN_BULLET) )
	{
		pf.wNumbering = 0;
		pf.dxOffset = 0;
		pf.dxStartIndent = 0;
		pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{
		pf.wNumbering = PFN_BULLET;
		pf.dwMask = PFM_NUMBERING;
		if (pf.dxOffset == 0)
		{
			pf.dxOffset = 4;
			pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}

	SetParaFormat(pf);
}

// function added on Friday 28 september 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetFontName
// @func    Set the font 
// @parm    CString  | sFontName | the font name
//----------------------------------------------------------------------------
// taken from www.codeproject.com 
// Jeremy Iverson
// @revs 
// 28-09-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetFontName(CString sFontName)
{
    GetSelectionCharFormat(cf);

	// Set the font name.
	strcpy (cf.szFaceName, sFontName);
	cf.dwMask |=CFM_FACE;
	SetSelectionCharFormat(cf);
}

// function added on Tuesday 2 Octomber 2001 
//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetFontSize
// @func    Set the font 
// @parm    int  | nFontSize | the font size
//----------------------------------------------------------------------------
// taken from www.codeproject.com 
// Jemery Iverson
// @revs 
// 2-10-2001 - First implementation
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetFontSize(int nFontSize)
{
	GetSelectionCharFormat(cf);
	nFontSize *= 2;	// convert from to twips
	cf.yHeight = nFontSize ;
	cf.dwMask = CFM_SIZE;

	SetSelectionCharFormat ( cf );
}


void CMyRichEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	CMenu mnu;
//	mnu.LoadMenu(IDR_MENU2);
//	
//	ScreenToClient(&point);
//	ClientToScreen(&point);
//	
//	mnu.GetSubMenu(0)->TrackPopupMenu(TPM_CENTERALIGN,point.x,point.y,AfxGetMainWnd());  

	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

//----------------------------------------------------------------------------
// Function CMyRichEditCtrl::SetColor
// @func    sets the selection to the color
// @parm    COLORREF | clr | 
// @xref    <c CMyRichEditCtrl>
//----------------------------------------------------------------------------
// @prog 
// Frunza Adriana
// @revs 
//----------------------------------------------------------------------------
// @todo 
//----------------------------------------------------------------------------

void CMyRichEditCtrl::SetColor(COLORREF clr)
{
	if (cf.dwEffects & CFE_AUTOCOLOR) 
		cf.dwEffects -= CFE_AUTOCOLOR;
	
		cf.crTextColor = clr;

		cf.dwMask = CFM_COLOR;	

	SetSelectionCharFormat(cf);
}

void CMyRichEditCtrl::SetParagraphStartIndent(int nStartIndent/* =400 */)
{
	GetParaFormat(pf);
	
	pf.dwMask|=PFM_RTLPARA|PFM_OFFSET|PFM_STARTINDENT;
	pf.dxOffset=0;
	pf.dxStartIndent=nStartIndent;

	SetParaFormat(pf);

}