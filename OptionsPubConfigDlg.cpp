// OptionsPubConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "OptionsPubConfigDlg.h"
#include "Topic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsPubConfigDlg property page

IMPLEMENT_DYNCREATE(COptionsPubConfigDlg, CMFCPropertyPage)

COptionsPubConfigDlg::COptionsPubConfigDlg() : CMFCPropertyPage(COptionsPubConfigDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsPubConfigDlg)
	m_strSql = _T("");
	//}}AFX_DATA_INIT
	m_dwTopicUnits=0;
}

COptionsPubConfigDlg::~COptionsPubConfigDlg()
{
}

void COptionsPubConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPubConfigDlg)
	DDX_Control(pDX, IDC_LIST_TOPIC_UNIT, m_listTopicUnit);
	DDX_Control(pDX, IDC_LIST_TABLE_UNIT, m_listTableUnit);
	DDX_Control(pDX, IDC_COMBO_TABLES, m_ctrTables);
	DDX_Text(pDX, IDC_SQL, m_strSql);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPubConfigDlg, CMFCPropertyPage)
	//{{AFX_MSG_MAP(COptionsPubConfigDlg)
	ON_BN_CLICKED(IDC_BUTTON_ASSOC, OnButtonAssoc)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLES, OnSelchangeComboTables)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsPubConfigDlg message handlers

BOOL COptionsPubConfigDlg::OnInitDialog() 
{
	CMFCPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	struct TOPIC_UNIT_TAG
	{
		LPCTSTR   strTopicUnitName;
		int       nUnitValue;
	}
	TopicUnits [] =
	{
		{ "引标",CTopic::T_HEADTITLE },
		{ "标题",CTopic::T_TITLE },
		{ "副标",CTopic::T_SUBTITLE },
		{ "时间",CTopic::T_TIME },
		{ "来源",CTopic::T_SOURCE },
		{ "作者",CTopic::T_AUTHOR },
		{ "大类",CTopic::T_BIGCLASS },
		{ "小类",CTopic::T_SMALLCLASS },
		{ "关键字",CTopic::T_KEYWORD },
		{ "专题",CTopic::T_SUBJECT },
		{ "内容",CTopic::T_CONTENT }
	};

    for(int i=0;i<sizeof(TopicUnits)/sizeof(TOPIC_UNIT_TAG);i++)
	{
		m_listTopicUnit.SetItemData(m_listTopicUnit.AddString(TopicUnits[i].strTopicUnitName),
			TopicUnits[i].nUnitValue);
//		m_mapTopicUnits.SetAt(TopicUnits[i].strTopicUnitName,TopicUnits[i].dwUnitValue);
	}
    CString strSql=_T("select name from sysobjects \
		where type = 'U' and name <> 'dtproperties' order by name");

	CCommonFunction::InsertDataToComboBox(m_ctrTables,strSql,_T("name"),1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsPubConfigDlg::OnButtonAssoc() 
{
	// TODO: Add your control notification handler code here 
	int nCount = m_listTopicUnit.GetSelCount();
	if(nCount<=0)
	{
		MessageBox("没有选择文章要素!","错误",MB_OK|MB_ICONWARNING);
		return;
	}

	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_listTopicUnit.GetSelItems(nCount, aryListBoxSel.GetData()); 
   
	CString       strTableUnit;
	int nTableIndex=m_listTableUnit.GetCurSel();
	if(nTableIndex==LB_ERR)
	{
		MessageBox("没有选择数据表要素!","错误",MB_OK|MB_ICONWARNING);
		return;
	}
    m_listTableUnit.GetText(nTableIndex,strTableUnit);

	if(m_strSql.IsEmpty())
	{

	}
	else
	{
		
	}
}

void COptionsPubConfigDlg::OnSelchangeComboTables() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_ctrTables.GetCurSel();
	if(nIndex==CB_ERR)
		return;

	m_listTableUnit.ResetContent();
    
	CString strSql,str;
	m_ctrTables.GetLBText(nIndex,str);
	strSql.Format("Select a.status as Status, a.cdefault as cdefault, a.id as id,  \
		a.colorder as colorder, c.name as TableName, a.name as ColumnName,  \
		b.name as TypeName, a.length as Length, a.scale as Scale, a.isnullable as IsNullAble  \
		from syscolumns a, systypes b, sysObjects c  \
		where a.xtype = b.xusertype and a.id = c.id and c.xtype='U' and c.name = '%s' order by a.colorder",str);
    
	theApp.CloseDB(CWebDataMiningApp::remoteServer);
	if(theApp.ConnectDB(CWebDataMiningApp::remoteServer))
	{    
		CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));
		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			while(!rs.IsEOF())
			{
				rs.GetFieldValue("ColumnName",str);
				m_listTableUnit.AddString(str);
				
				rs.MoveNext();			
			}
			rs.Close();
		}
	}
}
