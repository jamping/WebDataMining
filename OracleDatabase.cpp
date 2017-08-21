// OracleDatabase.cpp: implementation of the COracleDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "OracleDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COracleDatabase::COracleDatabase(LPCTSTR strUserName,LPCTSTR strPassword,LPCTSTR strDataSource,int nPort/* =1521 */)
:m_strUserName(strUserName)
,m_strPassword(strPassword)
,m_strDataSource(strDataSource)
,m_nPort(nPort)
{

}

COracleDatabase::~COracleDatabase()
{

}
CString COracleDatabase::GetDataBaseConnString()
{
	 CString strConn;

	 strConn.Format("Provider=OraOLEDB.Oracle;Data Source=%s;User Id=%s;Password=%s;",
		m_strDataSource,m_strUserName,m_strPassword);

	return strConn;
}