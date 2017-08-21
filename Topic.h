// Topic.h: interface for the CTopic class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseObject.h"
#include "wdmdatabase.h"
#include "WDMTable.h"
#include "NewsPicture.h"

class CWDMOperatorUnit;

typedef struct tagTopicParam
{
	char     strTopicID[40];
	BOOL     bIsPublished;
}TopicParam,*LPTOPICPARAM;

class CTopic : public CBaseObject  
{
public:
	struct TopicData 
	{
		CString  m_strTopicID;             //文章ID		
		CString  m_strEntryID;             //条目ID
		CString  m_strURL;                 //原文URL
		CString  m_strOldTitle;            //原文标题
		CString  m_strHeadTitle;           //引标
		CString  m_strTitle;               //标题
		CString  m_strMainTitle;           //缩标
		CString  m_strSubTitle;            //子标题
		CString  m_strContent;             //内容
		CString  m_strSource;              //来源
		CString  m_strTime;                //时间
		CString  m_strAuthor;              //作者
		CString  m_strKeyWord;             //关键字
		CString  m_strSubject;             //专题词		
		BOOL     m_bIsPrimary;             //是否重要
		BOOL     m_bIsPublish;             //是否发布
		BOOL     m_bIsSendto;              //是否传送到内网
		COleDateTime m_timeDownload;       //下载时间
	    CString  m_strServerColumnIDs;     //服务器栏目IDs
		int      m_nWeight;				   //文章权重
		CString  m_strBigClass;            //大类
		CString  m_strSmallClass;          //小类
        CString  m_strServerAutoID;        //保存发布成功后返回的服务器自增ID
		CString  m_strThumb;               //缩略图路径

		TopicData()
		{
			m_bIsPrimary=TRUE;
			m_bIsPublish=FALSE;
			m_bIsSendto=FALSE;
			m_nWeight=0;
			m_strServerAutoID=_T("");
		}
	};
	enum TopicUnit
	{
		T_DEFAULTVALUE = 0  ,          // 缺省值
		T_SERVERAUTOID   	,          // 自增ID
		T_TOPICID        	,		   // 文章ID
		T_HEADTITLE         ,          // 引标
		T_TITLE			    ,	       // 标题
		T_OLDTITLE			,		   // 原标题
		T_OLDURL		    ,          // 原链接
		T_MAINTITLE     	,		   // 缩标
		T_SUBTITLE			,          // 副标
		T_TIME				,          // 时间
		T_SOURCE			,	       // 来源
		T_AUTHOR			,		   // 作者
		T_KEYWORD        	,          // 关键字
		T_SUBJECT			,          // 专题词
		T_CONTENT			,	       // 内容
		T_BIGCLASS			,		   // 大类
		T_SMALLCLASS		,          // 小类
		T_PRIMARY         	,          // 是否重要
		T_THUMB          	,          // 缩略图路径
		T_SERVERCOLUMNID	,          // 服务器栏目ID
		T_SERVERCHANNELID 	,	       // 服务器频道ID
		T_RESOURCEID      	,	       // 图片ID
		T_RESOURCEURL     	,		   // 图片URL
		T_RESOURCEEXPLAIN 	,          // 图片说明
		T_RESOURCENAME    	,          // 图片名称
		T_RESOURCEEXT     	,	       // 图片扩展名
		T_RESOURCENUM     	,          // 图片数量
		T_RESOURCESIZE		,	       // 图片大小	
		T_ISTHUMB         	,	       // 是否是缩略图
		T_TIMESTAMP      	,		   // 时间戳
		T_LINKURL        	,          // 转向链接
		T_EDITOR          	,		   // 编辑
		T_WEIGHT   		    ,          // 文章权重
		T_REFERVALUE        ,          // 引用缓存值
		T_READDATAOFDB                 // 读取库中数据
	};						          
						 	       
	//static	const DWORD64	T_HEADTITLE               =1ULL;
	//static	const DWORD64	T_TITLE				   	  =1ULL<<1;
	//static	const DWORD64	T_MAINTITLE     	   	  =1ULL<<2;
	//static	const DWORD64	T_SUBTITLE			   	  =1ULL<<3;	
	//static	const DWORD64	T_TIME				   	  =1ULL<<4;
	//static	const DWORD64	T_SOURCE			   	  =1ULL<<5;
	//static	const DWORD64	T_AUTHOR			   	  =1ULL<<6;
	//static	const DWORD64	T_KEYWORD        	   	  =1ULL<<7;
	//static	const DWORD64	T_SUBJECT			   	  =1ULL<<8;
	//static	const DWORD64	T_CONTENT			   	  =1ULL<<9;
	//static	const DWORD64	T_BIGCLASS			   	  =1ULL<<10;
	//static	const DWORD64	T_SMALLCLASS		   	  =1ULL<<11;
	//static	const DWORD64	T_SERVERCOLUMNID	   	  =1ULL<<12;
	//static	const DWORD64	T_SERVERCHANNELID 	   	  =1ULL<<13;
	//static	const DWORD64	T_TOPICID        	   	  =1ULL<<14;
	//static	const DWORD64	T_PRIMARY         	   	  =1ULL<<15;
	//static	const DWORD64	T_RESOURCENUM     	   	  =1ULL<<16;
	//static	const DWORD64	T_RESOURCEID      	   	  =1ULL<<17;
	//static	const DWORD64	T_RESOURCEURL     	   	  =1ULL<<18;
	//static	const DWORD64	T_RESOURCEEXPLAIN 	   	  =1ULL<<19;
	//static	const DWORD64	T_RESOURCENAME    	   	  =1ULL<<20;
	//static	const DWORD64	T_RESOURCEEXT     	   	  =1ULL<<21;
	//static	const DWORD64	T_TIMESTAMP      	   	  =1ULL<<22;
	//static	const DWORD64	T_LINKURL        	   	  =1ULL<<23;
	//static	const DWORD64	T_THUMB          	   	  =1ULL<<24;
	//static	const DWORD64	T_ISTHUMB         	   	  =1ULL<<25;
	//static	const DWORD64	T_EDITOR          	   	  =1ULL<<26;
	//static	const DWORD64	T_PRIMARYNUM   		   	  =1ULL<<27;
	//static	const DWORD64	T_SERVERAUTOID   	   	  =1ULL<<28;
	//static	const DWORD64	T_DEFAULTVALUE  	   	  =1ULL<<29;
	//static	const DWORD64	T_RESOURCESIZE		   	  =1ULL<<30;
	//static	const DWORD64	T_OLDTITLE			      =1ULL<<31;
	//static	const DWORD64	T_OLDURL			      =1ULL<<32;

	CTopic();
	virtual ~CTopic();
	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);

	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
	
	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf=TRUE);
	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadInfo(CADORecordset &rs);	
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	
    virtual void DelSubObject(BOOL bDelFromDB =TRUE );

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);
		
	void StreamOutTxt(ostream& stream,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual TiXmlNode* Export(CString& strDir,TiXmlNode* pNode,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream

	CString GetDirectory();
	BOOL PublishToServer();
	BOOL DeleteFromServer();
    BOOL IsValid();	
	BOOL IsConverted();
    void UpdateTopic();	

	CWDMDatabase* GetDatabase();	
    BOOL Match(CString strTarget,int nType,BOOL bMatchAll);

	void AlterTopic(int nTopicUnit,int nProcessType,const CString& strValue);
private:
	//Get topic unit value 
	CString GetSqlString(CWDMTable* pTable,CWDMOperator::OperatorType type);
	CString GetTopicUnitValue(TopicUnit nUnitValue);
	CString GetResourceUnitValue(TopicUnit nUnitValue,CNewsPicture* pPicture);
    BOOL GetServerOperUnit(CString& strOperUnitName,CWDMTable* pTable,TopicUnit nUnitValue);
//	BOOL PublishToServer_New();
	BOOL PublishToServer_New2();
	void ProcessContent(CString& strContent,CString strKeyWord,int nType);
	
	CString GetUnitValue(CWDMOperatorUnit* pUnit,CADORecordset* pRS,CNewsPicture* pPicture=NULL);
	CString ProcessUnitValue(CString strValue,CWDMOperatorUnit* pUnit,TopicUnit nUnitValue,int iProcessType,CNewsPicture* pPicture=NULL);
	CString GetServerAutoID(CWDMTable* pTable,CADODatabase& ado);
	CString GetServerChannelIDByColumnID(CString strColumnID);
	CString GetServerDatabaseID();

	CString             m_strRemoteResourceDir;
	CString             m_strRemoteResourcePath;
//	CADODatabase        m_adoServer;
	CMapStringToString  m_mapCache;
public:
	TopicData  m_data;

};