// Schedule.h: interface for the CSchedule class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"

class CSchedule : public CBaseObject  
{
public:
	struct ScheduleData
	{
		CString        m_strScheduleID;
		CString        m_strObjectID;
		int            m_nObjectType;
		CString        m_strScheduleName;
		int            m_nEnabled;
		int            m_nFreqType;
		int            m_nFreqInterval;  
		int            m_nFreqSubdayType;
		int            m_nFreqSubdayInterval;
		int            m_nFreqRelativeInterval;
		int            m_nFreqRecurrenceFactor;
		int            m_nActiveStartDate;
		int            m_nActiveEndDate;
		int            m_nActiveStartTime;
		int            m_nActiveEndTime;
		int            m_nNextRunDate;
		int            m_nNextRunTime;
		COleDateTime   m_timeCreated;
		ScheduleData()
		{
			m_nEnabled=TRUE;
			m_nFreqType=every_day;
			m_nFreqInterval=1;
			m_nFreqSubdayType=subday_hour;
			m_nFreqSubdayInterval=2;
			m_nFreqRelativeInterval=0;
			m_nFreqRecurrenceFactor=1;
			m_nActiveStartDate=0;
			m_nActiveEndDate=99991231;
			m_nActiveStartTime=0;
			m_nActiveEndTime=235959;
			m_nNextRunDate=0;
			m_nNextRunTime=0;
			m_timeCreated=COleDateTime::GetCurrentTime();
		}
	};

	enum freq_type
	{
		every_day=0x04,
		every_week=0x08,
		every_month1=0x10,
		every_month2=0x20
	};
	enum freq_subday_type
	{
		subday_once=0x01,
		subday_minute=0x04,
		subday_hour=0x08
	};
	enum week_day
	{
		sunday=0x01,
		monday=0x02,
		tuesday=0x04,
		wednsday=0x08,
		thursday=0x10,
		friday=0x20,
		saturday=0x40
	};
	
	CSchedule();
	virtual ~CSchedule();
    
	BOOL& IsTimeUp() { return m_bTimeUp; }
	COleDateTime GetNextRunTime();

    BOOL IsValid();
    CBaseObject* GetScheduledObject();
	
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
	ScheduleData m_data;
private:
	BOOL SetNextRunDateTime();

	BOOL         m_bTimeUp;
};