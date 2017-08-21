// TimerWork.h: interface for the CTimerWork class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CTimerWork  
{
public:
	CTimerWork();
	virtual ~CTimerWork();

	virtual void Process() = 0;
};