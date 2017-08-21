#pragma once
#include "FtpManager.h"

class CFileSend
{
public:
	CFileSend(void);
	virtual ~CFileSend(void);

	virtual BOOL  SendFileToServer(CString strExistsFile,CString strNewFile,BOOL bFailIfExists)=0;
	virtual BOOL  DeleteFileFromServer(CString strExistsFile)=0;
	virtual BOOL  ConnectToServer()=0;

	BOOL IsConnected() { return m_bConnected ;}

protected:
	BOOL       m_bConnected;
	
};

class CNetShareFileSend : public CFileSend
{
public:
	CNetShareFileSend(CString strServer,CString strUserName,CString strPassWord,CString strLocalName = _T("Z:"));
	~CNetShareFileSend();

	BOOL  SendFileToServer(CString strExistsFile,CString strNewFile,BOOL bFailIfExists);
	BOOL  DeleteFileFromServer(CString strExistsFile);
	BOOL  ConnectToServer();
private:
	CString    m_strServer;
	CString    m_strUserName;
	CString    m_strPassWord;
	CString    m_strLocalName;
};


class CFtpFileSend : public CFileSend
{
public:
	CFtpFileSend(CString strServer,CString strUserName,CString strPassWord,int nPort = 21);
	~CFtpFileSend();

	BOOL  SendFileToServer(CString strExistsFile,CString strNewFile,BOOL bFailIfExists);
	BOOL  DeleteFileFromServer(CString strExistsFile);
	BOOL  ConnectToServer();
private:
	CString    m_strServer;
	CString    m_strUserName;
	CString    m_strPassWord;
	int        m_nPort;

	CFtpManager<CFtpFileSend> m_ftp;

};

