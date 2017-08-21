#pragma once
#include "ipenum.h"

class CGetHostIP :public CIPEnum
{
public:
	CGetHostIP(void);
	~CGetHostIP(void);
	
	CString GetHostIP() { return m_strIP; }
	CString GetHostName() { return m_strHost; }
private:
	virtual BOOL EnumCallbackFunction(LPTSTR strHost, int nAdapter, const in_addr& address);	

	CString   m_strIP;
	CString   m_strHost;
};

