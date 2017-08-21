// SQLServerDataEngine.h: interface for the CSQLServerDataEngine class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "DatabaseEngine.h"

class CSQLServerDataEngine : public CDataBaseDataEngine  
{
public:
	CSQLServerDataEngine();
	CSQLServerDataEngine(CString str);

	virtual ~CSQLServerDataEngine();

	int GetDataBaseDataEngineType() { return SQLSERVER_DATABASE_DATA_ENGINE; }
};