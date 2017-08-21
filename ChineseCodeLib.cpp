// ChineseCodeLib.cpp: implementation of the CChineseCodeLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChineseCodeLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChineseCodeLib::CChineseCodeLib()
{

}

CChineseCodeLib::~CChineseCodeLib()
{

}

CString CChineseCodeLib::GBKToUtf8(LPCTSTR strGBK) 
{ 
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0); 
	unsigned short * wszUtf8 = new unsigned short[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
	CString str(szUtf8); 
	delete[] szUtf8; 
	delete[] wszUtf8; 

	return str;
}
CString CChineseCodeLib::Utf8ToGBK(LPCTSTR strUtf8) 
{ 
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0,(LPCWSTR) wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
	CString str(szGBK); 
	delete[] szGBK; 
	delete[] wszGBK; 

	return str;
}
CString CChineseCodeLib::Big5ToGBK(LPCTSTR strBig5)
{ 
	char* pszBig5=NULL; //Big5编码的字符 
	wchar_t* wszUnicode=NULL; //Unicode编码的字符 
	char* pszGbt=NULL; //Gb编码的繁体字符 
	char* pszGbs=NULL; //Gb编码的简体字符 
	CString strGBK; //返回的字符串 
	int iLen=0; //需要转换的字符数 
	pszBig5=(LPTSTR)strBig5; //读入需要转换的字符参数 
	//计算转换的字符数 
	iLen=MultiByteToWideChar (950, 0, pszBig5, -1, NULL,0) ; 
	//给wszUnicode分配内存 
	wszUnicode=new wchar_t[iLen+1]; 
	//转换Big5码到Unicode码，使用了API函数MultiByteToWideChar 
	MultiByteToWideChar (950, 0, pszBig5, -1, wszUnicode,iLen); 
	//计算转换的字符数 
	iLen=WideCharToMultiByte (936, 0, (PWSTR) wszUnicode, -1, NULL,0, NULL, NULL) ; 
	//给pszGbt分配内存 
	pszGbt=new char[iLen+1]; 
	//给pszGbs分配内存 
	pszGbs=new char[iLen+1]; 
	//转换Unicode码到Gb码繁体，使用API函数WideCharToMultiByte 
	WideCharToMultiByte (936, 0, (PWSTR) wszUnicode, -1, pszGbt,iLen, NULL, NULL) ; 
	//转换Gb码繁体到Gb码简体，使用API函数LCMapString 
	LCMapString(0x0804,LCMAP_SIMPLIFIED_CHINESE, pszGbt, -1, pszGbs, iLen); 
	//返回Gb码简体字符 
	strGBK=pszGbs; 
	//释放内存 
	delete [] wszUnicode; 
	delete [] pszGbt; 
	delete [] pszGbs; 

	return strGBK; 
} 
CString CChineseCodeLib::GBKToBig5(LPCTSTR strGBK)
{
	char* pszGbt=NULL; //Gb编码的繁体字符
	char* pszGbs=NULL; //Gb编码的简体字符
	wchar_t* wszUnicode=NULL; //Unicode编码的字符
	char* pszBig5=NULL; //Big5编码的字符
	int iLen=0; //需要转换的字符数
	CString strBig5; //返回的字符串 

	pszGbs=(char*)strGBK; //读入需要转换的字符参数

	//计算转换的字符数
	iLen=MultiByteToWideChar (936, 0, pszGbs, -1, NULL,0) ;

	//给pszGbt分配内存
	pszGbt=new char[iLen*2+1];
	//转换Gb码简体到Gb码繁体，使用API函数LCMapString
	LCMapString(0x0804,LCMAP_TRADITIONAL_CHINESE, pszGbs, -1, pszGbt, iLen*2);

	//给wszUnicode分配内存
	wszUnicode=new wchar_t[iLen+1];
	//转换Gb码到Unicode码，使用了API函数MultiByteToWideChar
	MultiByteToWideChar (936, 0, pszGbt, -1, wszUnicode,iLen);

	//计算转换的字符数
	iLen=WideCharToMultiByte (950, 0, (PWSTR) wszUnicode, -1, NULL,0, NULL, NULL) ;
	//给pszBig5分配内存
	pszBig5=new char[iLen+1];
	//转换Unicode码到Big5码，使用API函数WideCharToMultiByte
	WideCharToMultiByte (950, 0, (PWSTR) wszUnicode, -1, pszBig5,iLen, NULL, NULL) ;

	//返回Big5码字符
	strBig5 = pszBig5;

	//释放内存
	delete [] wszUnicode;
	delete [] pszGbt;
	delete [] pszBig5;

	return strBig5;
}
