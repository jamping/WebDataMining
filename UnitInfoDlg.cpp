// UnitInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "UnitInfoDlg.h"
#include "WDMTable.h"
#include "WDMDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CString CUnitInfoDlg::unitCombos[]={ 
	_T("无"),                    
	_T("首字母"),                
	_T("计算MD5值"),             
	_T("截取前N个字符"),       
	_T("转换成Unix时间戳"),      
	_T("新建GUID"),              
	_T("引用缓存值"),            
	_T("转换成URL参数"),         
	_T("获取本机IP"),            
	_T("获取文件MIME类型"),      
	_T("去除文件名"),
	_T("引用自增ID"),
	_T("数值递增"),
	_T("时间参数转换"),
	_T("空值清空")
};

//////////////////////////////////////////////////////////////////////////
CString CUnitInfoDlg::topicUnits[]={ 
	_T("缺省值"),
	_T("自增ID"),
	_T("文章ID"),
	_T("引标"),
	_T("标题"),
	_T("原标题"),
	_T("原链接"),
	_T("缩标"),
	_T("副标"),
	_T("时间"),
	_T("来源"),
	_T("作者"),
	_T("关键字"),
	_T("专题词"),
	_T("内容"),
	_T("大类"),
	_T("小类"),
	_T("是否重要"),
	_T("缩略图路径"),
	_T("服务器栏目ID"),
	_T("服务器频道ID"),
	_T("图片ID"),
	_T("图片URL"),
	_T("图片说明"),
	_T("图片名称"),
	_T("图片扩展名"),
	_T("图片数量"),
	_T("图片大小"),	
	_T("是否是缩略图"),
	_T("时间戳"),
	_T("转向链接"),
	_T("编辑"),
	_T("文章权重"),
	_T("引用缓存值"),
	_T("读取库中数据")
};
//////////////////////////////////////////////////////////////////////////
// CUnitInfoDlg dialog
CUnitInfoDlg::CUnitInfoDlg(CBaseObject* pOb,CWnd* pParent /* = NULL */)
	: CDialogEx(CUnitInfoDlg::IDD, pParent),m_pOb(pOb)
{
	//{{AFX_DATA_INIT(CUnitInfoDlg)
	m_bKey = FALSE;	
	m_bCache = FALSE;
	m_strDefaultValue = _T("");
	m_strUnitName = _T("");
	m_strUnitDefault = _T("");
	m_strUnitKey = _T("");
	m_strUnitIsNull = _T("");
	m_nLen = 0;	
	m_nProcessOrder = 0;
	m_strUnitType = _T("");
	//}}AFX_DATA_INIT
 	m_strUnitCombos = _T("");
	m_nCurSelectedItem = -1;
}


void CUnitInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnitInfoDlg)
	DDX_Control(pDX, IDC_UNIT_NAME, m_ctlUnitName);	
	DDX_Check(pDX, IDC_CHECK_KEY, m_bKey);
	DDX_CBString(pDX, IDC_UNIT_NAME, m_strUnitName);
	DDX_Text(pDX, IDC_UNIT_LEN, m_nLen);
	DDX_Control(pDX, IDC_LIST_UNIT_COMBO, m_listUnitCombo);
	DDX_Text(pDX, IDC_EDIT_PROCESS_ORDER, m_nProcessOrder);
	DDV_MinMaxInt(pDX, m_nProcessOrder,0,100);
	DDX_Text(pDX, IDC_UNIT_TYPE, m_strUnitType);
	DDX_Text(pDX, IDC_UNIT_DEFAULT, m_strUnitDefault);	
	DDX_Text(pDX, IDC_UNIT_KEY, m_strUnitKey);	
	DDX_Text(pDX, IDC_UNIT_NULL, m_strUnitIsNull);	
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DEFAULT, m_strDefaultValue);
	DDX_Check(pDX, IDC_CHECK_CACHE_VALUE, m_bCache);
	DDX_Control(pDX, IDC_COMBO_UNIT_NAME, m_comboUnitName);
}

void CUnitInfoDlg::SetValue(CWDMOperatorUnit::WDMOperatorUnitData& data)
{
	m_strUnitName=data.m_strUnitName;
	m_bKey=data.m_bIsKey;
	m_bCache=data.m_bCache;
	m_nLen=data.m_nUnitLength;
	m_strUnitCombos=data.m_strUnitCombos;
	m_nProcessOrder=data.m_nProcessOrder;
	m_strUnitType=data.m_strUnitType;
	m_strUnitDefault=data.m_strUnitDefault;
	m_strUnitKey=data.m_strUnitKey;
	m_strUnitIsNull=data.m_strUnitIsNull;
}

BOOL CUnitInfoDlg::GetValue(CWDMOperatorUnit::WDMOperatorUnitData& data)
{
	BOOL bModify=FALSE;
	if( !CCommonFunction::IsSameString(data.m_strUnitName,m_strUnitName))
	{
		data.m_strUnitName=m_strUnitName;
		bModify |= TRUE;
	}
	if(data.m_nUnitLength!=m_nLen)
	{
		data.m_nUnitLength=m_nLen;
		bModify |= TRUE;
	}
	if(data.m_nProcessOrder!=m_nProcessOrder)
	{
		data.m_nProcessOrder=m_nProcessOrder;
		bModify |= TRUE;
	}
	if(data.m_bIsKey!=m_bKey)
	{
		data.m_bIsKey=m_bKey;
		bModify |= TRUE;
	}
	if(data.m_bCache!=m_bCache)
	{
		data.m_bCache=m_bCache;
		bModify |= TRUE;
	}
	
	if( !CCommonFunction::IsSameString(data.m_strUnitCombos,m_strUnitCombos))
	{
		data.m_strUnitCombos=m_strUnitCombos;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString(data.m_strUnitType,m_strUnitType))
	{
		data.m_strUnitType=m_strUnitType;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString(data.m_strUnitDefault,m_strUnitDefault))
	{
		data.m_strUnitDefault=m_strUnitDefault;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString(data.m_strUnitKey,m_strUnitKey))
	{
		data.m_strUnitKey=m_strUnitKey;
		bModify |= TRUE;
	}
	if( !CCommonFunction::IsSameString(data.m_strUnitIsNull,m_strUnitIsNull))
	{
		data.m_strUnitIsNull=m_strUnitIsNull;
		bModify |= TRUE;
	}
	
	return bModify;
}
BEGIN_MESSAGE_MAP(CUnitInfoDlg, CDialogEx)
	//{{AFX_MSG_MAP(CUnitInfoDlg)
	ON_CBN_DROPDOWN(IDC_UNIT_NAME, OnDropdownUnitName)
	ON_CBN_SELCHANGE(IDC_UNIT_NAME, OnSelchangeUnitName)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
	ON_MESSAGE(WM_SET_ITEMS,OnLoadComboboxData)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEdit)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_DEFAULT,OnUpdateDefaultValue)
//	ON_CONTROL_RANGE(BN_CLICKED,IDC_CHECK_HEADTITLE,IDC_CHECK_OLDURL, OnBnClickedCheck)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_UP,OnUpdateButtonUp)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DOWN,OnUpdateButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DEL,OnUpdateButtonDel)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ADD,OnUpdateButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UNIT_COMBO, OnItemchangedListUnitCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnitInfoDlg message handlers

BOOL CUnitInfoDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	static_cast<CEdit*>(GetDlgItem(IDC_UNIT_LEN))->SetReadOnly(TRUE);
	static_cast<CEdit*>(GetDlgItem(IDC_UNIT_TYPE))->SetReadOnly(TRUE);
	static_cast<CEdit*>(GetDlgItem(IDC_UNIT_DEFAULT))->SetReadOnly(TRUE);
	static_cast<CEdit*>(GetDlgItem(IDC_UNIT_KEY))->SetReadOnly(TRUE);
	static_cast<CEdit*>(GetDlgItem(IDC_UNIT_NULL))->SetReadOnly(TRUE);

	CSpinButtonCtrl* pSpin = ((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1));
	pSpin->SetRange(0, 100);
	//加载要素列表
	for( int i=0;i<sizeof(topicUnits)/sizeof(CString);i++)
	{
		m_comboUnitName.AddString(topicUnits[i]);
	}
	//初始化要素组合列表
	m_listUnitCombo.ModifyStyle(0,LVS_REPORT | LVS_EDITLABELS);
	m_listUnitCombo.SetExtendedStyle(m_listUnitCombo.GetExtendedStyle() | 
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listUnitCombo.SetHeadings(_T("序号,50;要素名,100;缺省值,150;要素再处理,150;"));
	m_listUnitCombo.SetReadOnlyColumns(0);
	m_listUnitCombo.SetReadOnlyColumns(1);
	m_listUnitCombo.SetReadOnlyColumns(2);
	m_listUnitCombo.SetComboColumns(3);
	m_listUnitCombo.EnableSort(FALSE);	

	//Initial the list
	int i = 0;
	CString strUintCombo;
	while( AfxExtractSubString(strUintCombo,m_strUnitCombos,i,_T(';')))
	{
		// Prepare to move to the next substring
		i++;
		if( strUintCombo.IsEmpty())
			continue;
		// The data format is
		// unit vale,default value,combo value;unit vale,default value,combo value;...
		// individual name and value elements
		CString str,strUnit,strDefault,strCombo; 
		int nUnit,iCombo;
		// Attempt to extract the name and value element from the pair
		if (AfxExtractSubString(strUnit, strUintCombo, 0, _T(',')) && 
			AfxExtractSubString(strDefault, strUintCombo, 1, _T(','))&& 
			AfxExtractSubString(strCombo, strUintCombo, 2, _T(',')))
		{
			 nUnit = atoi(strUnit);  //_tcstoui64((LPCTSTR)strUnit,0,10);
			 iCombo = atoi(strCombo);

			 UNIT_INFO* pui = new UNIT_INFO;
			 pui->nUnitValue = nUnit;
			 pui->strDefault = strDefault;
			 pui->nProcess = iCombo;

			 str.Format("%d",i);
			 m_listUnitCombo.SetItemData(m_listUnitCombo.AddItem(str,topicUnits[nUnit],strDefault,unitCombos[iCombo]),(DWORD)pui);
		}

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUnitInfoDlg::OnDropdownUnitName() 
{
	// TODO: Add your control notification handler code here
	ClearUnitContent();
    UpdateData();

	CWDMTable* pTable=(CWDMTable*)m_pOb->BackTo(OBJECT_TYPE_TABLE);
    CWDMDatabase* pDatabase=(CWDMDatabase*)m_pOb->BackTo(OBJECT_TYPE_DATABASE);
	COptionsData::OptionsData::DBServerData data;
	CString strSql,strFieldName;

	pDatabase->GetDBServerData(data);	

	switch ( data.m_nType )
	{
	case 0://SQL server
		//strSql.Format("Select a.status as Status, a.cdefault as cdefault, a.id as id,  \
		//			  a.colorder as colorder, c.name as TableName, a.name as ColumnName,  \
		//			  b.name as DataType, a.length as Length, a.scale as Scale, a.isnullable as IsNullable  \
		//			  from syscolumns a, systypes b, sysObjects c  \
		//			  where a.xtype = b.xusertype and a.id = c.id and c.xtype='U' and c.name = '%s' order by a.colorder",pTable->GetDesc());
		strSql.Format("select col.name as ColumnName, col.max_length as DataLength, col.is_nullable as IsNullable, t.name as DataType,  \
			( select top 1 ind.name from sys.index_columns ic  \
			left join sys.indexes ind on ic.object_id=ind.object_id	and ic.index_id=ind.index_id  \
		    where ic.object_id=obj.object_id and ic.column_id=col.column_id ) as PrimaryKey,  \
			cm.text as DataDefault 	from sys.objects obj  \
			inner join sys.columns col	on obj.object_id=col.object_id  \
			left join sys.types t on t.user_type_id=col.user_type_id  \
			left join sys.syscomments cm on cm.id=col.default_object_id where obj.name='%s'",pTable->GetDesc());
		break;
	case 1://Oracle
		strSql.Format("select col.COLUMN_NAME as ColumnName,col.DATA_TYPE as DataType,col.DATA_LENGTH as DataLength, \
		     col.NULLABLE as IsNullable,col.DATA_DEFAULT as DataDefault, uc.constraint_type as PrimaryKey \
			 from user_tab_columns col left join user_cons_columns ucc on ucc.table_name=col.table_name and ucc.column_name=col.column_name \
			 left join user_constraints uc on uc.constraint_name = ucc.constraint_name and uc.constraint_type='P' \
		     where col.table_name='%s' order by ColumnName",pTable->GetDesc());
		break;
	case 2://Mysql
		{
			data.m_strDatabase = _T("information_schema");
			strSql.Format("select COLUMN_NAME as ColumnName,COLUMN_TYPE as DataType,CHARACTER_MAXIMUM_LENGTH as DataLength, \
						  COLUMN_DEFAULT as DataDefault,COLUMN_KEY as PrimaryKey,IS_NULLABLE as IsNullable \
						  FROM information_schema.columns where TABLE_SCHEMA='%s' and TABLE_NAME='%s' order by ColumnName",
						  pDatabase->GetDesc(),pTable->GetDesc());
		}
		break;
	}	
	if(theApp.ConnectDB(data,CWebDataMiningApp::remoteServer))
	{    
		CADORecordset rs(theApp.GetDB(CWebDataMiningApp::remoteServer));
		if(rs.Open(strSql,CADORecordset::openQuery))
		{
			while(!rs.IsEOF())
			{
				FIELD_INFO* pfi = new FIELD_INFO;

				rs.GetFieldValue("ColumnName",strFieldName);
				rs.GetFieldValue("DataLength",pfi->nLength);
				rs.GetFieldValue("DataType",pfi->strTypeName);
				rs.GetFieldValue("DataDefault",pfi->strDefault);
				rs.GetFieldValue("PrimaryKey",pfi->strPrimaryKey);
				rs.GetFieldValue("IsNullable",pfi->strIsNull);

				m_ctlUnitName.SetItemData(m_ctlUnitName.AddString(strFieldName),(DWORD)pfi);
				
				rs.MoveNext();			
			}
			rs.Close();
		}	
		theApp.CloseDB(CWebDataMiningApp::remoteServer);
	}
}

void CUnitInfoDlg::OnSelchangeUnitName() 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_ctlUnitName.GetCurSel();
	if(nIndex==CB_ERR)
		return;

	FIELD_INFO* pfi = (FIELD_INFO*)m_ctlUnitName.GetItemData(nIndex);
	m_nLen = pfi->nLength;
	m_strUnitType = pfi->strTypeName;
	m_strUnitKey = pfi->strPrimaryKey;
	m_strUnitDefault = pfi->strDefault;
	m_strUnitIsNull = pfi->strIsNull;

// 	if( pfi->strTypeName.CompareNoCase(_T("datetime")) == 0 )
// 		m_strUnitDefault = _T("time()");
// 	else
// 		m_strUnitDefault = _T("");

	UpdateData(FALSE);
}

void CUnitInfoDlg::ClearUnitContent()
{
	if( m_ctlUnitName.GetSafeHwnd() != NULL)
	{
		for(int i=0;i<m_ctlUnitName.GetCount();i++)
		{
			FIELD_INFO* pfi = (FIELD_INFO*)m_ctlUnitName.GetItemData(i);
			delete pfi;
			pfi = NULL;
		}
		m_ctlUnitName.ResetContent();
	}	
}
void CUnitInfoDlg::ClearUnitCombos()
{	
	if( m_listUnitCombo.GetSafeHwnd() != NULL)
	{
		for(int i=0;i<m_listUnitCombo.GetItemCount();i++)
		{
			UNIT_INFO* pui = (UNIT_INFO*)m_listUnitCombo.GetItemData(i);
			delete pui;
			pui = NULL;
		}
		m_listUnitCombo.DeleteAllItems();
	}
}
void CUnitInfoDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearUnitContent();
	ClearUnitCombos();
}


void CUnitInfoDlg::OnBnClickedButtonUp()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listUnitCombo.GetFirstSelectedItemPosition();
	int i = m_listUnitCombo.GetNextSelectedItem(pos);

	UNIT_INFO* pui1 = (UNIT_INFO*)m_listUnitCombo.GetItemData(i-1);
	UNIT_INFO* pui2 = (UNIT_INFO*)m_listUnitCombo.GetItemData(i);

	// 交换两行数据，第一列序号不变 2=>1
	m_listUnitCombo.SetItemText(i-1,1,topicUnits[pui2->nUnitValue]);
	m_listUnitCombo.SetItemText(i-1,2,pui2->strDefault);
	m_listUnitCombo.SetItemText(i-1,3,unitCombos[pui2->nProcess]);
	m_listUnitCombo.SetItemData(i-1,(DWORD)pui2);
	
	m_listUnitCombo.SetItemText(i,1,topicUnits[pui1->nUnitValue]);
	m_listUnitCombo.SetItemText(i,2,pui1->strDefault);
	m_listUnitCombo.SetItemText(i,3,unitCombos[pui1->nProcess]);
	m_listUnitCombo.SetItemData(i,(DWORD)pui1);

	m_listUnitCombo.SetFocus();
	m_listUnitCombo.SetItemState(i-1,LVIS_SELECTED, LVIS_SELECTED);
	m_listUnitCombo.SetItemState(i,0, LVIS_SELECTED);

}


void CUnitInfoDlg::OnBnClickedButtonDown()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listUnitCombo.GetFirstSelectedItemPosition();
	int i = m_listUnitCombo.GetNextSelectedItem(pos);

	UNIT_INFO* pui1 = (UNIT_INFO*)m_listUnitCombo.GetItemData(i);
	UNIT_INFO* pui2 = (UNIT_INFO*)m_listUnitCombo.GetItemData(i+1);

	// 交换两行数据，第一列序号不变 1=>2
	m_listUnitCombo.SetItemText(i,1,topicUnits[pui2->nUnitValue]);
	m_listUnitCombo.SetItemText(i,2,pui2->strDefault);
	m_listUnitCombo.SetItemText(i,3,unitCombos[pui2->nProcess]);
	m_listUnitCombo.SetItemData(i,(DWORD)pui2);

	m_listUnitCombo.SetItemText(i+1,1,topicUnits[pui1->nUnitValue]);
	m_listUnitCombo.SetItemText(i+1,2,pui1->strDefault);
	m_listUnitCombo.SetItemText(i+1,3,unitCombos[pui1->nProcess]);
	m_listUnitCombo.SetItemData(i+1,(DWORD)pui1);

	m_listUnitCombo.SetFocus();
	m_listUnitCombo.SetItemState(i, 0, LVIS_SELECTED);
	m_listUnitCombo.SetItemState(i+1, LVIS_SELECTED, LVIS_SELECTED);

}
BOOL CUnitInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CUnitInfoDlg::OnLoadComboboxData(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	int nRowIndex,nColumnIndex;
	nRowIndex=LOWORD((DWORD)wParam);
	nColumnIndex=HIWORD((DWORD)wParam);
	CStringList* strList=(CStringList*)lParam;	
	
	if( nColumnIndex == 3 )
	{
		//Set the  data to combobox
		for( int i=0;i<sizeof(unitCombos)/sizeof(CString);i++)
			strList->AddTail(unitCombos[i]);
	}
	return 0;
}
LRESULT CUnitInfoDlg::OnEndLabelEdit(WPARAM wParam, LPARAM lParam) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	// TODO: Add your control notification handler code here
	UNIT_INFO* pui=(UNIT_INFO*)m_listUnitCombo.GetItemData(pDispInfo->item.iItem);

	switch( pDispInfo->item.iSubItem )
	{
	case 0:
		break;
	case 3:   //要素后处理信息更新
		for(int i=0;i<sizeof(unitCombos)/sizeof(CString);i++)
		{
			if( unitCombos[i].CompareNoCase(pDispInfo->item.pszText) == 0 )
			{
				pui->nProcess = i;
				break;
			}
		}
		break;
	}

	return 1;
}
void CUnitInfoDlg::OnUpdateDefaultValue(CCmdUI * pCmdUI)
{
//	pCmdUI->Enable( m_listUnitCombo.GetItemCount() > 1 );
}
void CUnitInfoDlg::OnUpdateButtonUp(CCmdUI * pCmdUI)
{
	POSITION pos = m_listUnitCombo.GetFirstSelectedItemPosition();

	pCmdUI->Enable( m_listUnitCombo.GetItemCount() > 1 
		         && m_listUnitCombo.GetSelectedCount() ==1 
				 && m_listUnitCombo.GetNextSelectedItem(pos)>0);
}
void CUnitInfoDlg::OnUpdateButtonDown(CCmdUI * pCmdUI)
{
	POSITION pos = m_listUnitCombo.GetFirstSelectedItemPosition();

	pCmdUI->Enable( m_listUnitCombo.GetItemCount() > 1 
		&& m_listUnitCombo.GetSelectedCount() ==1 
		&& m_listUnitCombo.GetNextSelectedItem(pos) < m_listUnitCombo.GetItemCount()-1);
}
//void CUnitInfoDlg::OnBnClickedCheck(UINT uID)
//{
//	// TODO: Add your control notification handler code here
//	UpdateData();
// 
// #define IDC_CHECK_HEADTITLE             1400
// #define IDC_CHECK_TITLE                 1401
// #define IDC_CHECK_MAITITLE              1402
// #define IDC_CHECK_SUBTITLE              1403
// #define IDC_CHECK_TIME                  1404
// #define IDC_CHECK_AUTHOR                1405
// #define IDC_CHECK_SOURCE                1406
// #define IDC_CHECK_CONTENT               1407
// #define IDC_CHECK_KEYWORD               1408
// #define IDC_CHECK_SUBJECT               1409
// #define IDC_CHECK_BIGCLASS              1410
// #define IDC_CHECK_SMALLCLASS            1411
// #define IDC_CHECK_ISPRIMARY             1412
// #define IDC_CHECK_ISELITE               1413
// #define IDC_CHECK_ISONTOP               1414
// #define IDC_CHECK_TOPICID               1415
// #define IDC_CHECK_COLUMNID              1416
// #define IDC_CHECK_RESOURCENUM           1417
// #define IDC_CHECK_RESOURCEID            1418
// #define IDC_CHECK_RESOURCENAME          1419
// #define IDC_CHECK_RESOURCEURL           1420
// #define IDC_CHECK_RESOURCE_EXPLAIN      1421
// #define IDC_CHECK_RESOURCEEXT           1422
// #define IDC_CHECK_TIMESTAMP             1423
// #define IDC_CHECK_LINKURL               1424
// #define IDC_CHECK_CHANNELID             1425
// #define IDC_CHECK_EDITOR                1426
// #define IDC_CHECK_PRIMARY_NUMBER        1427
// #define IDC_CHECK_DEFAULT               1428
// #define IDC_CHECK_SERVER_AUTOID         1429
// #define IDC_CHECK_RESOURCE_SIZE         1430
// #define IDC_CHECK_ISTHUMB               1431
// #define IDC_CHECK_THUMB                 1432
// #define IDC_CHECK_OLDTITLE              1431
// #define IDC_CHECK_OLDURL                1432

//	CString strName = _T("");
//	DWORD64 dwUnitValue = 0ULL;
//	BOOL bSave = TRUE;
//
//	switch ( uID )
//	{
//	case IDC_CHECK_HEADTITLE:
//		strName = _T("引标");
//		dwUnitValue = CTopic::T_HEADTITLE;
//		bSave = m_bHeadTitle;
//		break;
//	case IDC_CHECK_TITLE:
//		strName = _T("标题");
//		dwUnitValue = CTopic::T_TITLE;
//		bSave = m_bTitle;
//		break;
//	case IDC_CHECK_MAITITLE:
//		strName = _T("缩标");
//		dwUnitValue = CTopic::T_MAINTITLE;
//		bSave = m_bMainTitle;
//		break;
//	case IDC_CHECK_SUBTITLE:
//		strName = _T("副标");
//		dwUnitValue = CTopic::T_SUBJECT;
//		bSave = m_bSubTitle;
//		break;
//	case IDC_CHECK_TIME:
//		strName = _T("时间");
//		dwUnitValue = CTopic::T_TIME;
//		bSave = m_bTime;
//		break;
//	case IDC_CHECK_AUTHOR:
//		strName = _T("作者");
//		dwUnitValue = CTopic::T_AUTHOR;
//		bSave = m_bAuthor;
//		break;
//	case IDC_CHECK_SOURCE:
//		strName = _T("来源");
//		dwUnitValue = CTopic::T_SOURCE;
//		bSave = m_bSource;
//		break;
//	case IDC_CHECK_CONTENT:
//		strName = _T("内容");
//		dwUnitValue = CTopic::T_CONTENT;
//		bSave = m_bContent;
//		break;
//	case IDC_CHECK_KEYWORD:
//		strName = _T("关键字");
//		dwUnitValue = CTopic::T_KEYWORD;
//		bSave = m_bKeyWord;
//		break;
//	case IDC_CHECK_SUBJECT:
//		strName = _T("专题");
//		dwUnitValue = CTopic::T_SUBJECT;
//		bSave = m_bSubject;
//		break;
//	case IDC_CHECK_BIGCLASS:
//		strName = _T("大类");
//		dwUnitValue = CTopic::T_BIGCLASS;
//		bSave = m_bBigClass;
//		break;
//	case IDC_CHECK_SMALLCLASS:
//		strName = _T("小类");
//		dwUnitValue = CTopic::T_SMALLCLASS;
//		bSave = m_bSmallClass;
//		break;
//	case IDC_CHECK_ISPRIMARY:
//		strName = _T("重要");
//		dwUnitValue = CTopic::T_PRIMARY;
//		bSave = m_bPrimary;
//		break;
//	//case IDC_CHECK_ISELITE:
//	//	strName = _T("推荐");
//	//	dwUnitValue = CTopic::Elite;
//	//	bSave = m_bElite;
//	//	break;
//	//case IDC_CHECK_ISONTOP:
//	//	strName = _T("置顶");
//	//	dwUnitValue = CTopic::OnTop;
//	//	bSave = m_bOnTop;
//	//	break;
//	case IDC_CHECK_TOPICID:
//		strName = _T("文章ID");
//		dwUnitValue = CTopic::T_TOPICID;
//		bSave = m_bTopicID;
//		break;
//	case IDC_CHECK_COLUMNID:
//		strName = _T("栏目ID");
//		dwUnitValue = CTopic::T_SERVERCOLUMNID;
//		bSave = m_bColumnID;
//		break;
//	case IDC_CHECK_RESOURCENUM:
//		strName = _T("资源数");
//		dwUnitValue = CTopic::T_RESOURCENUM;
//		bSave = m_bResourceNum;
//		break;
//	case IDC_CHECK_RESOURCEID:
//		strName = _T("资源ID");
//		dwUnitValue = CTopic::T_RESOURCEID;
//		bSave = m_bResourceID;
//		break;
//	case IDC_CHECK_RESOURCENAME:
//		strName = _T("资源名");
//		dwUnitValue = CTopic::T_RESOURCENAME;
//		bSave = m_bResourceName;
//		break;
//	case IDC_CHECK_RESOURCEURL:
//		strName = _T("URL");
//		dwUnitValue = CTopic::T_RESOURCEURL;
//		bSave = m_bResourceUrl;
//		break;
//	case IDC_CHECK_RESOURCE_EXPLAIN:
//		strName = _T("说明");
//		dwUnitValue = CTopic::T_RESOURCEEXPLAIN;
//		bSave = m_bResourceExplain;
//		break;
//	case IDC_CHECK_RESOURCEEXT:
//		strName = _T("扩展名");
//		dwUnitValue = CTopic::T_RESOURCEEXT;
//		bSave = m_bResourceExt;
//		break;
//	case IDC_CHECK_TIMESTAMP:
//		strName = _T("时间戳");
//		dwUnitValue = CTopic::T_TIMESTAMP;
//		bSave = m_bTimeStamp;
//		break;
//	case IDC_CHECK_LINKURL:
//		strName = _T("转向链接");
//		dwUnitValue = CTopic::T_LINKURL;
//		bSave = m_bLinkUrl;
//		break;
//	case IDC_CHECK_CHANNELID:
//		strName = _T("频道ID");
//		dwUnitValue = CTopic::T_SERVERCHANNELID;
//		bSave = m_bChannelID;
//		break;
//	case IDC_CHECK_EDITOR:
//		strName = _T("责编");
//		dwUnitValue = CTopic::T_EDITOR;
//		bSave = m_bEditor;
//		break;
//	case IDC_CHECK_PRIMARY_NUMBER:
//		strName = _T("要闻编号");
//		dwUnitValue = CTopic::T_PRIMARYNUM;
//		bSave = m_bPrimaryNumber;
//		break;
//	case IDC_CHECK_SERVER_AUTOID:
//		strName = _T("自增ID");
//		dwUnitValue = CTopic::T_SERVERAUTOID;
//		bSave = m_bServerAutoID;
//		break;
//	case IDC_CHECK_RESOURCE_SIZE:
//		strName = _T("资源大小");
//		dwUnitValue = CTopic::T_RESOURCESIZE;
//		bSave = m_bResourceSize;
//		break;
//	case IDC_CHECK_ISTHUMB:
//		strName = _T("是否是缩略图");
//		dwUnitValue = CTopic::T_ISTHUMB;
//		bSave = m_bIsThumb;
//		break;
//	case IDC_CHECK_THUMB:
//		strName = _T("缩略图");
//		dwUnitValue = CTopic::T_THUMB;
//		bSave = m_bThumb;
//		break;
//	case IDC_CHECK_OLDTITLE:
//		strName = _T("原标题");
//		dwUnitValue = CTopic::T_OLDTITLE;
//		bSave = m_bOldTitle;
//		break;
//	case IDC_CHECK_OLDURL:
//		strName = _T("原链接");
//		dwUnitValue = CTopic::T_OLDURL;
//		bSave = m_bOldURL;
//		break;
//	default:
//		ASSERT(FALSE);
//	}
//	UpdateList(strName,dwUnitValue,bSave);
//}

void CUnitInfoDlg::OnOK()
{
	// 装配组合标签信息
	// 格式：unitvalue1,defaultvalue1,p1;unitvalue2,defaultvalue2,p2;......
	CString strTemp,strCombo;
	m_strUnitCombos.Empty();
	
	for( int i=0;i<m_listUnitCombo.GetItemCount();i++ )
	{
		UNIT_INFO* pui = (UNIT_INFO*)m_listUnitCombo.GetItemData(i);		
		strCombo = m_listUnitCombo.GetItemText(i,3);		

		for(int j=0;j<sizeof(unitCombos)/sizeof(CString);j++)
		{
			if( strCombo.CompareNoCase(unitCombos[j]) == 0 )
			{
				pui->nProcess = j;
				break;
			}
		}
		strTemp.Format("%d,%s,%d;",pui->nUnitValue,pui->strDefault,pui->nProcess);

		m_strUnitCombos += strTemp;

	}
	CDialogEx::OnOK();
}

void CUnitInfoDlg::UpdateList(CString strUnitName,CTopic::TopicUnit nUnitValue,BOOL bSave)
{
	//CString str;

	//if( bSave )
	//{
	//	// add the new data into the list
	//	UNIT_INFO* pui = new UNIT_INFO;
	//	pui->strName = strUnitName;
	//	pui->nValue = nUnitValue;

	//	str.Format("%d",m_listUnitCombo.GetItemCount()+1);

	//	m_listUnitCombo.SetItemData(m_listUnitCombo.AddItem(str,pui->strName,_T("无")),(DWORD)pui);
	//}
	//else
	//{
	//	// delete the old data from the list		
	//	for( int i=0;i<m_listUnitCombo.GetItemCount();i++)
	//	{
	//		UNIT_INFO* pui =(UNIT_INFO*)m_listUnitCombo.GetItemData(i);
	//		if( pui->nValue == nUnitValue )
	//		{
	//			m_listUnitCombo.DeleteItem(i);
	//			delete pui;
	//			pui = NULL;

	//			for(int j=i;j<m_listUnitCombo.GetItemCount();j++)
	//			{
	//				str.Format("%d",j+1);
	//				m_listUnitCombo.SetItemText(j,0,str);
	//			}
	//			break;
	//		}
	//	}
	//}
}


void CUnitInfoDlg::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	if(MessageBox(_T("你将删除选择的要素,确定吗？"),_T("请注意"),MB_YESNO|MB_ICONWARNING)==IDNO)
		return ;
	
	CArray<int,int> selItems;
	POSITION pos=m_listUnitCombo.GetFirstSelectedItemPosition();
	while (pos)
	{
		int i=m_listUnitCombo.GetNextSelectedItem(pos);
		selItems.Add(i);
	}
	for(int i=0;i<selItems.GetSize();i++)
	{		
		//Delete the data from listctrl
		m_listUnitCombo.DeleteItem(selItems.GetAt(i));
		//update the other pos value
		for(int j=0;j<selItems.GetSize();j++)
		{
			if(selItems.GetAt(j)>selItems.GetAt(i))
				selItems.SetAt(j,selItems.GetAt(j)-1);
		}		
		//更新列表序号信息
		CString str;
		for(int j=i;j<m_listUnitCombo.GetItemCount();j++)
		{
			str.Format("%d",j+1);
			m_listUnitCombo.SetItemText(j,0,str);
		}
		UpdateData(FALSE);
	}
}
void CUnitInfoDlg::OnUpdateButtonDel(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_listUnitCombo.GetSelectedCount() > 0);
}

void CUnitInfoDlg::OnUpdateButtonAdd(CCmdUI * pCmdUI)
{
	pCmdUI->Enable( m_comboUnitName.GetCurSel()!=CB_ERR || m_listUnitCombo.GetSelectedCount() == 1);
	pCmdUI->SetText( (m_listUnitCombo.GetSelectedCount() == 1) ?_T("修改"):_T("添加"));
}

void CUnitInfoDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	BOOL bModify = m_listUnitCombo.GetSelectedCount() == 1;

	CString strMsg;
	strMsg.Format("确认要%s选择的要素吗？",bModify ?_T("修改"):_T("添加"));

	if(MessageBox(strMsg,_T("请注意"),MB_YESNO|MB_ICONWARNING)==IDNO)
		return ;

	UpdateData();

	int nSel = m_comboUnitName.GetCurSel();
	if( bModify )
	{
		UNIT_INFO* pui = (UNIT_INFO*)m_listUnitCombo.GetItemData(m_nCurSelectedItem);
		
		pui->nUnitValue = nSel;
		pui->strDefault = m_strDefaultValue;

		m_listUnitCombo.SetItemText(m_nCurSelectedItem,1,topicUnits[nSel]);
		m_listUnitCombo.SetItemText(m_nCurSelectedItem,2,m_strDefaultValue);
		
		m_listUnitCombo.UpdateData(FALSE);
	}
	else
	{
		UNIT_INFO* pui = new UNIT_INFO;
		pui->nUnitValue = nSel;
		pui->strDefault = m_strDefaultValue;
		pui->nProcess = 0;

		CString strOder;
		strOder.Format("%d",m_listUnitCombo.GetItemCount()+1);

		m_listUnitCombo.SetItemData(m_listUnitCombo.AddItem(strOder,topicUnits[nSel],m_strDefaultValue,unitCombos[0]),(DWORD)pui);

	}

}


void CUnitInfoDlg::OnItemchangedListUnitCombo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	//更新对话框的数据显示	
	if( pNMLV->uChanged == LVIF_STATE &&  
		pNMLV->uNewState == (LVIS_FOCUSED | LVIS_SELECTED)) 
//	if( pNMLV->uNewState == 3 )
	{
		m_nCurSelectedItem = pNMLV->iItem;

		if( m_nCurSelectedItem != LB_ERR )
		{
			UNIT_INFO* pui = (UNIT_INFO*)m_listUnitCombo.GetItemData(m_nCurSelectedItem);	

			m_comboUnitName.SetCurSel(pui->nUnitValue);
			m_strDefaultValue = pui->strDefault;			
		}
	}
	else
	{
		m_nCurSelectedItem = -1;
		m_comboUnitName.SetCurSel(-1);
		m_strDefaultValue = _T("");
	}
	UpdateData(FALSE);

	*pResult = 0;
}
