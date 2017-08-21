// WDMOperatorUnit.h: interface for the CWDMOperatorUnit class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseObject.h"

class CWDMOperatorUnit : public CBaseObject  
{
public:
	CWDMOperatorUnit();
	virtual ~CWDMOperatorUnit();

	struct WDMOperatorUnitData
	{
		CString  m_strUnitID;
		CString  m_strOperatorID;
		CString  m_strUnitName;
		int      m_nUnitLength;
		CString  m_strUnitDefault;
		CString  m_strUnitType;
		CString  m_strUnitIsNull;
		CString  m_strUnitKey;
		BOOL     m_bIsKey;
		BOOL     m_bCache;
		CString  m_strUnitCombos;
		int      m_nProcessOrder;

		WDMOperatorUnitData()
		{
			m_bIsKey=FALSE;
			m_bCache=FALSE;
//			m_dwTopicUnit=0ULL;
			m_nUnitLength=0;
			m_strUnitType=_T("");
			m_strUnitCombos=_T("");
			m_nProcessOrder=0;
		}
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

	BOOL ContainTopicUnit(CTopic::TopicUnit nUnit);
	int GetUnitNum();
	CString GetKeyString();
public:
	WDMOperatorUnitData m_data;
};