// LogsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "LogsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogsDlg dialog


CLogsDlg::CLogsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogsDlg)
	m_strPageNum = _T("");
	//}}AFX_DATA_INIT
}


void CLogsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogsDlg)
	DDX_Control(pDX, IDC_LIST_LOGS, m_listLogs);
	DDX_Text(pDX, IDC_PAGENUM, m_strPageNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogsDlg, CDialogEx)
	//{{AFX_MSG_MAP(CLogsDlg)
	ON_BN_CLICKED(IDC_CLEAR_LOGS, OnClearLogs)
	ON_BN_CLICKED(IDC_FIRST_PAGE, OnFirstPage)
	ON_BN_CLICKED(IDC_PREV_PAGE, OnPrevPage)
	ON_BN_CLICKED(IDC_NEXT_PAGE, OnNextPage)
	ON_BN_CLICKED(IDC_LAST_PAGE, OnLastPage)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_CLEAR_LOGS,OnUpdateClear)
	ON_UPDATE_COMMAND_UI(IDC_FIRST_PAGE,OnUpdateFirstPage)
	ON_UPDATE_COMMAND_UI(IDC_PREV_PAGE,OnUpdatePrevPage)
	ON_UPDATE_COMMAND_UI(IDC_NEXT_PAGE,OnUpdateNextPage)
	ON_UPDATE_COMMAND_UI(IDC_LAST_PAGE,OnUpdateLastPage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogsDlg message handlers


void CLogsDlg::OnClearLogs() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("清空所有日志吗？","请确认",MB_YESNO |MB_ICONQUESTION)==IDYES)
	{
		CWaitCursor wait;
		
		if(!theApp.DBIsOpen())
			theApp.ConnectDB();

        CString strSql=_T("delete from wdm_log");
		if(theApp.GetDB()->Execute(strSql))
		{
			CLogs::WriteLogs("清空所有日志",CLogs::log_system);
		    m_listLogs.DeleteAllItems();
		}
	}
}

BOOL CLogsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CLogsDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CLogs::WriteLogs("管理系统日志",CLogs::log_system);

	m_listLogs.ModifyStyle(0,LVS_REPORT);
	m_listLogs.SetExtendedStyle(m_listLogs.GetExtendedStyle()
		|LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_listLogs.SetHeadings(_T("序号,60;时间,150;来源,100;详细内容,300;计算机,150"));
    for(int i=0;i<m_listLogs.GetColumnCounts();i++)
		m_listLogs.SetReadOnlyColumns(i);

	CString strSql,strTemp;
	strSql=_T("select count(*) as total from wdm_log ");

	CADORecordset rs(theApp.GetDB());

	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		long  lTotal=0;
		rs.GetFieldValue(_T("total"),lTotal);

		m_nTotalCounts=lTotal;
		
		 if((m_nTotalCounts%NUM_PER_PAGE)==0)
			 m_nTotalPages=m_nTotalCounts/NUM_PER_PAGE;
		 else
			 m_nTotalPages=m_nTotalCounts/NUM_PER_PAGE+1;

		 m_nCurPage=1;	
		 
         rs.Close();

		 ShowContent();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CLogsDlg::OnUpdateClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_listLogs.GetItemCount()>0);
}
void CLogsDlg::OnFirstPage() 
{
	// TODO: Add your control notification handler code here
	m_nCurPage=1;
	ShowContent();
}

void CLogsDlg::OnPrevPage() 
{
	// TODO: Add your control notification handler code here
	m_nCurPage-=1;
// 	for(int i=0;i<NUM_PER_PAGE*2 && !m_rs.IsBOF();i++)
// 	    m_rs.MovePrevious();
// 
// 	if(m_rs.IsBOF())
// 		m_rs.MoveFirst();

	ShowContent();
}

void CLogsDlg::OnNextPage() 
{
	// TODO: Add your control notification handler code here
	m_nCurPage+=1;

	ShowContent();
}

void CLogsDlg::OnLastPage() 
{
	// TODO: Add your control notification handler code here
	m_nCurPage=m_nTotalPages;
// 	m_rs.MoveLast();
// 
// 	for(int i=0;i<m_nTotalCounts-(m_nTotalPages-1)*NUM_PER_PAGE-1;i++)
// 	    m_rs.MovePrevious();

	ShowContent();
}
void CLogsDlg::ShowContent()
{
	m_listLogs.DeleteAllItems();

	m_strPageNum.Format("第%d页，共%d页",m_nCurPage,m_nTotalPages);

	CString strTemp,strSql;

	if(m_nCurPage==1)
	{
		strSql.Format("select top %d * from wdm_log order by log_time desc,log_id",NUM_PER_PAGE);
	}
	else
	{
		strSql.Format("select top %d * from wdm_log where log_time < \
			 (select min (log_time) from (select top %d log_id,log_time from wdm_log order by log_time desc,log_id ) as T ) \
			 order by log_time desc,log_id",NUM_PER_PAGE,(m_nCurPage-1)*NUM_PER_PAGE);
	}

	CADORecordset rs(theApp.GetDB());
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
        int i=0;
		
		while (!rs.IsEOF())
		{
			strTemp.Format("%d",(m_nCurPage-1)*NUM_PER_PAGE+i+1);           
			m_listLogs.AddItem(strTemp,_T(""),_T(""),_T(""),_T(""));

			rs.GetFieldValue("log_time",strTemp);
			m_listLogs.SetItemText(i,1,strTemp);
			rs.GetFieldValue("log_source",strTemp);
			m_listLogs.SetItemText(i,2,strTemp);
			rs.GetFieldValue("log_message",strTemp);
			m_listLogs.SetItemText(i,3,strTemp);
			rs.GetFieldValue("log_owner",strTemp);
			m_listLogs.SetItemText(i,4,strTemp);			
	
			i++;
			rs.MoveNext();
		}
		rs.Close();
	}
	UpdateData(FALSE);
}

void CLogsDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();
	
	// TODO: Add your message handler code here
// 	m_rs.Close();
}
void CLogsDlg::OnUpdateFirstPage(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_nCurPage>1 || m_nCurPage==0 );
}
void CLogsDlg::OnUpdatePrevPage(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_nCurPage>1);
}
void CLogsDlg::OnUpdateNextPage(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_nCurPage>=1 && m_nCurPage<m_nTotalPages);
}
void CLogsDlg::OnUpdateLastPage(CCmdUI* pCmdUI)
{
     pCmdUI->Enable(m_nCurPage<m_nTotalPages);
}

// void CLogsDlg::OnAllPage() 
// {
// 	// TODO: Add your control notification handler code here
// 	CWaitCursor wait;
// 
// 	m_listLogs.DeleteAllItems();
// 
// 	m_strPageNum.Format("共%d条记录",m_nTotalCounts);
// 
//     int i=0;
// 	CString strTemp;
// 
// 	m_rs.MoveFirst();
// 
//     while(!m_rs.IsEOF())
// 	{			
// 		strTemp.Format("%d",i+1);           
// 		m_listLogs.AddItem(strTemp,_T(""),_T(""),_T(""),_T(""));
// 
// 		m_rs.GetFieldValue("log_time",strTemp);
// 		m_listLogs.SetItemText(i,1,strTemp);
// 		m_rs.GetFieldValue("log_source",strTemp);
// 		m_listLogs.SetItemText(i,2,strTemp);
// 		m_rs.GetFieldValue("log_message",strTemp);
// 		m_listLogs.SetItemText(i,3,strTemp);
// 		m_rs.GetFieldValue("log_owner",strTemp);
// 		m_listLogs.SetItemText(i++,4,strTemp);			
// 	
// 		m_rs.MoveNext();
// 	}
// 	UpdateData(FALSE);
// 	m_nCurPage=0;
// }
