// EntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "EntryDlg.h"
#include "RuleGroupDlg.h"
#include "Regex.h"
#include "RegexWDM.h"
#include "DownloadHttpFile.h"
#include "Url.h"
#include "MainFrm.h"
#include "CharsetDetector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntryDlg dialog


CEntryDlg::CEntryDlg(BOOL bReadOnly,CWnd* pParent /* = NULL */)
	: CSetReadOnlyDialog(bReadOnly,CEntryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntryDlg)
	m_bAutoPublish = FALSE;
	m_strEntryName = _T("");
	m_strRule = _T("");
	m_nTopicMax = 0;
	m_nTryTimes = 0;
	m_strURL = _T("");
	m_strWebName = _T("");
	m_strServerColumnID = _T("");
	//}}AFX_DATA_INIT
	m_strRuleID= _T("");
	m_strEntryID= _T("");
}


void CEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CSetReadOnlyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntryDlg)
	DDX_Control(pDX, IDC_COMBO_RULE, m_comboRule);
	DDX_Check(pDX, IDC_AUTO_PUBLISH, m_bAutoPublish);
	DDX_Text(pDX, IDC_ENTRY_NAME, m_strEntryName);
	DDX_Text(pDX, IDC_RULE, m_strRule);
	DDV_MaxChars(pDX, m_strRule, 300);
	DDX_Text(pDX, IDC_TOPICMAX, m_nTopicMax);
	DDV_MinMaxInt(pDX, m_nTopicMax, 0, 100);
	DDX_Text(pDX, IDC_TRYTIMES, m_nTryTimes);
	DDV_MinMaxInt(pDX, m_nTryTimes, 0, 10);
	DDX_Text(pDX, IDC_URL, m_strURL);
	DDV_MaxChars(pDX, m_strURL, 100);
	DDX_Text(pDX, IDC_WEB_NAME, m_strWebName);
	DDX_Text(pDX, IDC_SERVER_COLUMNID, m_strServerColumnID);
	//}}AFX_DATA_MAP
}

void CEntryDlg::SetValue(CEntry::EntryData& data)
{
	m_strEntryName=data.m_strEntryName;
	m_strWebName=data.m_strWebSiteName;
	m_strRule=data.m_strMatchRule;
	m_strURL=data.m_strURL;
	m_nTopicMax=data.m_nTopicMax;
	m_nTryTimes=data.m_nTryTimes;
	m_bAutoPublish=data.m_bAutoPublish;
	m_strRuleID=data.m_strRuleID;
	m_strEntryID=data.m_strEntryID;
	m_strServerColumnID=data.m_strServerColumnID;
}
BOOL CEntryDlg::GetValue(CEntry::EntryData& data)
{
	BOOL bModify=FALSE;
	if(!CCommonFunction::IsSameString(data.m_strEntryName,m_strEntryName))
	{
	    data.m_strEntryName=m_strEntryName;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strWebSiteName,m_strWebName))
	{
    	data.m_strWebSiteName=m_strWebName;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strMatchRule,m_strRule))
	{
		data.m_strMatchRule=m_strRule;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strURL,m_strURL))
	{
		data.m_strURL=m_strURL;
		bModify |= TRUE;
	}
	if(data.m_nTopicMax!=m_nTopicMax)
	{
		data.m_nTopicMax=m_nTopicMax;
		bModify |= TRUE;
	}
	if(data.m_nTryTimes!=m_nTryTimes)
	{
    	data.m_nTryTimes=m_nTryTimes;
		bModify |= TRUE;
	}
	if(data.m_bAutoPublish!=m_bAutoPublish)
	{
		data.m_bAutoPublish=m_bAutoPublish;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strRuleID,m_strRuleID))
	{
		data.m_strRuleID=m_strRuleID;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strServerColumnID,m_strServerColumnID))
	{
		data.m_strServerColumnID=m_strServerColumnID;
		bModify |= TRUE;
	}
	
	return bModify;
}
BEGIN_MESSAGE_MAP(CEntryDlg, CSetReadOnlyDialog)
	//{{AFX_MSG_MAP(CEntryDlg)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntryDlg message handlers

BOOL CEntryDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CSetReadOnlyDialog::PreTranslateMessage(pMsg);
}



void CEntryDlg::OnButtonTest() 
{
	// TODO: Add your control notification handler code here
	CWaitCursor wait;
	UpdateData();
	
	char* buf=NULL;
	int nLen=0;
    CFile file;
    CString strURL=CRegexWDM::TimeFormatStrConvert(m_strURL);
	
	CDownloadHttpFile download(static_cast<CMainFrame*>(AfxGetMainWnd())->GetOutPutWindowHWND());
	download.Fetch(strURL,&buf,nLen);	
	
	if(nLen>0)
	{				
		CString strBuf(buf);
		//编码转换 Added at 2019.04.11
		CCharsetDetector detecotr;
		charset_t ct = detecotr.GetCharset(buf,nLen);
		TRACE("The page code is %s\n",Charset::TypeToName(ct).c_str());

		if( ct == Charset::UNKNOWN && !m_strRuleID.IsEmpty() )
		{
			//如自动获取编码失败，则使用手动设置模式，避免乱码出现
			CRule* pRule=(CRule*)theApp.GetRuleTree()->m_objectHelper.FindSubObject(OBJECT_TYPE_RULE,m_strRuleID);
			if( pRule && pRule->m_data.m_nRuleEncoding == 1 )
				ct = Charset::UTF_8;
		}

		switch ( ct )
		{
		case Charset::UTF_8:
			strBuf=CChineseCodeLib::Utf8ToGBK(strBuf);
			break;
		case Charset::BIG5:
			strBuf = CChineseCodeLib::Big5ToGBK(strBuf);
			break;
		}
		free(buf);
		buf=NULL;		
		
		CString str=CRegex::Search(strBuf,m_strRule,TRUE);		
		//Process the url
		CString strBaseURL=CRegexWDM::GetWebPageBaseURL(strBuf);
		if(strBaseURL.IsEmpty())
	        strBaseURL = strURL;

		CArray<HREF*,HREF*> array;
		CRegexWDM::FetchHrefs(str,array,strBaseURL);		

		file.Open("result.txt",CFile::modeCreate |CFile::modeWrite);
		for(int i=0;i<array.GetSize();i++)
		{	
			file.Write(array.GetAt(i)->m_strLink,array.GetAt(i)->m_strLink.GetLength());
			file.Write("\r\n",2);
			file.Write(array.GetAt(i)->m_strContent,array.GetAt(i)->m_strContent.GetLength());
			file.Write("\r\n",2);
		}
		file.Close();

		//clear memory
		for(int i=0;i<array.GetSize();i++)
		{
			delete array[i];
		}
		
		ShellExecute(NULL,"open","result.txt",NULL,NULL,SW_SHOWNORMAL);	
	}

}

BOOL CEntryDlg::OnInitDialog() 
{
	CSetReadOnlyDialog::OnInitDialog();
    //Set a invalid value
	m_comboRule.SetItemData(m_comboRule.AddString(_T("通用规则")),0);

	CBaseObject* pRuleRoot=&(theApp.GetRuleTree()->m_objectHelper);
	
	CBaseObject *pOb=NULL;
	POSITION pos=pRuleRoot->GetFirstChildPosition();
	while (pos!=NULL)
	{
        pOb=pRuleRoot->GetNextChild(pos);
        if( pOb!=NULL && pOb->GetType()==OBJECT_TYPE_RULE)
		{
			m_comboRule.SetItemData(m_comboRule.AddString(pOb->GetDesc()),(DWORD)pOb);
		}
	}
	m_comboRule.SetCurSel(0);
	pOb=NULL;
	for(int i=0;i<m_comboRule.GetCount();i++)
	{
		pOb=(CBaseObject*)m_comboRule.GetItemData(i);
		if(pOb!=NULL && pOb->GetID().Compare(m_strRuleID)==0)
		{
			m_comboRule.SetCurSel(i);
			break;
		}
	}	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntryDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
    if(m_strEntryName.IsEmpty())
	{
		MessageBox("条目名不能为空!","请注意",MB_OK | MB_ICONWARNING);
		return;
	}
	int idx=m_comboRule.GetCurSel();
	CBaseObject* pOb=((CBaseObject*)m_comboRule.GetItemData(idx));

	if( idx !=LB_ERR && pOb!=NULL)
	{
		m_strRuleID=pOb->GetID();
	}
    else
	{
		m_strRuleID=_T("");
	} 
	
	CSetReadOnlyDialog::OnOK();
}

void CEntryDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CSetReadOnlyDialog::OnCancel();
}

