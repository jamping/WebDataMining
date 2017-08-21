// Logs.cpp: implementation of the CLogs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "Logs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogs::CLogs()
{

}

CLogs::~CLogs()
{

}
void CLogs::WriteLogs(CString strLogs,Log_type nType/* =log_unknow */,CString strSource /* = _T */)
{
	if(!theApp.DBIsOpen())
		return;

	char szComputerName[255];
	DWORD NameLen=255;
	GetComputerName((LPSTR)szComputerName,&NameLen);
    CString strComputerName=CString(szComputerName,NameLen);

	CString strSql;
	strSql.Format("insert into wdm_log (log_id,log_time,log_source,log_type,log_message,log_owner) values \
		('%s','%s','%s',%d,'%s','%s')",
		CCommonFunction::AllocateID(),COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),theApp.GetCurUserName(),(int)nType,strLogs,strComputerName);

	theApp.GetDB()->Execute(strSql);
}