// WDMTable.h: interface for the CWDMTable class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
#include "WDMOperator.h"

class CWDMTable : public CBaseObject  
{
public:
	CWDMTable();
	virtual ~CWDMTable();    
    
	enum TableType
	{
		Table_Pre=0,
		Table_Topic,
		Table_Picture,
		Table_Post,
		Table_LinkUrl
	};
	struct WDMTableData
	{
		CString  m_strTableID;
		CString  m_strDatabaseID;
		CString  m_strTableName;
		int      m_nTableType;
		int      m_nProcessOrder;
		BOOL     m_bPublishRepeat;

		WDMTableData()
		{
			m_nTableType = 0;
			m_nProcessOrder = 1;
			m_bPublishRepeat = TRUE;
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
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream

	CWDMOperator* GetOperator(CWDMOperator::OperatorType type);
public:
	WDMTableData m_data;
};