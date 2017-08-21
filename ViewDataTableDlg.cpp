// ViewDataTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ViewDataTableDlg.h"
#include "Column.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDataTableDlg dialog


CViewDataTableDlg::CViewDataTableDlg(CString strEntryID,CWnd* pParent /* = NULL */)
	: CDialogEx(CViewDataTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewDataTableDlg)
	//}}AFX_DATA_INIT
	m_pEntry=(CEntry*)theApp.GetSourceTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_ENTRY,strEntryID,TRUE);
	ASSERT(m_pEntry!=NULL);
}


void CViewDataTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDataTableDlg)
	DDX_Control(pDX, IDC_LIST_SELECTING_FIELD, m_listSelectingFields);
	DDX_Control(pDX, IDC_LIST_SELECTED_FIELD, m_lisSelectedFields);
	DDX_Control(pDX, IDC_LIST_CONTENT, m_listTableContent);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_ctlDataTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDataTableDlg, CDialogEx)
	//{{AFX_MSG_MAP(CViewDataTableDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_CBN_DROPDOWN(IDC_COMBO_TABLE, OnDropdownComboTable)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, OnSelchangeComboTable)
	ON_LBN_DBLCLK(IDC_LIST_SELECTED_FIELD, OnDblclkListSelectedField)
	ON_LBN_DBLCLK(IDC_LIST_SELECTING_FIELD, OnDblclkListSelectingField)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_ADD,OnUpdateAdd)
	ON_UPDATE_COMMAND_UI(IDC_DEL,OnUpdateDel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDataTableDlg message handlers

BOOL CViewDataTableDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_listTableContent.ModifyStyle(0,LVS_REPORT | LVS_EDITLABELS);
	m_listTableContent.SetExtendedStyle(m_listTableContent.GetExtendedStyle()
		                               |LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewDataTableDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	int nCount = m_listSelectingFields.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listSelectingFields.GetSelItems(nCount, aryListBoxSel.GetData()); 
   
	CString strFieldName;
    for(i=0;i<aryListBoxSel.GetSize();i++)
	{
		m_listSelectingFields.GetText(aryListBoxSel.GetAt(i),strFieldName);
		m_lisSelectedFields.AddString(strFieldName);
	}
	CArray<int,int> arySorted;
	for(i=0;i<aryListBoxSel.GetSize();i++)	
	{
         for(j=0;j<arySorted.GetSize();j++)
		 {
			 if(aryListBoxSel.GetAt(i)>arySorted.GetAt(j))
			 {
				 arySorted.InsertAt(j,aryListBoxSel.GetAt(i));
				 break;
			 }

		 }
		 if(j==arySorted.GetSize())
			 arySorted.Add(aryListBoxSel.GetAt(i));
	}
    for(i=0;i<arySorted.GetSize();i++)
		m_listSelectingFields.DeleteString(arySorted.GetAt(i));

	m_listSelectingFields.SetSel(-1,FALSE);
	// Show the table content
    ShowContent();
}

void CViewDataTableDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	int nCount = m_lisSelectedFields.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_lisSelectedFields.GetSelItems(nCount, aryListBoxSel.GetData()); 
   
	CString strFieldName;
    for(i=0;i<aryListBoxSel.GetSize();i++)
	{
		m_lisSelectedFields.GetText(aryListBoxSel.GetAt(i),strFieldName);
		m_listSelectingFields.AddString(strFieldName);
	}
	CArray<int,int> arySorted;
	for(i=0;i<aryListBoxSel.GetSize();i++)	
	{
         for(j=0;j<arySorted.GetSize();j++)
		 {
			 if(aryListBoxSel.GetAt(i)>arySorted.GetAt(j))
			 {
				 arySorted.InsertAt(j,aryListBoxSel.GetAt(i));
				 break;
			 }

		 }
		 if(j==arySorted.GetSize())
			 arySorted.Add(aryListBoxSel.GetAt(i));
	}
    for(i=0;i<arySorted.GetSize();i++)
		m_lisSelectedFields.DeleteString(arySorted.GetAt(i));

	m_lisSelectedFields.SetSel(-1,FALSE);
	//
	ShowContent();
}

void CViewDataTableDlg::OnDropdownComboTable() 
{
	// TODO: Add your control notification handler code here
	m_ctlDataTable.ResetContent();
	UpdateData();
    
	CWDMDatabase* pDB=GetDatabase();
	if(pDB==NULL)
		return;

	CString strSql;
	COptionsData::OptionsData::DBServerData data;
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
	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{
		CCommonFunction::InsertDataToComboBox(m_ctlDataTable,strSql,_T("name"),CWebDataMiningApp::remoteServer);

		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}

void CViewDataTableDlg::OnSelchangeComboTable() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_ctlDataTable.GetCurSel();
	if(nIndex==CB_ERR)
		return;

	m_listSelectingFields.ResetContent();
    m_lisSelectedFields.ResetContent();
	m_listTableContent.DeleteAllItems();
	m_listTableContent.DeleteAllColumn();

	CWDMDatabase* pDB=GetDatabase();
	if(pDB==NULL)
		return;

	CString strSql,strTableName;
	COptionsData::OptionsData::DBServerData data;

    pDB->GetDBServerData(data);
	m_ctlDataTable.GetLBText(nIndex,strTableName);

	switch ( data.m_nType )
	{
	case 0://SQL server
		strSql.Format("Select a.status as Status, a.cdefault as cdefault, a.id as id,  \
					  a.colorder as colorder, c.name as TableName, a.name as ColumnName,  \
					  b.name as TypeName, a.length as Length, a.scale as Scale, a.isnullable as IsNullAble  \
					  from syscolumns a, systypes b, sysObjects c  \
					  where a.xtype = b.xusertype and a.id = c.id and c.xtype='U' and c.name = '%s' order by a.colorder",strTableName);
		break;
	case 1://Oracle
		strSql.Format("select COLUMN_NAME as ColumnName,DATA_TYPE as TypeName,DATA_LENGTH as Length \
					  from user_tab_columns where table_name='%s' order by ColumnName",strTableName);
		break;
	case 2://Mysql
		{
			data.m_strDatabase = _T("information_schema");
			strSql.Format("SELECT COLUMN_NAME as ColumnName,DATA_TYPE as TypeName,CHARACTER_MAXIMUM_LENGTH as Length \
						  FROM information_schema.columns where TABLE_SCHEMA='%s' and TABLE_NAME='%s' order by ColumnName",
						  pDB->GetDesc(),strTableName);
		}
		break;
	}	

	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{ 
		CString str;
		CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));
		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			while(!rs.IsEOF())
			{
				rs.GetFieldValue("ColumnName",str);
				m_listSelectingFields.AddString(str);
				
				rs.MoveNext();			
			}
			rs.Close();
		}		
		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}

void CViewDataTableDlg::OnDblclkListSelectedField() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_lisSelectedFields.GetCurSel();
	if(nIndex==LB_ERR)
		return;
    
	CString str;
	m_lisSelectedFields.GetText(nIndex,str);
	m_lisSelectedFields.DeleteString(nIndex);
	m_listSelectingFields.AddString(str);

	ShowContent();

}

void CViewDataTableDlg::OnDblclkListSelectingField() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_listSelectingFields.GetCurSel();
	if(nIndex==LB_ERR)
		return;
    
	CString str;
	m_listSelectingFields.GetText(nIndex,str);
	m_listSelectingFields.DeleteString(nIndex);
	m_lisSelectedFields.AddString(str);

	ShowContent();
}

BOOL CViewDataTableDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CViewDataTableDlg::OnUpdateAdd(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_listSelectingFields.GetSelCount()>0);
}

void CViewDataTableDlg::OnUpdateDel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_lisSelectedFields.GetSelCount()>0);
}

CString CViewDataTableDlg::GetColumnString()
{
	CString strRet,str;		
	CClientDC dc (this);
//     TEXTMETRIC tm;
// 	dc.GetTextMetrics (&tm);

	for(int i=0;i<m_lisSelectedFields.GetCount();i++)
	{
         m_lisSelectedFields.GetText(i,str);
		 strRet+=str;
		 str.Format(",%d;",dc.GetTextExtent(str).cx+30);
		 strRet+=str;
	}
	return strRet.Mid(0,strRet.GetLength()-1);
}	
void CViewDataTableDlg::ShowContent()
{
    m_listTableContent.DeleteAllItems();
	m_listTableContent.DeleteAllColumn();
    
	if(m_lisSelectedFields.GetCount()<=0)
        return;

	m_listTableContent.SetHeadings(GetColumnString());
//     for(int i=0;i<m_listTableContent.GetColumnCounts();i++)
// 		m_listTableContent.SetReadOnlyColumns(i);

 	CWDMDatabase* pDB=GetDatabase();
	if(pDB==NULL)
		return;

	COptionsData::OptionsData::DBServerData data;
    pDB->GetDBServerData(data);

	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{
		CString strSql,str,strValue;
        m_ctlDataTable.GetLBText(m_ctlDataTable.GetCurSel(),str);

		strSql.Format("select * from %s",str);
	    CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));

		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			CStringArray  listItemTexts;
			while (!rs.IsEOF())
			{
				listItemTexts.RemoveAll();
				for(int i=0;i<m_lisSelectedFields.GetCount();i++)
				{
					m_lisSelectedFields.GetText(i,str);
                    rs.GetFieldValue(str,strValue);
					listItemTexts.Add(strValue);
				}
				m_listTableContent.AddItem(listItemTexts);

				rs.MoveNext();				
			}
			rs.Close();
		}

		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}

CWDMDatabase* CViewDataTableDlg::GetDatabase()
{
	CColumn* pColumn=(CColumn*)m_pEntry->BackTo(OBJECT_TYPE_COLUMN);
	ASSERT(pColumn!=NULL);
    // find the top column
	while (pColumn!=NULL && pColumn->m_data.m_strDatabaseID.IsEmpty())
	{
		pColumn=(CColumn*)pColumn->BackTo(OBJECT_TYPE_COLUMN);
	}
    
	if(pColumn==NULL)
		return NULL;

	CBaseObject* pOb=theApp.GetDBServerTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_DATABASE,pColumn->m_data.m_strDatabaseID,TRUE);
    if(pOb==NULL)
		return NULL;

	return static_cast<CWDMDatabase*>(pOb);	
}

void CViewDataTableDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialogEx::PostNcDestroy();
}
