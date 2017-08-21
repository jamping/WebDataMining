// Regex.cpp: implementation of the CRegex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include <sstream>
#include "Regex.h"
#include <boost/regex.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CRegex::Search(const CString& str,const CString& strRule,CStringArray& arrayResult)
{
	if( str.IsEmpty() || strRule.IsEmpty() )
        return -1;

	arrayResult.RemoveAll();

	std::string s(str);
	std::string::const_iterator start, end; 
	start = s.begin(); 
	end = s.end(); 
	
	boost::match_results<std::string::const_iterator> what; 
	boost::match_flag_type flags = boost::match_default; 
	boost::regex e(strRule,boost::regex::normal |boost::regbase::icase);
	
	while(boost::regex_search(start, end, what, e, flags)) 
	{ 
		std::string s1=std::string(what[1].first,what[1].second);
		// update search position
		start = what[0].second;      
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;	
		
		arrayResult.Add(s1.c_str());
	} 
	return arrayResult.GetSize();
}
CString CRegex::Search(const CString&str,const CString& strRule,int& nStartPos,int& nEndPos)
{
	if( str.IsEmpty() || strRule.IsEmpty() )
        return _T("");

	std::string s1;
	std::string s(str);
	std::string::const_iterator start, end; 
	start = s.begin(); 
	end = s.end(); 
	
	boost::match_results<std::string::const_iterator> what; 
	boost::match_flag_type flags = boost::match_default; 
	boost::regex e(strRule,boost::regex::normal |boost::regbase::icase);
	
	if(boost::regex_search(start, end, what, e, flags)) 
	{ 
		s1=std::string(what[1].first,what[1].second);	
		
		if(nStartPos == 0)
		{
			nStartPos = what[1].first-s.begin();
			nEndPos = what[1].second-s.begin();
		}
		else
		{
			nStartPos = (what[1].first-s.begin())<nStartPos?(what[1].first-s.begin()):nStartPos;
			nEndPos = (what[1].second-s.begin())>nEndPos?(what[1].second-s.begin()):nEndPos;
		}				
	} 
	return s1.c_str();
}  
CString CRegex::Search(const CString&str,const CString& strRule,BOOL bAll /* = FALSE */)
{
	if( str.IsEmpty() || strRule.IsEmpty() )
		return _T("");

	std::string s(str),s1,s2;
	std::string::const_iterator start, end; 
	start = s.begin(); 
	end = s.end(); 

	boost::match_results<std::string::const_iterator> what; 
	boost::match_flag_type flags = boost::match_default; 
	boost::regex e(strRule,boost::regex::normal |boost::regbase::icase);

	while(boost::regex_search(start, end, what, e, flags)) 
	{ 
		s2=std::string(what[1].first,what[1].second);
		// update search position
		start = what[0].second;      
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;	

		s1 += s2;
		if( !bAll )
			break;
	} 
	return s1.c_str();
}  
CString CRegex::Filter(const CString& str,const CString& strRule,FILTER_TYPE type)
{
	if(str.IsEmpty())
		return _T("");

	std::string s(str);
	std::string s1=s;

	std::string::const_iterator start, end; 
	start = s.begin(); 
	end = s.end(); 

	boost::match_results<std::string::const_iterator> what; 
	boost::match_flag_type flags = boost::match_default; 
	boost::regex e(strRule,boost::regex::normal |boost::regbase::icase);
	std::string::size_type idx=0;

	while(boost::regex_search(start, end, what, e, flags)) 
	{
		std::string s2(what[0].first,what[0].second);
		idx=s1.find(s2,idx);
		if(idx!=std::string::npos)
		{
			switch(type)
			{
			case FRONT:
				s1.erase(0,idx+s2.length());
				break;
			case BACK:
				s1.erase(idx,s1.length()-idx);
				break;
			case SELF:
				s1.erase(idx,s2.length());
				break;
			}
		}	
		// update search position
		start = what[0].second;      
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;		
	}
	return s1.c_str();	
}
BOOL CRegex::Match(const CString& str,const CString& strRule)
{	
	boost::regex e(strRule);
	std::string s(str);

	return boost::regex_match(s,e);
}
CString CRegex::Replace(const CString& str,const CString& strRule,const CString& strFomat)
{
	if( str.IsEmpty())
		return _T("");

	boost::regex e(strRule);

	std::string in(str);
	std::ostringstream t(std::ios::out | std::ios::binary);
	std::ostream_iterator<char, char> out(t);

	boost::regex_replace(out, in.begin(), in.end(),
		e, (const char*)strFomat, boost::match_default | boost::format_all);

	std::string os(t.str());

	return os.c_str();
}