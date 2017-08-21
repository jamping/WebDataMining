// WDMOperator.h: interface for the CWDMOperator class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
class CWDMOperatorUnit;

class CWDMOperator : public CBaseObject  
{
public:
	CWDMOperator();
	virtual ~CWDMOperator();

	enum OperatorType { opSave,opDelete };

	struct WDMOperatorData
	{
		CString  m_strOperatorID;
		CString  m_strTableID;
		CString  m_strOperatorName;
	};

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

	BOOL IsValid();
	BOOL IsConverted();
	int GetWDMOperatorUnits(CTypedPtrArray<CPtrArray,CWDMOperatorUnit*>& units);
public:
	WDMOperatorData m_data;
};