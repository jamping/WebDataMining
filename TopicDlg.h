// TopicDlg.h : header file
//
#pragma once

#include "Topic.h"
#include "CDragEdit.h"
#include "AutoCompl.h"
#include "CheckComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CTopicDlg dialog

class CTopicDlg : public CDialogEx
{
// Construction
public:
	CTopicDlg(CTopic* pTopic,CWnd* pParent = NULL);   // standard constructor
    ~CTopicDlg() {}

	void SetValue(CTopic::TopicData& data);
	BOOL GetValue(CTopic::TopicData& data);
// Dialog Data
	//{{AFX_DATA(CTopicDlg)
	enum { IDD = IDD_TOPIC_DLG };
	CCheckComboBox	m_ctlColumn;
	CDragEdit	m_ctlMainTitle;
	CDragEdit	m_ctlTitle;
	CDragEdit	m_ctlSubTitle;
	CDragEdit	m_ctlHeadTitle;
	CDragEdit	m_ctlContent;
	CDragEdit	m_ctlAuthor;
	CString	m_strContent;
	CString	m_strHeadTitle;
	CString	m_strSource;
	CString	m_strSubTitle;
	CString	m_strSubject;
	CString	m_strTitle;
	CString	m_strAuthor;
	CString	m_strTime;
	BOOL	m_bIsPrimary;
	CString	m_strKeyWord;
	CString	m_strMainTitle;
	int m_nWeight;
	//BOOL	m_bIsElite;
	//BOOL	m_bIsOnTop;
	//int		m_nPrimaryNumber;
	//}}AFX_DATA
	CString	m_strBigClass;
	CString	m_strSmallClass;
	CString m_strServerColumnIds;

	CBaseObject* m_pParent;
    CTopic*    m_pTopic;
    int        m_nCurKeyWord;
	CStringArray m_arKeyWord;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopicDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CAutoComplete   autoKeyWord,autoSubject,autoSource,autoTime;

	void InitColumn(CBaseObject* pRoot,int nLevel);
	void Init();
	void GetKeyWord();
	// Generated message map functions
	//{{AFX_MSG(CTopicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnViewPicture();
	afx_msg void OnButtonPublish();
	afx_msg void OnKeywordPrev();
	afx_msg void OnKeywordNext();
	afx_msg void OnButtonView();
	afx_msg void OnButtonModify();
	virtual void OnOK();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonNewTopic();
	afx_msg void OnPasteCode();
	afx_msg void OnFormat();
	afx_msg void OnFilter();
	//}}AFX_MSG
	afx_msg void OnUpdatePublish(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewPicture(CCmdUI *pCmdUI);
	afx_msg void OnUpdateKeyWordPrev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateKeyWordNext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowPrevTopic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowNextTopic(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePasteCode(CCmdUI *pCmdUI);
	afx_msg LRESULT OnComboTreeCheck(WPARAM w,LPARAM l);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
};