// CJMaskEdit.cpp : implementation file
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
 * $Date: 6/23/99 12:04a $
 * $Revision: 7 $
 * $Archive: /CodeJockey/CJLibrary/CJMaskEdit.cpp $
 *
 * $History: CJMaskEdit.cpp $
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:04a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:42a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:15a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 6/23/99    Time: 12:33a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 7/19/99    Time: 12:35a
 * Created in $/CodeJockey/CJLibrary
 * Initial creation and addition to library.
 * 
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CJMaskEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJMaskEdit

CCJMaskEdit::CCJMaskEdit()
{
	m_bUseMask				= true;
	m_bMaskKeyInProgress	= false;
	m_strWindowText			= _T("");
	m_strMask				= _T("");
	m_strLiteral			= _T("");
	m_strValid				= _T("");
	m_strMaskLiteral		= _T("");
}

CCJMaskEdit::~CCJMaskEdit()
{
}

IMPLEMENT_DYNAMIC(CCJMaskEdit, CEdit)

BEGIN_MESSAGE_MAP(CCJMaskEdit, CEdit)
	//{{AFX_MSG_MAP(CCJMaskEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJMaskEdit message handlers

void CCJMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( m_bUseMask )
	{
		switch( nChar )
		{
		case VK_DELETE:
		case VK_INSERT: return;
		}
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCJMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bMaskKeyInProgress ) {
		if( !CheckChar( nChar )) {
			return;
		}
	}
	
	if( m_bUseMask )
	{
		int nStartPos;
		int nEndPos;
		GetSel( nStartPos, nEndPos );

		if( isprint( nChar ))
		{
			SetSel( nStartPos, nEndPos+1 );
			Clear();
			m_strWindowText.SetAt( nEndPos, nChar );
		}
		else if( nChar == VK_BACK )
		{
			if(( nStartPos == nEndPos ) && 
			   ( nStartPos >= 1 ) && 
			   ( nStartPos <= m_strWindowText.GetLength( )))
			{
				UINT c;
				// get the masked literal representation.
				if( !m_strMaskLiteral.IsEmpty( )) {
					c = m_strMaskLiteral[ nStartPos-1 ];
				}

				// backspace the cursor.
				SendMessage( WM_KEYDOWN, VK_LEFT, 0 );
				if( !m_strMaskLiteral.IsEmpty( ))
				{
					// update the char backspacing over.
					SendChar( c );
					// backspace the cursor again.
					SendMessage( WM_KEYDOWN, VK_LEFT, 0 );
				}
			}
			else {
				MessageBeep((UINT)-1);
			}

			return;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	if( !m_bMaskKeyInProgress && m_bUseMask && !m_strLiteral.IsEmpty( ))
	{
		int nStartPos;
		int nEndPos;
		GetSel( nStartPos, nEndPos );

		// make sure the string is not longer than the mask
		if( nEndPos < m_strLiteral.GetLength( ))
		{
			UINT c = m_strLiteral.GetAt( nEndPos );
			if (c != '_') {
				SetSel( nEndPos+1, nEndPos+1 );
			}
		}
	}
}

bool CCJMaskEdit::CheckChar(UINT nChar)
{
	// do not use mask
	if( !m_bUseMask ) {
		return true;
	}
	
	// control character, OK
	if( !isprint( nChar )) {
		return true;
	}
	
	// unselect all selections, if any
	int nStartPos, nEndPos;
	GetSel( nStartPos, nEndPos );
	SetSel( -1, 0 );
	
	// advance to the first character input position.
	for( nStartPos; nStartPos < m_strLiteral.GetLength(); ++nStartPos )
	{
		if( m_strLiteral.GetAt( nStartPos ) == '_' ) {
			SetSel( nStartPos, nStartPos );
			break;
		}
	}
	
	// check the key against the mask
	GetSel( nStartPos, nEndPos );
	
	// make sure the string is not longer than the mask
	if( nEndPos >= m_strMask.GetLength( )) {
		MessageBeep((UINT)-1);
		return false;
	}
	
	// check to see if a literal is in this position
	UINT c = '_';
	if( !m_strLiteral.IsEmpty( )) {
		c = m_strLiteral.GetAt( nEndPos );
	}
	
	if( c != '_' ) {
		SendChar( c );
		GetSel( nStartPos, nEndPos );
	}
	
	// check the valid string character
	if( m_strValid.Find( nChar ) != -1 ) {
		return true;
	}
	
	if( ProcessMask( nChar, nEndPos ))
		return true;

	MessageBeep((UINT)-1);
	return false;
}

void CCJMaskEdit::SendChar(UINT nChar)
{
	m_bMaskKeyInProgress = true;
#ifdef WIN32
    AfxCallWndProc(this, m_hWnd, WM_CHAR, nChar, 1);
#else
    SendMessage(WM_CHAR, nChar, 1);
#endif
	m_bMaskKeyInProgress = false;
}

bool CCJMaskEdit::ProcessMask(UINT nChar, int nEndPos)
{
	// check the key against the mask
	switch( m_strMask.GetAt( nEndPos ))
	{
    case '0':		// digit only //completely changed this
		{
			if( isdigit( nChar )) {
				return true;
			}
			break;
		}
    case '9':		// digit or space
		{
			if( isdigit( nChar )) {
				return true;
			}
			if( nChar == VK_SPACE ) {
				return true;
			}
			break;
		}
    case '#':		// digit or space or '+' or '-'
		{
			if( isdigit( nChar )) {
				return true;
			}
			if( nChar == VK_SPACE || nChar == VK_ADD || nChar == VK_SUBTRACT ) {
				return true;
			}
			break;
		}
    case 'L':		// alpha only
		{
			if( isalpha( nChar )) {
				return true;
			}
			break;
		}
    case '?':		// alpha or space
		{
			if( isalpha( nChar )) {
				return true;
			}
			if( nChar == VK_SPACE ) {
				return true;
			}
			break;
		}
    case 'A':		// alpha numeric only
		{
			if( isalnum( nChar )) {
				return true;
			}
			break;
		}
    case 'a':		// alpha numeric or space
		{
			if( isalnum( nChar )) {
				return true;
			}
			if( nChar == VK_SPACE ) {
				return true;
			}
			break;
		}
    case '&':		// all print character only
		{
			if( isprint( nChar )) {
				return true;
			}
			break;
		}
    case 'H':		// hex digit
		{
			if( isxdigit( nChar )) {
				return true;
			}
			break;
		}
	case 'X':		// hex digit or space
		{
			if( isxdigit( nChar )) {
				return true;
			}
			if( nChar == VK_SPACE ) {
				return true;
			}
			break;
		}
	}

	return false;
}

void CCJMaskEdit::SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszWindowText)
{
	ASSERT(lpszMask);
	ASSERT(lpszLiteral);
	ASSERT(lpszWindowText);

	// initialize the mask for the control.
	m_strMask        = lpszMask;
	m_strLiteral     = lpszLiteral;
	m_strWindowText  = lpszWindowText;
	m_strMaskLiteral = m_strWindowText;

	// set the window text for the control.
	SetWindowText( lpszWindowText );

	// initialize the font used by the control.
	UpdateFont();
}

/////////////////////////////////////////////////////////////////////////////
// DDX for date mask control

void AFXAPI DDX_OleDateTime(CDataExchange* pDX, int nIDC, CCJDateEdit& rControl, COleDateTime& rDateTime)
{
	DDX_Control( pDX, nIDC, rControl );
	
	if( pDX->m_bSaveAndValidate ) {
		rDateTime = rControl.GetDateTime();
	}
	else {
		rControl.SetDateTime( rDateTime );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCJDateEdit class

IMPLEMENT_DYNAMIC(CCJDateEdit, CCJMaskEdit)

CCJDateEdit::CCJDateEdit()
{
	m_bUseMask		= true;
	m_strMask		= _T("0000-00-00");
	m_strLiteral	= _T("____-__-__");
}

COleDateTime CCJDateEdit::ReadOleDateTime(LPCTSTR lpszData)
{
	COleDateTime dt;
	dt.ParseDateTime(lpszData);
	return dt;
}

void CCJDateEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	strData = dt.Format("%Y-%m-%d");
}

void CCJDateEdit::SetDateTime(COleDateTime& dt)
{
	CString strText;
	FormatOleDateTime(strText, dt);
	m_strWindowText = m_strMaskLiteral = strText;
	SetWindowText(strText);
}

void CCJDateEdit::SetDateTime(CString strDate)
{
	m_strWindowText = m_strMaskLiteral = strDate;
	SetWindowText(strDate);
}

COleDateTime CCJDateEdit::GetDateTime()
{
	CString strText;
	GetWindowText(strText);
	return ReadOleDateTime(strText);
}

CString CCJDateEdit::GetWindowDateTime()
{
	CString strText;
	GetWindowText(strText);
	return strText;
}

bool CCJDateEdit::ProcessMask(UINT nChar, int nEndPos)
{
	// check the key against the mask
	switch( m_strMask.GetAt( nEndPos ))
	{
    case '0':		// digit only //completely changed this
		{
			bool bReturn = true;

			if( isdigit( nChar ))
			{
				if( nEndPos == 5 )
				{
					if( nChar > '1' ) {
						bReturn = false;
					}
				}
				else if( nEndPos == 6)
				{
					if( m_strWindowText.GetAt(5) == '1' )
					{
						if( nChar > '2' ) {
							bReturn = false;
						}
					}
				}
				else if( nEndPos == 8)
				{
					if( nChar > '3' ) {
						bReturn = false;
					}
				}
				else if( nEndPos == 9 )
				{
					if( m_strWindowText.GetAt(8) == '3' )
					{
						if( nChar > '1') {
							bReturn = false;
						}
					}
				}
				return bReturn;
			}
			break;
		}
	}

	MessageBeep((UINT)-1);
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CCJTimeEdit class

IMPLEMENT_DYNAMIC(CCJTimeEdit, CCJDateEdit)

CCJTimeEdit::CCJTimeEdit()
{
	m_bUseMask		= true;
	m_strMask		= _T("00:00:00");
	m_strLiteral	= _T("__:__:__");
	m_strHours		= _T("23");
	m_strMins		= _T("59");
	m_strSecs       =_T("59");
}

void CCJTimeEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	if( dt.m_dt == 0 ) { 
		strData = _T("00:00:00"); 
	}
	else {
		strData = dt.Format(_T("%H:%M:%S")); 
	}
}

bool CCJTimeEdit::ProcessMask(UINT nChar, int nEndPos)
{
	// check the key against the mask
	switch( m_strMask.GetAt( nEndPos ))
	{
    case '0':		// digit only //completely changed this
		{
			bool bReturn = true;

			if( isdigit( nChar ))
			{
				if( nEndPos == 0 )
				{
					if(nChar > (UINT)m_strHours[0]) {
						bReturn = false;
					}
				}
				else if( nEndPos == 1 )
				{
					if(m_strWindowText.GetAt(0) == m_strHours[0])
					{
						if( nChar > (UINT)m_strHours[1] ) {
							bReturn = false;
						}
					}
				}
				else if( nEndPos == 3 )
				{
					if( nChar > (UINT)m_strMins[0] ) {
						bReturn = false;
					}
				}
				else if( nEndPos == 4 )
				{
					if( m_strWindowText.GetAt(3) == m_strMins[0] )
					{
						if( nChar > (UINT)m_strMins[1] ) {
							bReturn = false;
						}
					}
				}
				else if(nEndPos==6)
				{
					if(nChar >(UINT)m_strSecs[0])
					{
						bReturn=false;
					}
				}
				else if(nEndPos == 7)
				{
					if( m_strWindowText.GetAt(6) == m_strSecs[0] )
					{
						if( nChar > (UINT)m_strSecs[1] ) {
							bReturn = false;
						}
					}

				}
				return bReturn;
			}
			break;
		}
	}

	MessageBeep((UINT)-1);
	return false;
}

void CCJTimeEdit::SetHours(int nHours)
{
	m_strHours.Format("%d", nHours);
}

void CCJTimeEdit::SetMins(int nMins)
{
	m_strMins.Format("%d", nMins);
}
void CCJTimeEdit::SetSecs(int nSecs)
{
	m_strMins.Format("%d", nSecs);
}
void CCJMaskEdit::UpdateFont()
{
	m_Font.DeleteObject();
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	VERIFY(m_Font.CreateFontIndirect(&ncm.lfMessageFont));
	SetFont(&m_Font);
}
