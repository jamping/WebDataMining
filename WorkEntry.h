// WorkEntry.h: interface for the CWorkEntry class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Work.h"

class CWorkEntry : public plugware/*robert*/::core::work_unit,CWork 
{
public:
	CWorkEntry(CEntry* pEntry,CRule* pRule);
	virtual ~CWorkEntry();

	void process();	
};