// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog


CSearchDlg::CSearchDlg(CStringArray& listTarget,CWnd* pParent /* = NULL */)
	: CDialogEx(CSearchDlg::IDD, pParent),m_listTarget(listTarget)
{
	//{{AFX_DATA_INIT(CSearchDlg)
	m_bAll = FALSE;
	m_bContent = FALSE;
	m_bTitle = TRUE;
	m_strTarget = _T("");
	//}}AFX_DATA_INIT
}


void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchDlg)
	DDX_Control(pDX, IDC_TARGET, m_cbTarget);
	DDX_Check(pDX, IDC_CHECK_ALL, m_bAll);
	DDX_Check(pDX, IDC_CHECK_CONTENT, m_bContent);
	DDX_Check(pDX, IDC_CHECK_TITLE, m_bTitle);
	DDX_Text(pDX, IDC_TARGET, m_strTarget);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	//{{AFX_MSG_MAP(CSearchDlg)
	ON_BN_CLICKED(IDC_SEARCH_NEXT, OnSearchNext)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_SEARCH_NEXT,OnUpdateSearchNext)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg message handlers

void CSearchDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialogEx::PostNcDestroy();
}

BOOL CSearchDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetTopics(&theApp.GetSourceTree()->m_objectHelper);
    int i =0;
	for( i=0;i<m_listTarget.GetSize();i++)
	{
		m_cbTarget.AddString(m_listTarget.GetAt(i));
	}
	m_cbTarget.SetCurSel(i-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchDlg::OnSearchNext() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//缓存查找关键字
	int i=0;
	for(;i<m_listTarget.GetSize();i++)
	{
		if(m_strTarget.Compare(m_listTarget.GetAt(i))==0)
			break;
	}
    if(i==m_listTarget.GetSize())
	{
		m_cbTarget.AddString(m_strTarget);
		m_listTarget.Add(m_strTarget);
	}

	BOOL bRet=FALSE;
	int nType=0;
	if(m_bTitle)
		nType =0x01;
	if(m_bContent)
		nType |=0x02;

    while (m_qTopics.size() && bRet==FALSE )
    {
		bRet=m_qTopics.front()->Match(m_strTarget,nType,m_bAll);
		m_qTopics.pop();
    }
	if(m_qTopics.empty())
	{
		if(MessageBox(_T("文章搜索完毕，是否再重新搜索一次？"),NULL,MB_YESNO)==IDYES)
		{
			GetTopics(&theApp.GetSourceTree()->m_objectHelper);
			OnSearchNext();
		}
		else
		{
			DestroyWindow();
		}
	}
}
void CSearchDlg::GetTopics(CBaseObject* pOb)
{
	if(pOb)
	{
		POSITION pos=pOb->GetFirstChildPosition();
		while (pos)
		{
			CBaseObject* pChild=pOb->GetNextChild(pos);
			if(pChild)
			{
				if(pChild->GetType()==OBJECT_TYPE_TOPIC)
				{
					m_qTopics.push((CTopic*)pChild);
				}
				else
				{
					GetTopics(pChild);	
				}
			}
		}
	}
}

BOOL CSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CSearchDlg::OnUpdateSearchNext(CCmdUI * pCmdUI)
{
	UpdateData();
    pCmdUI->Enable(!m_strTarget.IsEmpty() && (m_bTitle || m_bContent));
}
