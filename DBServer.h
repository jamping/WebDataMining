// DBServer.h: interface for the CDBServer class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
#include "FileSend.h"

class CDBServer : public CBaseObject  
{
public:
	CDBServer();
	virtual ~CDBServer();

	struct DBServerData
	{
		CString  m_strServerID;
		CString  m_strServerName;
		CString  m_strUserName;
		CString  m_strUserPasswd;
		CString  m_strResourceDir;
		CString  m_strServerDesc;
		int      m_nDatabaseType;
		CString  m_strResourcePath;
		int      m_nServerPort;
		CString  m_strServerCode;
		int      m_nFileSendType;
		CString  m_strFileServerName;
		CString  m_strFileServerUserName;
		CString  m_strFileServerPassWord;
		int      m_nFileServerPort;
		BOOL     m_bThumb;
		CString  m_strThumbName;

		DBServerData()
		{
			m_nDatabaseType = 0;
			m_strResourceDir = _T("Z:\\wdm");
			m_strResourcePath = _T("http://21.114.11.9/files");
			m_nServerPort = 0;
			m_strServerCode = _T("");
			m_bThumb = FALSE;
			m_strThumbName = _T("thumb_");
		}
	};

	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
	virtual void ShowInTreeView(CCustomTree *pTree,BOOL bChild=FALSE);
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);

	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadInfo(CADORecordset &rs);	
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);
		
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
	//文件服务器对象使用后需手动清除内存
	CFileSend* GetFileSend();
public:
	DBServerData m_data;

};