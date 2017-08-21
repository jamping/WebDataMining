// SelContentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "SelContentDlg.h"
#include "WDMDatabase.h"
#include "WDMTable.h"
#include "WDMOperator.h"
#include "SQLInfoEnumerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelContentDlg dialog


CSelContentDlg::CSelContentDlg(CBaseObject* pObject,CWnd* pParent /* = NULL */)
	: CDialogEx(CSelContentDlg::IDD, pParent),m_pOb(pObject)
{
	//{{AFX_DATA_INIT(CSelContentDlg)
	m_strPrompt = _T("");
	m_strSelContent = _T("");
	//}}AFX_DATA_INIT
}


void CSelContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelContentDlg)
	DDX_Control(pDX, IDC_CONTENT, m_ctlContent);
	DDX_Text(pDX, IDC_PROMPT, m_strPrompt);
	DDX_CBString(pDX, IDC_CONTENT, m_strSelContent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelContentDlg, CDialogEx)
	//{{AFX_MSG_MAP(CSelContentDlg)
	ON_CBN_DROPDOWN(IDC_CONTENT, OnDropdownContent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelContentDlg message handlers

BOOL CSelContentDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString  strTitle;
	switch(m_pOb->GetType())
	{
	case OBJECT_TYPE_DATABASE:
		strTitle=_T("数据库信息");
		m_strPrompt=_T("请选择数据库");
		m_strSelContent=(static_cast<CWDMDatabase*>(m_pOb))->m_data.m_strDatabaseName;
		break;
	case OBJECT_TYPE_TABLE:
		strTitle=_T("数据表信息");
		m_strPrompt=_T("请选择数据表");
		m_strSelContent=(static_cast<CWDMTable*>(m_pOb))->m_data.m_strTableName;
		break;
	case OBJECT_TYPE_OPERATOR:
		strTitle=_T("数据表操作信息");
		m_strPrompt=_T("请选择数据表操作");
		m_strSelContent=(static_cast<CWDMOperator*>(m_pOb))->m_data.m_strOperatorName;
		break;
	default:
		ASSERT(FALSE);
	}
    SetWindowText(strTitle);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelContentDlg::OnDropdownContent() 
{
	// TODO: Add your control notification handler code here
	BOOL bInsert = TRUE;
	CString strSql;
	COptionsData::OptionsData::DBServerData data;

	switch(m_pOb->GetType())
	{
	case OBJECT_TYPE_DATABASE:
		{
			m_ctlContent.ResetContent();
			UpdateData();

			CWDMDatabase* pDB=(CWDMDatabase*)m_pOb;
			pDB->GetDBServerData(data);

			switch ( data.m_nType )
			{				
			case 0:   // SQL Server
				{					
					strSql=_T("SELECT name FROM sys.databases");
				}
				break;
			case 1:  // Oracle
				{
					strSql=_T("SELECT name FROM  v$database");
				}
				break;
			case 2:  //MySQL
				{
					data.m_strDatabase = _T("information_schema");
					strSql=_T("SELECT SCHEMA_NAME as name FROM information_schema.SCHEMATA");
				}
				break;
			}			
		}
		break;
	case OBJECT_TYPE_TABLE:
		{
			m_ctlContent.ResetContent();
			UpdateData();
			
			CWDMDatabase* pDB=(CWDMDatabase*)m_pOb->GetParent();
            pDB->GetDBServerData(data);

			switch ( data.m_nType )
			{				
			case 0:   // SQL Server
				{
					strSql=_T("select name from sysobjects \
							  where type = 'U' and name <> 'dtproperties' order by name");
				}
				break;
			case 1:  // Oracle
				{
					strSql = _T("select table_name as name from user_tables order by name");

				}
				break;
			case 2:  //MySQL
				{
					data.m_strDatabase = _T("information_schema");
					strSql.Format("SELECT TABLE_NAME as name FROM information_schema.TABLES \
								  where TABLE_SCHEMA='%s' order by name",pDB->GetDesc());
				}
				break;
			}
		}
		break;
	case OBJECT_TYPE_OPERATOR:
		{
			bInsert = FALSE;
			m_ctlContent.ResetContent();
			UpdateData();

			m_ctlContent.AddString(_T("保存"));
			m_ctlContent.AddString(_T("删除"));
		}		
		break;
	default:
		ASSERT(FALSE);
	}

	//Insert the data into the combo ctrl
	if(bInsert && theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{			
		CCommonFunction::InsertDataToComboBox(m_ctlContent,strSql,_T("name"),CWebDataMiningApp::remoteServer);

		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}
