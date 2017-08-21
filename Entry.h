// Entry.h: interface for the CEntry class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
#include "Rule.h"

class CEntry : public CBaseObject  
{
public:
	struct EntryData 
	{
		CString        m_strEntryID;         //��ĿID
		CString        m_strColumnID;        //��ĿID
		CString        m_strServerColumnID;  //��������ĿID
//		CString        m_strServerChannelID; //������Ƶ��ID
		CString        m_strRuleID;          //�������ID
		CString        m_strEntryName;       //��Ŀ��
		CString        m_strWebSiteName;     //��վ��
		CString        m_strURL;             //���ص�ַ
		CString        m_strMatchRule;       //ƥ�����    
		BOOL           m_bAutoPublish;       //�Զ�������־
		int            m_nTryTimes;          //���Դ���
		int            m_nTopicMax;          //���ط�ֵ
		COleDateTime   m_lastDownloadTime;   //�������ʱ��
		EntryData()
		{
			m_nTopicMax=50;
			m_nTryTimes=3;
			m_bAutoPublish=FALSE;
		}
	};
	CEntry();
	virtual ~CEntry();

	//Download topic
	virtual void DownloadObject(BOOL bAuto =FALSE );
	virtual void DelSubObject(BOOL bDelFromDB =TRUE );
	//GetRule
	CRule* GetRule();
    BOOL IsValid();
	//Get Channel name
	CString GetChannelName();

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
		
	void StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual TiXmlNode* Export(CString& strDir,TiXmlNode* pNode,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
public:
    EntryData m_data;
};