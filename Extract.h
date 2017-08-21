//================================================================================
// CLASS   : CExtract
// FUNCTION: 提取网页内容，去除html标签。
// AUTHOR  : 2006-05-21 Created by ocean
// REFER   : 输入文件名（*.htm/html）
// NOTE    : Extract.h , interface for the Extract class.
// Modified by ocean in 2006.11.11
//================================================================================
#pragma once
/********************************************************************************/
//用户配置文件 0:否；1:是
#define STR_USEFUL "<TITLE>,<TR>,<TD>,<P ,<P>,</P>,<BR>,<DIV>,<IMG>"
#define STR_A       0          //是否删除<A></A>及其之间的内容［a<A>b</A>c变为ac］

#define STR_SPACE   0          //是否删除所有空格
#define STR_REPLACE_VERTICAL 1 //是否将|替换成空格
#define STR_TI_CAPITAL   1     //是否要将小写(title)变为大写(TITLE)
#define STR_TR_CAPITAL   1     //是否要将小写(tr)变为大写(TR)
#define STR_TD_CAPITAL   1     //是否要将小写(td)变为大写(TD)
#define STR_P_CAPITAL    1     //是否要将小写(p)变为大写(P)
#define STR_BR_CAPITAL   1     //是否要将小写(br)变为大写(BR)
#define STR_DI_CAPITAL   1     //是否要将小写(div)变为大写(DIV)
#define STR_A_CAPITAL    1     //是否要将小写(a)变为大写(A)
#define STR_XML_CAPITAL  1     //是否要将小写(xml)变为大写(XML)  注：一般为小写
/********************************************************************************/
#include "string"
#include "vector"
//using namespace std;

class CExtract  
{
public:
	CExtract();	
	virtual ~CExtract();

	void GetPureText(std::string& strResult,bool bSaveImg=true,bool bSaveEmptyLine=false);
	void GetPureHtml(std::string& strResult);
private:
	void Process(std::string filename);
	void Result(std::string strResult,bool bSaveEmptyLine=false);
	void Output(std::string& strResult);
	void ConvCapital(std::string& strLine);
	void PreProcess(std::string& strLine);
	//CountSymbol函数：返回位置i,j间的symbol个数。
	int  CountSymbol(std::string strResult,int i,int j,std::string symbol);
	void Delete(std::string& strResult,std::string separator);
	void Delete2(std::string& strResult,std::string separator,int nPos);
	void KeepListLabel(std::string& strResult,int nPosOld,bool bSaveImg=true);
	void Replace(std::string& strLine,std::string strOld,std::string strNew);
	void Replace(std::wstring& strLine,std::wstring strOld,std::wstring strNew); //处理宽字符
	void DelSideSpace(std::string& strLine);
	void DelBegAnd(std::string& strResult,int nPos);   //去除以&开头以;结束的字符串
	void DelUselessLink(std::string& strResult);       //删除无用链接
private:
	std::vector<std::string> vecResult; //存放结果，一个向量中存一行。
	std::string strUseful;	
};