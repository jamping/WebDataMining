//Engine.h
#pragma once

enum Engine_Type
{
	DOWNLOAD_ENGINE,
	DATA_ENGINE,
	WDM_ENGINE     //Web data mining engine
};

class CEngine
{
public:
	CEngine();
	~CEngine();

	virtual Engine_Type GetEngineType()=0;
};