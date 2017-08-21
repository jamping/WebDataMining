// PictureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "PictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg dialog

CPictureDlg::CPictureDlg(CNewsPicture* pPicture,CWnd* pParent /* = NULL */)
	: m_pPicture(pPicture),CDialogEx(CPictureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPictureDlg)
	m_strExplain = _T("");
	m_strURL = _T("");
	//}}AFX_DATA_INIT	
	m_bAnotherPicture=FALSE;
	m_strOldURL = _T("");
}
CPictureDlg::~CPictureDlg()
{

}
void CPictureDlg::SetValue(CNewsPicture::NewsPictureData& data)
{
	m_strURL = data.m_strURL;
	m_strOldURL = data.m_strOldURL;
	m_strExplain = data.m_strExplain;
}
BOOL CPictureDlg::GetValue(CNewsPicture::NewsPictureData& data)
{
	BOOL bModify=FALSE;

// 	if(!CCommonFunction::IsSameString(data.m_strURL,m_strURL))
// 	{
// 		data.m_strURL=m_strURL;
// 		bModify |= TRUE;
// 	}
	if(!CCommonFunction::IsSameString(data.m_strExplain,m_strExplain))
	{
		data.m_strExplain=m_strExplain;
		bModify |= TRUE;
	}
	return bModify;
}

void CPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureDlg)
	DDX_Control(pDX, IDC_PREVIEW, m_Preview);
	DDX_Text(pDX, IDC_EXPLAIN, m_strExplain);
	DDX_Text(pDX, IDC_URL, m_strURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPictureDlg, CDialogEx)
	//{{AFX_MSG_MAP(CPictureDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg message handlers

BOOL CPictureDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
    CString strDir = _T("");

	strDir.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),m_strURL);
	CCommonFunction::Replace(strDir,"/","\\");
	
	CString ext = strDir.Mid(strDir.ReverseFind('.')+1);

	if( pic.Load(strDir,CxImage::GetTypeIdFromName(ext)) )
	{
		RECT	rc ;
		m_Preview.GetClientRect (&rc) ;
		DWORD	style = m_Preview.GetStyle () ;
		style |= SS_BITMAP | SS_CENTERIMAGE ;
		::SetWindowLong (m_Preview.GetSafeHwnd(), GWL_STYLE, style) ;

		float ratioPic = ((float)pic.GetWidth())/((float)pic.GetHeight());
		float ratioPre = (float)rc.right/rc.bottom;
		if( ratioPic< ratioPre )
		{
			if ( (pic.GetHeight() > rc.bottom) )
				pic.Resample(rc.bottom*ratioPic, rc.bottom) ;
		}
		else
		{
			if ( (pic.GetWidth() > rc.right) )
				pic.Resample(rc.right, rc.right/ratioPic) ;
		}
		
		CDC* hdc = m_Preview.GetDC();
		m_Preview.SetBitmap (pic.MakeBitmap(hdc->m_hDC)) ;

		if (hdc->m_hDC) m_Preview.ReleaseDC(hdc);
	}	
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CPictureDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPictureDlg::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	char szFilters[]="图片文件 (*.jpg;*jpeg;*bmp;*png;*gif)|*.jpg;*.jpeg;*.bmp;*.png;*.gif|All Files (*.*)|*.*||";
	
	CFileDialog fileDlg(TRUE,NULL,"",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		CWaitCursor wait;
		m_bAnotherPicture = TRUE;

		if( !m_strOldURL.IsEmpty() && m_strOldURL.CompareNoCase(fileDlg.GetPathName())==0)
		{
			m_bAnotherPicture = FALSE;
			return ;
		}
		m_pPicture->m_data.m_strOldURL = m_strOldURL;
		m_strOldURL = fileDlg.GetPathName();

		GetDlgItem(IDC_URL)->SetWindowText(m_strOldURL);

		CString ext = m_strOldURL.Mid(m_strOldURL.ReverseFind('.')+1);

		if( pic.Load(m_strOldURL,CxImage::GetTypeIdFromName(ext)) )
		{
			RECT	rc ;
			m_Preview.GetClientRect (&rc) ;
			DWORD	style = m_Preview.GetStyle () ;
			style |= SS_BITMAP | SS_CENTERIMAGE ;
			::SetWindowLong (m_Preview.GetSafeHwnd(), GWL_STYLE, style) ;

			float ratioPic = ((float)pic.GetWidth())/((float)pic.GetHeight());
			float ratioPre = (float)rc.right/rc.bottom;
			if( ratioPic< ratioPre )
			{
				if ( (pic.GetHeight() > rc.bottom) )
					pic.Resample(rc.bottom*ratioPic, rc.bottom) ;
			}
			else
			{
				if ( (pic.GetWidth() > rc.right) )
					pic.Resample(rc.right, rc.right/ratioPic) ;
			}

			CDC* hdc = m_Preview.GetDC();
			m_Preview.SetBitmap (pic.MakeBitmap(hdc->m_hDC)) ;

			if (hdc->m_hDC) m_Preview.ReleaseDC(hdc);
		}

		UpdateData(FALSE);
	}
}

void CPictureDlg::OnOK() 
{
	// TODO: Add extra validation here
	if( m_bAnotherPicture )
	{    		
		CWaitCursor wait;

		CString strSource,strDest,strFileName,strID;
		strSource=m_strOldURL;

		if(m_pPicture->m_data.m_strURL.IsEmpty())
		{
             strID=CCommonFunction::AllocateID();
		     strFileName.Format("%s%s",strID,strSource.Mid(strSource.ReverseFind('.')));

		     m_pPicture->m_data.m_strURL.Format("%s/%s",COleDateTime::GetCurrentTime().Format("%Y/%m/%d"),strFileName);	
			 m_pPicture->SetID(strID);

			 m_strURL = m_pPicture->m_data.m_strURL;
		}

		strDest.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),m_strURL);		
		
		CCommonFunction::Replace(strDest,"/","\\");
		CCommonFunction::CreateDir(strDest);
		CopyFile(strSource,strDest,FALSE);

        //调整图片大小
		int nSize = CCommonFunction::ResamplePicture(strDest,strDest,
			theApp.GetPictureWidth(),theApp.GetPictureHeight(),theApp.GetPictureInterpolation());
		// save the picture size
		if( nSize > 0 )
		{
			m_pPicture->m_data.m_nPictureSize = nSize;
		}
	}

	CDialogEx::OnOK();
}
