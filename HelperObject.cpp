// HelperObject.cpp: implementation of the CHelperObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WebDataMining.h"
#include "HelperObject.h"
#include "Schedule.h"
#include "DBServer.h"
#include "Column.h"
#include "des.h"
#include "Rule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct HELPEROBJECT_TAG
{
	LPCSTR strID;		//
	LPCSTR strDesc;		//
	int nImg;
	int nSelImg;
	UINT nPromptMenuID;	//
	int nPromptMenuPos;	//
}
	g_HelpObjects[] =
{
	/* 0 - HELPER_SOURCE	*/	{ NULL_ID,"采集源", 0,1, IDR_TREE_MENU, 0 },
	/* 1 - HELPER_RULE  	*/	{ NULL_ID, "采集规则", 10,11, IDR_TREE_MENU, 0 },
	/* 2 - HELPER_SCHEDULE  */  { NULL_ID,"调度计划",16,17,IDR_TREE_MENU,0 },
	/* 3 - HELPER_DBSERVER  */  { NULL_ID,"数据库服务器",28,29,IDR_TREE_MENU,0 }
};


CHelperObject::CHelperObject(int nClass) : CBaseObject(OBJECT_TYPE_HELPER)
{
	m_bCanDelete=FALSE;
	m_nClass=nClass;
	m_nImg=g_HelpObjects[m_nClass].nImg;
	m_nSelImg=g_HelpObjects[m_nClass].nSelImg;
	m_strID=g_HelpObjects[m_nClass].strID;
	m_strDesc=g_HelpObjects[m_nClass].strDesc;
}

BOOL CHelperObject::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

/*
	HELPER_SOURCE=0,         //采集源
	HELPER_RULE              //采集规则
	HELPER_SCHEDULE          //调度计划
	HELPER_DBSERVER          //数据库服务器
*/
	int nObType,nHelperClass;
	nObType=pOB->GetType();
	if(nObType == OBJECT_TYPE_HELPER)
		nHelperClass=((CHelperObject *)pOB)->m_nClass;

	switch(m_nClass)
	{
	case HELPER_SOURCE:
		if(nObType == OBJECT_TYPE_COLUMN)	//采集源
			bAccept=TRUE;
		break;
	case HELPER_RULE:
		if(nObType == OBJECT_TYPE_RULE)	    //采集规则
			bAccept=TRUE;
		break;
	case HELPER_SCHEDULE:                   //调度计划
		if(nObType == OBJECT_TYPE_SCHEDULE)
			bAccept=TRUE;
		break;
	case HELPER_DBSERVER:                   //数据库服务器
		if(nObType==OBJECT_TYPE_DBSERVER)
			bAccept=TRUE;
		break;
	}

	return bAccept;
}

CHelperObject::~CHelperObject()
{

}

UINT CHelperObject::PromptMenuID()
{
	return g_HelpObjects[m_nClass].nPromptMenuID;
}

int CHelperObject::PromptMenuPos()		
{
	return g_HelpObjects[m_nClass].nPromptMenuPos;
}

void CHelperObject::UpdateMenu(CMenu *pMenu)
{	
	UINT newID=RetrieveMenuItemID(pMenu,"新建");
	UINT delID=RetrieveMenuItemID(pMenu,"删除");
    UINT delTopics=RetrieveMenuItemID(pMenu,"删除文章");

	ASSERT(newID!=-1 && delID!=-1 && delTopics!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(delTopics,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));

	switch(GetHelpType())
	{
	case HELPER_SOURCE:
		pMenu->ModifyMenu(newID,MF_BYCOMMAND,newID,"新建栏目");
		pMenu->EnableMenuItem(newID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
		break;
	case HELPER_RULE:
		pMenu->ModifyMenu(newID,MF_BYCOMMAND,newID,"新建规则");
		pMenu->EnableMenuItem(newID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
		//Delete the invalide menu item
		pMenu->DeleteMenu(10,MF_BYPOSITION);
		pMenu->DeleteMenu(9,MF_BYPOSITION);
		pMenu->DeleteMenu(8,MF_BYPOSITION);
		break;
	case HELPER_SCHEDULE:
		//Delete the invalide menu item
		pMenu->DeleteMenu(10,MF_BYPOSITION);
		pMenu->DeleteMenu(9,MF_BYPOSITION);
		pMenu->DeleteMenu(8,MF_BYPOSITION);
		pMenu->DeleteMenu(0,MF_BYPOSITION);
		pMenu->DeleteMenu(0,MF_BYPOSITION);
		break;
	case HELPER_DBSERVER:
		pMenu->ModifyMenu(newID,MF_BYCOMMAND,newID,"新建服务器");
		pMenu->EnableMenuItem(newID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
		pMenu->DeleteMenu(10,MF_BYPOSITION);
		pMenu->DeleteMenu(9,MF_BYPOSITION);
		pMenu->DeleteMenu(8,MF_BYPOSITION);
		break;
	}	
	CBaseObject::UpdateMenu(pMenu);	
}

int CHelperObject::GetHelpType()
{
     return m_nClass;
}
void CHelperObject::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);

	CString strText;
	CStringArray strList;

	if(bSelf)
	{
        pListCtrl->SetHeadings(_T("名称,150"));
		pListCtrl->SetItemData(pListCtrl->AddItem(GetDesc()),(DWORD)this);
	}
	else
	{
		switch(GetHelpType())
		{
		case HELPER_SOURCE:
		    strText=_T("采集源名称,300;调度计划数,150;数据库名称,150");
			break;
		case HELPER_RULE:
			strText=_T("规则名称,300");
			break;
		case HELPER_SCHEDULE:
			strText=_T("调度计划,300;下次运行时间,200;关联对象,300");
			break;
		case HELPER_DBSERVER:
			strText=_T("服务器名,300;数据库类型,150;图片相对路径,300;文件传送方式,100;图片存储网络映射目录,300;文件服务器名,100");
			break;
		}	
        pListCtrl->SetHeadings(_T("序号,60;")+strText);
	
		int i=0;
		POSITION pos=GetFirstChildPosition();
		while (pos)
		{
			CBaseObject * pOb=GetNextChild(pos);
			if(pOb!=NULL)
			{
				strList.RemoveAll();
					
				strText.Format("%d",++i);
				strList.Add(strText);
	
				strList.Add(pOb->GetDesc());
					
				switch(GetHelpType())
				{
				case HELPER_SOURCE:
					{
						CColumn* pColumn=(CColumn*)pOb;
						CBaseObject *pTemp=NULL;
						int i=0;
						POSITION pos =theApp.GetScheduleTree()->m_objectHelper.GetFirstChildPosition();
						while (pos)
						{
							CSchedule* pSchedule=(CSchedule*)theApp.GetScheduleTree()->m_objectHelper.GetNextChild(pos);
							if(pSchedule!=NULL)
							{
								if(pSchedule->m_data.m_strObjectID==pColumn->GetID())
									i++;
							}        
						}
						strText.Format("%d",i);
						strList.Add(strText);						
						
						CBaseObject* pDB=theApp.GetDBServerTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_DATABASE,pColumn->m_data.m_strDatabaseID,TRUE);
						if(pDB!=NULL)
							strList.Add(pDB->GetDesc());
						else
							strList.Add(_T("数据库没有配置"));
						
					}
					break;
				case HELPER_SCHEDULE:
					{
						CSchedule* pSchedule=(CSchedule*)pOb;
						strList.Add(pSchedule->GetNextRunTime().Format("%Y-%m-%d %H:%M:%S"));
						CBaseObject* pOb=pSchedule->GetScheduledObject();
                        strList.Add(pOb!=NULL?pOb->GetDesc():_T(""));
					}			
					break;
				case HELPER_DBSERVER:
					{
						CDBServer* pServer=(CDBServer*)pOb;
						switch( pServer->m_data.m_nDatabaseType )
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
						strList.Add(pServer->m_data.m_strResourcePath);
						strList.Add((pServer->m_data.m_nFileSendType == 0)?_T("网络映射目录"):_T("FTP"));
						strList.Add(pServer->m_data.m_strResourceDir);
						strList.Add(pServer->m_data.m_strFileServerName);
						
					}				
					break;
				}
				pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pOb);
			
			}
		} 
	}
}

TiXmlNode*  CHelperObject::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=NULL;
	
	pTemp=new TiXmlElement(_T("wdm_root"));
	pTemp->SetAttribute(_T("name"),GetDesc());
	pTemp->SetAttribute(_T("type"),GetHelpType());
	
	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}

BOOL CHelperObject::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	int nType=-1;
	TiXmlHandle handle(pNode);
    handle.ToElement()->QueryIntAttribute(_T("type"),&nType);

	switch(nType)
	{
	case HELPER_SOURCE:
		{
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
		}
		break;
	case HELPER_RULE:
		{
			CRule* pRule=NULL;
			for(pElement=pNode->FirstChildElement(_T("wdm_rule"));pElement;pElement=pElement->NextSiblingElement())
			{
				strID=pElement->Attribute(_T("rule_id"));
				if(!strID.IsEmpty())
				{
					pRule=(CRule*)FindSubObject(OBJECT_TYPE_RULE,strID);
					if(pRule==NULL)
					{
						pRule=new CRule;
						pRule->m_data.m_strRuleID=strID;						
						pRule->AddTo(this);						
					}
					pRule->m_data.m_strRuleName=pElement->Attribute(_T("rule_name"));
					pElement->QueryIntAttribute(_T("rule_encoding"),&pRule->m_data.m_nRuleEncoding);

					pRule->SetID(strID,pRule->m_data.m_strRuleName);
					pRule->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

					pRule->StreamIn(pElement,bChild,pProgressBar);
				}
			}
		}
		break;
	case HELPER_SCHEDULE:
		{
			CSchedule* pSchedule=NULL;
			for(pElement=pNode->FirstChildElement(_T("wdm_schedule"));pElement;pElement=pElement->NextSiblingElement())
			{
				strID=pElement->Attribute(_T("schedule_id"));
				if(!strID.IsEmpty())
				{
					pSchedule=(CSchedule*)FindSubObject(OBJECT_TYPE_SCHEDULE,strID);
					if(pSchedule==NULL)
					{
						pSchedule=new CSchedule;
						pSchedule->m_data.m_strScheduleID=strID;
						pSchedule->AddTo(this);					
					}
					pSchedule->m_data.m_strScheduleName=pElement->Attribute(_T("schedule_name"));
					pSchedule->m_data.m_strObjectID=pElement->Attribute(_T("object_id"));
					pElement->QueryIntAttribute(_T("object_type"),&(pSchedule->m_data.m_nObjectType));
					pElement->QueryIntAttribute(_T("enabled"),&(pSchedule->m_data.m_nEnabled));
					pElement->QueryIntAttribute(_T("freq_type"),&(pSchedule->m_data.m_nFreqType));
					pElement->QueryIntAttribute(_T("freq_interval"),&(pSchedule->m_data.m_nFreqInterval));
					pElement->QueryIntAttribute(_T("freq_subday_type"),&(pSchedule->m_data.m_nFreqSubdayInterval));
					pElement->QueryIntAttribute(_T("freq_relative_interval"),&(pSchedule->m_data.m_nFreqRelativeInterval));
					pElement->QueryIntAttribute(_T("freq_recurrence_factor"),&(pSchedule->m_data.m_nFreqRecurrenceFactor));
					pElement->QueryIntAttribute(_T("freq_subday_interval"),&(pSchedule->m_data.m_nFreqSubdayInterval));
					pElement->QueryIntAttribute(_T("active_start_date"),&(pSchedule->m_data.m_nActiveStartDate));
					pElement->QueryIntAttribute(_T("active_end_date"),&(pSchedule->m_data.m_nActiveEndDate));
					pElement->QueryIntAttribute(_T("active_start_time"),&(pSchedule->m_data.m_nActiveStartTime));
					pElement->QueryIntAttribute(_T("active_end_time"),&(pSchedule->m_data.m_nActiveEndTime));
					pElement->QueryIntAttribute(_T("next_run_date"),&(pSchedule->m_data.m_nNextRunDate));
					pElement->QueryIntAttribute(_T("next_run_time"),&(pSchedule->m_data.m_nNextRunTime));
					pSchedule->m_data.m_timeCreated.ParseDateTime(pElement->Attribute(_T("created_date")));

					pSchedule->SetID(strID,pSchedule->m_data.m_strScheduleName);					
					pSchedule->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

					pSchedule->StreamIn(pElement,bChild,pProgressBar);
				}
			}
		}
		break;
	case HELPER_DBSERVER:
		{
			CDBServer* pServer=NULL;
			for(pElement=pNode->FirstChildElement(_T("wdm_server"));pElement;pElement=pElement->NextSiblingElement())
			{
				strID=pElement->Attribute(_T("server_id"));
				if(!strID.IsEmpty())
				{
					pServer=(CDBServer*)FindSubObject(OBJECT_TYPE_DBSERVER,strID);
					if(pServer==NULL)
					{
						pServer=new CDBServer;
						pServer->m_data.m_strServerID=strID;
						pServer->AddTo(this);					
					}
					pServer->m_data.m_strServerName=pElement->Attribute(_T("server_name"));
					pServer->m_data.m_strUserName=pElement->Attribute(_T("user_name"));
					pServer->m_data.m_strUserPasswd=pElement->Attribute(_T("user_passwd"));
					pServer->m_data.m_strResourceDir=pElement->Attribute(_T("resource_dir"));
					pServer->m_data.m_strServerDesc=pElement->Attribute(_T("server_desc"));
					pElement->QueryIntAttribute(_T("database_type"),&(pServer->m_data.m_nDatabaseType));
					pServer->m_data.m_strResourcePath=pElement->Attribute(_T("resource_path"));
					pElement->QueryIntAttribute(_T("resource_thumb"),&(pServer->m_data.m_bThumb));
					pServer->m_data.m_strThumbName=pElement->Attribute(_T("resource_thumb_name"));
					pElement->QueryIntAttribute(_T("server_port"),&(pServer->m_data.m_nServerPort));
					pServer->m_data.m_strServerCode=pElement->Attribute(_T("server_code"));
					pElement->QueryIntAttribute(_T("file_send_type"),&(pServer->m_data.m_nFileSendType));
					pServer->m_data.m_strFileServerName=pElement->Attribute(_T("file_server_name"));
					pServer->m_data.m_strFileServerUserName=pElement->Attribute(_T("file_server_user_name"));
					pServer->m_data.m_strFileServerPassWord=pElement->Attribute(_T("file_server_password"));
					pElement->QueryIntAttribute(_T("file_server_port"),&(pServer->m_data.m_nFileServerPort));

					pServer->SetID(strID,pServer->m_data.m_strServerName);
					CDes des;
					pServer->m_data.m_strUserPasswd=des.DesCrypteString(_T("auqfvfhpgu"),pServer->m_data.m_strUserPasswd,0);
					pServer->m_data.m_strFileServerPassWord=des.DesCrypteString(_T("auqfvfhpgu"),pServer->m_data.m_strFileServerPassWord,0);
					pServer->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
					
					pServer->StreamIn(pElement,bChild,pProgressBar);
				}
			}
		}
		break;
	default:
		ASSERT(FALSE);
	}
	return TRUE;
}

TiXmlNode* CHelperObject::Export(CString& strDir,TiXmlNode* pNode,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{		
	CBaseObject::Export(strDir,pNode,bChild,pProgressBar);
	return pNode;
}