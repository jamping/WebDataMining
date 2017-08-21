// NewsPicture.h: interface for the CNewsPicture class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"

class CNewsPicture : public CBaseObject  
{
public:
	struct NewsPictureData 
	{
		CString  m_strPictureID;
		CString  m_strTopicID;
		CString  m_strURL;
		CString  m_strOldURL;
		CString  m_strExplain;
		int      m_nStartPos;
		int      m_nEndPos;
		int      m_nPictureSize;
		CString  m_strServerAutoID;
		BOOL     m_bThumb;

		NewsPictureData()
		{
			m_nStartPos=0;
			m_nEndPos=0;
			m_nPictureSize=0;
			m_strServerAutoID=_T("");
			m_bThumb=FALSE;
		}
	};

	CNewsPicture();
	virtual ~CNewsPicture();

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
public:
	NewsPictureData m_data;

};