// Regex.h: interface for the CRegx class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CRegex  
{
public:
	enum FILTER_TYPE{FRONT,BACK,SELF};

	static int Search(const CString& str,const CString& strRule,CStringArray& arrayResult);
	static CString Search(const CString&str,const CString& strRule,int& nStartPos,int& nEndPos); 
	static CString Search(const CString&str,const CString& strRule,BOOL bAll = FALSE); 
	static CString Filter(const CString& str,const CString& strRule,FILTER_TYPE type);	
    static CString Replace(const CString& str,const CString& strRule,const CString& strFomat);
	static BOOL Match(const CString& str,const CString& strRule);
};