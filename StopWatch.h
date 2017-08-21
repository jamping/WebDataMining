// Stopwatch.h

//使用方法
//	Create a stopwatch timer
//	(which defaults to the current time).
//	CStopWatch stopwatch;
//
//Execute the code I want to profile here.
//
//Get how much time has elapsed up to now.
//__int64 qwElapsedTime = stopwatch.Now();
//
//qwElapsedTime indicates how long 
//the profiled code executed in milliseconds.
//摘自 windows核心编程
#pragma  once

class CStopWatch 
{
public:
	CStopWatch() 
	{ 
		QueryPerformanceFrequency(&m_liPerfFreq);
		Start();
	}

	void Start() 
	{ 
		QueryPerformanceCounter(&m_liPerfStart);
	}

	double Now() const 
	{
		//Returns # of milliseconds since
		//Start was called

		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);

		return (((liPerfNow.QuadPart - 
			m_liPerfStart.QuadPart))/
			(double)m_liPerfFreq.QuadPart);
	}

private:

	//Counts per second
	LARGE_INTEGER m_liPerfFreq;   

	//Starting count
	LARGE_INTEGER m_liPerfStart;  
}; 