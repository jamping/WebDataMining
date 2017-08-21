// ChineseCodeLib.h: interface for the CChineseCodeLib class.
//
//////////////////////////////////////////////////////////////////////
/*
功能：汉字GB2312与UTF-8、BIG5编码互转
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