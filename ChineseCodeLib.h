// ChineseCodeLib.h: interface for the CChineseCodeLib class.
//
//////////////////////////////////////////////////////////////////////
/*
���ܣ�����GB2312��UTF-8��BIG5���뻥ת
*/

#pragma once

class CChineseCodeLib  
{
public:
	static CString GBKToUtf8(LPCTSTR strGBK); 
	static CString Utf8ToGBK(LPCTSTR strUtf8); 

	static CString Big5ToGBK(LPCTSTR strBig5);
	static CString GBKToBig5(LPCTSTR strGBK);
private:
	CChineseCodeLib();
	virtual ~CChineseCodeLib();
};