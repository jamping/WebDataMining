// MyTimer.cpp: implementation of the CMyTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTimer.h"
#include "TimerWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTimerMap CMyTimer::m_sTimeMap;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyTimer::CMyTimer()
{
	m_nTimerID = 0;
}

CMyTimer::~CMyTimer()
{

}

void CALLBACK CMyTimer::MyTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	m_sTimeMap[idEvent]->m_pTimerWork->Process();
}

void CMyTimer::SetMyTimer(UINT nElapse,CTimerWork* pTimerWork)
{
	m_pTimerWork = pTimerWork;
	m_nTimerID = SetTimer(NULL,NULL,nElapse,MyTimerProc);
	m_sTimeMap[m_nTimerID] = this;
}

void CMyTimer::KillMyTimer()
{
	KillTimer(NULL,m_nTimerID);
	m_sTimeMap.RemoveKey(m_nTimerID);
} 
