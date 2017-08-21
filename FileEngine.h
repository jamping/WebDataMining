#pragma once

#include "DataEngine.h"
#include <fstream>

enum File_Data_Engine_Type
{

};

struct File_Arg
{
	
};

class CFileDataEngine : public CDataEngine
{
protected:

public:
	CFileDataEngine();
	CFileDataEngine(CString str);
	virtual ~CFileDataEngine();

	int GetDataEngineType() { return FILE_DATA_ENGINE; }
	virtual int GetFileDataEngineType() = 0;
};