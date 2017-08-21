// ServerTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ServerTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerTableDlg dialog


CServerTableDlg::CServerTableDlg(CBaseObject* pObject,CWnd* pParent /* = NULL */)
	: CDialogEx(CServerTableDlg::IDD, pParent),m_pOb(pObject)
{
	//{{AFX_DATA_INIT(CServerTableDlg)
	m_nType = 0;
	m_strServerTable = _T("");
	m_nProcessOrder = 0;
	m_bPublishRepeat = TRUE;
	//}}AFX_DATA_INIT
}

BOOL CServerTableDlg::GetValue(CWDMTable::WDMTableData& data)
{
	BOOL bModify=FALSE;
	if( !CCommonFunction::IsSameString(data.m_strTableName,m_strServerTable))
	{
		data.m_strTableName=m_strServerTable;
		bModify |= TRUE;
	}
	if( data.m_nTableType!=m_nType)
	{
		data.m_nTableType=m_nType;
		bModify |= TRUE;
	}
	if( data.m_nProcessOrder!=m_nProcessOrder)
	{
		data.m_nProcessOrder=m_nProcessOrder;
		bModify |= TRUE;
	}
	if( data.m_bPublishRepeat!=m_bPublishRepeat)
	{
		data.m_bPublishRepeat=m_bPublishRepeat;
		bModify |= TRUE;
	}
	return bModify;
}
void CServerTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerTableDlg)
	DDX_Control(pDX, IDC_CONTENT, m_ctlServerTable);
	DDX_Radio(pDX, IDC_TABLE_TYPE, m_nType);
	DDX_CBString(pDX, IDC_CONTENT, m_strServerTable);
	DDX_Text(pDX,IDC_PROCESS_ORDER,m_nProcessOrder);
	DDV_MinMaxInt(pDX, m_nProcessOrder, 1, 10);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_REPEAT, m_bPublishRepeat);
}


BEGIN_MESSAGE_MAP(CServerTableDlg, CDialogEx)
	//{{AFX_MSG_MAP(CServerTableDlg)
	ON_CBN_DROPDOWN(IDC_CONTENT, OnDropdownContent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerTableDlg message handlers

BOOL CServerTableDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Set the spin ctrl
	CSpinButtonCtrl* pSpin = ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1));
	pSpin->SetRange(1, 10);

// 	UDACCEL ua;
// 	pSpin->GetAccel(1,&ua);
// 	ua.nInc = ua.nInc*10;
// 	pSpin->SetAccel(1,&ua);

	m_strServerTable=(static_cast<CWDMTable*>(m_pOb))->m_data.m_strTableName;
	m_nType=(static_cast<CWDMTable*>(m_pOb))->m_data.m_nTableType;
	m_nProcessOrder=(static_cast<CWDMTable*>(m_pOb))->m_data.m_nProcessOrder;
	m_bPublishRepeat = (static_cast<CWDMTable*>(m_pOb))->m_data.m_bPublishRepeat;

    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerTableDlg::OnDropdownContent() 
{
	// TODO: Add your control notification handler code here
	m_ctlServerTable.ResetContent();
	UpdateData();
	
	CString strSql;
	COptionsData::OptionsData::DBServerData data;
	CWDMDatabase* pDB=(CWDMDatabase*)m_pOb->GetParent();
	pDB->GetDBServerData(data);
	
	switch ( data.m_nType )
	{
	case 0:  // Sql server
		strSql=_T("select name from sysobjects \
				  where type = 'U' and name <> 'dtproperties' order by name");
		break;
	case 1:  // Oracle
		strSql = _T("select table_name as name from user_tables");
		break;
	case 2: // Mysql 
		{
			data.m_strDatabase = _T("information_schema");
			strSql.Format("SELECT TABLE_NAME as name FROM information_schema.TABLES where TABLE_SCHEMA='%s' order by name",pDB->GetDesc());
		}
		break;
	}
	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{
		CCommonFunction::InsertDataToComboBox(m_ctlServerTable,strSql,_T("name"),CWebDataMiningApp::remoteServer);
		
		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}
