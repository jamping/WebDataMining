// DataBase.h: interface for the CDataBase class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CDataBase  
{
public:
    CDataBase();
	virtual ~CDataBase();
public:
	virtual CString GetDataBaseConnString()=0;
};