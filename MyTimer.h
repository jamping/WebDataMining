// MyTimer.h: interface for the CMyTimer class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>

class CTimerWork;
class CMyTimer;

typedef CMap<UINT,UINT,CMyTimer*,CMyTimer*> CTimerMap;

class CMyTimer  
{
public:
	//Set Timer
	void SetMyTimer(UINT nElapse,CTimerWork* pTimerWork);
	//Destroy Timer
	void KillMyTimer();
	//Save the timer ID
	UINT          m_nTimerID;
	CTimerWork*   m_pTimerWork;
	//Save the all timers
	static CTimerMap m_sTimeMap;
	// Call back function
	static void CALLBACK MyTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);

	CMyTimer();
	virtual ~CMyTimer();

};