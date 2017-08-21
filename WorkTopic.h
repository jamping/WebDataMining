// WorkTopic.h: interface for the CWorkTopic class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "thread_pool.h"
//#include "thread_pool.hpp"
#include "Topic.h"

class CWorkTopic : public plugware/*robert*/::core::work_unit  
{
public:
	CWorkTopic(CTopic* pTopic,BOOL bPublish=TRUE);
	virtual ~CWorkTopic();

	void process();
private:
	CTopic*   m_pTopic;
	BOOL      m_bPublish;
};
