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
		CString        m_strEntryID;         //条目ID
		CString        m_strColumnID;        //栏目ID
		CString        m_strServerColumnID;  //服务器栏目ID
//		CString        m_strServerChannelID; //服务器频道ID
		CString        m_strRuleID;          //分离规则ID
		CString        m_strEntryName;       //条目名
		CString        m_strWebSiteName;     //网站名
		CString        m_strURL;             //下载地址
		CString        m_strMatchRule;       //匹配规则    
		BOOL           m_bAutoPublish;       //自动发布标志
		int            m_nTryTimes;          //重试次数
		int            m_nTopicMax;          //下载阀值
		COleDateTime   m_lastDownloadTime;   //最后下载时间
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