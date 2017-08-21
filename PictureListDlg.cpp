// PictureListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "PictureListDlg.h"
#include "PictureDlg.h"
#include "Topic.h"
#include "AlterPictureDlg.h"
#include "Regex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureListDlg dialog


CPictureListDlg::CPictureListDlg(CTopic* pTopic,CWnd* pParent /* = NULL */)
	:m_pTopic(pTopic),CDialogEx(CPictureListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPictureListDlg)
	//}}AFX_DATA_INIT
}

void CPictureListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureListDlg)
	DDX_Control(pDX, IDC_LIST_PICTURE, m_listPictures);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPictureListDlg, CDialogEx)
	//{{AFX_MSG_MAP(CPictureListDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PICTURE, OnDblclkListPicture)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_FILTER, OnFilter)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_DELETE,OnUpdateDel)
	ON_UPDATE_COMMAND_UI(IDC_FILTER,OnUpdateFilter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureListDlg message handlers

BOOL CPictureListDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listPictures.SetExtendedStyle(m_listPictures.GetExtendedStyle()
		  | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT /*|LVS_EX_ONECLICKACTIVATE |LVS_EX_CHECKBOXES */);

	m_listPictures.InsertColumn(0,"序号",LVCFMT_LEFT,45);
	m_listPictures.InsertColumn(1,"图片路径",LVCFMT_LEFT,200);

	m_listPictures.InsertColumn(2,"图片说明",LVCFMT_LEFT,300);

    ShowContent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPictureListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogEx::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
// 	if (m_listPictures.GetSafeHwnd())
// 	{
// 		m_listPictures.MoveWindow(10,10,cx-10,cy);
// 	}
}


void CPictureListDlg::OnDblclkListPicture(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem=pNMListView->iItem;
	
	if(nItem!=-1)
	{
        CNewsPicture* pPicture=(CNewsPicture*)m_listPictures.GetItemData(nItem);
		pPicture->EditObject(this);
           
		if(pPicture->IsModified())
		{
			m_listPictures.SetItemText(nItem,1,pPicture->m_data.m_strURL);
			m_listPictures.SetItemText(nItem,2,pPicture->m_data.m_strExplain);

			m_listPictures.UpdateData(FALSE);
		}
	}	
	*pResult = 0;
}

void CPictureListDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("删除选择的图片,确定吗？","请注意",MB_YESNO|MB_ICONWARNING)==IDNO)
		return ;
    
	CArray<int,int> selectedItems;
	CString strPicURL;
	POSITION pos=m_listPictures.GetFirstSelectedItemPosition();
	while (pos)
	{
		int i=m_listPictures.GetNextSelectedItem(pos);
		CNewsPicture * pPicture=(CNewsPicture*)m_listPictures.GetItemData(i);
		//Delete the data from database
		if(theApp.DBIsOpen())
            pPicture->DeleteFromDB(*theApp.GetDB());
		//Delete the data from the tree
		if(pPicture->GetRefItem())
		    theApp.GetSourceTree()->DeleteItem(pPicture->GetRefItem());
		//Delete the data from the memory
		m_pTopic->GetRidOf(pPicture);
		//Delete the picture from disk
		strPicURL.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),
			pPicture->m_data.m_strURL);
		CCommonFunction::Replace(strPicURL,"/","\\");
		DeleteFile(strPicURL);
		delete pPicture; 			
        //save the index value in order to delete data from list ctrl
		selectedItems.Add(i);
	}
    m_pTopic->UpdateTopic();

	for(int i=0;i<selectedItems.GetSize();i++)
	{
    	m_listPictures.DeleteItem(selectedItems.GetAt(i));
		//update the other pos
		for(int j=0;j<selectedItems.GetSize();j++)
		{
			if(selectedItems.GetAt(j)>selectedItems.GetAt(i))
				selectedItems.SetAt(j,selectedItems.GetAt(j)-1);
		}
	}
}

BOOL CPictureListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CPictureListDlg::OnUpdateDel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_listPictures.GetSelectedCount()>0);
}

void CPictureListDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	
	CNewsPicture* pPicture=new CNewsPicture;
	pPicture->IsNew()=TRUE;

	if(pPicture->EditObject(this,FALSE)==IDOK)
	{
		pPicture->AddTo(m_pTopic);

		CString str;
		int nIndex=-1;
		nIndex=m_listPictures.GetItemCount();
		str.Format("%d",nIndex+1);

        m_listPictures.InsertItem(nIndex,str);
		m_listPictures.SetItemText(nIndex,1,pPicture->GetDesc());
		m_listPictures.SetItemText(nIndex,2,pPicture->m_data.m_strExplain);

		m_listPictures.SetItemData(nIndex,(DWORD)pPicture);

	}
	else
	{
		delete pPicture;
		pPicture=NULL;
	}
}

void CPictureListDlg::OnFilter() 
{
	// TODO: Add your control notification handler code here
	CAlterPictureDlg dlg;
    
	if( dlg.DoModal() == IDOK )
	{
		CString strKeyWord = dlg.m_strValue;
        int nType = dlg.m_nProcessType;		

		POSITION pos = m_pTopic->GetFirstChildPosition();
		while (pos)
		{
			CNewsPicture* pPic = (CNewsPicture*)m_pTopic->GetNextChild(pos);
			if( pPic != NULL )
			{
				switch( nType )
				{	
				case 0: //删除前面所有内容		
				case 1: //删除后面所有内容		
				case 2: //删除本身所有内容
					if(!strKeyWord.IsEmpty())
					{						
						pPic->m_data.m_strExplain = CRegex::Filter(pPic->m_data.m_strExplain,strKeyWord,(CRegex::FILTER_TYPE)nType);
					}
					break;
				case 3: //替换
					pPic->m_data.m_strExplain = strKeyWord;
					break;
				default:
					ASSERT(FALSE);
				}
				pPic->SaveToDB(*theApp.GetDB(),FALSE);
			}
		}
		ShowContent();       
	}
}
void CPictureListDlg::OnUpdateFilter(CCmdUI* pCmdUI)
{
    pCmdUI->Enable( m_pTopic->GetChildCount() > 0 );
}

void CPictureListDlg::ShowContent()
{
	m_listPictures.DeleteAllItems();

    POSITION pos=m_pTopic->GetFirstChildPosition();
	int i=0;
	CString str;

	while (pos)
	{
		CNewsPicture* pPicture=(CNewsPicture*)m_pTopic->GetNextChild(pos);
		if(pPicture!=NULL)
		{
			str.Format("%d",i+1);
			m_listPictures.InsertItem(i,str);

			m_listPictures.SetItemText(i,1,pPicture->GetDesc());
			m_listPictures.SetItemText(i,2,pPicture->m_data.m_strExplain.Left(300));
            
			m_listPictures.SetItemData(i++,(DWORD)pPicture);
		}
	}
}
