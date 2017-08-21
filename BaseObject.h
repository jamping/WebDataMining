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
公共基类，用以提供树视图对栏目、条目、文章和规则等管理的公共界面
它与 CCustomTree 基类构成下述关系

  CBaseObject <--> CCustomTree Item
*/

//用于区分各个对象类型
enum OBJECT_TYPE
{
	OBJECT_TYPE_UNKNOWN,			//未知类型 
	OBJECT_TYPE_HELPER,				//辅助类型
	OBJECT_TYPE_COLUMN,				//采集栏目
	OBJECT_TYPE_ENTRY,				//采集条目
	OBJECT_TYPE_TOPIC,				//采集文章
	OBJECT_TYPE_PICTURE,			//图片
	OBJECT_TYPE_RULE,				//采集规则
	OBJECT_TYPE_RULETREE,			//规则树	
	OBJECT_TYPE_SCHEDULE,			//调度计划
	OBJECT_TYPE_DBSERVER,			//数据库服务器
	OBJECT_TYPE_DATABASE,			//数据库
	OBJECT_TYPE_TABLE,				//数据库表
	OBJECT_TYPE_OPERATOR,			//数据库表操作
	OBJECT_TYPE_OPERATOR_UNIT,       //操作元素
	OBJECT_TYPE_SERVER_COLUMN        //服务器栏目
};

//数据更新方式
enum DATA_STATUS
{
	MODIFY_NONE,			//数据没有更新
	MODIFY_ADD,				//新添加数据
	MODIFY_UPDATE,			//数据被更新
	MODIFY_DELETE			//该对象被删除
};
//定义默认空ID的值
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
	virtual void RelatingDelete();       //级联删除
    virtual void DownloadObject(BOOL bAuto =FALSE );        //下载文章
    virtual void DelSubObject(BOOL bDelFromDB =TRUE );     //删除下载文章

	virtual void SetID(LPCSTR strID,LPCSTR strDesc=NULL);	//
	virtual void ChangeDesc(LPCSTR strDesc);
	CString GetID();								//
	CString GetDesc();								//

	BOOL &CanDelete();				//For some objects, can't delete them by user

	BOOL &IsNew();
	BOOL IsModified();
	
	void SetModify(BOOL bModify = TRUE);

	//挂接操作
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
	CBaseObject *BackTo(OBJECT_TYPE type);			//回溯到指定类型的节点
	CBaseObject *FindSubObject(OBJECT_TYPE type,LPCSTR strID,BOOL bSub=FALSE);		//获取指定ID的子节点
    
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

	//显示操作
	void SetImg(int nImg,int nSelImg);
	int GetNormImg();
	int GetSelImg();

	//是否突出显示
    BOOL IsSpecShow();
	void SetSpecShowFlag(BOOL bSpecShow=TRUE);
	void SpecShowInTree(CCustomTree *pTree,BOOL bSpecShow=TRUE);
	
	virtual BOOL IsShownInTree(CCustomTree *pTree);	//determine whether this object is shown in a tree view
	virtual void ShowInTreeView(CCustomTree *pTree,BOOL bChild=FALSE);	//show self in a tree view

	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);				//show self in a modal dialog , 
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	void FillListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	//数据库操作
	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);		//Save to database
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	//Load from database
	virtual BOOL LoadInfo(CADORecordset &rs);		//Load from database
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	//从数据库中删除
    //菜单操作
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
