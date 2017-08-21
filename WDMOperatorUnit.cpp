// WDMOperatorUnit.cpp: implementation of the CWDMOperatorUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WDMOperatorUnit.h"
#include "UnitInfoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWDMOperatorUnit::CWDMOperatorUnit():CBaseObject(OBJECT_TYPE_OPERATOR_UNIT)
{
    SetImg(24,25);
}

CWDMOperatorUnit::~CWDMOperatorUnit()
{

}
void CWDMOperatorUnit::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strUnitID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CWDMOperatorUnit::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	return bAccept;
}
void CWDMOperatorUnit::DoJoin(CBaseObject *pParent)
{
	m_data.m_strOperatorID=pParent->GetID();
	CBaseObject::DoJoin(this);
}		
void CWDMOperatorUnit::DoLeave(CBaseObject *pParent)
{
	m_data.m_strOperatorID.Empty();
	CBaseObject::DoLeave(this);
}

UINT CWDMOperatorUnit::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CUnitInfoDlg dlg(this);

	dlg.SetValue(m_data);
	nRet=dlg.DoModal();
	if(nRet== IDOK)
	{
		if(dlg.GetValue(m_data))
			SetModify();
		m_strDesc=m_data.m_strUnitName;
	}
	return nRet;
}
BOOL CWDMOperatorUnit::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_operator_unit where unit_id='%s'",m_data.m_strUnitID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("unit_id",m_data.m_strUnitID);
		}
		else
			rs.Edit();
	
		bRet&=rs.SetFieldValue("operator_id",m_data.m_strOperatorID);
		bRet&=rs.SetFieldValue("unit_name",m_data.m_strUnitName);
		bRet&=rs.SetFieldValue("unit_length",m_data.m_nUnitLength);
		bRet&=rs.SetFieldValue("unit_default",m_data.m_strUnitDefault);
		bRet&=rs.SetFieldValue("unit_isnull",m_data.m_strUnitIsNull);
		bRet&=rs.SetFieldValue("unit_key",m_data.m_strUnitKey);
		//char str[65];
		//_ui64tot_s(m_data.m_dwTopicUnit,str,65,10);
		//CString strUnit = str;
		//bRet&=rs.SetFieldValue("topic_unit",strUnit); 

		bRet&=rs.SetFieldValue("unit_type",m_data.m_strUnitType); 
		bRet&=rs.SetFieldValue("is_key",(int)m_data.m_bIsKey); 
		bRet&=rs.SetFieldValue("is_cache",(int)m_data.m_bCache);
		bRet&=rs.SetFieldValue("unit_combos",m_data.m_strUnitCombos);  
		bRet&=rs.SetFieldValue("process_order",m_data.m_nProcessOrder); 

		bRet&=rs.Update();	

		rs.Close();
	}

	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CWDMOperatorUnit::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CWDMOperatorUnit::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("unit_id",m_data.m_strUnitID);
	bRet&=rs.GetFieldValue("operator_id",m_data.m_strOperatorID);
	bRet&=rs.GetFieldValue("unit_name",m_data.m_strUnitName);
	bRet&=rs.GetFieldValue("unit_length",m_data.m_nUnitLength);
	bRet&=rs.GetFieldValue("unit_default",m_data.m_strUnitDefault);
	bRet&=rs.GetFieldValue("unit_isnull",m_data.m_strUnitIsNull);
	bRet&=rs.GetFieldValue("unit_key",m_data.m_strUnitKey);
	//CString str;
 //   bRet&=rs.GetFieldValue("topic_unit",str);
	//m_data.m_dwTopicUnit = _tcstoui64((LPCTSTR)str,0,10);

	bRet&=rs.GetFieldValue("unit_type",m_data.m_strUnitType);
    bRet&=rs.GetFieldValue("is_key",m_data.m_bIsKey);
	bRet&=rs.GetFieldValue("is_cache",m_data.m_bCache);
	bRet&=rs.GetFieldValue("unit_combos",m_data.m_strUnitCombos); 
	bRet&=rs.GetFieldValue("process_order",m_data.m_nProcessOrder);

// #ifdef _DEBUG
// 	if( m_data.m_strUnitCombos.IsEmpty() )
// 		m_data.m_strUnitCombos.Format("%d,0;",m_data.m_lTopicUnit);
// #endif

	m_strID=m_data.m_strUnitID;
	m_strDesc=m_data.m_strUnitName;

	return bRet;
}
BOOL CWDMOperatorUnit::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_operator_unit where unit_id='%s'",m_data.m_strUnitID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CWDMOperatorUnit::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CWDMOperatorUnit::PromptMenuPos()
{
	return 11;
}
void CWDMOperatorUnit::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除要素");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}
void CWDMOperatorUnit::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
	CString strText;
	CStringArray strList;

	if(bSelf)
	{
		 strText=_T("字段名称,100;关键字,100;字段类型,100;最大长度,100;缺省值,100;允许为空,100;主键情况,100;保存为缓存值,100;处理优先级,100;要素组合,200");
		 pListCtrl->SetHeadings(strText);

		 strList.Add(m_data.m_strUnitName);
		 strList.Add(m_data.m_bIsKey?_T("是"):_T("否"));
		 strList.Add(m_data.m_strUnitType);
		 strText.Format("%d",m_data.m_nUnitLength);
		 strList.Add(strText);
		 strList.Add(m_data.m_strUnitDefault);
		 strList.Add(m_data.m_strUnitIsNull);
		 strList.Add(m_data.m_strUnitKey);
		 strList.Add(m_data.m_bCache?_T("是"):_T("否"));
		 strText.Format("%d",m_data.m_nProcessOrder);
		 strList.Add(strText);
		 strList.Add(m_data.m_strUnitCombos);

		 pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
	}
	else
	{
 		strText=_T("没有子对象,150");  
		pListCtrl->SetHeadings(strText);	
	}
}

TiXmlNode*  CWDMOperatorUnit::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_unit"));

	pTemp->SetAttribute(_T("unit_id"),m_data.m_strUnitID);
	pTemp->SetAttribute(_T("operator_id"),m_data.m_strOperatorID);
	pTemp->SetAttribute(_T("unit_name"),m_data.m_strUnitName);
	pTemp->SetAttribute(_T("unit_length"),m_data.m_nUnitLength);
	pTemp->SetAttribute(_T("unit_default"),m_data.m_strUnitDefault);
	pTemp->SetAttribute(_T("unit_key"),m_data.m_strUnitKey);
	pTemp->SetAttribute(_T("unit_isnull"),m_data.m_strUnitIsNull);

	//char str[65];
	//_ui64tot_s(m_data.m_dwTopicUnit,str,65,10);
	//pTemp->SetAttribute(_T("topic_unit"),str);

//	pTemp->SetAttribute(_T("unit_value"),m_data.m_strUnitDefault);
	pTemp->SetAttribute(_T("unit_type"),m_data.m_strUnitType);
	pTemp->SetAttribute(_T("is_key"),m_data.m_bIsKey);
	pTemp->SetAttribute(_T("is_cache"),m_data.m_bCache);
	pTemp->SetAttribute(_T("unit_combos"),m_data.m_strUnitCombos);
//	pTemp->SetAttribute(_T("unit_separator"),m_data.m_strUnitSeparator);
	pTemp->SetAttribute(_T("process_order"),m_data.m_nProcessOrder);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}


BOOL CWDMOperatorUnit::ContainTopicUnit(CTopic::TopicUnit nUnit)
{
	BOOL bRet = FALSE;

	int i = 0,nUnitValue;
	CString strUintCombo,strUnit;
	while( AfxExtractSubString(strUintCombo,m_data.m_strUnitCombos,i,_T(';')))
	{		
		// Prepare to move to the next substring
		i++;
		if( strUintCombo.IsEmpty())
			continue;

		// The data format is
		// unit vale1,default value1,combo value1;unit vale2,default value2,combo value2;...	
		// Attempt to extract the name and value element from the pair
		if ( AfxExtractSubString(strUnit, strUintCombo, 0, _T(',')) )
		{
			nUnitValue = atoi(strUnit);

			if( nUnitValue == nUnit )
			{
				bRet = TRUE;
				break;
			}
		}
	}
	return bRet;
}


CString CWDMOperatorUnit::GetKeyString()
{
	CString strKey;

	CWDMTable* pTable = (CWDMTable*)BackTo(OBJECT_TYPE_TABLE);

	strKey.Format("%s_%s",pTable->GetDesc(),GetDesc());

	return strKey;
}

int CWDMOperatorUnit::GetUnitNum()
{
	int i = 0;
	CString strUintCombo;
	while( AfxExtractSubString(strUintCombo,m_data.m_strUnitCombos,i,_T(';')))
	{		
		// Prepare to move to the next substring
		i++;
		if( strUintCombo.IsEmpty())
			continue;
	}
	return i-1;
}