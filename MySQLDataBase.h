// MySQLDataBase.h: interface for the CMySQLDataBase class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "DataBase.h"

class CMySQLDataBase : public CDataBase  
{
public:
	CMySQLDataBase(LPCTSTR strServer,LPCTSTR strDatabase,LPCTSTR strUserName,LPCTSTR strPassword,int nPort = 3306,CString strCode = _T("utf8"));
	virtual ~CMySQLDataBase();
public:
	virtual CString GetDataBaseConnString();

private:
	CString    m_strServer;
	CString    m_strDatabase;
	CString    m_strUserName;
	CString    m_strPassword;
	int        m_nPort;
	CString    m_strCode;
};