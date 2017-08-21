// CommonFunction.h: interface for the CCommonFunction class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CCommonFunction  
{
public:	
    static void OpenInNet();
	static void OpenOutNet();

	static void ToChar(CString str, char *buf, int len);
    static BOOL IsString(char *str, char *teststr, int bytenum);

	static void CreateDir(CString FilePath);
	static void Replace(CString& str,CString strOld,CString strNew);
    static BOOL IsSameString(CString str1,CString str2);
	
	static CString GetStrFromIniFile(CString sectionName, CString keyName, CString filePath);
    static CString GetFileFullPath(CString filename);
	static CString AllocateID();

	static COleDateTime Int2OleDateTime(int nValue,BOOL bDate=TRUE);
	static int OleDateTime2Int(const COleDateTime& time,BOOL bDate=TRUE);
	static void InsertDataToComboBox(CComboBox& comboBox,CString strSql,CString strFieldName,int nServerType=0);

	static CString GetPureText(const CString& str,BOOL bSaveImg = FALSE);
	static CString GetHtmlText(const CString& str);

    static CString Time2UnixTimeStamp(const CString& strTime = _T(""));
	static CString UnixTimeStamp2Time(const CString& strUnixTimeStamp);
	static BOOL AddPrefixString(LPTSTR lpOutPut, LPCTSTR lpOldString, LPCTSTR lpPrefix, UINT uMaxSize , BOOL bSensitivity);	
	static BOOL AddPostfixString(LPTSTR lpOutPut, LPCTSTR lpOldString, LPCTSTR lpPostfix, UINT uMaxSize, BOOL bSensitivity);
	static BOOL RemovePrefixString(LPTSTR lpOutPut,LPCTSTR lpPrefix, BOOL bSensitivity);
	static BOOL RemovePostfixString(LPTSTR lpOutPut,LPCTSTR lpPostfix, BOOL bSensitivity);

	static CString GetMimeType(const CString& strExt);
	static CString ConvertChineseToPyt(const CString& str,BOOL bInitial = FALSE,TCHAR cSeparator = ' ');
	static int  ResamplePicture(const CString& strPath,const CString& strNewPath,float lNewWidth,float lNewHeight,int nInterpolation);
	static BOOL PictureIsValid(const CString& strPath);
}; 