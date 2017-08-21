#pragma once

#include "DataEngine.h"

enum DataBase_Data_Engine_Type
{
	SQLSERVER_DATABASE_DATA_ENGINE,
	ORACLE_DATABASE_DATA_ENGINE,
	ACCESS_DATABASE_DATA_ENGINE
};

class CDataBaseDataEngine : public CDataEngine
{
public:
	CDataBaseDataEngine(CString str);
	CDataBaseDataEngine();
	virtual ~CDataBaseDataEngine();

	int GetDataEngineType() { return DATABASE_DATA_ENGINE; }
    virtual int GetDataBaseDataEngineType() =0;
};