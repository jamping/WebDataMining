// ReceiveSocket.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ReceiveSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReceiveSocket

CReceiveSocket::CReceiveSocket(CExtractSourceTree* pTree):m_pTree(pTree)
{
}

CReceiveSocket::~CReceiveSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CReceiveSocket, CSocket)
	//{{AFX_MSG_MAP(CReceiveSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CReceiveSocket member functions

void CReceiveSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	TCHAR buf[256];

	int nRet=Receive(buf,256);
	if(nRet!=ERROR)
	{
		m_pTree->ProcessPendingRead(buf);
	}

	CSocket::OnReceive(nErrorCode);
}
