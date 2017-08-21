// BaseObject.h: interface for the CBaseObject class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
using namespace std;

class CCustomTree;
class CListCtrlEx;
class CProgressBar;

/*
�������࣬�����ṩ����ͼ����Ŀ����Ŀ�����º͹���ȹ���Ĺ�������
���� CCustomTree ���๹��������ϵ

  CBaseObject <--> CCustomTree Item
*/

//�������ָ�����������
enum OBJECT_TYPE
{
	OBJECT_TYPE_UNKNOWN,			//δ֪���� 
	OBJECT_TYPE_HELPER,				//��������
	OBJECT_TYPE_COLUMN,				//�ɼ���Ŀ
	OBJECT_TYPE_ENTRY,				//�ɼ���Ŀ
	OBJECT_TYPE_TOPIC,				//�ɼ�����
	OBJECT_TYPE_PICTURE,			//ͼƬ
	OBJECT_TYPE_RULE,				//�ɼ�����
	OBJECT_TYPE_RULETREE,			//������	
	OBJECT_TYPE_SCHEDULE,			//���ȼƻ�
	OBJECT_TYPE_DBSERVER,			//���ݿ������
	OBJECT_TYPE_DATABASE,			//���ݿ�
	OBJECT_TYPE_TABLE,				//���ݿ��
	OBJECT_TYPE_OPERATOR,			//���ݿ�����
	OBJECT_TYPE_OPERATOR_UNIT,       //����Ԫ��
	OBJECT_TYPE_SERVER_COLUMN        //��������Ŀ
};

//���ݸ��·�ʽ
enum DATA_STATUS
{
	MODIFY_NONE,			//����û�и���
	MODIFY_ADD,				//���������
	MODIFY_UPDATE,			//���ݱ�����
	MODIFY_DELETE			//�ö���ɾ��
};
//����Ĭ�Ͽ�ID��ֵ
static char NULL_ID[] = _T("00000000-0000-0000-0000-000000000000");

class CBaseObject  
{
public:
	CBaseObject();
	CBaseObject(const CBaseObject& ob);
	CBaseObject& operator=(const CBaseObject& ob);
	CBaseObject(int nObType);
	virtual ~CBaseObject();

protected:
	CString m_strID;			//ID of this element
	int     m_nObType;	        //Type of Object
	
	CBaseObject *m_pParent;		//Parent object,maybe a troop,a equipment,a mission etc. or even a helper
	CPtrList m_listChildren;	//Children of this element

	CString m_strDesc;			//Description of this element, it will be tree item text
	CWnd *m_pTreeWnd;			//Which tree window is this object shown
	HTREEITEM m_hItem;			//UI element
	int  m_nImg,m_nSelImg;      //Tree node icon selected or not
    BOOL m_bSpecShow;           //whether the node is special show	
	


	BOOL m_bNew;                //New 
	BOOL m_bDirty;				//Data  Update when true
	BOOL m_bCanDelete;          //whether is it can been deleted or not

	UINT m_nMenuState;          //menu updated command UI

		
public:
	virtual void RelatingDelete();       //����ɾ��
    virtual void DownloadObject(BOOL bAuto =FALSE );        //��������
    virtual void DelSubObject(BOOL bDelFromDB =TRUE );     //ɾ����������

	virtual void SetID(LPCSTR strID,LPCSTR strDesc=NULL);	//
	virtual void ChangeDesc(LPCSTR strDesc);
	CString GetID();								//
	CString GetDesc();								//

	BOOL &CanDelete();				//For some objects, can't delete them by user

	BOOL &IsNew();
	BOOL IsModified();
	
	void SetModify(BOOL bModify = TRUE);

	//�ҽӲ���
    int GetType()	{ return m_nObType; }	//Get object type
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);		//Determine whether it can accept such type of object or not
	void AddTo(CBaseObject *pParent);				//Add self to a object
	void Add(CBaseObject *pChild);					//Add a object to my child list
	void LeaveFrom(CBaseObject *pParent);			//
	void GetRidOf(CBaseObject *pChild);				//
	void RemoveAllChild();							//
	virtual void DoJoin(CBaseObject *pParent);			//Called when an object joined to its parent
	virtual void DoLeave(CBaseObject *pParent);			//called when an object leaved from its parent

	CBaseObject *GetParent();						//
	CBaseObject *BackTo(OBJECT_TYPE type);			//���ݵ�ָ�����͵Ľڵ�
	CBaseObject *FindSubObject(OBJECT_TYPE type,LPCSTR strID,BOOL bSub=FALSE);		//��ȡָ��ID���ӽڵ�
    
	POSITION GetFirstChildPosition();				//Start to enumerate 
	POSITION GetTailChildPosition();
	POSITION GetChildPosition(CBaseObject* pChild);
	CBaseObject *GetNextChild(POSITION &pos);		//
	CBaseObject *GetPrevChild(POSITION &pos);		//
    BOOL IsHasChildrens();
	int  GetChildCount();
	int  GetChildType();
    BOOL IsChildSameType();
    
	CWnd *GetTreeWnd();								//get the tree window
	void SetRefItem(HTREEITEM hItem);				//if this object is shown in a tree view, the item handle should be set here
	HTREEITEM GetRefItem();							//object --> tree item handle
	void InvalidateItemHandle();					//Invalidate m_hItem attribute, including all the children's
	void RefreshLabel(CCustomTree *pTree);			//Fresh item text

	//��ʾ����
	void SetImg(int nImg,int nSelImg);
	int GetNormImg();
	int GetSelImg();

	//�Ƿ�ͻ����ʾ
    BOOL IsSpecShow();
	void SetSpecShowFlag(BOOL bSpecShow=TRUE);
	void SpecShowInTree(CCustomTree *pTree,BOOL bSpecShow=TRUE);
	
	virtual BOOL IsShownInTree(CCustomTree *pTree);	//determine whether this object is shown in a tree view
	virtual void ShowInTreeView(CCustomTree *pTree,BOOL bChild=FALSE);	//show self in a tree view

	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);				//show self in a modal dialog , 
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	void FillListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	//���ݿ����
	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);		//Save to database
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	//Load from database
	virtual BOOL LoadInfo(CADORecordset &rs);		//Load from database
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	//�����ݿ���ɾ��
    //�˵�����
    void SetMenuState(UINT nMenuItemState);
	UINT GetMenuState();
	UINT RetrieveMenuItemID(CMenu *pMenu,CString strMenuItem);

	virtual UINT PromptMenuID();			//Get the prompt menu id
	virtual int PromptMenuPos();			//Get the submenu position
	virtual void UpdateMenu(CMenu *pMenu);	//OnUpdateUI

	void* operator new(size_t nSize);
	void operator delete(void* p);
#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
	void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);	
#if _MSC_VER >= 1200
	void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine);
#endif
#endif

    virtual TiXmlNode* Export(CString& strDir,TiXmlNode* pNode,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an xml
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an xml
	virtual void StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
private:

};
