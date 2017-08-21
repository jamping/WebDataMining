// TopicPublishDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "TopicPublishDlg.h"
#include "TopicListDlg.h"
#include "ExportDlg.h"
#include "progressbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopicPublishDlg dialog

CTopicPublishDlg::CTopicPublishDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTopicPublishDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTopicPublishDlg)
	m_bIsPrimary = TRUE;
	m_strKeyWord = _T("");
	m_strSubject = _T("");
	//}}AFX_DATA_INIT
}


void CTopicPublishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTopicPublishDlg)
	DDX_Control(pDX, IDC_SERVER_COLUMN, m_ctlServerColumn);
	DDX_Control(pDX, IDC_COLUMN, m_ctlColumn);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Check(pDX, IDC_CHECK_RPIMARY1, m_bIsPrimary);
	DDX_CBString(pDX, IDC_COMBO_KEYWORD, m_strKeyWord);
	DDX_CBString(pDX, IDC_SUBJECT, m_strSubject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTopicPublishDlg, CDialogEx)
	//{{AFX_MSG_MAP(CTopicPublishDlg)
	ON_BN_CLICKED(IDC_SELECTED_ALL, OnSelectedAll)
	ON_BN_CLICKED(IDC_PUBLISH_TOPIC, OnPublishTopic)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_FRESH, OnFresh)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	//}}AFX_MSG_MAP
	ON_MESSAGE(NOTIFY_TREECOMBO_CHECK,OnComboTreeCheck)
	ON_UPDATE_COMMAND_UI(IDC_SELECTED_ALL,OnUpdateSelAll)
	ON_UPDATE_COMMAND_UI(IDC_PUBLISH_TOPIC,OnUpdatePublish)
	ON_UPDATE_COMMAND_UI(IDC_DELETE,OnUpdateDel)
	ON_UPDATE_COMMAND_UI(IDC_EXPORT,OnUpdateExport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopicPublishDlg message handlers

void CTopicPublishDlg::OnSelectedAll() 
{
	// TODO: Add your control notification handler code here
	int nCount = GetCurPage()->m_listTopic.GetItemCount();
	// Set the check state of every other item to TRUE and 
	// all others to FALSE.	
	GetCurPage()->GetSelAll() = !GetCurPage()->GetSelAll();
	for (int i=0;i < nCount;i++)
	{
		GetCurPage()->m_listTopic.SetCheck(i, GetCurPage()->GetSelAll());	
	}
}

void CTopicPublishDlg::OnPublishTopic() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CString strIDs=m_ctlServerColumn.GetCheckedIDs();
	GetCurPage()->PublishTopicToServer(m_bIsPrimary,strIDs,m_strKeyWord,m_strSubject);
    //Clear the old info
	GetDlgItem(IDC_COMBO_KEYWORD)->SetWindowText(_T(""));
	GetDlgItem(IDC_SUBJECT)->SetWindowText(_T(""));
	m_ctlServerColumn.SetChecks(strIDs,FALSE);
}

void CTopicPublishDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	GetCurPage()->DeleteTopicFromServer();
}
void CTopicPublishDlg::InitColumn(CBaseObject* pRoot,int nLevel)
{
	POSITION pos=pRoot->GetFirstChildPosition();
	while (pos)
	{
		CBaseObject* pChild=pRoot->GetNextChild(pos);
		if(pChild!=NULL)
		{
			if(pChild->GetType()==OBJECT_TYPE_ENTRY)
			{
				m_ctlColumn.AddString(pChild->GetDesc(),pChild->GetID(),ROOT_LEVEL+nLevel);
				
				CBaseObject* pSel=theApp.GetSourceTree()->GetSelectedObject();
				if(pSel!=NULL)
				{
					if(pSel->GetType()>(int)OBJECT_TYPE_ENTRY)
						pSel=pSel->BackTo(OBJECT_TYPE_ENTRY);

					if( (pSel->GetID().Compare(pChild->GetID())==0) ||
				        pSel->FindSubObject(OBJECT_TYPE_ENTRY,pChild->GetID(),TRUE)!=NULL)
					{
						m_strSelIDs+=pChild->GetID()+_T(" ");
					}
				}
			}			    
			else
			{
				m_ctlColumn.AddString(pChild->GetDesc(),INVALID_ID,ROOT_LEVEL+nLevel);
				InitColumn(pChild,nLevel+1);
			}			
		}
	}

}
void CTopicPublishDlg::InitServerColumn(CBaseObject* pRoot,int nLevel)
{
	POSITION pos=pRoot->GetFirstChildPosition();
	while (pos)
	{
		CBaseObject* pChild=pRoot->GetNextChild(pos);
		if(pChild!=NULL)
		{
			if(pChild->GetChildCount()>0)
			    m_ctlServerColumn.AddString(pChild->GetDesc(),INVALID_ID,ROOT_LEVEL+nLevel);
			else
				m_ctlServerColumn.AddString(pChild->GetDesc(),pChild->GetID(),ROOT_LEVEL+nLevel);

			InitServerColumn(pChild,nLevel+1);
		}
	}
}

BOOL CTopicPublishDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	autoKeyWord.Init(GetDlgItem(IDC_COMBO_KEYWORD));
	autoSubject.Init(GetDlgItem(IDC_SUBJECT));

	InitColumn(&theApp.GetSourceTree()->m_objectHelper,1);
	InitServerColumn(theApp.GetServerColumnRoot(),1);
	m_ctlServerColumn.EnableSendMsgToParent(FALSE);
	
	CTopicListDlg* pDialog=NULL;
	CStringList list;
	pDialog=(CTopicListDlg *)(RUNTIME_CLASS(CTopicListDlg))->CreateObject();
	pDialog->SetParams(&list,CTopicListDlg::UNPUBLISHED);
	pDialog->Create(IDD_TOPIC_LIST_DLG,&m_tab);
	m_tab.AddPage("未发布",pDialog);
		
	pDialog=(CTopicListDlg *)(RUNTIME_CLASS(CTopicListDlg))->CreateObject();
	pDialog->SetParams(&list,CTopicListDlg::PUBLISHED);
	pDialog->Create(IDD_TOPIC_LIST_DLG,&m_tab);
	m_tab.AddPage("已发布",pDialog);

	m_tab.ActivePage(0);
    //Set default value
	m_ctlColumn.SetChecks(m_strSelIDs,TRUE);
	SendMessage(NOTIFY_TREECOMBO_CHECK);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CTopicPublishDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CTopicPublishDlg::OnUpdateSelAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCurPage()->m_listTopic.GetItemCount()>0);
	pCmdUI->SetText(GetCurPage()->IsSelAll()?_T("取消全选"):_T("全选"));
}

void CTopicPublishDlg::OnUpdateDel(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetCurPage()->GetTopicStatus()==CTopicListDlg::PUBLISHED && GetCurPage()->IsValid() && theApp.GetNetworkType()==1);
}
void CTopicPublishDlg::OnUpdatePublish(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetCurPage()->IsValid() && theApp.GetNetworkType()==1);
}

CTopicListDlg* CTopicPublishDlg::GetCurPage()
{
	CTopicListDlg* pDialog=NULL;

	pDialog=static_cast<CTopicListDlg*>(m_tab.GetCurPage());
    
	return pDialog;
}

LRESULT CTopicPublishDlg::OnComboTreeCheck(WPARAM w,LPARAM l)
{
	CString strIDs=m_ctlColumn.GetCheckedIDs();
	//Get ID
	int nIndex=0;
	CString strID;
	CStringList slist;
	
	for(nIndex=strIDs.Find(' ');nIndex!=-1;nIndex=strIDs.Find(' '))
	{
		strID=strIDs.Left(nIndex);
		slist.AddTail(strID);
		
		strIDs=strIDs.Mid(nIndex+1);
		strIDs.TrimLeft();
	}
	slist.AddTail(strIDs);

	for(int i=0;i<m_tab.GetPageCount();i++)
	{
		CTopicListDlg* pDialog=(CTopicListDlg*)m_tab.GetPage(i);
		if(pDialog!=NULL)
		{
			CString str(_T("已发布"));
			BOOL bPublish=str.Compare(m_tab.GetPageTitle(i))==0;
			pDialog->ShowContent(&slist,bPublish?CTopicListDlg::PUBLISHED:CTopicListDlg::UNPUBLISHED);
		}
	}

	return 0;
}

void CTopicPublishDlg::OnFresh() 
{
	// TODO: Add your control notification handler code here
	SendMessage(NOTIFY_TREECOMBO_CHECK);
}


void CTopicPublishDlg::OnExport() 
{
	// TODO: Add your control notification handler code here
	BOOL bRet=FALSE;
	CExportDlg dlg(EXPORT_TXT);
	dlg.m_nType=1;
	dlg.m_strFileDir=theApp.ExportDir();
	if(dlg.DoModal()==IDOK)
	{   
		CWaitCursor wait;
		CString strDir=dlg.m_strFileDir;
		theApp.ExportDir()=strDir;

		CProgressBar bar(_T("导出所有文章内容"),60,5000);

        ofstream stream;
		CString strFile;

		strFile.Format("%s\\%s.txt",strDir,COleDateTime::GetCurrentTime().Format("%Y_%m_%d_%H_%M_%S"));
		CCommonFunction::CreateDir(strFile);
		stream.open(strFile);

		for(int i=0;i<GetCurPage()->m_listTopic.GetItemCount();i++)
		{
			if(GetCurPage()->m_listTopic.GetCheck(i))
			{   
				CTopic* pTopic=(CTopic*)GetCurPage()->m_listTopic.GetItemData(i);
				pTopic->StreamOutTxt(stream,FALSE,&bar);
			}
		}
	    stream.close();
	    bRet=TRUE;

		bar.Finish(_T("导出数据完毕"));
	}
	MessageBox(bRet?_T("所有文章导出成功"):_T("所有文章导出失败"));
}
void CTopicPublishDlg::OnUpdateExport(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetCurPage()->IsValid());
}