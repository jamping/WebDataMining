// SQLDataBase.cpp: implementation of the CSQLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "SQLDataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLDataBase::CSQLDataBase(LPCTSTR strUserName,LPCTSTR strPassword,LPCTSTR strDataSource,LPCTSTR strDatabaseName)
:m_strUserName(strUserName)
,m_strPassword(strPassword)
,m_strDataSource(strDataSource)
,m_strDatabaseName(strDatabaseName)
{

}

CSQLDataBase::~CSQLDataBase()
{

}

CString CSQLDataBase::GetDataBaseConnString()
{
	 CString strConn;

	 strConn.Format("Provider=SQLOLEDB.1;User ID=%s;Password=%s;Data Source=%s;Database=%s", 
		m_strUserName,m_strPassword,m_strDataSource,m_strDatabaseName);

	return strConn;
}