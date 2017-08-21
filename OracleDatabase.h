// OracleDatabase.h: interface for the COracleDatabase class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "DataBase.h"

class COracleDatabase : public CDataBase  
{
public:
	COracleDatabase(LPCTSTR strUserName,LPCTSTR strPassword,LPCTSTR strDataSource,int nPort=1521);
	virtual ~COracleDatabase();
public:
	virtual CString GetDataBaseConnString();
	private:
	CString    m_strUserName;
	CString    m_strPassword;
	CString    m_strDataSource;
	int        m_nPort;
};