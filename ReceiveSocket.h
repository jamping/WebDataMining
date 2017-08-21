// ReceiveSocket.h : header file
//
#pragma once

#include "ExtractSourceTree.h"

/////////////////////////////////////////////////////////////////////////////
// CReceiveSocket command target
class CExtractSourceTree;
class CReceiveSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CReceiveSocket(CExtractSourceTree* pTree);
	virtual ~CReceiveSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReceiveSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CReceiveSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CExtractSourceTree*   m_pTree;
};