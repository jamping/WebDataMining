// ScheduleTree.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ScheduleTree.h"
#include "Schedule.h"
#include "progressbar.h"
#include "ExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScheduleTree

CScheduleTree::CScheduleTree():m_objectHelper(HELPER_SCHEDULE)
{
	m_objectHelper.CanDelete()=FALSE;
}

CScheduleTree::~CScheduleTree()
{
}

BOOL CScheduleTree::DeleteSelection(BOOL bDelSelf/* =TRUE */)
{
	BOOL bRet=TRUE;
	CBaseObject *pOB=GetSelectedObject();
	
	if(theApp.DBIsOpen())
	{
		if(MessageBox("ȷ��Ҫɾ����ѡ������","��ȷ��",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			bRet=pOB->DeleteFromDB(*theApp.GetDB(),bDelSelf);
		}
		else
			return bRet;
		
		if(!bRet)
		{
			MessageBox("����ɾ��ʧ��");
		}
		else
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	else
	{
		if(MessageBox("���ݿ�������Ͽ�,��������ɾ����ѡ����,������","��ȷ��",MB_YESNO|MB_ICONQUESTION) == IDYES)
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	
	return bRet;
}

void CScheduleTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/* =FALSE */)
{
	CWaitCursor wait;
	//������е��ȼƻ�
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
		//�����ݿ����ؽ����ȼƻ���
		strSql.Format("select * from wdm_schedule");
		
		if(rs.Open(theApp.GetDB()->GetActiveConnection(),strSql,CADORecordset::openQuery))
		{
			if(bShowInfo)
			{
				pBar=new CProgressBar(_T("׼����ȡ���ȼƻ�����"),60,5000);
			}

			CSchedule *pSchedule=NULL;
			while(!rs.IsEOF())
			{
				pSchedule=new CSchedule;
				pSchedule->LoadInfo(rs);
				pSchedule->AddTo(&m_objectHelper);				
				
				pSchedule->LoadFromDB(*theApp.GetDB(),pBar);
			
				rs.MoveNext();
			}
			rs.Close();
 
			if(bShowInfo)
			{
			   pBar->Finish("���ڳ�ʼ�����ȼƻ�������");
			   delete pBar;
			   pBar=NULL;
			}
			
		}
	}
     m_objectHelper.ShowInTreeView(this,TRUE);
}
BEGIN_MESSAGE_MAP(CScheduleTree, CCustomTree)
	//{{AFX_MSG_MAP(CScheduleTree)
	ON_COMMAND(ID_ROOT_SAVE, OnRootSave)
	ON_COMMAND(ID_ROOT_FRESH, OnRootFresh)
	ON_COMMAND(ID_ROOT_DELETE, OnRootDelete)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SCHEDULE_DELETE, OnScheduleDelete)
	ON_COMMAND(ID_SCHEDULE_EDIT, OnScheduleEdit)
	ON_COMMAND(ID_SCHEDULE_SAVE, OnScheduleSave)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_IMPORT, OnImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScheduleTree message handlers

void CScheduleTree::OnRootSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("����ѡ��Ľڵ�������"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
    
	CProgressBar* pBar=new CProgressBar(_T("����ѡ��Ľڵ�����"),60,5000);

	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
	{
		MessageBox(_T("����ɹ�!"));
	}	
	else
		MessageBox(_T("����ʧ��!"));

	pBar->Finish(_T("����ڵ����"));
	delete pBar;
	pBar=NULL;
}

void CScheduleTree::OnRootFresh() 
{
	// TODO: Add your command handler code here
	InitUpdate(TRUE,TRUE);
}

void CScheduleTree::OnRootDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection(FALSE);
}

void CScheduleTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE && HIWORD(GetKeyState(VK_CONTROL)) )
	{
		DeleteSelection();
	}	
	CCustomTree::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CScheduleTree::OnScheduleDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CScheduleTree::OnScheduleEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,!(theApp.GetUserLevel()>0))==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("�����Ѿ����£��Ƿ񱣴浽���ݿ�"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CScheduleTree::OnScheduleSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("����ѡ��Ľڵ�������"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
    
	CProgressBar* pBar=new CProgressBar(_T("����ѡ��Ľڵ�����"),60,5000);

	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
	{
		MessageBox(_T("����ɹ�!"));
	}	
	else
		MessageBox(_T("����ʧ��!"));

	pBar->Finish(_T("����ڵ����"));
	delete pBar;
	pBar=NULL;
}

void CScheduleTree::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		// Download topics scheduly
		POSITION pos=m_objectHelper.GetFirstChildPosition();
		while (pos)
		{
			CSchedule* pSchedule=(CSchedule*)m_objectHelper.GetNextChild(pos);
			if(pSchedule!=NULL && pSchedule->IsValid())
			{
                if(pSchedule->GetNextRunTime()==COleDateTime::GetCurrentTime())
				{
                     (pSchedule->GetScheduledObject())->DownloadObject(TRUE);
					 pSchedule->IsTimeUp()=TRUE;
				}
			}
		}
        // Auto Send topic to wdm server
// 		if( theApp.IsAutoSendTopic()==TRUE
// 			&& theApp.GetThreadPool().get_current_working_count()==0
// 			&& (theApp.GetDownloadTopicsList().GetSize()>0 || theApp.GetAutoPublishTopicList().GetSize()>0))
// 		{
// 			theApp.SendTopicToServer();
// 		}
		if( theApp.IsAutoSendTopic()==TRUE
			&& thread_pool::instance().get_current_working_count()==0
			&& (theApp.GetDownloadTopicsList().GetSize()>0 || theApp.GetAutoPublishTopicList().GetSize()>0))
		{
			theApp.SendTopicToServer();
		}
		break;
	}
	CCustomTree::OnTimer(nIDEvent);
}

int CScheduleTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCustomTree::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_nTimerID=SetTimer(1,1000,NULL);
	return 0;
}

void CScheduleTree::OnDestroy() 
{
	CCustomTree::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_nTimerID==1)
	    KillTimer(m_nTimerID);
}

void CScheduleTree::OnExport() 
{
	// TODO: Add your command handler code here
	BOOL bRet=FALSE;
	CExportDlg dlg(EXPORT_XML);
	dlg.m_strFileDir=theApp.ExportDir();
	if(dlg.DoModal()==IDOK)
	{   
		CWaitCursor wait;
		CString strDir=dlg.m_strFileDir;
		theApp.ExportDir()=strDir;

		CProgressBar bar(_T("�����ڵ���������"),60,5000);
        
		TiXmlDocument doc;
		TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
		
		doc.LinkEndChild(pDecl);
		TiXmlNode* pNode=(TiXmlNode*)&doc;
		
		GetSelectedObject()->StreamOut(pNode,TRUE,&bar);

		CString strFile;
		strFile.Format("%s\\%s_���ȼƻ�����.xml",strDir,GetSelectedObject()->GetDesc());
		CCommonFunction::CreateDir(strFile);
		bRet=doc.SaveFile(strFile);			
		
		bar.Finish(_T("�����������"));
	}
	MessageBox(bRet?_T("���ȼƻ����ݵ����ɹ�"):_T("���ȼƻ����ݵ���ʧ��"));
}

void CScheduleTree::OnImport() 
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

			CProgressBar bar(_T("������ȼƻ�����"),60,5000);
			pElement=hDoc.FirstChildElement(_T("wdm_root")).ToElement();
			if(pElement)
			{
				 int nType=-1;	
                 pElement->QueryIntAttribute(_T("type"),&nType);
				 if(nType==2)
				 {
			         bRet&=GetSelectedObject()->StreamIn(pElement,TRUE,&bar);
				     GetSelectedObject()->ShowInTreeView(this,TRUE);
				 }
				 else
				 {
					MessageBox(_T("���ݸ�ʽ��ƥ��"));
					bRet=FALSE;
				 }				
			}
			bar.Finish(_T("���ȼƻ����ݵ������"));
		}
	}
	else
	{
		bRet=FALSE;
	}
	MessageBox(bRet?_T("���ȼƻ����ݵ���ɹ�"):_T("���ȼƻ����ݵ���ʧ��"));
}
