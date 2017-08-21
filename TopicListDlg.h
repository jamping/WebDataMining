// TopicListDlg.h : header file
//
#pragma once

#include "TopicListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTopicListDlg dialog

class CTopicListDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CTopicListDlg)

// Construction
public:
	enum TopicStatus {UNPUBLISHED,PUBLISHED };

	CTopicListDlg(CWnd* pParent = NULL);   // standard constructor
    ~CTopicListDlg() {}

	void SetParams(CStringList* pList,TopicStatus type=UNPUBLISHED);
	void ShowContent(CStringList* pList,TopicStatus type=UNPUBLISHED);
	BOOL IsSelAll();
	BOOL &GetSelAll() { return m_bIsSelAll; }
	BOOL IsTopicSelected();
	BOOL IsValid();
    TopicStatus GetTopicStatus() { return m_type; }
	BOOL PublishTopicToServer(BOOL bIsPrimary,CString strServerColumnIDs=_T(""),CString strKeyWord=_T(""),CString strSubject=_T(""));
	BOOL DeleteTopicFromServer();
// Dialog Data
	//{{AFX_DATA(CTopicListDlg)
	enum { IDD = IDD_TOPIC_LIST_DLG };
	CTopicListCtrl	m_listTopic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopicListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringList* m_pList;
    TopicStatus  m_type; 

	BOOL       m_bIsSelAll;
	// Generated message map functions
	//{{AFX_MSG(CTopicListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkListTopic(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListTopic(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//	afx_msg LRESULT OnLoadComboboxData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEdit(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};