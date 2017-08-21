// ExtractSourceTree.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ExtractSourceTree.h"
#include "Column.h"
#include "schedule.h"
#include "Topic.h"
#include "WorkFile.h"
#include "WorkUrl.h"
#include "RuleGroupDlg.h"
#include "urldlg.h"
#include "DBServerDlg.h"
#include "schedulelistdlg.h"
#include "DatabaseInfoDlg.h"
#include "progressbar.h"
#include "ExportDlg.h"
#include "InfoDlg.h"
#include "ColumnClassInfoDlg.h"
#include <fstream>
#include <stack>
#include "WorkTopic.h"
#include "AlterTopicUnitDlg.h"
#include "Extract.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtractSourceTree

CExtractSourceTree::CExtractSourceTree():m_objectHelper(HELPER_SOURCE)
{
     m_objectHelper.CanDelete()=FALSE;	 
	 EnableMultiSelect();
	 //Init the socket 
	 m_pReceiveSocket=new CReceiveSocket(this);
	 m_pReceiveSocket->Create(theApp.GetNetworkPortNum(),SOCK_DGRAM);
}

CExtractSourceTree::~CExtractSourceTree()
{
 	delete m_pReceiveSocket;
}


BOOL CExtractSourceTree::DeleteSelection(BOOL bDelSelf/* =TRUE */)
{
	BOOL bRet=TRUE;
	CPtrList list;

	if(IsMultiSelect())
		GetMultiSelectedObjects(list);
	else
		list.AddTail(GetSelectedObject());

	CBaseObject *pOB;
	
	if(theApp.DBIsOpen())
	{
		if(MessageBox("确认要删除所选内容吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			POSITION pos=list.GetHeadPosition();
			while (pos)
			{
				pOB=(CBaseObject*)list.GetNext(pos);
                bRet&=pOB->DeleteFromDB(*theApp.GetDB(),bDelSelf);				
			}
			
		}
		else
			return FALSE;
		
		if(!bRet)
		{
			MessageBox("数据删除失败");
		}
		else
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	else
	{
		if(MessageBox("数据库服务器断开,不能真正删除所选内容,继续吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDYES)
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	
	return bRet;
}

void CExtractSourceTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/* =FALSE */)
{
    CWaitCursor wait;
	//清空现有采集源
	POSITION pos=m_objectHelper.GetFirstChildPosition();
	while(pos)
	{
		CBaseObject *pChild=m_objectHelper.GetNextChild(pos);
		if(pChild != NULL)
		{
			DeleteItem(pChild->GetRefItem());
			if(pChild->CanDelete())
			{
				pChild->LeaveFrom(&m_objectHelper);
				delete pChild;
			}
		}
	}

	if(theApp.DBIsOpen() && bReadOfDB)
	{
		CADORecordset rs;
	    CString strSql,str;
		CProgressBar* pBar=NULL;
		//从数据库中重建采集源树
		//strSql=_T("select * from wdm_column where parent_id  = \'0\'");
		strSql.Format("select * from wdm_column where parent_id  = '%s'",NULL_ID);
		
		if(rs.Open(theApp.GetDB()->GetActiveConnection(),strSql,CADORecordset::openQuery))
		{
			if(bShowInfo)
			{
				pBar=new CProgressBar(_T("准备读取采集源数据"),60,5000);
			}

			CColumn *pColumn;
			while(!rs.IsEOF())
			{
				pColumn=new CColumn;
				pColumn->LoadInfo(rs);

				BOOL bFound=FALSE;
				for(int i=0;i<theApp.GetShowTopColumnList().GetSize();i++)
				{
					if(theApp.GetShowTopColumnList().GetAt(i).Compare(pColumn->GetID())==0)
					{
						bFound=TRUE;
						break;
					}
				}	
				if(bFound)
				{					
                    pColumn->AddTo(&m_objectHelper);
				    pColumn->LoadFromDB(*theApp.GetDB(),pBar);
				}
				else
					delete pColumn;
				
				rs.MoveNext();
			}
			rs.Close();

            if(bShowInfo)
			{
			   pBar->Finish("正在初始化采集源树……");	
			   delete pBar;
			   pBar=NULL;
			}
		}
	}
     m_objectHelper.ShowInTreeView(this,TRUE);	
}
BOOL CExtractSourceTree::DoDrop(HTREEITEM hItemDragged, HTREEITEM hItemTarget, int nAction)
{
	CBaseObject *pObDrag,*pObTarget;

	pObDrag=(CBaseObject *)GetItemData(hItemDragged);
	pObTarget= (CBaseObject *)GetItemData(hItemTarget);

	if(!pObTarget->CanAccept(pObDrag,this))
		return FALSE;

	//Delete from tree view
	DeleteItem(hItemDragged);   
    //Add objects to target and show in the tree
	pObDrag->AddTo(pObTarget);
	pObDrag->ShowInTreeView(this,TRUE);

    //Save object
	if(theApp.DBIsOpen())
	    pObTarget->SaveToDB(*theApp.GetDB());

	return TRUE;
}
BEGIN_MESSAGE_MAP(CExtractSourceTree, CCustomTree)
	//{{AFX_MSG_MAP(CExtractSourceTree)
	ON_COMMAND(ID_ROOT_NEW, OnRootNew)
	ON_COMMAND(ID_COLUMN_NEWENTRY, OnColumnNewentry)
	ON_COMMAND(ID_ROOT_SAVE, OnRootSave)
	ON_COMMAND(ID_ROOT_FRESH, OnRootFresh)
	ON_COMMAND(ID_ROOT_DOWNLOAD, OnRootDownload)
	ON_COMMAND(ID_ROOT_DELETE, OnRootDelete)
	ON_COMMAND(ID_COLUMN_DELETE, OnColumnDelete)
	ON_COMMAND(ID_COLUMN_DOWNLOAD, OnColumnDownload)
	ON_COMMAND(ID_COLUMN_EDIT, OnColumnEdit)
	ON_COMMAND(ID_COLUMN_SAVE, OnColumnSave)
	ON_COMMAND(ID_ENTRY_DELETE, OnEntryDelete)
	ON_COMMAND(ID_ENTRY_DOWNLOAD, OnEntryDownload)
	ON_COMMAND(ID_ENTRY_EDIT, OnEntryEdit)
	ON_COMMAND(ID_ENTRY_SAVE, OnEntrySave)
	ON_COMMAND(ID_PICTURE_DELETE, OnPictureDelete)
	ON_COMMAND(ID_PICTURE_EDIT, OnPictureEdit)
	ON_COMMAND(ID_PICTURE_SAVE, OnPictureSave)
	ON_COMMAND(ID_TOPIC_DELETE, OnTopicDelete)
	ON_COMMAND(ID_TOPIC_EDIT, OnTopicEdit)
	ON_COMMAND(ID_TOPIC_PUBLISH, OnTopicPublish)
	ON_COMMAND(ID_TOPIC_SAVE, OnTopicSave)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_COLUMN_DEL_TOPIC, OnColumnDelTopic)
	ON_COMMAND(ID_ROOT_DEL_TOPIC, OnRootDelTopic)
	ON_COMMAND(ID_ENTRY_DEL_TOPIC, OnEntryDelTopic)
	ON_COMMAND(ID_COLUMN_NEW, OnColumnNew)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_COLUMN_SET_DBSERVER, OnColumnSetDbserver)
	ON_COMMAND(ID_COLUMN_SCHEDULE, OnColumnSchedule)
	ON_COMMAND(ID_ENTRY_SCHEDULE, OnEntrySchedule)
	ON_COMMAND(ID_TOPIC_DEL_PICTURE, OnTopicDelPicture)
	ON_COMMAND(ID_COLUMN_FRESH, OnColumnFresh)
	ON_COMMAND(ID_ENTRY_FRESH, OnEntryFresh)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_IMPORT, OnImport)
	ON_COMMAND(ID_ENTRY_SET_COLUMNID, OnEntrySetColumnid)
	ON_COMMAND(ID_ENTRY_PASTE_TOPIC, OnEntryPasteTopic)
	ON_COMMAND(ID_PICTURE_CUT, OnPictureCut)
	ON_COMMAND(ID_TOPIC_CUT, OnTopicCut)
	ON_COMMAND(ID_TOPIC_PASTE_PIC, OnTopicPastePic)
	ON_COMMAND(ID_ALTER_TOPIC_UNIT, OnAlterTopicUnit)
	ON_COMMAND(ID_ENTRY_NEW_TOPIC, OnEntryNewTopic)
	ON_COMMAND(ID_COLUMN_COLLAPSE, OnColumnCollapse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtractSourceTree message handlers

void CExtractSourceTree::OnRootNew() 
{
	// TODO: Add your command handler code here
	CColumn *pColumn=new CColumn;
	
	if(pColumn->EditObject(this,FALSE) == IDOK)
	{
		pColumn->SetID(CCommonFunction::AllocateID());
		pColumn->AddTo(&m_objectHelper);
		pColumn->ShowInTreeView(this,TRUE);

		if(MessageBox(_T("是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		    pColumn->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
		delete pColumn;
}

void CExtractSourceTree::OnColumnNewentry() 
{
	// TODO: Add your command handler code here
	CEntry *pEntry=new CEntry;
	
	pEntry->SetID(CCommonFunction::AllocateID());
	
	if(pEntry->EditObject(this,FALSE) == IDOK)
	{
		pEntry->AddTo(GetSelectedObject());
		pEntry->ShowInTreeView(this,TRUE);

		if(MessageBox(_T("是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		   pEntry->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
	{
		delete pEntry;
	}
}

void CExtractSourceTree::OnRootSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

    CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	CWebDataMiningApp::DBServerType type=CWebDataMiningApp::localServer;
    
	if(theApp.GetNetworkType()==0 && MessageBox(_T("保存到内网服务器吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
		pBar->UpdateBar(_T("正在连接内网,请等待..."),200);
        type=CWebDataMiningApp::wdmServer;
        
		CCommonFunction::OpenInNet();
		Sleep(5000);
		Sleep(3000);

		theApp.ConnectDB(type);
	}
    
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(type),TRUE,pBar))
	{
		if(type==CWebDataMiningApp::wdmServer)
		{
			theApp.CloseDB(type);
			CCommonFunction::OpenOutNet();
		}

		MessageBox(_T("保存成功!"));
	}
	else
		MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
    pBar=NULL;
}

void CExtractSourceTree::OnRootFresh() 
{
	// TODO: Add your command handler code here
	InitUpdate(TRUE,TRUE);
}

void CExtractSourceTree::OnRootDownload() 
{
	// TODO: Add your command handler code here
	GetSelectedObject()->DownloadObject();
}

void CExtractSourceTree::OnRootDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection(FALSE);
}


void CExtractSourceTree::OnColumnDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CExtractSourceTree::OnColumnDownload() 
{
	// TODO: Add your command handler code here
    GetSelectedObject()->DownloadObject();
}

void CExtractSourceTree::OnColumnEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,!(theApp.GetUserLevel()>0))==IDOK && 
	   GetSelectedObject()->IsModified() &&
	   MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CExtractSourceTree::OnColumnSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	CWebDataMiningApp::DBServerType type=CWebDataMiningApp::localServer;

	if(theApp.GetNetworkType()==0 && MessageBox(_T("保存到内网服务器吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
		pBar->UpdateBar(_T("正在连接内网,请等待..."),200);
        type=CWebDataMiningApp::wdmServer;

		CCommonFunction::OpenInNet();
		Sleep(5000);
		Sleep(3000);

		theApp.ConnectDB(type);
	}
    
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(type),TRUE,pBar))
	{
		if(type==CWebDataMiningApp::wdmServer)
		{
			theApp.CloseDB(type);
			CCommonFunction::OpenOutNet();
		}

		MessageBox(_T("保存成功!"));
	}
	else
		MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}


void CExtractSourceTree::OnEntryDelete() 
{
	// TODO: Add your command handler code here
	//Firstly delete the schedule objects
// 	CString strScheduleID=GetSelectedObject()->GetID();
// 	POSITION pos=theApp.GetScheduleTree()->m_objectHelper.GetFirstChildPosition();
// 	while (pos)
// 	{
// 		CBaseObject *pOb=theApp.GetScheduleTree()->m_objectHelper.GetNextChild(pos);
// 		if(pOb!=NULL && (static_cast<CSchedule*>(pOb))->m_data.m_strEntryID==strScheduleID)
// 		{
// 			//Delete the data from database
// 			if(theApp.DBIsOpen())
// 				pOb->DeleteFromDB(*theApp.GetDB());
// 			//Delete the data from the tree
// 			theApp.GetScheduleTree()->DeleteItem(pOb->GetRefItem());
// 			//Delete the data from the memory
// 			theApp.GetScheduleTree()->m_objectHelper.GetRidOf(pOb);
// 			delete pOb; 
// 		}
// 	}
	DeleteSelection();
}

void CExtractSourceTree::OnEntryDownload() 
{
	// TODO: Add your command handler code here
    GetSelectedObject()->DownloadObject();
}

void CExtractSourceTree::OnEntryEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,!(theApp.GetUserLevel()>0))==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CExtractSourceTree::OnEntrySave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	CWebDataMiningApp::DBServerType type=CWebDataMiningApp::localServer;

	if(theApp.GetNetworkType()==0 && MessageBox(_T("保存到内网服务器吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
		pBar->UpdateBar(_T("正在连接内网,请等待..."),200);
        type=CWebDataMiningApp::wdmServer;

		CCommonFunction::OpenInNet();
		Sleep(5000);
		Sleep(3000);

		theApp.ConnectDB(type);
	}
    
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(type),TRUE,pBar))
	{
		if(type==CWebDataMiningApp::wdmServer)
		{
			theApp.CloseDB(type);
			CCommonFunction::OpenOutNet();
		}

		MessageBox(_T("保存成功!"));
	}
	else
		MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}


void CExtractSourceTree::OnPictureDelete() 
{
	// TODO: Add your command handler code here
	CPtrList listSel,listTopic;
	if(IsMultiSelect())
		GetMultiSelectedObjects(listSel);
	else
		listSel.AddTail(GetSelectedObject());

	POSITION pos=listSel.GetHeadPosition();
	while (pos)
	{
		CBaseObject* pOb=(CBaseObject*)listSel.GetNext(pos);
		if(pOb!=NULL && pOb->GetType()==OBJECT_TYPE_PICTURE)
			listTopic.AddTail(pOb->GetParent());
	}
	if(DeleteSelection())
	{
		pos=listTopic.GetHeadPosition();
		while (pos)
		{
			CTopic* pTopic=(CTopic*)listTopic.GetNext(pos);
			pTopic->UpdateTopic();
		}
	}

}

void CExtractSourceTree::OnPictureEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CExtractSourceTree::OnPictureSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox("保存选择的节点内容吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB()))
		MessageBox("保存成功!");
	else
		MessageBox(_T("保存失败!"));
}

void CExtractSourceTree::OnTopicDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection();	
}

void CExtractSourceTree::OnTopicEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CExtractSourceTree::OnTopicPublish() 
{
	// TODO: Add your command handler code here
	CTopic* pTopic=static_cast<CTopic*>(GetSelectedObject());
    
	BOOL bPublish=!pTopic->m_data.m_bIsPublish;
    if( !bPublish && MessageBox(_T("重新发布还是从服务器删除？"),_T("请选择"),MB_YESNO|MB_ICONQUESTION)==IDYES )	
	{
		bPublish=TRUE;
	}

	thread_pool::instance().queue_request(new CWorkTopic(pTopic,bPublish));
 	//request_ptr request = request_ptr( std::auto_ptr<CWorkTopic>(new CWorkTopic(pTopic,bPublish)));
 	//thread_pool::instance().add_request(request);

}

void CExtractSourceTree::OnTopicSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
    
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB()))
	{
		MessageBox(_T("保存成功!"));
	}
	else
		MessageBox(_T("保存失败!"));
}

void CExtractSourceTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE && HIWORD(GetKeyState(VK_CONTROL)) )
	{
		DeleteSelection();
	}
	CCustomTree::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExtractSourceTree::OnColumnDelTopic() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("删除所有文章,确认吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		 return;
	GetSelectedObject()->DelSubObject();
}

void CExtractSourceTree::OnRootDelTopic() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("删除所有文章,确认吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		 return;
	GetSelectedObject()->DelSubObject();	
}

void CExtractSourceTree::OnEntryDelTopic() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("删除所有文章,确认吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		 return;
	GetSelectedObject()->DelSubObject();
}

void CExtractSourceTree::OnColumnNew() 
{
	// TODO: Add your command handler code here
	CColumn *pColumn=new CColumn;
	
	pColumn->IsNew()=TRUE;
	if(pColumn->EditObject(this,FALSE) == IDOK)
	{
		pColumn->SetID(CCommonFunction::AllocateID());
		pColumn->AddTo(GetSelectedObject());
		pColumn->ShowInTreeView(this,TRUE);

		if(MessageBox(_T("是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
			pColumn->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
		delete pColumn;
}

int CExtractSourceTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCustomTree::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_nTimerID=SetTimer(1,1000,NULL);

	return 0;
}

void CExtractSourceTree::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		COleDateTime time=COleDateTime::GetCurrentTime();
		COleDateTime delTopicTime,temp;   
		// At 23:59:59 delete all topics from the entry
		delTopicTime.SetTime(23,59,59);
		temp.SetTime(time.GetHour(),time.GetMinute(),time.GetSecond());
   
		if(temp==delTopicTime)
		{
			m_objectHelper.DelSubObject(FALSE);	   
			Sleep(1000);
			theApp.DeleteOldTopics();
		}

		break;
	}
	CCustomTree::OnTimer(nIDEvent);
}

void CExtractSourceTree::OnDestroy() 
{
	CCustomTree::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_nTimerID==1)
		KillTimer(m_nTimerID);
}

void CExtractSourceTree::OnColumnSetDbserver() 
{
	// TODO: Add your command handler code here
	CColumn* pTopColumn=(CColumn*)GetSelectedObject();
	if (pTopColumn->m_data.m_strDatabaseID.IsEmpty())
	{
		CDatabaseInfoDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			pTopColumn->m_data.m_strDatabaseID=dlg.GetDatabaseID();
			pTopColumn->SaveToDB(*theApp.GetDB(),FALSE);
			MessageBox(_T("指定数据库成功!"));
		}
		else
			MessageBox(_T("指定数据库失败!"));
	}
	else
	{
		if(MessageBox(_T("取消指定的数据库，确定吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			pTopColumn->m_data.m_strDatabaseID.Empty();
			GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
			MessageBox(_T("取消数据库成功!"));
		}
	}
} 

void CExtractSourceTree::OnColumnSchedule() 
{
	// TODO: Add your command handler code here
	CScheduleListDlg dlg(GetSelectedObject());
	dlg.DoModal();
}

void CExtractSourceTree::OnEntrySchedule() 
{
	// TODO: Add your command handler code here
	CScheduleListDlg dlg(GetSelectedObject());
	dlg.DoModal();
}
void CExtractSourceTree::ProcessPendingRead(char* buf)
{
	LPTOPICPARAM pTp=(LPTOPICPARAM)buf;

	CTopic* pTopic=(CTopic*)m_objectHelper.FindSubObject(OBJECT_TYPE_TOPIC,pTp->strTopicID,TRUE);
	if(pTopic!=NULL)	
	{
        pTopic->m_data.m_bIsPublish=pTp->bIsPublished;
		SetItemSpecShow(pTopic->GetRefItem(),pTopic->m_data.m_bIsPublish);
	}
}

void CExtractSourceTree::OnTopicDelPicture() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("删除文章所有图片,确认吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		 return;
	GetSelectedObject()->DelSubObject(TRUE);
}

void CExtractSourceTree::OnColumnFresh() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	//Delete the old content,but not from database
	CProgressBar* pBar=new CProgressBar(_T("刷新选择的节点内容"),60,5000);

	DeleteAllChildItem(GetSelectedObject()->GetRefItem());
	GetSelectedObject()->LoadFromDB(*theApp.GetDB(),pBar);
    GetSelectedObject()->ShowInTreeView(this,TRUE);

	pBar->Finish(_T("刷新节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CExtractSourceTree::OnEntryFresh() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	//Delete the old content,but not from database
	CProgressBar* pBar=new CProgressBar(_T("刷新选择的节点内容"),60,5000);

	DeleteAllChildItem(GetSelectedObject()->GetRefItem());
	GetSelectedObject()->LoadFromDB(*theApp.GetDB(),pBar);
    GetSelectedObject()->ShowInTreeView(this,TRUE);

	pBar->Finish(_T("刷新节点完毕"));
	delete pBar;
	pBar=NULL;
}



void CExtractSourceTree::OnExport() 
{
	// TODO: Add your command handler code here
	BOOL bRet=FALSE;
	CExportDlg dlg;
	dlg.m_strFileDir=theApp.ExportDir();
	if(dlg.DoModal()==IDOK)
	{   
		CWaitCursor wait;
		CString strDir=dlg.m_strFileDir;
		theApp.ExportDir()=strDir;

		CProgressBar bar(_T("导出节点所有内容"),60,5000);
        //装载父亲节点
		std::stack<CBaseObject*> s;
		CBaseObject* pOb=GetSelectedObject()->GetParent();
		while (pOb)
		{
			s.push(pOb);
			pOb=pOb->GetParent();
		}
		switch(dlg.m_nType)
		{           
		case 0:    //XML
			{
				TiXmlDocument doc;
				TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
				
				doc.LinkEndChild(pDecl);
				TiXmlNode* pNode=(TiXmlNode*)&doc;
				while (s.size())
				{
					pNode=s.top()->StreamOut(pNode,FALSE,&bar);
					s.pop();
				}
				GetSelectedObject()->StreamOut(pNode,TRUE,&bar);
				CString strFile;
				strFile.Format("%s\\%s_采集源数据.xml",strDir,GetSelectedObject()->GetDesc());
				CCommonFunction::CreateDir(strFile);
				bRet=doc.SaveFile(strFile);	
			}
			break;
		case 1:   //TXT
			{
				ofstream stream;
				CString strFile;
				strFile.Format("%s\\%s.txt",strDir,GetSelectedObject()->GetDesc());
				CCommonFunction::CreateDir(strFile);
		        stream.open(strFile);

				while (s.size())
				{
					s.top()->StreamOutTxt(stream,FALSE,&bar);
					s.pop();
				}
				GetSelectedObject()->StreamOutTxt(stream,TRUE,&bar);
				
				stream.close();
				bRet=TRUE;
			}
			break;
		case 2:   //蓝网文件
			{
				if(s.size()==0)
				{
					POSITION pos=GetSelectedObject()->GetFirstChildPosition();
					while (pos)
					{
						CBaseObject* pChild=GetSelectedObject()->GetNextChild(pos);
						if(pChild)
						{
							bRet=TRUE;
							strDir=dlg.m_strFileDir;
							
							TiXmlDocument doc;
							TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
							
							doc.LinkEndChild(pDecl);
							TiXmlNode* pNode=(TiXmlNode*)&doc;
							
							pChild->Export(strDir,pNode,TRUE,&bar);
							CString strFile;
							strFile.Format("%s\\Index\\ClassIndex.xml",strDir);
							bRet&=(BOOL)doc.SaveFile(strFile);
						}
					}
				}
				else
				{
					TiXmlDocument doc;
					TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
					
					doc.LinkEndChild(pDecl);
					TiXmlNode* pNode=(TiXmlNode*)&doc;
					while (s.size())
					{
						pNode=s.top()->Export(strDir,pNode,FALSE,&bar);
						s.pop();
					}
					GetSelectedObject()->Export(strDir,pNode,TRUE,&bar);
					CString strFile;
					strFile.Format("%s\\Index\\ClassIndex.xml",strDir);
					bRet=doc.SaveFile(strFile);
				}
			}
			break;
		}	
		bar.Finish(_T("导出数据完毕"));
	}
	MessageBox(bRet?_T("采集源数据导出成功"):_T("采集源数据导出失败"));
}

void CExtractSourceTree::OnImport() 
{
	// TODO: Add your command handler code here
	char szFilters[]="Xml Files (*.xml)|*.xml||";
	CString strFileName;
    BOOL bRet=TRUE;

	CFileDialog fileDlg(TRUE,"","",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		CWaitCursor wait;
		strFileName= fileDlg.GetPathName();
		TiXmlDocument doc(strFileName);
		bRet&=(BOOL)doc.LoadFile();

		if(bRet)
		{
            TiXmlHandle hDoc(&doc);		    
            TiXmlElement* pElement=NULL;

			CProgressBar bar(_T("导入采集源数据"),60,5000);
			pElement=hDoc.FirstChildElement(_T("wdm_root")).ToElement();
		
			if(pElement)
			{
				 int nType=-1;	
                 pElement->QueryIntAttribute(_T("type"),&nType);
				 if(nType==0)
				 {
			         bRet&=GetSelectedObject()->StreamIn(pElement,TRUE,&bar);
				     GetSelectedObject()->ShowInTreeView(this,TRUE);
				 }
				 else
				 {
					MessageBox(_T("数据格式不匹配"));
					bRet=FALSE;
				 }			
			}
			bar.Finish(_T("采集源数据导入完毕"));
		}
	}
	else
	{
		bRet=FALSE;
	}

	MessageBox(bRet?_T("采集源数据导入成功"):_T("采集源数据导入失败"));
}
void CExtractSourceTree::GetEntrys(CBaseObject* pOb)
{
	if(pOb)
	{
		POSITION pos=pOb->GetFirstChildPosition();
		while (pos)
		{
			CBaseObject* pChild=pOb->GetNextChild(pos);
			if(pChild)
			{
				if(pChild->GetType()==OBJECT_TYPE_ENTRY)
				{
					m_qEntrys.push((CEntry*)pChild);
				}
				else
				{
					GetEntrys(pChild);	
				}
			}
		}
	}
}
void CExtractSourceTree::GetTopics(CBaseObject* pOb)
{
	if(pOb)
	{
		POSITION pos=pOb->GetFirstChildPosition();
		while (pos)
		{
			CBaseObject* pChild=pOb->GetNextChild(pos);
			if(pChild)
			{
				if(pChild->GetType()==OBJECT_TYPE_TOPIC)
				{
					m_qTopics.push((CTopic*)pChild);
				}
				else
				{
					GetTopics(pChild);	
				}
			}
		}
	}
}
// void CExtractSourceTree::OnColumnSetChannelid() 
// {
// 	// TODO: Add your command handler code here
// 	CInfoDlg dlg;
// 	dlg.m_strTitle=_T("频道ID");
//     if(dlg.DoModal()==IDOK)
// 	{
//          CWaitCursor wait;
// 		 GetEntrys(GetSelectedObject());
//           
// 		 CEntry* pEntry;
// 		 while (m_qEntrys.size())
// 		 {
// 			 pEntry=m_qEntrys.front();
// 			 pEntry->m_data.m_strServerChannelID=dlg.m_strName;
//              pEntry->SaveToDB(*theApp.GetDB(),FALSE);
// 			 
// 			 m_qEntrys.pop();
// 		 }
// 	}
// }

void CExtractSourceTree::OnEntrySetColumnid() 
{
	// TODO: Add your command handler code here
	CColumnClassInfoDlg *pDlg=new CColumnClassInfoDlg;
    pDlg->Create(CColumnClassInfoDlg::IDD,this);
	pDlg->ShowWindow(SW_NORMAL);
	pDlg->CenterWindow();
}

// void CExtractSourceTree::OnColumnSetSubject() 
// {
// 	// TODO: Add your command handler code here
// 	CInfoDlg dlg;
// 	dlg.m_strTitle=_T("专题词");
//     if(dlg.DoModal()==IDOK)
// 	{
//          CWaitCursor wait;
// 		 GetTopics(GetSelectedObject());
//           
// 		 CTopic* pTopic;
// 		 while (m_qTopics.size())
// 		 {
// 			 pTopic=m_qTopics.front();
// 			 pTopic->m_data.m_strSubject=dlg.m_strName;
//              pTopic->SaveToDB(*theApp.GetDB(),FALSE);
// 			 
// 			 m_qTopics.pop();
// 		 }
// 	}
//}

// void CExtractSourceTree::OnColumnSetRel1() 
// {
// 	// TODO: Add your command handler code here
// 	CInfoDlg dlg;
// 	dlg.m_strTitle=_T("关联词");
//     if(dlg.DoModal()==IDOK)
// 	{
//          CWaitCursor wait;
// 		 GetTopics(GetSelectedObject());
//           
// 		 CTopic* pTopic;
// 		 while (m_qTopics.size())
// 		 {
// 			 pTopic=m_qTopics.front();
// 			 pTopic->m_data.m_strKeyWord=dlg.m_strName;
//              pTopic->SaveToDB(*theApp.GetDB(),FALSE);
// 			 
// 			 m_qTopics.pop();
// 		 }
// 	}
//}

void CExtractSourceTree::OnEntryPasteTopic() 
{
	// TODO: Add your command handler code here
	CBaseObject* pParent=GetSelectedObject();
	
	POSITION pos=m_listObject.GetHeadPosition();
	while (pos)
	{
		CBaseObject* pChild=(CBaseObject*)m_listObject.GetNext(pos);
		if(pParent->CanAccept(pChild,this))
		{
			//Delete old objects from tree view
			DeleteItem(pChild->GetRefItem());
			pParent->Add(pChild);
			//Save object
			if(theApp.DBIsOpen())
				pChild->SaveToDB(*theApp.GetDB());
			
			pParent->ShowInTreeView(this,TRUE);
		}
	}
	m_listObject.RemoveAll();
}

void CExtractSourceTree::OnPictureCut() 
{
	// TODO: Add your command handler code here
	m_listObject.RemoveAll();

	if(IsMultiSelect())
		GetMultiSelectedObjects(m_listObject);
	else
		m_listObject.AddTail(GetSelectedObject());

}

void CExtractSourceTree::OnTopicCut() 
{
	// TODO: Add your command handler code here
	m_listObject.RemoveAll();

	if(IsMultiSelect())
		GetMultiSelectedObjects(m_listObject);
	else
		m_listObject.AddTail(GetSelectedObject());
}

void CExtractSourceTree::OnTopicPastePic() 
{
	// TODO: Add your command handler code here
	CBaseObject* pParent=GetSelectedObject();
	
	POSITION pos=m_listObject.GetHeadPosition();
	while (pos)
	{
		CBaseObject* pChild=(CBaseObject*)m_listObject.GetNext(pos);
		if(pParent->CanAccept(pChild,this))
		{
			//Delete old objects from tree view
			DeleteItem(pChild->GetRefItem());
			pParent->Add(pChild);
			//Save object
			if(theApp.DBIsOpen())
				pChild->SaveToDB(*theApp.GetDB());
			
			pParent->ShowInTreeView(this,TRUE);
		}
	}
	m_listObject.RemoveAll();
}

void CExtractSourceTree::OnAlterTopicUnit() 
{
	// TODO: Add your command handler code here
	CAlterTopicUnitDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		int nTopicUnit=dlg.m_nTopicUnit;
		int nProcessType=dlg.m_nProcessType;
		CString strValue=dlg.m_strValue;

		CWaitCursor wait;
		GetTopics(GetSelectedObject());
		
		CTopic* pTopic;
		while (m_qTopics.size())
		{
			pTopic=m_qTopics.front();			
	        pTopic->AlterTopic(nTopicUnit,nProcessType,strValue);	
			
			m_qTopics.pop();
		}		
	}
}
// void CExtractSourceTree::ProcessContent(CString& strContent,CString strKeyWord,int nType)
// {
// 	switch(nType)
// 	{	
// 	case 0: //删除前面所有内容		
// 	case 1: //删除后面所有内容		
// 	case 2: //删除本身所有内容
// 		{
// 		    CRegexWDM wdm;
// 		    strContent=wdm.FilterContent(strContent,strKeyWord,(CRegexWDM::FILTER_TYPE)nType);
// 		}
// 		break;
// 	case 3: //替换
// 		strContent=strKeyWord;
// 		break;
// 	default:
// 		ASSERT(FALSE);
// 	}
// }

void CExtractSourceTree::OnEntryNewTopic() 
{
	// TODO: Add your command handler code here
	CTopic* pTopic=new CTopic;

	pTopic->IsNew()=TRUE;
	pTopic->SetID(CCommonFunction::AllocateID());
	pTopic->m_data.m_timeDownload=COleDateTime::GetCurrentTime();

	pTopic->AddTo(GetSelectedObject());
	if(pTopic->EditObject(this,FALSE)==IDCANCEL)
	{
		if(pTopic->IsNew())
		{
			POSITION pos=pTopic->GetFirstChildPosition();
			while (pos)
			{
				CNewsPicture* pPicture=(CNewsPicture*)pTopic->GetNextChild(pos);
				if(pPicture!=NULL)
				{
					CString strPath;
					strPath.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),
						pPicture->m_data.m_strURL);
					CCommonFunction::Replace(strPath,"/","\\");
					DeleteFile(strPath);
				}
			}
			GetSelectedObject()->GetRidOf(pTopic);
			delete pTopic;
			pTopic=NULL;
		}
	}
}

void CExtractSourceTree::OnColumnCollapse() 
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	Expand(GetSelectedObject()->GetRefItem(),TVE_COLLAPSE,TRUE);
}
