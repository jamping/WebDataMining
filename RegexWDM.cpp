// RegexWDM.cpp: implementation of the CRegexWDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "RegexWDM.h"
#include "NewsPicture.h"
#include "Url.h"
#include <boost/regex.hpp>
#include "Regex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegexWDM::CRegexWDM()
{
	m_nStartPos=0;
	m_nEndPos=0;
}
CRegexWDM::CRegexWDM(CRule* pRule): m_pRule(pRule)
{
	m_nStartPos=0;
	m_nEndPos=0;
}
CRegexWDM::~CRegexWDM()
{

}

BOOL CRegexWDM::FetchTopic(const CString& strBuf,CTopic** pTopic,CString& strResultHtml)
{
    if(strBuf.IsEmpty())
		return FALSE;
	else

	if(m_pRule == NULL)
		return FALSE;

 	m_nStartPos=0;
	m_nEndPos=0;
	//Fetch the topic content
	POSITION pos=m_pRule->GetFirstChildPosition();
	while (pos)
	{
		CRuleTree* pRuleTree=(CRuleTree*)m_pRule->GetNextChild(pos);
		if(pRuleTree!=NULL && pRuleTree->IsValid())
		{
			CString strFetchContent=FetchContentByRule(strBuf,pRuleTree);
	
			strFetchContent = CCommonFunction::GetPureText(strFetchContent,pRuleTree->m_data.m_nSaveType == 3);
			strFetchContent = CRegex::Filter(strFetchContent,_T("\\?+"),CRegex::SELF);		

			ASSERT(pRuleTree->m_data.m_nProcessType==0);

			switch(pRuleTree->m_data.m_nSaveType)
			{
			case 0:       //Head Title
				(*pTopic)->m_data.m_strHeadTitle = strFetchContent.Left(300);
				break;
			case 1:       //Title
				strFetchContent = CRegex::Replace(strFetchContent,_T("(\")|(\")"),_T("(?1“)(?2”)"));
                strFetchContent = CRegex::Filter(strFetchContent,_T("\""),CRegex::SELF);
				strFetchContent = CRegex::Filter(strFetchContent,_T("_"),CRegex::BACK);
				(*pTopic)->m_data.m_strTitle=CRegex::Filter(strFetchContent,_T("[(（]图[)）]"),CRegex::SELF).Left(100);
				break;
			case 2:       //Sub Title
				(*pTopic)->m_data.m_strSubTitle=strFetchContent.Left(300);
				break;
			case 3:       //Content
				(*pTopic)->m_data.m_strContent=strFetchContent;				
				break;
			case 4:       //Time				
				(*pTopic)->m_data.m_strTime=strFetchContent;
				break;
			case 5:       //Author
				(*pTopic)->m_data.m_strAuthor=strFetchContent.Left(50);
				break;
			case 6:       //Source
				(*pTopic)->m_data.m_strSource=strFetchContent.Left(100);
				break;
			}			
		}		
	}	 
    //Fetch the pictures of the topic
	strResultHtml = CCommonFunction::GetHtmlText(strBuf.Mid(m_nStartPos,m_nEndPos-m_nStartPos+1));

	if((*pTopic)->m_data.m_strSource.IsEmpty())
		(*pTopic)->m_data.m_strSource=GetRule()->GetDesc();

	return !(*pTopic)->m_data.m_strTitle.IsEmpty();
}


CString CRegexWDM::FetchContentByRule(const CString& strBuf,CRuleTree* pRuleTree)
{
	CString strFetchContent;	
	// pRuleTree->m_data.m_nSaveType == 3 is Content
	switch(pRuleTree->m_data.m_nProcessType)
	{
	case 0:
		strFetchContent=(pRuleTree->m_data.m_nSaveType == 3)?CRegex::Search(strBuf,pRuleTree->m_data.m_strMatchRule,m_nStartPos,m_nEndPos):CRegex::Search(strBuf,pRuleTree->m_data.m_strMatchRule);
		break;
	case 1:
		strFetchContent=CRegex::Filter(strBuf,pRuleTree->m_data.m_strMatchRule,CRegex::FRONT);
		break;
	case 2:
		strFetchContent=CRegex::Filter(strBuf,pRuleTree->m_data.m_strMatchRule,CRegex::BACK);
		break;
	case 3:
		strFetchContent=CRegex::Filter(strBuf,pRuleTree->m_data.m_strMatchRule,CRegex::SELF);
		break;
	}
//	strFetchContent = CCommonFunction::GetPureText(strFetchContent,pRuleTree->m_data.m_nSaveType == 3);
	//Process sub rules 
	POSITION pos =pRuleTree->GetFirstChildPosition();
	while (pos)
	{
		CRuleTree* pChildRuleTree=(CRuleTree*)pRuleTree->GetNextChild(pos);
		if(pChildRuleTree!=NULL && pChildRuleTree->IsValid())
		{
			strFetchContent=FetchContentByRule(strFetchContent,pChildRuleTree);
		}		
	}
	return strFetchContent;
}
BOOL CRegexWDM::IsValidMorePage(const HREF* pHref)
{
	//////////////////////////////////////////////////////////////////////////
	
	const char* ruleText = "\\d{1,2}|"
		                   "\\[\\s*\\d{1,2}\\s*\\]|"
				           "\\(\\s*\\d{1,2}\\s*\\)|"
				           "【\\s*\\d{1,2}\\s*】";

	return CRegex::Match(pHref->m_strContent,ruleText);
	//////////////////////////////////////////////////////////////////////////
}


CString CRegexWDM::TimeFormatStrConvert(const CString& strURL)
{
	//解释URL
	//http://www.chinamil.zz/site1/xwpdxw/jrywxw/2008-01/04/jrywxw.htm
	//http://www.chinamil.zz/site1/xwpdxw/jrywxw/##4y-2m/2d##/jrywxw.htm
	CString str,strRet(strURL);

	str = CRegex::Search(strURL,_T("(##.*?##)"));
	if(!str.IsEmpty())
	{
		CString strYear,strMonth,strDay;
		CString strYear1,strMonth1,strDay1;			
		int nY=0,nM=0,nD=0;
		
		CString strNew=str.Mid(2,str.GetLength()-4);
		COleDateTime time=COleDateTime::GetCurrentTime();
		
		strYear = CRegex::Search(str,_T("(\\d?y)"));
		if(!strYear.IsEmpty())
		{
			if(strYear.GetLength()>1)
				nY=_ttoi(strYear.Left(1));
			if(nY==2)
				strYear1.Format("%s",time.Format("%Y").Right(2));
			else
				strYear1.Format("%s",time.Format("%Y"));
			
			CCommonFunction::Replace(strNew,strYear,strYear1);
		}
		strMonth = CRegex::Search(str,_T("(\\d?m)"));
		if(!strMonth.IsEmpty())
		{
			if(strMonth.GetLength()>1)
				nM=_ttoi(strMonth.Left(1));	
			if(nM==1)
				strMonth1.Format("%d",time.GetMonth());
			else
				strMonth1.Format("%02d",time.GetMonth());
			
			CCommonFunction::Replace(strNew,strMonth,strMonth1);
		}
		strDay = CRegex::Search(str,_T("(\\d?d)"));
		if(!strDay.IsEmpty())
		{
			if(strDay.GetLength()>1)
				nD=_ttoi(strDay.Left(1));
			if(nD==1)
				strDay1.Format("%d",time.GetDay());
			else
				strDay1.Format("%02d",time.GetDay());
			
			CCommonFunction::Replace(strNew,strDay,strDay1);
		}
		CCommonFunction::Replace(strRet,str,strNew);
	}
	return strRet;
}
CString CRegexWDM::GetWebPageBaseURL(const CString& strBuf)
{
	return CRegex::Search(strBuf,_T("<\\s*BASE\\s+[^>]*?href\\s*=\\s*[\"\']?\\s*(.*?)[\"\'> ].*?>"));
}

int CRegexWDM::FetchPictures(const CString& strBuf,CTopic** pTopic,const CString& strBaseUrl)
{
	//  picture href e.g
	//  	   <IMG style="MARGIN-BOTTOM: 5px" alt=科技时代_巴西航空工业公司E-190飞机亮相中国(组图) 
	//  	   src="巴西航空工业公司E-190飞机亮相中国(组图)_科学探索_科技时代_新浪网.files/U1235P2T1D1185799F13DT20061016150308.jpg" 
	//  	 	 	border=1><BR><FONT class=title12>巴西航空工业公司中国区总裁关东元(陈诚/摄)<BR>
	//         <img src="http://pic0.v4.cc/ps/mmbiz.qpic.cn/mmbiz_jpg/S7lKogFiaSHoDgJwOfYUVicuN8BRe6bo6zPdhg556MdZ0ZH9lwPfoHg0pQwFsmzhiblxRAibURyxQicPfEiaMacibywicw/0?wx_fmt=jpeg">
	////////////////////////////////////////////////////////////////////////// 	
	const char* rule_text=_T("<\\s*IMG\\s+[^>]*?src\\s*=\\s*[\"\']?\\s*(.*?)[\"\'> ].*?>");	
	boost::regex e(rule_text,boost::regex::normal | boost::regbase::icase);			

	std::string s(strBuf);	
	std::string::const_iterator start, end;
	start = s.begin();
	end = s.end();   
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	CNewsPicture* pPicture=NULL;
	CArray<CNewsPicture*,CNewsPicture*> arrayPictures;

	CUrl url;
	url.ParseUrlEx(strBaseUrl);

	while(boost::regex_search(start, end, what, e, flags))   
	{
		CString strImageURL=std::string(what[1].first,what[1].second).c_str();		

		if(CUrl::IsImageUrl(strImageURL))
		{
			pPicture=new CNewsPicture;
			pPicture->SetID(CCommonFunction::AllocateID());			
			//解释地址
			pPicture->m_data.m_strURL=url.MakeURL(strImageURL);
			//替换为新地址
			CCommonFunction::Replace((*pTopic)->m_data.m_strContent,strImageURL,pPicture->m_data.m_strURL);
			//保存相对位置
			pPicture->m_data.m_nStartPos=what[0].first-s.begin();
			pPicture->m_data.m_nEndPos=what[0].second-s.begin();
			//Add to the topic 
			pPicture->AddTo(*pTopic);
			arrayPictures.Add(pPicture);
		} 
		// update search position
		start = what[0].second;      
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;		
	}
	//Get explain of picture
	//目前，仅支持中文图片说明提取
	std::string strResult(strBuf);
	int nStart,nEnd;
	int pos1,pos2;
	for(int i=0;i<arrayPictures.GetSize();i++)
	{
		nStart=arrayPictures.GetAt(i)->m_data.m_nEndPos+1;

		if(i==arrayPictures.GetSize()-1)
		{
			//last one            			
			nEnd=strResult.length()-1;           
		}
		else
		{
			nEnd=arrayPictures.GetAt(i+1)->m_data.m_nStartPos-1;
		}
		if(nStart>=nEnd)
			continue;
		// get the explain text of the picture
		pos1=nStart;
		do
		{
			if(strResult[pos1]=='<')
				pos1=strResult.find(">",pos1);			
		}
		while (pos1!=-1 && ++pos1<nEnd && !(strResult[pos1] & 0x80));

		if( pos1==-1 || pos1>=nEnd )
			continue;
		//if the head has date
		pos1=strResult.rfind(">",pos1);	
		if(pos1==-1)
			continue;

		nStart=++pos1;		
		//find the last tag
		pos2=strResult.find("<",nStart);
		if(pos2==-1)
			continue;

		nEnd=pos2;
		while (pos1<pos2)
		{
			if(strResult[pos1]=='\r')
			{
				nEnd=pos1;
				break;
			}
			else
				pos1++;
		}
		if(nEnd<=nStart)
			continue;
			
		CString strExplain = strResult.substr(nStart,nEnd-nStart).c_str();
		strExplain = CRegex::Filter(strExplain,_T("\\?+"),CRegex::SELF);	
		strExplain = CRegex::Filter(strExplain,_T("点击.*?图片"),CRegex::SELF);
		arrayPictures.GetAt(i)->m_data.m_strExplain = strExplain;
	}
	return arrayPictures.GetSize();
}
int CRegexWDM::FetchMorePages(const CString& strBuf,CArray<HREF*,HREF*>& morePageHrefs,const CString& strBaseUrl)
{
	//  more page href e.g
	//  	<P align=right>　[1]　<A 
	//             href="http://jczs.news.sina.com.cn/p/2006-10-16/0749404539.html">[2]</A>　<A 
	//             href="http://jczs.news.sina.com.cn/p/2006-10-16/0749404540.html">[3]</A>　<A 
	//             style="FONT-SIZE: 14px" 
	//             href="http://jczs.news.sina.com.cn/p/2006-10-16/0749404539.html">[下一页]</A></P>
	///////////////////////////////////////////////////////////////////////////////
	FetchHrefs(strBuf,morePageHrefs,strBaseUrl);	

	for(int i=0;i<morePageHrefs.GetSize();i++)
	{
		HREF* pHref=morePageHrefs.GetAt(i);
		if(!IsValidMorePage(pHref))
		{
			delete pHref;
			morePageHrefs.RemoveAt(i--);
		}		
	}
	return morePageHrefs.GetSize();
}

int CRegexWDM::FetchHrefs(const CString& strBuf,CArray<HREF*,HREF*>& arHrefs,const CString& strBaseUrl)
{
	//////////////////////////////////////////////////////////
	//  URL href e.g
	//  	<A class=a06 href="http://news.sina.com.cn/w/2006-10-16/103711249169.shtml" 
	//  	 	 	target=_blank>朝核爆后民众生活如常</A>
	//////////////////////////////////////////////////////////////////////////
	if( strBuf.IsEmpty() )
		return 0;

	arHrefs.RemoveAll();
	const char* rule_text=_T("<\\s*A\\s+[^>]*?href\\s*=\\s*[\"\']?\\s*(.*?)[\"\'> ](.*?)</A>");

	boost::regex e(rule_text,boost::regex::normal | boost::regbase::icase);			

	std::string s(strBuf);
	std::string::const_iterator start, end;
	start = s.begin();
	end = s.end();   
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	HREF* pHref=NULL;

	CUrl url;
	url.ParseUrlEx(strBaseUrl);

	while(boost::regex_search(start, end, what, e, flags))   
	{
		pHref=new HREF(std::string(what[1].first,what[1].second).c_str(),
			CCommonFunction::GetPureText(std::string(what[2].first,what[2].second).c_str()));

		CCommonFunction::Replace(pHref->m_strLink,"\\","/");
		CCommonFunction::Replace(pHref->m_strLink,"&amp;","&");
		pHref->m_strLink = url.MakeURL(pHref->m_strLink);

		int nPos = pHref->m_strContent.Find('>');
		if(nPos != -1)
			pHref->m_strContent = pHref->m_strContent.Mid(nPos+1);
		//push the data 
		arHrefs.Add(pHref);
		// update search position
		start = what[0].second;      
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;		
	}
	return arHrefs.GetSize();
}