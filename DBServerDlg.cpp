// DBServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "DBServerDlg.h"
#include "SQLInfoEnumerator.h"
#include "FileSend.h"
#include "FTPServerInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBServerDlg dialog


CDBServerDlg::CDBServerDlg(BOOL bReadOnly/* =FALSE */,CWnd* pParent /* = NULL */)
	: CSetReadOnlyDialog(bReadOnly,CDBServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBServerDlg)
	m_strDBServer = _T("");
	m_strResourceDir = _T("");
	m_strPass = _T("");
	m_strUser = _T("");
	m_nDataBaseType = 0;
	m_strResourcePath = _T("");
	m_nServerPort = 0;
	m_strServerCode = _T("");
	//}}AFX_DATA_INIT
	m_strFileServerName = _T("");
	m_strFileServerPassWord = _T("");
	m_nFileServerPort = 0;
	m_strFileServerUserName = _T("");
	m_nFileSendType = 0;
	m_bAnonymous = FALSE;
	m_bThumb = FALSE;
	m_strThumbName = _T("");
}


void CDBServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CSetReadOnlyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBServerDlg)
	DDX_Control(pDX, IDC_DBSERVER, m_ctlDBServer);
	DDX_CBString(pDX, IDC_DBSERVER, m_strDBServer);
	DDX_Text(pDX, IDC_FILE_SAVE_DIR, m_strResourceDir);
	DDX_Text(pDX, IDC_PIC_DIR, m_strResourcePath);
	DDX_Text(pDX, IDC_PASS, m_strPass);
	DDX_Text(pDX, IDC_USER, m_strUser);
	DDX_Radio(pDX, IDC_SQLSERVER, m_nDataBaseType);
	DDX_Text(pDX, IDC_PORT,m_nServerPort);
	DDX_CBString(pDX,IDC_COMBO_CODE,m_strServerCode);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_FILE_SERVER_NAME, m_strFileServerName);
	DDX_Text(pDX, IDC_FILE_SERVER_PASSWORD, m_strFileServerPassWord);
	DDX_Text(pDX, IDC_FILE_SERVER_PORT, m_nFileServerPort);
	DDX_Text(pDX, IDC_FILE_SERVER_USER_NAME, m_strFileServerUserName);
	DDX_Radio(pDX, IDC_FILE_SEND_TYPE, m_nFileSendType);
	DDX_Check(pDX, IDC_CHECK_ANONYMOUS, m_bAnonymous);
	DDX_Check(pDX, IDC_CHECK_THUMB, m_bThumb);
	DDX_Text(pDX, IDC_EDIT_THUMBNAME, m_strThumbName);
}
void CDBServerDlg::SetValue(CDBServer::DBServerData& data)
{
    m_strDBServer=data.m_strServerName;
	m_strUser=data.m_strUserName;
	m_strPass=data.m_strUserPasswd;
	m_strResourceDir=data.m_strResourceDir;	
	m_nDataBaseType=data.m_nDatabaseType;
	m_strResourcePath=data.m_strResourcePath;
	m_bThumb=data.m_bThumb;
	m_strThumbName=data.m_strThumbName;
	m_nServerPort=data.m_nServerPort;
	m_strServerCode=data.m_strServerCode;
	m_nFileSendType=data.m_nFileSendType;
	m_strFileServerName=data.m_strFileServerName;
	m_strFileServerUserName=data.m_strFileServerUserName;
	m_strFileServerPassWord=data.m_strFileServerPassWord;
	m_nFileServerPort=data.m_nFileServerPort;
}

BOOL CDBServerDlg::GetValue(CDBServer::DBServerData& data)
{
	BOOL bModify=FALSE;
	if( !CCommonFunction::IsSameString( data.m_strServerName,m_strDBServer ))
	{
		data.m_strServerName=m_strDBServer;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strUserName,m_strUser ))
	{
		data.m_strUserName=m_strUser;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strUserPasswd,m_strPass ))
	{
		data.m_strUserPasswd=m_strPass;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strResourceDir,m_strResourceDir ))
	{
		data.m_strResourceDir=m_strResourceDir;
		bModify |= TRUE;
	}
	if( data.m_nDatabaseType != m_nDataBaseType )
	{
		data.m_nDatabaseType = m_nDataBaseType;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strResourcePath,m_strResourcePath ))
	{
		data.m_strResourcePath=m_strResourcePath;
		bModify |= TRUE;
	}
	if( data.m_bThumb != m_bThumb )
	{
		data.m_bThumb = m_bThumb;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strThumbName,m_strThumbName ))
	{
		data.m_strThumbName=m_strThumbName;
		bModify |= TRUE;
	}
	if( data.m_nServerPort != m_nServerPort )
	{
		data.m_nServerPort = m_nServerPort;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strServerCode,m_strServerCode ))
	{
		data.m_strServerCode=m_strServerCode;
		bModify |= TRUE;
	}

	if( data.m_nFileSendType != m_nFileSendType )
	{
		data.m_nFileSendType = m_nFileSendType;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strFileServerName,m_strFileServerName ))
	{
		data.m_strFileServerName=m_strFileServerName;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strFileServerUserName,m_strFileServerUserName ))
	{
		data.m_strFileServerUserName=m_strFileServerUserName;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString( data.m_strFileServerPassWord,m_strFileServerPassWord ))
	{
		data.m_strFileServerPassWord=m_strFileServerPassWord;
		bModify |= TRUE;
	}
	if( data.m_nFileServerPort != m_nFileServerPort )
	{
		data.m_nFileServerPort = m_nFileServerPort;
		bModify |= TRUE;
	}
	return bModify;
}

BEGIN_MESSAGE_MAP(CDBServerDlg, CSetReadOnlyDialog)
	//{{AFX_MSG_MAP(CDBServerDlg)
//	ON_BN_CLICKED(IDC_BUTTON_FILESAVE, OnButtonFilesave)
	ON_CBN_DROPDOWN(IDC_DBSERVER, OnDropdownDbserver)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_PORT,OnUpdateServerPort)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_CODE,OnUpdateServerCode)
	ON_BN_CLICKED(IDC_SQLSERVER, OnBnClickedDatabaseType)
	ON_BN_CLICKED(IDC_ORACEL, OnBnClickedDatabaseType)
	ON_BN_CLICKED(IDC_MYSQL, OnBnClickedDatabaseType)
	ON_BN_CLICKED(IDC_FILE_SEND_TYPE, OnBnClickedDatabaseType)
	ON_BN_CLICKED(IDC_FTP, OnBnClickedDatabaseType)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_TEST,OnUpdateButtonTest)
	ON_UPDATE_COMMAND_UI(IDC_FILE_SAVE_DIR,OnUpdateDirPath)
	ON_UPDATE_COMMAND_UI(IDC_FILE_SERVER_PORT,OnUpdateFileServerPort)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_CHECK_ANONYMOUS, OnBnClickedCheckAnonymous)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_THUMBNAME,OnUpdateThumbName)
	ON_BN_CLICKED(IDC_CHECK_THUMB, &CDBServerDlg::OnClickedCheckThumb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBServerDlg message handlers

BOOL CDBServerDlg::OnInitDialog() 
{
	CSetReadOnlyDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bAnonymous = (m_strFileServerUserName.CompareNoCase(_T("Anonymous"))==0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// void CDBServerDlg::OnButtonFilesave() 
// {
// 	// TODO: Add your control notification handler code here
// 	UpdateData();
// 
// 	if(theApp.GetShellManager()->BrowseForFolder(m_strResourceDir,this,m_strResourceDir,_T("选择文件夹")))
// 	{
//         UpdateData(FALSE);
// 	}
// }


void CDBServerDlg::OnDropdownDbserver() 
{
	// TODO: Add your control notification handler code here
	switch ( m_nDataBaseType )
	{
	case 0:
		{
			//Fill List of SQL Servers
			CWaitCursor wait;
			m_ctlDBServer.ResetContent();
			CSQLInfoEnumerator obSQLEnumerate;
			if(obSQLEnumerate.EnumerateSQLServers())
			{
				int iCount = obSQLEnumerate.m_szSQLServersArray.GetSize();
				for(int i = 0; i < iCount; i++)
				{
					m_ctlDBServer.AddString(obSQLEnumerate.m_szSQLServersArray[i]);
				}
			}
		}
		break;
	case 1:
		// Oracle 
		break;
	case 2:
		// MySQL 
		break;
	}
}
BOOL CDBServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CSetReadOnlyDialog::PreTranslateMessage(pMsg);
}

void CDBServerDlg::OnUpdateServerPort(CCmdUI * pCmdUI)
{
	pCmdUI->Enable( m_nDataBaseType == 1 || m_nDataBaseType == 2 );
}

void CDBServerDlg::OnUpdateServerCode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_nDataBaseType == 2 );
}

void CDBServerDlg::OnUpdateButtonTest(CCmdUI * pCmdUI)
{
	pCmdUI->SetText( (m_nFileSendType == 0)?_T("选择映射目录"):_T("选择FTP目录"));
}
void CDBServerDlg::OnUpdateDirPath(CCmdUI * pCmdUI)
{
//	pCmdUI->Enable( m_nFileSendType == 0 );
}
void CDBServerDlg::OnUpdateFileServerPort(CCmdUI * pCmdUI)
{
	pCmdUI->Enable( m_nFileSendType == 1 );
}
void CDBServerDlg::OnUpdateThumbName(CCmdUI * pCmdUI)
{
	pCmdUI->Enable( m_bThumb );
}
void CDBServerDlg::OnBnClickedDatabaseType()
{
	// TODO: Add your control notification handler code here	
	UpdateData(TRUE);
}
void CDBServerDlg::OnOK()
{
	// TODO: Add your control notification handler code here	
	UpdateData();

	if( m_strResourcePath.Right(1) != _T("/") )
	{
		MessageBox(_T("附件URL访问路径请以“/”结尾"),_T("注意"),MB_OK|MB_ICONWARNING);
	    return ;
	}
	CSetReadOnlyDialog::OnOK();
}


void CDBServerDlg::OnBnClickedButtonTest()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CFileSend* pFileSend = NULL;

	switch( m_nFileSendType )
	{
	case 0:
		{
			CString strLocalName = m_strResourceDir.Left(m_strResourceDir.Find(_T(":"))+1);
			if ( strLocalName.IsEmpty() )
				strLocalName = _T("Z:");

			pFileSend = new CNetShareFileSend( m_strFileServerName,m_strFileServerUserName,m_strFileServerPassWord,strLocalName);
			if( !pFileSend->IsConnected() )
			{
				MessageBox(_T("创建网络映射驱动器失败，请检查网络！"),_T("错误"),MB_ICONWARNING|MB_OK);
				break;
			}
			if(theApp.GetShellManager()->BrowseForFolder(m_strResourceDir,this,m_strResourceDir,_T("选择文件夹")))
			{
				UpdateData(FALSE);
			}
		}
		break;
	case 1:
		{
			CFTPServerInfoDlg dlg( m_strFileServerName,m_strFileServerUserName,m_strFileServerPassWord,m_nFileServerPort,m_strResourceDir);

			if( dlg.DoModal() == IDOK )
			{
				m_strResourceDir = dlg.m_strDir;

				UpdateData(FALSE);
			}
		}
		break;
	}

	delete pFileSend;
	pFileSend = NULL;
}


void CDBServerDlg::OnBnClickedCheckAnonymous()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if( m_bAnonymous )
	{
		GetDlgItem(IDC_FILE_SERVER_USER_NAME)->SetWindowText(_T("Anonymous"));
		GetDlgItem(IDC_FILE_SERVER_PASSWORD)->SetWindowText(_T("Anonymous"));
	}
	else
	{
		GetDlgItem(IDC_FILE_SERVER_USER_NAME)->SetWindowText(_T(""));
		GetDlgItem(IDC_FILE_SERVER_PASSWORD)->SetWindowText(_T(""));
	}
}



void CDBServerDlg::OnClickedCheckThumb()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
