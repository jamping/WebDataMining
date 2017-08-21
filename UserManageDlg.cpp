// UserManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "UserManageDlg.h"
#include "des.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserManageDlg dialog


CUserManageDlg::CUserManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserManageDlg)
	m_strUserDesc = _T("");
	m_strUserName = _T("");
	m_strUserPasswd = _T("");
	m_nUserLevel = -1;
	//}}AFX_DATA_INIT
	m_nCurSelectedItem=-1;
}


void CUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserManageDlg)
	DDX_Control(pDX, IDC_COMBO_PRIVS, m_comboUserLevel);
	DDX_Control(pDX, IDC_LIST_USERS, m_listUsers);
	DDX_Text(pDX, IDC_USER_DESC, m_strUserDesc);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_USER_PASSWD, m_strUserPasswd);
	DDX_CBIndex(pDX, IDC_COMBO_PRIVS, m_nUserLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserManageDlg, CDialogEx)
	//{{AFX_MSG_MAP(CUserManageDlg)
	ON_BN_CLICKED(IDC_ADD_USER, OnAddUser)
	ON_BN_CLICKED(IDC_CLEAR_USERS, OnClearUsers)
	ON_BN_CLICKED(IDC_DEL_USER, OnDelUser)
	ON_BN_CLICKED(IDC_MODIFY_USER, OnModifyUser)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USERS, OnItemchangedListUsers)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_ADD_USER,OnUpdateAddUser)
	ON_UPDATE_COMMAND_UI(IDC_CLEAR_USERS,OnUpdateClearUser)
	ON_UPDATE_COMMAND_UI(IDC_DEL_USER,OnUpdateDelUser)
	ON_UPDATE_COMMAND_UI(IDC_MODIFY_USER,OnUpdateModifyUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserManageDlg message handlers

void CUserManageDlg::OnAddUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(MessageBox("创建用户: "+m_strUserName+" 确定吗？","请确认",MB_YESNO |MB_ICONQUESTION)==IDYES)
	{
		if(m_listUserName.Find(m_strUserName)!=NULL)
		{
			MessageBox(m_strUserName+" 用户名已经存在,请重新输入!","请注意",MB_OK|MB_ICONWARNING);
            GetDlgItem(IDC_USER_NAME)->SetFocus();
            static_cast<CEdit*>(GetDlgItem(IDC_USER_NAME))->SetSel(0,-1);
			return;
		}
        
		CString str;
		COleDateTime time=COleDateTime::GetCurrentTime();
        //add to database
		CString strSql;
        CDes des;

		strSql.Format("insert into wdm_user (user_name,user_passwd,user_level,user_desc,create_time) \
			values ('%s','%s',%d,'%s','%s')",m_strUserName,des.DesCrypteString(_T("auqfvfhpgu"),m_strUserPasswd,1),
			m_nUserLevel,m_strUserDesc,time.Format("%Y-%m-%d %H:%M:%S"));

		if(!theApp.DBIsOpen())
			theApp.ConnectDB();

		if(theApp.GetDB()->Execute(strSql))
		{
			CLogs::WriteLogs("增加用户："+m_strUserName,CLogs::log_admin);

			m_listUserName.AddTail(m_strUserName);
			//add to list
			int nItem=m_listUsers.GetItemCount();
			
			m_listUsers.AddItem(m_strUserName,des.DesCrypteString(_T("auqfvfhpgu"),m_strUserPasswd,1),
				theApp.GetUserDescName((CWebDataMiningApp::UserLevelType)m_nUserLevel),
				m_strUserDesc,time.Format("%Y-%m-%d %H:%M:%S"));

			ClearCtrlText();
		}
	}
}

void CUserManageDlg::OnClearUsers() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("删除所有用户，确定吗？","请确认",MB_YESNO |MB_ICONQUESTION)==IDYES)
	{
		if(!theApp.DBIsOpen())
			theApp.ConnectDB();

        CString strSql=_T("delete from wdm_user");
		if(theApp.GetDB()->Execute(strSql))
		{
			CLogs::WriteLogs("删除所有用户",CLogs::log_admin);
			m_listUsers.DeleteAllItems();
			m_listUserName.RemoveAll();
		}
	}
}

void CUserManageDlg::OnDelUser() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("删除选择的用户，确定吗？","请确认",MB_YESNO |MB_ICONQUESTION)==IDNO)
		return;

	UINT i, uSelectedCount = m_listUsers.GetSelectedCount();
	int  nItem = -1;
	
	// Delete all of the selected items.
	if (uSelectedCount > 0)
	{		
		if(!theApp.DBIsOpen())
			theApp.ConnectDB();

        CString strSql,strUserName;
		CUIntArray listItem;

		for (i=0;i < uSelectedCount;i++)
		{
			nItem = m_listUsers.GetNextItem(nItem, LVNI_SELECTED);
			if(nItem != -1)
			{
				int j=0;
				for( j=0;j<listItem.GetSize();j++)
				{
					if(nItem>listItem.GetAt(j))
					{
						listItem.InsertAt(0,nItem);
						break;
					}

				}
				if(j==listItem.GetSize())
				    listItem.Add(nItem);

				strUserName=m_listUsers.GetItemText(nItem,0);
				strSql.Format("delete from wdm_user where user_name='%s'",strUserName);

				if(theApp.GetDB()->Execute(strSql))
				{
					CLogs::WriteLogs("删除用户:"+strUserName,CLogs::log_admin);

					POSITION pos=m_listUserName.Find(strUserName);
					ASSERT(pos);
					m_listUserName.RemoveAt(pos);
				}
			}
		}
		for(i=0;i<listItem.GetSize();i++)
			m_listUsers.DeleteItem(listItem.GetAt(i));
	}

}

void CUserManageDlg::OnModifyUser() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("修改用户，确定吗？","请确认",MB_YESNO |MB_ICONQUESTION)==IDNO)
		return;
    
	UpdateData();
	
	CString strSql,strOldUserName;
    strOldUserName=m_listUsers.GetItemText(m_nCurSelectedItem,0);
    //delete the old user name
	POSITION pos=m_listUserName.Find(strOldUserName);
	ASSERT(pos);
	m_listUserName.RemoveAt(pos);

	if(m_listUserName.Find(m_strUserName)!=NULL)
	{
		MessageBox(m_strUserName+" 用户名已经存在,请重新输入!","请注意",MB_OK|MB_ICONWARNING);
        GetDlgItem(IDC_USER_NAME)->SetFocus();
        static_cast<CEdit*>(GetDlgItem(IDC_USER_NAME))->SetSel(0,-1);
		return;
	}
	COleDateTime time=COleDateTime::GetCurrentTime();
	CDes des;
	strSql.Format("update wdm_user set user_name='%s',user_passwd='%s',user_level=%d,user_desc='%s', \
		create_time='%s' where user_name='%s'",
		m_strUserName,des.DesCrypteString(_T("auqfvfhpgu"),m_strUserPasswd,1),m_nUserLevel,
		m_strUserDesc,time.Format("%Y-%m-%d %H:%M:%S"),strOldUserName);
   
	if(!theApp.DBIsOpen())
		theApp.ConnectDB();

    if(theApp.GetDB()->Execute(strSql))
	{
		CString str;
		if(m_strUserName.CompareNoCase(strOldUserName)==0)
			str.Format("修改用户：%s",strOldUserName);
		else
			str.Format("修改用户：%s,用户名改为%s",strOldUserName,m_strUserName);
		CLogs::WriteLogs(str,CLogs::log_admin);

		m_listUserName.AddTail(m_strUserName);		

		m_listUsers.SetItemText(m_nCurSelectedItem,0,m_strUserName);
		m_listUsers.SetItemText(m_nCurSelectedItem,1,des.DesCrypteString(_T("auqfvfhpgu"),m_strUserPasswd,1));
		m_listUsers.SetItemText(m_nCurSelectedItem,2,theApp.GetUserDescName((CWebDataMiningApp::UserLevelType)m_nUserLevel));
		m_listUsers.SetItemText(m_nCurSelectedItem,3,m_strUserDesc);
		m_listUsers.SetItemText(m_nCurSelectedItem,4,time.Format("%Y-%m-%d %H:%M:%S"));

		m_listUsers.UpdateData(FALSE);
	}

}

BOOL CUserManageDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CUserManageDlg::OnUpdateAddUser(CCmdUI* pCmdUI)
{
	UpdateData();

    pCmdUI->Enable(!m_strUserName.IsEmpty() && !m_strUserPasswd.IsEmpty() && m_nUserLevel!=-1);
}

void CUserManageDlg::OnUpdateDelUser(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_listUsers.GetSelectedCount()>0);
}
void CUserManageDlg::OnUpdateClearUser(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_listUsers.GetItemCount()>0);
}
void CUserManageDlg::OnUpdateModifyUser(CCmdUI* pCmdUI)
{
	UpdateData();
    pCmdUI->Enable(m_listUsers.GetSelectedCount()==1 && !m_strUserName.IsEmpty() && 
		!m_strUserPasswd.IsEmpty() && m_nUserLevel!=-1 && IsModified());
}
BOOL CUserManageDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CLogs::WriteLogs("管理系统用户",CLogs::log_admin);

	m_listUsers.ModifyStyle(0,LVS_REPORT);
	m_listUsers.SetExtendedStyle(m_listUsers.GetExtendedStyle()
		|LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_listUsers.SetHeadings(_T("用户名,100;密码,150;身份,100;描述,300;创建时间,150"));
    for(int i=0;i<m_listUsers.GetColumnCounts();i++)
		m_listUsers.SetReadOnlyColumns(i);

	CString strSql,strTemp;
	strSql=_T("select * from wdm_user order by user_level desc");

	if(!theApp.DBIsOpen())
		theApp.ConnectDB();

	CADORecordset rs(theApp.GetDB());

	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		int nItem=0;
		while (!rs.IsEOF())
		{	
            rs.GetFieldValue("user_name",strTemp);
			m_listUserName.AddTail(strTemp);
			nItem=m_listUsers.AddItem(strTemp,_T(""),_T(""),_T(""),_T(""));
		
			rs.GetFieldValue("user_passwd",strTemp);
			m_listUsers.SetItemText(nItem,1,strTemp);
			int nLevel;
			rs.GetFieldValue("user_level",nLevel);
			m_listUsers.SetItemText(nItem,2,theApp.GetUserDescName((CWebDataMiningApp::UserLevelType)nLevel));
			rs.GetFieldValue("user_desc",strTemp);
			m_listUsers.SetItemText(nItem,3,strTemp);
			rs.GetFieldValue("create_time",strTemp);
			m_listUsers.SetItemText(nItem,4,strTemp);

			rs.MoveNext();
		}
		rs.Close();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserManageDlg::OnItemchangedListUsers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//更新对话框的数据显示
	if(pNMListView->uNewState==3)
	{
	    m_nCurSelectedItem=pNMListView->iItem;
		if(m_nCurSelectedItem!=LB_ERR)
		{
			m_strUserName=m_listUsers.GetItemText(m_nCurSelectedItem,0);
			CDes des;
			m_strUserPasswd=des.DesCrypteString(_T("auqfvfhpgu"),m_listUsers.GetItemText(m_nCurSelectedItem,1),0);			
			m_nUserLevel=m_comboUserLevel.SelectString(-1,m_listUsers.GetItemText(m_nCurSelectedItem,2));
            m_strUserDesc=m_listUsers.GetItemText(m_nCurSelectedItem,3);
			
			UpdateData(FALSE);
		}
	}
// 	else if(pNMListView->uNewState==0)
// 	{
// 		m_nCurSelectedItem=-1;
// 		ClearCtrlText();
// 	}
	*pResult = 0;
}

void CUserManageDlg::ClearCtrlText()
{
	GetDlgItem(IDC_USER_NAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_USER_PASSWD)->SetWindowText(_T(""));
	GetDlgItem(IDC_USER_DESC)->SetWindowText(_T(""));
	m_comboUserLevel.SetCurSel(-1);
}
BOOL CUserManageDlg::IsModified()
{
	BOOL bRet=TRUE;

    return bRet;
}