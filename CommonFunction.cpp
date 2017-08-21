// CommonFunction.cpp: implementation of the CCommonFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "CommonFunction.h"
#include <stack>
#include "Extract.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CCommonFunction::OpenInNet()
{
	typedef int (*TURN_B_ON)(void);
    try
	{
		TURN_B_ON Turn_B_On;
		HINSTANCE hdll=LoadLibrary("NETSWUSB.dll");
		if (hdll!=NULL)
		{
			Turn_B_On=(TURN_B_ON)GetProcAddress(hdll,"Turn_B_On");
			int x=Turn_B_On();
			
			FreeLibrary(hdll);
		}
	}
	catch (CException *e)
	{
		e->Delete();
	}
}

void CCommonFunction::OpenOutNet()
{
	typedef int (*TURN_A_ON)(void);
    try
	{
		TURN_A_ON Turn_A_On;
		HINSTANCE hdll=LoadLibrary("NETSWUSB.dll");
		if (hdll!=NULL)
		{
			Turn_A_On=(TURN_A_ON)GetProcAddress(hdll,"Turn_A_On");
			int x=Turn_A_On();
			
			FreeLibrary(hdll);
		}
	}
	catch (CException *e)
	{
		e->Delete();
	}
}
void CCommonFunction::ToChar(CString str, char *buf, int len)
{
	int i;
	for (i=0;i<len;i++) 
		buf[i]=0;
	for (i=0;i<str.GetLength() && i<len;i++) 
		buf[i]=str.GetAt(i);
}

BOOL CCommonFunction::IsString(char *str, char *teststr, int bytenum)
{
	char chr;
	char chr1;
	for (int i=0;i<bytenum;i++)
	{   
		chr=toupper(str[i]);
		chr1=toupper(teststr[i]);
		if (chr!=chr1) 
			return false;
	}
	return true;
}
void CCommonFunction::Replace(CString& str,CString strOld,CString strNew)
{
	if(strOld.IsEmpty())
		return ;

	std::string s(str);
	std::string s1(strOld);
	std::string s2(strNew);

	int nPos=0;
	while ((nPos=s.find(strOld,nPos)) != -1) 
	{
 		s=s.substr(0,nPos)+s2+s.substr(nPos+s1.length());
		nPos+=s2.length();
	}
	str=s.c_str();
}
void CCommonFunction::CreateDir(CString FilePath)
{		
	Replace(FilePath,"/","\\");
    
	CString strTemp;
	std::stack<CString> dirs;

	for( int i = FilePath.GetLength()-1; i > 2; i-- )
	{
		if( FilePath[i] == '\\' )
		{
			 strTemp = FilePath.Left(i);

			 if( PathIsDirectory(strTemp) )
				 break;
			 else
				 dirs.push(strTemp);
		}
	}
	while ( dirs.size() ) 
	{
		CreateDirectory(dirs.top(), NULL); 
		dirs.pop();
	}
	
}

CString CCommonFunction::GetStrFromIniFile(CString sectionName, CString keyName, CString filePath)
{
	char buf[1024];
	
	::GetPrivateProfileString(sectionName,
		keyName,
		"",
		buf, 
		sizeof(buf),
		filePath);
	return  CString(buf);				
}

CString CCommonFunction::GetFileFullPath(CString filename)
{
	CString fullpath;
	fullpath=_T("");
	char path[256];

	::GetModuleFileName(NULL,path,256);
	fullpath=(CString)path;
	fullpath=fullpath.Left(fullpath.ReverseFind('\\')+1)+filename;
    
	return fullpath;
}
CString CCommonFunction::AllocateID()
{
	CString strID=_T("");
	GUID guid = GUID_NULL;

	if (S_OK == ::CoCreateGuid(&guid))
	{
		strID.Format("%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}
 	return strID;
}
int CCommonFunction::OleDateTime2Int(const COleDateTime& time,BOOL bDate/* =TRUE */)
{
	// Output format is 20061212 or 083030
	int nRet=0;
	if(bDate)
	{
        nRet=time.GetYear()*10000+time.GetMonth()*100+time.GetDay();
	}
	else
	{
		nRet=time.GetHour()*10000+time.GetMinute()*100+time.GetSecond();
	}
	return nRet;
}
COleDateTime CCommonFunction::Int2OleDateTime(int nValue,BOOL bDate/* =TRUE */)
{
	COleDateTime time;
	int nLeft,nMiddle,nRight;
	
	nLeft=nValue/10000;
	nMiddle=(nValue-nLeft*10000)/100;
	nRight=nValue-nLeft*10000-nMiddle*100;

	if(bDate)
	{
		time.SetDate(nLeft,nMiddle,nRight);
	}
	else
	{
		time.SetTime(nLeft,nMiddle,nRight);
	}
	if(time.GetStatus()!=COleDateTime::invalid)
	   return time;
	else
		return COleDateTime::GetCurrentTime();
}

void CCommonFunction::InsertDataToComboBox(CComboBox& comboBox,CString strSql,CString strFieldName,int nServerType/* =0 */)
{
    if(!theApp.DBIsOpen((CWebDataMiningApp::DBServerType)nServerType))
	     theApp.ConnectDB((CWebDataMiningApp::DBServerType)nServerType);

	CADORecordset rs(theApp.GetDB((CWebDataMiningApp::DBServerType)nServerType));
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		comboBox.ResetContent();
		CString str;
		while(!rs.IsEOF())
		{
			rs.GetFieldValue(strFieldName,str);
			comboBox.AddString(str);
			
			rs.MoveNext();			
		}
		rs.Close();
	}
	
}

BOOL CCommonFunction::IsSameString(CString str1,CString str2)
{
	return str1.CompareNoCase(str2)==0;
}

CString CCommonFunction::GetPureText(const CString& str,BOOL bSaveImg /* = FALSE */)
{
	if(str.IsEmpty())
		return _T("");

	std::string s(str);
	CExtract e;
	e.GetPureText(s,bSaveImg);

	return s.c_str();
}
CString CCommonFunction::GetHtmlText(const CString& str)
{
	if(str.IsEmpty())
		return _T("");

	std::string s(str);
	CExtract e;
	e.GetPureHtml(s);

	return s.c_str();
}


CString CCommonFunction::Time2UnixTimeStamp(const CString& strTime /* = _T */)
{
	//系统标准形式时间转换Unix 时间戳
	//unix时间戳为1900年以来的秒数
	//获取系统当前时间的Unix时间戳
	CString nowTime;
	SYSTEMTIME sysTime;
	COleDateTime dt;

	if(dt.ParseDateTime(strTime))
		dt.GetAsSystemTime(sysTime);
	else
		GetLocalTime(&sysTime);

	CTime m_tTime(sysTime);
	time_t unixTime = m_tTime.GetTime();
	nowTime.Format("%d",unixTime);//输出UNIX时间戳字符串

	return nowTime;
}
CString CCommonFunction::UnixTimeStamp2Time(const CString& strUnixTimeStamp)
{
	//11位Unix 时间戳转换成系统标准形式时间字符串
	CString strOutput;

	int nUnixTime = atoi(strUnixTimeStamp);
	struct tm *newtime;
	__time64_t long_time = nUnixTime;
	newtime = _localtime64( &long_time ); // 值存在newtime 中

	strOutput.Format("%04d-%02d-%02d %02d:%02d:%02d"
		,newtime->tm_year + 1900    //年需要加1900
		,newtime->tm_mon + 1     //月需要加1 (它是从0 到11)
		,newtime->tm_mday
		,newtime->tm_hour
		,newtime->tm_min
		,newtime->tm_sec);

	return strOutput;
}

BOOL CCommonFunction::AddPrefixString(LPTSTR lpOutPut, LPCTSTR lpOldString, LPCTSTR lpPrefix, UINT uMaxSize , BOOL bSensitivity)
{
	if (lpOutPut == NULL  ||  lpOldString == NULL  ||  lpPrefix == NULL )
		return  FALSE;
	CString strOutput  =  lpOldString;
	int  iPreLen  =  lstrlen(lpPrefix);
	if (bSensitivity)
	{
		if (StrCmpN(lpPrefix,lpOldString,iPreLen) == 0 )
		{
			if (lpOutPut == lpOldString)
				return  TRUE;
			else 
				lstrcpyn(lpOutPut,lpOldString,uMaxSize);
		}
		else 
		{
			lstrcpyn(lpOutPut,lpPrefix,uMaxSize);
			StrCatBuff(lpOutPut,strOutput,uMaxSize);
		}
	}
	else 
	{
		if (StrCmpNI(lpPrefix,lpOldString,iPreLen) == 0 )
		{
			if (lpOutPut == lpOldString)
				return  TRUE;
			else 
				lstrcpyn(lpOutPut,lpOldString,uMaxSize);
		}
		else 
		{
			lstrcpyn(lpOutPut,lpPrefix,uMaxSize);
			StrCatBuff(lpOutPut,strOutput,uMaxSize);
		}
	}
	return  TRUE;
}

BOOL CCommonFunction::AddPostfixString(LPTSTR lpOutPut, LPCTSTR lpOldString, LPCTSTR lpPostfix, UINT uMaxSize, BOOL bSensitivity)
{
	if (lpOutPut == NULL  ||  lpOldString == NULL  ||  lpPostfix == NULL )
		return  FALSE;
	int  iOldLen  =  lstrlen(lpOldString);
	int  iPostLen  =  lstrlen(lpPostfix);
	if (iPostLen  >  iOldLen)
	{
		if (lpOutPut == lpOldString)
		{
			StrCatBuff(lpOutPut,lpPostfix,uMaxSize);
		}
		else 
		{
			lstrcpyn(lpOutPut,lpOldString,uMaxSize);
			StrCatBuff(lpOutPut,lpPostfix,uMaxSize);
		}
		return  TRUE;
	}
	if (bSensitivity)
	{
		if (StrCmpN(lpPostfix,(lpOldString + iOldLen - iPostLen),iPostLen) == 0 )
		{
			if (lpOutPut == lpOldString)
				return  TRUE;
			else 
				lstrcpyn(lpOutPut,lpOldString,uMaxSize);
		}
		else 
		{
			if (lpOutPut == lpOldString)
			{
				StrCatBuff(lpOutPut,lpPostfix,uMaxSize);
			}
			else 
			{
				lstrcpyn(lpOutPut,lpOldString,uMaxSize);
				StrCatBuff(lpOutPut,lpPostfix,uMaxSize);
			}
		}
	}
	else 
	{
		if (StrCmpNI(lpPostfix,(lpOldString + iOldLen - iPostLen),iPostLen) == 0 )
		{
			if (lpOutPut == lpOldString)
				return  TRUE;
			else 
				lstrcpyn(lpOutPut,lpOldString,uMaxSize);
		}
		else 
		{
			if (lpOutPut == lpOldString)
			{
				StrCatBuff(lpOutPut,lpPostfix,uMaxSize);
			}
			else 
			{
				lstrcpyn(lpOutPut,lpOldString,uMaxSize);
				StrCatBuff(lpOutPut,lpPostfix,uMaxSize);
			}
		}
	}
	return  TRUE;
}

BOOL CCommonFunction::RemovePrefixString(LPTSTR lpOutPut,LPCTSTR lpPrefix, BOOL bSensitivity)
{
	if ( lpOutPut == NULL  ||  lpPrefix ==  NULL )
		return  FALSE;

	std::string strOutput  =  lpOutPut;
	int  iPreLen  =  lstrlen(lpPrefix);
	if (bSensitivity)
	{
		if (StrCmpN(lpPrefix,lpOutPut,iPreLen) == 0 )
		{
			lstrcpyn(lpOutPut,strOutput.c_str() + iPreLen,strOutput.length());
		}
	}
	else 
	{
		if (StrCmpNI(lpPrefix,lpOutPut,iPreLen) == 0 )
		{
			lstrcpyn(lpOutPut,strOutput.c_str() + iPreLen,strOutput.length());
		}
	}
	return  TRUE;
}

BOOL CCommonFunction::RemovePostfixString(LPTSTR lpOutPut,LPCTSTR lpPostfix, BOOL bSensitivity)
{
	if ( lpOutPut == NULL  ||  lpPostfix ==  NULL )
		return  FALSE;
	int  iPostLen  =  lstrlen(lpPostfix);
	int  iOldLen  =  lstrlen(lpOutPut);
	if (bSensitivity)
	{
		if (StrCmpN(lpPostfix,(lpOutPut + iOldLen - iPostLen),iPostLen) == 0 )
		{
			lpOutPut[iOldLen - iPostLen]  =  TEXT('\0 ');
		}
	}
	else 
	{
		if (StrCmpNI(lpPostfix,(lpOutPut + iOldLen - iPostLen),iPostLen) == 0 )
		{
			lpOutPut[iOldLen - iPostLen]  =  TEXT('\0 ');
		}
	}
	return  TRUE;
} 

CString CCommonFunction::GetMimeType(const CString& strExt)
{
	CString strMime;
	CMapStringToString map;

	struct EXT_MIME_MAP
	{
		LPCSTR strExt;		//扩展名
		LPCSTR strMime;		//MIME
	}
	ext_mime_maps[] = 
	{
        //#region Big freaking list of mime types
		// combination of values from Windows 7 Registry and
		// from C:\Windows\System32\inetsrv\config\applicationHost.config
		// some added, including .7z and .dat
		{".323", "text/h323"},
		{".3g2", "video/3gpp2"},
		{".3gp", "video/3gpp"},
		{".3gp2", "video/3gpp2"},
		{".3gpp", "video/3gpp"},
		{".7z", "application/x-7z-compressed"},
		{".aa", "audio/audible"},
		{".AAC", "audio/aac"},
		{".aaf", "application/octet-stream"},
		{".aax", "audio/vnd.audible.aax"},
		{".ac3", "audio/ac3"},
		{".aca", "application/octet-stream"},
		{".accda", "application/msaccess.addin"},
		{".accdb", "application/msaccess"},
		{".accdc", "application/msaccess.cab"},
		{".accde", "application/msaccess"},
		{".accdr", "application/msaccess.runtime"},
		{".accdt", "application/msaccess"},
		{".accdw", "application/msaccess.webapplication"},
		{".accft", "application/msaccess.ftemplate"},
		{".acx", "application/internet-property-stream"},
		{".AddIn", "text/xml"},
		{".ade", "application/msaccess"},
		{".adobebridge", "application/x-bridge-url"},
		{".adp", "application/msaccess"},
		{".ADT", "audio/vnd.dlna.adts"},
		{".ADTS", "audio/aac"},
		{".afm", "application/octet-stream"},
		{".ai", "application/postscript"},
		{".aif", "audio/x-aiff"},
		{".aifc", "audio/aiff"},
		{".aiff", "audio/aiff"},
		{".air", "application/vnd.adobe.air-application-installer-package+zip"},
		{".amc", "application/x-mpeg"},
		{".application", "application/x-ms-application"},
		{".art", "image/x-jg"},
		{".asa", "application/xml"},
		{".asax", "application/xml"},
		{".ascx", "application/xml"},
		{".asd", "application/octet-stream"},
		{".asf", "video/x-ms-asf"},
		{".ashx", "application/xml"},
		{".asi", "application/octet-stream"},
		{".asm", "text/plain"},
		{".asmx", "application/xml"},
		{".aspx", "application/xml"},
		{".asr", "video/x-ms-asf"},
		{".asx", "video/x-ms-asf"},
		{".atom", "application/atom+xml"},
		{".au", "audio/basic"},
		{".avi", "video/x-msvideo"},
		{".axs", "application/olescript"},
		{".bas", "text/plain"},
		{".bcpio", "application/x-bcpio"},
		{".bin", "application/octet-stream"},
		{".bmp", "image/bmp"},
		{".c", "text/plain"},
		{".cab", "application/octet-stream"},
		{".caf", "audio/x-caf"},
		{".calx", "application/vnd.ms-office.calx"},
		{".cat", "application/vnd.ms-pki.seccat"},
		{".cc", "text/plain"},
		{".cd", "text/plain"},
		{".cdda", "audio/aiff"},
		{".cdf", "application/x-cdf"},
		{".cer", "application/x-x509-ca-cert"},
		{".chm", "application/octet-stream"},
		{".class", "application/x-java-applet"},
		{".clp", "application/x-msclip"},
		{".cmx", "image/x-cmx"},
		{".cnf", "text/plain"},
		{".cod", "image/cis-cod"},
		{".config", "application/xml"},
		{".contact", "text/x-ms-contact"},
		{".coverage", "application/xml"},
		{".cpio", "application/x-cpio"},
		{".cpp", "text/plain"},
		{".crd", "application/x-mscardfile"},
		{".crl", "application/pkix-crl"},
		{".crt", "application/x-x509-ca-cert"},
		{".cs", "text/plain"},
		{".csdproj", "text/plain"},
		{".csh", "application/x-csh"},
		{".csproj", "text/plain"},
		{".css", "text/css"},
		{".csv", "text/csv"},
		{".cur", "application/octet-stream"},
		{".cxx", "text/plain"},
		{".dat", "application/octet-stream"},
		{".datasource", "application/xml"},
		{".dbproj", "text/plain"},
		{".dcr", "application/x-director"},
		{".def", "text/plain"},
		{".deploy", "application/octet-stream"},
		{".der", "application/x-x509-ca-cert"},
		{".dgml", "application/xml"},
		{".dib", "image/bmp"},
		{".dif", "video/x-dv"},
		{".dir", "application/x-director"},
		{".disco", "text/xml"},
		{".dll", "application/x-msdownload"},
		{".dll.config", "text/xml"},
		{".dlm", "text/dlm"},
		{".doc", "application/msword"},
		{".docm", "application/vnd.ms-word.document.macroEnabled.12"},
		{".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
		{".dot", "application/msword"},
		{".dotm", "application/vnd.ms-word.template.macroEnabled.12"},
		{".dotx", "application/vnd.openxmlformats-officedocument.wordprocessingml.template"},
		{".dsp", "application/octet-stream"},
		{".dsw", "text/plain"},
		{".dtd", "text/xml"},
		{".dtsConfig", "text/xml"},
		{".dv", "video/x-dv"},
		{".dvi", "application/x-dvi"},
		{".dwf", "drawing/x-dwf"},
		{".dwp", "application/octet-stream"},
		{".dxr", "application/x-director"},
		{".eml", "message/rfc822"},
		{".emz", "application/octet-stream"},
		{".eot", "application/octet-stream"},
		{".eps", "application/postscript"},
		{".etl", "application/etl"},
		{".etx", "text/x-setext"},
		{".evy", "application/envoy"},
		{".exe", "application/octet-stream"},
		{".exe.config", "text/xml"},
		{".fdf", "application/vnd.fdf"},
		{".fif", "application/fractals"},
		{".filters", "Application/xml"},
		{".fla", "application/octet-stream"},
		{".flr", "x-world/x-vrml"},
		{".flv", "video/x-flv"},
		{".fsscript", "application/fsharp-script"},
		{".fsx", "application/fsharp-script"},
		{".generictest", "application/xml"},
		{".gif", "image/gif"},
		{".group", "text/x-ms-group"},
		{".gsm", "audio/x-gsm"},
		{".gtar", "application/x-gtar"},
		{".gz", "application/x-gzip"},
		{".h", "text/plain"},
		{".hdf", "application/x-hdf"},
		{".hdml", "text/x-hdml"},
		{".hhc", "application/x-oleobject"},
		{".hhk", "application/octet-stream"},
		{".hhp", "application/octet-stream"},
		{".hlp", "application/winhlp"},
		{".hpp", "text/plain"},
		{".hqx", "application/mac-binhex40"},
		{".hta", "application/hta"},
		{".htc", "text/x-component"},
		{".htm", "text/html"},
		{".html", "text/html"},
		{".htt", "text/webviewhtml"},
		{".hxa", "application/xml"},
		{".hxc", "application/xml"},
		{".hxd", "application/octet-stream"},
		{".hxe", "application/xml"},
		{".hxf", "application/xml"},
		{".hxh", "application/octet-stream"},
		{".hxi", "application/octet-stream"},
		{".hxk", "application/xml"},
		{".hxq", "application/octet-stream"},
		{".hxr", "application/octet-stream"},
		{".hxs", "application/octet-stream"},
		{".hxt", "text/html"},
		{".hxv", "application/xml"},
		{".hxw", "application/octet-stream"},
		{".hxx", "text/plain"},
		{".i", "text/plain"},
		{".ico", "image/x-icon"},
		{".ics", "application/octet-stream"},
		{".idl", "text/plain"},
		{".ief", "image/ief"},
		{".iii", "application/x-iphone"},
		{".inc", "text/plain"},
		{".inf", "application/octet-stream"},
		{".inl", "text/plain"},
		{".ins", "application/x-internet-signup"},
		{".ipa", "application/x-itunes-ipa"},
		{".ipg", "application/x-itunes-ipg"},
		{".ipproj", "text/plain"},
		{".ipsw", "application/x-itunes-ipsw"},
		{".iqy", "text/x-ms-iqy"},
		{".isp", "application/x-internet-signup"},
		{".ite", "application/x-itunes-ite"},
		{".itlp", "application/x-itunes-itlp"},
		{".itms", "application/x-itunes-itms"},
		{".itpc", "application/x-itunes-itpc"},
		{".IVF", "video/x-ivf"},
		{".jar", "application/java-archive"},
		{".java", "application/octet-stream"},
		{".jck", "application/liquidmotion"},
		{".jcz", "application/liquidmotion"},
		{".jfif", "image/pjpeg"},
		{".jnlp", "application/x-java-jnlp-file"},
		{".jpb", "application/octet-stream"},
		{".jpe", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".jpg", "image/jpeg"},
		{".js", "application/x-javascript"},
		{".jsx", "text/jscript"},
		{".jsxbin", "text/plain"},
		{".latex", "application/x-latex"},
		{".library-ms", "application/windows-library+xml"},
		{".lit", "application/x-ms-reader"},
		{".loadtest", "application/xml"},
		{".lpk", "application/octet-stream"},
		{".lsf", "video/x-la-asf"},
		{".lst", "text/plain"},
		{".lsx", "video/x-la-asf"},
		{".lzh", "application/octet-stream"},
		{".m13", "application/x-msmediaview"},
		{".m14", "application/x-msmediaview"},
		{".m1v", "video/mpeg"},
		{".m2t", "video/vnd.dlna.mpeg-tts"},
		{".m2ts", "video/vnd.dlna.mpeg-tts"},
		{".m2v", "video/mpeg"},
		{".m3u", "audio/x-mpegurl"},
		{".m3u8", "audio/x-mpegurl"},
		{".m4a", "audio/m4a"},
		{".m4b", "audio/m4b"},
		{".m4p", "audio/m4p"},
		{".m4r", "audio/x-m4r"},
		{".m4v", "video/x-m4v"},
		{".mac", "image/x-macpaint"},
		{".mak", "text/plain"},
		{".man", "application/x-troff-man"},
		{".manifest", "application/x-ms-manifest"},
		{".map", "text/plain"},
		{".master", "application/xml"},
		{".mda", "application/msaccess"},
		{".mdb", "application/x-msaccess"},
		{".mde", "application/msaccess"},
		{".mdp", "application/octet-stream"},
		{".me", "application/x-troff-me"},
		{".mfp", "application/x-shockwave-flash"},
		{".mht", "message/rfc822"},
		{".mhtml", "message/rfc822"},
		{".mid", "audio/mid"},
		{".midi", "audio/mid"},
		{".mix", "application/octet-stream"},
		{".mk", "text/plain"},
		{".mmf", "application/x-smaf"},
		{".mno", "text/xml"},
		{".mny", "application/x-msmoney"},
		{".mod", "video/mpeg"},
		{".mov", "video/quicktime"},
		{".movie", "video/x-sgi-movie"},
		{".mp2", "video/mpeg"},
		{".mp2v", "video/mpeg"},
		{".mp3", "audio/mpeg"},
		{".mp4", "video/mp4"},
		{".mp4v", "video/mp4"},
		{".mpa", "video/mpeg"},
		{".mpe", "video/mpeg"},
		{".mpeg", "video/mpeg"},
		{".mpf", "application/vnd.ms-mediapackage"},
		{".mpg", "video/mpeg"},
		{".mpp", "application/vnd.ms-project"},
		{".mpv2", "video/mpeg"},
		{".mqv", "video/quicktime"},
		{".ms", "application/x-troff-ms"},
		{".msi", "application/octet-stream"},
		{".mso", "application/octet-stream"},
		{".mts", "video/vnd.dlna.mpeg-tts"},
		{".mtx", "application/xml"},
		{".mvb", "application/x-msmediaview"},
		{".mvc", "application/x-miva-compiled"},
		{".mxp", "application/x-mmxp"},
		{".nc", "application/x-netcdf"},
		{".nsc", "video/x-ms-asf"},
		{".nws", "message/rfc822"},
		{".ocx", "application/octet-stream"},
		{".oda", "application/oda"},
		{".odc", "text/x-ms-odc"},
		{".odh", "text/plain"},
		{".odl", "text/plain"},
		{".odp", "application/vnd.oasis.opendocument.presentation"},
		{".ods", "application/oleobject"},
		{".odt", "application/vnd.oasis.opendocument.text"},
		{".one", "application/onenote"},
		{".onea", "application/onenote"},
		{".onepkg", "application/onenote"},
		{".onetmp", "application/onenote"},
		{".onetoc", "application/onenote"},
		{".onetoc2", "application/onenote"},
		{".orderedtest", "application/xml"},
		{".osdx", "application/opensearchdescription+xml"},
		{".p10", "application/pkcs10"},
		{".p12", "application/x-pkcs12"},
		{".p7b", "application/x-pkcs7-certificates"},
		{".p7c", "application/pkcs7-mime"},
		{".p7m", "application/pkcs7-mime"},
		{".p7r", "application/x-pkcs7-certreqresp"},
		{".p7s", "application/pkcs7-signature"},
		{".pbm", "image/x-portable-bitmap"},
		{".pcast", "application/x-podcast"},
		{".pct", "image/pict"},
		{".pcx", "application/octet-stream"},
		{".pcz", "application/octet-stream"},
		{".pdf", "application/pdf"},
		{".pfb", "application/octet-stream"},
		{".pfm", "application/octet-stream"},
		{".pfx", "application/x-pkcs12"},
		{".pgm", "image/x-portable-graymap"},
		{".pic", "image/pict"},
		{".pict", "image/pict"},
		{".pkgdef", "text/plain"},
		{".pkgundef", "text/plain"},
		{".pko", "application/vnd.ms-pki.pko"},
		{".pls", "audio/scpls"},
		{".pma", "application/x-perfmon"},
		{".pmc", "application/x-perfmon"},
		{".pml", "application/x-perfmon"},
		{".pmr", "application/x-perfmon"},
		{".pmw", "application/x-perfmon"},
		{".png", "image/png"},
		{".pnm", "image/x-portable-anymap"},
		{".pnt", "image/x-macpaint"},
		{".pntg", "image/x-macpaint"},
		{".pnz", "image/png"},
		{".pot", "application/vnd.ms-powerpoint"},
		{".potm", "application/vnd.ms-powerpoint.template.macroEnabled.12"},
		{".potx", "application/vnd.openxmlformats-officedocument.presentationml.template"},
		{".ppa", "application/vnd.ms-powerpoint"},
		{".ppam", "application/vnd.ms-powerpoint.addin.macroEnabled.12"},
		{".ppm", "image/x-portable-pixmap"},
		{".pps", "application/vnd.ms-powerpoint"},
		{".ppsm", "application/vnd.ms-powerpoint.slideshow.macroEnabled.12"},
		{".ppsx", "application/vnd.openxmlformats-officedocument.presentationml.slideshow"},
		{".ppt", "application/vnd.ms-powerpoint"},
		{".pptm", "application/vnd.ms-powerpoint.presentation.macroEnabled.12"},
		{".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
		{".prf", "application/pics-rules"},
		{".prm", "application/octet-stream"},
		{".prx", "application/octet-stream"},
		{".ps", "application/postscript"},
		{".psc1", "application/PowerShell"},
		{".psd", "application/octet-stream"},
		{".psess", "application/xml"},
		{".psm", "application/octet-stream"},
		{".psp", "application/octet-stream"},
		{".pub", "application/x-mspublisher"},
		{".pwz", "application/vnd.ms-powerpoint"},
		{".qht", "text/x-html-insertion"},
		{".qhtm", "text/x-html-insertion"},
		{".qt", "video/quicktime"},
		{".qti", "image/x-quicktime"},
		{".qtif", "image/x-quicktime"},
		{".qtl", "application/x-quicktimeplayer"},
		{".qxd", "application/octet-stream"},
		{".ra", "audio/x-pn-realaudio"},
		{".ram", "audio/x-pn-realaudio"},
		{".rar", "application/octet-stream"},
		{".ras", "image/x-cmu-raster"},
		{".rat", "application/rat-file"},
		{".rc", "text/plain"},
		{".rc2", "text/plain"},
		{".rct", "text/plain"},
		{".rdlc", "application/xml"},
		{".resx", "application/xml"},
		{".rf", "image/vnd.rn-realflash"},
		{".rgb", "image/x-rgb"},
		{".rgs", "text/plain"},
		{".rm", "application/vnd.rn-realmedia"},
		{".rmi", "audio/mid"},
		{".rmp", "application/vnd.rn-rn_music_package"},
		{".roff", "application/x-troff"},
		{".rpm", "audio/x-pn-realaudio-plugin"},
		{".rqy", "text/x-ms-rqy"},
		{".rtf", "application/rtf"},
		{".rtx", "text/richtext"},
		{".ruleset", "application/xml"},
		{".s", "text/plain"},
		{".safariextz", "application/x-safari-safariextz"},
		{".scd", "application/x-msschedule"},
		{".sct", "text/scriptlet"},
		{".sd2", "audio/x-sd2"},
		{".sdp", "application/sdp"},
		{".sea", "application/octet-stream"},
		{".searchConnector-ms", "application/windows-search-connector+xml"},
		{".setpay", "application/set-payment-initiation"},
		{".setreg", "application/set-registration-initiation"},
		{".settings", "application/xml"},
		{".sgimb", "application/x-sgimb"},
		{".sgml", "text/sgml"},
		{".sh", "application/x-sh"},
		{".shar", "application/x-shar"},
		{".shtml", "text/html"},
		{".sit", "application/x-stuffit"},
		{".sitemap", "application/xml"},
		{".skin", "application/xml"},
		{".sldm", "application/vnd.ms-powerpoint.slide.macroEnabled.12"},
		{".sldx", "application/vnd.openxmlformats-officedocument.presentationml.slide"},
		{".slk", "application/vnd.ms-excel"},
		{".sln", "text/plain"},
		{".slupkg-ms", "application/x-ms-license"},
		{".smd", "audio/x-smd"},
		{".smi", "application/octet-stream"},
		{".smx", "audio/x-smd"},
		{".smz", "audio/x-smd"},
		{".snd", "audio/basic"},
		{".snippet", "application/xml"},
		{".snp", "application/octet-stream"},
		{".sol", "text/plain"},
		{".sor", "text/plain"},
		{".spc", "application/x-pkcs7-certificates"},
		{".spl", "application/futuresplash"},
		{".src", "application/x-wais-source"},
		{".srf", "text/plain"},
		{".SSISDeploymentManifest", "text/xml"},
		{".ssm", "application/streamingmedia"},
		{".sst", "application/vnd.ms-pki.certstore"},
		{".stl", "application/vnd.ms-pki.stl"},
		{".sv4cpio", "application/x-sv4cpio"},
		{".sv4crc", "application/x-sv4crc"},
		{".svc", "application/xml"},
		{".swf", "application/x-shockwave-flash"},
		{".t", "application/x-troff"},
		{".tar", "application/x-tar"},
		{".tcl", "application/x-tcl"},
		{".testrunconfig", "application/xml"},
		{".testsettings", "application/xml"},
		{".tex", "application/x-tex"},
		{".texi", "application/x-texinfo"},
		{".texinfo", "application/x-texinfo"},
		{".tgz", "application/x-compressed"},
		{".thmx", "application/vnd.ms-officetheme"},
		{".thn", "application/octet-stream"},
		{".tif", "image/tiff"},
		{".tiff", "image/tiff"},
		{".tlh", "text/plain"},
		{".tli", "text/plain"},
		{".toc", "application/octet-stream"},
		{".tr", "application/x-troff"},
		{".trm", "application/x-msterminal"},
		{".trx", "application/xml"},
		{".ts", "video/vnd.dlna.mpeg-tts"},
		{".tsv", "text/tab-separated-values"},
		{".ttf", "application/octet-stream"},
		{".tts", "video/vnd.dlna.mpeg-tts"},
		{".txt", "text/plain"},
		{".u32", "application/octet-stream"},
		{".uls", "text/iuls"},
		{".user", "text/plain"},
		{".ustar", "application/x-ustar"},
		{".vb", "text/plain"},
		{".vbdproj", "text/plain"},
		{".vbk", "video/mpeg"},
		{".vbproj", "text/plain"},
		{".vbs", "text/vbscript"},
		{".vcf", "text/x-vcard"},
		{".vcproj", "Application/xml"},
		{".vcs", "text/plain"},
		{".vcxproj", "Application/xml"},
		{".vddproj", "text/plain"},
		{".vdp", "text/plain"},
		{".vdproj", "text/plain"},
		{".vdx", "application/vnd.ms-visio.viewer"},
		{".vml", "text/xml"},
		{".vscontent", "application/xml"},
		{".vsct", "text/xml"},
		{".vsd", "application/vnd.visio"},
		{".vsi", "application/ms-vsi"},
		{".vsix", "application/vsix"},
		{".vsixlangpack", "text/xml"},
		{".vsixmanifest", "text/xml"},
		{".vsmdi", "application/xml"},
		{".vspscc", "text/plain"},
		{".vss", "application/vnd.visio"},
		{".vsscc", "text/plain"},
		{".vssettings", "text/xml"},
		{".vssscc", "text/plain"},
		{".vst", "application/vnd.visio"},
		{".vstemplate", "text/xml"},
		{".vsto", "application/x-ms-vsto"},
		{".vsw", "application/vnd.visio"},
		{".vsx", "application/vnd.visio"},
		{".vtx", "application/vnd.visio"},
		{".wav", "audio/wav"},
		{".wave", "audio/wav"},
		{".wax", "audio/x-ms-wax"},
		{".wbk", "application/msword"},
		{".wbmp", "image/vnd.wap.wbmp"},
		{".wcm", "application/vnd.ms-works"},
		{".wdb", "application/vnd.ms-works"},
		{".wdp", "image/vnd.ms-photo"},
		{".webarchive", "application/x-safari-webarchive"},
		{".webtest", "application/xml"},
		{".wiq", "application/xml"},
		{".wiz", "application/msword"},
		{".wks", "application/vnd.ms-works"},
		{".WLMP", "application/wlmoviemaker"},
		{".wlpginstall", "application/x-wlpg-detect"},
		{".wlpginstall3", "application/x-wlpg3-detect"},
		{".wm", "video/x-ms-wm"},
		{".wma", "audio/x-ms-wma"},
		{".wmd", "application/x-ms-wmd"},
		{".wmf", "application/x-msmetafile"},
		{".wml", "text/vnd.wap.wml"},
		{".wmlc", "application/vnd.wap.wmlc"},
		{".wmls", "text/vnd.wap.wmlscript"},
		{".wmlsc", "application/vnd.wap.wmlscriptc"},
		{".wmp", "video/x-ms-wmp"},
		{".wmv", "video/x-ms-wmv"},
		{".wmx", "video/x-ms-wmx"},
		{".wmz", "application/x-ms-wmz"},
		{".wpl", "application/vnd.ms-wpl"},
		{".wps", "application/vnd.ms-works"},
		{".wri", "application/x-mswrite"},
		{".wrl", "x-world/x-vrml"},
		{".wrz", "x-world/x-vrml"},
		{".wsc", "text/scriptlet"},
		{".wsdl", "text/xml"},
		{".wvx", "video/x-ms-wvx"},
		{".x", "application/directx"},
		{".xaf", "x-world/x-vrml"},
		{".xaml", "application/xaml+xml"},
		{".xap", "application/x-silverlight-app"},
		{".xbap", "application/x-ms-xbap"},
		{".xbm", "image/x-xbitmap"},
		{".xdr", "text/plain"},
		{".xht", "application/xhtml+xml"},
		{".xhtml", "application/xhtml+xml"},
		{".xla", "application/vnd.ms-excel"},
		{".xlam", "application/vnd.ms-excel.addin.macroEnabled.12"},
		{".xlc", "application/vnd.ms-excel"},
		{".xld", "application/vnd.ms-excel"},
		{".xlk", "application/vnd.ms-excel"},
		{".xll", "application/vnd.ms-excel"},
		{".xlm", "application/vnd.ms-excel"},
		{".xls", "application/vnd.ms-excel"},
		{".xlsb", "application/vnd.ms-excel.sheet.binary.macroEnabled.12"},
		{".xlsm", "application/vnd.ms-excel.sheet.macroEnabled.12"},
		{".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
		{".xlt", "application/vnd.ms-excel"},
		{".xltm", "application/vnd.ms-excel.template.macroEnabled.12"},
		{".xltx", "application/vnd.openxmlformats-officedocument.spreadsheetml.template"},
		{".xlw", "application/vnd.ms-excel"},
		{".xml", "text/xml"},
		{".xmta", "application/xml"},
		{".xof", "x-world/x-vrml"},
		{".XOML", "text/plain"},
		{".xpm", "image/x-xpixmap"},
		{".xps", "application/vnd.ms-xpsdocument"},
		{".xrm-ms", "text/xml"},
		{".xsc", "application/xml"},
		{".xsd", "text/xml"},
		{".xsf", "text/xml"},
		{".xsl", "text/xml"},
		{".xslt", "text/xml"},
		{".xsn", "application/octet-stream"},
		{".xss", "application/xml"},
		{".xtp", "application/octet-stream"},
		{".xwd", "image/x-xwindowdump"},
		{".z", "application/x-compress"},
		{".zip", "application/x-zip-compressed"}
        //#endregion
	};

	for(int i=0;i<sizeof(ext_mime_maps)/sizeof(EXT_MIME_MAP);i++)
	{
		map.SetAt(ext_mime_maps[i].strExt,ext_mime_maps[i].strMime);
	}

	if(!map.Lookup(strExt,strMime))
		strMime = _T("application/unknown");

	return strMime;
}

CString CCommonFunction::ConvertChineseToPyt(const CString& str,BOOL bInitial /* = FALSE */,TCHAR cSeparator /* = */ )
{
	CString strRet = _T("");
	
	// 汉字转换为拼音数据字典
	const  int pyvalue[] = {-20319,-20317,-20304,-20295,-20292,-20283,-20265,-20257,-20242,-20230,-20051,-20036,-20032,-20026,  
		-20002,-19990,-19986,-19982,-19976,-19805,-19784,-19775,-19774,-19763,-19756,-19751,-19746,-19741,-19739,-19728,  
		-19725,-19715,-19540,-19531,-19525,-19515,-19500,-19484,-19479,-19467,-19289,-19288,-19281,-19275,-19270,-19263,  
		-19261,-19249,-19243,-19242,-19238,-19235,-19227,-19224,-19218,-19212,-19038,-19023,-19018,-19006,-19003,-18996,  
		-18977,-18961,-18952,-18783,-18774,-18773,-18763,-18756,-18741,-18735,-18731,-18722,-18710,-18697,-18696,-18526,  
		-18518,-18501,-18490,-18478,-18463,-18448,-18447,-18446,-18239,-18237,-18231,-18220,-18211,-18201,-18184,-18183,  
		-18181,-18012,-17997,-17988,-17970,-17964,-17961,-17950,-17947,-17931,-17928,-17922,-17759,-17752,-17733,-17730,  
		-17721,-17703,-17701,-17697,-17692,-17683,-17676,-17496,-17487,-17482,-17468,-17454,-17433,-17427,-17417,-17202,  
		-17185,-16983,-16970,-16942,-16915,-16733,-16708,-16706,-16689,-16664,-16657,-16647,-16474,-16470,-16465,-16459,  
		-16452,-16448,-16433,-16429,-16427,-16423,-16419,-16412,-16407,-16403,-16401,-16393,-16220,-16216,-16212,-16205,  
		-16202,-16187,-16180,-16171,-16169,-16158,-16155,-15959,-15958,-15944,-15933,-15920,-15915,-15903,-15889,-15878,  
		-15707,-15701,-15681,-15667,-15661,-15659,-15652,-15640,-15631,-15625,-15454,-15448,-15436,-15435,-15419,-15416,  
		-15408,-15394,-15385,-15377,-15375,-15369,-15363,-15362,-15183,-15180,-15165,-15158,-15153,-15150,-15149,-15144,  
		-15143,-15141,-15140,-15139,-15128,-15121,-15119,-15117,-15110,-15109,-14941,-14937,-14933,-14930,-14929,-14928,  
		-14926,-14922,-14921,-14914,-14908,-14902,-14894,-14889,-14882,-14873,-14871,-14857,-14678,-14674,-14670,-14668,  
		-14663,-14654,-14645,-14630,-14594,-14429,-14407,-14399,-14384,-14379,-14368,-14355,-14353,-14345,-14170,-14159,  
		-14151,-14149,-14145,-14140,-14137,-14135,-14125,-14123,-14122,-14112,-14109,-14099,-14097,-14094,-14092,-14090,  
		-14087,-14083,-13917,-13914,-13910,-13907,-13906,-13905,-13896,-13894,-13878,-13870,-13859,-13847,-13831,-13658,  
		-13611,-13601,-13406,-13404,-13400,-13398,-13395,-13391,-13387,-13383,-13367,-13359,-13356,-13343,-13340,-13329,  
		-13326,-13318,-13147,-13138,-13120,-13107,-13096,-13095,-13091,-13076,-13068,-13063,-13060,-12888,-12875,-12871,  
		-12860,-12858,-12852,-12849,-12838,-12831,-12829,-12812,-12802,-12607,-12597,-12594,-12585,-12556,-12359,-12346,  
		-12320,-12300,-12120,-12099,-12089,-12074,-12067,-12058,-12039,-11867,-11861,-11847,-11831,-11798,-11781,-11604,  
		-11589,-11536,-11358,-11340,-11339,-11324,-11303,-11097,-11077,-11067,-11055,-11052,-11045,-11041,-11038,-11024,  
		-11020,-11019,-11018,-11014,-10838,-10832,-10815,-10800,-10790,-10780,-10764,-10587,-10544,-10533,-10519,-10331,  
		-10329,-10328,-10322,-10315,-10309,-10307,-10296,-10281,-10274,-10270,-10262,-10260,-10256,-10254};  

	const char pystr[396][7] = {"a","ai","an","ang","ao","ba","bai","ban","bang","bao","bei","ben","beng","bi","bian","biao",  
		"bie","bin","bing","bo","bu","ca","cai","can","cang","cao","ce","ceng","cha","chai","chan","chang","chao","che","chen",  
		"cheng","chi","chong","chou","chu","chuai","chuan","chuang","chui","chun","chuo","ci","cong","cou","cu","cuan","cui",  
		"cun","cuo","da","dai","dan","dang","dao","de","deng","di","dian","diao","die","ding","diu","dong","dou","du","duan",  
		"dui","dun","duo","e","en","er","fa","fan","fang","fei","fen","feng","fo","fou","fu","ga","gai","gan","gang","gao",  
		"ge","gei","gen","geng","gong","gou","gu","gua","guai","guan","guang","gui","gun","guo","ha","hai","han","hang",  
		"hao","he","hei","hen","heng","hong","hou","hu","hua","huai","huan","huang","hui","hun","huo","ji","jia","jian",  
		"jiang","jiao","jie","jin","jing","jiong","jiu","ju","juan","jue","jun","ka","kai","kan","kang","kao","ke","ken",  
		"keng","kong","kou","ku","kua","kuai","kuan","kuang","kui","kun","kuo","la","lai","lan","lang","lao","le","lei",  
		"leng","li","lia","lian","liang","liao","lie","lin","ling","liu","long","lou","lu","lv","luan","lue","lun","luo",  
		"ma","mai","man","mang","mao","me","mei","men","meng","mi","mian","miao","mie","min","ming","miu","mo","mou","mu",  
		"na","nai","nan","nang","nao","ne","nei","nen","neng","ni","nian","niang","niao","nie","nin","ning","niu","nong",  
		"nu","nv","nuan","nue","nuo","o","ou","pa","pai","pan","pang","pao","pei","pen","peng","pi","pian","piao","pie",  
		"pin","ping","po","pu","qi","qia","qian","qiang","qiao","qie","qin","qing","qiong","qiu","qu","quan","que","qun",  
		"ran","rang","rao","re","ren","reng","ri","rong","rou","ru","ruan","rui","run","ruo","sa","sai","san","sang",  
		"sao","se","sen","seng","sha","shai","shan","shang","shao","she","shen","sheng","shi","shou","shu","shua",  
		"shuai","shuan","shuang","shui","shun","shuo","si","song","sou","su","suan","sui","sun","suo","ta","tai",  
		"tan","tang","tao","te","teng","ti","tian","tiao","tie","ting","tong","tou","tu","tuan","tui","tun","tuo",  
		"wa","wai","wan","wang","wei","wen","weng","wo","wu","xi","xia","xian","xiang","xiao","xie","xin","xing",  
		"xiong","xiu","xu","xuan","xue","xun","ya","yan","yang","yao","ye","yi","yin","ying","yo","yong","you",  
		"yu","yuan","yue","yun","za","zai","zan","zang","zao","ze","zei","zen","zeng","zha","zhai","zhan","zhang",  
		"zhao","zhe","zhen","zheng","zhi","zhong","zhou","zhu","zhua","zhuai","zhuan","zhuang","zhui","zhun","zhuo",  
		"zi","zong","zou","zu","zuan","zui","zun","zuo"};  

	// 转换成Unicode编码
	string szU8 = str;  
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szU8.c_str(), strlen( szU8.c_str()), NULL, 0);  
	wchar_t* wszString = new wchar_t[wcsLen + 1];  
	memset(wszString,0,wcsLen+1);
	::MultiByteToWideChar(CP_ACP, NULL,  szU8.c_str(), strlen( szU8.c_str()), wszString, wcsLen);  
	wszString[wcsLen] = '\0';  

	int chrasc = 0;
	//unicode转换为ANSI  
	int length = WideCharToMultiByte(CP_ACP, 0, wszString, -1, 0, 0, 0, 0 );   
	char* nowchar = new char[length+1];  
	memset(nowchar,0,length+1);  
	WideCharToMultiByte( CP_ACP, 0, wszString, -1, nowchar,length, 0, 0 );  
	nowchar[length] = '\0';

	//转换ANSI,字符部分不变，汉字转换成相应的拼音,增加一个字符用于存放分隔符  
	char *returnstr = new char[6*length+6+1];  
	memset(returnstr,0,6*length+6+1);  

	int offset = 0;   
	for (int j = 0; j < length;) // 循环处理字节数组  
	{  
		if (nowchar[j] >= 0 && nowchar[j] < 128) // 非汉字处理  
		{  
			returnstr[offset] = nowchar[j];  
			offset++;
			// 插入分隔符
			if( cSeparator !='\0' )
			{
				returnstr[offset] = cSeparator;
				offset++;
			}			

			j++;  
			continue;  
		}  

		// 汉字处理  
		chrasc = nowchar[j] * 256 + nowchar[j+1]+256;  

		if (chrasc > 0 && chrasc < 160)  
		{  
			returnstr[offset]= nowchar[j];  
			offset++;  
			// 插入分隔符
			if( cSeparator !='\0' )
			{
				returnstr[offset] = cSeparator;
				offset++;
			}	

			j++;  
		}  
		else  
		{  
			for (int i = (sizeof(pyvalue)/sizeof(pyvalue[0]) - 1); i >= 0; i--)  
			{  
				if (pyvalue[i] <= chrasc)  
				{  
					// 如果只取首字母
					if( bInitial )
					{
						returnstr[offset] = pystr[i][0];
						offset++;
					}
					else
					{
						strcpy(returnstr+offset ,pystr[i]);
						offset +=strlen(pystr[i]); 
					}				
					// 插入分隔符
					if( cSeparator !='\0' )
					{
						returnstr[offset] = cSeparator;
						offset++;
					}	

					break;  
				}  
			}  
			j +=2;  
		}  
	}  
	if(strlen(returnstr)>0)  
	{  
		strRet = returnstr;
	}  
	delete []returnstr;  
	delete []nowchar;  
	delete []wszString;

	return strRet;  
}

int  CCommonFunction::ResamplePicture(const CString& strPath,const CString& strNewPath,float lNewWidth,float lNewHeight,int nInterpolation)
{
	 int nSize = 0;
	 //调整图片大小
	 CxImage pic;
	 CString ext = strPath.Mid(strPath.ReverseFind('.')+1);

	 if( pic.Load(strPath,CxImage::GetTypeIdFromName(ext)) )
	 {	
		 float ratioPic = (float)pic.GetWidth()/pic.GetHeight();
		 // Modified by jjp 2017.06.09
		 // 适配图片按原比例压缩

		 if( ratioPic < 1 )
		 {			 
			 lNewWidth = (pic.GetWidth() > lNewWidth ) ? lNewWidth : pic.GetWidth();
			 lNewHeight = (pic.GetWidth() > lNewWidth ) ? lNewWidth/ratioPic : pic.GetHeight();
		 }
		 else
		 {
			 lNewHeight = (pic.GetHeight() > lNewHeight ) ? lNewHeight : pic.GetHeight();
			 lNewWidth = (pic.GetHeight() > lNewHeight ) ? lNewHeight*ratioPic : pic.GetWidth();			 
		 }	
		 
		//picture resample options
		CxImage::InterpolationMethod rintm;
		switch ( nInterpolation ) 
		{
		case 0: rintm=CxImage::IM_NEAREST_NEIGHBOUR; break;
		case 1: rintm=CxImage::IM_BILINEAR; break;
		case 2: rintm=CxImage::IM_BILINEAR; break;
		case 3: rintm=CxImage::IM_BICUBIC; break;
		case 4: rintm=CxImage::IM_BICUBIC2; break;
		case 5: rintm=CxImage::IM_BSPLINE; break;
		case 6: rintm=CxImage::IM_LANCZOS; break;
		case 7: rintm=CxImage::IM_HERMITE; break;
		default: throw(0);
		}//switch
		switch ( nInterpolation )
		{
		case 0:
			pic.Resample(lNewWidth,lNewHeight,1);
			break;
		case 1:
			pic.Resample(lNewWidth,lNewHeight,0);
			break;
		case 2:
			if ( (pic.GetWidth()>lNewWidth) && (pic.GetHeight()>lNewHeight))
				pic.QIShrink(lNewWidth,lNewHeight);
			else
				pic.Resample2(lNewWidth,lNewHeight,rintm,CxImage::OM_REPEAT);
			break;
		default:
			pic.Resample2(lNewWidth,lNewHeight,rintm,CxImage::OM_REPEAT);
		}							
		if( pic.Save(strNewPath,CxImage::GetTypeIdFromName(ext)) )
			nSize = pic.GetSize();
	 }	
	 return nSize;
}

BOOL CCommonFunction::PictureIsValid(const CString& strPath)
{
	BOOL bRet = FALSE;
	
	CString ext = strPath.Mid(strPath.ReverseFind('.')+1);
	CxImage pic;

	if( pic.Load(strPath,CxImage::GetTypeIdFromName(ext)) )
	{	
		bRet = pic.IsValid();
	}

	return bRet;
}