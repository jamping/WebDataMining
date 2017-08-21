// UserLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "UserLoginDlg.h"
#include "Des.h"
#include "Column.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog


CUserLoginDlg::CUserLoginDlg(BOOL bAutoClose/* =FALSE */,CWnd* pParent /* = NULL */)
	:CExpandingDialog(CUserLoginDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_ADVANCED,
			_T(">>>"),_T("<<<")),m_bAutoClose(bAutoClose)
{
	//{{AFX_DATA_INIT(CUserLoginDlg)
	m_nUserLevel = -1;
	m_bSavePasswd = FALSE;
	m_strUserName = _T("");
	m_strUserPasswd = _T("");
	m_bIsAutoSendTopic = TRUE;
	//}}AFX_DATA_INIT
	m_nDelay=5;
	m_bIsSelAll=FALSE;
}


void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CExpandingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLoginDlg)
	DDX_Control(pDX, IDC_LIST_TOPCOLUMN, m_listTopColumn);
	DDX_CBIndex(pDX, IDC_COMBO_PRIVS, m_nUserLevel);
	DDX_Check(pDX, IDC_IS_SAVE_PASS, m_bSavePasswd);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_USER_PASSWD, m_strUserPasswd);
	DDX_Check(pDX, IDC_IS_AUTO_SEND_TOPIC, m_bIsAutoSendTopic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CExpandingDialog)
	//{{AFX_MSG_MAP(CUserLoginDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	ON_BN_CLICKED(IDC_SELALL, OnSelall)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_SELALL,OnUpdateSelall)
	ON_BN_CLICKED(IDC_REPAIR, &CUserLoginDlg::OnBnClickedRepair)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg message handlers

void CUserLoginDlg::OnOK() 
{
	if(!theApp.DBIsOpen())
		return;
	// TODO: Add extra validation here
	UpdateData();

	CString strSql,strUserPasswd;
		
	strSql.Format("select * from wdm_user where user_name ='%s'",m_strUserName);
	
	CADORecordset rs(theApp.GetDB());
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		if(rs.IsEOF())
		{
			MessageBox(_T("用户不存在，请与管理员联系"),_T("注意"),MB_OK|MB_ICONWARNING);
			return ;
		}
		
		rs.GetFieldValue("user_passwd",strUserPasswd);
		CDes des;
		if(des.DesCrypteString("auqfvfhpgu",strUserPasswd,0).CompareNoCase(m_strUserPasswd)==0)
		{
			int nLevel;
			rs.GetFieldValue("user_level",nLevel);
			if(nLevel!=m_nUserLevel)
			{
				MessageBox(_T("用户身份不正确，请重新选择"),_T("注意"),MB_OK|MB_ICONWARNING);
				return;
			}
		}
		else
		{
			MessageBox(_T("密码不正确，请重新输入"),_T("注意"),MB_OK|MB_ICONWARNING);
			return;
		}
		rs.GetFieldValue("user_desc",m_strUserDesc);
		rs.Close();
	}
	theApp.GetShowTopColumnList().RemoveAll();
    for(int i=0;i<m_listTopColumn.GetItemCount();i++)
	{
		if(m_listTopColumn.GetCheck(i))
		{
			CColumn* pColumn=(CColumn*)m_listTopColumn.GetItemData(i);
			theApp.GetShowTopColumnList().Add(pColumn->GetID());
		}
	}
	CExpandingDialog::OnOK();
}

BOOL CUserLoginDlg::OnInitDialog() 
{
	CExpandingDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Expand(m_bIsExpanded);

	m_listTopColumn.SetHeadings(_T("采集源栏目,300")); 

	m_listTopColumn.SetReadOnlyColumns(0);
	m_listTopColumn.EnableVScroll(); 

	ShowContent();

	if(m_bAutoClose)
		SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserLoginDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nDelay==0)
	{
		OnOK();
	}
	
    m_nDelay=m_nDelay-1;

	CExpandingDialog::OnTimer(nIDEvent);
}

void CUserLoginDlg::OnDestroy() 
{
	CExpandingDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_bAutoClose)
		KillTimer(1);
	ClearMemory();
}

void CUserLoginDlg::OnAdvanced() 
{
	// TODO: Add your control notification handler code here
	OutputDebugString("CUserLoginDlg::Advanced button has been clicked.\n");
    m_bIsExpanded=!m_bIsExpanded;
}
BOOL CUserLoginDlg::OnDialogExpanding(BOOL /*bExpanded*/)
{
	UpdateData();
	OutputDebugString("CUserLoginDlg::OnDialogExpanding called.\n");
	return TRUE;
}

void CUserLoginDlg::OnDialogExpanded(BOOL bExpanded)
{
	OutputDebugString("CUserLoginDlg::OnDialogExpanded called.\n");

	UpdateData(FALSE);
}
void CUserLoginDlg::ShowContent()
{	
	if(theApp.DBIsOpen())
	{		
		CString strSql,str;
		CADORecordset rs(theApp.GetDB());
		
		//从数据库中读取采集源
		//strSql=_T("select * from wdm_column where parent_id = \'0\'");
		strSql.Format("select * from wdm_column where parent_id = '%s'",NULL_ID);
		
		if(rs.Open(strSql,CADORecordset::openQuery))
		{		
			CColumn* pColumn=NULL;
			int nItem=0;
			while(!rs.IsEOF())
			{
				pColumn=new CColumn;
				if(pColumn->LoadInfo(rs))
				{
					nItem=m_listTopColumn.AddItem(pColumn->GetDesc());
					m_listTopColumn.SetItemData(nItem,(DWORD)pColumn);	
					//Set init status
					for(int i=0;i<theApp.GetShowTopColumnList().GetSize();i++)
					{
						if(theApp.GetShowTopColumnList().GetAt(i).Compare(pColumn->GetID())==0)
							m_listTopColumn.SetCheck(nItem,TRUE);
					}
				}
				
				rs.MoveNext();
			}
			rs.Close();
		}
	}

}

void CUserLoginDlg::OnSelall() 
{
	// TODO: Add your control notification handler code here
	m_bIsSelAll=!m_bIsSelAll;

	for(int i=0;i<m_listTopColumn.GetItemCount();i++)
	{
		m_listTopColumn.SetCheck(i,m_bIsSelAll);
	}
	UpdateData(FALSE);
}
void CUserLoginDlg::OnUpdateSelall(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(IsSelAll()?_T("取消全选"):_T("全选"));
}
BOOL CUserLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CExpandingDialog::PreTranslateMessage(pMsg);
}
BOOL CUserLoginDlg::IsSelAll()
{
	UpdateData();

	BOOL bSelAll=TRUE;
	for(int i=0;i<m_listTopColumn.GetItemCount();i++)
        bSelAll&=m_listTopColumn.GetCheck(i);

    m_bIsSelAll=bSelAll;

	return m_bIsSelAll;
}
void CUserLoginDlg::OnConfig() 
{
	// TODO: Add your control notification handler code here
	//Reset the top column list
	UpdateData();

	theApp.GetShowTopColumnList().RemoveAll();
    for(int i=0;i<m_listTopColumn.GetItemCount();i++)
	{
		if(m_listTopColumn.GetCheck(i))
		{
			CColumn* pColumn=(CColumn*)m_listTopColumn.GetItemData(i);
			theApp.GetShowTopColumnList().Add(pColumn->GetID());
		}
	}
	theApp.OnToolOptions();
	//Fresh list
	ClearMemory();
	m_listTopColumn.DeleteAllItems();
	ShowContent();
}
void CUserLoginDlg::ClearMemory()
{
    for(int i=0;i<m_listTopColumn.GetItemCount();i++)
	{
		CColumn* pColumn=(CColumn*)m_listTopColumn.GetItemData(i);
		delete pColumn;
	}
}

void CUserLoginDlg::OnBnClickedRepair()
{
	// TODO: Add your control notification handler code here
	if( MessageBox(_T("修复采集源栏目，确定吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO )
		return;

	if(!theApp.DBIsOpen())
	{
		MessageBox(_T("数据库没有连接"),_T("注意"),MB_OK|MB_ICONWARNING);
		return;
	}
	//CString strSql = _T("update wdm_column set parent_id = 0 where parent_id='' or parent_id is null");
	CString strSql;
	strSql.Format("update wdm_column set parent_id = '%s' where parent_id='0' or parent_id='' or parent_id is null",NULL_ID);
			
	BOOL bRet = theApp.GetDB()->Execute(strSql);

	ClearMemory();
	m_listTopColumn.DeleteAllItems();
	ShowContent();	
}
