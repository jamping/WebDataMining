// ServerColumn.h: interface for the CServerColumn class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"

class CServerColumn : public CBaseObject  
{
public:
	struct ServerColumnData
	{
		CString  m_strID;
		CString  m_strParentID;
		CString  m_strColumnName;
	};

	CServerColumn();
	virtual ~CServerColumn();

	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
//	virtual void ShowInTreeView(CCustomTree *pTree,BOOL bChild=FALSE);	
//	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	
//	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);

	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadInfo(CADORecordset &rs);	
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	

//	virtual UINT PromptMenuID();
//	virtual int PromptMenuPos();
//	virtual void UpdateMenu(CMenu *pMenu);

//	void StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
//  virtual TiXmlNode* Export(CString& strDir,TiXmlNode* pNode,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
	public:
		ServerColumnData  m_data;
};