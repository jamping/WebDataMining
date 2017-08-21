// UpdateServerColumnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "UpdateServerColumnDlg.h"
#include "ServerColumn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdateServerColumnDlg dialog


CUpdateServerColumnDlg::CUpdateServerColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateServerColumnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdateServerColumnDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_strRootValue = _T("0");
	//}}AFX_DATA_INIT
	m_pDB=NULL;	
}


void CUpdateServerColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdateServerColumnDlg)
	DDX_Control(pDX, IDC_PROGRESS_UPDATE, m_progress);
	DDX_Control(pDX, IDC_LIST_CONTENT, m_listContent);
	DDX_Control(pDX, IDC_LIST_SELECTING_FIELD, m_listSelectingField);
	DDX_Control(pDX, IDC_LIST_SELECTED_FIELD, m_listSelectedField);
	DDX_Control(pDX, IDC_LIST_FIELD_MAP, m_listFieldMap);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_comboTable);
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDB);
	DDX_Text(pDX, IDC_EDIT_ROOT_VALUE, m_strRootValue);
	DDX_Control(pDX, IDC_TREE_SERVER_COLUMNS, m_tree);	
	//}}AFX_DATA_MAP
}
void CUpdateServerColumnDlg::Init()
{
	//初始化已有服务器栏目
	m_tree.SetImageList(IDB_TREEIMGLIST,16,4,RGB(255,255,255));
	theApp.GetServerColumnRoot()->ShowInTreeView(&m_tree,TRUE);
	//初始化数据库信息
	CBaseObject* pServer=NULL;
	POSITION pos,pos1;

	pos=theApp.GetDBServerTree()->m_objectHelper.GetFirstChildPosition();
	while (pos)
	{
		pServer=theApp.GetDBServerTree()->m_objectHelper.GetNextChild(pos);
		if(pServer!=NULL)
		{
            pos1=pServer->GetFirstChildPosition();
			while (pos1)
			{
				CWDMDatabase* pDB=(CWDMDatabase*)pServer->GetNextChild(pos1);
				if(pDB!=NULL)
				{
					m_comboDB.SetItemData(m_comboDB.AddString(pDB->GetDesc()),(DWORD)pDB);
				}
			}
		}
	}
	//初始化数据表列表
	m_listContent.ModifyStyle(0,LVS_REPORT | LVS_EDITLABELS);
	m_listContent.SetExtendedStyle(m_listContent.GetExtendedStyle() | 
		                            LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//初始化字段映射列表
	m_listFieldMap.ModifyStyle(0,LVS_REPORT | LVS_EDITLABELS);
	m_listFieldMap.SetExtendedStyle(m_listFieldMap.GetExtendedStyle() | 
		                            LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listFieldMap.SetHeadings(_T("序号,60;本地字段名,150;服务器字段名,150;字段说明,200"));
	m_listFieldMap.SetReadOnlyColumns(0);
	m_listFieldMap.SetReadOnlyColumns(1);
    m_listFieldMap.SetComboColumns(2);
	m_listFieldMap.SetReadOnlyColumns(3);

	m_listFieldMap.AddItem(_T("1"),_T("type_id"),_T(""),_T("本级节点ID，varchar(40)"));
	m_listFieldMap.AddItem(_T("2"),_T("parent_id"),_T(""),_T("上级节点ID，varchar(40)"));
	m_listFieldMap.AddItem(_T("3"),_T("type_name"),_T(""),_T("节点名称，varchar(40)"));

	UpdateData(FALSE);
}
void CUpdateServerColumnDlg::InitMapList()
{
	m_listFieldMap.SetItemText(0,2,_T(""));
	m_listFieldMap.SetItemText(1,2,_T(""));
	m_listFieldMap.SetItemText(2,2,_T(""));

	m_listFieldMap.UpdateData(FALSE);
}
BOOL CUpdateServerColumnDlg::IsUpdate()
{
	UpdateData();

	BOOL bRet=( m_pDB!=NULL && m_listSelectedField.GetCount()>=3 );

    if(bRet)
	{
         CString str;
		 str=m_listFieldMap.GetItemText(0,2);
		 bRet &= !str.IsEmpty();

		 str=m_listFieldMap.GetItemText(1,2);		 
		 bRet &= !str.IsEmpty();

		 str=m_listFieldMap.GetItemText(2,2);
		 bRet &= !str.IsEmpty();
	}
//	bRet &= !m_strRootValue.IsEmpty();

	return bRet;
}
CString CUpdateServerColumnDlg::GetColumnString()
{
	CString strRet,str;		
	CClientDC dc (this);

	for(int i=0;i<m_listSelectedField.GetCount();i++)
	{
         m_listSelectedField.GetText(i,str);
		 strRet+=str;
		 str.Format(",%d;",dc.GetTextExtent(str).cx+30);
		 strRet+=str;
	}
	return strRet.Mid(0,strRet.GetLength()-1);
}	
void CUpdateServerColumnDlg::ShowContent()
{
    m_listContent.DeleteAllItems();
	m_listContent.DeleteAllColumn();
    
	if(m_listSelectedField.GetCount()<=0)
        return;

	m_listContent.SetHeadings(GetColumnString());
    for(int i=0;i<m_listContent.GetColumnCounts();i++)
		m_listContent.SetReadOnlyColumns(i);

	if(m_pDB==NULL)
		return;

	COptionsData::OptionsData::DBServerData data;
    m_pDB->GetDBServerData(data);

	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{
		CString strSql,str,strValue;

		strSql=_T("select ");

		if( data.m_nType == 0 )// sql server
			strSql += _T("top 100");

		for(int i=0;i<m_listSelectedField.GetCount();i++)
		{
			m_listSelectedField.GetText(i,str);
			if(i!=m_listSelectedField.GetCount()-1)
				str+=_T(",");

			strSql+=str;
             
		}
		str.Format(" from %s",m_strTableName);
		strSql+=str;

		if( data.m_nType ==1 )//oracle
			strSql += _T(" where rownum<100");
		else if( data.m_nType == 2 )//mysql
			strSql += _T(" LIMIT 0 , 100");

	    CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));

		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			CStringArray  listItemTexts;
			while (!rs.IsEOF())
			{
				listItemTexts.RemoveAll();
				for(int i=0;i<m_listSelectedField.GetCount();i++)
				{
					m_listSelectedField.GetText(i,str);
                    rs.GetFieldValue(str,strValue);
					listItemTexts.Add(strValue);
				}
				m_listContent.AddItem(listItemTexts);

				rs.MoveNext();				
			}
			rs.Close();
		}

		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}
BEGIN_MESSAGE_MAP(CUpdateServerColumnDlg, CDialogEx)
	//{{AFX_MSG_MAP(CUpdateServerColumnDlg)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_DATABASE, OnSelchangeComboDatabase)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, OnSelchangeComboTable)
	ON_LBN_DBLCLK(IDC_LIST_SELECTING_FIELD, OnDblclkListSelectingField)
	ON_LBN_DBLCLK(IDC_LIST_SELECTED_FIELD, OnDblclkListSelectedField)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SET_ITEMS,OnLoadComboboxData)
	ON_UPDATE_COMMAND_UI(IDC_ADD,OnUpdateAdd)
	ON_UPDATE_COMMAND_UI(IDC_DEL,OnUpdateDel)
	ON_UPDATE_COMMAND_UI(IDC_UPDATE,OnUpdateUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateServerColumnDlg message handlers

void CUpdateServerColumnDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox(_T("导入服务器栏目信息！"),_T("请确认"),MB_YESNO|MB_ICONQUESTION)==IDYES)
	{		
		CWaitCursor wait;
		UpdateData(TRUE);

		COptionsData::OptionsData::DBServerData data;
        m_pDB->GetDBServerData(data);
		
		if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
		{
			CString strSql,str;
			
			strSql.Format("select %s,%s,%s from %s",
				m_listFieldMap.GetItemText(0,2),
				m_listFieldMap.GetItemText(1,2),
				m_listFieldMap.GetItemText(2,2),
				m_strTableName);

			CADORecordset rsServer(theApp.GetDB(CWebDataMiningApp::remoteServer));
			
			if(rsServer.Open(strSql,CADORecordset::openQuery))
			{
				CADORecordset rs(theApp.GetDB());
				//Delete the old data
				//theApp.GetDB()->Execute(_T("delete from wdm_k_type"));

				CBaseObject* pRoot = theApp.GetServerColumnRoot();
				CServerColumn* pServerColumn = NULL;

				CBaseObject* pServerDB = pRoot->FindSubObject(OBJECT_TYPE_SERVER_COLUMN,m_pDB->GetID(),TRUE);
				if( pServerDB != NULL )
				{
					pServerColumn = static_cast<CServerColumn*>(pServerDB);
					//find the old data, and delete all the data
					//delete the data from the db
					pServerDB->DeleteFromDB(*theApp.GetDB(),FALSE);
					//delete the data from the tree
					m_tree.DeleteAllChildItem(pServerDB->GetRefItem());
				}
				else
				{
					
					// insert the DBserver object as the root
					strSql.Format("select * from wdm_k_type where type_id='%s'",m_pDB->GetID());
					if(rs.Open(strSql,CADORecordset::openQuery))
					{
						rs.AddNew();
						rs.SetFieldValue(_T("type_id"),m_pDB->GetID());	
						rs.SetFieldValue(_T("parent_id"),(CString)NULL_ID);
						rs.SetFieldValue(_T("type_name"),m_pDB->GetDesc());

						rs.Update();
						rs.Close();
					}
					//add the data to the root tree
					pServerColumn = new CServerColumn;
					pServerColumn->SetID(m_pDB->GetID(),m_pDB->GetDesc());
					pServerColumn->m_data.m_strColumnName=m_pDB->GetDesc();
					pServerColumn->AddTo(pRoot);
				}

				m_progress.SetRange32(0,rsServer.GetRecordCount());
				int i=0;

               //insert the DBserver object as the root
// 				strSql.Format("select * from wdm_k_type where type_id='%s'",m_pDB->GetID());
// 				if(rs.Open(strSql,CADORecordset::openQuery))
// 				{
// 					rs.AddNew();
// 					rs.SetFieldValue(_T("type_id"),m_pDB->GetID());	
// 					rs.SetFieldValue(_T("parent_id"),(CString)NULL_ID);
// 					rs.SetFieldValue(_T("type_name"),m_pDB->GetDesc());
// 
// 					rs.Update();
// 					rs.Close();
// 				}
				//Insert the server data into the local db
                strSql.Format("select * from wdm_k_type where parent_id='%s'",m_pDB->GetID());
				if(rs.Open(strSql,CADORecordset::openQuery))
				{
					while (!rsServer.IsEOF())
					{	
						rs.AddNew();
						// insert the new id
						rsServer.GetFieldValue(m_listFieldMap.GetItemText(0,2),str);
						rs.SetFieldValue(_T("type_id"),str);
						// get parent id
						rsServer.GetFieldValue(m_listFieldMap.GetItemText(1,2),str);
						// if the parent id is equal to the root value
						if( str.IsEmpty() || str.CompareNoCase(m_strRootValue) == 0 )
							str = m_pDB->GetID();
						rs.SetFieldValue(_T("parent_id"),str);
						//get the name
						rsServer.GetFieldValue(m_listFieldMap.GetItemText(2,2),str);
						rs.SetFieldValue(_T("type_name"),str);

						rs.Update();

						rsServer.MoveNext();
						m_progress.SetPos(++i);
					}
					rs.Close();
				}
				rsServer.Close();
				//reload the new data	

 				//theApp.InitServerColumn();
 				//m_tree.DeleteAllItems();
				//theApp.GetServerColumnRoot()->ShowInTreeView(&m_tree,TRUE);
				theApp.InitServerColumn(pServerColumn);
				pServerColumn->ShowInTreeView(&m_tree,TRUE);
			}
			theApp.CloseDB(CWebDataMiningApp::remoteServer);
		}		
	}
}

void CUpdateServerColumnDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	int nCount = m_listSelectedField.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listSelectedField.GetSelItems(nCount, aryListBoxSel.GetData()); 
   
	CString strFieldName;
    for(i=0;i<aryListBoxSel.GetSize();i++)
	{
		m_listSelectedField.GetText(aryListBoxSel.GetAt(i),strFieldName);
		m_listSelectingField.AddString(strFieldName);
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
		m_listSelectedField.DeleteString(arySorted.GetAt(i));

	m_listSelectedField.SetSel(-1,FALSE);
	//
	ShowContent();
	InitMapList();
}

void CUpdateServerColumnDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	int i,j;
	int nCount = m_listSelectingField.GetSelCount();

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listSelectingField.GetSelItems(nCount, aryListBoxSel.GetData()); 
   
	CString strFieldName;
    for(i=0;i<aryListBoxSel.GetSize();i++)
	{
		m_listSelectingField.GetText(aryListBoxSel.GetAt(i),strFieldName);
		m_listSelectedField.AddString(strFieldName);
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
		m_listSelectingField.DeleteString(arySorted.GetAt(i));

	m_listSelectingField.SetSel(-1,FALSE);
	// Show the table content
    ShowContent();
	InitMapList();
}

void CUpdateServerColumnDlg::OnSelchangeComboDatabase() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_comboDB.GetCurSel();
	if(nIndex==CB_ERR)
		return;

	m_comboTable.ResetContent();
	m_listSelectingField.ResetContent();
	m_listSelectedField.ResetContent();
	ShowContent();
	InitMapList();

    UpdateData();
	
	m_pDB=(CWDMDatabase*)m_comboDB.GetItemData(nIndex);	
	if(m_pDB==NULL)
		return;	

	CWaitCursor wait;

	CString strSql;
	COptionsData::OptionsData::DBServerData data;
    m_pDB->GetDBServerData(data);

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
						  where TABLE_SCHEMA='%s' order by name",m_pDB->GetDesc());
		}
		break;
	}
	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{	
		CCommonFunction::InsertDataToComboBox(m_comboTable,strSql,_T("name"),CWebDataMiningApp::remoteServer);

		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}

void CUpdateServerColumnDlg::OnSelchangeComboTable() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_comboTable.GetCurSel();
	if(nIndex==CB_ERR)
		return;

	m_comboTable.GetLBText(nIndex,m_strTableName);

	m_listSelectingField.ResetContent();
    m_listSelectedField.ResetContent();
	ShowContent();
	InitMapList();

	UpdateData();

	if(m_pDB==NULL)
		return;	

	CString strSql;
	COptionsData::OptionsData::DBServerData data;
    m_pDB->GetDBServerData(data);

	switch ( data.m_nType )
	{
	case 0://SQL server
		strSql.Format("Select a.status as Status, a.cdefault as cdefault, a.id as id,  \
					  a.colorder as colorder, c.name as TableName, a.name as ColumnName,  \
					  b.name as TypeName, a.length as Length, a.scale as Scale, a.isnullable as IsNullAble  \
					  from syscolumns a, systypes b, sysObjects c  \
					  where a.xtype = b.xusertype and a.id = c.id and c.xtype='U' and c.name = '%s' order by a.colorder",m_strTableName);
		break;
	case 1://Oracle
		strSql.Format("select COLUMN_NAME as ColumnName,DATA_TYPE as TypeName,DATA_LENGTH as Length \
					  from user_tab_columns where table_name='%s' order by ColumnName",m_strTableName);
		break;
	case 2://Mysql
		{
			data.m_strDatabase = _T("information_schema");
			strSql.Format("SELECT COLUMN_NAME as ColumnName,DATA_TYPE as TypeName,CHARACTER_MAXIMUM_LENGTH as Length \
						  FROM information_schema.columns where TABLE_SCHEMA='%s' and TABLE_NAME='%s' order by ColumnName",
						  m_pDB->GetDesc(),m_strTableName);
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
				m_listSelectingField.AddString(str);
				
				rs.MoveNext();			
			}
			rs.Close();
		}		
		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}

BOOL CUpdateServerColumnDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here	
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
LRESULT CUpdateServerColumnDlg::OnLoadComboboxData(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	CStringList* strList=(CStringList*)lParam;
    //Read data from directory table
	CString str;
    for(int i=0;i<m_listSelectedField.GetCount();i++)
	{
         m_listSelectedField.GetText(i,str);
		 strList->AddTail(str);
	}

	return 0;
}
void CUpdateServerColumnDlg::OnUpdateAdd(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_listSelectingField.GetSelCount()>0);
}

void CUpdateServerColumnDlg::OnUpdateDel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_listSelectedField.GetSelCount()>0);
}

void CUpdateServerColumnDlg::OnUpdateUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsUpdate());
}

BOOL CUpdateServerColumnDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CUpdateServerColumnDlg::OnDblclkListSelectingField() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_listSelectingField.GetCurSel();
	if(nIndex==LB_ERR)
		return;
    
	CString str;
	m_listSelectingField.GetText(nIndex,str);
	m_listSelectingField.DeleteString(nIndex);
	m_listSelectedField.AddString(str);

	ShowContent();
	InitMapList();
}

void CUpdateServerColumnDlg::OnDblclkListSelectedField() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_listSelectedField.GetCurSel();
	if(nIndex==LB_ERR)
		return;
    
	CString str;
	m_listSelectedField.GetText(nIndex,str);
	m_listSelectedField.DeleteString(nIndex);
	m_listSelectingField.AddString(str);

	ShowContent();
	InitMapList();
}
void CUpdateServerColumnDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	theApp.GetServerColumnRoot()->InvalidateItemHandle();
}