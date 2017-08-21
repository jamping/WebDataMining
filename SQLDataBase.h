// SQLDataBase.h: interface for the CSQLDataBase class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "DataBase.h"

class CSQLDataBase : public CDataBase  
{
public:
	CSQLDataBase(LPCTSTR strUserName,LPCTSTR strPassword,LPCTSTR strDataSource,LPCTSTR strDatabaseName);
	virtual ~CSQLDataBase();
public:
	virtual CString GetDataBaseConnString();
private:
	CString    m_strUserName;
	CString    m_strPassword;
	CString    m_strDatabaseName;
	CString    m_strDataSource;
};