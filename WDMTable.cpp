// WDMTable.cpp: implementation of the CWDMTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WDMTable.h"
#include "WDMOperator.h"
#include "ServerTableDlg.h"
#include "WDMOperatorUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWDMTable::CWDMTable():CBaseObject(OBJECT_TYPE_TABLE)
{
    SetImg(24,25);
}

CWDMTable::~CWDMTable()
{

}
void CWDMTable::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strTableID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CWDMTable::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;
    switch(pOB->GetType())
    {
// 	case OBJECT_TYPE_TABLE:
// 		bAccept=TRUE;
// 		break;
	case OBJECT_TYPE_OPERATOR:
		bAccept=TRUE;
		break;
    }
	return bAccept;
}
void CWDMTable::DoJoin(CBaseObject *pParent)
{
	m_data.m_strDatabaseID=pParent->GetID();
	CBaseObject::DoJoin(this);
}		
void CWDMTable::DoLeave(CBaseObject *pParent)
{
	m_data.m_strDatabaseID.Empty();
	CBaseObject::DoLeave(this);
}
UINT CWDMTable::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CServerTableDlg dlg(this);

	nRet=dlg.DoModal();
	if(nRet== IDOK)
	{
		if(dlg.GetValue(m_data))
			SetModify();
		m_strDesc=m_data.m_strTableName;
	}
	return nRet;
}
BOOL CWDMTable::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_table where table_id='%s'",m_data.m_strTableID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("table_id",m_data.m_strTableID);
		}
		else
			rs.Edit();
	
		bRet&=rs.SetFieldValue("database_id",m_data.m_strDatabaseID);
		bRet&=rs.SetFieldValue("table_name",m_data.m_strTableName);
		bRet&=rs.SetFieldValue("table_type",m_data.m_nTableType);
		bRet&=rs.SetFieldValue("process_order",m_data.m_nProcessOrder);
		bRet&=rs.SetFieldValue("publish_repeat",(int)m_data.m_bPublishRepeat);

		bRet&=rs.Update();	

		rs.Close();
	}

	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CWDMTable::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;

	strSql.Format("select * from wdm_operator where table_id='%s'",m_data.m_strTableID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CWDMOperator* pOperator=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pOperator=new CWDMOperator;
			pOperator->LoadInfo(rs);
			pOperator->AddTo(this);
 
			rs.MoveNext();
		}
		rs.Close();
	}
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CWDMTable::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("table_id",m_data.m_strTableID);
	bRet&=rs.GetFieldValue("database_id",m_data.m_strDatabaseID);
	bRet&=rs.GetFieldValue("table_name",m_data.m_strTableName);
    bRet&=rs.GetFieldValue("table_type",m_data.m_nTableType);
	bRet&=rs.GetFieldValue("process_order",m_data.m_nProcessOrder);
	bRet&=rs.GetFieldValue("publish_repeat",m_data.m_bPublishRepeat);

	m_strID=m_data.m_strTableID;
	m_strDesc=m_data.m_strTableName;
	
	return bRet;
}
BOOL CWDMTable::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_table where table_id='%s'",m_data.m_strTableID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CWDMTable::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CWDMTable::PromptMenuPos()
{
	return 9;
}
void CWDMTable::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除数据表");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}


CWDMOperator* CWDMTable::GetOperator(CWDMOperator::OperatorType type)
{
    CWDMOperator* pOperator=NULL;
	CString strOper;
	switch(type)
	{
	case CWDMOperator::opSave:
        strOper=_T("保存");
		break;
	case CWDMOperator::opDelete:
		strOper=_T("删除");
		break;
	default:			
        ASSERT(FALSE);
	}
	BOOL bFound=FALSE;
	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		pOperator=(CWDMOperator*)GetNextChild(pos);
		if(pOperator!=NULL && strOper.CompareNoCase(pOperator->m_data.m_strOperatorName)==0)
		{
			bFound=TRUE;
			break;
		}
	}
	return bFound?pOperator:NULL;
}
void CWDMTable::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
	CString strText;
	CStringArray strList;

	if(bSelf)
	{
        strText=_T("数据表名,200;数据表类型,100;处理优先级,100;允许重发,100");
		pListCtrl->SetHeadings(strText);
        strList.Add(m_data.m_strTableName);
        switch(m_data.m_nTableType)
        {
		case 0:
			strList.Add(_T("预处理"));
			break;
		case 1:
			strList.Add(_T("文章"));
			break;
		case 2:
			strList.Add(_T("图片"));
			break;
		case 3:
			strList.Add(_T("后处理"));
			break;
		case 4:
			strList.Add(_T("转向链接"));
			break;
        }
		strText.Format("%d级",m_data.m_nProcessOrder);
		strList.Add(strText);
		strList.Add(m_data.m_bPublishRepeat?_T("是"):_T("否"));

		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
	}
	else
	{
 		 strText=_T("序号,60;操作名称,100");
		 pListCtrl->SetHeadings(strText);
         
		 int i=0;
		 POSITION pos=GetFirstChildPosition();
		 while (pos)
		 {
			 CWDMOperator* pOperator=(CWDMOperator*)GetNextChild(pos);
			 if(pOperator!=NULL)
			 {
				 strList.RemoveAll();
				 
				 strText.Format("%d",++i);
				 strList.Add(strText);
				 
				 strList.Add(pOperator->m_data.m_strOperatorName);	
				 
				 pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pOperator);
			 }

		 }
	
	}
}

TiXmlNode*  CWDMTable::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_table"));

	pTemp->SetAttribute(_T("table_id"),m_data.m_strTableID);
	pTemp->SetAttribute(_T("database_id"),m_data.m_strDatabaseID);
	pTemp->SetAttribute(_T("table_name"),m_data.m_strTableName);
	pTemp->SetAttribute(_T("table_type"),m_data.m_nTableType);
	pTemp->SetAttribute(_T("process_order"),m_data.m_nProcessOrder);
	pTemp->SetAttribute(_T("publish_repeat"),m_data.m_bPublishRepeat);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CWDMTable::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CWDMOperator* pOperator=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_operator"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("operator_id"));
		if(!strID.IsEmpty())
		{
			pOperator=(CWDMOperator*)FindSubObject(OBJECT_TYPE_OPERATOR,strID);
			if(pOperator==NULL)
			{
				pOperator=new CWDMOperator;
				pOperator->m_data.m_strOperatorID=strID;			
				pOperator->AddTo(this);				
			}
			pOperator->m_data.m_strOperatorName=pElement->Attribute(_T("operator_name"));
			pOperator->m_data.m_strTableID=pElement->Attribute(_T("table_id"));		
			
			pOperator->SetID(strID,pOperator->m_data.m_strOperatorName);
			pOperator->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

			pOperator->StreamIn(pElement,bChild,pProgressBar);
		}		
	}
	return TRUE;
}