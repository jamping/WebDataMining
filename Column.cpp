// Column.cpp: implementation of the CColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "Column.h"
#include "InfoDlg.h"
#include "Entry.h"
#include "Schedule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColumn::CColumn() :CBaseObject(OBJECT_TYPE_COLUMN)
{
	SetImg(2,3);

}

CColumn::~CColumn()
{

}
void CColumn::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strColumnID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CColumn::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_COLUMN:
		bAccept=TRUE;        //子栏目
		break;
	case OBJECT_TYPE_ENTRY:	//可以挂接条目
		bAccept=TRUE;
		break;
	}

	return bAccept;
}
void CColumn::DoJoin(CBaseObject *pParent)
{
	m_data.m_strParentID=pParent->GetID();	
	CBaseObject::DoJoin(this);
}		
void CColumn::DoLeave(CBaseObject *pParent)
{
	m_data.m_strParentID.Empty();
	CBaseObject::DoLeave(this);
}
UINT CColumn::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CInfoDlg dlg(bReadOnly);

	dlg.m_strName=m_data.m_strColumnName;
	nRet=dlg.DoModal();
	if(nRet== IDOK)
	{	
		if(!IsNew())
		{
            if(!CCommonFunction::IsSameString(m_data.m_strColumnName,dlg.m_strName))
				SetModify();
			else
				return nRet;
		}
		m_data.m_strColumnName=dlg.m_strName;
		ChangeDesc(m_data.m_strColumnName);			
	}	

	return nRet;
}
BOOL CColumn::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_column where column_id='%s'",m_data.m_strColumnID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("column_id",m_data.m_strColumnID);
		}
		else
			rs.Edit();
	
		bRet&=rs.SetFieldValue("database_id",m_data.m_strDatabaseID);
		bRet&=rs.SetFieldValue("parent_id",m_data.m_strParentID);
		bRet&=rs.SetFieldValue("column_name",m_data.m_strColumnName.Left(20));       
		bRet&=rs.Update();	

		rs.Close();
	} 
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CColumn::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;
    
	//Load sub columns
	strSql.Format("select *from wdm_column where parent_id='%s'",m_data.m_strColumnID);
    bRet=rs.Open(strSql,CADORecordset::openQuery);
	CColumn* pColumn=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pColumn=new CColumn;
			pColumn->LoadInfo(rs);
			pColumn->AddTo(this);

			rs.MoveNext();
		}	
	    rs.Close();
	}
	//Load entrys
	strSql.Format("select * from wdm_entry where column_id='%s'",m_data.m_strColumnID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CEntry* pEntry=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pEntry=new CEntry;
			pEntry->LoadInfo(rs);
			pEntry->AddTo(this);

			rs.MoveNext();
		}	
	    rs.Close();
	}
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CColumn::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("column_id",m_data.m_strColumnID);
	bRet&=rs.GetFieldValue("database_id",m_data.m_strDatabaseID);
	bRet&=rs.GetFieldValue("parent_id",m_data.m_strParentID);
	bRet&=rs.GetFieldValue("column_name",m_data.m_strColumnName);

	m_strID=m_data.m_strColumnID;
	m_strDesc=m_data.m_strColumnName;
	
	return bRet;
}
BOOL CColumn::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_column where column_id='%s'",m_data.m_strColumnID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CColumn::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CColumn::PromptMenuPos()
{
	return 1;
}
void CColumn::UpdateMenu(CMenu *pMenu)
{
	UINT newSubID=RetrieveMenuItemID(pMenu,"新建子栏目");
	UINT newEntryID=RetrieveMenuItemID(pMenu,"新建条目");
	UINT delID=RetrieveMenuItemID(pMenu,"删除栏目");
    UINT delTopics=RetrieveMenuItemID(pMenu,"删除文章");
    UINT delSetDBserver=RetrieveMenuItemID(pMenu,"指定数据库");
//    UINT nSetChannelID=RetrieveMenuItemID(pMenu,"设置频道ID");

	ASSERT(newSubID!=-1 && newEntryID!=-1 &&delID!=-1 
		&& delTopics!=-1 && delSetDBserver!=-1 );

	int nLevel=theApp.GetUserLevel();
	pMenu->EnableMenuItem(newSubID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(newEntryID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(delTopics,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
//	pMenu->EnableMenuItem(delSetDBserver,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
	pMenu->ModifyMenu(delSetDBserver,MF_BYCOMMAND,delSetDBserver,m_data.m_strDatabaseID.IsEmpty()?_T("指定数据库"):_T("取消数据库"));
	// 根节点ID默认为“0”
	if(!m_data.m_strParentID.CompareNoCase(CString(NULL_ID)) == 0)
	{
	    pMenu->DeleteMenu(delSetDBserver,MF_BYCOMMAND);	
//		pMenu->DeleteMenu(nSetChannelID,MF_BYCOMMAND);
		pMenu->DeleteMenu(newSubID,MF_BYCOMMAND);
	}
 	else    //频道
 	{	
         pMenu->DeleteMenu(newEntryID,MF_BYCOMMAND);
 	}
}
void CColumn::ShowInTreeView(CCustomTree *pTree,BOOL bChild/*=FALSE*/)
{
	CBaseObject::ShowInTreeView(pTree,bChild);
	((CTreeCtrl *)pTree)->EnsureVisible(m_hItem);
}
void CColumn::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);

    CString strText;
	CStringArray strList;

	if(bSelf)
	{
		strText=_T("栏目名称,200;调度计划数,150");
		if(m_data.m_strParentID.CompareNoCase((CString)NULL_ID)==0/*m_data.m_strParentID.IsEmpty()*/)
			strText+=_T(";数据库名称,150");
    	pListCtrl->SetHeadings(strText);
        
		strList.Add(m_data.m_strColumnName);

		CBaseObject *pOb=NULL;
		int i=0;
		POSITION pos =theApp.GetScheduleTree()->m_objectHelper.GetFirstChildPosition();
		while (pos)
		{
			pOb=theApp.GetScheduleTree()->m_objectHelper.GetNextChild(pos);
			if(pOb!=NULL)
			{
				CSchedule* pSchedule=(CSchedule*)pOb;
				if(pSchedule->m_data.m_strObjectID==GetID())
				    i++;
			}        
		}
		strText.Format("%d",i);
		strList.Add(strText);

		if(m_data.m_strParentID.CompareNoCase((CString)NULL_ID)==0/*m_data.m_strParentID.IsEmpty()*/)
		{
			CBaseObject* pDB=theApp.GetDBServerTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_DATABASE,m_data.m_strDatabaseID,TRUE);
			if(pDB!=NULL)
				strList.Add(pDB->GetDesc());
			else
                strList.Add(_T("数据库没有配置"));
		}     
		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
	}
	else
	{
		 // top column, child is sub column
         if(GetChildType()==OBJECT_TYPE_COLUMN)
		 {
			 strText=_T("栏目名称,200;调度计划数,150");
			 
			 pListCtrl->SetHeadings(strText);
			 
			 POSITION pos=GetFirstChildPosition();
			 while (pos)
			 {
				 CBaseObject* pChildColumn=GetNextChild(pos);
                 if(pChildColumn!=NULL)
				 {		
					 strList.RemoveAll();
					 strList.Add(pChildColumn->GetDesc());
					 
					 CBaseObject *pOb=NULL;
					 int i=0;
					 POSITION pos1 =theApp.GetScheduleTree()->m_objectHelper.GetFirstChildPosition();
					 while (pos1)
					 {
						 pOb=theApp.GetScheduleTree()->m_objectHelper.GetNextChild(pos1);
						 if(pOb!=NULL)
						 {
							 CSchedule* pSchedule=(CSchedule*)pOb;
							 if(pSchedule->m_data.m_strObjectID==pChildColumn->GetID())
								 i++;
						 }        
					 }
					 strText.Format("%d",i);
					 strList.Add(strText);
					 
					 pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pChildColumn);					 
				 }
			 }

		 }
		 if(GetChildType()==OBJECT_TYPE_ENTRY)  //child is entry
		 {
			  strText=_T("条目名称,100;网站名称,100;下载地址,150;文章列表匹配规则,200;提取规则名,150;文章最大数,100;重试次数,100;自动发布,100;服务器栏目ID,150");					  
			  pListCtrl->SetHeadings(strText);
             
			  POSITION pos=GetFirstChildPosition();
			  while (pos)
			  {
				  CEntry* pEntry=(CEntry*)GetNextChild(pos);
				  if(pEntry!=NULL)
				  {
                      strList.RemoveAll();
					  strList.Add(pEntry->m_data.m_strEntryName);
					  strList.Add(pEntry->m_data.m_strWebSiteName);
					  strList.Add(pEntry->m_data.m_strURL);
					  strList.Add(pEntry->m_data.m_strMatchRule);

					  CRule* pRule=pEntry->GetRule();					  
					  strList.Add(pRule!=NULL?pRule->GetDesc():_T(""));

					  strText.Format("%d",pEntry->m_data.m_nTopicMax);
					  strList.Add(strText);
					  strText.Format("%d",pEntry->m_data.m_nTryTimes);
					  strList.Add(strText);
					  strList.Add(pEntry->m_data.m_bAutoPublish?_T("是"):_T("否"));
					  strList.Add(pEntry->m_data.m_strServerColumnID);
					  
					  pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pEntry);
				  }
			  }
		 }
	}
}

TiXmlNode*  CColumn::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_column"));

	pTemp->SetAttribute(_T("column_id"),m_data.m_strColumnID);
	pTemp->SetAttribute(_T("database_id"),m_data.m_strDatabaseID);
	pTemp->SetAttribute(_T("parent_id"),m_data.m_strParentID);
	pTemp->SetAttribute(_T("column_name"),m_data.m_strColumnName);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CColumn::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CColumn* pColumn=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_column"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("column_id"));
		if(!strID.IsEmpty())
		{
			pColumn=(CColumn*)FindSubObject(OBJECT_TYPE_COLUMN,strID);
			if(pColumn==NULL)
			{
				pColumn=new CColumn;
				pColumn->m_data.m_strColumnID=strID;
                pColumn->AddTo(this);
			}			
			pColumn->m_data.m_strColumnName=pElement->Attribute(_T("column_name"));
			pColumn->m_data.m_strDatabaseID=pElement->Attribute(_T("database_id"));
			pColumn->m_data.m_strParentID=pElement->Attribute(_T("parent_id"));
			
			pColumn->SetID(strID,pColumn->m_data.m_strColumnName);
			pColumn->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
			
			pColumn->StreamIn(pElement,bChild,pProgressBar);
		}
		
	}
	for(pElement=pNode->FirstChildElement(_T("wdm_entry"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("entry_id"));
		
		if(!strID.IsEmpty())
		{
			CEntry* pEntry=(CEntry*)FindSubObject(OBJECT_TYPE_ENTRY,strID);
			if(pEntry==NULL)
			{
				pEntry=new CEntry;
				pEntry->m_data.m_strEntryID=strID;
				pEntry->AddTo(this);
			}			
			pEntry->m_data.m_strColumnID=pElement->Attribute(_T("column_id"));
			pEntry->m_data.m_strServerColumnID=pElement->Attribute(_T("server_column_id"));
			pEntry->m_data.m_strRuleID=pElement->Attribute(_T("rule_id"));
			pEntry->m_data.m_strEntryName=pElement->Attribute(_T("entry_name"));
			pEntry->m_data.m_strWebSiteName=pElement->Attribute(_T("website_name"));			
			pEntry->m_data.m_strURL=pElement->Attribute(_T("url"));
			pEntry->m_data.m_strMatchRule=pElement->Attribute(_T("match_rule"));
			pElement->QueryIntAttribute(_T("topic_max"),&pEntry->m_data.m_nTopicMax);
			pElement->QueryIntAttribute(_T("try_times"),&pEntry->m_data.m_nTryTimes);
			pElement->QueryIntAttribute(_T("is_auto_publish"),&(int)pEntry->m_data.m_bAutoPublish);
			pEntry->m_data.m_lastDownloadTime.ParseDateTime(pElement->Attribute(_T("last_download_time")));		
			
			pEntry->SetID(strID,pEntry->m_data.m_strEntryName);
			pEntry->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
			
			pEntry->StreamIn(pElement,bChild,pProgressBar);
		}
	}
	return TRUE;
}

TiXmlNode* CColumn::Export(CString& strDir,TiXmlNode* pNode,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	//Create the directory
	//Top column	
	if(GetParent()->GetParent()==NULL)
	{
		CString strTempDir;
		strTempDir.Format("%s\\%s",strDir,GetID());
	    strDir=strTempDir;

		CCommonFunction::CreateDir(strDir+_T("\\Articles"));
		CCommonFunction::CreateDir(strDir+_T("\\Index"));
		
		TiXmlElement *pRoot=new TiXmlElement(_T("Root"));
		pNode=pNode->LinkEndChild(pRoot);
	}
	TiXmlElement * pTemp=new TiXmlElement(_T("Class"));

	pTemp->SetAttribute(_T("id"),m_data.m_strColumnID);
	pTemp->SetAttribute(_T("name"),m_data.m_strColumnName);

	pNode->LinkEndChild(pTemp);

	CBaseObject::Export(strDir,pTemp,bChild,pProgressBar);

	return pTemp;
}

void CColumn::StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
// 	CString str;
// 	if(GetParent()->GetParent()==NULL)
// 		str=_T("频道: ");
// 	else
// 		str=_T("大栏目: ");
// 
// 	stream <<str<<m_data.m_strColumnName<<endl;
//     stream <<endl;

	CBaseObject::StreamOutTxt(stream,bChild,pProgressBar);
}