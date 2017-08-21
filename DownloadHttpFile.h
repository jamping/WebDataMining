// DownloadHttpFile.h: interface for the CDownloadHttpFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <wininet.h>
#include <mmsystem.h>
#include "DownloadEngine.h"

#define WM_PROCESS_INFO WM_USER+10

class CDownloadHttpFile :public CDownloadEngine
{
public:
	CDownloadHttpFile(HWND hMessageWnd);
	CDownloadHttpFile();
	virtual ~CDownloadHttpFile();
	 
	int Fetch(CString strURL,char** pFileBuf,int& bufLen);

	void SetHWND(HWND hMessageWnd) { m_hMessageWnd=hMessageWnd; }
    Download_Type GetDownloadType() { return HTTP_DOWNLOAD; }
private:    
    void SendDownloadMessage(CString strInfo);
    int  CheckBufSize(char **buf, int *bufsize, int more);
    
	enum
	{
		INTERNET_ERROR_OPENURL=-1,
		INTERNET_ERROR_FILEOPEN=-2,
		INTERNET_ERROR_READFILE=-3,
		INTERNET_ERROR_OPEN=-4,
		ALLOCATE_MEMORY_ERROR=-5
	};

	HWND         m_hMessageWnd;        // process message  
};