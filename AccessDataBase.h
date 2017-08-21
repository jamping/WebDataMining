// AccessDataBase.h: interface for the CAccessDataBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataBase.h"

class CAccessDataBase : public CDataBase  
{
public:
	CAccessDataBase(LPCTSTR strDataSource);
	virtual ~CAccessDataBase();
public:
	virtual CString GetDataBaseConnString( );
private:
	CString   m_strDataSource;
};