// WorkFile.h: interface for the CWorkFile class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Work.h"

class CWorkFile : public plugware/*robert*/::core::work_unit,CWork  
{
public:
	CWorkFile(CEntry* pEntry,CRule* pRule,CString strFileDir);

	virtual ~CWorkFile();
    
	void process();
private:
    void RecurseRetriveTopic(CString strFileDir);

    CString    m_strFileDir;
};