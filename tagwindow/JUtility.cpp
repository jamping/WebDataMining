#include "..\StdAfx.h"
#include "JUtility.h"
#include <string>
#include <iterator>
#include <boost/regex.hpp>
#include <boost/regex/mfc.hpp>
#include "../ChineseCodeLib.h"
#include "../Extract.h"

using namespace boost;

BOOL CJUtility::IsTableNode(TidyNode e)
{
	// {"TR", "TD"};
	if( tidyNodeIsTR(e) || tidyNodeIsTD(e) )
		return TRUE;
	else
		return FALSE;
}
BOOL CJUtility::IsLargeNode(TidyNode e)
{
	// {"TABLE","DIV" };
	if( tidyNodeIsTABLE(e) || tidyNodeIsDIV(e) )
		return TRUE;
	else
		return FALSE;
}
BOOL CJUtility::IsInfoNode(TidyNode e)
{
	// {"P", "SPAN", "H1", "H2", "B", "I"}
	if( tidyNodeIsP(e) || 
		tidyNodeIsSPAN(e) || 
		tidyNodeIsH1(e) || 
		tidyNodeIsH2(e) ||
		tidyNodeIsB(e) ||
		tidyNodeIsI(e))
		return TRUE;
	else
		return FALSE;
}
BOOL CJUtility::IsHeading(TidyNode e)
{
	// {"TITLE", "H1", "H2", "H3", "H4", "H5", "H6", "H7"}
	if( tidyNodeIsTITLE(e) || tidyNodeIsHeader(e) )
		return TRUE;
	else
		return FALSE;
}
BOOL CJUtility::IsInvalidElement(TidyNode e)
{
	// {"STYLE","COMMENT", "SCRIPT", "OPTION","IFRAME"};
	if( tidyNodeIsSTYLE(e) || 
		tidyNodeGetType(e) == TidyNode_Comment ||
		tidyNodeIsSCRIPT(e) || 
		tidyNodeIsOPTION(e) || 
		tidyNodeIsIFRAME(e))
		return TRUE;
	else
		return FALSE;	
}
BOOL CJUtility::IsLinkNode(TidyNode e)
{
	 return tidyNodeIsA(e); 
}
BOOL CJUtility::IsImgNode(TidyNode e)
{
	return tidyNodeIsIMG(e);
}
BOOL CJUtility::IsNeedWarp(TidyNode e)
{
	if (IsHeading(e) || tidyNodeIsP(e) || IsTableNode(e) || IsLargeNode(e))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CJUtility::IsNeedSpace(TidyNode e)
{
	// {"BR", "SPAN"};
	if( tidyNodeIsBR(e) || tidyNodeIsSPAN(e))
		return TRUE;
	else
		return FALSE;	
}
BOOL CJUtility::IsAppearanceTag(TidyNode e)
{
	// heading title
	if (IsHeading(e)) 
	{
		return TRUE;
	}
	// colored fonts
	if ( tidyNodeIsFONT(e) )
	{
		TidyAttr a = tidyAttrGetById(e,TidyAttr_COLOR);
		ctmbstr v = tidyAttrValue(a);
		if( v && _tcsicmp(v,_T("")) !=0 )
			return TRUE;
	}
	// stronged texts
    if( tidyNodeIsB(e) || tidyNodeIsI(e) || tidyNodeIsSTRONG(e))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CJUtility::IsContainsInput(TidyNode e)
{
	CList<TidyNode,TidyNode> list;
	if( GetElementsByTagName(e,_T("input"),list) > 0)
	{
		BOOL bAllHidden = TRUE;
		POSITION pos = list.GetHeadPosition();
		while( pos )
		{
			TidyNode e1 = list.GetNext(pos);
			
			TidyAttr a = tidyAttrGetById(e,TidyAttr_TYPE);
			ctmbstr v = tidyAttrValue(a);
			if( v  && _tcsicmp(v,_T("hidden"))!=0 )
			{
				bAllHidden = FALSE;
				break;
			}
		}
		return !bAllHidden;
	}
	return FALSE;
}
BOOL CJUtility::IsContainsNoise(const CString& strText)
{
	 return ( boost::regex_match(strText,tregex(_T("copyright"),boost::regex::icase|boost::regex::perl)) ||
			  boost::regex_match(strText,tregex(_T("all rights reserved"),boost::regex::icase|boost::regex::perl)) ||
			  boost::regex_match(strText,tregex(_T("版权所有"),boost::regex::icase|boost::regex::perl)) ||
			  boost::regex_match(strText,tregex(_T("\\u00A9"),boost::regex::icase|boost::regex::perl)) ||
			  boost::regex_match(strText,tregex(_T("ICP备"),boost::regex::icase|boost::regex::perl)));
}
// 当前节点下有多少超链接节点？
int CJUtility::NumLinkNode(TidyNode e)
{
	int num = IsLinkNode(e) ? 1 : 0;
	for(TidyNode child = tidyGetChild(e);child;child = tidyGetNext(child))
	{
		if( tidyNodeIsElement(child))
		    num += NumLinkNode(child);		
	}
	return num;
}

int CJUtility::GetElementsByTagName(TidyNode e,const CString& strTagName,CList<TidyNode,TidyNode>& list,BOOL bSelf)
{
	// checking self first
	ctmbstr name = tidyNodeGetName(e);
	if(bSelf && name && strTagName.CompareNoCase(name) == 0 )
		list.AddTail(e);

	for(TidyNode child = tidyGetChild(e);child;child = tidyGetNext(child))
	{
		name = tidyNodeGetName(child);
		if( name && strTagName.CompareNoCase(name) == 0 )
			list.AddTail(child);
		// checking child of child
		GetElementsByTagName(child,strTagName,list,FALSE);
	}
	return list.GetCount();
}
CString CJUtility::ReplaceAll(const CString& strText,const CString& strRegex,const CString& strReplacement)
{
	return boost::regex_replace(strText, tregex(strRegex,boost::regex::icase|boost::regex::perl),strReplacement,boost::match_default | boost::format_all);
}

void CJUtility::FilterInvalidElements( CString& strText)
{
	strText = ReplaceAll(strText,_T("(?is)<!DOCTYPE.*?>"), _T(""));
	strText = ReplaceAll(strText,_T("(?is)<!--.*?-->"), _T(""));
	strText = ReplaceAll(strText,_T("(?is)<script.*?>.*?</script>"), _T(""));
	strText = ReplaceAll(strText,_T("(?is)<style.*?>.*?</style>"), _T(""));
	strText = ReplaceAll(strText,_T("(?is)<iframe.*?>.*?</iframe>"), _T(""));
	strText = ReplaceAll(strText,_T("&.{2,5};|&#.{2,5};"), _T(""));

	strText = strText.Trim();
}

CString CJUtility::GetNodeText(TidyDoc doc,TidyNode node,BOOL bOnlyContent /* = TRUE */)
{
	TidyBuffer buf;
	tidyBufInit(&buf);
	tidyNodeGetText(doc,node,&buf);

	std::string strText((LPCSTR)(LPSTR)buf.bp);

	tidyBufFree(&buf);

	if( bOnlyContent )
	{
		CExtract e;
	    e.GetPureText(strText,false);
	}
	return strText.c_str();
}

TidyNode CJUtility::RemoveChild(TidyNode child)
{
	return tidyNodeRemove(child);
}
TidyNode CJUtility::DiscardChild(TidyDoc doc,TidyNode child)
{
	return tidyNodeDiscard(doc,child);
}
