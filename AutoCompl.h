////////////////////////////////////////////////////////////////
// VCKBASE -- August 2000
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
#pragma once

#include "subclass.h"

//////////////////
// Generic auto-completion object you can use to do auto-completion
// for edit and combobox controls
//
// To use:
// - instantiate one of these for each edit/combobox you want to hook
// - add some strings to the string array
// - call Init
//
class CAutoComplete : public CSubclassWnd {
protected:
	CStringArray m_arStrings;	 // list (array) of strings
	CString		 m_sPrevious;	 // previous content
	int	m_bIgnoreChangeMsg;	 // ignore EN_CHANGE message?
	UINT	m_idMyControl;			 // ID of control I am subclassing
	int	m_iType;					 // type of control (edit/combo)
	int	m_iCurString;			 // index to current string
	enum { Edit=1,ComboBox };
	
	// hook fn
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);

	// helper fns you can override but shouldn't need to
	virtual UINT GetMatches(LPCTSTR pszText, CStringArray& arMatches,
		BOOL bFirstOnly=FALSE);
	virtual void OnFirstString();
	virtual BOOL OnNextString(CString& sNext);
	virtual BOOL OnMatchString(const CString& s, const CString& sMatch);
	virtual BOOL IgnoreCompletion(CString s);
	virtual void OnComplete(CWnd* pWnd, CString s);
	virtual void DoCompletion(CWnd* pWnd, CString s,
		const CStringArray& arMatches);

	void ReadCookies(UINT nControlID);


public:
	CAutoComplete();
	~CAutoComplete();
	void Init(CWnd* pWnd);
	CStringArray& GetStringList() { return m_arStrings; }
};
