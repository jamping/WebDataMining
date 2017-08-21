#include "stdafx.h"
#include "WebDataMining.h"
#include "DatabaseEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDataBaseDataEngine::CDataBaseDataEngine()
{

}
CDataBaseDataEngine::CDataBaseDataEngine(CString str) : CDataEngine(str)
{
}

CDataBaseDataEngine::~CDataBaseDataEngine()
{
}
