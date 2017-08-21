// WorkUrl.h: interface for the CWorkUrl class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Work.h"

class CWorkUrl :  public plugware/*robert*/::core::work_unit,CWork 
{
public:
	CWorkUrl(CEntry* pEntry,CRule* pRule,CStringArray& arrayUrl);
	virtual ~CWorkUrl();

	void process();
private:
	CStringArray m_arrayUrl;
};
