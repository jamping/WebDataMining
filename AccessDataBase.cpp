// AccessDataBase.cpp: implementation of the CAccessDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "AccessDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccessDataBase::CAccessDataBase(LPCTSTR strDataSource):m_strDataSource(strDataSource)
{

}

CAccessDataBase::~CAccessDataBase()
{

}
CString CAccessDataBase::GetDataBaseConnString()
{
	 CString strConn;

	 strConn.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s",m_strDataSource);

	return strConn;
}