// MySQLDataBase.cpp: implementation of the CMySQLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "MySQLDataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySQLDataBase::CMySQLDataBase(LPCTSTR strServer,LPCTSTR strDatabase,LPCTSTR strUserName,LPCTSTR strPassword,int nPort /* = 3306 */,CString strCode /* = _T */)
	:m_strServer(strServer)
	,m_strDatabase(strDatabase)
	,m_strUserName(strUserName)
	,m_strPassword(strPassword)	
	,m_nPort(nPort)
	,m_strCode(strCode)
{

}

CMySQLDataBase::~CMySQLDataBase()
{

}

CString CMySQLDataBase::GetDataBaseConnString()
{
	 CString strConn;	 
	 strConn.Format("DRIVER={%s};\
					SERVER=%s;\
					port=%d;\
					charset=%s;\
					DATABASE=%s;\
					USER=%s;\
					PASSWORD=%s;\
					OPTION=3;",
		 theApp.GetMysqlDrive(),m_strServer,m_nPort,m_strCode,m_strDatabase,m_strUserName,m_strPassword);

	return strConn;
}