// WDMDatabase.cpp: implementation of the CWDMDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WDMDatabase.h"
#include "WDMTable.h"
#include "SelContentDlg.h"
#include "DBServer.h"
#include "DataBase.h"
#include "AccessDataBase.h"
#include "SQLDataBase.h"
#include "OracleDatabase.h"
#include "MySQLDataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWDMDatabase::CWDMDatabase():CBaseObject(OBJECT_TYPE_DATABASE)
{
    SetImg(22,23);
}

CWDMDatabase::~CWDMDatabase()
{

}
void CWDMDatabase::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strDatabaseID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CWDMDatabase::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;
    switch(pOB->GetType())
    {
	case OBJECT_TYPE_TABLE:
		bAccept=TRUE;
		break;
    }
	return bAccept;
}
void CWDMDatabase::DoJoin(CBaseObject *pParent)
{
	m_data.m_strServerID=pParent->GetID();

	CBaseObject::DoJoin(this);
}		
void CWDMDatabase::DoLeave(CBaseObject *pParent)
{
	m_data.m_strServerID.Empty();

	CBaseObject::DoLeave(this);
}
UINT CWDMDatabase::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CSelContentDlg dlg(this);

	nRet=dlg.DoModal();
	if(nRet== IDOK)
	{
		if(!CCommonFunction::IsSameString(m_data.m_strDatabaseName,dlg.m_strSelContent))
		{
			SetModify();
			m_data.m_strDatabaseName=dlg.m_strSelContent;
		    m_strDesc=m_data.m_strDatabaseName;
		}
	}
	return nRet;
}
BOOL CWDMDatabase::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_database where database_id='%s'",m_data.m_strDatabaseID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("database_id",m_data.m_strDatabaseID);
		}
		else
			rs.Edit();
	
		bRet&=rs.SetFieldValue("server_id",m_data.m_strServerID);
		bRet&=rs.SetFieldValue("database_name",m_data.m_strDatabaseName);
	
		bRet&=rs.Update();	

		rs.Close();
	}

	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CWDMDatabase::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;

	strSql.Format("select * from wdm_table where database_id='%s' order by process_order asc",m_data.m_strDatabaseID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CWDMTable* pTable=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pTable=new CWDMTable;
			pTable->LoadInfo(rs);
			pTable->AddTo(this);
            
			rs.MoveNext();
		}
		rs.Close();
	}
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CWDMDatabase::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("database_id",m_data.m_strDatabaseID);
	bRet&=rs.GetFieldValue("server_id",m_data.m_strServerID);
	bRet&=rs.GetFieldValue("database_name",m_data.m_strDatabaseName);

	m_strID=m_data.m_strDatabaseID;
	m_strDesc=m_data.m_strDatabaseName;
	
	return bRet;
}
BOOL CWDMDatabase::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_database where database_id='%s'",m_data.m_strDatabaseID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CWDMDatabase::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CWDMDatabase::PromptMenuPos()
{
	return 8;
}
void CWDMDatabase::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除数据库");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}

BOOL CWDMDatabase::GetDBServerData(COptionsData::OptionsData::DBServerData& data)
{
	CDBServer* pDBServer=(CDBServer*)GetParent();
	
	data.m_strServer=pDBServer->m_data.m_strServerName;
	data.m_strUser=pDBServer->m_data.m_strUserName;
	data.m_strPass=pDBServer->m_data.m_strUserPasswd;
	data.m_strDatabase=m_data.m_strDatabaseName;
	data.m_strResourceDir=pDBServer->m_data.m_strResourceDir;
	data.m_strResourcePath=pDBServer->m_data.m_strResourcePath;
    data.m_nType=pDBServer->m_data.m_nDatabaseType;
	data.m_nPort=pDBServer->m_data.m_nServerPort;
	data.m_strCode=pDBServer->m_data.m_strServerCode;

	return TRUE;
}
CWDMTable* CWDMDatabase::GetWDMTable(CWDMTable::TableType type)
{
	CWDMTable* pTable=NULL;

	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		pTable=(CWDMTable*)GetNextChild(pos);
		if(pTable!=NULL && pTable->m_data.m_nTableType==(int)type)
		{
			return pTable;
		}		
	}
	return NULL;
}
int CWDMDatabase::GetWDMTables(CTypedPtrArray<CPtrArray,CWDMTable*>& tables,CWDMTable::TableType type,BOOL bAsc /* = TRUE */)
{
	tables.RemoveAll();

	CWDMTable* pTable=NULL;
	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		pTable=(CWDMTable*)GetNextChild(pos);
		if(pTable!=NULL && pTable->m_data.m_nTableType==(int)type)
		{			
			tables.Add(pTable);
		}		
	}
    //select sort
	register int i, j, min; 
	for( i=0;i<tables.GetCount()-1;i++ )
	{
		min = i;
		//查找最小值 
		for( j=i+1;j<tables.GetCount();j++)
		{
			if( tables[min]->m_data.m_nProcessOrder > tables[j]->m_data.m_nProcessOrder )
				min = j;
		}
		//交换
		if( min != i )
		{
			pTable = tables[min];
			tables[min] = tables[i];
			tables[i] = pTable;
		}
	}
	// 降序排列
	if( bAsc == FALSE )
	{
		 int num = tables.GetCount();
		 for(int i=0;i<num/2;i++)
		 {
			 // 对称轴数据交换
			 pTable = tables[i];
			 tables[i] = tables[num-1-i];
			 tables[num-1-i] = pTable;
		 }
	}
	return tables.GetCount();
}

BOOL CWDMDatabase::IsValid()
{
    if(GetChildCount()==0)
		return FALSE;

    POSITION pos=GetFirstChildPosition();
    while (pos)
    {
		CWDMTable* pTable=(CWDMTable*)GetNextChild(pos);
		//check operator
		CWDMOperator* pOperator=pTable->GetOperator(CWDMOperator::opSave);
		if(pOperator==NULL)
			return FALSE;
        if(pOperator->IsValid()==FALSE)
			return FALSE;
    }
	return TRUE;
}
BOOL CWDMDatabase::IsConverted()
{
	POSITION pos=GetFirstChildPosition();
    while (pos)
    {
		CWDMTable* pTable=(CWDMTable*)GetNextChild(pos);
		//check operator
		CWDMOperator* pOperator=pTable->GetOperator(CWDMOperator::opSave);
		
        if(pOperator->IsConverted()==TRUE)
			return TRUE;
    }
	return FALSE;
}
void CWDMDatabase::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
   	CString strText;
	CStringArray strList;

	if(bSelf)
	{
        strText=_T("数据库名,200");
		pListCtrl->SetHeadings(strText);

		pListCtrl->SetItemData(pListCtrl->AddItem(m_data.m_strDatabaseName),(DWORD)this);
	}
	else
	{
 		 strText=_T("序号,60;数据表名,200;数据表类型,100;处理优先级,100;允许重发,100");
		 pListCtrl->SetHeadings(strText);
         
		 int i=0;
		 POSITION pos=GetFirstChildPosition();
		 while (pos)
		 {
			 CWDMTable* pTable=(CWDMTable*)GetNextChild(pos);
			 if(pTable!=NULL)
			 {
				 strList.RemoveAll();
				 
				 strText.Format("%d",++i);
				 strList.Add(strText);
				 
				 strList.Add(pTable->m_data.m_strTableName);	
				 switch(pTable->m_data.m_nTableType)
				 {
				 case 0:
					 strList.Add(_T("预处理"));
					 break;
				 case 1:
					 strList.Add(_T("文章"));
					 break;
				 case 2:
					 strList.Add(_T("图片"));
					 break;
				 case 3:
					 strList.Add(_T("后处理"));
					 break;
				 case 4:
					 strList.Add(_T("转向链接"));
					 break;
				 }
				 strText.Format("%d级",pTable->m_data.m_nProcessOrder);
				 strList.Add(strText);
				 strList.Add(pTable->m_data.m_bPublishRepeat?_T("是"):_T("否"));

				 pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pTable);
			 }

		 }
	
	}
}
TiXmlNode*  CWDMDatabase::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_database"));

	pTemp->SetAttribute(_T("database_id"),m_data.m_strDatabaseID);
	pTemp->SetAttribute(_T("database_name"),m_data.m_strDatabaseName);
	pTemp->SetAttribute(_T("server_id"),m_data.m_strServerID);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CWDMDatabase::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CWDMTable* pTable=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_table"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("table_id"));
		if(!strID.IsEmpty())
		{
			pTable=(CWDMTable*)FindSubObject(OBJECT_TYPE_TABLE,strID);
			if(pTable==NULL)
			{
				pTable=new CWDMTable;
				pTable->m_data.m_strTableID=strID;			
				pTable->AddTo(this);				
			}
			pTable->m_data.m_strTableName=pElement->Attribute(_T("table_name"));
			pTable->m_data.m_strDatabaseID=pElement->Attribute(_T("database_id"));
			pElement->QueryIntAttribute(_T("table_type"),&(pTable->m_data.m_nTableType));
			pElement->QueryIntAttribute(_T("process_order"),&(pTable->m_data.m_nProcessOrder));
			pElement->QueryIntAttribute(_T("publish_repeat"),&((int)pTable->m_data.m_bPublishRepeat));

			pTable->SetID(strID,pTable->m_data.m_strTableName);
			pTable->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

			pTable->StreamIn(pElement,bChild,pProgressBar);
		}		
	}
	return TRUE;
}

BOOL CWDMDatabase::GetAdoDatabase(CADODatabase& adoServer)
{
	BOOL bRet = FALSE;
	COptionsData::OptionsData::DBServerData data;
	GetDBServerData(data);
	// 根据数据库信息建立一个连接对象
	CString connStr;
	CDataBase* pServerDB = NULL;

	switch ( data.m_nType )
	{
	case 0:   //SQL server
		pServerDB = new CSQLDataBase(data.m_strUser,		
			data.m_strPass,
			data.m_strServer,
			data.m_strDatabase);
		break;
	case 1:   //Oracel server 
		pServerDB = new COracleDatabase(data.m_strUser,
			data.m_strPass,
			data.m_strServer,
			data.m_nPort);
		break;
	case 2:   //MySQL  server
		pServerDB = new CMySQLDataBase(data.m_strServer,
			data.m_strDatabase,
			data.m_strUser,
			data.m_strPass,
			data.m_nPort,
			data.m_strCode);
		break;
	case 3:   //Access
		break;
	}

	connStr = pServerDB->GetDataBaseConnString();
	delete pServerDB;
	pServerDB = NULL;	

	if( adoServer.IsOpen() )
		adoServer.Close();

	bRet = adoServer.Open(connStr);

	return bRet;
}