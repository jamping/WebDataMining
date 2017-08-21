//#######################################################################################
//## CheckTreeData.h : header file
//## [Magerusan G. Cosmin] 20-apr-2002
//#######################################################################################
#pragma once

//## ====================================================================================
#define INVALID_ID _T("00000000-0000-0000-0000-000000000000")  //0xFFFFFFFFL
//## ====================================================================================
#define ROOT_LEVEL 0L
#define ROOT_INDEX 0L
#define ROOT_CAPTION TEXT("ËùÓÐÆµµÀ")
#define TREE_MAX_LEVELS 22L
//#######################################################################################
class CCheckTreeNode
{
//## CONSTRUCTOR
public:
	CCheckTreeNode(LPCTSTR lpszString = TEXT(""));

//## STATE
public:
	CString strCaption;
	CString strID;
	long nLevel;
	BOOL bChecked;
	BOOL bIsLeaf;
};
//#######################################################################################
class CCheckTreeData : public CArray<CCheckTreeNode, CCheckTreeNode&>
{	
//## CONSTRUCTOR
public:
	CCheckTreeData();
	virtual ~CCheckTreeData();

//## ATTRIBUTES
private:
	long m_nMaxLevel;

//## METHODS
private:
	void UpdateChecksDown(long nIndex);
	void UpdateChecksUp(long nLevel);
	void UpdateChecksUp();

public:
	void Reset();
	void AddString(LPCTSTR lpszString, CString strID = INVALID_ID, long nLevel = ROOT_LEVEL + 1);
	long GetMaxLevel(){ return m_nMaxLevel; };

//## SELECTION Specific
public:
	void UpdateChecks(long nIndex);
	void CheckAll(BOOL bSelect);
	BOOL GetCheck(CString strID);
	BOOL GetCheckAtIndex(long nIndex);
	void SetCheck(CString strID, BOOL bSelect);
	void SetChecks(CString strIDs,BOOL bSelect);
	CString GetCheckedTexts();
	CString GetCheckedIDs();
};