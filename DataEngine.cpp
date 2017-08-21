#include "StdAfx.h"
#include "WebDataMining.h"
#include "DataEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDataEngine::CDataEngine():CEngine()
{
}

CDataEngine::CDataEngine(CString str):CEngine()
{
	m_str = str;
}

CDataEngine::~CDataEngine()
{
}
