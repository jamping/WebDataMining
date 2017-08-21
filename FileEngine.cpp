#include "stdafx.h"
#include "WebDataMining.h"
#include "FileEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CFileDataEngine::CFileDataEngine():CDataEngine()
{
}

CFileDataEngine::CFileDataEngine(CString str) : CDataEngine (str)
{
}

CFileDataEngine::~CFileDataEngine()
{

}


