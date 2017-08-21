// BaseObject.cpp: implementation of the CBaseObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BaseObject.h"
#include "CustomTree.h"
#include "WebDataMining.h"
#include "ListCtrlEx.h"
#include "progressbar.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//记录所有已创建的对象，用以进行内存管理
CPtrList g_ObjectsList;
void g_CleanupMemory()		//本函数用于清理内存
{
	CBaseObject *pOB;	
	while(g_ObjectsList.GetCount()>0)
	{
		pOB=(CBaseObject *)g_ObjectsList.RemoveHead();
		if(pOB != NULL)
			delete pOB;
	}
}

//////////////////////////////////////////////////////////////////////

CBaseObject::CBaseObject()
{
	m_pTreeWnd=NULL;
	m_hItem=NULL;
	m_pParent = NULL;
	m_listChildren.RemoveAll();
	m_nObType=OBJECT_TYPE_UNKNOWN;
	m_bNew=FALSE;
	m_bCanDelete=TRUE;
	m_nImg=0;
	m_nSelImg=1;	
	m_bDirty=FALSE;
	m_nMenuState=0xFFFFFFFF;
	m_bSpecShow=FALSE;
}

CBaseObject::CBaseObject(int nObType)
{
	m_pTreeWnd=NULL;
	m_hItem=NULL;
	m_pParent = NULL;
	m_listChildren.RemoveAll();
	m_nObType=nObType;
	m_bNew=FALSE;
	m_bDirty=FALSE;
	m_bCanDelete=TRUE;
	m_nImg=0;
	m_nSelImg=1;
	m_nMenuState=0xFFFFFFFF;
	m_bSpecShow=FALSE;
}
CBaseObject::CBaseObject(const CBaseObject& ob)
{
	m_nObType=ob.m_nObType;
	m_bNew=ob.m_bNew;
	m_bDirty=ob.m_bDirty;
	m_bCanDelete=ob.m_bCanDelete;
	m_nImg=ob.m_nImg;
	m_nSelImg=ob.m_nSelImg;
	m_nMenuState=ob.m_nMenuState;
	m_strID=ob.m_strID;
	m_strDesc=ob.m_strDesc;
	m_bSpecShow=ob.m_bSpecShow;

}
CBaseObject& CBaseObject::operator =(const CBaseObject& ob)
{
	if(&ob==this)
		return *this;

	m_nObType=ob.m_nObType;
	m_bNew=ob.m_bNew;
	m_bDirty=ob.m_bDirty;
	m_bCanDelete=ob.m_bCanDelete;
	m_nImg=ob.m_nImg;
	m_nSelImg=ob.m_nSelImg;
	m_nMenuState=ob.m_nMenuState;
	m_strID=ob.m_strID;
	m_strDesc=ob.m_strDesc;
	m_bSpecShow=ob.m_bSpecShow;

	return *this;
}
CBaseObject::~CBaseObject()
{
	POSITION pos=GetFirstChildPosition();
	while(pos)
	{
		CBaseObject *pChild=GetNextChild(pos);
		if(pChild != NULL && pChild->CanDelete())
		{
			delete pChild;
		}
	}
	m_listChildren.RemoveAll();
}

void CBaseObject::SetID(LPCSTR strID,LPCSTR strDesc)
{
	m_strID = strID;
	if(strDesc != NULL)
		m_strDesc=strDesc;
	else if(m_strDesc == _T(""))
		m_strDesc=strID;
}

void CBaseObject::ChangeDesc(LPCSTR strDesc)
{
	m_strDesc=strDesc;
}

CString CBaseObject::GetID()
{
	return m_strID;
}

CString CBaseObject::GetDesc()
{
	return m_strDesc;
}

BOOL &CBaseObject::IsNew()
{
	return m_bNew;
}

BOOL CBaseObject::IsModified()
{
	return m_bDirty;
}

void CBaseObject::SetModify(BOOL bModify)
{
	m_bDirty=bModify;
}

BOOL CBaseObject::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	return TRUE;
}

void CBaseObject::AddTo(CBaseObject *pParent)
{
	if(m_pParent != NULL)
		LeaveFrom(m_pParent);

	m_pParent=pParent;
	try
	{
		if(m_pParent != NULL)
		{
			m_pParent->m_listChildren.AddTail(this);
			DoJoin(m_pParent);

			SetModify();
		}
	}
	catch(CMemoryException *e)
	{
		e->ReportError();
	}
}

void CBaseObject::Add(CBaseObject *pChild)
{
	pChild->AddTo(this);

	SetModify();
}

void CBaseObject::LeaveFrom(CBaseObject *pParent)
{
	if(m_pParent != NULL && pParent == m_pParent)
	{
		pParent->GetRidOf(this);
	}

	SetModify();
}

void CBaseObject::GetRidOf(CBaseObject *pChild)
{
	POSITION pos=m_listChildren.Find(pChild);
	if(pos != NULL)
	{
		CBaseObject *pOb;		

		pOb=(CBaseObject *)m_listChildren.GetAt(pos);
		
		pOb->DoLeave(pOb->m_pParent);
		pOb->m_pParent=NULL;
		m_listChildren.RemoveAt(pos);

		SetModify();
	}
}

void CBaseObject::RemoveAllChild()
{
	CBaseObject *pOb;

	while(m_listChildren.GetCount() != 0)
	{
		pOb=(CBaseObject *)(m_listChildren.RemoveHead());
	
		pOb->DoLeave(pOb->m_pParent);
		pOb->m_pParent=NULL;
	}
}

void CBaseObject::DoJoin(CBaseObject *pParent)
{
	//In order to update relation correctly
	CBaseObject *pOB;
	POSITION pos=GetFirstChildPosition();
	
	while(pos != NULL)
	{
		pOB=(CBaseObject *)GetNextChild(pos);
		if(pOB != NULL)
			pOB->DoJoin(this);
	}
}

void CBaseObject::DoLeave(CBaseObject *pParent)
{
}


CBaseObject *CBaseObject::GetParent()
{
	return m_pParent;
}

CBaseObject *CBaseObject::BackTo(enum OBJECT_TYPE type)
{
	CBaseObject *pOB=GetParent();
	while(pOB != NULL)
	{
		if(pOB->GetType() == type)
			return pOB;
		pOB=pOB->GetParent();
	}

	return NULL;
}

CBaseObject *CBaseObject::FindSubObject(enum OBJECT_TYPE type,LPCSTR strID,BOOL bSub)
{
	POSITION pos=GetFirstChildPosition();
	CBaseObject *pOB,*pOB2;
	BOOL bFound=FALSE;

	while(pos)
	{
		pOB=GetNextChild(pos);
		if(pOB==NULL)
			continue;
		
		if(pOB->GetType() == type && pOB->GetID() == strID)
		{
			bFound=TRUE;
			break;
		}
		if(bSub && (pOB2=pOB->FindSubObject(type,strID,bSub)) != NULL)
		{
			bFound=TRUE;
			pOB=pOB2;
			break;
		}
	}

	if(!bFound)
		pOB=NULL;

	return pOB;
}

POSITION CBaseObject::GetFirstChildPosition()
{
	return m_listChildren.GetHeadPosition();
}

POSITION CBaseObject::GetTailChildPosition()
{
	return m_listChildren.GetTailPosition();
}
POSITION CBaseObject::GetChildPosition(CBaseObject* pChild)
{
	if( !IsHasChildrens() || pChild==NULL )
		return NULL;

    return m_listChildren.Find(pChild);
}
CBaseObject *CBaseObject::GetNextChild(POSITION &pos)
{
	if(pos != NULL)
		return (CBaseObject *)(m_listChildren.GetNext(pos));
	return NULL;
}

CBaseObject *CBaseObject::GetPrevChild(POSITION &pos)
{
	if(pos != NULL)
		return (CBaseObject *)(m_listChildren.GetPrev(pos));
	return NULL;
}

CWnd *CBaseObject::GetTreeWnd()
{
	return m_pTreeWnd;
}

void CBaseObject::SetRefItem(HTREEITEM hItem)
{
	m_hItem = hItem;
}

HTREEITEM CBaseObject::GetRefItem()
{
	return m_hItem;
}

void CBaseObject::InvalidateItemHandle()
{
	m_hItem=NULL;	
	
	CBaseObject *pOB;
	POSITION pos=m_listChildren.GetHeadPosition();
	while(pos != NULL)
	{
		pOB=(CBaseObject *)m_listChildren.GetNext(pos);		
		if(pOB != NULL)
			pOB->InvalidateItemHandle();
	}
}

void CBaseObject::RefreshLabel(CCustomTree *pTree)
{
	SetID(GetID());
	if(m_hItem != NULL)
		pTree->SetItemText(m_hItem,m_strDesc);
}


void CBaseObject::SetImg(int nImg,int nSelImg)
{
	m_nImg=nImg;
	m_nSelImg=nSelImg;
}

int CBaseObject::GetNormImg()
{
	return m_nImg;
}

int CBaseObject::GetSelImg()
{
	return m_nSelImg;
}

BOOL CBaseObject::IsShownInTree(CCustomTree *pTree)
{
	if(m_pTreeWnd == pTree)
	{
		TVITEM item;

		memset(&item,0,sizeof(TVITEM));
		item.mask=TVIF_HANDLE;
		item.hItem=m_hItem;

		return pTree->GetItem(&item);
	}

	return FALSE;
}

void CBaseObject::ShowInTreeView(CCustomTree *pTree,BOOL bChild)
{
	if(!IsShownInTree(pTree))
	{
		m_pTreeWnd=(CWnd *)pTree;
		pTree->InsertItem(this);
		TRACE("Add Item: pp %X, p %X, h %X\n",m_pParent,this,m_hItem);
	}
#ifdef _DEBUG
	else
	{
		TRACE("This object is already shown in tree view\n");
	}
#endif //_DEBUG

	if(bChild)
	{
		POSITION pos=GetFirstChildPosition();
		while(pos != NULL)
		{
			GetNextChild(pos)->ShowInTreeView(pTree,bChild);
		}
	}
}



UINT CBaseObject::EditObject(CWnd *pParent,BOOL bReadOnly/* =TRUE */)
{
	return IDOK;
}


BOOL CBaseObject::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在保存:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}
	
	CBaseObject *pOB;
	POSITION pos=GetFirstChildPosition();

	while(pos != NULL && bChild)
	{
		pOB=(CBaseObject *)GetNextChild(pos);
		if(pOB != NULL)
			pOB->SaveToDB(db,bChild,pProgressBar);
	}

	SetModify(FALSE);
    IsNew()=FALSE;

	return TRUE;
}

BOOL CBaseObject::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在加载:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}

	CBaseObject *pOB;
	POSITION pos=GetFirstChildPosition();

	while(pos != NULL)
	{
		pOB=(CBaseObject *)GetNextChild(pos);
		if(pOB != NULL)
			pOB->LoadFromDB(db,pProgressBar);
	}

	SetModify(FALSE);

	return TRUE;
}

BOOL CBaseObject::LoadInfo(CADORecordset &rs)
{
	return TRUE;
}

BOOL CBaseObject::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在删除:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}

	POSITION pos=GetTailChildPosition();
	CBaseObject* pChild;
	while(pos)
	{
		pChild=GetPrevChild(pos);
		if(pChild!=NULL)
		{
			pChild->DeleteFromDB(db,bDelSelf,pProgressBar);
		}
	}
	//Do delete operation

	return TRUE;
}



BOOL &CBaseObject::CanDelete()
{
	return m_bCanDelete;
}

UINT CBaseObject::PromptMenuID()
{
	return 0;
}

int CBaseObject::PromptMenuPos()	
{
	return -1;
}

void CBaseObject::UpdateMenu(CMenu *pMenu)
{

}

void* CBaseObject::operator new(size_t nSize)
{
	void *p=::operator new(nSize);
	g_ObjectsList.AddTail(p);

	return p;
}

void CBaseObject::operator delete(void* p)
{
	POSITION pos=g_ObjectsList.Find(p);
	if(pos)  g_ObjectsList.RemoveAt(pos);
	
	#ifdef _DEBUG
		#ifdef _AFX_NO_DEBUG_CRT
			free(p);
		#else
			_free_dbg(p, _CLIENT_BLOCK);
		#endif //_AFX_NO_DEBUG_CRT
	#else
		::operator delete(p);
	#endif //_DEBUG
}

//由于调试环境重定义了new和delete运算符，因此需要提供额外的重载形式
 #ifdef _DEBUG
	void* PASCAL CBaseObject::operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
	{
		void *p;

		#ifdef _AFX_NO_DEBUG_CRT
			p=::operator new(nSize);
		#else
			p=::operator new(nSize, _CLIENT_BLOCK, NULL, 0);
		#endif // _AFX_NO_DEBUG_CRT

		g_ObjectsList.AddTail(p);
		return p;
	}

	#if _MSC_VER >= 1200
	void PASCAL CBaseObject::operator delete(void *p, LPCSTR lpszFileName, int nLine)
	{
		POSITION pos=g_ObjectsList.Find(p);
		if(pos)  g_ObjectsList.RemoveAt(pos);

		#ifdef _AFX_NO_DEBUG_CRT
			free(p);
		#else
			_free_dbg(p, _CLIENT_BLOCK);
		#endif //_AFX_NO_DEBUG_CRT
	}
 	#endif //_MSC_VER >= 1200
#endif //_DEBUG

TiXmlNode*  CBaseObject::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在导出:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}
	if(bChild)
	{
		POSITION pos=GetFirstChildPosition();
		CBaseObject *pOB;
		while(pos)
		{
			pOB=GetNextChild(pos);
			if(pOB)
				pOB->StreamOut(pNode,bChild,pProgressBar);
		}
	}
	return pNode->FirstChild();
}

BOOL CBaseObject::StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在导入:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}
// 	if(bChild)
// 	{
// 		POSITION pos=GetFirstChildPosition();
// 		CBaseObject *pOB;
// 		while(pos)
// 		{
// 			pOB=GetNextChild(pos);
// 			if(pOB)
// 				pOB->StreamIn(pNode,bChild,pProgressBar);
// 		}
// 	}
	return TRUE;
}

UINT CBaseObject::RetrieveMenuItemID(CMenu* pMenu,CString strMenuItem)
{
	UINT id=-1;
	ASSERT(pMenu);
	ASSERT(::IsMenu(pMenu->GetSafeHmenu()));
	
	int count = pMenu->GetMenuItemCount();
	for (int i = 0; i < count; i++)
	{
		CString str;
		if (pMenu->GetMenuString(i, str, MF_BYPOSITION) &&
			(strcmp(str, strMenuItem) == 0))
		{
			id=pMenu->GetMenuItemID(i);
			break;
		}
	}
	return id;	
	
}

void CBaseObject::SetMenuState(UINT nMenuItemState)
{
	m_nMenuState=nMenuItemState;
}

UINT CBaseObject::GetMenuState()
{
	return m_nMenuState;

}


void CBaseObject::RelatingDelete()
{
	CBaseObject* pOb;
	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		pOb=GetNextChild(pos);
		if (pOb!=NULL)
		{
			pOb->RelatingDelete();
		}
	}

}

void CBaseObject::DownloadObject(BOOL bAuto /* =FALSE  */)
{
	CBaseObject* pOb;
	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		pOb=GetNextChild(pos);
		if (pOb!=NULL)
		{
			pOb->DownloadObject(bAuto);
		}
	}
}
void CBaseObject::DelSubObject(BOOL bDelFromDB /*=TRUE*/ )
{
	CBaseObject* pOb;
	POSITION pos=GetFirstChildPosition();
	while (pos)
	{
		pOb=GetNextChild(pos);
		if (pOb!=NULL)
		{
			pOb->DelSubObject(bDelFromDB);
		}
	}
}

BOOL CBaseObject::IsHasChildrens()
{
	return m_listChildren.GetCount()>0;
}
int CBaseObject::GetChildCount()
{
	return m_listChildren.GetCount();
}
int CBaseObject::GetChildType()
{
	int nType=OBJECT_TYPE_UNKNOWN;
	if(IsChildSameType() && GetChildCount()>0 )
	{
		CBaseObject* pChild=(CBaseObject*)m_listChildren.GetHead();
		nType=pChild->GetType();
    }
	return nType;
}
BOOL CBaseObject::IsChildSameType()
{
	if(GetChildCount()>2)
	{
         int nOldType=-1,nNewType;
		 POSITION pos=GetFirstChildPosition();
		 while (pos)
		 {
			 CBaseObject* pChild=GetNextChild(pos);
			 if(pChild!=NULL)
			 {			
				 nNewType=pChild->GetType();
				 if(nOldType==-1)
					 nOldType=nNewType;
				 else
				 {
					 if(nNewType!=nOldType)
					     return FALSE;
				 }
			 }
		 }
	}	
	return TRUE;
}
BOOL CBaseObject::IsSpecShow()
{
	return m_bSpecShow;
}

void CBaseObject::SetSpecShowFlag(BOOL bSpecShow/* =TRUE */)
{
	m_bSpecShow=bSpecShow;
}

void CBaseObject::SpecShowInTree(CCustomTree *pTree,BOOL bSpecShow/* =TRUE */)
{
//	ASSERT_VALID(pTree);
//	ASSERT_KINDOF(CCustomTree,pTree);

	pTree->SetItemSpecShow(GetRefItem(),bSpecShow);
}
void CBaseObject::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	pListCtrl->DeleteAllItems();
	pListCtrl->DeleteAllColumn();
}

void CBaseObject::FillListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	ShowListCtrl(pListCtrl,bSelf);

	for(int i=0;i<pListCtrl->GetColumnCounts();i++)
	{
		pListCtrl->SetReadOnlyColumns(i);
	}
}
TiXmlNode* CBaseObject::Export(CString& strDir,TiXmlNode* pNode,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在导出:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}
	if(bChild)
	{
		POSITION pos=GetFirstChildPosition();
		CBaseObject *pOB;
		while(pos)
		{
			pOB=GetNextChild(pos);
			if(pOB)
				pOB->Export(strDir,pNode,bChild,pProgressBar);
		}
	}
	return (pNode==NULL)?NULL:pNode->FirstChild();
}
void CBaseObject::StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	if(pProgressBar)
	{
		CString str;
	    str.Format("正在导出:%s",GetDesc());
		pProgressBar->UpdateBar(str,100);
	}
	if(bChild)
	{
		POSITION pos=GetFirstChildPosition();
		CBaseObject *pOB;
		while(pos)
		{
			pOB=GetNextChild(pos);
			if(pOB)
				pOB->StreamOutTxt(stream,bChild,pProgressBar);
		}
	}
}