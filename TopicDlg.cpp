// TopicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebDataMining.h"
#include "TopicDlg.h"
#include "PictureListDlg.h"
#include "WorkTopic.h"
#include "Regex.h"
#include "Extract.h"
#include "AlterTopicUnitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopicDlg dialog


CTopicDlg::CTopicDlg(CTopic* pTopic,CWnd* pParent /* = NULL */)
	: m_pTopic(pTopic),CDialogEx(CTopicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTopicDlg)
	m_strContent = _T("");
	m_strHeadTitle = _T("");
	m_strSource = _T("");
	m_strSubTitle = _T("");
	m_strSubject = _T("");
	m_strTitle = _T("");
	m_strAuthor = _T("");
	m_strTime = _T("");
	m_bIsPrimary = TRUE;
	m_strKeyWord = _T("");
	m_strMainTitle = _T("");
	//m_bIsElite = FALSE;
	//m_bIsOnTop = FALSE;
	//m_nPrimaryNumber = 0;
	//}}AFX_DATA_INIT
	m_strBigClass = _T("");
	m_strSmallClass = _T("");
	m_nCurKeyWord=0;
	m_arKeyWord.RemoveAll();
	m_pParent=m_pTopic->GetParent();
	m_nWeight = 0;
}
void CTopicDlg::SetValue(CTopic::TopicData& data)
{
	m_strHeadTitle=data.m_strHeadTitle;
	m_strTitle=data.m_strTitle;
	m_strMainTitle=data.m_strMainTitle;
	m_strSubTitle=data.m_strSubTitle;
	m_strKeyWord=data.m_strKeyWord;
	m_strSubject=data.m_strSubject;
	m_strBigClass=data.m_strBigClass;
	m_strSmallClass=data.m_strSmallClass;
	m_strContent=data.m_strContent;
	m_strSource=data.m_strSource;
	m_strTime=data.m_strTime;
	m_strAuthor=data.m_strAuthor;
	m_bIsPrimary=data.m_bIsPrimary;	
	//m_bIsElite=data.m_bIsElite;
	//m_bIsOnTop=data.m_bIsOnTop;
	m_strServerColumnIds=data.m_strServerColumnIDs;
	m_nWeight=data.m_nWeight;
}
BOOL CTopicDlg::GetValue(CTopic::TopicData& data)
{
	BOOL bModify=FALSE;
	if(!CCommonFunction::IsSameString(data.m_strHeadTitle,m_strHeadTitle))
	{
		data.m_strHeadTitle=m_strHeadTitle;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strTitle,m_strTitle))
	{
		data.m_strTitle=m_strTitle;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strMainTitle,m_strMainTitle))
	{
		data.m_strMainTitle=m_strMainTitle;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strSubTitle,m_strSubTitle))
	{
		data.m_strSubTitle=m_strSubTitle;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strKeyWord,m_strKeyWord))
	{
		data.m_strKeyWord=m_strKeyWord;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strSubject,m_strSubject))
	{
		data.m_strSubject=m_strSubject;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strBigClass,m_strBigClass))
	{
		data.m_strBigClass=m_strBigClass;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strSmallClass,m_strSmallClass))
	{
		data.m_strSmallClass=m_strSmallClass;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strContent,m_strContent))
	{
		data.m_strContent=m_strContent;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strSource,m_strSource))
	{
		data.m_strSource=m_strSource;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strTime,m_strTime))
	{
		data.m_strTime=m_strTime;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strAuthor,m_strAuthor))
	{
		data.m_strAuthor=m_strAuthor;
		bModify |= TRUE;
	}
	if(data.m_bIsPrimary!=m_bIsPrimary)
	{
		data.m_bIsPrimary=m_bIsPrimary;
		bModify |= TRUE;
	}
	//if(data.m_bIsElite!=m_bIsElite)
	//{
	//	data.m_bIsElite=m_bIsElite;
	//	bModify |= TRUE;
	//}
	//if(data.m_bIsOnTop!=m_bIsOnTop)
	//{
	//	data.m_bIsOnTop=m_bIsOnTop;
	//	bModify |= TRUE;
	//}
	m_strServerColumnIds=m_ctlColumn.GetCheckedIDs();
	if(!CCommonFunction::IsSameString(data.m_strServerColumnIDs,m_strServerColumnIds))
	{
		data.m_strServerColumnIDs=m_strServerColumnIds;
		bModify |= TRUE;
	}
	if(data.m_nWeight!=m_nWeight)
	{
		data.m_nWeight=m_nWeight;
		bModify |= TRUE;
	}
	return bModify;
}
void CTopicDlg::GetKeyWord()
{
	//提取文章关键词
// 	m_arKeyWord.RemoveAll();
// 
// 	CString strContent=m_pTopic->m_data.m_strTitle+m_pTopic->m_data.m_strContent;
// 	int nCount = ICTCLAS_GetParagraphProcessAWordCount(strContent);
// 	
// 	if( nCount <= 0)
// 		return;
// 	//分词,提取关键词
//   	result_t *result =(result_t*)malloc(sizeof(result_t)*nCount);
//   	ICTCLAS_ParagraphProcessAW(nCount,result);//获取结果存到客户的内存中
// 	
// 	//关键词提取，须在ICTCLAS_ParagraphProcessAW函数执行完后执行
// 	result_t *resultKey = (result_t*)malloc(sizeof(result_t)*nCount);
// 	int nCountKey;
// 	ICTCLAS_KeyWord(resultKey, nCountKey);
// 	
// 	//获得关键词个数
// 	if(theApp.GetTopicTopKeyWordCount() < nCountKey )
// 		nCountKey=theApp.GetTopicTopKeyWordCount();
// 	
// 	for (int i=0; i<nCountKey; i++)
// 	{
// 		char buf[100];
// 		memset(buf, 0, 100);
// 		int index = resultKey[i].start;
// 		memcpy(buf,(void *)(strContent.GetBuffer(0)+index), resultKey[i].length);
//         strContent.ReleaseBuffer();
// 
// 		m_arKeyWord.Add(buf);
// 	}	
// 	free(resultKey);
//	free(result);
	//提取文章关键字
	CWaitCursor wait;
	m_arKeyWord.RemoveAll();
	HANDLE hHandle = HLOpenSplit();
	if(hHandle != INVALID_HANDLE_VALUE)
	{
		int nExtra = 0;
		nExtra |= HL_CAL_OPT_KEYWORD;
		
		HLSplitWord(hHandle,(LPCTSTR)(m_pTopic->m_data.m_strTitle+m_pTopic->m_data.m_strContent),nExtra);//进行分词			
		
		int nKeyCount = HLGetFileKeyCnt(hHandle) ;//获得关键词个数	
		if( nKeyCount > 0 )
		{			
			int nCount=theApp.GetTopicTopKeyWordCount();
			if(nCount>nKeyCount)
				nCount=nKeyCount;

			SHLSegWord* pKey=NULL;			
			for(int i=0;i<nCount;i++)
			{
				pKey = HLGetFileKeyAt(hHandle,i);//获得指定的关键词
				if(pKey)
				{
					m_arKeyWord.Add(pKey->s_szWord);				
				}
			}		
		}
		HLCloseSplit(hHandle);//关闭分词句柄
	}
	m_strKeyWord.TrimLeft();
	m_strKeyWord.TrimRight();
	
	if(!m_strKeyWord.IsEmpty())	
	{
		int n=-1;
		do 
		{
			m_nCurKeyWord++;   
			n=m_strKeyWord.Find(' ',n+1);
		} 
		while( n!=-1 );
	}	
}
void CTopicDlg::Init()
{
	m_arKeyWord.RemoveAll();
	m_nCurKeyWord=0;
	
	m_ctlColumn.CheckAll(FALSE);
	m_ctlColumn.SetChecks(m_pTopic->m_data.m_strServerColumnIDs,TRUE);
	//Set the spin ctrl
	CSpinButtonCtrl* pSpin = ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1));
	pSpin->SetRange(0, 100);

    UDACCEL ua;
	pSpin->GetAccel(1,&ua);
	ua.nInc = ua.nInc*10;
	pSpin->SetAccel(1,&ua);
    //Initial the 
	/*CString strColumn=m_ctlColumn.GetCheckedTexts();
	if(strColumn.CompareNoCase(_T("要闻"))!=0)
	{
	GetDlgItem(IDC_CODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NUMBER)->ShowWindow(SW_HIDE);
	pSpin->ShowWindow(SW_HIDE);
	}*/

	UpdateData(FALSE);
}

void CTopicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTopicDlg)
	DDX_Control(pDX, IDC_COMBO_COLUMN, m_ctlColumn);
	DDX_Control(pDX, IDC_MAIN_TITLE, m_ctlMainTitle);
	DDX_Control(pDX, IDC_TITLE, m_ctlTitle);
	DDX_Control(pDX, IDC_SUB_TITLE, m_ctlSubTitle);
	DDX_Control(pDX, IDC_HEAD_TITLE, m_ctlHeadTitle);
	DDX_Control(pDX, IDC_CONTENT, m_ctlContent);
	DDX_Control(pDX, IDC_AUTHOR, m_ctlAuthor);
	DDX_Text(pDX, IDC_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_HEAD_TITLE, m_strHeadTitle);
	DDX_Text(pDX, IDC_SOURCE, m_strSource);
	DDV_MaxChars(pDX, m_strSource, 100);
	DDX_Text(pDX, IDC_SUB_TITLE, m_strSubTitle);
	DDX_CBString(pDX, IDC_SUBJECT, m_strSubject);
	DDV_MaxChars(pDX, m_strSubject, 100);
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_AUTHOR, m_strAuthor);
	DDV_MaxChars(pDX, m_strAuthor, 50);
	DDX_Text(pDX, IDC_TIME, m_strTime);
	DDX_Check(pDX, IDC_CHECK_PRIMARY2, m_bIsPrimary);
	DDX_CBString(pDX, IDC_COMBO_KEYWORD, m_strKeyWord);
	DDV_MaxChars(pDX, m_strKeyWord, 100);
	DDX_Text(pDX, IDC_MAIN_TITLE, m_strMainTitle);
	//DDX_Check(pDX, IDC_CHECK_ELITE, m_bIsElite);
	//DDX_Check(pDX, IDC_CHECK_ONTOP, m_bIsOnTop);
	//DDX_Text(pDX, IDC_NUMBER, m_nPrimaryNumber);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_WEIGHT, m_nWeight);
	DDV_MinMaxInt(pDX, m_nWeight, 0, 100);
}


BEGIN_MESSAGE_MAP(CTopicDlg, CDialogEx)
	//{{AFX_MSG_MAP(CTopicDlg)
	ON_BN_CLICKED(IDC_VIEW_PICTURE, OnViewPicture)
	ON_BN_CLICKED(IDC_BUTTON_PUBLISH, OnButtonPublish)
	ON_BN_CLICKED(IDC_KEYWORD_PREV, OnKeywordPrev)
	ON_BN_CLICKED(IDC_KEYWORD_NEXT, OnKeywordNext)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_NEW_TOPIC, OnButtonNewTopic)
	ON_BN_CLICKED(IDC_PASTE_CODE, OnPasteCode)
	ON_BN_CLICKED(IDC_FORMAT, OnFormat)
	ON_BN_CLICKED(IDC_FILTER, OnFilter)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PUBLISH,OnUpdatePublish)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_PICTURE,OnUpdateViewPicture)
	ON_UPDATE_COMMAND_UI(IDC_KEYWORD_PREV,OnUpdateKeyWordPrev)
	ON_UPDATE_COMMAND_UI(IDC_KEYWORD_NEXT,OnUpdateKeyWordNext)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PREV,OnUpdateShowPrevTopic)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_NEXT,OnUpdateShowNextTopic)
	ON_UPDATE_COMMAND_UI(IDC_PASTE_CODE,OnUpdatePasteCode)
	ON_MESSAGE(NOTIFY_TREECOMBO_CHECK,OnComboTreeCheck)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopicDlg message handlers

BOOL CTopicDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
    // Init the drag control
	m_ctlTitle.Init();
	m_ctlSubTitle.Init();
	m_ctlMainTitle.Init();
	m_ctlHeadTitle.Init();
	m_ctlContent.Init();
	m_ctlAuthor.Init();
	// Init the auto complete control
	autoKeyWord.Init(GetDlgItem(IDC_COMBO_KEYWORD));
	autoSubject.Init(GetDlgItem(IDC_SUBJECT));
	autoSource.Init(GetDlgItem(IDC_SOURCE));
	autoTime.Init(GetDlgItem(IDC_TIME));
	//初始化服务器栏目信息
    InitColumn(theApp.GetServerColumnRoot(),1);
	//初始化文章信息
    Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTopicDlg::OnViewPicture() 
{
	// TODO: Add your control notification handler code here
	CPictureListDlg dlg(m_pTopic);
	if(dlg.DoModal()==IDOK)
	{
		 if(m_pTopic->IsNew())
			 return ;

         POSITION pos=m_pTopic->GetFirstChildPosition();
		 while (pos)
		 {
			 CBaseObject* pChild=m_pTopic->GetNextChild(pos);
			 if(pChild!=NULL)
			 {
				 if(pChild->IsNew() && pChild->GetRefItem()==NULL )
					 pChild->ShowInTreeView(theApp.GetSourceTree());
				 if(pChild->IsModified())
				    pChild->SaveToDB(*theApp.GetDB(),FALSE);				 
			 }
		 }
	}
	else
	{
		 POSITION pos=m_pTopic->GetFirstChildPosition();
		 while (pos)
		 {
			 CNewsPicture* pPicture=(CNewsPicture*)m_pTopic->GetNextChild(pos);
			 if(pPicture!=NULL && pPicture->IsNew())
			 {
				  //Delete the picture
				  CString strFilePath;
				  strFilePath.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),
					  pPicture->m_data.m_strURL);

				  CCommonFunction::Replace(strFilePath,"/","\\");
				  DeleteFile(strFilePath);
        
				  m_pTopic->GetRidOf(pPicture);
				  if(pPicture->GetRefItem())
					  theApp.GetSourceTree()->DeleteItem(pPicture->GetRefItem());
				  delete pPicture;
				  pPicture=NULL;
			 }
		 }
	}
}

void CTopicDlg::OnButtonPublish() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(GetValue(m_pTopic->m_data))
	{
		m_pTopic->SetModify();
		m_pTopic->ChangeDesc(m_pTopic->m_data.m_strTitle);
	}
    if(m_pTopic->IsNew())
	{
		m_pTopic->SaveToDB(*theApp.GetDB());
		m_pTopic->ShowInTreeView(theApp.GetSourceTree(),TRUE);
	}

	BOOL bPublish=!m_pTopic->m_data.m_bIsPublish;
    if( !bPublish )
	{
		switch(MessageBox(_T("文章：")+m_pTopic->m_data.m_strTitle+_T("\n已经发布，选择“是”为重新发布，选择“否”为从服务器删除"),_T("请选择"),MB_YESNOCANCEL|MB_ICONQUESTION))	
		{
		case IDYES:
			bPublish=TRUE;
			break;
		case IDCANCEL:
			return;
		}
	}	 	
	
    thread_pool::instance().queue_request(new CWorkTopic(m_pTopic,bPublish));
	//request_ptr request = request_ptr( std::auto_ptr<CWorkTopic>(new CWorkTopic(m_pTopic,bPublish)));
	//thread_pool::instance().add_request(request);

}

BOOL CTopicDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	UpdateDialogControls(this,FALSE);
    
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CTopicDlg::OnUpdatePublish(CCmdUI *pCmdUI)
{
	m_pTopic->m_data.m_strServerColumnIDs=m_ctlColumn.GetCheckedIDs();
	pCmdUI->Enable(m_pTopic->IsValid() && theApp.GetNetworkType()==1);
    pCmdUI->SetText(m_pTopic->m_data.m_bIsPublish?_T("已发布"):_T("发布"));
}

void CTopicDlg::OnKeywordPrev() 
{
	// TODO: Add your control notification handler code here
	//删除文章最后一个关键词
    m_strKeyWord=m_strKeyWord.Left(m_strKeyWord.ReverseFind(' '));
	m_nCurKeyWord--;
	UpdateData(FALSE);
}

void CTopicDlg::OnKeywordNext() 
{
	// TODO: Add your control notification handler code here
	//增加文章下一个关键词	
	if( m_arKeyWord.GetSize()==0 )
		GetKeyWord();

	if( m_arKeyWord.GetSize()==0 )
		return;

	m_strKeyWord+=_T(" ")+m_arKeyWord.GetAt(m_nCurKeyWord++);
	m_strKeyWord.TrimLeft();			
	
	UpdateData(FALSE);
}
void CTopicDlg::OnUpdateViewPicture(CCmdUI *pCmdUI)
{
	CString str;
	str.Format("图片%d",m_pTopic->GetChildCount());
    
	pCmdUI->SetText(str);
//	pCmdUI->Enable(m_nResourceCount>0);
}
void CTopicDlg::OnUpdateKeyWordPrev(CCmdUI *pCmdUI)
{
     pCmdUI->Enable(m_nCurKeyWord>0);
}
void CTopicDlg::OnUpdateKeyWordNext(CCmdUI *pCmdUI)
{
     pCmdUI->Enable( m_arKeyWord.GetSize()==0 || m_nCurKeyWord<m_arKeyWord.GetSize()-1);
}

void CTopicDlg::OnButtonView() 
{
	// TODO: Add your control notification handler code here
	CString strPath,strTopicID;

	if( !m_pTopic->m_data.m_strServerAutoID.IsEmpty() )
	{
		strTopicID = m_pTopic->m_data.m_strServerAutoID;
	}
	else
	{
		strTopicID = m_pTopic->GetID();
	}
	strPath.Format("%s%s",theApp.GetTopicViewPath(),strTopicID);
	ShellExecute(NULL,"open",strPath,NULL,NULL,SW_SHOWNORMAL);	
}

void CTopicDlg::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	// http://21.114.11.9/manager/EditArticle.aspx?id=205FC6B9-39D3-42ef-9AA6-AAD66DF3346A&classid=9cf682c2-401a-4e73-8614-17b04167fe98
	
	//CString strPath,strServerColumnID,strTopicID;
	//AfxExtractSubString(strServerColumnID,m_pTopic->m_data.m_strServerColumnIDs,0,' ');

	//if( m_pTopic->m_data.m_nServerAutoID >0 )
	//{
	//	strTopicID.Format("%d",m_pTopic->m_data.m_nServerAutoID);
	//}
	//else
	//{
	//	strTopicID = m_pTopic->GetID();
	//}
	//strPath.Format("%s%s&classid=%s",theApp.GetTopicModifyPath(),strTopicID,strServerColumnID);

	CString strPath,strTopicID;

	if( !m_pTopic->m_data.m_strServerAutoID.IsEmpty() )
	{
		strTopicID = m_pTopic->m_data.m_strServerAutoID;
	}
	else
	{
		strTopicID = m_pTopic->GetID();
	}
	strPath.Format("%s%s",theApp.GetTopicModifyPath(),strTopicID);
	ShellExecute(NULL,"open",strPath,NULL,NULL,SW_SHOWNORMAL);	
}

void CTopicDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(GetValue(m_pTopic->m_data))
	{
		m_pTopic->SetModify();
		m_pTopic->ChangeDesc(m_pTopic->m_data.m_strTitle);
	}

    if(m_pTopic->IsNew())
	{		
		if(MessageBox(_T("是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
			m_pTopic->SaveToDB(*theApp.GetDB());
		m_pTopic->ShowInTreeView(theApp.GetSourceTree(),TRUE);
	}
	CDialogEx::OnOK();
}
void CTopicDlg::InitColumn(CBaseObject* pRoot,int nLevel)
{
	POSITION pos=pRoot->GetFirstChildPosition();
	while (pos)
	{		
		CBaseObject* pChild=pRoot->GetNextChild(pos);
		if(pChild!=NULL)
		{
			if(pChild->GetChildCount()>0)
			    m_ctlColumn.AddString(pChild->GetDesc(),INVALID_ID,ROOT_LEVEL+nLevel);
			else
				m_ctlColumn.AddString(pChild->GetDesc(),pChild->GetID(),ROOT_LEVEL+nLevel);

			InitColumn(pChild,nLevel+1);
		}
	}

}

LRESULT CTopicDlg::OnComboTreeCheck(WPARAM w,LPARAM l)
{
/*    CString strSelColumn=m_ctlColumn.GetCheckedTexts();
	BOOL bShow=strSelColumn.CompareNoCase(_T("要闻"))==0;
	
	GetDlgItem(IDC_CODE)->ShowWindow(bShow?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_NUMBER)->ShowWindow(bShow?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_SPIN1)->ShowWindow(bShow?SW_SHOW:SW_HIDE);*/	

	return 0;
}
void CTopicDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetValue(m_pTopic->m_data);

	POSITION pos=m_pParent->GetChildPosition(m_pTopic);
	//Get rid of self
	m_pParent->GetPrevChild(pos);
	m_pTopic=(CTopic*)(m_pParent->GetPrevChild(pos));
    SetValue(m_pTopic->m_data);
	Init();
}

void CTopicDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetValue(m_pTopic->m_data);

	POSITION pos=m_pParent->GetChildPosition(m_pTopic);
	//Get rid of self
	m_pParent->GetNextChild(pos);
	m_pTopic=(CTopic*)(m_pParent->GetNextChild(pos));
	SetValue(m_pTopic->m_data);
	Init();
}

void CTopicDlg::OnButtonNewTopic() 
{
	// TODO: Add your control notification handler code here
	//关闭原来对话框
    SendMessage(WM_COMMAND,IDCANCEL);

	m_pTopic=new CTopic;

	m_pTopic->IsNew()=TRUE;
	m_pTopic->SetID(CCommonFunction::AllocateID());
	m_pTopic->m_data.m_timeDownload=COleDateTime::GetCurrentTime();

	m_pTopic->AddTo(m_pParent);
	if(m_pTopic->EditObject(this,FALSE)==IDCANCEL)
	{		
		if(m_pTopic->IsNew())
		{
			POSITION pos=m_pTopic->GetFirstChildPosition();
			while (pos)
			{
				CNewsPicture* pPicture=(CNewsPicture*)m_pTopic->GetNextChild(pos);
				if(pPicture!=NULL)
				{
					CString strPath;
					strPath.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),
						pPicture->m_data.m_strURL);
					CCommonFunction::Replace(strPath,"/","\\");
					DeleteFile(strPath);
				}
			}
			m_pParent->GetRidOf(m_pTopic);
			delete m_pTopic;
			m_pTopic=NULL;
		}		
	}
}
void CTopicDlg::OnUpdateShowPrevTopic(CCmdUI *pCmdUI)
{
	POSITION pos=m_pParent->GetChildPosition(m_pTopic);
	m_pParent->GetPrevChild(pos);
	CBaseObject* pOb=m_pParent->GetPrevChild(pos);

	pCmdUI->Enable(pOb!=NULL);
}
void CTopicDlg::OnUpdateShowNextTopic(CCmdUI *pCmdUI)
{
	POSITION pos=m_pParent->GetChildPosition(m_pTopic);
	m_pParent->GetNextChild(pos);
	CBaseObject* pOb=m_pParent->GetNextChild(pos);
	
	pCmdUI->Enable(pOb!=NULL);
}

void CTopicDlg::OnPasteCode() 
{
	// TODO: Add your control notification handler code here
	HGLOBAL hGlobal;		// Global memory handle
	LPSTR lpszData;			// Pointer to clipboard data
    //Firstly,register the CF_HTML
	UINT CF_HTML=RegisterClipboardFormat(_T("HTML Format"));
	// Secondly, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	OpenClipboard();

	// Request a pointer to the text on the clipboard.
	hGlobal = GetClipboardData(CF_HTML);

	// If there was no text on the clipboard, we have
	// been returned a NULL handle.	
	if (hGlobal == NULL) 
	{
		CloseClipboard();
		return ;
	}

	// Now we have a global memory handle to the text
	// stored on the clipboard. We have to lock this global
	// handle so that we have access to it.
	lpszData = (LPSTR )GlobalLock(hGlobal);

    // Retrieve the useful information 
	// Get SourceURL
	CString strContent;  

    //utf8 to gbk
	CString strData(lpszData);
	strData=CChineseCodeLib::Utf8ToGBK(strData);    
 	// Get useful info between <!--StartFragment--> and <!--EndFragment-->	
    strContent=CRegex::Search(strData,_T("<!--StartFragment-->(.*?)<!--EndFragment-->"));
 	//Insert into the content
	//set dropped point
	//set caret position
	CPoint pt;
	pt=m_ctlContent.GetCaretPos();
    int nChar=(int)LOWORD((DWORD)m_ctlContent.CharFromPos(pt));
    m_ctlContent.SetSel(nChar,nChar);
	//insert string and select the inserted string
	int nBeg, nEnd;
	m_ctlContent.GetSel(nBeg,nEnd);        
	nEnd+=strContent.GetLength();
	m_ctlContent.ReplaceSel(strContent,TRUE);
	m_ctlContent.SetSel(nBeg,nEnd);
	// Now, simply unlock the global memory pointer
	// and close the clipboard.
	GlobalUnlock(hGlobal);
	EmptyClipboard();
	CloseClipboard();
}
void CTopicDlg::OnUpdatePasteCode(CCmdUI *pCmdUI)
{
	HGLOBAL hGlobal;		// Global memory handle
	LPSTR lpszData;			// Pointer to clipboard data
	unsigned long nSize=0;	// Size of clipboard data

    //Firstly,register the CF_HTML
	UINT CF_HTML=RegisterClipboardFormat(_T("HTML Format"));
	// Secondly, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	OpenClipboard();

	// Request a pointer to the text on the clipboard.
	hGlobal = GetClipboardData(CF_HTML);

	// If there was no text on the clipboard, we have
	// been returned a NULL handle.	
	if (hGlobal != NULL) 
	{
		// Now we have a global memory handle to the text
		// stored on the clipboard. We have to lock this global
		// handle so that we have access to it.
		lpszData = (LPSTR)GlobalLock(hGlobal);
		nSize = GlobalSize(hGlobal);
		// Now, simply unlock the global memory pointer	
		GlobalUnlock(hGlobal);	
	}
	// and close the clipboard.
	CloseClipboard();	
   
    pCmdUI->Enable( nSize>0 );
}

void CTopicDlg::OnFormat() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox(_T("是否格式化文章内容"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
		
	UpdateData();
	
	CExtract e;
	std::string s = m_strContent;
	e.GetPureText(s);
	m_strContent =s .c_str();

	UpdateData(FALSE);
}

void CTopicDlg::OnFilter() 
{
	// TODO: Add your control notification handler code here
	CAlterTopicUnitDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		UpdateData();

		int nTopicUnit=dlg.m_nTopicUnit;
		int nProcessType=dlg.m_nProcessType;
		CString strValue=dlg.m_strValue;
		
		m_pTopic->AlterTopic(nTopicUnit,nProcessType,strValue);				

	    SetValue(m_pTopic->m_data);
        UpdateData(FALSE);
	}
}


void CTopicDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	if(MessageBox(_T("是否保存文章所有内容"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	UpdateData();
	if(GetValue(m_pTopic->m_data))
	{
		m_pTopic->SetModify();
		m_pTopic->ChangeDesc(m_pTopic->m_data.m_strTitle);
	}
	m_pTopic->SaveToDB(*theApp.GetDB());
}
