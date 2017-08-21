// ServerColumn.cpp: implementation of the CServerColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "ServerColumn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerColumn::CServerColumn() : CBaseObject(OBJECT_TYPE_SERVER_COLUMN)
{
    SetImg(4,5);
}

CServerColumn::~CServerColumn()
{

}
void CServerColumn::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CServerColumn::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_SERVER_COLUMN:
		bAccept=TRUE;        //всю╦д©
		break;
	}

	return bAccept;
}
void CServerColumn::DoJoin(CBaseObject *pParent)
{
	if(pParent->GetType()==OBJECT_TYPE_SERVER_COLUMN)
	{
		m_data.m_strParentID=pParent->GetID();
	}
	else
		m_data.m_strParentID.Empty();

	CBaseObject::DoJoin(this);
}		
void CServerColumn::DoLeave(CBaseObject *pParent)
{
	m_data.m_strParentID.Empty();

	CBaseObject::DoLeave(this);
}

BOOL CServerColumn::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_k_type where type_id='%s'",m_data.m_strID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("type_id",m_data.m_strID);
		}
		else
			rs.Edit();	
		
		bRet&=rs.SetFieldValue("parent_id",m_data.m_strParentID);
		bRet&=rs.SetFieldValue("type_name",m_data.m_strColumnName.Left(40));  

		bRet&=rs.Update();	

		rs.Close();
	} 
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CServerColumn::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;
    
	//Load sub columns
	strSql.Format("select * from wdm_k_type where parent_id='%s'",m_data.m_strID);
    bRet=rs.Open(strSql,CADORecordset::openQuery);

	CServerColumn* pColumn=NULL;

    if(bRet)
	{
		while (!rs.IsEOF())
		{
			pColumn=new CServerColumn;
			pColumn->LoadInfo(rs);
			pColumn->AddTo(this);

			rs.MoveNext();
		}	
	    rs.Close();
	}

	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CServerColumn::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("type_id",m_data.m_strID);
	bRet&=rs.GetFieldValue("parent_id",m_data.m_strParentID);
	bRet&=rs.GetFieldValue("type_name",m_data.m_strColumnName);

	m_strID=m_data.m_strID;
	m_strDesc=m_data.m_strColumnName;
	
	return bRet;
}
BOOL CServerColumn::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_k_type where type_id='%s'",m_data.m_strID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	


TiXmlNode*  CServerColumn::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_server_column"));

	pTemp->SetAttribute(_T("type_id"),m_data.m_strID);
	pTemp->SetAttribute(_T("parent_id"),m_data.m_strParentID);
	pTemp->SetAttribute(_T("column_name"),m_data.m_strColumnName);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CServerColumn::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CServerColumn* pColumn=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_server_column"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("type_id"));
		if(!strID.IsEmpty())
		{
// 			pColumn=(CServerColumn*)FindSubObject(OBJECT_TYPE_SERVER_COLUMN,strID);
// 			if(pColumn==NULL)
// 			{
				pColumn=new CServerColumn;
				pColumn->m_data.m_strID=strID;
                pColumn->AddTo(this);
//			}				
			pColumn->m_data.m_strParentID=pElement->Attribute(_T("parent_id"));
	    	pColumn->m_data.m_strColumnName=pElement->Attribute(_T("column_name"));
			
			pColumn->SetID(strID,pColumn->m_data.m_strColumnName);
			pColumn->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
			
			pColumn->StreamIn(pElement,bChild,pProgressBar);

			if(pColumn->GetChildCount()>0)
				pColumn->SetImg(2,3);
		}
		
	}	
	return TRUE;
}

