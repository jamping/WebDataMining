// SQLServerDataEngine.cpp: implementation of the CSQLServerDataEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "SQLServerDataEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLServerDataEngine::CSQLServerDataEngine()
{

}
CSQLServerDataEngine::CSQLServerDataEngine(CString str):CDataBaseDataEngine(str)
{

}
CSQLServerDataEngine::~CSQLServerDataEngine()
{

}
