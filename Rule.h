// Rule.h: interface for the CRule class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
#include "RuleTree.h"

class CRule : public CBaseObject  
{
public:
	struct RuleData 
	{
		CString  m_strRuleID;
		CString  m_strRuleName;
		int      m_nRuleEncoding;
		RuleData()
		{
			m_nRuleEncoding=0;
		}
	};
	CRule();
	virtual ~CRule();

	void NewSubRules(CString strRuleTreeName,int nProcessType,int nSaveType);

	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
	virtual void ShowInTreeView(CCustomTree *pTree,BOOL bChild=FALSE);

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
public:
	RuleData  m_data;
};