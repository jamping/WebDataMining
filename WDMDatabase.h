// WDMDatabase.h: interface for the CWDMDatabase class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
#include "WDMTable.h"
#include "optionsData.h"

class CWDMDatabase : public CBaseObject  
{
public:
	CWDMDatabase();
	virtual ~CWDMDatabase();

	struct WDMDatabaseData
	{
		CString  m_strDatabaseID;
		CString  m_strServerID;
		CString  m_strDatabaseName;
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

	BOOL GetDBServerData(COptionsData::OptionsData::DBServerData& data);
	BOOL GetAdoDatabase(CADODatabase& adoServer);

	CWDMTable* GetWDMTable(CWDMTable::TableType type);
	int GetWDMTables(CTypedPtrArray<CPtrArray,CWDMTable*>& tables,CWDMTable::TableType type,BOOL bAsc = TRUE);

	BOOL IsValid();
	BOOL IsConverted();
public:
	WDMDatabaseData m_data;
};