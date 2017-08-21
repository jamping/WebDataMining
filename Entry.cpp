// Entry.cpp: implementation of the CEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "Column.h"
#include "Entry.h"
#include "EntryDlg.h"
#include "Topic.h"
#include "schedule.h"
#include "workentry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntry::CEntry() :CBaseObject(OBJECT_TYPE_ENTRY)
{
	SetImg(4,5);
}

CEntry::~CEntry()
{

}
void CEntry::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strEntryID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CEntry::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_TOPIC:	//可以挂接文章
		bAccept=TRUE;
		break;
// 	case OBJECT_TYPE_SCHEDULE://调度计划
// 		bAccept=TRUE;
// 		break;
	}

	return bAccept;
}
void CEntry::DoJoin(CBaseObject *pParent)
{
	m_data.m_strColumnID=pParent->GetID();
	
	CBaseObject::DoJoin(this);
}		
void CEntry::DoLeave(CBaseObject *pParent)
{
	m_data.m_strColumnID.Empty();

	CBaseObject::DoLeave(this);
}
UINT CEntry::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
    CEntryDlg dlg(bReadOnly);

	dlg.SetValue(m_data);
	nRet=dlg.DoModal();

	if(nRet== IDOK )
	{
		if(dlg.GetValue(m_data))
		{
     		SetModify();
			m_strDesc=m_data.m_strEntryName;
			if(!m_data.m_strWebSiteName.IsEmpty())
		        m_strDesc+="-"+m_data.m_strWebSiteName;
		}
	}
	return nRet;
}
BOOL CEntry::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_entry where entry_id='%s'",m_data.m_strEntryID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("entry_id",m_data.m_strEntryID);
		}
		else
			rs.Edit();

		bRet&=rs.SetFieldValue("column_id",m_data.m_strColumnID);  
		bRet&=rs.SetFieldValue("server_column_id",m_data.m_strServerColumnID); 
//		bRet&=rs.SetFieldValue("server_channel_id",m_data.m_strServerChannelID); 
		bRet&=rs.SetFieldValue("rule_id",m_data.m_strRuleID);       
		bRet&=rs.SetFieldValue("entry_name",m_data.m_strEntryName);       
		bRet&=rs.SetFieldValue("website_name",m_data.m_strWebSiteName);       
		bRet&=rs.SetFieldValue("url",m_data.m_strURL);       
		bRet&=rs.SetFieldValue("match_rule",m_data.m_strMatchRule);       
		bRet&=rs.SetFieldValue("topic_max",m_data.m_nTopicMax);       
		bRet&=rs.SetFieldValue("try_times",m_data.m_nTryTimes);       
		bRet&=rs.SetFieldValue("is_auto_publish",m_data.m_bAutoPublish); 
		bRet&=rs.SetFieldValue("last_download_time",m_data.m_lastDownloadTime);
		bRet&=rs.Update();

		rs.Close();
	}
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CEntry::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
    BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;
    //Load topic
// 	COleDateTime nextDay;
// 	nextDay=theApp.GetLoadTopicDatetime()+COleDateTimeSpan(1,0,0,0);
// 
//     if(theApp.GetLocalDatabaseType()==0)
// 	{
// 		strSql.Format("select * from wdm_topic where entry_id='%s' and download_date>='%s' and download_date<'%s' order by [time] desc",
// 			m_data.m_strEntryID,theApp.GetLoadTopicDatetime().Format("%Y-%m-%d"),nextDay.Format("%Y-%m-%d"));
// 	}
// 	else
// 	{
// 	    strSql.Format("select * from wdm_topic where entry_id='%s' and download_date >=datevalue('%s') and download_date<datevalue('%s') order by [time] desc",
// 		    m_data.m_strEntryID,theApp.GetLoadTopicDatetime().Format("%Y-%m-%d"),nextDay.Format("%Y-%m-%d"));
// 	}
	if(theApp.GetLocalDatabaseType()==0)
	{
		strSql.Format("select * from wdm_topic where entry_id='%s' and download_date>='%s' order by [time] desc",
			m_data.m_strEntryID,theApp.GetLoadTopicDatetime().Format("%Y-%m-%d"));
	}
	else
	{
	    strSql.Format("select * from wdm_topic where entry_id='%s' and download_date >=datevalue('%s')  order by [time] desc",
		    m_data.m_strEntryID,theApp.GetLoadTopicDatetime().Format("%Y-%m-%d"));
	}
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CTopic* pTopic=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pTopic=new CTopic;
			pTopic->LoadInfo(rs);
			// check the topic status
			if(pTopic->m_data.m_bIsSendto==FALSE)
				theApp.GetDownloadTopicsList().Add(pTopic);

			pTopic->AddTo(this);

			rs.MoveNext();
		}
		rs.Close();
	}

	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CEntry::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("entry_id",m_data.m_strEntryID);
	bRet&=rs.GetFieldValue("column_id",m_data.m_strColumnID);
	bRet&=rs.GetFieldValue("server_column_id",m_data.m_strServerColumnID);
//	bRet&=rs.GetFieldValue("server_channel_id",m_data.m_strServerChannelID);
	bRet&=rs.GetFieldValue("rule_id",m_data.m_strRuleID);
	bRet&=rs.GetFieldValue("entry_name",m_data.m_strEntryName);
	bRet&=rs.GetFieldValue("website_name",m_data.m_strWebSiteName);
	bRet&=rs.GetFieldValue("url",m_data.m_strURL);
	bRet&=rs.GetFieldValue("match_rule",m_data.m_strMatchRule);
	bRet&=rs.GetFieldValue("topic_max",m_data.m_nTopicMax);
	bRet&=rs.GetFieldValue("try_times",m_data.m_nTryTimes);
	bRet&=rs.GetFieldValue("is_auto_publish",m_data.m_bAutoPublish);
	bRet&=rs.GetFieldValue("last_download_time",m_data.m_lastDownloadTime);
   
	m_strID=m_data.m_strEntryID;
	m_strDesc=m_data.m_strEntryName+"-"+m_data.m_strWebSiteName;

	return bRet;
}
BOOL CEntry::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_entry where entry_id='%s'",m_data.m_strEntryID);
		bRet=db.Execute(strSql);
	}
    
	return bRet;
}	

UINT CEntry::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CEntry::PromptMenuPos()
{
	return 2;
}
void CEntry::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除条目");
    UINT delTopics=RetrieveMenuItemID(pMenu,"删除文章");
    
	ASSERT(delID!=-1 && delTopics!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(delTopics,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}

CRule* CEntry::GetRule()
{
	CRule* pRule=(CRule*)theApp.GetRuleTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_RULE,m_data.m_strRuleID);
	
	return pRule;
}
BOOL CEntry::IsValid()
{	
	if(m_data.m_strURL.IsEmpty())
		return FALSE;
	if(m_data.m_strRuleID.IsEmpty())
		return TRUE;
	if(m_data.m_strMatchRule.IsEmpty())
		return FALSE;

	return TRUE;
}
void CEntry::DownloadObject(BOOL bAuto /* =FALSE  */)
{
	if(!bAuto)
		CLogs::WriteLogs("下载："+GetDesc(),CLogs::log_download);

	thread_pool::instance().queue_request(new CWorkEntry(this,GetRule()));
  	//request_ptr request = request_ptr( std::auto_ptr<CWorkEntry>(new CWorkEntry(this,GetRule())));
  	//thread_pool::instance().add_request(request);

}
void CEntry::DelSubObject(BOOL bDelFromDB /*=TRUE*/ )
{
	if(bDelFromDB)
        DeleteFromDB(*theApp.GetDB(),FALSE);
	
	theApp.GetSourceTree()->DeleteAllChildItem(GetRefItem());
}
void CEntry::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
    
	CString strText;
	CStringArray strList;

	if(bSelf)
	{
		strText=_T("条目名称,100;网站名称,100;下载地址,150;文章列表匹配规则,200;提取规则名,150;文章最大数,100;重试次数,100;自动发布,50;服务器栏目ID,150");					  
		pListCtrl->SetHeadings(strText);		

		strList.Add(m_data.m_strEntryName);
		strList.Add(m_data.m_strWebSiteName);
		strList.Add(m_data.m_strURL);
		strList.Add(m_data.m_strMatchRule);
		
		CRule* pRule=GetRule();					  
		strList.Add(pRule!=NULL?pRule->GetDesc():_T(""));
		
		strText.Format("%d",m_data.m_nTopicMax);
		strList.Add(strText);
		strText.Format("%d",m_data.m_nTryTimes);
		strList.Add(strText);
		strList.Add(m_data.m_bAutoPublish?_T("是"):_T("否"));
		strList.Add(m_data.m_strServerColumnID);
		
		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
		
	}
	else
	{
        strText=_T("序号,60;文章标题,500;图片数量,100;是否发布,100;文章时间,150");
		pListCtrl->SetHeadings(strText);

		POSITION pos=GetFirstChildPosition();
		int i=0;
		while (pos)
		{
			CTopic* pTopic=(CTopic*)GetNextChild(pos);
			if(pTopic!=NULL)
			{
                strList.RemoveAll();

				strText.Format("%d",++i);
				strList.Add(strText);
				strList.Add(pTopic->m_data.m_strTitle);
				strText.Format("%d",pTopic->GetChildCount());
				strList.Add(strText);
				strList.Add(pTopic->m_data.m_bIsPublish?_T("已发布"):_T("未发布"));
                strList.Add(pTopic->m_data.m_strTime);

				pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pTopic);
			}
		}
	}
}

TiXmlNode*  CEntry::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_entry"));

	pTemp->SetAttribute(_T("entry_id"),m_data.m_strEntryID);
	pTemp->SetAttribute(_T("column_id"),m_data.m_strColumnID);
	pTemp->SetAttribute(_T("server_column_id"),m_data.m_strServerColumnID);
    pTemp->SetAttribute(_T("rule_id"),m_data.m_strRuleID);
    pTemp->SetAttribute(_T("entry_name"),m_data.m_strEntryName);
	pTemp->SetAttribute(_T("website_name"),m_data.m_strWebSiteName);
    pTemp->SetAttribute(_T("url"),m_data.m_strURL);
    pTemp->SetAttribute(_T("match_rule"),m_data.m_strMatchRule);
	pTemp->SetAttribute(_T("topic_max"),m_data.m_nTopicMax);
    pTemp->SetAttribute(_T("try_times"),m_data.m_nTryTimes);
    pTemp->SetAttribute(_T("is_auto_publish"),(int)m_data.m_bAutoPublish);
	pTemp->SetAttribute(_T("last_download_time"),m_data.m_lastDownloadTime.Format("%Y-%m-%d %H:%M:%S"));   

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CEntry::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	
	for(pElement=pNode->FirstChildElement(_T("wdm_topic"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("topic_id"));
		if(!strID.IsEmpty())
		{
			CTopic* pTopic=(CTopic*)FindSubObject(OBJECT_TYPE_TOPIC,strID);
			if(pTopic==NULL)
			{
				pTopic=new CTopic;
				pTopic->m_data.m_strTopicID=strID;
				pTopic->AddTo(this);
			}				
			pTopic->m_data.m_strEntryID=pElement->Attribute(_T("entry_id"));
			pTopic->m_data.m_strURL=pElement->Attribute(_T("url"));
			pTopic->m_data.m_strTitle=pElement->Attribute(_T("title"));
			pTopic->m_data.m_strOldTitle=pElement->Attribute(_T("old_title"));
			pTopic->m_data.m_strMainTitle=pElement->Attribute(_T("main_title"));
			pTopic->m_data.m_strHeadTitle=pElement->Attribute(_T("head_title"));
			pTopic->m_data.m_strSubTitle=pElement->Attribute(_T("sub_title"));
			pTopic->m_data.m_strSource=pElement->Attribute(_T("source"));
			pTopic->m_data.m_strTime=pElement->Attribute(_T("time"));
			pTopic->m_data.m_strAuthor=pElement->Attribute(_T("author"));
			pTopic->m_data.m_strKeyWord=pElement->Attribute(_T("keyword"));
			pTopic->m_data.m_strSubject=pElement->Attribute(_T("subject"));
			pTopic->m_data.m_strContent=pElement->Attribute(_T("content"));
			pElement->QueryIntAttribute(_T("is_publish"),&(int)pTopic->m_data.m_bIsPublish);
			pElement->QueryIntAttribute(_T("is_sendto"),&(int)pTopic->m_data.m_bIsSendto);
			pElement->QueryIntAttribute(_T("is_primary"),&(int)pTopic->m_data.m_bIsPrimary);
			//pElement->QueryIntAttribute(_T("is_elite"),&(int)pTopic->m_data.m_bIsElite);
			//pElement->QueryIntAttribute(_T("is_ontop"),&(int)pTopic->m_data.m_bIsOnTop);
			pTopic->m_data.m_timeDownload.ParseDateTime(pElement->Attribute(_T("download_date")));			
			pTopic->m_data.m_strServerColumnIDs=pElement->Attribute(_T("server_column_ids"));
            pElement->QueryIntAttribute(_T("weight"),&(pTopic->m_data.m_nWeight));
			pTopic->m_data.m_strServerAutoID=pElement->Attribute(_T("server_auto_id"));
			pTopic->m_data.m_strThumb=pElement->Attribute(_T("thumb"));

			pTopic->SetID(strID,pTopic->m_data.m_strTitle);
			if(pTopic->m_data.m_bIsPublish)
		        pTopic->SetSpecShowFlag(TRUE);

			pTopic->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
			pTopic->StreamIn(pElement,bChild,pProgressBar);
		}
	}	
    return TRUE;
}

TiXmlNode* CEntry::Export(CString& strDir,TiXmlNode* pNode,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	TiXmlElement * pTemp=new TiXmlElement(_T("Index"));

	pTemp->SetAttribute(_T("id"),m_data.m_strEntryID);
	pTemp->SetAttribute(_T("name"),m_data.m_strEntryName);

	pNode->LinkEndChild(pTemp);
	
	//Create the file
	CString strFile;
	strFile.Format("%s\\Index\\%s.xml",strDir,GetID());

	TiXmlDocument doc(strFile);
    TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T(""));
	doc.LinkEndChild(pDecl);
    
	TiXmlElement* pRoot=new TiXmlElement(_T("Root"));
	doc.LinkEndChild(pRoot);

	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		CTopic* pTopic=(CTopic*)GetNextChild(pos);
		if(pTopic)
		{
			TiXmlElement* pElement=new TiXmlElement(_T("Article"));
			pElement->SetAttribute(_T("id"),pTopic->GetID());
			pElement->SetAttribute(_T("url"),pTopic->m_data.m_strURL);
			pElement->SetAttribute(_T("title"),pTopic->GetDesc());
			pElement->SetAttribute(_T("subTime"),pTopic->m_data.m_strTime);
            pRoot->LinkEndChild(pElement);
		}
	}
	doc.SaveFile();
    //Create the dir
	CString strTempDir;
	strTempDir.Format("%s\\Articles\\%s",strDir,GetID());
	CCommonFunction::CreateDir(strTempDir);

	CBaseObject::Export(strTempDir,NULL,bChild,pProgressBar);

	return NULL;
}
void CEntry::StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
// 	stream <<_T("小栏目: ")<<m_data.m_strEntryName<<endl;
//     stream <<endl;

	CBaseObject::StreamOutTxt(stream,bChild,pProgressBar);
}
CString CEntry::GetChannelName()
{
    CString str=_T("");
	 
	CColumn* pColumn=(CColumn*)BackTo(OBJECT_TYPE_COLUMN);
    // find the top column
	while (pColumn!=NULL && pColumn->m_data.m_strParentID.CompareNoCase((CString)NULL_ID)!=0)
	{
		pColumn=(CColumn*)pColumn->BackTo(OBJECT_TYPE_COLUMN);
	}
	ASSERT(pColumn);

    str=pColumn->m_data.m_strColumnName;
	int i=str.Find('(');
	if(i>0)
		str=str.Left(i);

	return str;
}