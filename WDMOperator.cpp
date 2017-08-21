// WDMOperator.cpp: implementation of the CWDMOperator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WDMOperator.h"
#include "WDMOperatorUnit.h"
#include "SelContentDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWDMOperator::CWDMOperator():CBaseObject(OBJECT_TYPE_OPERATOR)
{
    SetImg(26,27);
}

CWDMOperator::~CWDMOperator()
{

}
void CWDMOperator::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strOperatorID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CWDMOperator::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;
    switch(pOB->GetType())
    {
	case OBJECT_TYPE_OPERATOR_UNIT:
		bAccept=TRUE;
		break;
    }
	return bAccept;
}
void CWDMOperator::DoJoin(CBaseObject *pParent)
{
	m_data.m_strTableID=pParent->GetID();
	CBaseObject::DoJoin(this);
}		
void CWDMOperator::DoLeave(CBaseObject *pParent)
{
	m_data.m_strTableID.Empty();
	CBaseObject::DoLeave(this);
}
UINT CWDMOperator::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CSelContentDlg dlg(this);

	nRet=dlg.DoModal();
	if(nRet== IDOK)
	{
		if(!CCommonFunction::IsSameString(m_data.m_strOperatorName,dlg.m_strSelContent))
		{
			SetModify();
			m_data.m_strOperatorName=dlg.m_strSelContent;
		    m_strDesc=m_data.m_strOperatorName;
		}
	}
	return nRet;
}
BOOL CWDMOperator::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_operator where operator_id='%s'",m_data.m_strOperatorID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("operator_id",m_data.m_strOperatorID);
		}
		else
			rs.Edit();
	
		bRet&=rs.SetFieldValue("table_id",m_data.m_strTableID);
		bRet&=rs.SetFieldValue("operator_name",m_data.m_strOperatorName); 
		bRet&=rs.Update();	

		rs.Close();
	}

	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CWDMOperator::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;

	strSql.Format("select * from wdm_operator_unit where operator_id='%s' order by process_order asc",m_data.m_strOperatorID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CWDMOperatorUnit* pOperatorUnit=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pOperatorUnit=new CWDMOperatorUnit;
			pOperatorUnit->LoadInfo(rs);
			pOperatorUnit->AddTo(this);

			rs.MoveNext();
		}
		rs.Close();
	}
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CWDMOperator::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("operator_id",m_data.m_strOperatorID);
	bRet&=rs.GetFieldValue("table_id",m_data.m_strTableID);
	bRet&=rs.GetFieldValue("operator_name",m_data.m_strOperatorName);
  
	m_strID=m_data.m_strOperatorID;
	m_strDesc=m_data.m_strOperatorName;
	
	return bRet;
}
BOOL CWDMOperator::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_operator where operator_id='%s'",m_data.m_strOperatorID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CWDMOperator::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CWDMOperator::PromptMenuPos()
{
	return 10;
}
void CWDMOperator::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除操作");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}
BOOL CWDMOperator::IsValid()
{
    if(GetChildCount()==0)
		return FALSE;

	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)GetNextChild(pos);
		if(pUnit->m_data.m_bIsKey)
			return TRUE;
	}
	return FALSE;
}
BOOL CWDMOperator::IsConverted()
{
	//POSITION pos=GetFirstChildPosition();
	//while (pos)
	//{
	//	CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)GetNextChild(pos);
	//	if(pUnit->m_data.m_dwTopicUnit & CTopic::T_SERVERCHANNELID ||
	//	   pUnit->m_data.m_dwTopicUnit & CTopic::T_SERVERCOLUMNID)
	//		return TRUE;
	//}
	return FALSE;
}
void CWDMOperator::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
	CString strText;
	CStringArray strList;

	if(bSelf)
	{
        strText=_T("操作名称,200");
		pListCtrl->SetHeadings(strText);

		pListCtrl->SetItemData(pListCtrl->AddItem(m_data.m_strOperatorName),(DWORD)this);
	}
	else
	{
 		 strText=_T("序号,60;字段名称,100;关键字,100;字段类型,100;最大长度,100;缺省值,100;允许为空,100;主键情况,100;保存为缓存值,100;处理优先级,100;要素组合,200");
		 pListCtrl->SetHeadings(strText);
         
		 int i=0;
		 POSITION pos=GetFirstChildPosition();
		 while (pos)
		 {
			 CWDMOperatorUnit* pUnit=(CWDMOperatorUnit*)GetNextChild(pos);
			 if(pUnit!=NULL)
			 {
				 strList.RemoveAll();
				 
				 strText.Format("%d",++i);
				 strList.Add(strText);
				 
				 strList.Add(pUnit->m_data.m_strUnitName);
				 strList.Add(pUnit->m_data.m_bIsKey?_T("是"):_T("否"));
				 strList.Add(pUnit->m_data.m_strUnitType);
				 strText.Format("%d",pUnit->m_data.m_nUnitLength);
				 strList.Add(strText);
				 strList.Add(pUnit->m_data.m_strUnitDefault);
				 strList.Add(pUnit->m_data.m_strUnitIsNull);
				 strList.Add(pUnit->m_data.m_strUnitKey);
				 strList.Add(pUnit->m_data.m_bCache?_T("是"):_T("否"));	
				 strText.Format("%d",pUnit->m_data.m_nProcessOrder);
				 strList.Add(strText);
				 strList.Add(pUnit->m_data.m_strUnitCombos);

				 pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pUnit);
			 }

		 }
	
	}
}

TiXmlNode*  CWDMOperator::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_operator"));

	pTemp->SetAttribute(_T("operator_id"),m_data.m_strOperatorID);
	pTemp->SetAttribute(_T("table_id"),m_data.m_strTableID);
	pTemp->SetAttribute(_T("operator_name"),m_data.m_strOperatorName);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CWDMOperator::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CWDMOperatorUnit* pUnit=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_unit"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("unit_id"));
		if(!strID.IsEmpty())
		{
			pUnit=(CWDMOperatorUnit*)FindSubObject(OBJECT_TYPE_OPERATOR_UNIT,strID);
			if(pUnit==NULL)
			{
				pUnit=new CWDMOperatorUnit;
				pUnit->m_data.m_strUnitID=strID;			
				pUnit->AddTo(this);				
			}
			pUnit->m_data.m_strUnitName=pElement->Attribute(_T("unit_name"));
			pUnit->m_data.m_strOperatorID=pElement->Attribute(_T("operator_id"));
//			pUnit->m_data.m_strUnitDefault=pElement->Attribute(_T("unit_value"));
			pUnit->m_data.m_strUnitType=pElement->Attribute(_T("unit_type"));	
			pElement->QueryIntAttribute(_T("unit_length"),&(pUnit->m_data.m_nUnitLength));
			pUnit->m_data.m_strUnitDefault=pElement->Attribute(_T("unit_default"));
			pUnit->m_data.m_strUnitKey=pElement->Attribute(_T("unit_key"));
			pUnit->m_data.m_strUnitIsNull=pElement->Attribute(_T("unit_isnull"));
			//CString str = pElement->Attribute(_T("topic_unit"));
			//pUnit->m_data.m_dwTopicUnit = _tcstoui64((LPCTSTR)str,0,10);

			pElement->QueryIntAttribute(_T("is_key"),&((int)pUnit->m_data.m_bIsKey));
			pElement->QueryIntAttribute(_T("is_cache"),&((int)pUnit->m_data.m_bCache));
			pUnit->m_data.m_strUnitCombos=pElement->Attribute(_T("unit_combos"));
//			pUnit->m_data.m_strUnitSeparator=pElement->Attribute(_T("unit_separator"));
			pElement->QueryIntAttribute(_T("process_order"),&(pUnit->m_data.m_nProcessOrder));

			pUnit->SetID(strID,pUnit->m_data.m_strUnitName);
			pUnit->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

			pUnit->StreamIn(pElement,bChild,pProgressBar);
		}		
	}
	return TRUE;
}


int CWDMOperator::GetWDMOperatorUnits(CTypedPtrArray<CPtrArray,CWDMOperatorUnit*>& units)
{
	units.RemoveAll();

	CWDMOperatorUnit* pUnit = NULL;
	POSITION pos=GetFirstChildPosition();

	while (pos)
	{
		pUnit=(CWDMOperatorUnit*)GetNextChild(pos);
		if( pUnit!=NULL )
		{			
			units.Add(pUnit);
		}		
	}
	//select sort
	register int i, j, min; 
	for( i=0;i<units.GetCount()-1;i++ )
	{
		min = i;
		//查找最小值 
		for( j=i+1;j<units.GetCount();j++)
		{
			if( units[min]->m_data.m_nProcessOrder > units[j]->m_data.m_nProcessOrder )
				min = j;
		}
		//交换
		if( min != i )
		{
			pUnit = units[min];
			units[min] = units[i];
			units[i] = pUnit;
		}
	}

	return units.GetCount();
}