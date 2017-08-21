// Schedule.cpp: implementation of the CSchedule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "Schedule.h"
#include "ScheduleDlg.h"
#include "Entry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSchedule::CSchedule():CBaseObject(OBJECT_TYPE_SCHEDULE),m_bTimeUp(TRUE)
{
    SetImg(18,19);
}

CSchedule::~CSchedule()
{

}

void CSchedule::SetID(LPCSTR strID,LPCSTR strDesc/*=NUL*/)
{
	m_data.m_strScheduleID=strID;
	CBaseObject::SetID(strID,strDesc);
}
BOOL CSchedule::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	return FALSE;
}
void CSchedule::DoJoin(CBaseObject *pParent)
{

}		
void CSchedule::DoLeave(CBaseObject *pParent)
{

}
	
UINT CSchedule::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;

	CScheduleDlg dlg(bReadOnly);
	dlg.SetValue(m_data);

	nRet=dlg.DoModal();
	if(nRet==IDOK)
	{
		if(dlg.GetValue(m_data))
			SetModify();

		m_strDesc=m_data.m_strScheduleName;
		m_bTimeUp=TRUE;
	}
	return nRet;
}

BOOL CSchedule::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_schedule where schedule_id='%s'",m_data.m_strScheduleID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("schedule_id",m_data.m_strScheduleID);
		}
		else
			rs.Edit();

		bRet&=rs.SetFieldValue("object_id",m_data.m_strObjectID); 
		bRet&=rs.SetFieldValue("object_type",m_data.m_nObjectType);
		bRet&=rs.SetFieldValue("schedule_name",m_data.m_strScheduleName);  
		bRet&=rs.SetFieldValue("enabled",m_data.m_nEnabled);  
		bRet&=rs.SetFieldValue("freq_type",m_data.m_nFreqType);  
		bRet&=rs.SetFieldValue("freq_interval",m_data.m_nFreqInterval);  
		bRet&=rs.SetFieldValue("freq_subday_type",m_data.m_nFreqSubdayType);
		bRet&=rs.SetFieldValue("freq_subday_interval",m_data.m_nFreqSubdayInterval);
		bRet&=rs.SetFieldValue("freq_relative_interval",m_data.m_nFreqRelativeInterval);  
		bRet&=rs.SetFieldValue("freq_recurrence_factor",m_data.m_nFreqRecurrenceFactor);  
		bRet&=rs.SetFieldValue("active_start_date",(long)m_data.m_nActiveStartDate);  
		bRet&=rs.SetFieldValue("active_end_date",(long)m_data.m_nActiveEndDate);  
		bRet&=rs.SetFieldValue("active_start_time",(long)m_data.m_nActiveStartTime);  
		bRet&=rs.SetFieldValue("active_end_time",(long)m_data.m_nActiveEndTime); 
		bRet&=rs.SetFieldValue("next_run_date",(long)m_data.m_nNextRunDate);  
		bRet&=rs.SetFieldValue("next_run_time",(long)m_data.m_nNextRunTime);  
		bRet&=rs.SetFieldValue("created_date",m_data.m_timeCreated);  
		
		bRet&=rs.Update();

		rs.Close();
	}
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}
BOOL CSchedule::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	return TRUE;
}	
BOOL CSchedule::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet &=rs.GetFieldValue("schedule_id",m_data.m_strScheduleID);
	bRet &=rs.GetFieldValue("object_id",m_data.m_strObjectID);
	bRet &=rs.GetFieldValue("object_type",m_data.m_nObjectType);
	bRet &=rs.GetFieldValue("schedule_name",m_data.m_strScheduleName);
	bRet &=rs.GetFieldValue("enabled",m_data.m_nEnabled);
    bRet &=rs.GetFieldValue("freq_type",m_data.m_nFreqType);
	bRet &=rs.GetFieldValue("freq_interval",m_data.m_nFreqInterval);
	bRet &=rs.GetFieldValue("freq_subday_type",m_data.m_nFreqSubdayType);
	bRet &=rs.GetFieldValue("freq_subday_interval",m_data.m_nFreqSubdayInterval);
	bRet &=rs.GetFieldValue("freq_relative_interval",m_data.m_nFreqRelativeInterval);
	bRet &=rs.GetFieldValue("freq_recurrence_factor",m_data.m_nFreqRecurrenceFactor);
	bRet &=rs.GetFieldValue("active_start_date",(long&)m_data.m_nActiveStartDate);
	bRet &=rs.GetFieldValue("active_end_date",(long&)m_data.m_nActiveEndDate);
	bRet &=rs.GetFieldValue("active_start_time",(long&)m_data.m_nActiveStartTime);
	bRet &=rs.GetFieldValue("active_end_time",(long&)m_data.m_nActiveEndTime);
	bRet &=rs.GetFieldValue("next_run_date",(long&)m_data.m_nNextRunDate);
	bRet &=rs.GetFieldValue("next_run_time",(long&)m_data.m_nNextRunTime);
	bRet &=rs.GetFieldValue("created_date",m_data.m_timeCreated);

	m_strID=m_data.m_strScheduleID;
	m_strDesc=m_data.m_strScheduleName;

	return bRet;
}	
BOOL CSchedule::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_schedule where schedule_id='%s'",m_data.m_strScheduleID);
		bRet=db.Execute(strSql);
	}

	return bRet;

}	

UINT CSchedule::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CSchedule::PromptMenuPos()
{
	return 6;
}
void CSchedule::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除调度");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}
COleDateTime CSchedule::GetNextRunTime()
{
	COleDateTime date,time;
	if(m_bTimeUp==TRUE)
	{
		 //重新设定调度时间
		if(SetNextRunDateTime())
			m_bTimeUp=FALSE;
	}
	date=CCommonFunction::Int2OleDateTime(m_data.m_nNextRunDate);
	time=CCommonFunction::Int2OleDateTime(m_data.m_nNextRunTime,FALSE);

	return COleDateTime(date.GetYear(),date.GetMonth(),date.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());

}
BOOL CSchedule::SetNextRunDateTime()
{
	COleDateTime start,end,temp,time;
    COleDateTimeSpan span;

	COleDateTime curTime=COleDateTime::GetCurrentTime();
	temp.SetDate(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay());
    start=CCommonFunction::Int2OleDateTime(m_data.m_nActiveStartDate);
	end=CCommonFunction::Int2OleDateTime(m_data.m_nActiveEndDate);

    //if current time > end time ,the next time is invalid
    if( temp > end)
        return FALSE;

	int nDays=0;
	span=curTime-start;	
    // set date 
	if( span.m_span<0 )
	{
		time=start;
	}
	else
	{
		nDays=span.GetDays()%m_data.m_nFreqInterval;
		if( nDays==0 )
			time=temp;
		else
			time=temp+COleDateTimeSpan(m_data.m_nFreqInterval-nDays,0,0,0);
	}
	m_data.m_nNextRunDate=CCommonFunction::OleDateTime2Int(time);
	// set time
	start=CCommonFunction::Int2OleDateTime(m_data.m_nActiveStartTime,FALSE);
	end=CCommonFunction::Int2OleDateTime(m_data.m_nActiveEndTime,FALSE);
    // is tomorrow day ,or more...
    if(span.m_span<0 || nDays>0)
	{
		m_data.m_nNextRunTime=m_data.m_nActiveStartTime;
	}
	else
	{
		// is today
		switch( m_data.m_nFreqSubdayType)
		{
		case subday_once:
			{
				m_data.m_nNextRunTime=m_data.m_nActiveStartTime;

				temp=CCommonFunction::Int2OleDateTime(m_data.m_nActiveStartTime,FALSE);
				COleDateTimeSpan once(0,temp.GetHour(),temp.GetMinute(),temp.GetSecond());
				COleDateTimeSpan cur(0,curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());
                //time is expires, get next valid date
				if(once<cur)
				{
					m_data.m_nNextRunDate=CCommonFunction::OleDateTime2Int(CCommonFunction::Int2OleDateTime(m_data.m_nNextRunDate)
						+COleDateTimeSpan(m_data.m_nFreqInterval,0,0,0));
				}
			}
			break;
		case subday_hour:
			span=COleDateTimeSpan(0,m_data.m_nFreqSubdayInterval,0,0);
			break;
		case subday_minute:
			span=COleDateTimeSpan(0,0,m_data.m_nFreqSubdayInterval,0);
			break;
		default:
			ASSERT(FALSE);
		}
		if(m_data.m_nFreqSubdayType!=subday_once)
		{
			temp.SetTime(curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond());
			
			for(;start<temp;start+=span);
			//get next valid date
			if(start>end)
			{
                m_data.m_nNextRunDate=CCommonFunction::OleDateTime2Int(CCommonFunction::Int2OleDateTime(m_data.m_nNextRunDate)
						                                                   +COleDateTimeSpan(m_data.m_nFreqInterval,0,0,0));
				m_data.m_nNextRunTime=m_data.m_nActiveStartTime;
			}	
			else
			{
				time+=COleDateTimeSpan(0,start.GetHour(),start.GetMinute(),start.GetSecond());
				m_data.m_nNextRunTime=CCommonFunction::OleDateTime2Int(time,FALSE);
			}
		}	
	}	
    return TRUE;
}
BOOL CSchedule::IsValid()
{
	if(m_data.m_nEnabled==0)
		return FALSE;

	return GetScheduledObject()!=NULL;
}
CBaseObject* CSchedule::GetScheduledObject()
{
    return theApp.GetSourceTree()->m_objectHelper.FindSubObject((OBJECT_TYPE)m_data.m_nObjectType,m_data.m_strObjectID,TRUE);
}

void CSchedule::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);
	CString strText;
	CStringArray strList;

	if(bSelf)
	{
		strText=_T("调度名称,300;下次运行时间,150;关联对象,300");					  
		pListCtrl->SetHeadings(strText);
		
        strList.Add(GetDesc());
		strList.Add(GetNextRunTime().Format("%Y-%m-%d %H:%M:%S"));
		CBaseObject* pOb=GetScheduledObject();
        strList.Add(pOb!=NULL?pOb->GetDesc():_T(""));

		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);			 
	}
	else
	{
 		strText=_T("没有子对象,150");  
		pListCtrl->SetHeadings(strText);	
	}
}
TiXmlNode*  CSchedule::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_schedule"));

	pTemp->SetAttribute(_T("schedule_id"),m_data.m_strScheduleID);
	pTemp->SetAttribute(_T("object_id"),m_data.m_strObjectID);
	pTemp->SetAttribute(_T("object_type"),m_data.m_nObjectType);
	pTemp->SetAttribute(_T("schedule_name"),m_data.m_strScheduleName);
	pTemp->SetAttribute(_T("enabled"),m_data.m_nEnabled);
	pTemp->SetAttribute(_T("freq_type"),m_data.m_nFreqType);
	pTemp->SetAttribute(_T("freq_interval"),m_data.m_nFreqInterval);
	pTemp->SetAttribute(_T("freq_subday_type"),m_data.m_nFreqSubdayType);
	pTemp->SetAttribute(_T("freq_subday_interval"),m_data.m_nFreqSubdayInterval);
	pTemp->SetAttribute(_T("freq_relative_interval"),m_data.m_nFreqRelativeInterval);
	pTemp->SetAttribute(_T("freq_recurrence_factor"),m_data.m_nFreqRecurrenceFactor);
	pTemp->SetAttribute(_T("active_start_date"),m_data.m_nActiveStartDate);

	pTemp->SetAttribute(_T("active_end_date"),m_data.m_nActiveEndDate);
	pTemp->SetAttribute(_T("active_start_time"),m_data.m_nActiveStartTime);
	pTemp->SetAttribute(_T("active_end_time"),m_data.m_nActiveEndTime);
	pTemp->SetAttribute(_T("next_run_date"),m_data.m_nNextRunDate);
	pTemp->SetAttribute(_T("next_run_time"),m_data.m_nNextRunTime);
	pTemp->SetAttribute(_T("created_date"),m_data.m_timeCreated.Format("%Y-%m-%d %H:%M:%S"));

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}
