#include "StdAfx.h"
#include "GetHostIP.h"


CGetHostIP::CGetHostIP(void)
{
	Enumerate();
}


CGetHostIP::~CGetHostIP(void)
{
}

BOOL CGetHostIP::EnumCallbackFunction(LPTSTR strHost, int nAdapter, const in_addr& address)
{
	m_strHost = strHost;
	m_strIP.Format("%d.%d.%d.%d\n", address.S_un.S_un_b.s_b1, 
		address.S_un.S_un_b.s_b2, 
		address.S_un.S_un_b.s_b3, 
		address.S_un.S_un_b.s_b4);

	return FALSE;
}