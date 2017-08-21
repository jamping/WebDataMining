// TopicPublishDlg.h : header file
//
#pragma once

#include "contabctrl.h"
#include "Column.h"
#include "AutoCompl.h"
#include "CheckComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CTopicPublishDlg dialog
class CTopicListDlg;
class CTopicPublishDlg : public CDialogEx
{
// Construction
public:
	CTopicPublishDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTopicPublishDlg)
	enum { IDD = IDD_TOPIC_PUBLISH_DLG };
	CCheckComboBox	m_ctlServerColumn;
	CCheckComboBox	m_ctlColumn;
	CConTabCtrl	m_tab;
	BOOL	m_bIsPrimary;
	CString	m_strKeyWord;
	CString	m_strSubject;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopicPublishDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strSelIDs;
	CAutoComplete   autoKeyWord,autoSubject;

	void InitColumn(CBaseObject* pRoot,int nLevel);
	void InitServerColumn(CBaseObject* pRoot,int nLevel);
	CTopicListDlg* GetCurPage();    

	// Generated message map functions
	//{{AFX_MSG(CTopicPublishDlg)
	afx_msg void OnSelectedAll();
	afx_msg void OnPublishTopic();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnFresh();
	afx_msg void OnExport();
	//}}AFX_MSG
	afx_msg void OnUpdateSelAll(CCmdUI* pCmdUI);
    afx_msg void OnUpdatePublish(CCmdUI* pCmdUI);
    afx_msg void OnUpdateDel(CCmdUI* pCmdUI);
    afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
    afx_msg LRESULT OnComboTreeCheck(WPARAM w,LPARAM l);

	DECLARE_MESSAGE_MAP()
};