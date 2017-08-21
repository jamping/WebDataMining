//================================================================================
// CLASS   : CExtract
// FUNCTION: 提取网页内容，去除html标签。
// AUTHOR  : 2006-05-21 Created by navy
// REFER   : 输入文件名（*.htm/html）
// NOTE    : Extract.cpp , implementation file
//================================================================================
#include "StdAfx.h"
#include "Extract.h"
#include "windows.h"

CExtract::CExtract()
{	
	strUseful = STR_USEFUL ;           // "<TITLE>,<TR>,<TD>,<P,</P>,<BR>,<DIV>,<IMG>"
}

//--------------------------------------------------------------------------------
// FUNCTION: 将处理结果存入向量中。
// IN      : strResult
// OUT     : strResult
// AUTHOR  : 2006-11-11 Created by ocean .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::GetPureText(std::string& strResult,bool bSaveImg/* =true */,bool bSaveEmptyLine/*=false*/)
{	
    ConvCapital(strResult);           //改为大字字符
	
	Delete2(strResult,"SCRIPT",0);    //去除<SCRIPT></SCRIPT>间的内容
	Delete2(strResult,"STYLE",0);     //去除<STYLE></STYLE>间的内容
	Delete2(strResult,"XML",0);       //去除<XML></XML>间的内容

	DelBegAnd(strResult,0);           //替换以&开头以;结束的字符串
	KeepListLabel(strResult,0,bSaveImg);       
 	DelUselessLink(strResult);        //删除无用链接
	
	if ( STR_REPLACE_VERTICAL ) 
		Replace(strResult,"|"," "); 
	                   
	Replace(strResult,"　　","");       //删除连续两个全角空格
	Replace(strResult,"\t"," ");        //用空格替换TAB 
	Replace(strResult,"  "," ");        //最多保留一个空格

	Replace(strResult,"\r","<BR");
	Replace(strResult,"\n","<BR");

	Result(strResult,bSaveEmptyLine);
	Output(strResult);
}
void CExtract::GetPureHtml(std::string& strResult)
{
    ConvCapital(strResult);           //改为大字字符
	
	Delete2(strResult,"SCRIPT",0);    //去除<SCRIPT></SCRIPT>间的内容
	Delete2(strResult,"STYLE",0);     //去除<STYLE></STYLE>间的内容
	Delete2(strResult,"XML",0);       //去除<XML></XML>间的内容
    Delete2(strResult,"IFRAME",0);    //去除<IFRAME></IFRAME>间的内容

	DelBegAnd(strResult,0);           //替换以&开头以;结束的字符串
}
void CExtract::Output(std::string& strResult)
{
	strResult.erase();
	std::string strTmp,strTmpOld;

	for (int i=0;i<vecResult.size();i++ )
	{
		strTmpOld = strTmp ; 
		strTmp = vecResult[i] ;	     
		DelSideSpace(strTmp); 
		if((i > 0) && (strTmp == strTmpOld)) continue;
		if ( strTmp == "·" ) continue;
		if(i==vecResult.size()-1)
			strResult+=strTmp;
		else
			strResult+=strTmp+"\r\n";
	}
}
//--------------------------------------------------------------------------------
// FUNCTION: 将处理结果存入向量中。
// IN      : strResult
// OUT     : vecResult
// AUTHOR  : 2006-05-22 Created by navy .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::Result(std::string strResult,bool bSaveEmptyLine/*=false*/)
{
	int nPos;
	std::string strTmp;

	vecResult.clear();
	nPos = strResult.find("<BR");
	while (nPos != -1)
	{
		strTmp = strResult.substr(0,nPos);
		DelSideSpace(strTmp);       //去除前后空格
		if ( bSaveEmptyLine || !strTmp.empty() ) {
			vecResult.push_back(strTmp);
		}
		strResult = strResult.substr(nPos+3);
		nPos = strResult.find("<BR");
	} 
	DelSideSpace(strResult); 
	if (!strResult.empty()) 
	{
		vecResult.push_back(strResult);
	}
}


//--------------------------------------------------------------------------------
// FUNCTION: 将小写字母转为大写。
// IN      : 一行。
// OUT     : 
// AUTHOR  : 2006-05-27 Created by navy .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::ConvCapital(std::string& strLine)
{
	if(STR_TI_CAPITAL)  
	{
		Replace(strLine,"<ti","<TI");
		Replace(strLine,"</ti","</TI");
	}
	if(STR_TR_CAPITAL)  
	{
		Replace(strLine,"<tr","<TR");
		Replace(strLine,"</tr","</TR");
	}
	if(STR_TD_CAPITAL)  
	{
		Replace(strLine,"<td","<TD");
		Replace(strLine,"</td","</TD");
	}
	if(STR_P_CAPITAL)   
	{
		Replace(strLine,"<p","<P");
		Replace(strLine,"</p","</P");
	}
	if(STR_BR_CAPITAL)  
	{
		Replace(strLine,"<br","<BR"); 
	}
	if(STR_DI_CAPITAL)  
	{
		Replace(strLine,"<di","<DI");
		Replace(strLine,"</di","</DI");
	}
	if(STR_XML_CAPITAL) 
	{
		Replace(strLine,"<xml","<XML");
		Replace(strLine,"</xml","</XML");
	}
	if(STR_A_CAPITAL ) 
	{		
		Replace(strLine,"<a","<A");
		Replace(strLine,"</a","</A");
	}
	if(STR_SPACE ) 
	{
		Replace(strLine," ",""); 
	}
	Replace(strLine,"<script","<SCRIPT");
	Replace(strLine,"</script","</SCRIPT");

	Replace(strLine,"<style","<STYLE");
	Replace(strLine,"</style","</STYLE");

	Replace(strLine,"<iframe","<IFRAME");
	Replace(strLine,"</iframe","</IFRAME");
	Replace(strLine,"<img","<IMG");
}


//--------------------------------------------------------------------------------
// FUNCTION: 对strLine进行预处理。
// IN      : 一行。
// OUT     : 将结果保存在变量中。
// AUTHOR  : 2006-05-21 Created by navy .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::PreProcess(std::string& strLine)
{
	wchar_t ws[1024];
	char ss[2048];
	int nTrans = MultiByteToWideChar(CP_ACP, 0, strLine.c_str(), strLine.size(), ws, 1024 );
	ws[nTrans] =0;

	std::wstring wss=ws;

	Replace(wss,L"　",L" ");        //将"　"(全角的空格)替换成空格

	nTrans = WideCharToMultiByte(CP_ACP, 0, wss.c_str(), wss.size(), ss, 2048, NULL, NULL );
	ss[nTrans] = 0;
	strLine=ss;
}


//--------------------------------------------------------------------------------
// FUNCTION: 去除<>间的字符。
// IN      : strResult，以及分隔符。
// OUT     : 传址
// AUTHOR  : 2006-05-21 Created by navy .
//           2006-05-25 Modified by navy .
// CALL    : Delete(strResult,"<TD"); 
// NOTE    : Eg：<TD style="FONT-SIZE: 24pt" width="100%"> 变为：<TD>
//--------------------------------------------------------------------------------
void CExtract::Delete(std::string& strResult,std::string separator)
{
	std::string strTemp1,strTemp2;
	int nPos1,nPos2;
	int n1,n2;
	nPos1 = strResult.find(separator);
	nPos2 = strResult.find(">",nPos1);
	if ((nPos1 == -1) || (nPos2 == -1)) return; //如果没有<或>，直接退出。
	
	n1 = CountSymbol(strResult,nPos1,nPos2,"<"); //n1、n2至少等于1
	n2 = CountSymbol(strResult,nPos1,nPos2,">");

	while (n1 != n2)
	{
		nPos2 = strResult.find(">",nPos2+1);
		n1 = CountSymbol(strResult,nPos1,nPos2,"<");
		n2 = CountSymbol(strResult,nPos1,nPos2,">");
	}

	if(nPos2 == -1) return;                      //注意：如果找不到，要返回。

	strTemp1=strResult.substr(0,nPos1);
	strTemp2=strResult.substr(nPos2+1);
	
	Delete(strTemp2,separator);
	strResult=strTemp1+separator+">"+strTemp2;
}

//--------------------------------------------------------------------------------
// FUNCTION: 返回位置i,j间的symbol个数。
// IN      : 
// OUT     : 
// AUTHOR  : 2006-05-25 Created by navy .
// CALL    : int n = CountSymbol(strResult,4,19,"<"); 结果n=4
// NOTE    : Eg：<TD><A<B<BR>>BC</B>>DD</A></TD> 变为：<TD>
//               0123456789012345678901234567890  
//                         1         2         3
//--------------------------------------------------------------------------------
int CExtract::CountSymbol(std::string strResult,int i,int j,std::string symbol)
{
	int nPos;
	int nCount = 0 ;
	nPos = strResult.find(symbol,i);
	while((nPos != -1) && (nPos <= j))
	{
		nCount ++ ; 
		nPos = strResult.find(symbol,nPos+1);
	}
	return nCount ; 
}

//--------------------------------------------------------------------------------
// FUNCTION: 去除<><>间的字符。
// IN      : strResult，以及分隔符。
// OUT     : 传址
// AUTHOR  : 2006-05-22 Created by navy .
//           2006-05-29 Modified by navy .
// NOTE    : Eg：abc<SCRIPT language=JavaScript>……</SCRIPT>abc 变为：abcabc
// NOTE    : 存在嵌套的情况
//           abc<SCRIPT><SCRIPT>……</SCRIPT>……</SCRIPT>abc 变为：abcabc
//--------------------------------------------------------------------------------
void CExtract::Delete2(std::string& strResult,std::string separator,int nPos)
{
	std::string strTemp1,strTemp2;
	std::string sepa1 = "<"+separator ;
	std::string sepa2 = "</"+separator+">";
	int nPos1,nPos2;
	int n1,n2;
	
	nPos1=strResult.find(sepa1,nPos);
	nPos2=strResult.find(sepa2,nPos1);
	if ((nPos1 == -1) || (nPos2 == -1)) return; //如果没有<或>，直接退出。

	n1 = CountSymbol(strResult,nPos1,nPos2,sepa1);
	n2 = CountSymbol(strResult,nPos1,nPos2+sepa2.length(),sepa2);

	while (n1 != n2)
	{
		nPos2 = strResult.find(sepa2,nPos2+1);
		n1 = CountSymbol(strResult,nPos1,nPos2,sepa1);
		n2 = CountSymbol(strResult,nPos1,nPos2+sepa2.length(),sepa2);
	}

	if(nPos2 == -1) return;                      //注意：如果找不到，要返回。

	strTemp1=strResult.substr(0,nPos1);
	strTemp2=strResult.substr(nPos2+sepa2.length());
	
	Delete2(strTemp2,separator,0);
	strResult=strTemp1 + strTemp2 ;
}


//--------------------------------------------------------------------------------
// FUNCTION: 保留指定标签之间的字符。
// IN      : strResult
// OUT     : strResult
// AUTHOR  : 2006-05-22 Created by navy .
//           2006-05-26 Modified by navy .
// NOTE    : Eg：<HTML><TD><FONT>a<A>b</A>c</FONT></TD></HTML> 变为：<TD>a<A>bc
// NOTE    : Eg：<TABLE <BR>><TD>a</TD></TABLE> 变为：_a (_表示空格)
//--------------------------------------------------------------------------------
void CExtract::KeepListLabel(std::string& strResult,int nPosOld,bool bSaveImg/* =true */)
{
	//std::string strUseful = "<TITLE>,<TR>,<TD>,<P ,<P>,</P>,<BR>,<DIV>,<IMG>"
	std::string strTemp1,strTemp2;
	std::string sepa;
	int nPos,nPos1,nPos2;
	int n1,n2;

	nPos1 = strResult.find("<"); 
	nPos2 = strResult.find(">",nPos1); 

	while ((nPos1 != -1) && (nPos2 != -1)) 
	{
		sepa=strResult.substr(nPos1,3);
		DelSideSpace(sepa);
		nPos=strUseful.find(sepa); 
		n1 = CountSymbol(strResult,nPos1,nPos2,"<");
		n2 = CountSymbol(strResult,nPos1,nPos2,">");
		while (n1 != n2)
		{
			nPos2 = strResult.find(">",nPos2+1);
			n1 = CountSymbol(strResult,nPos1,nPos2,"<");
			n2 = CountSymbol(strResult,nPos1,nPos2,">");
		}
		if(nPos2 == -1)                  //<TITLE>博客园 - webcool</</TITLE>
		{
			nPos2 = strResult.find(">",nPos1); 
		}
		strTemp1 = strResult.substr(0,nPos1); 
		strTemp2 = strResult.substr(nPos2+1); 
		DelSideSpace(strTemp1);
		DelSideSpace(strTemp2);
		if ( nPos != -1 ) 
		{
			if (sepa == "<TD") 
				strTemp1 += " " ;       
			else if(sepa == "<IM" && bSaveImg)
                strTemp1 += "<BR" +strResult.substr(nPos1,nPos2-nPos1+1)+"<BR" ;
			else
				strTemp1 += "<BR" ; 
		}
		strResult = strTemp1+strTemp2;
		nPos1=strResult.find("<",strTemp1.length()); 
		nPos2=strResult.find(">",strTemp1.length()); 
	}	
}

//--------------------------------------------------------------------------------
// FUNCTION: 将strLine中的字符串strOld用strNew替换。
// IN      : 输入一行
// OUT     : 替换后的行
// AUTHOR  : 2006-05-21 Created by navy .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::Replace(std::string& strLine,std::string strOld,std::string strNew)
{
	int nPos=0;
	while ((nPos=strLine.find(strOld,nPos)) != -1) 
	{
 		strLine=strLine.substr(0,nPos)+strNew+strLine.substr(nPos+strOld.length());
		nPos += strNew.length();
	}
}

//--------------------------------------------------------------------------------
// FUNCTION: 将strLine中的字符串strOld用strNew替换。重载函数，处理宽字符。
// IN      : 输入一行
// OUT     : 替换后的行
// AUTHOR  : 2006-05-21 Created by navy .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::Replace(std::wstring& strLine,std::wstring strOld,std::wstring strNew)
{
	int nPos=0;
	while ((nPos=strLine.find(strOld,nPos)) != -1) 
	{
 		strLine=strLine.substr(0,nPos)+strNew+strLine.substr(nPos+strOld.length());
		nPos += strNew.length();
	}
}

//--------------------------------------------------------------------------------
// FUNCTION: 去除strLine两边的空格。
// IN      : strLine
// OUT     : strLine
// AUTHOR  : 2006-05-22 Created by navy .
// NOTE    : 
//--------------------------------------------------------------------------------
void CExtract::DelSideSpace(std::string& strLine)
{
	int nPos;
	if (strLine.empty()) return;
	nPos=strLine.find_first_not_of(" ");
	if ( nPos == -1 ) {
		strLine = "";
		return;
	}
	strLine=strLine.substr(nPos);
	nPos=strLine.find_last_not_of(" ");
	strLine=strLine.substr(0,nPos+1);
}

//--------------------------------------------------------------------------------
// FUNCTION: 去除以&开头以;结束的字符串。
// IN      : strResult
// OUT     : strResult
// AUTHOR  : 2006-05-24 Created by navy .
//           2006-05-29 Modified by navy .
// NOTE    : 1."&nbsp;"-->" "   2."&lt;"-->"<"      3."&gt;"-->">"
//           4."&amp;"-->"&"    5."&quot;"-->"\"    6."&copy;"-->"(C)"
//           7."&reg;"-->"?"    8."&trade;"-->"TM"  9."&#8226;"-->"·"
//--------------------------------------------------------------------------------
void CExtract::DelBegAnd(std::string& strResult,int nPos) //从第nPos个位置开始
{
	//a&nbsp;&&nbsp;;;&nbsp;编辑
	std::string strTmp1,strTmp2;
	std::string strSymbol;
	int nPos1,nPos2;

	nPos1 = strResult.find("&",nPos);
	nPos2 = strResult.find(";",nPos1);
	while ((nPos1 != -1) || (nPos2 != -1)) 
	{
		strSymbol = strResult.substr(nPos1+1,nPos2-nPos1-1);
		if(strSymbol == "nbsp")       strSymbol = " ";
		else if(strSymbol == "lt")    strSymbol = "<";
		else if(strSymbol == "gt")    strSymbol = ">";
		else if(strSymbol == "amp")   strSymbol = "&";
		else if(strSymbol == "quot")  strSymbol = "\"";
		else if(strSymbol == "copy")  strSymbol = "(C)";
		else if(strSymbol == "reg")   strSymbol = "?";
		else if(strSymbol == "trade") strSymbol = "TM";
		else if(strSymbol == "#183")  strSymbol = "·";
		else if(strSymbol == "#8226") strSymbol = "·";
		else if(strSymbol == "#8220") strSymbol = "“";
		else if(strSymbol == "#8221") strSymbol = "”";
		else if(strSymbol == "#8216") strSymbol = "‘";
		else if(strSymbol == "#8217") strSymbol = "’";
		else if(strSymbol == "#8212") strSymbol = "—";
		else
		{
			nPos1 = strResult.find("&",nPos1+1); //找下一个&
			nPos2 = strResult.find(";",nPos1);
			continue;
		}

		strTmp1 = strResult.substr(0,nPos1) ;
		strTmp2 = strResult.substr(nPos2 + 1) ;
		strResult = strTmp1 + strSymbol + strTmp2;
		
		nPos1 = strResult.find("&",strTmp1.length());
		nPos2 = strResult.find(";",nPos1);
	}	
}

//--------------------------------------------------------------------------------
// FUNCTION: 删除无用链接(在<TR>和</TR>间除链接外，没有其他内容)
// IN      : strResult
// OUT     : 传址
// AUTHOR  : 2006-05-27 Created by navy .
// NOTE    : Eg：a<TR><TD><A>bc</A></TD></TR><TR><TD>d</TD></TR>e 变为：
//               a<TR><TD>d</TD></TR>e
//--------------------------------------------------------------------------------
void CExtract::DelUselessLink(std::string& strResult) 
{
	std::string strTmp1,strTmp2,strTmp3;
	int nPos1,nPos2;
	nPos1 = strResult.find("<TR");
	nPos2 = strResult.find("</TR>");
	while ((nPos1 != -1) && (nPos2 != -1)) {
		strTmp1 = strResult.substr(0,nPos1);
		strTmp2 = strResult.substr(nPos1,nPos2 - nPos1 + 5);
		strTmp3 = strResult.substr(nPos2 + 5);
		Delete2(strTmp2,"A",0);               //删除<A>与</A>间的内容
		KeepListLabel(strTmp2,0);
		Replace(strTmp2,"<BR","");
		Replace(strTmp2," ","");
		if (strTmp2.empty())
		{
			strResult = strTmp1 + strTmp3 ; 
			nPos1 = strResult.find("<TR",strTmp1.length());
			nPos2 = strResult.find("</TR>",nPos1);
			continue;
		}

		nPos1 = strResult.find("<TR",nPos2 + 5);
		nPos2 = strResult.find("</TR>",nPos1);
	}
}

CExtract::~CExtract()
{

}

