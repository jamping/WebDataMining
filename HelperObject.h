// HelperObject.h: interface for the CHelperObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseObject.h"
/************************************************************************
 辅助对象
************************************************************************/

enum HELPER_TYPE
{
	HELPER_SOURCE=0,         //采集源
	HELPER_RULE,             //采集规则
	HELPER_SCHEDULE,         //调度计划
	HELPER_DBSERVER          //数据库服务器
};


class CHelperObject : public CBaseObject
{
protected:
	int  m_nClass;
public:
	CHelperObject(int nClass);
	virtual ~CHelperObject();

public:
	int GetHelpType();
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	UINT PromptMenuID();
	int PromptMenuPos();		
	void UpdateMenu(CMenu *pMenu);

	virtual TiXmlNode* Export(CString& strDir,TiXmlNode* pNode,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
};




















