// TopicListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "TopicListDlg.h"
#include "Column.h"
#include "Entry.h"
#include "Topic.h"
#include "WorkTopic.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopicListDlg dialog
IMPLEMENT_DYNCREATE(CTopicListDlg,CDialogEx)

CTopicListDlg::CTopicListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopicListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTopicListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pList=NULL;
	m_type=UNPUBLISHED;
	m_bIsSelAll=FALSE;
}


void CTopicListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTopicListDlg)
	DDX_Control(pDX, IDC_LIST_TOPIC, m_listTopic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTopicListDlg, CDialogEx)
	//{{AFX_MSG_MAP(CTopicListDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TOPIC, OnDblclkListTopic)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TOPIC, OnClickListTopic)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SET_ITEMS,OnLoadComboboxData)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopicListDlg message handlers

BOOL CTopicListDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listTopic.SetHeadings(_T("序号,50;标题,300;日期,160;图片数量,80;关键字,100;专题,100")); 
    m_listTopic.LoadColumnInfo();

	CString  strValidChars= "0123456789.";
	m_listTopic.SetColumnValidEditCtrlCharacters(strValidChars,0);//digit edit only 
	m_listTopic.SetReadOnlyColumns(0);
//	m_listTopic.SetReadOnlyColumns(1);
	m_listTopic.SetReadOnlyColumns(2);
    m_listTopic.SetReadOnlyColumns(3);
// 	m_listTopic.SetComboColumns(4);
// 	m_listTopic.SetComboColumns(5);


	m_listTopic.EnableVScroll(); 

    ShowContent(m_pList,m_type);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTopicListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogEx::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_listTopic.GetSafeHwnd())
	{
		CRect rect;
		GetWindowRect(&rect);
		
		rect.DeflateRect(2,2,2,4);
		ScreenToClient(&rect);
		m_listTopic.MoveWindow(rect);
	}

}

void CTopicListDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialogEx::PostNcDestroy();
}
void CTopicListDlg::SetParams(CStringList* pList,TopicStatus type/* =UNPUBLISHED */)
{
	m_pList=pList;
	m_type=type;
}
void CTopicListDlg::ShowContent(CStringList* pList,TopicStatus type/* =UNPUBLISHED */)
{
	SetParams(pList,type);
	m_listTopic.DeleteAllItems();

	if(m_pList==NULL)
		return;

	BOOL bPublish=FALSE;
	switch(m_type)
	{
	case UNPUBLISHED:
		break;
	case PUBLISHED:
		bPublish=TRUE;
		break;
	default:
		ASSERT(FALSE);
	}
    CString str;
	CBaseObject* pEntry=NULL;
	int i=0;

	POSITION pos=m_pList->GetHeadPosition();
	while (pos)
	{
        pEntry=theApp.GetSourceTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_ENTRY,m_pList->GetNext(pos),TRUE);
		if(pEntry==NULL)
			continue;

		POSITION pos1=pEntry->GetFirstChildPosition();
		while (pos1)
		{
			CTopic* pTopic=(CTopic*)pEntry->GetNextChild(pos1);
			if(pTopic!=NULL && pTopic->m_data.m_bIsPublish==bPublish)
			{
				str.Format("%d",i+1);
				
				m_listTopic.AddItem(str,_T(""),_T(""),_T(""),_T(""),_T(""));
				m_listTopic.SetItemText(i,1,pTopic->m_data.m_strTitle);
				m_listTopic.SetItemText(i,2,pTopic->m_data.m_strTime);
				str.Format("%d",pTopic->GetChildCount());
				m_listTopic.SetItemText(i,3,str);
// 				m_listTopic.SetItemText(i,4,pTopic->m_data.m_strBigClass);
// 				m_listTopic.SetItemText(i,5,pTopic->m_data.m_strSmallClass);	
				m_listTopic.SetItemText(i,4,pTopic->m_data.m_strKeyWord);
				m_listTopic.SetItemText(i,5,pTopic->m_data.m_strSubject);
				
				m_listTopic.SetItemData(i++,(DWORD)pTopic);
			}
		}
	}
}



void CTopicListDlg::OnDblclkListTopic(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	 int nItem=pNMListView->iItem;

	 if(nItem!=-1)
	 {
		 UpdateData();

		 CTopic* pTopic=(CTopic*)m_listTopic.GetItemData(nItem);
		 CWaitCursor wait;
		 pTopic->EditObject(this);

		 if( pTopic->IsModified() )
		 {
			 CString str;
			 m_listTopic.SetItemText(nItem,1,pTopic->m_data.m_strTitle);
			 m_listTopic.SetItemText(nItem,2,pTopic->m_data.m_strTime);
			 str.Format("%d",pTopic->GetChildCount());
			 m_listTopic.SetItemText(nItem,3,str);
			 m_listTopic.SetItemText(nItem,4,pTopic->m_data.m_strKeyWord);
			 m_listTopic.SetItemText(nItem,5,pTopic->m_data.m_strSubject);

			 m_listTopic.UpdateData(FALSE);
		 }
	 }

	*pResult = 0;
}

void CTopicListDlg::OnClickListTopic(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
     int nItem=pNMListView->iItem;
     if(nItem!=-1)
	 {
//	     m_listTopic.SetCheck(nItem,!m_listTopic.GetCheck(nItem));
	 }

	*pResult = 0;
}
// LRESULT CTopicListDlg::OnLoadComboboxData(WPARAM wParam, LPARAM lParam)
// {
// 	// TODO: Add your control notification handler code here
// 	int nRowIndex,nColumnIndex;
// 	nRowIndex=LOWORD((DWORD)wParam);
// 	nColumnIndex=HIWORD((DWORD)wParam);
// 
// 	CStringList* strList=(CStringList*)lParam;
//     //Read data from directory table
// 	CString strChannelID,str;  
// 	strChannelID=theApp.GetServerChannelIDByColumnID(m_pEntry->m_data.m_strServerColumnID);
// 	CBaseObject* pRoot=theApp.GetServerColumnRoot()->FindSubObject(OBJECT_TYPE_SERVER_COLUMN,strChannelID,TRUE);
// 
// 	switch(nColumnIndex)
// 	{
// 	case 4:
// 		break;
// 	case 5:
// 		str=m_listTopic.GetItemText(nRowIndex,4);
// 		if(pRoot!=NULL)
// 		{
// 			POSITION pos=pRoot->GetFirstChildPosition();
// 			while (pos)
// 			{
// 				CBaseObject* pChild=pRoot->GetNextChild(pos);
// 				if(pChild!=NULL && pChild->GetDesc().Compare(str)==0)
// 				{
// 					pRoot=pChild;
// 					break;
// 				}
// 			}
// 		}
// 		break;
// 	default:
// 		ASSERT(FALSE);
// 	}
// 	if(pRoot!=NULL)
// 	{
// 		POSITION pos=pRoot->GetFirstChildPosition();
// 		while (pos)
// 		{
// 			CBaseObject* pChild=pRoot->GetNextChild(pos);
// 			if(pChild!=NULL)
// 				strList->AddTail(pChild->GetDesc());
// 		}
// 	}
// 
// 	return 0;
// }
BOOL CTopicListDlg::IsSelAll()
{
	UpdateData();

	BOOL bSelAll=TRUE;
	for(int i=0;i<m_listTopic.GetItemCount();i++)
        bSelAll&=m_listTopic.GetCheck(i);

    m_bIsSelAll=bSelAll;

	return m_bIsSelAll;
}

BOOL CTopicListDlg::IsTopicSelected()
{
	UpdateData();
	
	BOOL bRet=FALSE;
    for(int i=0;i<m_listTopic.GetItemCount();i++)
	{
        if(m_listTopic.GetCheck(i))
		{
			bRet= TRUE;
			break;
		}
	}
    return bRet;
}
BOOL CTopicListDlg::IsValid()
{
	BOOL bRet=FALSE;
	if(IsTopicSelected())
	{
		bRet=TRUE;
		for(int i=0;i<m_listTopic.GetItemCount();i++)
		{
			if(m_listTopic.GetCheck(i))
			{
				CTopic* pTopic=(CTopic*)m_listTopic.GetItemData(i);
				if(!pTopic->IsValid())
				{
					bRet=FALSE;
					break;
				}
			}
		}
	}
    return bRet;
}
BOOL CTopicListDlg::PublishTopicToServer(BOOL bIsPrimary,CString strServerColumnIDs/* =_T */,CString strKeyWord/* =_T */,CString strSubject/* =_T */)
{
	CWaitCursor wait;

	BOOL bRet = FALSE;
	CString str;

	CMainFrame * pMainFrm=static_cast<CMainFrame*>(theApp.GetMainWnd());

    for(int i=0;i<m_listTopic.GetItemCount();i++)
	{
		if(m_listTopic.GetCheck(i))
		{   
			CTopic* pTopic=(CTopic*)m_listTopic.GetItemData(i);
			pTopic->m_data.m_bIsPrimary=bIsPrimary;

			if(!strServerColumnIDs.IsEmpty())
				pTopic->m_data.m_strServerColumnIDs=strServerColumnIDs;
			if(!strKeyWord.IsEmpty())
				pTopic->m_data.m_strKeyWord=strKeyWord;
            if(!strSubject.IsEmpty())
				pTopic->m_data.m_strSubject=strSubject;

			//bRet = pTopic->PublishToServer();

			//if(pMainFrm!=NULL)
			//{				
			//	str.Format("%s%s====>%s",_T("发布文章："),pTopic->GetDesc(),bRet?_T("成功"):_T("失败"));
			//	pMainFrm->UpdateOutputInfo(str);
			//}		
			
    		thread_pool::instance().queue_request(new CWorkTopic(pTopic,TRUE));
			//Sleep(1000 );
		}
	}
	return TRUE;
}
BOOL CTopicListDlg::DeleteTopicFromServer()
{
    for(int i=0;i<m_listTopic.GetItemCount();i++)
	{
		if(m_listTopic.GetCheck(i))
		{
            CTopic* pTopic=(CTopic*)m_listTopic.GetItemData(i);
			//Open database firstly
			if(!theApp.DBIsOpen(CWebDataMiningApp::remoteServer))
			{
				CWDMDatabase* pDatabase=pTopic->GetDatabase();
				ASSERT(pDatabase!=NULL);
				
				COptionsData::OptionsData::DBServerData data;
				pDatabase->GetDBServerData(data);
				
				if(!theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
                    return FALSE;
			}
			
			thread_pool::instance().queue_request(new CWorkTopic(pTopic,FALSE));
 			//request_ptr request = request_ptr( std::auto_ptr<CWorkTopic>(new CWorkTopic((pTopic,FALSE))));
 			//thread_pool::instance().add_request(request);

		}
	}
	return TRUE;
}
LRESULT CTopicListDlg::OnEndLabelEdit(WPARAM wParam, LPARAM lParam) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	// TODO: Add your control notification handler code here
	int nIndex=pDispInfo->item.iItem;
	
	m_listTopic.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	CTopic* pTopic=(CTopic*)m_listTopic.GetItemData(pDispInfo->item.iItem);

	switch(pDispInfo->item.iSubItem)
	{
	case 4:
		pTopic->m_data.m_strKeyWord=pDispInfo->item.pszText;
		break;
	case 5:
		pTopic->m_data.m_strSubject=pDispInfo->item.pszText;
		break;
	}

	return 1;
}


void CTopicListDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();
	
	// TODO: Add your message handler code here
	m_listTopic.SaveColumnInfo();

}
