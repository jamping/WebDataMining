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
	char* pszBig5=NULL; //Big5������ַ� 
	wchar_t* wszUnicode=NULL; //Unicode������ַ� 
	char* pszGbt=NULL; //Gb����ķ����ַ� 
	char* pszGbs=NULL; //Gb����ļ����ַ� 
	CString strGBK; //���ص��ַ��� 
	int iLen=0; //��Ҫת�����ַ��� 
	pszBig5=(LPTSTR)strBig5; //������Ҫת�����ַ����� 
	//����ת�����ַ��� 
	iLen=MultiByteToWideChar (950, 0, pszBig5, -1, NULL,0) ; 
	//��wszUnicode�����ڴ� 
	wszUnicode=new wchar_t[iLen+1]; 
	//ת��Big5�뵽Unicode�룬ʹ����API����MultiByteToWideChar 
	MultiByteToWideChar (950, 0, pszBig5, -1, wszUnicode,iLen); 
	//����ת�����ַ��� 
	iLen=WideCharToMultiByte (936, 0, (PWSTR) wszUnicode, -1, NULL,0, NULL, NULL) ; 
	//��pszGbt�����ڴ� 
	pszGbt=new char[iLen+1]; 
	//��pszGbs�����ڴ� 
	pszGbs=new char[iLen+1]; 
	//ת��Unicode�뵽Gb�뷱�壬ʹ��API����WideCharToMultiByte 
	WideCharToMultiByte (936, 0, (PWSTR) wszUnicode, -1, pszGbt,iLen, NULL, NULL) ; 
	//ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString 
	LCMapString(0x0804,LCMAP_SIMPLIFIED_CHINESE, pszGbt, -1, pszGbs, iLen); 
	//����Gb������ַ� 
	strGBK=pszGbs; 
	//�ͷ��ڴ� 
	delete [] wszUnicode; 
	delete [] pszGbt; 
	delete [] pszGbs; 

	return strGBK; 
} 
CString CChineseCodeLib::GBKToBig5(LPCTSTR strGBK)
{
	char* pszGbt=NULL; //Gb����ķ����ַ�
	char* pszGbs=NULL; //Gb����ļ����ַ�
	wchar_t* wszUnicode=NULL; //Unicode������ַ�
	char* pszBig5=NULL; //Big5������ַ�
	int iLen=0; //��Ҫת�����ַ���
	CString strBig5; //���ص��ַ��� 

	pszGbs=(char*)strGBK; //������Ҫת�����ַ�����

	//����ת�����ַ���
	iLen=MultiByteToWideChar (936, 0, pszGbs, -1, NULL,0) ;

	//��pszGbt�����ڴ�
	pszGbt=new char[iLen*2+1];
	//ת��Gb����嵽Gb�뷱�壬ʹ��API����LCMapString
	LCMapString(0x0804,LCMAP_TRADITIONAL_CHINESE, pszGbs, -1, pszGbt, iLen*2);

	//��wszUnicode�����ڴ�
	wszUnicode=new wchar_t[iLen+1];
	//ת��Gb�뵽Unicode�룬ʹ����API����MultiByteToWideChar
	MultiByteToWideChar (936, 0, pszGbt, -1, wszUnicode,iLen);

	//����ת�����ַ���
	iLen=WideCharToMultiByte (950, 0, (PWSTR) wszUnicode, -1, NULL,0, NULL, NULL) ;
	//��pszBig5�����ڴ�
	pszBig5=new char[iLen+1];
	//ת��Unicode�뵽Big5�룬ʹ��API����WideCharToMultiByte
	WideCharToMultiByte (950, 0, (PWSTR) wszUnicode, -1, pszBig5,iLen, NULL, NULL) ;

	//����Big5���ַ�
	strBig5 = pszBig5;

	//�ͷ��ڴ�
	delete [] wszUnicode;
	delete [] pszGbt;
	delete [] pszBig5;

	return strBig5;
}
