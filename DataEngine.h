#pragma once

#include "Engine.h"

enum DataEngine_Type
{
	FILE_DATA_ENGINE,
	DATABASE_DATA_ENGINE
};

class CDataEngine : public CEngine
{
public:
	CString m_str;	// database engine ---connecting string
			// file engine ---file path & name

public:
	CDataEngine(CString str);
	CDataEngine();
	virtual ~CDataEngine();

	virtual Engine_Type GetEngineType() { return DATA_ENGINE; }
    virtual int GetDataEngineType() =0;

	virtual BOOL Write(void *arg) = 0;
	virtual BOOL Read(void *arg) =0;
	virtual BOOL Open(CString str) = 0;
	virtual BOOL Close() = 0;
};