
#include "StdAfx.h"
#include "FileSend.h"
#include <Winnetwk.h>
#include <io.h>

#pragma comment(lib, "mpr.lib")

//////////////////////////////////////////////////////////////////////////
CFileSend::CFileSend(void)
	:m_bConnected(FALSE)
{
	
}


CFileSend::~CFileSend(void)
{
}

//////////////////////////////////////////////////////////////////////////
CNetShareFileSend::CNetShareFileSend(CString strServer,CString strUserName,CString strPassWord,CString strLocalName /* = _T */)
	:m_strServer(strServer)
	,m_strUserName(strUserName)
	,m_strPassWord(strPassWord)
	,m_strLocalName(strLocalName)
{
	ConnectToServer();
}

CNetShareFileSend::~CNetShareFileSend(void)
{
}

BOOL CNetShareFileSend::ConnectToServer()
{
	DWORD dwResult;
	//�ж�ӳ������������·���Ƿ����
	if( _access( m_strLocalName,0 ) != -1 )
	{
		//������ڣ������ж��Ƿ���Զ�̷�����
		CHAR szRemoteName[255]; 
		DWORD cchBuff = sizeof(szRemoteName); 

		dwResult = WNetGetConnection(m_strLocalName,(LPSTR) szRemoteName,&cchBuff); 

		if( dwResult == NO_ERROR )
		{
			//�����Զ�̷���������ɹ�����
			if( m_strServer.Compare(szRemoteName) == 0 )
			{
				m_bConnected = TRUE;

				return TRUE;
			}
		}
		else
		{
			strcpy(szRemoteName,_T("δ֪·��"));
		}
		//������ǣ����ȶϿ�������				
		dwResult = WNetCancelConnection2(m_strLocalName,CONNECT_UPDATE_PROFILE,TRUE);
		if (dwResult != NO_ERROR)
		{
			TRACE(_T("�Ͽ�����������%sʧ��!ӳ��·��Ϊ%s/n"),m_strLocalName,szRemoteName);
		}
	}
	//�Զ���������������
	NETRESOURCE netres; 
	// Zero out the NETRESOURCE struct
	memset(&netres, 0, sizeof (NETRESOURCE));

	netres.dwScope = RESOURCE_GLOBALNET;
	netres.dwType = RESOURCETYPE_DISK;
	netres.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;
	netres.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	netres.lpLocalName = m_strLocalName.GetBuffer(0); 
	netres.lpRemoteName = m_strServer.GetBuffer(0);
	netres.lpComment = NULL; 
	netres.lpProvider = NULL;	
	
	dwResult = WNetAddConnection2(&netres,m_strPassWord,m_strUserName,CONNECT_UPDATE_PROFILE);

 	m_strLocalName.ReleaseBuffer();
	m_strServer.ReleaseBuffer();

	return m_bConnected = (NO_ERROR == dwResult);
}

BOOL  CNetShareFileSend::SendFileToServer(CString strExistsFile,CString strNewFile,BOOL bFailIfExists)
{
	BOOL bRet = FALSE;

	if( IsConnected() )
	{
		CCommonFunction::CreateDir(strNewFile);	
		bRet = CopyFile(strExistsFile,strNewFile,bFailIfExists);
	}

	return bRet;
}
BOOL CNetShareFileSend::DeleteFileFromServer(CString strExistsFile)
{
	BOOL bRet = FALSE;

	if( IsConnected() )
	{
		CCommonFunction::Replace(strExistsFile,"/","\\");
		bRet = DeleteFile(strExistsFile);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
CFtpFileSend::CFtpFileSend(CString strServer,CString strUserName,CString strPassWord,int nPort /* = 21 */)
	:m_strServer(strServer)
	,m_strUserName(strUserName)
	,m_strPassWord(strPassWord)
	,m_nPort(nPort)
{
	ConnectToServer();
}
CFtpFileSend::~CFtpFileSend(void)
{
}

BOOL CFtpFileSend::ConnectToServer()
{	
	if( !m_ftp.IsFtpConnect() )
	{
		m_bConnected = m_ftp.ConnectFtp(m_strServer,m_nPort,m_strUserName,m_strPassWord,FALSE);
	}

	return m_bConnected;
}

BOOL  CFtpFileSend::SendFileToServer(CString strExistsFile,CString strNewFile,BOOL bFailIfExists)
{
	BOOL bRet = FALSE;

	if( !m_ftp.IsFtpConnect() )
		m_ftp.ReConnectFtp(0);

	CCommonFunction::Replace(strNewFile,"\\","/");
	CCommonFunction::Replace(strNewFile,"//","/");

	bRet = m_ftp.FtpUpLoadFile(strExistsFile,strNewFile);

	return bRet;
}
BOOL CFtpFileSend::DeleteFileFromServer(CString strExistsFile)
{
	BOOL bRet = FALSE;

	if( !m_ftp.IsFtpConnect() )
		m_ftp.ReConnectFtp(0);

	CCommonFunction::Replace(strExistsFile,"\\","/");
	CCommonFunction::Replace(strExistsFile,"//","/");

	bRet = m_ftp.FtpDelFile(strExistsFile);

	return bRet;
}
