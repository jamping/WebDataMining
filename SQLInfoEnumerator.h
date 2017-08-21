// SQLInfoEnumerator.h: interface for the CSQLInfoEnumerator class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <sql.h>
#include <sqlext.h>

#define MAX_RET_LENGTH	4824

class CSQLInfoEnumerator  
{
public:
	CSQLInfoEnumerator();
	virtual ~CSQLInfoEnumerator();
	int m_iRetcode;
public:
	CStringArray m_szSQLServersArray;
	CStringArray m_szSQLServerDatabaseArray;
	CStringArray m_szSQLServerLanguageArray;
public:
	BOOL EnumerateDatabase(LPCTSTR pszSQLServer,LPCTSTR pszUserId,LPCTSTR pszPwd);
	BOOL EnumerateDatabaseLanguage(LPCTSTR pszSQLServer,LPCTSTR pszUserId,LPCTSTR pszPwd);
	BOOL EnumerateSQLServers();
protected:
	BOOL RetrieveInformation(LPCTSTR pszInputParam,LPCTSTR pszLookUpKey,CStringArray &szArray);
	BOOL FillupStringArray(LPCTSTR pszData,CStringArray &szArray,TCHAR chSep = ',');
	BOOL ClearAll();
};