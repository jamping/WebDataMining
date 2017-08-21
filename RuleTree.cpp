// RuleTree.cpp: implementation of the CRuleTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "RuleTree.h"
#include "RuleDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuleTree::CRuleTree():m_data("",-1,-1),CBaseObject(OBJECT_TYPE_RULETREE)
{
	SetImg(14,15);
}

CRuleTree::~CRuleTree()
{

}
void CRuleTree::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strRuleTreeID=strID;

	CBaseObject::SetID(strID,strDesc);
}

BOOL CRuleTree::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_RULETREE:	//可以挂接子规则
		bAccept=TRUE;
		break;
	}

	return bAccept;
}
void CRuleTree::DoJoin(CBaseObject *pParent)
{
	m_data.m_strRuleID=BackTo(OBJECT_TYPE_RULE)->GetID();
	m_data.m_strParentID=pParent->GetID();
    
	CBaseObject::DoJoin(this);
}		
void CRuleTree::DoLeave(CBaseObject *pParent)
{
    m_data.m_strParentID.Empty();

	CBaseObject::DoLeave(this);
}
UINT CRuleTree::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;

	CRuleDlg dlg(bReadOnly);

	dlg.SetValue(m_data);
	nRet=dlg.DoModal();

	if (nRet== IDOK )
	{
		if(dlg.GetValue(m_data))
			SetModify();
		ChangeDesc(m_data.m_strRuleTreeName);
	}
	return nRet;
}
BOOL CRuleTree::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_rule_tree where rule_tree_id='%s'",m_data.m_strRuleTreeID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("rule_tree_id",m_data.m_strRuleTreeID);
		}
		else
			rs.Edit();

		bRet&=rs.SetFieldValue("rule_id",m_data.m_strRuleID);  
		bRet&=rs.SetFieldValue("parent_id",m_data.m_strParentID);  
		bRet&=rs.SetFieldValue("rule_tree_name",m_data.m_strRuleTreeName);  
		bRet&=rs.SetFieldValue("match_rule",m_data.m_strMatchRule);  
		bRet&=rs.SetFieldValue("process_type",m_data.m_nProcessType);  
		bRet&=rs.SetFieldValue("save_type",m_data.m_nSaveType);  
		bRet&=rs.Update();

		rs.Close();
	}
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CRuleTree::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_rule_tree where parent_id='%s'",m_data.m_strRuleTreeID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CRuleTree* pRuleTree=NULL;
	if(bRet)
	{
		while (!rs.IsEOF())
		{
			pRuleTree=new CRuleTree;
			pRuleTree->LoadInfo(rs);
			pRuleTree->AddTo(this);
			rs.MoveNext();
		}

		rs.Close();
	}
	bRet&=CBaseObject::LoadFromDB(db,pProgressBar);

	return bRet;
}
BOOL CRuleTree::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet &=rs.GetFieldValue("rule_tree_id",m_data.m_strRuleTreeID);
	bRet &=rs.GetFieldValue("rule_id",m_data.m_strRuleID);
	bRet &=rs.GetFieldValue("parent_id",m_data.m_strParentID);
	bRet &=rs.GetFieldValue("rule_tree_name",m_data.m_strRuleTreeName);
	bRet &=rs.GetFieldValue("match_rule",m_data.m_strMatchRule);
	bRet &=rs.GetFieldValue("process_type",m_data.m_nProcessType);
	bRet &=rs.GetFieldValue("save_type",m_data.m_nSaveType);
	
	m_strID=m_data.m_strRuleTreeID;
	m_strDesc=m_data.m_strRuleTreeName;

	return bRet;
}
BOOL CRuleTree::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_rule_tree where rule_tree_id='%s'",m_data.m_strRuleTreeID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CRuleTree::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CRuleTree::PromptMenuPos()
{
	return 3;
}
void CRuleTree::UpdateMenu(CMenu *pMenu)
{
	UINT newSubRuleID=RetrieveMenuItemID(pMenu,"新建子规则");
	UINT delID=RetrieveMenuItemID(pMenu,"删除规则");

	ASSERT(newSubRuleID!=-1 && delID!=-1);
	
	int nLevel=theApp.GetUserLevel();

    pMenu->EnableMenuItem(newSubRuleID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    //Delete the unused items
	pMenu->DeleteMenu(7,MF_BYPOSITION);
	pMenu->DeleteMenu(6,MF_BYPOSITION);
}

void CRuleTree::SetValue(RuleTreeData& data)
{
	m_data.m_strRuleTreeName=data.m_strRuleTreeName;
	m_data.m_nProcessType=data.m_nProcessType;
	m_data.m_nSaveType=data.m_nSaveType;
}
BOOL CRuleTree::IsValid()
{
	return m_data.m_strMatchRule.GetLength()>0;
}
void CRuleTree::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
	CString strText;
	CStringArray strList;

	if(bSelf)
	{	
		strText=_T("子规则名称,150;匹配规则,300;保存类型,100;处理类型,300");					  
		pListCtrl->SetHeadings(strText);		
		
		strList.Add(m_data.m_strRuleTreeName);
		strList.Add(m_data.m_strMatchRule);
		
		switch(m_data.m_nSaveType)
		{
		case CRuleTree::SaveHeadTitle:
			strList.Add(_T("引标"));
			break;
		case CRuleTree::SaveTitle:
			strList.Add(_T("标题"));
			break;
		case CRuleTree::SaveSubTitle:
			strList.Add(_T("副标"));
			break;
		case CRuleTree::SaveSource:
			strList.Add(_T("来源"));
			break;
		case CRuleTree::SaveTime:
			strList.Add(_T("时间"));
			break;
		case CRuleTree::SaveAuthor:
			strList.Add(_T("作者"));
			break;
		case CRuleTree::SaveContent:
			strList.Add(_T("内容"));
			break;	
		default:
			strList.Add(_T(""));
		}
		switch(m_data.m_nProcessType)
		{
		case CRuleTree::SaveOfBackParam:
			strList.Add(_T("用后面参数保存"));
			break;
		case CRuleTree::DeleteFront:
			strList.Add(_T("删除前面所有内容"));
			break;
		case CRuleTree::DeleteBack:
			strList.Add(_T("删除后面所有内容"));
			break;
		case CRuleTree::DeleteSelf:
			strList.Add(_T("删除本身内容"));
			break;		
		default:
			strList.Add(_T(""));
		}		
		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);				 
	}
	else
	{
		strText=_T("序号,60;子规则名称,150;匹配规则,300;保存类型,100;处理类型,300");					  
		pListCtrl->SetHeadings(strText);		
		
		int i=0;
		POSITION pos=GetFirstChildPosition();
		while (pos)
		{
			CRuleTree* pSubRule=(CRuleTree*)GetNextChild(pos);
			if(pSubRule!=NULL)
			{
                strList.RemoveAll();
				strText.Format("%d",++i);
				strList.Add(strText);
				strList.Add(pSubRule->m_data.m_strRuleTreeName);
				strList.Add(pSubRule->m_data.m_strMatchRule);
				
				switch(pSubRule->m_data.m_nSaveType)
				{
				case CRuleTree::SaveHeadTitle:
					strList.Add(_T("引标"));
					break;
				case CRuleTree::SaveTitle:
					strList.Add(_T("标题"));
					break;
				case CRuleTree::SaveSubTitle:
					strList.Add(_T("副标"));
					break;
				case CRuleTree::SaveSource:
					strList.Add(_T("来源"));
					break;
				case CRuleTree::SaveTime:
					strList.Add(_T("时间"));
					break;
				case CRuleTree::SaveAuthor:
					strList.Add(_T("作者"));
					break;
				case CRuleTree::SaveContent:
					strList.Add(_T("内容"));
					break;	
				default:
					strList.Add(_T(""));
				}
				switch(pSubRule->m_data.m_nProcessType)
				{
				case CRuleTree::SaveOfBackParam:
					strList.Add(_T("用后面参数保存"));
					break;
				case CRuleTree::DeleteFront:
					strList.Add(_T("删除前面所有内容"));
					break;
				case CRuleTree::DeleteBack:
					strList.Add(_T("删除后面所有内容"));
					break;
				case CRuleTree::DeleteSelf:
					strList.Add(_T("删除本身内容"));
					break;		
				default:
					strList.Add(_T(""));
				}		
	        	pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pSubRule);
			}
		}
	}
}
TiXmlNode*  CRuleTree::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_rule_tree"));

	pTemp->SetAttribute(_T("rule_tree_id"),m_data.m_strRuleTreeID);
	pTemp->SetAttribute(_T("rule_id"),m_data.m_strRuleID);
	pTemp->SetAttribute(_T("parent_id"),m_data.m_strParentID);
	pTemp->SetAttribute(_T("rule_tree_name"),m_data.m_strRuleTreeName);
	pTemp->SetAttribute(_T("match_rule"),m_data.m_strMatchRule);
	pTemp->SetAttribute(_T("process_type"),m_data.m_nProcessType);
	pTemp->SetAttribute(_T("save_type"),m_data.m_nSaveType);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CRuleTree::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	CRuleTree* pRuleTree=NULL;
	for(pElement=pNode->FirstChildElement(_T("wdm_rule_tree"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("rule_tree_id"));
		if(!strID.IsEmpty())
		{
			pRuleTree=(CRuleTree*)FindSubObject(OBJECT_TYPE_RULETREE,strID);
			if(pRuleTree==NULL)
			{
				pRuleTree=new CRuleTree;
				pRuleTree->m_data.m_strRuleTreeID=strID;			
				pRuleTree->AddTo(this);				
			}
			pRuleTree->m_data.m_strRuleID=pElement->Attribute(_T("rule_id"));
			pRuleTree->m_data.m_strParentID=pElement->Attribute(_T("parent_id"));
			pRuleTree->m_data.m_strRuleTreeName=pElement->Attribute(_T("rule_tree_name"));
			pRuleTree->m_data.m_strMatchRule=pElement->Attribute(_T("match_rule"));
			pElement->QueryIntAttribute(_T("process_type"),&(pRuleTree->m_data.m_nProcessType));
			pElement->QueryIntAttribute(_T("save_type"),&(pRuleTree->m_data.m_nSaveType));
			
			pRuleTree->SetID(strID,pRuleTree->m_data.m_strRuleTreeName);
			pRuleTree->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

			pRuleTree->StreamIn(pElement,bChild,pProgressBar);
		}		
	}
	return TRUE;
}