// RuleTree.h: interface for the CRuleTree class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"

class CRuleTree : public CBaseObject  
{
public:
	enum  ProcessType
	{
		SaveOfBackParam=0,
		DeleteFront,
		DeleteBack,
		DeleteSelf
	};
	enum SaveType
	{
		SaveHeadTitle=0,
		SaveTitle,
		SaveSubTitle,
		SaveContent,
		SaveTime,
		SaveAuthor,
		SaveSource
	};
	struct RuleTreeData 
	{
		CString     m_strRuleTreeID;
		CString     m_strRuleID;
		CString     m_strParentID;
		CString     m_strRuleTreeName;
		CString     m_strMatchRule;
		int         m_nProcessType;
		int         m_nSaveType;
		
		RuleTreeData(CString strRuleTreeName,int nProcessType,int nSaveType)
		{
			m_strRuleTreeName=strRuleTreeName;
			m_nProcessType=nProcessType;
			m_nSaveType=nSaveType;
		}
	};
	CRuleTree();
	virtual ~CRuleTree();

	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);

	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
	
	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadInfo(CADORecordset &rs);	
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);

	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
	
	void SetValue(RuleTreeData& data);
	BOOL IsValid();
public:
	RuleTreeData   m_data;
};