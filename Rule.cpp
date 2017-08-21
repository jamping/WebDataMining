// Rule.cpp: implementation of the CRule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "Rule.h"
#include "RuleInfoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRule::CRule():CBaseObject(OBJECT_TYPE_RULE)
{
	SetImg(12,13);
}

CRule::~CRule()
{
}
void CRule::NewSubRules(CString strRuleTreeName,int nProcessType,int nSaveType)
{
	CRuleTree* pRuleTree=NULL;

	pRuleTree=new CRuleTree;
	pRuleTree->SetID(CCommonFunction::AllocateID(),strRuleTreeName);
	pRuleTree->SetValue(CRuleTree::RuleTreeData(strRuleTreeName,nProcessType,nSaveType));
    pRuleTree->AddTo(this);
}

void CRule::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strRuleID=strID;

	CBaseObject::SetID(strID,strDesc);
}

BOOL CRule::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_RULETREE:	//可以挂接规则树
		bAccept=TRUE;
		break;
	}

	return bAccept;
}
void CRule::DoJoin(CBaseObject *pParent)
{	
	CBaseObject::DoJoin(this);
}		
void CRule::DoLeave(CBaseObject *pParent)
{
	CBaseObject::DoLeave(this);
}
UINT CRule::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CRuleInfoDlg dlg;

	dlg.SetValue(m_data);

	if((nRet=dlg.DoModal())==IDOK)
	{
		if(dlg.GetValue(m_data))
			SetModify();
		m_strDesc=m_data.m_strRuleName;
	}
	return nRet;
}
BOOL CRule::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_rule where rule_id='%s'",m_data.m_strRuleID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("rule_id",m_data.m_strRuleID);
		}
		else
			rs.Edit();

		bRet&=rs.SetFieldValue("rule_name",m_data.m_strRuleName);
		bRet&=rs.SetFieldValue("rule_encoding",m_data.m_nRuleEncoding);
		bRet&=rs.Update();

		rs.Close();
	}
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CRule::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_rule_tree where rule_id='%s' and parent_id='%s'",
		          m_data.m_strRuleID,m_data.m_strRuleID);
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
BOOL CRule::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet &=rs.GetFieldValue("rule_id",m_data.m_strRuleID);
	bRet &=rs.GetFieldValue("rule_name",m_data.m_strRuleName);
    bRet &=rs.GetFieldValue("rule_encoding",m_data.m_nRuleEncoding);
	
	m_strID=m_data.m_strRuleID;
	m_strDesc=m_data.m_strRuleName;

	return bRet;
}
BOOL CRule::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_rule where rule_id='%s'",m_data.m_strRuleID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CRule::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CRule::PromptMenuPos()
{
	return 3;
}
void CRule::UpdateMenu(CMenu *pMenu)
{
	UINT newSubRuleID=RetrieveMenuItemID(pMenu,"新建子规则");
	UINT delID=RetrieveMenuItemID(pMenu,"删除规则");

	ASSERT(newSubRuleID!=-1 && delID!=-1);
	
	int nLevel=theApp.GetUserLevel();

    pMenu->EnableMenuItem(newSubRuleID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));

	pMenu->DeleteMenu(4,MF_BYPOSITION);
}
void CRule::ShowInTreeView(CCustomTree *pTree,BOOL bChild /*=FALSE*/)
{
	CBaseObject::ShowInTreeView(pTree,bChild);
	((CTreeCtrl *)pTree)->EnsureVisible(m_hItem);

}
void CRule::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
   
	CString strText;
	CStringArray strList;

	if(bSelf)
	{
		strText=_T("规则名称,300;编码方式,100");
    	pListCtrl->SetHeadings(strText);

        strList.Add(GetDesc());
		if(m_data.m_nRuleEncoding==0)
			strList.Add(_T("GB2312"));
		else
			strList.Add(_T("UTF-8"));

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
			CRuleTree* pRule=(CRuleTree*)GetNextChild(pos);
			if(pRule!=NULL)
			{
				strList.RemoveAll();

				strText.Format("%d",++i);
				strList.Add(strText);
				strList.Add(pRule->m_data.m_strRuleTreeName);
				strList.Add(pRule->m_data.m_strMatchRule);

	            switch(pRule->m_data.m_nSaveType)
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
			    switch(pRule->m_data.m_nProcessType)
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
				
				pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pRule);
			}
		}		 
	}
}
TiXmlNode*  CRule::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_rule"));

	pTemp->SetAttribute(_T("rule_id"),m_data.m_strRuleID);
	pTemp->SetAttribute(_T("rule_name"),m_data.m_strRuleName);
    pTemp->SetAttribute(_T("rule_encoding"),m_data.m_nRuleEncoding);
	
	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
BOOL CRule::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
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