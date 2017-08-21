#include "stdafx.h"
#include "FtpManager.h " 
#include "CommonFunction.h " 
#include "MainFrm.h"

#pragma comment(lib, "Wininet.lib" )

#ifdef _DEBUG
#include  "afxinet.h" 
#endif  //  _DEBUG 

#ifdef _DEBUG
#undef THIS_FILE
static  char  THIS_FILE[] = __FILE__;
#define  new  DEBUG_NEW
#endif

// //////////////////////////////////////////////////////////////////// 
//  Construction/Destruction 
// //////////////////////////////////////////////////////////////////// 
template <class T>
CFtpManager<T>::CFtpManager(DWORD dwTimeOut)
{
	m_dwTimeOut  =  dwTimeOut  *   1000 ;
	m_hStartFtpOpen  =  NULL;
	m_hStartFtpConnct  =  NULL;
	m_hFtpFile  =  NULL;
	m_bCancleUpLoad  =  FALSE;
	m_lpfnInternetCallback  =  NULL;
//	ZeroMemory(m_lpParam,sizeof(m_lpParam));
}
template <class T>
CFtpManager<T>:: ~ CFtpManager()
{
	DisConnectFtp();
}
template <class T>
void  CFtpManager<T>::DisConnectFtp()
{
	if (m_hFtpFile)
	{
		InternetCloseHandle(m_hFtpFile);
		m_hFtpFile  =  NULL;
	}
	if (m_hStartFtpConnct)
	{
		static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(TEXT( " Disconnect FTP Connection.\n " ));
		InternetCloseHandle(m_hStartFtpConnct);
		m_hStartFtpConnct  =  NULL;
	}
	if (m_hStartFtpOpen)
	{
		InternetCloseHandle(m_hStartFtpOpen);
		m_hStartFtpOpen  =  NULL;
	}
}
template <class T>
BOOL CFtpManager<T>::ConnectFtp(LPCTSTR lpServer, UINT uPort, LPCTSTR lpUser, LPCTSTR lpPass, BOOL bPassive)
{
	BOOL bRet  =  FALSE;
	if (lpServer)
		m_strServer  =  lpServer;
	m_uPort  =  uPort;
	if (lpUser)
		m_strUser  =  lpUser;
	if (lpPass)
		m_strUserPass  =  lpPass;
	m_bPassive  =  bPassive;
	CCommonFunction::RemovePrefixString(const_cast < LPTSTR >  (m_strServer.c_str()) , TEXT( " ftp:// " ),FALSE);
	return  ReConnectFtp( 0 );
}
template <class T>
BOOL CFtpManager<T>::ReConnectFtp(DWORD dwDelay)
{
	BOOL bRet  =  FALSE;
	CString strInfo;

	DisConnectFtp();
	Sleep(dwDelay * 1000 );
	DWORD dwFlags  =   0 ;
	InternetGetConnectedState( & dwFlags,  0 );
	m_hStartFtpOpen  =  InternetOpen(TEXT( "Wdm_FtpConnect" ),
		(dwFlags  &  INTERNET_CONNECTION_PROXY)  ==  INTERNET_CONNECTION_PROXY  ?  INTERNET_OPEN_TYPE_PRECONFIG : INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,
		NULL , NULL ,  0 );

	if ( ! m_hStartFtpOpen)
	{
		strInfo.Format(TEXT( " Erron On Open Internet Connection. Err:%d\n " ) , GetLastError());
		static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
		return  bRet;
	}

	//  --set timeout durations for connects, sends, and receives. 

	if  (FALSE  ==  InternetSetOption(m_hStartFtpOpen, 
		INTERNET_OPTION_RECEIVE_TIMEOUT,  & m_dwTimeOut, 
		sizeof(DWORD)))
	{
		//  not necessarily a serious error, but report it anyway. 
		strInfo.Format(TEXT( " Erron On Set Connection Receive Timeout. Err:%d\n " ) , GetLastError());
		static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
	}

	if  (FALSE  ==  InternetSetOption(m_hStartFtpOpen, 
		INTERNET_OPTION_SEND_TIMEOUT,  & m_dwTimeOut, 
		sizeof(DWORD)))
	{
		//  not necessarily a serious error, but report it anyway. 
		strInfo.Format(TEXT( " Erron On Set Connection Send Timeout. Err:%d\n " ) , GetLastError());
		static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
	}

	if  (FALSE  ==  InternetSetOption(m_hStartFtpOpen, 
		INTERNET_OPTION_CONNECT_TIMEOUT,  & m_dwTimeOut, 
		sizeof(DWORD)))
	{
		//  not necessarily a serious error, but report it anyway. 
		strInfo.Format(TEXT( " Erron On Set Connection Connect Timeout. Err:%d\n " ) , GetLastError());
		static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
	}

	m_hStartFtpConnct  =  InternetConnect(m_hStartFtpOpen,m_strServer.c_str(),m_uPort,
		                                  m_strUser.c_str() ,m_strUserPass.c_str() , 
		                                  INTERNET_SERVICE_FTP ,
										  ( m_bPassive  ?  INTERNET_FLAG_PASSIVE :  0  ) , NULL );

	if ( ! m_hStartFtpConnct)
	{
		strInfo.Format(TEXT( " Erron On FTP Connection. Err:%d\n " ) , GetLastError());
		static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
		DisConnectFtp();
		return  bRet;
	}

	static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(TEXT( " Connect To FTP Server.\n " ));
//  InternetSetStatusCallback(m_hStartFtpConnct,m_lpfnInternetCallback ? m_lpfnInternetCallback : InternetStatusCallbackProc); 

	return  TRUE;
}
template <class T>
BOOL CFtpManager<T>::ChangeCWD(LPCTSTR lpWorkPath)
{
	BOOL bRet  =  FALSE;
	CString strInfo;

	if (m_hStartFtpConnct  &&  lpWorkPath)
	{
		// CStaticFunctions::RemovePostfixString(const_cast<LPTSTR> (strWorkPath.c_str()) , TEXT("/")); 
		if ( FtpSetCurrentDirectory(m_hStartFtpConnct,lpWorkPath)  ==  TRUE)
			bRet  =  TRUE;
		else 
		{
			strInfo.Format(TEXT( " Erron On Set Current FTP Directory. Err:%d\n " ) , GetLastError());
			static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
//          TCHAR szError[MAX_PATH] = {0}; 
//          DWORD dwErrorSize = MAX_PATH; 
//          DWORD dwLastErrorMsg = 0; 
//          InternetGetLastResponseInfo(&dwLastErrorMsg,szError,&dwErrorSize); 
		}
	}

	return  bRet;
}
template <class T>
int CFtpManager<T>::FtpFileExist(LPCTSTR lpFtpFileName)
{
	int  iRet  =   - 2 ;
	if ( ! lpFtpFileName)
		return   0 ;

	TCHAR szError[MAX_PATH]  =  { 0 };
	DWORD dwErrorSize  =  MAX_PATH;
	DWORD dwLastErrorMsg  =   0 ;
	HINTERNET hFind  =  NULL;
	std::string strCmd  =  TEXT( " SIZE  " );
	strCmd += lpFtpFileName;

	//  -- Try Receive File Size. 
	if ( FtpCommand(m_hStartFtpConnct,FALSE,FTP_TRANSFER_TYPE_ASCII,strCmd.c_str(),(DWORD_PTR) this ,NULL) )
	{
		InternetGetLastResponseInfo( & dwLastErrorMsg,szError, & dwErrorSize);
		dwLastErrorMsg  =  FtpErrorMsgToCode(szError);
		if ( dwLastErrorMsg == 550 )
		{
			//  550 File Not Found,  
			iRet  =   0 ;
		}
		else   if (dwLastErrorMsg  ==   500 )
		{
			//  500 Unknown Command. 
			iRet  =   - 1 ;
		}
		else   if (dwLastErrorMsg == 213 )
		{
			//  213 Return Size 
			iRet  =   1 ;
		}
	}
	else 
	{
		if (GetLastError() == ERROR_INTERNET_EXTENDED_ERROR)
		{
			InternetGetLastResponseInfo( & dwLastErrorMsg,szError, & dwErrorSize);
			dwLastErrorMsg  =  FtpErrorMsgToCode(szError);
			// ERROR_INVALID_PARAMETER 
			if ( dwLastErrorMsg == 550 )
			{
				//  550 File Not Found,  
				iRet  =   0 ;
			}
			else   if (dwLastErrorMsg  ==   500 )
			{
				//  500 Unknown Command. 
				iRet  =   - 1 ;
			}
		}
	}

	//  -- If Server Not Support SIZE Command, Try Use FtpOpenFile to Test it . 
	if (iRet ==- 1 )
	{
		HINTERNET hOpenTest  =  FtpOpenFile(m_hStartFtpConnct,lpFtpFileName,GENERIC_READ,FTP_TRANSFER_TYPE_BINARY,(DWORD_PTR) this );
		if (hOpenTest)
		{
			InternetCloseHandle(hOpenTest);
			//  -- Open Success,File Exist. 
			iRet  =   1 ;
		}
		else 
		{
			DWORD dwLastError  =  GetLastError();
			dwErrorSize  =  MAX_PATH;
			if (dwLastError == ERROR_INTERNET_EXTENDED_ERROR)
			{
				InternetGetLastResponseInfo( & dwLastError,szError, & dwErrorSize);
				CString strCmdRet  =  szError;
				if ( strCmdRet.Find(TEXT( " 553 " )) !=- 1   ||  strCmdRet.Find(TEXT( " 550 " )) !=- 1 )
					iRet  =   0 ;
				else 
				{
					strCmdRet.Format(TEXT( " FtpFileExist FtpOpenFile Error MSG:\n%s\n " ) , szError);
					static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strCmdRet);

					TRACE( " FtpFileExist Error MSG:\n%s\n "  , szError);
				}
			}
		}
	}

	//  -- If FtpOpenFile Break A Unknown Error , We Try FtpFindFirstFile to get it. 
	if (iRet  ==   - 1 )
	{
		WIN32_FIND_DATA FindInfo  =  { 0 };
		hFind  =  FtpFindFirstFile(m_hStartFtpConnct,lpFtpFileName, & FindInfo,  0  , (DWORD_PTR) this );
		if (hFind)
		{
			InternetCloseHandle(hFind);
			iRet  =   1 ;
		}
		else 
		{
			// ERROR_INTERNET_EXTENDED_ERROR. 
			DWORD dwLastError  =  GetLastError();
			dwErrorSize  =  MAX_PATH;
			if (dwLastError == ERROR_INTERNET_EXTENDED_ERROR)
			{
				InternetGetLastResponseInfo( & dwLastError,szError, & dwErrorSize);
				CString strCmdRet  =  szError;
				if ( strCmdRet.Find(TEXT( " 553 " )) !=- 1   ||  strCmdRet.Find(TEXT( " 550 " )) !=- 1 )
					iRet  =   0 ;
				else 
				{
					strCmdRet.Format(TEXT( " FtpFileExist FtpFindFirstFile Error MSG:\n%s\n " ) , szError);
					static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strCmdRet);

					TRACE( " FtpFileExist Error MSG:\n%s\n "  , szError);
				}
			}
		}

	}
	return  iRet;
}
template <class T>
void  CFtpManager<T>::InternetStatusCallbackProc(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	CFtpManager  * pThis  =  (CFtpManager  * )dwContext;
	TRACE(TEXT( " Status = %d\n " ),dwInternetStatus); // INTERNET_STATUS_CLOSING_CONNECTION INTERNET_FLAG_RELOAD 
	if (dwInternetStatus == INTERNET_STATUS_RESPONSE_RECEIVED)
		TRACE(TEXT( " \tRecive Size %d\n " ), * (PDWORD)lpvStatusInformation);
	else   if (dwInternetStatus == INTERNET_STATUS_REQUEST_SENT)
		TRACE(TEXT( " \tSend Size %d\n " ), * (PDWORD)lpvStatusInformation);

}
template <class T>
DWORD CFtpManager<T>::FtpErrorMsgToCode(LPCTSTR lpErrorMsg)
{
	DWORD dwRet  =  StrToInt(lpErrorMsg);
	return  dwRet;
}
template <class T>
BOOL CFtpManager<T>::FtpUpLoadFile(LPCTSTR lpLocalFile, LPCTSTR lpSavedName)
{
	BOOL bRet  =  FALSE;
	if (::PathFileExists(lpLocalFile))
	{
		CString strUpName;
		if (lpSavedName)
			strUpName  =  lpSavedName;
		else 
			strUpName  =  ::PathFindFileName(lpLocalFile);

		InternetSetStatusCallback(m_hStartFtpConnct,m_lpfnInternetCallback  ?  m_lpfnInternetCallback : InternetStatusCallbackProc);
		// This Function Can't Work With PassiveMode Correctly? 
		bRet  =  FtpPutFile(m_hStartFtpConnct,lpLocalFile,strUpName,FTP_TRANSFER_TYPE_BINARY,(DWORD_PTR) this );
		TCHAR szError[MAX_PATH]  =  { 0 };
		DWORD dwErrorSize  =  MAX_PATH;
		DWORD dwLastErrorMsg  =   0 ;
		if ( ! bRet)
		{
			CString strInfo;
			InternetGetLastResponseInfo( & dwLastErrorMsg,szError, & dwErrorSize);
			strInfo.Format(TEXT( " Erron On FtpPutFile. Err:%d\n " ) , GetLastError());
			static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);
			dwErrorSize  =  MAX_PATH;
		}
		InternetSetStatusCallback(m_hStartFtpConnct,NULL);
	}
	return  bRet;
}
template <class T>
BOOL CFtpManager<T>::FtpFindFiles(LPCTSTR lpDirectory ,DWORD dwListType ,ListFtpServerFilesCallBackProc lpfnCallBack,LPVOID lpParam,LPCTSTR lpExt)
{
	BOOL bRet  =  FALSE;
	BOOL bUserCancle  =  FALSE;
	{
		HINTERNET hFind  =  NULL;
		int  iMaxLen  =  lstrlen(lpDirectory) + 16 ;
		std::string strListPath;
		strListPath.resize(iMaxLen);
		if (lpDirectory)
			CCommonFunction::AddPostfixString(const_cast < LPTSTR > (strListPath.c_str()),lpDirectory,TEXT( " / " ),iMaxLen,FALSE);
		strListPath.resize(lstrlen(strListPath.c_str()));
		if (lpExt)
			strListPath  +=  lpExt;
		else 
			strListPath  +=  TEXT( " *.* " );

		WIN32_FIND_DATA m_FindFileData  =  { 0 };
		hFind  =  FtpFindFirstFile(m_hStartFtpConnct,strListPath.c_str(),  & m_FindFileData,  0  , (DWORD_PTR) this );
		if  (hFind  !=  NULL)  
		{    
			do  
			{
				if ( lstrcmp(m_FindFileData.cFileName, TEXT( " . " )) == 0   ||  lstrcmp(m_FindFileData.cFileName, TEXT( " .. " )) == 0  )
					continue ;
				strListPath.resize(iMaxLen);
				if (lpDirectory)
					CCommonFunction::AddPostfixString(const_cast < LPTSTR > (strListPath.c_str()),lpDirectory,TEXT( " / " ),iMaxLen,FALSE);
				else 
					strListPath.resize( 0 );
				strListPath.resize(lstrlen(strListPath.c_str()));
				strListPath  +=  m_FindFileData.cFileName;
				if ( (m_FindFileData.dwFileAttributes  &  FILE_ATTRIBUTE_DIRECTORY) )
				{
					//  -- Test if Find File is Directory. 
					if ( dwListType  &  FileTypeDirectory )
					{
						if (  ! lpfnCallBack( this ,strListPath.c_str(),m_FindFileData,lpParam) )
						{
							bUserCancle  =  TRUE;
							break ;
						}
					}

					if (dwListType  &  FileTypeSubDirectory)
					{
						bRet  =  FtpFindFiles(strListPath.c_str(),dwListType,lpfnCallBack,lpParam,lpExt);
						if (GetLastError() == ERROR_CANCELLED)
						{
							bUserCancle  =  TRUE;
							break ;
						}
					}
				}
				else 
				{
					if ( dwListType  &  FileTypeFile )
					{
						if (  ! lpfnCallBack( this ,strListPath.c_str(),m_FindFileData,lpParam) )
						{
							bUserCancle  =  TRUE;
							break ;
						}
					}
				}                                
			}  while (InternetFindNextFile(hFind,  & m_FindFileData)  !=   0 );

			InternetCloseHandle(hFind);
			bRet  =  TRUE;
		}
		else 
		{
			TCHAR szError[MAX_PATH]  =  { 0 };
			DWORD dwErrorSize  =  MAX_PATH;
			DWORD dwLastErrorMsg  =   0 ;
			InternetGetLastResponseInfo( & dwLastErrorMsg,szError, & dwErrorSize);
			CString strInfo;
			strInfo.Format(TEXT( " FtpFindFiles return NULL. Err:%d\n " ) , GetLastError());
			static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(strInfo);

			TRACE( " FtpFindFiles Error MSG:\n%s\n "  , szError);
		}
	}
	if (bUserCancle)
	{
		SetLastError(ERROR_CANCELLED);
	}
	else 
		SetLastError(ERROR_SUCCESS);
	return  bRet;    
}
