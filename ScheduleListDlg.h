// ScheduleListDlg.h : header file
//
#pragma once

#include "schedule.h"
/////////////////////////////////////////////////////////////////////////////
// CScheduleListDlg dialog

class CScheduleListDlg : public CDialogEx
{
// Construction
public:
	CScheduleListDlg(CBaseObject* pOb,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScheduleListDlg)
	enum { IDD = IDD_SCHEDULE_LIST_DLG };
	CListCtrl	m_listSchedule;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduleListDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CBaseObject*    m_pSelOb;

	CString GetScheduleDesc(const CSchedule::ScheduleData& data);
	// Generated message map functions
	//{{AFX_MSG(CScheduleListDlg)
	afx_msg void OnNew();
	afx_msg void OnEdit();
	afx_msg void OnDel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnUpdateNew(CCmdUI * pCmdUI);
	afx_msg void OnUpdateEdit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDel(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
};