// UrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "UrlDlg.h"
#include "Regex.h"
#include "RegexWDM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUrlDlg dialog


CUrlDlg::CUrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUrlDlg)
//	m_strFileDir = _T("");
	m_strURL = _T("");
	//}}AFX_DATA_INIT
	m_strSelColumn=_T("");
	m_strSelEntry=_T("");
}


void CUrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUrlDlg)
	DDX_Control(pDX, IDC_COMBO_RULE, m_comboRule);
	DDX_Control(pDX, IDC_LIST_URL, m_listUrls);
//	DDX_Text(pDX, IDC_DIR, m_strFileDir);
	DDX_Text(pDX, IDC_URL, m_strURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUrlDlg, CDialogEx)
	//{{AFX_MSG_MAP(CUrlDlg)
	ON_BN_CLICKED(IDC_CLEAR_URL, OnClearUrl)
	ON_BN_CLICKED(IDC_DEL_URL, OnDelUrl)
	ON_BN_CLICKED(IDC_PASTE_URL_CLIPBOARD, OnPasteUrlClipboard)
//	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_ADD_URL, OnAddUrl)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_CLEAR_URL,OnUpdateClearUrl)
	ON_UPDATE_COMMAND_UI(IDC_DEL_URL,OnUpdateDelUrl)
	ON_UPDATE_COMMAND_UI(IDC_PASTE_URL_CLIPBOARD,OnUpdatePasteUrlClipboard)
	ON_UPDATE_COMMAND_UI(IDC_ADD_URL,OnUpdateAddURL)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlDlg message handlers

BOOL CUrlDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listUrls.SetExtendedStyle(m_listUrls.GetExtendedStyle()
		  | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    
	m_listUrls.InsertColumn(0,_T("下载文章标题"),LVCFMT_LEFT,670);
	//////////////////////////////////////////////////////////////////////////
    // Insert into rule data
	// 首先增加通用规则选项
	m_comboRule.SetItemData(m_comboRule.AddString(_T("通用规则")),0);

	CBaseObject* pOb;
    POSITION pos=theApp.GetRuleTree()->m_objectHelper.GetFirstChildPosition();
	while (pos)
	{
        pOb=(CBaseObject*)theApp.GetRuleTree()->m_objectHelper.GetNextChild(pos);
		if(pOb!=NULL)
		{
			m_comboRule.SetItemData(m_comboRule.AddString(pOb->GetDesc()),(DWORD)pOb);
		}
	}
	
	m_pEntry=(CEntry*)theApp.GetSourceTree()->GetSelectedObject();	
	//select rule of the entry
	m_comboRule.SetCurSel(0);
	CRule*pEntryRule=m_pEntry->GetRule();
	if(pEntryRule!=NULL)
	{            
		for(int i=1;i<m_comboRule.GetCount();i++)
		{
			CRule* pRule=(CRule*)m_comboRule.GetItemData(i);
			if(pRule && pRule->GetID().Compare(pEntryRule->GetID())==0)
			{
				m_comboRule.SetCurSel(i);
				break;
			}
		}
	}		

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUrlDlg::OnClearUrl() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("清空列表中所有下载文章地址,确定吗？","请注意",MB_YESNO|MB_ICONWARNING)==IDNO)
		return ;

     m_listUrls.DeleteAllItems();
	 m_mapURL.RemoveAll();
}

void CUrlDlg::OnDelUrl() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("删除选择的下载文章地址,确定吗？","请注意",MB_YESNO|MB_ICONWARNING)==IDNO)
		return ;
    
	CArray<int,int> selectedItems;
	POSITION pos=m_listUrls.GetFirstSelectedItemPosition();
	while (pos)
	{
		int i=m_listUrls.GetNextSelectedItem(pos);
        //save the index value in order to delete data from list ctrl
		selectedItems.Add(i);
	}
	for(int i=0;i<selectedItems.GetSize();i++)
	{
		//delete from map
		m_mapURL.RemoveKey(m_listUrls.GetItemText(selectedItems.GetAt(i),0));

    	m_listUrls.DeleteItem(selectedItems.GetAt(i));
		//update the other pos
		for(int j=0;j<selectedItems.GetSize();j++)
		{
			if(selectedItems.GetAt(j)>selectedItems.GetAt(i))
				selectedItems.SetAt(j,selectedItems.GetAt(j)-1);
		}
	}
}

void CUrlDlg::OnPasteUrlClipboard() 
{
	// TODO: Add your control notification handler code here
	//clear old content
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
	CString strUrl,strContent;  

    //Retrieve the Source url
	CString strData(lpszData);
	strData=CChineseCodeLib::Utf8ToGBK(strData);

    strUrl=CRegex::Search(strData,_T("SourceURL:(.*?)<"));
 	// Get useful info between <!--StartFragment--> and <!--EndFragment-->	
    strContent=CRegex::Search(strData,_T("<!--StartFragment-->(.*?)<!--EndFragment-->"));
 	CArray<HREF*,HREF*> listHrefs;
 	// Get urls
 	CRegexWDM::FetchHrefs(strContent,listHrefs,strUrl);  

	for(int i=0;i<listHrefs.GetSize();i++)
	{
		//show  content 
		strUrl=listHrefs.GetAt(i)->m_strLink;	
		strContent=listHrefs.GetAt(i)->m_strContent; 

		m_mapURL.SetAt(strContent,strUrl);

		m_listUrls.InsertItem(m_listUrls.GetItemCount(),strContent);

		delete listHrefs.GetAt(i);
	}
	// Now, simply unlock the global memory pointer
	// and close the clipboard.
	GlobalUnlock(hGlobal);
	EmptyClipboard();
	CloseClipboard();
}

void CUrlDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	m_pRule=(CRule*)m_comboRule.GetItemData(m_comboRule.GetCurSel());
	CString str;
	str.Format("当前提取规则：%s,确定吗?",( m_pRule == NULL )?_T("通用规则"):m_pRule->GetDesc());

    if(MessageBox(str,NULL,MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	CDialogEx::OnOK();
}

BOOL CUrlDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CUrlDlg::OnUpdateClearUrl(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_listUrls.GetItemCount()>0);
}
void CUrlDlg::OnUpdateDelUrl(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_listUrls.GetSelectedCount()>0);
}
void CUrlDlg::OnUpdatePasteUrlClipboard(CCmdUI* pCmdUI)
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

    pCmdUI->Enable(nSize>0);
}

int CUrlDlg::GetUrls(CStringArray& arrayUrl)
{
	POSITION pos=m_mapURL.GetStartPosition();
    CString strKey,strValue;
	while (pos)
	{
		m_mapURL.GetNextAssoc(pos,strKey,strValue);
		arrayUrl.Add(strValue);
	}
 
	return arrayUrl.GetSize();
}

// void CUrlDlg::OnSelchangeComboTopcolumn() 
// {
// 	// TODO: Add your control notification handler code here
// 	int nIndex=m_comboTopColumn.GetCurSel();
// 	if(nIndex==CB_ERR)
// 		return;
// 
//     m_comboColumn.ResetContent();
// 
//   	CBaseObject* pOb=(CBaseObject*)m_comboTopColumn.GetItemData(nIndex);
// 	POSITION pos=pOb->GetFirstChildPosition();
// 	while (pos)
// 	{
// 		CBaseObject* pChild=pOb->GetNextChild(pos);
// 		if(pOb!=NULL)
// 		{
// 			m_comboColumn.SetItemData(m_comboColumn.AddString(pChild->GetDesc()),(DWORD)pChild);
// 		}
// 	}
//     if(m_comboColumn.SelectString(-1,m_strSelColumn) == CB_ERR )
// 		m_comboColumn.SetCurSel(0);
//         
// 	OnSelchangeComboColumn();
// }
// 
// void CUrlDlg::OnSelchangeComboColumn() 
// {
// 	// TODO: Add your control notification handler code here
// 	int nIndex=m_comboColumn.GetCurSel();
// 	if(nIndex==CB_ERR)
// 		return;
// 
//     m_comboEntry.ResetContent();
// 
// 	CBaseObject* pOb=(CBaseObject*)m_comboColumn.GetItemData(nIndex);
// 	POSITION pos=pOb->GetFirstChildPosition();
// 	while (pos)
// 	{
// 		CBaseObject* pChild=pOb->GetNextChild(pos);
// 		if(pOb!=NULL)
// 		{
// 			m_comboEntry.SetItemData(m_comboEntry.AddString(pChild->GetDesc()),(DWORD)pChild);
// 		}
// 	}
//     if(m_comboEntry.SelectString(-1,m_strSelEntry) == CB_ERR)
// 	    m_comboEntry.SetCurSel(0);
// 
// 	OnSelchangeComboEntry();
// }
// 
// void CUrlDlg::OnSelchangeComboEntry() 
// {
// 	// TODO: Add your control notification handler code here
// 	int nIndex=m_comboEntry.GetCurSel();
// 	if(nIndex==CB_ERR)
// 		return;
// 
// 	CEntry* pEntry=NULL;
// 	CRule*pEntryRule=NULL;
// 	BOOL bFound=FALSE;
// 	
// 	pEntry=(CEntry*)m_comboEntry.GetItemData(nIndex);
// 	pEntryRule=pEntry->GetRule();
// 	if(pEntryRule!=NULL)
// 	{            
// 		for(int i=0;i<m_comboRule.GetCount();i++)
// 		{
// 			CRule* pRule=(CRule*)m_comboRule.GetItemData(i);
// 			if(pRule->GetID().Compare(pEntryRule->GetID())==0)
// 			{
// 				bFound=TRUE;
// 				m_comboRule.SetCurSel(i);
// 				break;
// 			}
// 		}
// 	}
// 	
// 	if(bFound==FALSE)
// 		m_comboRule.SetCurSel(0);
// }

//void CUrlDlg::OnButtonBrowse() 
//{
//	// TODO: Add your control notification handler code here
//	theApp.GetShellManager()->BrowseForFolder(m_strFileDir,this,m_strFileDir,_T("选择文件夹"));
//
//	UpdateData(FALSE);	
//}

void CUrlDlg::OnAddUrl() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

    CString str;
	if(m_mapURL.Lookup(m_strURL,str))
	{
		MessageBox(_T("地址已经加入！"));
	}
	else
	{
		m_mapURL.SetAt(m_strURL,m_strURL);
		m_listUrls.InsertItem(m_listUrls.GetItemCount(),m_strURL);
	}
	
	m_strURL.Empty();
	UpdateData(FALSE);
}
void CUrlDlg::OnUpdateAddURL(CCmdUI* pCmdUI)
{
	UpdateData();
	pCmdUI->Enable(!m_strURL.IsEmpty());
}
