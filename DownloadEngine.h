//DownloadEngine.h
#pragma once

#include "Engine.h"

enum Download_Type
{
	HTTP_DOWNLOAD,
	FTP_DOWNLOAD
};

class CDownloadEngine :public CEngine
{
public:
	CDownloadEngine();
	virtual ~CDownloadEngine();

	virtual int Fetch(CString strURL,char** pFileBuf,int& bufLen)=0;
	virtual Engine_Type GetEngineType() { return DOWNLOAD_ENGINE; }
	virtual Download_Type GetDownloadType() = 0;
protected:

};