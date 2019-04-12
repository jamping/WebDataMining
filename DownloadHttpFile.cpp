// DownloadHttpFile.cpp: implementation of the CDownloadHttpFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "DownloadHttpFile.h"
#include <boost/regex/mfc.hpp>
#include "Url.h"
//#include "CharsetDetector.h"

#pragma comment(lib,"Wininet.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDownloadHttpFile::CDownloadHttpFile(HWND hMessageWnd):CDownloadEngine(),m_hMessageWnd(hMessageWnd)
{
}
CDownloadHttpFile::CDownloadHttpFile()
{

}
CDownloadHttpFile::~CDownloadHttpFile()
{

}

void CDownloadHttpFile::SendDownloadMessage(CString strInfo)
{
	if(m_hMessageWnd)
	{
		::SendMessage(m_hMessageWnd,WM_PROCESS_INFO,0,(LPARAM)strInfo.GetBuffer(0));
		strInfo.ReleaseBuffer();
	}
}
//return more than 0 success,or...
int CDownloadHttpFile::Fetch(CString strURL,char** pFileBuf,int& nFileLen)
{
	DWORD dwFlags;
	//检查网络连接状态
	if(!InternetGetConnectedState(&dwFlags, 0))
	{
		SendDownloadMessage(_T("网络连接失败！"));
		return INTERNET_ERROR_OPEN;
	}
	CHAR strAgent[64];
	sprintf(strAgent, "Agent%ld", timeGetTime());
	CHAR   szHead[] = "Accept: */*\r\n\r\n";
	HINTERNET  hOpen,hConnect;
    //建立连接
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hOpen)
	{
		// 处理网页重定向和跳转
		while(TRUE)
		{
			// 打开连接
			if ( !(hConnect = InternetOpenUrlA ( hOpen, strURL, szHead,
				lstrlenA (szHead), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
			{
				SendDownloadMessage(_T("访问失败：")+strURL);
				InternetCloseHandle(hOpen);

				return INTERNET_ERROR_OPENURL;
			}			  
			// 网页重定向
			DWORD dwRet, dwLen; 
			char szTemp[1024]; 
			dwLen = sizeof(szTemp); 
			//查询服务器请求信息
			HttpQueryInfo(hConnect,HTTP_QUERY_STATUS_CODE,szTemp,&dwLen,NULL); 
			dwRet = (DWORD)atoi(szTemp); 
			// 是否需要链接重定向
			if(dwRet == HTTP_STATUS_MOVED || dwRet == HTTP_STATUS_REDIRECT || dwRet == HTTP_STATUS_REDIRECT_METHOD) 
			{ 
				// 获得新地址
				CString strNewUrl;
				HttpQueryInfo(hConnect,HTTP_QUERY_LOCATION,NULL,&dwLen,0); 
				HttpQueryInfo(hConnect,HTTP_QUERY_LOCATION, strNewUrl.GetBufferSetLength(dwLen),&dwLen,0); 
				strNewUrl.ReleaseBuffer(dwLen); 

				CUrl url;
				url.ParseUrlEx(strURL);
				strURL = url.MakeURL(strNewUrl);

				TRACE("the redirect page url is %s\n",strURL);
				// 下载重定向页面
				continue;
			}			
			// 开始下载
			SendDownloadMessage(_T("开始下载：")+strURL);		

			DWORD dwByteToRead = 0;
			DWORD dwSizeOfRq = 4;	
			// 读取头信息, 确定文件长度
			if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
				(LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
			{
				dwByteToRead = 1024*1024;
			}
			int nBufLen=dwByteToRead+1;
			char* buf=(char*)malloc(nBufLen);
			if(buf==NULL)
			{
				InternetCloseHandle(hOpen);
				SendDownloadMessage(_T("分配内存失败：")+strURL);
				return ALLOCATE_MEMORY_ERROR;
			}
			buf[0]='\0';

			DWORD start;
			DWORD end;
			DWORD time;
			time = 10;	
			DWORD dwBytes = 0;
			DWORD dwSize;
			CString strInfo;
			const DWORD FILE_BUFSIZE=16*1024;		
			VOID* temp[FILE_BUFSIZE];

			start = timeGetTime();
			do
			{
				// Keep coping in 16 KB chunks, while file has any data left.
				// Note: bigger buffer will greatly improve performance.
				if (!InternetReadFile (hConnect, temp, FILE_BUFSIZE,  &dwSize) )
				{	
					SendDownloadMessage(_T("下载失败：")+strURL);	
					if(buf)
					{
						free(buf);	buf=NULL;
					}
					InternetCloseHandle(hOpen);

					return INTERNET_ERROR_READFILE;
				}
				if (!dwSize)
					break;  // Condition of dwSize=0 indicate EOF. Stop.
				else
				{
					if(CheckBufSize(&buf,&nBufLen,dwSize)==0)
					{
						memcpy(&buf[dwBytes],temp,dwSize);	
					}
					else
					{
						SendDownloadMessage(_T("内存空间不足：")+strURL);
						if (buf)
						{
							free(buf);buf=NULL;
						}
						InternetCloseHandle(hOpen);
						return ALLOCATE_MEMORY_ERROR;
					}
				}
				dwBytes+=dwSize;

				FLOAT fSpeed = 0;
				fSpeed = (float)dwBytes;
				fSpeed /= ((float)time)/1000.0f;
				fSpeed /= 1024.0f;

				strInfo.Format("%d KB / %d KB @ %1.1f KB/s", dwBytes/1024, dwByteToRead/1024, fSpeed);
				SendDownloadMessage(_T("下载速度:")+strInfo);

				end = timeGetTime();
				time = end - start;
				if(time == 0)
					time = 10;
			}   // do
			while (TRUE);		
			
			//清除多余内存空间
			if((int)dwBytes<nBufLen-1)
			{            
				char* newBuf=(char*)realloc(buf,dwBytes+1);
				if(newBuf==NULL)
				{
					SendDownloadMessage(_T("清除多余内存失败：")+strURL);
				}
				else
					buf=newBuf;
			}		
			buf[dwBytes]='\0';
				
			//////////////////////////////////////////////////////////////////////////
			// 此段代码不太符合实际---------2012.04.28修改
			//
			// 分析网页有无跳转
			// <META HTTP-EQUIV="REFRESH" CONTENT="0; URL=paperindex.htm">
			// <META HTTP-EQUIV="REFRESH" CONTENT="0; URL=html/2010-08/27/nbs.D110000renmrb_01.htm">
			//boost::tmatch mat;
			//boost::tregex rx( _T("<\\s*meta\\s+[^>]*?url\\s*=\\s*[\"\']?\\s*(.*?)[\"\'> ].*?>"),boost::regex::normal | boost::regex::icase );
			//if( boost::regex_search(buf,mat,rx))
			//{
			//	CString strNewUrl = CString(mat[1].first,mat.length(1));					

			//	if( !strNewUrl.IsEmpty() )
			//	{
			//		CUrl url;
			//		url.ParseUrlEx(strURL);
			//		strURL = url.MakeURL(strNewUrl);

			//		TRACE("the refresh page url is %s\n",strURL);

			//		free(buf);
			//		buf = NULL;
			//		// 下载跳转页面
			//		continue;
			//	}
			//}	
			// 关闭连接
			InternetCloseHandle(hOpen);
			// 编码转换取消，放在这里不合理，比如图片资源就不需要转码
			//BOOL bChange = TRUE;
			//CString strBuf(buf);
			//CCharsetDetector detecotr;
			//charset_t ct = detecotr.GetCharset(buf,dwBytes);
			//TRACE("The page code is %s\n",Charset::TypeToName(ct).c_str());

			//switch ( ct )
			//{
			//case Charset::UTF_8:
			//	strBuf=CChineseCodeLib::Utf8ToGBK(strBuf);	
			//	break;
			//case Charset::BIG5:
			//	strBuf = CChineseCodeLib::Big5ToGBK(strBuf);
			//	break;
			//default:
			//	bChange = FALSE;
			//}
			//// 返回内容和长度
			//if( bChange )
			//{
			//	if( dwBytes != strBuf.GetLength() )
			//	{
			//		char* newBuf=(char*)realloc(buf,strBuf.GetLength()+1);
			//		if(newBuf == NULL)
			//		{
			//			// 清除原来内存，重新申请内存
			//			free(buf);
			//			buf = NULL;
			//			buf=(char*)malloc(strBuf.GetLength()+1);
			//		}
			//		else
			//		{
			//			buf=newBuf;
			//		}
			//	}
			//	memcpy(buf,strBuf.GetBuffer(strBuf.GetLength()),strBuf.GetLength());
			//	strBuf.ReleaseBuffer(strBuf.GetLength());

			//	dwBytes = strBuf.GetLength();
			//	buf[dwBytes] = '\0';
			//}			

			*pFileBuf=buf;
			nFileLen=dwBytes;	

			SendDownloadMessage(_T("下载成功：")+strURL);	
			return dwBytes;	
		}		
	}
	else
	{
        SendDownloadMessage(_T("网络连接失败！"));
		return INTERNET_ERROR_OPEN;
	}	

}
	/*
	 * Determines if the given NULL-terminated buffer is large enough to
	 *      concatenate the given number of characters.  If not, it attempts to
	 *      grow the buffer to fit.
	 * Returns:
	 *      0 on success, or
	 *      -1 on error (original buffer is unchanged).
	 */

int CDownloadHttpFile::CheckBufSize(char **buf, int *bufsize, int more)
{
	char *tmp;
	int roomLeft = *bufsize - (strlen(*buf) + 1);

	if(roomLeft >= more) return 0;

	tmp = (char *)realloc(*buf, *bufsize + more + 1);

	if(tmp == NULL) return -1;

	*buf = tmp;
	*bufsize += more + 1;
	return 0;
}

