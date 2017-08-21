// DBServer.cpp: implementation of the CDBServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "DBServer.h"
#include "DBServerDlg.h"
#include "Des.h"
#include "WDMDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBServer::CDBServer():CBaseObject(OBJECT_TYPE_DBSERVER)
{
    SetImg(20,21);
	//m_pFileSend=NULL;
}

CDBServer::~CDBServer()
{
	//if( m_pFileSend )
	//{
	//	delete m_pFileSend;
	//	m_pFileSend = NULL;
	//}
}
void CDBServer::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strServerID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CDBServer::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
    BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_DATABASE:
		bAccept=TRUE;
		break;
	}

	return bAccept;
}
void CDBServer::DoJoin(CBaseObject *pParent)
{
	CBaseObject::DoJoin(this);
}		
void CDBServer::DoLeave(CBaseObject *pParent)
{
	CBaseObject::DoLeave(this);
}
UINT CDBServer::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CDBServerDlg dlg(FALSE);

	dlg.SetValue(m_data);
	nRet=dlg.DoModal();
	if(nRet== IDOK)
	{
		if(dlg.GetValue(m_data))
			SetModify();
		m_strDesc=m_data.m_strServerName;
	}	
	return nRet;
}
BOOL CDBServer::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_server where server_id='%s'",m_data.m_strServerID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("server_id",m_data.m_strServerID);
		}
		else
			rs.Edit();
	
		bRet&=rs.SetFieldValue("server_name",m_data.m_strServerName);
		bRet&=rs.SetFieldValue("user_name",m_data.m_strUserName);
		CDes des;
		bRet&=rs.SetFieldValue("user_passwd",des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_strUserPasswd,1));  
		bRet&=rs.SetFieldValue("resource_dir",m_data.m_strResourceDir); 
		bRet&=rs.SetFieldValue("server_desc",m_data.m_strServerDesc);  
		bRet&=rs.SetFieldValue("database_type",m_data.m_nDatabaseType);
		bRet&=rs.SetFieldValue("resource_path",m_data.m_strResourcePath);
		bRet&=rs.SetFieldValue("resource_thumb",m_data.m_bThumb);
		bRet&=rs.SetFieldValue("resource_thumb_name",m_data.m_strThumbName);
		bRet&=rs.SetFieldValue("server_port",(long)m_data.m_nServerPort);
		bRet&=rs.SetFieldValue("server_code",m_data.m_strServerCode);
		bRet&=rs.SetFieldValue("file_send_type",m_data.m_nFileSendType);
		bRet&=rs.SetFieldValue("file_server_name",m_data.m_strFileServerName);
		bRet&=rs.SetFieldValue("file_server_user_name",m_data.m_strFileServerUserName);
		bRet&=rs.SetFieldValue("file_server_password",des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_strFileServerPassWord,1));
		bRet&=rs.SetFieldValue("file_server_port",(long)m_data.m_nFileServerPort);		

		bRet&=rs.Update();	

		rs.Close();
	}

	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CDBServer::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;

	strSql.Format("select * from wdm_database where server_id='%s'",m_data.m_strServerID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CWDMDatabase* pDatabase=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pDatabase=new CWDMDatabase;
			pDatabase->LoadInfo(rs);
			pDatabase->AddTo(this);
            
			rs.MoveNext();
		}
		rs.Close();
	}
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CDBServer::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("server_id",m_data.m_strServerID);
	bRet&=rs.GetFieldValue("server_name",m_data.m_strServerName);
	bRet&=rs.GetFieldValue("user_name",m_data.m_strUserName);
	bRet&=rs.GetFieldValue("user_passwd",m_data.m_strUserPasswd);
    bRet&=rs.GetFieldValue("resource_dir",m_data.m_strResourceDir);
    bRet&=rs.GetFieldValue("server_desc",m_data.m_strServerDesc);
	bRet&=rs.GetFieldValue("database_type",m_data.m_nDatabaseType);
	bRet&=rs.GetFieldValue("resource_path",m_data.m_strResourcePath);
	bRet&=rs.GetFieldValue("resource_thumb",m_data.m_bThumb);
	bRet&=rs.GetFieldValue("resource_thumb_name",m_data.m_strThumbName);
	bRet&=rs.GetFieldValue("server_port",(long&)m_data.m_nServerPort);
	bRet&=rs.GetFieldValue("server_code",m_data.m_strServerCode);
	bRet&=rs.GetFieldValue("file_send_type",m_data.m_nFileSendType);
	bRet&=rs.GetFieldValue("file_server_name",m_data.m_strFileServerName);
	bRet&=rs.GetFieldValue("file_server_user_name",m_data.m_strFileServerUserName);
	bRet&=rs.GetFieldValue("file_server_password",m_data.m_strFileServerPassWord);
	bRet&=rs.GetFieldValue("file_server_port",(long&)m_data.m_nFileServerPort);		

	CDes des;
	m_data.m_strUserPasswd=des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_strUserPasswd,0);
	m_data.m_strFileServerPassWord=des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_strFileServerPassWord,0);

	m_strID=m_data.m_strServerID;
	m_strDesc=m_data.m_strServerName;
	
	return bRet;
}
BOOL CDBServer::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_server where server_id='%s'",m_data.m_strServerID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CDBServer::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CDBServer::PromptMenuPos()
{
	return 7;
}
void CDBServer::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除服务器");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}
void CDBServer::ShowInTreeView(CCustomTree *pTree,BOOL bChild/*=FALSE*/)
{
 	CBaseObject::ShowInTreeView(pTree,bChild);
// 	((CTreeCtrl *)pTree)->EnsureVisible(m_hItem);
}
void CDBServer::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
  	CString strText;
	CStringArray strList;

	if(bSelf)
	{
        strText=_T("服务器名,300;数据库类型,150;图片绝对路径,300;文件传送方式,100;图片存储网络映射目录,300;文件服务器名,100");
		pListCtrl->SetHeadings(strText);
		
		strList.Add(GetDesc());

		switch( m_data.m_nDatabaseType )
		{
		case 0:
			strList.Add(_T("SQL Server"));
			break;
		case 1:
			strList.Add(_T("Oracle"));
			break;
		case 2:
			strList.Add(_T("MySQL"));
			break;
		default:
			strList.Add(_T(""));
		}
		strList.Add(m_data.m_strResourcePath);
		strList.Add((m_data.m_nFileSendType == 0)?_T("网络映射目录"):_T("FTP"));
		strList.Add(m_data.m_strResourceDir);
		strList.Add(m_data.m_strFileServerName);

		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
	}
	else
	{
 		 strText=_T("序号,60;数据库名,200");
		 pListCtrl->SetHeadings(strText);
         
		 int i=0;
		 POSITION pos=GetFirstChildPosition();
		 while (pos)
		 {
			 CWDMDatabase* pDatabase=(CWDMDatabase*)GetNextChild(pos);
			 if(pDatabase!=NULL)
			 {
				 strList.RemoveAll();
				 
				 strText.Format("%d",++i);
				 strList.Add(strText);
				 
				 strList.Add(pDatabase->m_data.m_strDatabaseName);	
				 
				 pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pDatabase);
			 }

		 }
	
	}
}
TiXmlNode*  CDBServer::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_server"));

	pTemp->SetAttribute(_T("server_id"),m_data.m_strServerID);
	pTemp->SetAttribute(_T("server_name"),m_data.m_strServerName);
	pTemp->SetAttribute(_T("user_name"),m_data.m_strUserName);
	CDes des;
	pTemp->SetAttribute(_T("user_passwd"),des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_strUserPasswd,1));
    pTemp->SetAttribute(_T("server_desc"),m_data.m_strServerDesc);
    pTemp->SetAttribute(_T("resource_dir"),m_data.m_strResourceDir);
    pTemp->SetAttribute(_T("database_type"),m_data.m_nDatabaseType);
	pTemp->SetAttribute(_T("resource_path"),m_data.m_strResourcePath);
	pTemp->SetAttribute(_T("resource_thumb"),m_data.m_bThumb);
	pTemp->SetAttribute(_T("resource_thumb_name"),m_data.m_strThumbName);
	pTemp->SetAttribute(_T("server_port"),m_data.m_nServerPort);
	pTemp->SetAttribute(_T("server_code"),m_data.m_strServerCode);
	pTemp->SetAttribute(_T("file_send_type"),m_data.m_nFileSendType);
	pTemp->SetAttribute(_T("file_server_name"),m_data.m_strFileServerName);
	pTemp->SetAttribute(_T("file_server_user_name"),m_data.m_strFileServerUserName);
	pTemp->SetAttribute(_T("file_server_password"),des.DesCrypteString(_T("auqfvfhpgu"),m_data.m_strFileServerPassWord,1));
	pTemp->SetAttribute(_T("file_server_port"),m_data.m_nFileServerPort);		

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CDBServer::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CWDMDatabase* pDatabase=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_database"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("database_id"));
		if(!strID.IsEmpty())
		{
			pDatabase=(CWDMDatabase*)FindSubObject(OBJECT_TYPE_DATABASE,strID);
			if(pDatabase==NULL)
			{
				pDatabase=new CWDMDatabase;
				pDatabase->m_data.m_strDatabaseID=strID;
				pDatabase->AddTo(this);				
			}
			pDatabase->m_data.m_strDatabaseName=pElement->Attribute(_T("database_name"));
			pDatabase->m_data.m_strServerID=pElement->Attribute(_T("server_id"));
			pDatabase->SetID(strID,pDatabase->m_data.m_strDatabaseName);
			pDatabase->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

			pDatabase->StreamIn(pElement,bChild,pProgressBar);
		}		
	}
	return TRUE;
}


CFileSend* CDBServer::GetFileSend()
{	
	CFileSend* pFileSend = NULL;
	// 创建文件服务器对象
	if( m_data.m_nFileSendType == 0 )
	{
		pFileSend = new CNetShareFileSend(m_data.m_strFileServerName,
			m_data.m_strFileServerUserName,
			m_data.m_strFileServerPassWord);
	}
	else
	{
		pFileSend = new CFtpFileSend(m_data.m_strFileServerName,
			m_data.m_strFileServerUserName,
			m_data.m_strFileServerPassWord,
			m_data.m_nFileServerPort);
	}
	return pFileSend;
}