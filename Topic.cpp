// Topic.cpp: implementation of the CTopic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "Topic.h"
#include "TopicDlg.h"
#include "NewsPicture.h"
#include "Column.h"
#include "DBServer.h"
#include "WDMTable.h"
#include "WDMOperator.h"
#include "WDMOperatorUnit.h"
#include "Entry.h"
#include "Extract.h"
#include "MainFrm.h"
#include "Regex.h"
#include "RegexWDM.h"
#include "Md5.h"
#include "GetHostIP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTopic::CTopic() :CBaseObject(OBJECT_TYPE_TOPIC)
{
     SetImg(6,7);
}

CTopic::~CTopic()
{
}
void CTopic::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strTopicID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CTopic::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
    BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_PICTURE:	//可以挂接图片
		bAccept=TRUE;
		break;
	}

	return bAccept;
}
void CTopic::DoJoin(CBaseObject *pParent)
{
	m_data.m_strEntryID=pParent->GetID();
    m_data.m_strBigClass=pParent->GetParent()->GetDesc();
	m_data.m_strSmallClass=((CEntry*)pParent)->m_data.m_strEntryName;
	if(!m_data.m_bIsPublish)
		m_data.m_strServerColumnIDs=((CEntry*)pParent)->m_data.m_strServerColumnID;
    
	CBaseObject::DoJoin(this);
}		
void CTopic::DoLeave(CBaseObject *pParent)
{
	m_data.m_strEntryID.Empty();

	CBaseObject::DoLeave(this);
}
UINT CTopic::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
    UINT nRet;
	CTopicDlg dlg(this);

	dlg.SetValue(m_data);
	nRet=dlg.DoModal();
	if( nRet== IDOK )
	{
		if(dlg.GetValue(m_data))
		{
			SetModify();
		    m_strDesc=m_data.m_strTitle;			
		}
	}

	return nRet;
}
CWDMDatabase* CTopic::GetDatabase()
{
//  CColumn* pColumn=(CColumn*)BackTo(OBJECT_TYPE_COLUMN);
// 	ASSERT(pColumn!=NULL);
//  // find the top column
// 	while (pColumn!=NULL && pColumn->m_data.m_strDatabaseID.IsEmpty())
// 	{
// 		pColumn=(CColumn*)pColumn->BackTo(OBJECT_TYPE_COLUMN);
// 	}
//     
// 	if(pColumn==NULL)
// 		return NULL;

	CBaseObject* pOb=theApp.GetDBServerTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_DATABASE,GetServerDatabaseID()/*pColumn->m_data.m_strDatabaseID*/,TRUE);
    if(pOb==NULL)
		return NULL;

	return static_cast<CWDMDatabase*>(pOb);	
}


BOOL CTopic::IsValid()
{
	CWDMDatabase* pDatabse=GetDatabase();
	if(pDatabse==NULL)
		return FALSE;   
	if(m_data.m_strServerColumnIDs.IsEmpty())
		return FALSE;
    return pDatabse->IsValid();
}
BOOL CTopic::IsConverted()
{
	ASSERT(IsValid());   

	return GetDatabase()->IsConverted();
}
BOOL CTopic::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_topic where topic_id='%s'",m_data.m_strTopicID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("topic_id",m_data.m_strTopicID);
		}
		else
			rs.Edit();

		bRet&=rs.SetFieldValue("entry_id",m_data.m_strEntryID);       
		bRet&=rs.SetFieldValue("title",m_data.m_strTitle.Left(100));
		bRet&=rs.SetFieldValue("old_title",m_data.m_strOldTitle.Left(100));
		bRet&=rs.SetFieldValue("main_title",m_data.m_strMainTitle.Left(100));   
		bRet&=rs.SetFieldValue("head_title",m_data.m_strHeadTitle.Left(300));       
		bRet&=rs.SetFieldValue("sub_title",m_data.m_strSubTitle.Left(300));       
		bRet&=rs.SetFieldValue("source",m_data.m_strSource.Left(100));       
		bRet&=rs.SetFieldValue("url",m_data.m_strURL.Left(300));       
		bRet&=rs.SetFieldValue("author",m_data.m_strAuthor.Left(50));  
		m_data.m_strTime.Replace('.','-');
		COleDateTime dt;
		if(dt.ParseDateTime(m_data.m_strTime))
		    bRet&=rs.SetFieldValue("time",dt); 
//		bRet&=rs.SetFieldValue("time",m_data.m_strTime);       
		bRet&=rs.SetFieldValue("keyword",m_data.m_strKeyWord.Left(100));       
		bRet&=rs.SetFieldValue("subject",m_data.m_strSubject.Left(100));
		bRet&=rs.SetFieldValue("content",m_data.m_strContent);
//		bRet&=rs.SetChunk("content",m_data.m_strContent); 
		bRet&=rs.SetFieldValue("is_primary",m_data.m_bIsPrimary);
		//bRet&=rs.SetFieldValue("is_elite",m_data.m_bIsElite);
		//bRet&=rs.SetFieldValue("is_ontop",m_data.m_bIsOnTop);
		bRet&=rs.SetFieldValue("is_publish",m_data.m_bIsPublish);       
		bRet&=rs.SetFieldValue("is_sendto",m_data.m_bIsSendto); 
		bRet&=rs.SetFieldValue("download_date",m_data.m_timeDownload);
		bRet&=rs.SetFieldValue("server_column_ids",m_data.m_strServerColumnIDs);
		bRet&=rs.SetFieldValue("weight",m_data.m_nWeight);
		bRet&=rs.SetFieldValue("server_auto_id",m_data.m_strServerAutoID);
		bRet&=rs.SetFieldValue("thumb",m_data.m_strThumb);

		bRet&=rs.Update();

		rs.Close();
	}
	//copy thumb picture to wdm server
	CString strURL(m_data.m_strThumb);

	if(!strURL.IsEmpty() && theApp.GetNetworkType() == 0 && theApp.DBIsOpen(CWebDataMiningApp::wdmServer))
	{
		CString strSource,strDst;		

		CCommonFunction::Replace(strURL,"/","\\");

		strSource.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),strURL);
		strDst.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::wdmServer),strURL);

		try
		{
			CCommonFunction::CreateDir(strDst);
			CopyFile(strSource,strDst,FALSE);
		}
		catch (...)
		{
			CString str;
			str.Format("复制文件错误:%s",strSource);
			static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
		}

	}
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CTopic::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;

	strSql.Format("select * from wdm_picture where topic_id='%s' order by [start_pos]",m_data.m_strTopicID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	
	CNewsPicture* pPicture=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pPicture=new CNewsPicture;
			pPicture->LoadInfo(rs);
			pPicture->AddTo(this);            

			rs.MoveNext();
		}
		rs.Close();
	}
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CTopic::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet &=rs.GetFieldValue("topic_id",m_data.m_strTopicID);
	bRet &=rs.GetFieldValue("entry_id",m_data.m_strEntryID);
	bRet &=rs.GetFieldValue("head_title",m_data.m_strHeadTitle);
	bRet &=rs.GetFieldValue("title",m_data.m_strTitle);
	bRet &=rs.GetFieldValue("old_title",m_data.m_strOldTitle);
	bRet &=rs.GetFieldValue("main_title",m_data.m_strMainTitle);
	bRet &=rs.GetFieldValue("sub_title",m_data.m_strSubTitle);
	bRet &=rs.GetFieldValue("source",m_data.m_strSource);
	bRet &=rs.GetFieldValue("time",m_data.m_strTime);
	bRet &=rs.GetFieldValue("author",m_data.m_strAuthor);
	bRet &=rs.GetFieldValue("content",m_data.m_strContent);
//	bRet &=rs.GetChunk("content",m_data.m_strContent);
	bRet &=rs.GetFieldValue("keyword",m_data.m_strKeyWord);
	bRet &=rs.GetFieldValue("subject",m_data.m_strSubject);
	bRet &=rs.GetFieldValue("is_publish",m_data.m_bIsPublish);
	bRet &=rs.GetFieldValue("is_sendto",m_data.m_bIsSendto);
	bRet &=rs.GetFieldValue("is_primary",m_data.m_bIsPrimary);
	//bRet &=rs.GetFieldValue("is_elite",m_data.m_bIsElite);
	//bRet &=rs.GetFieldValue("is_ontop",m_data.m_bIsOnTop);
	bRet &=rs.GetFieldValue("url",m_data.m_strURL);
	bRet &=rs.GetFieldValue("download_date",m_data.m_timeDownload);
	bRet &=rs.GetFieldValue("server_column_ids",m_data.m_strServerColumnIDs);
    bRet &=rs.GetFieldValue("weight",m_data.m_nWeight);
	bRet &=rs.GetFieldValue("server_auto_id",m_data.m_strServerAutoID);
	bRet &=rs.GetFieldValue("thumb",m_data.m_strThumb);
	
	if(m_data.m_bIsPublish)
		SetSpecShowFlag(TRUE);
	
	m_strID=m_data.m_strTopicID;
	m_strDesc=m_data.m_strTitle;

	return bRet;
}
BOOL CTopic::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_topic where topic_id='%s'",m_data.m_strTopicID);
		bRet=db.Execute(strSql);
	}
    if(bRet)
		theApp.DeleteVisitedUrlMD5(m_data.m_strURL);

	return bRet;
}	

UINT CTopic::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CTopic::PromptMenuPos()
{
	return 4;
}
void CTopic::UpdateMenu(CMenu *pMenu)
{
	UINT ID=RetrieveMenuItemID(pMenu,"发布文章");
    UINT delTopics=RetrieveMenuItemID(pMenu,"删除文章");
    UINT delPic=RetrieveMenuItemID(pMenu,"删除图片");

	ASSERT(ID!=-1 && delTopics!=-1 && delPic!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delTopics,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
	pMenu->EnableMenuItem(delPic,GetChildCount()>0 ? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));

	pMenu->ModifyMenu(ID,MF_BYCOMMAND,ID,m_data.m_bIsPublish?_T("已发布"):_T("发布"));
	pMenu->EnableMenuItem(ID,(theApp.GetNetworkType()==1 && IsValid())?(MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}

CString CTopic::GetDirectory()
{
    CString strDir=_T("");
	CBaseObject* pParent=GetParent();

    do 
    {		
		strDir=pParent->GetDesc()+"\\"+strDir;		
        pParent=pParent->GetParent();
    } 
	while(pParent->GetType()!=OBJECT_TYPE_HELPER);
   
	strDir=strDir.Left(strDir.GetLength()-1);

	return strDir;
}
BOOL CTopic::PublishToServer()
{
	BOOL bRet=TRUE;
    int nIndex=0;
	CString strID,strIDs;
	CStringArray  ar;
    //获取服务器栏目信息
	strIDs=m_data.m_strServerColumnIDs;

	for(nIndex=strIDs.Find(' ');nIndex!=-1;nIndex=strIDs.Find(' '))
	{
        strID=strIDs.Left(nIndex);
		ar.Add(strID);

		strIDs=strIDs.Mid(nIndex+1);
		strIDs.TrimLeft();
	}
    ar.Add(strIDs);
	//发布文章
	bRet=PublishToServer_New2();
    //发布转向链接文章
	if(bRet && ar.GetSize()>1)
	{
		CWDMDatabase* pDatabase=GetDatabase();
		ASSERT(pDatabase!=NULL);

		CADODatabase adoServer;	

		if( pDatabase->GetAdoDatabase(adoServer) == FALSE )
			return FALSE;
		
		CString strSql,str;		
		//处理文章表类型
		CWDMTable* pTable=NULL;
		CWDMOperator* pSaveOperator=NULL;
		CWDMOperatorUnit* pUnit=NULL;
		CTypedPtrArray<CPtrArray,CWDMTable*> tables;
		CTypedPtrArray<CPtrArray,CWDMOperatorUnit*> units;

		// 第一个栏目已发布，转向链接从第二个开始
		for(int k=1;k<ar.GetSize();k++)
		{
			pDatabase->GetWDMTables(tables,CWDMTable::Table_LinkUrl);
			for(int i=0;i<tables.GetCount();i++)
			{
				pTable = tables.GetAt(i);							

				strSql=GetSqlString(pTable,CWDMOperator::opSave);

				CADORecordset rs(&adoServer);

				if(rs.Open(strSql,CADORecordset::openQuery))
				{
					if(rs.GetRecordCount()==1)
					{
						pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
						ASSERT(pSaveOperator!=NULL);					

						CStringArray fv;
						// 取出对应字段的值保存到数组
						pSaveOperator->GetWDMOperatorUnits(units);
						for(int j=0;j<units.GetCount();j++)
						{
							pUnit=units.GetAt(j);
							// 主键不用处理
							if( pUnit->m_data.m_bIsKey )
								continue;
							// T_SERVERCOLUMNID为服务器栏目ID
							if( pUnit->ContainTopicUnit(T_SERVERCOLUMNID ) )
								fv.Add(ar.GetAt(k));
							else
								fv.Add(GetUnitValue(pUnit,&rs));	
						}
						// 组合sql语句
						strSql.Format("insert into %s (",pTable->GetDesc());

						for( int j=0;j<units.GetCount();j++)
						{
							pUnit=units.GetAt(j);
							if( pUnit->m_data.m_bIsKey )
								continue;

							strSql += pUnit->m_data.m_strUnitName;
							strSql += _T(",");
						}
						// 去掉最后逗号
						strSql = strSql.Left(strSql.GetLength()-1);
						strSql += _T(") values (");

						for( int j=0;j<fv.GetCount();j++)
						{
							str.Format("'%s'",fv.GetAt(j));
							strSql += str;
							strSql += _T(",");
						}
						strSql = strSql.Left(strSql.GetLength()-1);
						strSql += _T(")");

						bRet &= adoServer.Execute(strSql);
					}
					rs.Close();
				}
			}
		}
		adoServer.Close();
	}	
	
	return bRet;
}
//
//BOOL CTopic::PublishToServer_New()
//{
//	CWDMDatabase* pDatabase=GetDatabase();
//	ASSERT(pDatabase!=NULL);
//
//	COptionsData::OptionsData::DBServerData data;
//	pDatabase->GetDBServerData(data);
//
//	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer)==FALSE)
//		return FALSE;
//
//	BOOL bRet=TRUE,bEdit=FALSE;
//	CString strSql,str;
//	// 取得远程服务器图片存储参数
//	m_strRemoteResourceDir = data.m_strResourceDir;
//	m_strRemoteResourcePath = data.m_strResourcePath;
//    //执行数据表指定的操作
//	//处理文章表类型
//	CWDMTable* pTable=NULL;
//	pTable=pDatabase->GetWDMTable(CWDMTable::Table_Topic);
//	if(pTable!=NULL)
//	{
//		strSql=GetSqlString(pTable,CWDMOperator::opSave);
//		CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));
//		
//		if(rs.Open(strSql,CADORecordset::openQuery))
//		{
//			if(rs.GetRecordCount()==0)
//			{
//				bRet&=rs.AddNew();
//			}
//			else
//			{
//				//重发
//				if( m_data.m_bIsPublish )  
//				{
//					rs.Edit();
//				    bEdit=TRUE;
//				}
//				//存在类似旧文章
//				else
//				{
//					switch(MessageBox(NULL,_T("文章：")+m_data.m_strTitle+_T("\n已经存在，选择“是”为履盖，“否”为新增"),_T("请确认"),MB_YESNOCANCEL|MB_ICONQUESTION))
//					{
//					case IDYES:
//						rs.Edit();
//				        bEdit=TRUE;
//						break;
//					case IDNO:
//						rs.AddNew();
//						break;
//				    default:
//						rs.Close();
//						return FALSE;						
//					}					
//				}
//			}
//			
//			CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
//			ASSERT(pSaveOperator!=NULL);
//			
//			POSITION pos1=pSaveOperator->GetFirstChildPosition();
//			while (pos1)
//			{
//				CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pSaveOperator->GetNextChild(pos1);
//				if(pUnit!=NULL && !(pUnit->ContainTopicUnit(T_LINKURL)))
//				{   
//					str.Empty();
//					str=GetTopicUnitValue(pUnit->m_data.m_dwTopicUnit);
//					//if(str.IsEmpty())
//					//{			
//					//	if(_tcscmp(pUnit->m_data.m_strUnitDefault,_T("newid()"))==0)
//					//		str=CCommonFunction::AllocateID();
//					//	else if(_tcscmp(pUnit->m_data.m_strUnitDefault,_T("time()"))==0)
//					//		str=COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
//					//	else
//					//		str=pUnit->m_data.m_strUnitDefault;
//					//}
//					if( !bEdit || !pUnit->m_data.m_bIsKey)
//					    bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
//				}
//			}
//			bRet&=rs.Update();
//			rs.Close();
//		}
//	}
//	//处理资源表类型
//    if(GetChildCount()>0)
//	{
//		pTable=NULL;
//		pTable=pDatabase->GetWDMTable(CWDMTable::Table_Resource);
//		if(pTable!=NULL)
//		{
//			//对资源按序存放
//			int i=0;
//			CArray<CNewsPicture*,CNewsPicture*> pictures;
//			
//			POSITION pos=GetFirstChildPosition();
//			while (pos)
//			{
//				CNewsPicture* pPicture=(CNewsPicture*)GetNextChild(pos);
//				if(pPicture!=NULL)
//				{
//					for(i=0;i<pictures.GetSize();i++)
//					{
//						if(pPicture->m_data.m_nStartPos<pictures.GetAt(i)->m_data.m_nStartPos)
//						{
//							pictures.InsertAt(i,pPicture);
//							break;
//						}
//					}
//					if(i==pictures.GetSize())
//						pictures.Add(pPicture);
//				}
//			}
//			//资源数据写入库
//			for(i=0;i<pictures.GetSize();i++)
//			{
//				strSql.Format("select * from %s where ",pTable->GetDesc());				
//				
//				CWDMOperator* pOperator=pTable->GetOperator(CWDMOperator::opSave);
//				POSITION pos=pOperator->GetFirstChildPosition();
//				while (pos)
//				{
//					CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pOperator->GetNextChild(pos);
//					if(pUnit!=NULL && pUnit->m_data.m_bIsKey)
//					{
//						strSql+=pUnit->m_data.m_strUnitName+_T("='");
//						strSql+=GetResourceUnitValue(pUnit->m_data.m_dwTopicUnit,pictures.GetAt(i))+_T("' and ");
//					}
//				}
//				strSql=strSql.Left(strSql.GetLength()-5);
//
//				CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));
//				
//				if(rs.Open(strSql,CADORecordset::openQuery))
//				{
//					bEdit=FALSE;
//					if(rs.GetRecordCount()==0)
//					{
//						bRet&=rs.AddNew();
//					}
//					else
//					{
//						rs.Edit();
//						bEdit=TRUE;
//					}
//					
//					CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
//					ASSERT(pSaveOperator!=NULL);
//					
//					POSITION pos1=pSaveOperator->GetFirstChildPosition();
//					while (pos1)
//					{
//						CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pSaveOperator->GetNextChild(pos1);
//						if(pUnit!=NULL)
//						{   
//							str.Empty();
//							str=GetResourceUnitValue(pUnit->m_data.m_dwTopicUnit,pictures.GetAt(i));
//							//if(str.IsEmpty())
//							//{			
//							//	if(_tcscmp(pUnit->m_data.m_strUnitDefault,_T("newid()"))==0)
//							//		str=CCommonFunction::AllocateID();
//							//	else if(_tcscmp(pUnit->m_data.m_strUnitDefault,_T("time()"))==0)
//							//		str=COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
//							//	else
//							//		str=pUnit->m_data.m_strUnitDefault;
//							//}
//							if( !bEdit || !pUnit->m_data.m_bIsKey)
//							     bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
//						}
//					}
//					bRet&=rs.Update();
//					rs.Close();
//				}
//			}			
//		}
//	}
//	//处理专题表类型
//	if(!m_data.m_strSubject.IsEmpty())
//	{
//		pTable=NULL;
//		pTable=pDatabase->GetWDMTable(CWDMTable::Table_Special);
//		if(pTable!=NULL)
//		{
//			strSql=GetSqlString(pTable,CWDMOperator::opSave);
//			CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));
//			
//			if(rs.Open(strSql,CADORecordset::openQuery))
//			{
//				if(rs.GetRecordCount()==0)
//				{
//					bRet&=rs.AddNew();
//
//					CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
//					ASSERT(pSaveOperator!=NULL);
//					
//					POSITION pos1=pSaveOperator->GetFirstChildPosition();
//					while (pos1)
//					{
//						CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pSaveOperator->GetNextChild(pos1);
//						if(pUnit!=NULL)
//						{   
//							str.Empty();
//							str=GetTopicUnitValue(pUnit->m_data.m_dwTopicUnit);
//							//if(str.IsEmpty())
//							//{			
//							//	if(_tcscmp(pUnit->m_data.m_strUnitDefault,_T("newid()"))==0)
//							//		str=CCommonFunction::AllocateID();
//							//	else if(_tcscmp(pUnit->m_data.m_strUnitDefault,_T("time()"))==0)
//							//		str=COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
//							//	else
//							//		str=pUnit->m_data.m_strUnitDefault;
//							//}
//							bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
//						}
//					}
//				    bRet&=rs.Update();
//				}			
//				rs.Close();
//			}
//		}
//	}
//    
//// 	theApp.CloseDB(CWebDataMiningApp::DBServerType::remoteServer);
//    //更新文章信息
//	if(bRet)
//	{
//		CLogs::WriteLogs("发布："+GetDesc(),CLogs::log_publish_topic);
//        //第一次发布
//		if(m_data.m_bIsPublish==FALSE)
//		{	
//			m_data.m_bIsPublish=TRUE;
//
//			// 初始化socket库
//			if (!AfxSocketInit())
//			{
//				AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//			}
//			else
//			{
//				TopicParam tp;
//				tp.bIsPublished=m_data.m_bIsPublish;
//				_tcscpy(tp.strTopicID,m_data.m_strTopicID);
//				//发送广播消息，通知文章已发布
//				CSocket m_hSocket;	
//				BOOL bSuccess=FALSE;			
//				if(m_hSocket.Create(2330,SOCK_DGRAM))
//				{
//					bSuccess=m_hSocket.SendTo((LPTOPICPARAM)&tp,sizeof(tp),theApp.GetNetworkPortNum(),theApp.GetBroadcastIP())!=SOCKET_ERROR;			     			 
//				}
//			}			
//			//设置本地信息          			
//			SpecShowInTree(theApp.GetSourceTree(),m_data.m_bIsPublish);				
//			SaveToDB(*theApp.GetDB(),TRUE);
//		}
//	}
//	return bRet;
//}
BOOL CTopic::PublishToServer_New2()
{	
	CWDMDatabase* pDatabase=GetDatabase();
	ASSERT(pDatabase!=NULL);

	CADODatabase adoServer;	

	if( pDatabase->GetAdoDatabase(adoServer) == FALSE )
		return FALSE;

	BOOL bRet=TRUE,bEdit=FALSE;
	CString str,strSql;
	// 取得远程服务器图片存储参数
	COptionsData::OptionsData::DBServerData data;
	pDatabase->GetDBServerData(data);

	m_strRemoteResourceDir = data.m_strResourceDir;
	m_strRemoteResourcePath = data.m_strResourcePath;
	//执行数据表指定的操作
	
	CWDMTable* pTable=NULL;
	CWDMOperatorUnit* pUnit= NULL;
	CWDMOperatorUnit* pUnitAutoID= NULL;
	CTypedPtrArray<CPtrArray,CWDMTable*> tables;
	CTypedPtrArray<CPtrArray,CWDMOperatorUnit*> units;
	CTypedPtrArray<CPtrArray,CWDMOperatorUnit*> unitsHaveAutoID;
	
	m_mapCache.RemoveAll();
	//预处理数据表	
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Pre);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);
		// 如果重发，检查该表是否允许重发
		if( m_data.m_bIsPublish && ( pTable->m_data.m_bPublishRepeat == FALSE ))
			continue;		

		bEdit=FALSE;
		strSql=GetSqlString(pTable,CWDMOperator::opSave);

		CADORecordset rs(&adoServer);
		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			if(rs.GetRecordCount()==0)
			{
				bRet&=rs.AddNew();
			}
			else
			{
				rs.Edit();
				bEdit=TRUE;				
			}

			CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
			ASSERT(pSaveOperator!=NULL);
				
			pUnitAutoID= NULL;
			unitsHaveAutoID.RemoveAll();
			pSaveOperator->GetWDMOperatorUnits(units);
			for(int i=0;i<units.GetCount();i++)
			{
				pUnit=units.GetAt(i);
				if( pUnit!=NULL  )
				{   
					// 自增ID
					if( pUnit->GetUnitNum() == 1 && pUnit->ContainTopicUnit(T_SERVERAUTOID ) )
					{
						pUnitAutoID = pUnit;
						continue;
					}
					// 包含自增id的复合字段集
					if( pUnit->ContainTopicUnit(T_SERVERAUTOID ))
						unitsHaveAutoID.Add(pUnit);

					str=GetUnitValue(pUnit,&rs);

					if( !bEdit || !pUnit->m_data.m_bIsKey )
						bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
				}
			}
			bRet&=rs.Update();
			//获取自增id值并保存为缓存值
			if( bRet && pUnitAutoID )
			{				
				if( bEdit )
				{								
					rs.GetFieldValue(pUnitAutoID->m_data.m_strUnitName,str);	
				}
				else
				{
					str = GetServerAutoID(pTable,adoServer);
				}
				m_mapCache.SetAt(pUnitAutoID->GetKeyString(),str);
												
				// 处理包含自增ID的复合字段	
				if( unitsHaveAutoID.GetCount()>0)
				{
					rs.Edit();
					for( int k=0;k<unitsHaveAutoID.GetCount();k++)
					{
						pUnit = unitsHaveAutoID.GetAt(k);						
						str=GetUnitValue(pUnit,&rs);

						bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
					}
					rs.Update();
				}									
			}
			rs.Close();
		}
	}
		
    //处理文章表类型
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Topic);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);
		// 如果重发，检查该表是否允许重发
		if( m_data.m_bIsPublish && ( pTable->m_data.m_bPublishRepeat == FALSE ))
			continue;

		strSql=GetSqlString(pTable,CWDMOperator::opSave);
		bEdit=FALSE;

		CADORecordset rs(&adoServer);
		//CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));

		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			if(rs.GetRecordCount()==0)
			{
				bRet&=rs.AddNew();
			}
			else
			{
				//重发
				if( m_data.m_bIsPublish )  
				{
					rs.Edit();
					bEdit=TRUE;
				}
				//存在类似旧文章
				else
				{
					switch(MessageBox(NULL,_T("文章：")+m_data.m_strTitle+_T("\n已经存在，选择“是”为履盖，“否”为新增"),_T("请确认"),MB_YESNOCANCEL|MB_ICONQUESTION))
					{
					case IDYES:
						rs.Edit();
						bEdit=TRUE;
						break;
					case IDNO:
						rs.AddNew();
						break;
					default:
						rs.Close();
						return FALSE;						
					}					
				}
			}

			CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
			ASSERT(pSaveOperator!=NULL);

			pUnitAutoID= NULL;
			unitsHaveAutoID.RemoveAll();
			pSaveOperator->GetWDMOperatorUnits(units);
			for(int i=0;i<units.GetCount();i++)
			{
				pUnit=units.GetAt(i);
				if( pUnit!=NULL /*&& !( pUnit->ContainTopicUnit(T_LINKURL) ) */)
				{   
					if( pUnit->GetUnitNum() == 1 && pUnit->ContainTopicUnit(T_SERVERAUTOID ) )
					{
						pUnitAutoID = pUnit;
						continue;
					}
					// 包含自增id的复合字段
					if( pUnit->ContainTopicUnit(T_SERVERAUTOID ))
						unitsHaveAutoID.Add(pUnit);

					str=GetUnitValue(pUnit,&rs);
					
					if( !bEdit || !pUnit->m_data.m_bIsKey )
						bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
				}
			}
			bRet&=rs.Update();
			//获取文章自增id并更新相关字段信息
			if( bRet && pUnitAutoID )
			{				
				if( bEdit )
				{								
					rs.GetFieldValue(pUnitAutoID->m_data.m_strUnitName,str);	
				}
				else
				{
					str = GetServerAutoID(pTable,adoServer);
				}
				m_mapCache.SetAt(pUnitAutoID->GetKeyString(),str);
				m_data.m_strServerAutoID = str;
				// 处理包含自增ID的复合字段	
				if( unitsHaveAutoID.GetCount()>0)
				{
					rs.Edit();
					for( int k=0;k<unitsHaveAutoID.GetCount();k++)
					{
						pUnit = unitsHaveAutoID.GetAt(k);						
						str=GetUnitValue(pUnit,&rs);

						bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
					}
					rs.Update();
				}									
			}
			rs.Close();
		}
	}
	//处理图片表类型
	if(GetChildCount()>0)
	{
		//对资源按序存放
		int nIndex=0;
		CNewsPicture* pPicture = NULL;
		CArray<CNewsPicture*,CNewsPicture*> pictures;

		POSITION pos=GetFirstChildPosition();
		while (pos)
		{
			pPicture=(CNewsPicture*)GetNextChild(pos);
			if(pPicture!=NULL)
			{
				for(nIndex=0;nIndex<pictures.GetSize();nIndex++)
				{
					if(pPicture->m_data.m_nStartPos<pictures.GetAt(nIndex)->m_data.m_nStartPos)
					{
						pictures.InsertAt(nIndex,pPicture);
						break;
					}
				}
				if(nIndex == pictures.GetSize())
					pictures.Add(pPicture);
			}
		}

		pTable=NULL;
		pDatabase->GetWDMTables(tables,CWDMTable::Table_Picture);
		for(int i=0;i<tables.GetCount();i++)
		{				
			pTable = tables.GetAt(i);
			// 如果重发，检查该表是否允许重发
			if( m_data.m_bIsPublish && ( pTable->m_data.m_bPublishRepeat == FALSE ))
				continue;
			//资源数据写入库
			for(int j=0;j<pictures.GetSize();j++)
			{
				pPicture = pictures.GetAt(j);
				strSql.Format("select * from %s where ",pTable->GetDesc());				

				CWDMOperator* pOperator=pTable->GetOperator(CWDMOperator::opSave);
				
				POSITION pos=pOperator->GetFirstChildPosition();
				while (pos)
				{
					CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pOperator->GetNextChild(pos);
					if(pUnit!=NULL && pUnit->m_data.m_bIsKey)
					{
						strSql+=pUnit->m_data.m_strUnitName+_T("='");
						strSql+=GetUnitValue(pUnit,NULL,pPicture)+_T("' and ");
					}
				}
				strSql=strSql.Left(strSql.GetLength()-5);

				CADORecordset rs(&adoServer);
				//CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));

				if(rs.Open(strSql,CADORecordset::openQuery))
				{
					bEdit=FALSE;
					if(rs.GetRecordCount()==0)
					{
						bRet&=rs.AddNew();
					}
					else
					{
						rs.Edit();
						bEdit=TRUE;
					}

					CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
					ASSERT(pSaveOperator!=NULL);

					pUnitAutoID = NULL;
					unitsHaveAutoID.RemoveAll();
					pSaveOperator->GetWDMOperatorUnits(units);
					for(int i=0;i<units.GetCount();i++)
					{
						pUnit=units.GetAt(i);
						if(pUnit!=NULL)
						{   
							if( pUnit->GetUnitNum() == 1 && pUnit->ContainTopicUnit(T_SERVERAUTOID ) )
							{
								pUnitAutoID = pUnit;
								continue;
							}
							if( pUnit->ContainTopicUnit(T_SERVERAUTOID ))
								unitsHaveAutoID.Add(pUnit);

							str=GetUnitValue(pUnit,&rs,pPicture);

							if( !bEdit || !pUnit->m_data.m_bIsKey )
								bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
						}
					}
					bRet&=rs.Update();
					//获取资源自增id并更新相关字段信息
					if( bRet && pUnitAutoID )
					{				
						if( bEdit )
						{								
							rs.GetFieldValue(pUnitAutoID->m_data.m_strUnitName,str);	
						}
						else
						{
							str = GetServerAutoID(pTable,adoServer);
						}
						m_mapCache.SetAt(pUnitAutoID->GetKeyString(),str);
						pPicture->m_data.m_strServerAutoID = str;
						// 处理包含自增ID的复合字段	
						if( unitsHaveAutoID.GetCount()>0)
						{
							rs.Edit();
							for( int k=0;k<unitsHaveAutoID.GetCount();k++)
							{
								pUnit = unitsHaveAutoID.GetAt(k);						
								str=GetUnitValue(pUnit,&rs);

								bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
							}
							rs.Update();
						}									
					}
					rs.Close();
				}
			}			
		}		
		//上传图片到远程服务器
		//获取文件服务器对象
		CDBServer* pServer = (CDBServer*)pDatabase->GetParent();
		CFileSend* pFileSend = pServer->GetFileSend();
		ASSERT(pFileSend!=NULL);

		try
		{				
			//上传文章缩略图
			if( !m_data.m_strThumb.IsEmpty() )
			{
				CString strSThumb,strDThumb;
				strSThumb.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),m_data.m_strThumb);
				CCommonFunction::Replace(strSThumb,"/","\\");
				strDThumb.Format("%s\\%s",m_strRemoteResourceDir,m_data.m_strThumb);
				CCommonFunction::Replace(strDThumb,"/","\\");

				if(  CCommonFunction::PictureIsValid(strSThumb) && !pFileSend->SendFileToServer(strSThumb,strDThumb,FALSE) )
				{
					str.Format("上传文章缩略图失败，图片路径:%s",strSThumb);
					static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
				}
			}
			//上传文章图片	
			CString strPicPath,strSource,strSourceDst,strSourceThumb,strThumbDst;

			for( int k=0;k<pictures.GetCount();k++)
			{
				pPicture = pictures.GetAt(k);

				strPicPath=pPicture->m_data.m_strURL;
				CCommonFunction::Replace(strPicPath,"/","\\");

				strSource.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),strPicPath);
				strSourceDst.Format("%s\\%s",m_strRemoteResourceDir,strPicPath);
				//上传图片
				if( CCommonFunction::PictureIsValid(strSource) && !pFileSend->SendFileToServer(strSource,strSourceDst,FALSE) )
				{					
					str.Format("上传图片失败，图片路径:%s",strSource);
					static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
				}	
				//上传图片缩略图
				if( pServer->m_data.m_bThumb )
				{
					strSourceThumb = strSource;
					strSourceThumb.Insert(strSourceThumb.ReverseFind('\\')+1,pServer->m_data.m_strThumbName);

					strThumbDst = strSourceDst;
					strThumbDst.Insert(strThumbDst.ReverseFind('\\')+1,pServer->m_data.m_strThumbName);
					//生成缩略图90*70
					CCommonFunction::ResamplePicture(strSource,strSourceThumb,90,70,theApp.GetPictureInterpolation());	

					if( CCommonFunction::PictureIsValid(strSourceThumb) && !pFileSend->SendFileToServer(strSourceThumb,strThumbDst,FALSE) )
					{
						str.Format("上传图片缩略图失败，图片路径:%s",strSourceThumb);
						static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
					}	
				}
			}
		}
		catch (...)
		{
			str.Format("上传图片到远程服务器异常");
			static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
		}

		delete pFileSend;
		pFileSend = NULL;
	}
	//处理后处理表类型	
	pTable=NULL;
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Post);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);
		// 如果重发，检查该表是否允许重发
		if( m_data.m_bIsPublish && ( pTable->m_data.m_bPublishRepeat == FALSE ))
			continue;

		bEdit=FALSE;
		strSql=GetSqlString(pTable,CWDMOperator::opSave);

		CADORecordset rs(&adoServer);
		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			if(rs.GetRecordCount()==0)
			{
				bRet&=rs.AddNew();
			}
			else
			{
				rs.Edit();
				bEdit=TRUE;				
			}

			CWDMOperator* pSaveOperator=pTable->GetOperator(CWDMOperator::opSave);
			ASSERT(pSaveOperator!=NULL);

			pUnitAutoID= NULL;
			unitsHaveAutoID.RemoveAll();
			pSaveOperator->GetWDMOperatorUnits(units);
			for(int i=0;i<units.GetCount();i++)
			{
				pUnit=units.GetAt(i);
				if( pUnit!=NULL  )
				{   
					// 自增ID
					if( pUnit->GetUnitNum() == 1 && pUnit->ContainTopicUnit(T_SERVERAUTOID ) )
					{
						pUnitAutoID = pUnit;
						continue;
					}
					// 包含自增id的复合字段集
					if( pUnit->ContainTopicUnit(T_SERVERAUTOID ))
						unitsHaveAutoID.Add(pUnit);

					str=GetUnitValue(pUnit,&rs);

					if( !bEdit || !pUnit->m_data.m_bIsKey )
						bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
				}
			}
			bRet&=rs.Update();
			//获取自增id值并保存为缓存值
			if( bRet && pUnitAutoID )
			{				
				if( bEdit )
				{								
					rs.GetFieldValue(pUnitAutoID->m_data.m_strUnitName,str);	
				}
				else
				{
					str = GetServerAutoID(pTable,adoServer);
				}
				m_mapCache.SetAt(pUnitAutoID->GetKeyString(),str);

				// 处理包含自增ID的复合字段	
				if( unitsHaveAutoID.GetCount()>0)
				{
					rs.Edit();
					for( int k=0;k<unitsHaveAutoID.GetCount();k++)
					{
						pUnit = unitsHaveAutoID.GetAt(k);						
						str=GetUnitValue(pUnit,&rs);

						bRet&=rs.SetFieldValue(pUnit->m_data.m_strUnitName,str);
					}
					rs.Update();
				}									
			}
			rs.Close();
		}
	}
	
	//关闭数据库连接
	adoServer.Close();
// 	theApp.CloseDB(CWebDataMiningApp::DBServerType::remoteServer);
	//更新文章信息
	if(bRet)
	{
		CLogs::WriteLogs("发布："+GetDesc(),CLogs::log_publish_topic);
		//第一次发布
		if(m_data.m_bIsPublish==FALSE)
		{	
			m_data.m_bIsPublish=TRUE;

			// 初始化socket库
			if (!AfxSocketInit())
			{
				AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
			}
			else
			{
				TopicParam tp;
				tp.bIsPublished=m_data.m_bIsPublish;
				_tcscpy(tp.strTopicID,m_data.m_strTopicID);
				//发送广播消息，通知文章已发布
				CSocket m_hSocket;	
				BOOL bSuccess=FALSE;			
				if(m_hSocket.Create(2330,SOCK_DGRAM))
				{
					bSuccess=m_hSocket.SendTo((LPTOPICPARAM)&tp,sizeof(tp),theApp.GetNetworkPortNum(),theApp.GetBroadcastIP())!=SOCKET_ERROR;			     			 
				}
			}			
			//设置本地信息          			
			SpecShowInTree(theApp.GetSourceTree(),m_data.m_bIsPublish);				
			SaveToDB(*theApp.GetDB(),TRUE);
		}
	}

	return bRet;
}

BOOL CTopic::DeleteFromServer()
{
	CWDMDatabase* pDatabase=GetDatabase();
	ASSERT(pDatabase!=NULL);

	CADODatabase adoServer;	

	if( pDatabase->GetAdoDatabase(adoServer) == FALSE )
		return FALSE;

	BOOL bRet=TRUE;
	CString strSql;
	// 取得远程服务器图片存储参数
	COptionsData::OptionsData::DBServerData data;
	pDatabase->GetDBServerData(data);

	m_strRemoteResourceDir = data.m_strResourceDir;
	m_strRemoteResourcePath = data.m_strResourcePath;
    //执行数据表指定的操作	
	CWDMTable* pTable=NULL;
	CWDMOperator* pOperator=NULL;
	CTypedPtrArray<CPtrArray,CWDMTable*> tables;
	//删除后处理数据表	
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Post,FALSE);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);

		pOperator=pTable->GetOperator(CWDMOperator::opDelete);
		if(pOperator==NULL || !pOperator->IsValid())
			continue;

		strSql=GetSqlString(pTable,CWDMOperator::opDelete);

		bRet &= adoServer.Execute(strSql);
		
	}
	//删除图片数据表	
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Picture,FALSE);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);

		pOperator=pTable->GetOperator(CWDMOperator::opDelete);
		if(pOperator==NULL || !pOperator->IsValid())
			continue;

		strSql=GetSqlString(pTable,CWDMOperator::opDelete);

		bRet &= adoServer.Execute(strSql);

	}
	//删除文章数据表	
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Topic,FALSE);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);

		pOperator=pTable->GetOperator(CWDMOperator::opDelete);
		if(pOperator==NULL || !pOperator->IsValid())
			continue;

		strSql=GetSqlString(pTable,CWDMOperator::opDelete);

		bRet &= adoServer.Execute(strSql);

	}
	//删除预处理数据表	
	pDatabase->GetWDMTables(tables,CWDMTable::Table_Pre,FALSE);
	for(int i=0;i<tables.GetCount();i++)
	{
		pTable = tables.GetAt(i);

		pOperator=pTable->GetOperator(CWDMOperator::opDelete);
		if(pOperator==NULL || !pOperator->IsValid())
			continue;

		strSql=GetSqlString(pTable,CWDMOperator::opDelete);

		bRet &= adoServer.Execute(strSql);

	}
	adoServer.Close();
	//处理资源表类型
	if(GetChildCount()>0)
	{
		//删除远程服务器的图片
		CString strPicPath,strPicUrl;
		CDBServer* pServer = (CDBServer*)pDatabase->GetParent();
		CFileSend* pFileSend = NULL;

		if( pServer->m_data.m_nFileSendType == 0 )
		{
			pFileSend = new CNetShareFileSend(pServer->m_data.m_strFileServerName,
				pServer->m_data.m_strFileServerUserName,
				pServer->m_data.m_strFileServerPassWord);
		}
		else
		{
			pFileSend = new CFtpFileSend(pServer->m_data.m_strFileServerName,
				pServer->m_data.m_strFileServerUserName,
				pServer->m_data.m_strFileServerPassWord,
				pServer->m_data.m_nFileServerPort);
		}
		
		CNewsPicture* pPicture = NULL;

		// 文章缩略图仅需要删除一次
		BOOL bTopicThumb = !m_data.m_strThumb.IsEmpty();
		POSITION pos=GetFirstChildPosition();

		while (pos)
		{
			pPicture=(CNewsPicture*)GetNextChild(pos);
			if(pPicture!=NULL)
			{
				strPicUrl=pPicture->m_data.m_strURL;
				
				strPicPath.Format("%s\\%s",m_strRemoteResourceDir,strPicUrl);

				try
				{
					pFileSend->DeleteFileFromServer(strPicPath);
					// 删除文章缩略图和图片缩略图
					if( pServer->m_data.m_bThumb )
					{
						// 删除图片缩略图
						CCommonFunction::Replace(strPicPath,"/","\\");
						strPicPath.Insert(strPicPath.ReverseFind('\\')+1,pServer->m_data.m_strThumbName);
						pFileSend->DeleteFileFromServer(strPicPath);
						// 删除文章缩略图
						if( bTopicThumb )
						{
							strPicPath.Format("%s/%s",m_strRemoteResourceDir,m_data.m_strThumb);			
							
							bTopicThumb = !pFileSend->DeleteFileFromServer(strPicPath);
						}
					}
				}
				catch (...)
				{
					CString str;
					str.Format("删除远程服务器图片错误，路径:%s",strPicUrl);
					static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
				}
			}
		}
		delete pFileSend;
		pFileSend = NULL;
	}	

    //更新文章信息
	if(bRet)
	{	
		CLogs::WriteLogs("删除："+GetDesc(),CLogs::log_delete_topic);

		if(m_data.m_bIsPublish==TRUE)
		{	
			m_data.m_bIsPublish=FALSE;
			// 初始化socket库
			if (!AfxSocketInit())
			{
				AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
			}
			else
			{
				TopicParam tp;
				tp.bIsPublished=m_data.m_bIsPublish;
				_tcscpy(tp.strTopicID,m_data.m_strTopicID);
				//发送广播消息，通知文章已删除
				CSocket m_hSocket;	
				BOOL bSuccess=FALSE;			
				if(m_hSocket.Create(2330,SOCK_DGRAM))
				{
					bSuccess=m_hSocket.SendTo((LPTOPICPARAM)&tp,sizeof(tp),theApp.GetNetworkPortNum(),theApp.GetBroadcastIP())!=SOCKET_ERROR;			     			 
				}
			}
			//设置本地信息			           			
			SpecShowInTree(theApp.GetSourceTree(),m_data.m_bIsPublish);				
			SaveToDB(*theApp.GetDB(),FALSE);
		}
	}
	return bRet;
}
CString CTopic::GetTopicUnitValue(TopicUnit nUnitValue)
{
	CString strValue=_T("");
	
	switch( nUnitValue )
	{
	case T_HEADTITLE:
		strValue = m_data.m_strHeadTitle;
		break;
	case T_TITLE:
		strValue=m_data.m_strTitle;
		CCommonFunction::Replace(strValue,_T("[分页]"),"");
		break;
	case T_MAINTITLE:
		strValue=m_data.m_strMainTitle;
		break;
	case T_SUBTITLE:
		strValue=m_data.m_strSubTitle;
		break;
	case T_TIME:
		{
			COleDateTime dt;
			if(dt.ParseDateTime(m_data.m_strTime))
				strValue=m_data.m_strTime;
			else
				strValue = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case T_SOURCE:
		strValue=m_data.m_strSource;
		break;
	case T_AUTHOR:
		strValue=m_data.m_strAuthor;
		break;
	case T_KEYWORD:
		strValue=m_data.m_strKeyWord;
		break;
	case T_SUBJECT:
		strValue=m_data.m_strSubject;
		break;
	case T_CONTENT:
		{		
			CStringArray strImgList;

			CRegex::Search(m_data.m_strContent,_T("(<\\s*IMG\\s+[^>]*?src\\s*=\\s*.*?>)"),strImgList);

			//对图片按索引顺序存放
			int i=0;
			CArray<CNewsPicture*,CNewsPicture*> pictures;

			POSITION pos=GetFirstChildPosition();
			while (pos)
			{
				CNewsPicture* pPicture=(CNewsPicture*)GetNextChild(pos);
				if(pPicture!=NULL)
				{
					for(i=0;i<pictures.GetSize();i++)
					{
						if(pPicture->m_data.m_nStartPos<pictures.GetAt(i)->m_data.m_nStartPos)
						{
							pictures.InsertAt(i,pPicture);
							break;
						}
					}
					if(i==pictures.GetSize())
						pictures.Add(pPicture);
				}
			}

			CString strTemp,strContent,strImg,strImgContent,strImageURL;	
			strContent=m_data.m_strContent;

			for(i=0;i<pictures.GetSize();i++)
			{				
				strImageURL.Format("%s%s",m_strRemoteResourcePath,pictures.GetAt(i)->m_data.m_strURL);

				CCommonFunction::Replace(strImageURL,"\\","/");

				BOOL bAdd = TRUE;
				if( strImgList.GetSize()>0 )
				{
					strTemp=pictures.GetAt(i)->m_data.m_strURL;
					CCommonFunction::Replace(strTemp,"\\","/");

					for(int i=0;i<strImgList.GetSize();i++)
					{
						if(strImgList.GetAt(i).Find(strTemp)!=-1)
						{
							strImg.Format("<p class=\"AutoSubmitImg\"><img src=\"%s\" border=0></p>",strImageURL);
							CCommonFunction::Replace(strContent,strImgList.GetAt(i),strImg);

							bAdd = FALSE;
						}
					}
				}
				if( bAdd )
				{
					strTemp.Format("<p class=\"AutoSubmitImg\"><img src=\"%s\" border=0></p>",strImageURL);				
					strImgContent=strImgContent+strTemp;				
					strTemp.Format("<p class=\"AutoSubmitImgExp\">%s</p>",pictures.GetAt(i)->m_data.m_strExplain);
					strImgContent=strImgContent+strTemp;	
				}			
			}		
			//转换内容为指定格式
			CCommonFunction::Replace(strContent,_T("\r\n"),_T("</p><p class=\"AutoSubmit\">"));
			strContent=_T("<p class=\"AutoSubmit\">")+strContent+_T("</p>");
			//添加不保存位置的图片
			strContent=strImgContent+strContent;

			strValue=strContent;          
		}
	    break;
	case T_BIGCLASS:
		strValue=m_data.m_strBigClass;
		break;
	case T_SMALLCLASS:
		strValue=m_data.m_strSmallClass;
		break;
	case T_SERVERCOLUMNID:
		AfxExtractSubString(strValue,m_data.m_strServerColumnIDs,0,' ');
		break;
	case T_TOPICID:
        strValue=m_data.m_strTopicID;
		break;
	case T_SERVERCHANNELID:
		{
			AfxExtractSubString(strValue,m_data.m_strServerColumnIDs,0,' ');
			strValue=GetServerChannelIDByColumnID(strValue);
		}
		break;
	case T_PRIMARY:
		strValue.Format("%d",(int)m_data.m_bIsPrimary);
		break;
	case T_WEIGHT:
		strValue.Format("%d",m_data.m_nWeight);
		break;
	//case Elite:
	//	strValue.Format("%d",(int)m_data.m_bIsElite);
	//	break;
	//case OnTop:
	//	strValue.Format("%d",(int)m_data.m_bIsOnTop);
	//	break;
	case T_RESOURCENUM:
		strValue.Format("%d",GetChildCount());
		break;
	case T_TIMESTAMP:
		strValue = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
		break;
	case T_LINKURL:
		strValue = m_data.m_strTopicID;
		break;
	case T_EDITOR:
		strValue = theApp.GetCurUserDesc();
		break;
	case T_SERVERAUTOID:
		strValue = m_data.m_strServerAutoID;
		break;
	case T_THUMB:
		strValue = m_data.m_strThumb;
		break;
	case T_OLDTITLE:
		strValue = m_data.m_strOldTitle;
		break;
	case T_OLDURL:
		strValue = m_data.m_strURL;
		break;
	}
	strValue.Trim();
    
	return strValue;
}
CString CTopic::GetResourceUnitValue(TopicUnit nUnitValue,CNewsPicture* pPicture)
{
	CString strValue=_T("");

	switch( nUnitValue )
	{
	case T_TOPICID:
		strValue=m_data.m_strTopicID;
		break;
	case T_RESOURCEID://Picture attribute	
		strValue=pPicture->m_data.m_strPictureID;
		break;
	case T_RESOURCEEXPLAIN:
		strValue=pPicture->m_data.m_strExplain;
		break;
	case T_RESOURCEURL:
		{
			strValue = pPicture->m_data.m_strURL;
			CCommonFunction::Replace(strValue,"\\","/");
		}
		break;
	case T_RESOURCENAME:
		{
			strValue=pPicture->m_data.m_strURL;
			CCommonFunction::Replace(strValue,"/","\\");
			int i=strValue.ReverseFind('\\');
			if(i!=-1)
				strValue=strValue.Mid(i+1);
		}
		break;
	case T_RESOURCEEXT:
		{
			strValue=pPicture->m_data.m_strURL;
			strValue=strValue.Mid(strValue.ReverseFind('.')+1);	
		}
		break;
	case T_TIMESTAMP:
		strValue = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
		break;
	case T_EDITOR:
		strValue=theApp.GetCurUserDesc();
		break;
	case T_RESOURCESIZE:
		strValue.Format("%d",pPicture->m_data.m_nPictureSize);
		break;
	case T_SERVERAUTOID:
		strValue = pPicture->m_data.m_strServerAutoID;
		break;
	case T_SERVERCOLUMNID:
		AfxExtractSubString(strValue,m_data.m_strServerColumnIDs,0,' ');
		break;
	case T_ISTHUMB:
		strValue.Format("%d",(int)pPicture->m_data.m_bThumb);
		break;
	}
	strValue.Trim();

	return strValue;
}
CString CTopic::GetSqlString(CWDMTable* pTable,CWDMOperator::OperatorType type)
{
     CString strSql;
     switch(type)
	 {
	 case CWDMOperator::opSave:
		 strSql.Format("select * from %s where ",pTable->GetDesc());
		 break;
	 case CWDMOperator::opDelete:
		 strSql.Format("delete from %s where ",pTable->GetDesc());
		 break;
	 default:			
		 ASSERT(FALSE);
	 }

	 CWDMOperator* pOperator=pTable->GetOperator(type);
     POSITION pos=pOperator->GetFirstChildPosition();
	 while (pos)
	 {
		 CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pOperator->GetNextChild(pos);
		 if(pUnit!=NULL && pUnit->m_data.m_bIsKey)
		 {
			 strSql+=pUnit->m_data.m_strUnitName+_T("='");
			 strSql+=GetUnitValue(pUnit,NULL)+_T("' and ");
		 }
	 }
	 strSql=strSql.Left(strSql.GetLength()-5);

	 return strSql;
}
BOOL CTopic::GetServerOperUnit(CString& strOperUnitName,CWDMTable* pTable,TopicUnit nUnitValue)
{
	 BOOL bRet=FALSE;
	 ASSERT(pTable);

	 CWDMOperator* pOperator=pTable->GetOperator(CWDMOperator::opSave);
     POSITION pos=pOperator->GetFirstChildPosition();
	 while (pos)
	 {
		 CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)pOperator->GetNextChild(pos);
		 if(pUnit!=NULL && (pUnit->ContainTopicUnit(nUnitValue) ))
		 {
             strOperUnitName=pUnit->m_data.m_strUnitName;
			 bRet=TRUE;
			 break;
		 }
	 }

	 return bRet;
}
void CTopic::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);

	CString strText;
	CStringArray strList;

	if(bSelf)
	{
		strText=_T("文章标题,500;图片数量,100;是否发布,100;文章时间,150");				  
		pListCtrl->SetHeadings(strText);		

		strList.Add(GetDesc());
		strText.Format("%d",GetChildCount());
		strList.Add(strText);
		strList.Add(m_data.m_bIsPublish?_T("已发布"):_T("未发布"));
		strList.Add(m_data.m_strTime);
		
		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
		
	}
	else
	{
        strText=_T("序号,60;图片路径,150;图片说明,500");
		pListCtrl->SetHeadings(strText);

		POSITION pos=GetFirstChildPosition();
		int i=0;
		while (pos)
		{
			CNewsPicture* pPicture=(CNewsPicture*)GetNextChild(pos);
			if(pPicture!=NULL)
			{
                strList.RemoveAll();

				strText.Format("%d",++i);
				strList.Add(strText);
				strList.Add(pPicture->GetDesc());
				strList.Add(pPicture->m_data.m_strExplain);

				pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pPicture);
			}
		}
	}
}
void CTopic::DelSubObject(BOOL bDelFromDB /*=TRUE*/ )
{
	if(bDelFromDB)
        DeleteFromDB(*theApp.GetDB(),FALSE);
	
	theApp.GetSourceTree()->DeleteAllChildItem(GetRefItem());
	UpdateTopic();
}
void CTopic::UpdateTopic()
{
	CStringArray arImg;
	CUIntArray   arDelImg;

	CString str,strContent=m_data.m_strContent;	
	
	CRegex::Search(strContent,_T("(<\\s*IMG\\s+[^>]*?src\\s*=\\s*.*?>)"),arImg);
	int i=0;
    for( i=0;i<arImg.GetSize();i++)
    {
		arDelImg.Add(1);
    }

    POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		CNewsPicture* pPicture=(CNewsPicture*)GetNextChild(pos);
		if(pPicture!=NULL)
		{
            for(i=0;i<arImg.GetSize();i++)
			{
				if(arImg.GetAt(i).Find(pPicture->m_data.m_strURL)!=-1)
					arDelImg.SetAt(i,0);
			}
		}
	}
	BOOL bAdjust=FALSE;
	for(i=0;i<arDelImg.GetSize();i++)
	{
		if(arDelImg.GetAt(i))
		{
			bAdjust=TRUE;
			CCommonFunction::Replace(m_data.m_strContent,arImg.GetAt(i),"");
		}
	}
    if( bAdjust )
	{
		CExtract e;
		std::string s=m_data.m_strContent;
		e.GetPureText(s);
		m_data.m_strContent=s.c_str();
	}
}
TiXmlNode*  CTopic::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_topic"));

	pTemp->SetAttribute(_T("topic_id"),m_data.m_strTopicID);
	pTemp->SetAttribute(_T("entry_id"),m_data.m_strEntryID);
	pTemp->SetAttribute(_T("url"),m_data.m_strURL);
	pTemp->SetAttribute(_T("title"),m_data.m_strTitle);
	pTemp->SetAttribute(_T("old_title"),m_data.m_strOldTitle);
	pTemp->SetAttribute(_T("main_title"),m_data.m_strMainTitle);
    pTemp->SetAttribute(_T("head_title"),m_data.m_strHeadTitle);
    pTemp->SetAttribute(_T("sub_title"),m_data.m_strSubTitle);
	pTemp->SetAttribute(_T("source"),m_data.m_strSource);
    pTemp->SetAttribute(_T("time"),m_data.m_strTime);
    pTemp->SetAttribute(_T("author"),m_data.m_strAuthor);
	pTemp->SetAttribute(_T("keyword"),m_data.m_strKeyWord);
    pTemp->SetAttribute(_T("subject"),m_data.m_strSubject);
	pTemp->SetAttribute(_T("content"),m_data.m_strContent);
    pTemp->SetAttribute(_T("is_publish"),(int)m_data.m_bIsPublish);
    pTemp->SetAttribute(_T("is_sendto"),(int)m_data.m_bIsSendto);
	pTemp->SetAttribute(_T("is_primary"),(int)m_data.m_bIsPrimary);
	//pTemp->SetAttribute(_T("is_elite"),(int)m_data.m_bIsElite);
	//pTemp->SetAttribute(_T("is_ontop"),(int)m_data.m_bIsOnTop);
	pTemp->SetAttribute(_T("download_date"),m_data.m_timeDownload.Format("%Y-%m-%d %H:%M:%S"));
    pTemp->SetAttribute(_T("server_column_ids"),m_data.m_strServerColumnIDs);
	pTemp->SetAttribute(_T("weight"),m_data.m_nWeight);
	pTemp->SetAttribute(_T("server_auto_id"),m_data.m_strServerAutoID);
	pTemp->SetAttribute(_T("thumb"),m_data.m_strThumb);
	
	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CTopic::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;

	for(pElement=pNode->FirstChildElement(_T("wdm_picture"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("picture_id"));
		if(!strID.IsEmpty())
		{
			CNewsPicture* pPicture=(CNewsPicture*)FindSubObject(OBJECT_TYPE_PICTURE,strID);
			if(pPicture==NULL)
			{
				pPicture=new CNewsPicture;
				pPicture->m_data.m_strPictureID=strID;
				pPicture->AddTo(this);
			}				
			pPicture->m_data.m_strTopicID=pElement->Attribute(_T("topic_id"));
			pPicture->m_data.m_strURL=pElement->Attribute(_T("url"));
			pPicture->m_data.m_strOldURL=pElement->Attribute(_T("old_url"));
			pPicture->m_data.m_strExplain=pElement->Attribute(_T("explain"));
			pElement->QueryIntAttribute(_T("start_pos"),&pPicture->m_data.m_nStartPos);
			pElement->QueryIntAttribute(_T("end_pos"),&pPicture->m_data.m_nEndPos);
			pElement->QueryIntAttribute(_T("picture_size"),&pPicture->m_data.m_nPictureSize);
			pPicture->m_data.m_strServerAutoID=pElement->Attribute(_T("server_auto_id"));
			pElement->QueryIntAttribute(_T("is_thumb"),&(int)pPicture->m_data.m_bThumb);

			pPicture->SetID(strID);
			pPicture->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
			pPicture->StreamIn(pElement,bChild,pProgressBar);
		}			
	}
	return TRUE;
}

TiXmlNode* CTopic::Export(CString& strDir,TiXmlNode* pNode,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::Export(strDir,NULL,FALSE,pProgressBar);

	TiXmlDocument doc;	
	TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T(""));
	doc.LinkEndChild(pDecl);
	
	TiXmlElement * pRoot=new TiXmlElement(_T("Root"));
    doc.LinkEndChild(pRoot);

    TiXmlElement * pTemp=new TiXmlElement(_T("Article"));
	pTemp->SetAttribute(_T("id"),GetID());
	pTemp->SetAttribute(_T("标题"),m_data.m_strTitle);
	//Process pictures	
	int i=0;
	CArray<CNewsPicture*,CNewsPicture*> pictures;
	
	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		CNewsPicture* pPicture=(CNewsPicture*)GetNextChild(pos);
		if(pPicture!=NULL)
		{
			for(i=0;i<pictures.GetSize();i++)
			{
				if(pPicture->m_data.m_nStartPos<pictures.GetAt(i)->m_data.m_nStartPos)
				{
					pictures.InsertAt(i,pPicture);
					break;
				}
			}
			if(i==pictures.GetSize())
				pictures.Add(pPicture);
		}
	}
	CString strSource,strDst;
	CString strImgContent=_T(""),strTemp,strTemp1;
	for(i=0;i<pictures.GetSize();i++)
	{
		//图片复制到指定的目录下
		strSource.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),pictures.GetAt(i)->m_data.m_strURL);
	    int nIndex=pictures.GetAt(i)->m_data.m_strURL.ReverseFind('\\');
		strTemp=pictures.GetAt(i)->m_data.m_strURL;
		if(nIndex!=-1)
			strTemp=pictures.GetAt(i)->m_data.m_strURL.Mid(nIndex+1);		
		strDst.Format("%s\\%s",strDir,strTemp);		
		CCommonFunction::CreateDir(strDst);
		CopyFile(strSource,strDst,FALSE);
		//图片信息写入到文章内容
// 		CBaseObject* pTopColumn=BackTo(OBJECT_TYPE_COLUMN);
// 		CBaseObject* pParent=pTopColumn->GetParent();		
// 		for(;pParent && pParent->GetType()!=OBJECT_TYPE_HELPER;pParent=pParent->GetParent())
// 			pTopColumn=pParent;
// 
// 		strTemp1.Format("<div align=center><img border=0 src=\"data/%s/Articles/%s/%s\"></div>",
// 			pTopColumn->GetID(),GetParent()->GetID(),strTemp);
// 		
// 		strImgContent=strImgContent+strTemp1;
// 		
// 		strTemp1.Format("<div align=center>%s</div>",pictures.GetAt(i)->m_data.m_strExplain);
// 		strImgContent=strImgContent+strTemp1;
		
	}
//	strImgContent=strImgContent+_T("\r\n")+m_data.m_strContent;
	pTemp->SetAttribute(_T("内容"),	m_data.m_strContent);
	pTemp->SetAttribute(_T("来源"),m_data.m_strSource);
	pTemp->SetAttribute(_T("发布时间"),m_data.m_strTime);
	pTemp->SetAttribute(_T("ParentID"),m_data.m_strEntryID);

	pRoot->LinkEndChild(pTemp);
	//Create file
	CString strFile;
	strFile.Format("%s\\%s.xml",strDir,GetID());
	CCommonFunction::CreateDir(strFile);
	doc.SaveFile(strFile);

	return NULL;
}

BOOL CTopic::Match(CString strTarget,int nType,BOOL bMatchAll)
{
	BOOL bRet=FALSE;
	if(nType==0 )
		return bRet;

    switch(nType)
	{
	case 1:    //title
		if(bMatchAll)
		{
            bRet=m_data.m_strTitle.CompareNoCase(strTarget)==0;
		}
		else
		{
			bRet=m_data.m_strTitle.Find(strTarget)!=-1;
		}		
		break;
	case 2:    //content
		if(!m_data.m_strContent.IsEmpty())
		{
			if(bMatchAll)
			{
				bRet=m_data.m_strContent.CompareNoCase(strTarget)==0;
			}
			else
			{
				bRet=m_data.m_strContent.Find(strTarget)!=-1;
			}
		}		
		break;
	case 3:    //title and content
		if(bMatchAll)
		{
            bRet=m_data.m_strTitle.CompareNoCase(strTarget)==0;
		}
		else
		{
			bRet=m_data.m_strTitle.Find(strTarget)!=-1;
		}
	
		if(!m_data.m_strContent.IsEmpty())
		{
			if(bMatchAll)
			{
				bRet |=m_data.m_strContent.CompareNoCase(strTarget)==0;
			}
			else
			{
				bRet |=m_data.m_strContent.Find(strTarget)!=-1;
			}
		}		
		break;
	}
	//Expand the item
	if(bRet)
	{
        CCustomTree* pTree=(CCustomTree*)GetTreeWnd();
		if(pTree)
		{
			pTree->SelectObject(this);
		}

	}
	return bRet;
}

void CTopic::StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamOutTxt(stream,FALSE,pProgressBar);

//	stream <<_T("引标: ")<<m_data.m_strHeadTitle<<endl;
	stream <<_T("标题: ")<<m_data.m_strTitle<<endl;
//	stream <<_T("副标: ")<<m_data.m_strSubTitle<<endl;
	stream <<_T("时间: ")<<m_data.m_strTime<<endl;
	std::string s(m_data.m_strContent);
	CExtract e;
	e.GetPureText(s,FALSE);
	stream <<_T("正文: ")<<s.c_str()<<endl;
	stream <<_T("来源: ")<<m_data.m_strSource<<endl;    
    stream <<endl;
}

void CTopic::ProcessContent(CString& strContent,CString strKeyWord,int nType)
{	
	switch(nType)
	{	
	case 0: //删除前面所有内容		
	case 1: //删除后面所有内容		
	case 2: //删除本身所有内容
		if(!strKeyWord.IsEmpty())
		{
		    strContent=CRegex::Filter(strContent,strKeyWord,(CRegex::FILTER_TYPE)nType);
		}
		break;
	case 3: //替换
		strContent=strKeyWord;
		break;
	default:
		ASSERT(FALSE);
	}
}

void CTopic::AlterTopic(int nTopicUnit,int nProcessType,const CString& strValue)
{
	switch(nTopicUnit)
	{
	case 0: //引标
		ProcessContent(m_data.m_strHeadTitle,strValue,nProcessType);
		break;
	case 1: //标题
		ProcessContent(m_data.m_strTitle,strValue,nProcessType);
		break;
	case 2: //缩标
		ProcessContent(m_data.m_strMainTitle,strValue,nProcessType);
		break;
	case 3: //副标
		ProcessContent(m_data.m_strSubTitle,strValue,nProcessType);
		break;
	case 4: //时间
		ProcessContent(m_data.m_strTime,strValue,nProcessType);
		break;
	case 5: //作者
		ProcessContent(m_data.m_strAuthor,strValue,nProcessType);
		break;
	case 6: //来源
		ProcessContent(m_data.m_strSource,strValue,nProcessType);
		break;
	case 7: //关键字
		ProcessContent(m_data.m_strKeyWord,strValue,nProcessType);
		break;
	case 8: //专题词
		ProcessContent(m_data.m_strSubject,strValue,nProcessType);
		break;
	case 9: //内容
		{
			ProcessContent(m_data.m_strContent,strValue,nProcessType);
			
			CExtract e;
			std::string s = m_data.m_strContent;
			e.GetPureText(s);
			m_data.m_strContent = s.c_str();
		}				
		break;
	default:
		ASSERT(FALSE);
	}		
	SaveToDB(*theApp.GetDB(),FALSE);
}

CString CTopic::GetUnitValue(CWDMOperatorUnit* pUnit,CADORecordset* pRS,CNewsPicture* pPicture/* =NULL */)
{
	int i = 0;
	CString strUintCombo,strValue,strValues;
	while( AfxExtractSubString(strUintCombo,pUnit->m_data.m_strUnitCombos,i,_T(';')))
	{		
		// Prepare to move to the next substring
		i++;
		if( strUintCombo.IsEmpty())
			continue;

		strValue.Empty();
		// The data format is
		// unit vale1,default value1,combo value1;unit vale2,default value2,combo value2;...
		CString strUnit,strDefault,strCombo; // individual name and value elements
		int nUnit;
		int iCombo;
		// Attempt to extract the name and value element from the pair
		if (AfxExtractSubString(strUnit, strUintCombo, 0, _T(',')) && 
			AfxExtractSubString(strDefault, strUintCombo, 1, _T(','))&&
			AfxExtractSubString(strCombo, strUintCombo, 2, _T(',')))
		{
			nUnit = atoi(strUnit);
			iCombo = atoi(strCombo);

			switch( nUnit )
			{
			case T_DEFAULTVALUE:
				strValue = strDefault;
				break;
			case T_REFERVALUE:
				if( !strDefault.IsEmpty() )
				    m_mapCache.Lookup(strDefault,strValue);
				break;
			case T_READDATAOFDB:
				if( pRS )
					pRS->GetFieldValue(pUnit->m_data.m_strUnitName,strValue);
				break;
			default:
				strValue = ( pPicture == NULL )?GetTopicUnitValue((TopicUnit)nUnit):GetResourceUnitValue((TopicUnit)nUnit,pPicture);
			}
			// 要素再处理优先处理的要素			
			switch( iCombo )
			{
			case 3:  // 截取前N个字符
				{
					int num = atoi(strDefault);
					strValue = CCommonFunction::GetPureText(strValue);
					strValue = strValue.Left(strValue.GetLength()>num?num:strValue.GetLength());
				}		
				break;
			case 6:  // 引用缓存值
				if( !strDefault.IsEmpty() )
					m_mapCache.Lookup(strDefault,strValue);
				break;
			case 14: // 空值清空
				if( strValue.IsEmpty() )
					strValues.Empty();
				break;
			default:
				{
					//其它再处理
					strValue = ProcessUnitValue(strValue,pUnit,(TopicUnit)nUnit,iCombo,pPicture);
				}
			}
		}
		strValues += strValue;
	}
	// 如果值不空，缓存备用
	if( pUnit->m_data.m_bCache && !strValues .IsEmpty())
	{
		m_mapCache.SetAt(pUnit->GetKeyString(),strValues);
	}
	return strValues;
}


CString CTopic::ProcessUnitValue(CString strValue,CWDMOperatorUnit* pUnit,TopicUnit nUnitValue,int iProcessType,CNewsPicture* pPicture/* =NULL */)
{
	//CString CUnitInfoDlg::unitCombos[]={ 
	//	_T("无"),                    
	//	_T("首字母"),                
	//	_T("计算MD5值"),             
	//	_T("截取前N个字符"),       
	//	_T("转换成Unix时间戳"),      
	//	_T("新建GUID"),              
	//	_T("引用缓存值"),            
	//	_T("转换成URL参数"),         
	//	_T("获取本机IP"),            
	//	_T("获取文件MIME类型"),      
	//	_T("去除文件名")，
	//	_T("引用自增ID"),
	//  _T("数值递增"),
	//  _T("时间参数转换"),
	//  _T("空值清空")
	//};

	CString strRet;

	switch( iProcessType )
	{
	case 0:
		strRet = strValue;
		break;
	case 1:
		strRet = CCommonFunction::ConvertChineseToPyt(strValue,TRUE,'\0');
		break;
	case 2:
		{
			CMD5 md5;
			md5.GenerateMD5((unsigned char*)(const char*)strValue,strValue.GetLength());

			strRet = md5.ToString().c_str();
		}
		break;
	//case 3:
	//	{
	//		strValue = CCommonFunction::GetPureText(strValue);
	//		strRet = strValue.Left(strValue.GetLength()>200?200:strValue.GetLength());
	//	}		
	//	break;
	case 4:
		strRet = CCommonFunction::Time2UnixTimeStamp(strValue);
		break;
	case 5:
		strRet = CCommonFunction::AllocateID();
		break;
	//case 6:
	//	m_mapCache.Lookup(pUnit->m_data.m,strRet);
	//	break;	
	case 7:
		{
			CWDMTable* pTable = (CWDMTable*)pUnit->BackTo(OBJECT_TYPE_TABLE);
			CString strFieldName;

			if( GetServerOperUnit(strFieldName,pTable,nUnitValue) )
				strRet.Format("%s=%s",strFieldName,strValue);
		}
		break;
	case 8:
		{
			CGetHostIP ip;
			strRet = ip.GetHostIP();
		}
		break;
	case 9:
		{			
			strValue.Insert(0,'.');
			strRet = CCommonFunction::GetMimeType(strValue);
		}
		break;
	case 10:
		{	
			strRet = strValue.Mid(0,strValue.ReverseFind('/')+1);
		}
		break;
	case 11:
		strRet = ( (nUnitValue == T_TOPICID) || pPicture == NULL )?m_data.m_strServerAutoID:pPicture->m_data.m_strServerAutoID;
		break;
	case 12:
		{
			DWORD64 uValue = 0ULL;
			char strValue[65];
			uValue = _tcstoui64((LPCTSTR)strValue,0,10);
			uValue++;

			_ui64tot_s(uValue,strValue,65,10);
			strRet = strValue;
		}
		break;
	case 13:
		strRet = CRegexWDM::TimeFormatStrConvert(strValue);
		break;
	}
	return strRet;
}

CString CTopic::GetServerAutoID(CWDMTable* pTable,CADODatabase& ado)
{
	unsigned long nID=0;
	CString strID;
	CString strSql;

	CWDMDatabase* pDatabase = (CWDMDatabase*)pTable->BackTo(OBJECT_TYPE_DATABASE);
	ASSERT(pDatabase!=NULL);

	COptionsData::OptionsData::DBServerData data;
	pDatabase->GetDBServerData(data);					

	switch ( data.m_nType )
	{				
	case 0:   // SQL Server
		{
			strSql.Format("select IDENT_CURRENT('%s') as auto_increment",pTable->GetDesc());
		}
		break;
	case 1:  // Oracle
		{
			strSql.Format("select seq_'%s'.currval as auto_increment from dual",pTable->GetDesc());
		}
		break;
	case 2:  //MySQL
		{
			//strSql.Format("SHOW TABLE STATUS WHERE name ='%s'",pTable->GetDesc());
			strSql = _T("select LAST_INSERT_ID( ) as auto_increment");
		}
		break;
	}						

	ASSERT(ado.IsOpen());
	
	CADORecordset rsServer(&ado);
	if(rsServer.Open(strSql,CADORecordset::openQuery))
	{
		rsServer.GetFieldValue(_T("auto_increment"),nID);
		rsServer.Close();								
	}
	
	strID.Format("%lu",nID);

	return strID;
}

CString CTopic::GetServerChannelIDByColumnID(CString strColumnID)
{
	CString strID;

	CBaseObject* pColumn=theApp.GetServerColumnRoot()->FindSubObject(OBJECT_TYPE_SERVER_COLUMN,strColumnID,TRUE);
	if(pColumn!=NULL)
	{
		CBaseObject* pChannel=pColumn;
		do 
		{
			strID=pChannel->GetID();
			pChannel=pChannel->GetParent();			
		} 
		while( pChannel!=NULL && pChannel->GetID()!=GetServerDatabaseID() );
	}
	return strID;
}

CString CTopic::GetServerDatabaseID()
{
	CString strID;

	CColumn* pColumn=(CColumn*)BackTo(OBJECT_TYPE_COLUMN);
	// find the top column
	while (pColumn!=NULL && pColumn->m_data.m_strParentID.CompareNoCase((CString)NULL_ID)!=0)
	{
		pColumn=(CColumn*)pColumn->BackTo(OBJECT_TYPE_COLUMN);
	}
	ASSERT(pColumn);

	strID = pColumn->m_data.m_strDatabaseID;	

	return strID;
}