////////////////////////////////////////////////////////////////
// VCKBASE -- August 2000
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
#include "stdafx.h"
#include "autocompl.h"

//////////////////
// ctor: initialize stuff to zero
//
CAutoComplete::CAutoComplete()
{
   m_bIgnoreChangeMsg=0;
   m_iType = 0;
   m_idMyControl = 0;
   m_iCurString = 0;   
}

CAutoComplete::~CAutoComplete()
{
	m_arStrings.RemoveAll();
}

//////////////////
// Install hook. Initialize control ID and type of control based on
// classname
//
void CAutoComplete::Init(CWnd* pWnd)
{
   CSubclassWnd::HookWindow(pWnd->GetParent());
   CString sClassName;
   ::GetClassName(pWnd->GetSafeHwnd(), sClassName.GetBuffer(32), 32);
   sClassName.ReleaseBuffer();
   if (sClassName=="Edit") 
   {
      m_iType = Edit;

   } else if (sClassName=="ComboBox") {
      m_iType = ComboBox;
   }
   m_idMyControl = pWnd->GetDlgCtrlID();
   //Read cookies
   ReadCookies(m_idMyControl);
}

//////////////////
// Scan string array for strings that match text, and add the matches
// to a new array. Returns number of matches. For edit controls, only
// need to find the first match, so to be efficient BOOL arg tells me
// that.
//
UINT CAutoComplete::GetMatches(LPCTSTR pszText, CStringArray& arMatches,
   BOOL bFirstOnly)
{
   arMatches.RemoveAll();
   int nMatch = 0;
   CString s=pszText;
   if (s.GetLength()>0) {
      OnFirstString();
      CString sMatch;
      while (OnNextString(sMatch)) {
         if (OnMatchString(s, sMatch)) {
            TRACE("Add %s\n",(LPCTSTR)sMatch);
            arMatches.Add(sMatch);
            nMatch++;
            if (bFirstOnly)
               break;
         }
      }
   }
   return nMatch;
}

//////////////////
// This virtual function takes the string entered and a potential match
// and returns TRUE if the strings match. Default implementation does a
// normal prefix compare--but you could override, for example to ignore
// 'www' at the start of either string.
//
BOOL CAutoComplete::OnMatchString(const CString& s, const CString& sMatch)
{
   return s==sMatch.Left(s.GetLength());
}

void CAutoComplete::OnFirstString()
{
   m_iCurString=0;
}

BOOL CAutoComplete::OnNextString(CString& sNext)
{
   if (m_iCurString < m_arStrings.GetSize()) {
      sNext = m_arStrings[m_iCurString++];
      return TRUE;
   }
   sNext = (LPCTSTR)NULL;
   return FALSE;
}

//////////////////
// "hook" function traps messages sent to edit control/combobox. I am
// interested in EN_CHANGE or CBN_EDITCHANGE: contents of edit control
// have changed.
//
LRESULT CAutoComplete::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
   if ((msg==WM_COMMAND && LOWORD(wp)==m_idMyControl) &&
       ((m_iType==Edit   && HIWORD(wp)==EN_CHANGE) ||
        (m_iType==ComboBox && HIWORD(wp)==CBN_EDITCHANGE))) {

      // since I will be changing the contents of the control, which
      // will trigger more EN_CHANGE messages, turn off processing
      // while I have control, using m_bIgnoreChangeMsg.
      if (!m_bIgnoreChangeMsg++) {
         CString s;
         CWnd* pWnd = CWnd::FromHandle((HWND)lp);
         pWnd->GetWindowText(s);
         OnComplete(pWnd, s);
      }
      m_bIgnoreChangeMsg--;
   }   
   if ((msg==WM_COMMAND && LOWORD(wp)==m_idMyControl) &&
       ((m_iType==Edit   && HIWORD(wp)==EN_KILLFOCUS) ||
        (m_iType==ComboBox && HIWORD(wp)==CBN_KILLFOCUS))) 
   {
	   //save data to memory
	   CString s;
	   CWnd* pWnd = CWnd::FromHandle((HWND)lp);
	   pWnd->GetWindowText(s);

	   if(!s.IsEmpty())
	   {
		   CString str;
		   BOOL bFound=FALSE;
			   for(int i=0;i<GetStringList().GetSize();i++)
		   {
			   if(s.Compare(GetStringList().GetAt(i))==0)	
			   {
				   bFound=TRUE;
				   break;
			   }
		   }
		   if(bFound==FALSE)
			   	GetStringList().Add(s);
	   }
   }
   
   if(msg==WM_DESTROY)
   {	   
	   CStdioFile file; 	   
	   if(file.Open(CCommonFunction::GetFileFullPath(_T("cookies.dat")),CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate))
	   {
		   file.SeekToBegin();

		   for(int i=0;i<GetStringList().GetSize();i++)
		      file.WriteString(GetStringList().GetAt(i)+"\n");
		   
		   file.Close();	   
	   }	   
   }
   return CSubclassWnd::WindowProc(msg, wp, lp);
}

//////////////////
// This is the main function that does the completion.
//
void CAutoComplete::OnComplete(CWnd* pWnd, CString s)
{
   CStringArray arMatches; // strings that match
   if (s.GetLength()>0 && GetMatches(s, arMatches, m_iType==Edit)>0) {
      DoCompletion(pWnd, s, arMatches);
   }
   m_sPrevious=s; // remember current string
}

void CAutoComplete::DoCompletion(CWnd* pWnd, CString s,
   const CStringArray& arMatches)
{
   if (m_iType==ComboBox) {
      // This cast is technically incorrect, but a standard MFC trick.
      CComboBox* pComboBox = (CComboBox*)pWnd;

      // update dropdown to reflect possible matches
      pComboBox->ResetContent();
      for (int i=0; i<arMatches.GetSize(); i++) {
         pComboBox->AddString(arMatches[i]);
      }
      // user arrow cursor so user can select
      ::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW))); 
      // show dropdown
      pComboBox->ShowDropDown();
      pComboBox->SetWindowText(IgnoreCompletion(s) ? s : arMatches[0]);
      pComboBox->SetEditSel(s.GetLength(),-1);

   } else if (m_iType==Edit && !IgnoreCompletion(s)) {
      // This cast is technically incorrect, but a standard MFC trick. 
      CEdit* pEdit = (CEdit*)pWnd;
	  
      pEdit->SetWindowText(arMatches[0]);
      pEdit->SetSel(s.GetLength(),-1);
   }
}

//////////////////
// This function is used to turn off the completion feature when the
// user presses Backspace to delete a character typed. In that case,
// the current string (s) will match the last (previously) entered
// string. If this is the case, don't do the completion. For example,
// if user types 'foo' and this causes me to complete to 'foobar', with
// 'bar' highlighted, I don't want to complete to foobar again if the
// user presses Backspace or Delete to delete 'bar'. Instead, 'foo'
// should remain--and likewise if the user keeps pressing backspace.
// This is one of the only functions I have ever written where the
// explanation is longer than the code itself!
//
BOOL CAutoComplete::IgnoreCompletion(CString s)
{
   return s==m_sPrevious.Left(s.GetLength());
}
void CAutoComplete::ReadCookies(UINT nControlID)
{
	//read data from file
	CStdioFile file;
	
	if(file.Open(CCommonFunction::GetFileFullPath(_T("cookies.dat")),CFile::modeRead))
	{
		CString strWord;
		while(file.ReadString(strWord))
		{
			GetStringList().Add(strWord);
		}
		file.Close();
	}
}

