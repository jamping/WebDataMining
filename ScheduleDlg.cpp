// ScheduleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ScheduleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScheduleDlg dialog


CScheduleDlg::CScheduleDlg(BOOL bReadOnly,CWnd* pParent /*=NULL*/)
	: CSetReadOnlyDialog(bReadOnly,CScheduleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScheduleDlg)
	m_strScheduleName = _T("");
	m_nEndDate = 1;
	m_timeActiveEndDate = COleDateTime::GetCurrentTime();
	m_timeActiveStartDate = COleDateTime::GetCurrentTime();
	m_timeActiveEndTime = COleDateTime(2099,12,31,23,59,59);
	m_timeActiveStartTime = COleDateTime(0,0);
	m_bEnabled = TRUE;
	m_nFreqSubdayType = 0;
	m_nTimeType = 1;
	m_timeOnce = COleDateTime(0,0);
	m_nNum = 1;
	m_nDays = 0;
	//}}AFX_DATA_INIT
}

void CScheduleDlg::Init()
{
	m_nDays=m_data.m_nFreq_interval;

	switch(m_data.m_nFreq_subday_type)
	{
	case CSchedule::subday_once:
		m_nFreqSubdayType=0;
		m_timeOnce=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_time,FALSE);
		break;
	case CSchedule::subday_minute:
		m_nFreqSubdayType=1;
		m_nNum=m_data.m_nFreq_subday_interval;
		m_nTimeType=0;
		m_timeActiveStartTime=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_time,FALSE);
		m_timeActiveEndTime=CCommonFunction::Int2OleDateTime(m_data.m_nActive_end_time,FALSE);
		break;
	case CSchedule::subday_hour:
		m_nFreqSubdayType=1;
		m_nNum=m_data.m_nFreq_subday_interval;
		m_nTimeType=1;
		m_timeActiveStartTime=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_time,FALSE);
		m_timeActiveEndTime=CCommonFunction::Int2OleDateTime(m_data.m_nActive_end_time,FALSE);
		break;
	}
	m_timeActiveStartDate=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_date);
	m_timeActiveEndDate=CCommonFunction::Int2OleDateTime(m_data.m_nActive_end_date);
	m_nEndDate=(m_timeActiveEndDate.GetYear()==9999)?1:0;
    
	UpdateData(FALSE);
}

void CScheduleDlg::SetValue(CSchedule::ScheduleData& data)
{
	m_bEnabled=data.m_nEnabled;
	m_strScheduleName=data.m_strScheduleName;
	m_data.m_strObjectID=data.m_strObjectID;
	m_data.m_nObjectType=data.m_nObjectType;
	m_data.m_nFreq_type=data.m_nFreqType;
	m_data.m_nFreq_interval=data.m_nFreqInterval;
	m_data.m_nFreq_subday_type=data.m_nFreqSubdayType;
	m_data.m_nFreq_subday_interval=data.m_nFreqSubdayInterval;
	m_data.m_nFreq_relative_interval=data.m_nFreqRelativeInterval;
	m_data.m_nFreq_recurrence_factor=data.m_nFreqRecurrenceFactor;
	m_data.m_nActive_start_date=data.m_nActiveStartDate;
	m_data.m_nActive_end_date=data.m_nActiveEndDate;
	m_data.m_nActive_start_time=data.m_nActiveStartTime;
	m_data.m_nActive_end_time=data.m_nActiveEndTime;
	m_data.m_nNext_run_date=data.m_nNextRunDate;
	m_data.m_nNext_run_time=data.m_nNextRunTime;
	m_data.m_timeCreated_date=data.m_timeCreated;
	//Init the time control
	m_timeOnce=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_time,FALSE);
	m_timeActiveStartTime=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_time,FALSE);
    m_timeActiveEndTime=CCommonFunction::Int2OleDateTime(m_data.m_nActive_end_time,FALSE);
	m_timeActiveStartDate=CCommonFunction::Int2OleDateTime(m_data.m_nActive_start_date);
	m_timeActiveEndDate=CCommonFunction::Int2OleDateTime(m_data.m_nActive_end_date);
}
BOOL CScheduleDlg::GetValue(CSchedule::ScheduleData& data)
{
	BOOL bModify=FALSE;
	if(data.m_nEnabled!=m_bEnabled)
	{
		data.m_nEnabled=m_bEnabled;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strScheduleName,m_strScheduleName))
	{
		data.m_strScheduleName=m_strScheduleName;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strObjectID,m_data.m_strObjectID))
	{
		data.m_strObjectID=m_data.m_strObjectID;
		bModify |= TRUE;
	}
	if(data.m_nObjectType!=m_data.m_nObjectType)
	{
		data.m_nObjectType=m_data.m_nObjectType;
		bModify |= TRUE;
	}
	if(data.m_nFreqType!=m_data.m_nFreq_type)
	{
		data.m_nFreqType=m_data.m_nFreq_type;
		bModify |= TRUE;
	}
	if(data.m_nFreqInterval!=m_data.m_nFreq_interval)
	{
		data.m_nFreqInterval=m_data.m_nFreq_interval;
		bModify |= TRUE;
	}
	if(data.m_nFreqSubdayType!=m_data.m_nFreq_subday_type)
	{
		data.m_nFreqSubdayType=m_data.m_nFreq_subday_type;
		bModify |= TRUE;
	}
	if(data.m_nFreqSubdayInterval!=m_data.m_nFreq_subday_interval)
	{
		data.m_nFreqSubdayInterval=m_data.m_nFreq_subday_interval;
		bModify |= TRUE;
	}
	if(data.m_nFreqRelativeInterval!=m_data.m_nFreq_relative_interval)
	{
		data.m_nFreqRelativeInterval=m_data.m_nFreq_relative_interval;
		bModify |= TRUE;
	}
	if(data.m_nFreqRecurrenceFactor!=m_data.m_nFreq_recurrence_factor)
	{
		data.m_nFreqRecurrenceFactor=m_data.m_nFreq_recurrence_factor;
		bModify |= TRUE;
	}
	if(data.m_nActiveStartDate!=m_data.m_nActive_start_date)
	{
		data.m_nActiveStartDate=m_data.m_nActive_start_date;
		bModify |= TRUE;
	}	
	if(data.m_nActiveEndDate!=m_data.m_nActive_end_date)
	{
		data.m_nActiveEndDate=m_data.m_nActive_end_date;
		bModify |= TRUE;
	}
	if(data.m_nActiveStartTime!=m_data.m_nActive_start_time)
	{
		data.m_nActiveStartTime=m_data.m_nActive_start_time;
		bModify |= TRUE;
	}	
	if(data.m_nActiveEndTime!=m_data.m_nActive_end_time)
	{
		data.m_nActiveEndTime=m_data.m_nActive_end_time;
		bModify |= TRUE;
	}	
	if(data.m_nNextRunDate!=m_data.m_nNext_run_date)
	{
		data.m_nNextRunDate=m_data.m_nNext_run_date;
		bModify |= TRUE;
	}
	
	if(data.m_nNextRunTime!=m_data.m_nNext_run_time)
	{
		data.m_nNextRunTime=m_data.m_nNext_run_time;
		bModify |= TRUE;
	}
	
	if(data.m_timeCreated!=m_data.m_timeCreated_date)
	{
		data.m_timeCreated=m_data.m_timeCreated_date;
		bModify |= TRUE;
	}
	return bModify;
}
void CScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CSetReadOnlyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScheduleDlg)
	DDX_Control(pDX, IDC_SPIN_DAY, m_spinDays);
	DDX_Control(pDX, IDC_SPIN_NUM, m_spinNum);
	DDX_Text(pDX, IDC_SCHEDULE_NAME, m_strScheduleName);
	DDX_Radio(pDX, IDC_RADIO_END, m_nEndDate);
	DDX_DateTimeCtrl(pDX, IDC_ACTIVE_END_DATE, m_timeActiveEndDate);
	DDX_DateTimeCtrl(pDX, IDC_ACTIVE_START_DATE, m_timeActiveStartDate);
	DDX_Text(pDX, IDC_ACTIVE_END_TIME, m_timeActiveEndTime);
	DDX_Text(pDX, IDC_ACTIVE_START_TIME, m_timeActiveStartTime);
	DDX_Check(pDX, IDC_ENABLED, m_bEnabled);
	DDX_Radio(pDX, IDC_FREQ_SUBDAY_TYPE, m_nFreqSubdayType);
	DDX_CBIndex(pDX, IDC_TIME_TYPE, m_nTimeType);
	DDX_Text(pDX, IDC_ONCE_TIME, m_timeOnce);
	DDX_Text(pDX, IDC_NUM, m_nNum);
	DDX_Text(pDX, IDC_DAY, m_nDays);
	//}}AFX_DATA_MAP
	DDX_OleDateTime(pDX,IDC_ACTIVE_END_TIME,m_dateEndTimeCtrl,m_timeActiveEndTime);
	DDX_OleDateTime(pDX,IDC_ACTIVE_START_TIME,m_dateStartTimeCtrl,m_timeActiveStartTime);
	DDX_OleDateTime(pDX,IDC_ONCE_TIME,m_dateOnceTimeCtrl,m_timeOnce);
}


BEGIN_MESSAGE_MAP(CScheduleDlg, CSetReadOnlyDialog)
	//{{AFX_MSG_MAP(CScheduleDlg)
	ON_BN_CLICKED(IDC_FREQ_SUBDAY_TYPE, OnOnce)
	ON_BN_CLICKED(IDC_FREQ_SUBDAY_TYPE2, OnOnce2)
	ON_BN_CLICKED(IDC_RADIO_END, OnRadioEnd)
	ON_BN_CLICKED(IDC_RADIO_END2, OnRadioEnd2)
	ON_CBN_SELCHANGE(IDC_TIME_TYPE, OnSelchangeTimeType)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_ONCE_TIME,OnUpdateOnceTime)
	ON_UPDATE_COMMAND_UI(IDC_NUM,OnUpdateNum)
	ON_UPDATE_COMMAND_UI(IDC_TIME_TYPE,OnUpdateNum)
	ON_UPDATE_COMMAND_UI(IDC_ACTIVE_START_TIME,OnUpdateNum)
	ON_UPDATE_COMMAND_UI(IDC_ACTIVE_END_TIME,OnUpdateNum)
	ON_UPDATE_COMMAND_UI(IDC_ACTIVE_END_DATE,OnUpdateEndDate)
//	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScheduleDlg message handlers

BOOL CScheduleDlg::OnInitDialog() 
{
	CSetReadOnlyDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //Set spin ctrl  
	m_spinDays.SetRange(1,366); 
	m_spinNum.SetRange(1,m_nTimeType==0?60:24);
	
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CScheduleDlg::ShowFreqsWindow(int nFreqs)
{
// 	switch(nFreqs)
// 	{
// 	case 0:
// 		m_everyDayDlg.ShowWindow(SW_NORMAL);
// 		m_everyWeekDlg.ShowWindow(SW_HIDE);
// 		m_everyMonthDlg.ShowWindow(SW_HIDE);
// 		break;
// 	case 1:
// 		m_everyDayDlg.ShowWindow(SW_HIDE);
// 		m_everyWeekDlg.ShowWindow(SW_NORMAL);
// 		m_everyMonthDlg.ShowWindow(SW_HIDE);
// 		break;
// 	case 2:
// 		m_everyDayDlg.ShowWindow(SW_HIDE);
// 		m_everyWeekDlg.ShowWindow(SW_HIDE);
// 		m_everyMonthDlg.ShowWindow(SW_NORMAL);
// 		break;
// 	}
}

// void CScheduleDlg::OnEveryDay() 
// {
// 	// TODO: Add your control notification handler code here
// 	UpdateData();
// 	ShowFreqsWindow(m_nFreqType);
// }
// 
// void CScheduleDlg::OnEveryDay2() 
// {
// 	// TODO: Add your control notification handler code here
// 	UpdateData();
//     ShowFreqsWindow(m_nFreqType);
// }
// 
// void CScheduleDlg::OnEveryDay3() 
// {
// 	// TODO: Add your control notification handler code here
// 	UpdateData();
//     ShowFreqsWindow(m_nFreqType);
//}

void CScheduleDlg::OnOnce() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CScheduleDlg::OnOnce2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

BOOL CScheduleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CSetReadOnlyDialog::PreTranslateMessage(pMsg);
}
void CScheduleDlg::OnUpdateOnceTime(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nFreqSubdayType==0);
}
void CScheduleDlg::OnUpdateNum(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nFreqSubdayType==1);
}

void CScheduleDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

    //freq_type params
	m_data.m_nFreq_type=CSchedule::every_day;
	m_data.m_nFreq_interval=m_nDays;
	//freq_subday_type params
	switch(m_nFreqSubdayType)
	{
	case 0:
		m_data.m_nFreq_subday_type=CSchedule::subday_once;
		m_data.m_nActive_start_time=CCommonFunction::OleDateTime2Int(m_timeOnce,FALSE);
		break;
	case 1:
		m_data.m_nFreq_subday_type=(m_nTimeType==0)?CSchedule::subday_minute:CSchedule::subday_hour;
		m_data.m_nFreq_subday_interval=m_nNum;
		m_data.m_nActive_start_time=CCommonFunction::OleDateTime2Int(m_timeActiveStartTime,FALSE);
		m_data.m_nActive_end_time=CCommonFunction::OleDateTime2Int(m_timeActiveEndTime,FALSE);
		break;
	}
	//start and end date
	m_data.m_nActive_start_date=CCommonFunction::OleDateTime2Int(m_timeActiveStartDate);
	m_data.m_nActive_end_date=(m_nEndDate==0)?CCommonFunction::OleDateTime2Int(m_timeActiveEndDate):99991231;
		
	CSetReadOnlyDialog::OnOK();
}
void CScheduleDlg::OnUpdateEndDate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nEndDate==0);
}

void CScheduleDlg::OnRadioEnd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CScheduleDlg::OnRadioEnd2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CScheduleDlg::OnSelchangeTimeType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_spinNum.SetRange(1,m_nTimeType==0?60:24);
}
