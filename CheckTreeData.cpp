//#######################################################################################
//## CheckTreeData.cpp : implementation file
//## [Magerusan G. Cosmin] 13-apr-2002
//#######################################################################################
#include "stdafx.h"

#include "CheckTreeData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//## ====================================================================================
CCheckTreeNode::CCheckTreeNode(LPCTSTR lpszString)
{
	//## DEFAULT
	strID = INVALID_ID;
	nLevel = ROOT_LEVEL;
	bChecked = FALSE;
	bIsLeaf = FALSE;

	//## SET String
	strCaption = lpszString;
}	
//#######################################################################################
CCheckTreeData::CCheckTreeData()
{
	//## INITIALIZE Data
	Reset();
}
//## ====================================================================================
CCheckTreeData::~CCheckTreeData()
{
	//## RESET
	Reset();
}
//## ====================================================================================
void CCheckTreeData::Reset()
{
	//## RESET Content
	RemoveAll();

	//## INITIALIZE
	m_nMaxLevel = ROOT_LEVEL;
}
//#######################################################################################
void CCheckTreeData::AddString(LPCTSTR lpszString, CString strID, long nLevel)
{
	//## ASSERT
	ASSERT(nLevel < TREE_MAX_LEVELS);

	//## INITIALIZE Node
	CCheckTreeNode node(lpszString);
	node.strID = strID;
	node.nLevel = nLevel;
	node.bIsLeaf = (strID != INVALID_ID) ? TRUE : FALSE;

	//## UPDATE MaxLevel
	if (nLevel > m_nMaxLevel) m_nMaxLevel = nLevel;

	//## ADD Node
	Add( node );
}
//#######################################################################################
void CCheckTreeData::UpdateChecksDown(long nIndex)
{
	//## GET Current level
	long nIndexLevel = ElementAt(nIndex).nLevel;
	BOOL bIndexChecked = ElementAt(nIndex).bChecked;

	//## SCAN & Update
	for(long i=nIndex+1; (i < GetSize()) && (ElementAt(i).nLevel > nIndexLevel); i++)
		ElementAt(i).bChecked = bIndexChecked;
}
//## ====================================================================================
void CCheckTreeData::UpdateChecksUp(long nLevel)
{
	//## DECLARE
	long nChecked = 0L, nUnChecked = 0L, nItemLevel = 0L;

	//## SCAN up -> down
	for(long i = (GetSize() - 1); i >= 0; i--){
		//## GET Item Level
		nItemLevel = ElementAt( i ).nLevel;

		//## COUNT checked/unchecked items
		if (nItemLevel == nLevel)
			if (ElementAt( i ).bChecked) nChecked++;
				else nUnChecked++;

		//## UP node
		if ((nItemLevel + 1) == nLevel){
			if ((nChecked > 0) || (nUnChecked > 0)){
				if (nUnChecked == 0) ElementAt( i ).bChecked = TRUE;
				if (nUnChecked > 0) ElementAt( i ).bChecked = FALSE;
			}
			nChecked = nUnChecked = 0L;
		}
	}
}
//## ====================================================================================
void CCheckTreeData::UpdateChecksUp()
{
	//## UPDATE Levels
	for(long i=m_nMaxLevel; i > 0; i--){
		UpdateChecksUp( i );
	}
}
//#######################################################################################
void CCheckTreeData::UpdateChecks(long nIndex)
{
	//## PROPAGATE Checks DOWN
	UpdateChecksDown(nIndex);

	//## PROPAGATE Checks UP
	UpdateChecksUp();
}
//#######################################################################################
void CCheckTreeData::CheckAll(BOOL bCheck)
{
	//## CHECK All Items
	for(long i=0; i<GetSize(); i++){
		ElementAt(i).bChecked = bCheck;
	}
}
//## ====================================================================================
BOOL CCheckTreeData::GetCheck(CString strID)
{
	//## GET check
	for(long i=0; i<GetSize(); i++)
		if (ElementAt(i).strID == strID)
			return ElementAt(i).bChecked;

	//## ITEM not found
	return FALSE;
}
//## ====================================================================================
BOOL CCheckTreeData::GetCheckAtIndex(long nIndex)
{
	return ElementAt(nIndex).bChecked;
}
//## ====================================================================================
void CCheckTreeData::SetCheck(CString strID, BOOL bCheck)
{
	//## CHECK Item
    long i=0;
	for( i=0; i<GetSize(); i++)
		if (ElementAt(i).strID == strID){
			ElementAt(i).bChecked = bCheck;
			break;
		}

	//## UPDATE Checks on levels, starting with node i
	if ( i < GetSize())
		UpdateChecks(i);
}
void CCheckTreeData::SetChecks(CString strIDs, BOOL bCheck)
{
    strIDs.TrimLeft();
	strIDs.TrimRight();
	//Get ID
	int nIndex=0;
	CString strID;
	CStringArray  ar;

	for(nIndex=strIDs.Find(' ');nIndex!=-1;nIndex=strIDs.Find(' '))
	{
        strID=strIDs.Left(nIndex);
		ar.Add(strID);

		strIDs=strIDs.Mid(nIndex+1);
		strIDs.TrimLeft();
	}
    ar.Add(strIDs);

	for(nIndex=0;nIndex<ar.GetSize();nIndex++)
		SetCheck(ar.GetAt(nIndex),bCheck);
    
}
//#######################################################################################
CString CCheckTreeData::GetCheckedTexts()
{
	//## ASSERT
	if (GetSize() <= 0) return TEXT("");

	//## CASE: when only a single element is in the list
	CString str;
	if ((GetSize() == 2) && (ElementAt(ROOT_INDEX).bChecked)){
		str = ElementAt(1).strCaption;
		return str;
	}

	//## CHECK if all items were Checked
	if (ElementAt(ROOT_INDEX).bChecked){
		str = ROOT_CAPTION;
		return str;
	}

	//## COLLECT all Checked items in a single string
	long nCount = 0;
	for(long i=0; i<GetSize(); i++)
		if ((ElementAt(i).bIsLeaf) && (ElementAt(i).bChecked)){
			str += ElementAt(i).strCaption + ", ";
			nCount++;
		}

	//## ADJUST String
	if (str.GetLength() > 2) str = str.Mid(0, str.GetLength()-2);
	if (nCount > 1) str = (CString)"(" + str + ")";

	//## RETURN
	return str;
}
//## ====================================================================================
CString CCheckTreeData::GetCheckedIDs()
{
	//## ASSERT
	if (GetSize() <= 0) return TEXT("");

	//## CHECK if all items were checked
	CString str, strID;
// 	if (ElementAt(ROOT_INDEX).bChecked){
// 		str = ROOT_CAPTION;
// 		return str;
// 	}

	//## COLLECT all checked items in a single string
	long nCount = 0;
	for(long i=0; i<GetSize(); i++)
		if ((ElementAt(i).bIsLeaf) && (ElementAt(i).bChecked)){
			strID=ElementAt(i).strID;
			str += strID + " ";
			nCount++;
		}

	//## ADJUST String
// 	if (str.GetLength() > 2) str = str.Mid(0, str.GetLength()-2);
// 	if (nCount > 0) str = (CString)"(" + str + ")";
    str.TrimLeft();
	str.TrimRight();
	//## RETURN
	return str;
}
//#######################################################################################
