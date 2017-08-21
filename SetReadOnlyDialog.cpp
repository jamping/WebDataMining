// SetReadOnlyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "SetReadOnlyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetReadOnlyDialog dialog


CSetReadOnlyDialog::CSetReadOnlyDialog(BOOL bReadOnly,UINT nIDTemplate,CWnd* pParent /* = NULL */)
	: CDialogEx(nIDTemplate, pParent),m_bReadOnly(bReadOnly)
{
	//{{AFX_DATA_INIT(CSetReadOnlyDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetReadOnlyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetReadOnlyDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetReadOnlyDialog, CDialogEx)
	//{{AFX_MSG_MAP(CSetReadOnlyDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetReadOnlyDialog message handlers

BOOL CSetReadOnlyDialog::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	//  - Set the enabled state for each control depending on whether
	// the control is currently visible or not	
	if(m_bReadOnly)
	{
		CWnd* pWndControl = this->GetWindow( GW_CHILD );
		while( pWndControl != NULL )
		{
			TCHAR str[255];
			GetClassName(pWndControl->GetSafeHwnd(),str,255);
             
			if(_tcscmp(str,"Edit")==0)
			{
                 static_cast<CEdit*>(pWndControl)->SetReadOnly();
			}
			
			pWndControl = pWndControl->GetWindow( GW_HWNDNEXT );
		}
		
		// - Check if a control still has the focus
		// (can lose it if the active control becomes disabled)
		CWnd* pWndActiveControl = CWnd::GetFocus();
		if( pWndActiveControl == NULL )
		{
			//  - Set focus to "first" control on dialog
			CWnd* pWndFirstControl = this->GetNextDlgTabItem( NULL );
			ASSERT_VALID( pWndFirstControl );
			ASSERT( pWndFirstControl->IsWindowEnabled() );
			pWndFirstControl->SetFocus();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
