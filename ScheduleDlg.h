// ScheduleDlg.h : header file
//
#pragma once

// #include "EveryDayDlg.h"
// #include "EveryWeekDlg.h"
// #include "EveryMonthDlg.h"
#include "CJMaskEdit.h"
#include "Schedule.h"
/////////////////////////////////////////////////////////////////////////////
// CScheduleDlg dialog
#include "SetReadOnlyDialog.h"

class CScheduleDlg : public CSetReadOnlyDialog
{
// Construction
public:
	CScheduleDlg(BOOL bReadOnly=FALSE,CWnd* pParent = NULL);   // standard constructor

    void SetValue(CSchedule::ScheduleData& data);
	BOOL GetValue(CSchedule::ScheduleData& data);

public:

//  CString GetScheduleDesc();
// 	void SetScheduleDesc(CString strScheduleDesc);
// Dialog Data
	//{{AFX_DATA(CScheduleDlg)
	enum { IDD = IDD_SCHEDULE_DLG };
	CSpinButtonCtrl	m_spinDays;
	CSpinButtonCtrl	m_spinNum;
	CString	m_strScheduleName;
	int		m_nEndDate;
	COleDateTime	m_timeActiveEndDate;
	COleDateTime	m_timeActiveStartDate;
	COleDateTime	m_timeActiveEndTime;
	COleDateTime	m_timeActiveStartTime;
	BOOL	m_bEnabled;
	int		m_nFreqSubdayType;
	int		m_nTimeType;
	COleDateTime	m_timeOnce;
	int		m_nNum;
	int		m_nDays;
	//}}AFX_DATA
    CCJTimeEdit    m_dateOnceTimeCtrl;
	CCJTimeEdit    m_dateEndTimeCtrl;
	CCJTimeEdit    m_dateStartTimeCtrl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	public:

		struct ScheduleDlgData
		{
			CString			m_strObjectID;      
			int             m_nObjectType;
			int				m_nFreq_type;                
			int				m_nFreq_interval;            
			int				m_nFreq_subday_type;         
			int				m_nFreq_subday_interval;     
			int				m_nFreq_relative_interval;   
			int				m_nFreq_recurrence_factor;   
			int				m_nActive_start_date;        
			int				m_nActive_end_date;          
			int				m_nActive_start_time;        
			int				m_nActive_end_time ;         
			int		    	m_nNext_run_date ;           
			int		    	m_nNext_run_time ;           
			COleDateTime	m_timeCreated_date ;  
			ScheduleDlgData()
			{
				m_strObjectID=_T(""); 
				m_nObjectType=OBJECT_TYPE_UNKNOWN;
				m_nFreq_type=CSchedule::every_day;               
				m_nFreq_interval=1;           
				m_nFreq_subday_type=CSchedule::subday_hour;        
				m_nFreq_subday_interval=2;    
				m_nFreq_relative_interval=1;  
				m_nFreq_recurrence_factor=1;  
				m_nActive_start_date=0;       
				m_nActive_end_date=99991231;         
				m_nActive_start_time=0;       
				m_nActive_end_time=235959 ;        
				m_nNext_run_date=0 ;          
				m_nNext_run_time =0;          
				m_timeCreated_date=COleDateTime::GetCurrentTime() ;  				
			}
		};

private:
		void Init();

		void ShowFreqsWindow(int nFreqs);

        ScheduleDlgData m_data;
protected:
	// Generated message map functions
	//{{AFX_MSG(CScheduleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOnce();
	afx_msg void OnOnce2();
	virtual void OnOK();
	afx_msg void OnRadioEnd();
	afx_msg void OnRadioEnd2();
	afx_msg void OnSelchangeTimeType();
	//}}AFX_MSG
	afx_msg void OnUpdateOnceTime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNum(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEndDate(CCmdUI* pCmdUI);
//	afx_msg void OnKickIdle();
	DECLARE_MESSAGE_MAP()
};