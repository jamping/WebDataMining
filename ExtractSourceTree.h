// ExtractSourceTree.h : header file
//
#pragma once

#include "customtree.h"
#include "HelperObject.h"
#include "ReceiveSocket.h"
#include "Entry.h"
#include "Topic.h"
#include <queue>
/////////////////////////////////////////////////////////////////////////////
// CExtractSourceTree window

class CExtractSourceTree : public CCustomTree
{
// Construction
public:
	CExtractSourceTree();

	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//响应删除键消息
	virtual void InitUpdate(BOOL bReadOfDB=TRUE,BOOL bShowInfo=FALSE);
    virtual BOOL DoDrop(HTREEITEM hItemDragged, HTREEITEM hItemTarget, int nAction);

	void ProcessPendingRead(char* buf);

	CHelperObject m_objectHelper;
	UINT          m_nTimerID;
    CReceiveSocket*   m_pReceiveSocket;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtractSourceTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtractSourceTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtractSourceTree)
	afx_msg void OnRootNew();
	afx_msg void OnColumnNewentry();
	afx_msg void OnRootSave();
	afx_msg void OnRootFresh();
	afx_msg void OnRootDownload();
	afx_msg void OnRootDelete();
	afx_msg void OnColumnDelete();
	afx_msg void OnColumnDownload();
	afx_msg void OnColumnEdit();
	afx_msg void OnColumnSave();
	afx_msg void OnEntryDelete();
	afx_msg void OnEntryDownload();
	afx_msg void OnEntryEdit();
	afx_msg void OnEntrySave();
	afx_msg void OnPictureDelete();
	afx_msg void OnPictureEdit();
	afx_msg void OnPictureSave();
	afx_msg void OnTopicDelete();
	afx_msg void OnTopicEdit();
	afx_msg void OnTopicPublish();
	afx_msg void OnTopicSave();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnColumnDelTopic();
	afx_msg void OnRootDelTopic();
	afx_msg void OnEntryDelTopic();
	afx_msg void OnColumnNew();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnColumnSetDbserver();
	afx_msg void OnColumnSchedule();
	afx_msg void OnEntrySchedule();
	afx_msg void OnTopicDelPicture();
	afx_msg void OnColumnFresh();
	afx_msg void OnEntryFresh();
	afx_msg void OnExport();
	afx_msg void OnImport();
	afx_msg void OnEntrySetColumnid();
	afx_msg void OnEntryPasteTopic();
	afx_msg void OnPictureCut();
	afx_msg void OnTopicCut();
	afx_msg void OnTopicPastePic();
	afx_msg void OnAlterTopicUnit();
	afx_msg void OnEntryNewTopic();
	afx_msg void OnColumnCollapse();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void GetEntrys(CBaseObject* pOb);
    void GetTopics(CBaseObject* pOb);
//    void ProcessContent(CString& strContent,CString strKeyWord,int nType);

	std::queue<CEntry *>  m_qEntrys;
	std::queue<CTopic *>  m_qTopics;
	CPtrList m_listObject;              //缓存选择对象
};