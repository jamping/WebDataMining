// SQLInfoEnumerator.cpp: implementation of the CSQLInfoEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLInfoEnumerator.h"

#pragma comment(lib,"odbc32.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
Author:		Santosh Rao
EMail Id:	SantoshRao@bigfoot.com
Date:		Dec 31st 1999
Comment:	Simple Class to enable enumeration 
			of SQL Servers and their databases and supported 
			languages.
			You may use this code in any way you want,
			You can remove my copyright too.
			But dont sue me in court please, i cant afford it.
*/

CSQLInfoEnumerator::CSQLInfoEnumerator()
{
	ClearAll();
}

CSQLInfoEnumerator::~CSQLInfoEnumerator()
{
	ClearAll();
}

/*
	Clear all internal storing CStringArrays
*/

BOOL CSQLInfoEnumerator::ClearAll()
{
	m_szSQLServersArray.RemoveAll();
	m_szSQLServerDatabaseArray.RemoveAll();
	m_szSQLServerLanguageArray.RemoveAll();
	return TRUE;
}

/*
	Retrieve Information of SQL Servers
	On Success the string contains identifier 
	SERVER:Server= followed by the list of SQL
	Servers
*/

BOOL CSQLInfoEnumerator::EnumerateSQLServers()
{
	//Browse Connect for SQL Server Driver defined servers
	//The return information would contain SERVER:Server= Keyword followed by 
	//{list of Servers} separated by the character ';'
	return RetrieveInformation(_T("Driver={SQL Server}"),_T("SERVER:Server="),m_szSQLServersArray);
}

/*
	Retrieve Information of databases in a SQL Server
	You have to provide the User Id and Password
	On Success the string contains identifier 
	DATABASE:Database= followed by the list of databases
*/
BOOL CSQLInfoEnumerator::EnumerateDatabase(LPCTSTR pszSQLServer, LPCTSTR pszUserId, LPCTSTR pszPwd)
{
	//Browse Connect for SQL Server Driver defined server using the authentication information
	//The return information would contain DATABASE:Database= Keyword followed by 
	//{list of databases} separated by the character ';'
	CString szInputParam;
	szInputParam.Format("Driver={SQL Server};SERVER=%s;UID=%s;PWD=%s",pszSQLServer,pszUserId,pszPwd);
	return RetrieveInformation(szInputParam,_T("DATABASE:Êý¾Ý¿â="),m_szSQLServerDatabaseArray);
}

/*
	Retrieve Information of languages in a SQL Server
	You have to provide the User Id and Password
	On Success the string contains identifier 
	LANGUAGE:Language= followed by the list of languages
	Character Translation is not done, so you may see
	special characters and question marks in the list of
	languages text
*/

BOOL CSQLInfoEnumerator::EnumerateDatabaseLanguage(LPCTSTR pszSQLServer, LPCTSTR pszUserId, LPCTSTR pszPwd)
{
	CString szInputParam;
	//Browse Connect for SQL Server Driver defined server using the authentication information
	//The return information would contain LANGUAGE:Language= Keyword followed by 
	//{list of languages} separated by the character ';'
	szInputParam.Format("Driver={SQL Server};SERVER=%s;UID=%s;PWD=%s",pszSQLServer,pszUserId,pszPwd);
	return RetrieveInformation(szInputParam,_T("LANGUAGE:ÓïÑÔ="),m_szSQLServerLanguageArray);
}

/*
	This Function Checks for retrieving additional information
	using the initial input information and returns true if the 
	look up key is found and fills ther result set into a string 
	array.
*/

BOOL CSQLInfoEnumerator::RetrieveInformation(LPCTSTR pszInputParam,LPCTSTR pszLookUpKey,CStringArray &szArray)
{
	SQLHENV       hSQLEnv;
	SQLHDBC       hSQLHdbc;
	short		  sConnStrOut;
	BOOL		  bReturn = FALSE;
	//Allocate the environment handle
	m_iRetcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hSQLEnv);
	if (m_iRetcode == SQL_SUCCESS || m_iRetcode == SQL_SUCCESS_WITH_INFO)
	{
		//Set the environment attribute to SQL_OV_ODBC3
		m_iRetcode = SQLSetEnvAttr(hSQLEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
		if (m_iRetcode == SQL_SUCCESS || m_iRetcode == SQL_SUCCESS_WITH_INFO) 
		{
		    //Allocate a connection handle
			m_iRetcode = SQLAllocHandle(SQL_HANDLE_DBC, hSQLEnv, &hSQLHdbc);
			if (m_iRetcode == SQL_SUCCESS || m_iRetcode == SQL_SUCCESS_WITH_INFO) 
			{
				CString szConnStrOut;
				//Call SQLBrowseConnect for additional information
				m_iRetcode = SQLBrowseConnect(hSQLHdbc, (SQLCHAR *)pszInputParam, SQL_NTS,
						 (SQLCHAR *)(szConnStrOut.GetBuffer(MAX_RET_LENGTH)), MAX_RET_LENGTH, &sConnStrOut);
				szConnStrOut.ReleaseBuffer();
				//if the look up key is found
				//fill in the result set
				int iFind = szConnStrOut.Find(pszLookUpKey);
				if(iFind != -1)
				{
					CString szLookUpKey = pszLookUpKey;
					szConnStrOut = szConnStrOut.Mid(iFind+szLookUpKey.GetLength());
					iFind = szConnStrOut.Find('{');
					if(iFind != -1)
					{
						szConnStrOut = szConnStrOut.Mid(iFind+1);
						iFind = szConnStrOut.Find('}');
						if(iFind != -1)
						{
							szConnStrOut = szConnStrOut.Left(iFind);
							FillupStringArray(szConnStrOut,szArray);
							bReturn = TRUE;
						}
					}
				}	

				SQLDisconnect(hSQLHdbc);
			}
			SQLFreeHandle(SQL_HANDLE_DBC, hSQLHdbc);
		}
		SQLFreeHandle(SQL_HANDLE_ENV, hSQLEnv);
	}	
	return TRUE;
}

/*Breaks up return information into a CStringArray for easy parsing*/

BOOL CSQLInfoEnumerator::FillupStringArray(LPCTSTR pszData,CStringArray &szArray,TCHAR chSep)
{
	CString szStr = pszData;
	CString szSep = CString(chSep);
	szStr.TrimLeft();
	szStr.TrimRight();
	szArray.RemoveAll();
	int iStrLen = szStr.GetLength(),iSepPos,iSepLength=szSep.GetLength();
	if(iStrLen>0 )
	{
		if(szStr.Right(iSepLength) != szSep)
			szStr += szSep;
		iStrLen = szStr.GetLength();
	}
	else 
		return FALSE;
	
	CString szContentStr;
	while(!szStr.IsEmpty())
	{
		iSepPos = szStr.Find(szSep);
		szContentStr = szStr.Left(iSepPos);
		szContentStr.TrimLeft();
		szContentStr.TrimRight();
		if(!szContentStr.IsEmpty())
			szArray.Add(szContentStr);
		iStrLen  = iStrLen - (iSepPos + iSepLength);
		szStr = szStr.Right(iStrLen);
	}
	return TRUE;
}
