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
		CString  m_strTopicID;             //����ID		
		CString  m_strEntryID;             //��ĿID
		CString  m_strURL;                 //ԭ��URL
		CString  m_strOldTitle;            //ԭ�ı���
		CString  m_strHeadTitle;           //����
		CString  m_strTitle;               //����
		CString  m_strMainTitle;           //����
		CString  m_strSubTitle;            //�ӱ���
		CString  m_strContent;             //����
		CString  m_strSource;              //��Դ
		CString  m_strTime;                //ʱ��
		CString  m_strAuthor;              //����
		CString  m_strKeyWord;             //�ؼ���
		CString  m_strSubject;             //ר���		
		BOOL     m_bIsPrimary;             //�Ƿ���Ҫ
		BOOL     m_bIsPublish;             //�Ƿ񷢲�
		BOOL     m_bIsSendto;              //�Ƿ��͵�����
		COleDateTime m_timeDownload;       //����ʱ��
	    CString  m_strServerColumnIDs;     //��������ĿIDs
		int      m_nWeight;				   //����Ȩ��
		CString  m_strBigClass;            //����
		CString  m_strSmallClass;          //С��
        CString  m_strServerAutoID;        //���淢���ɹ��󷵻صķ���������ID
		CString  m_strThumb;               //����ͼ·��

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
		T_DEFAULTVALUE = 0  ,          // ȱʡֵ
		T_SERVERAUTOID   	,          // ����ID
		T_TOPICID        	,		   // ����ID
		T_HEADTITLE         ,          // ����
		T_TITLE			    ,	       // ����
		T_OLDTITLE			,		   // ԭ����
		T_OLDURL		    ,          // ԭ����
		T_MAINTITLE     	,		   // ����
		T_SUBTITLE			,          // ����
		T_TIME				,          // ʱ��
		T_SOURCE			,	       // ��Դ
		T_AUTHOR			,		   // ����
		T_KEYWORD        	,          // �ؼ���
		T_SUBJECT			,          // ר���
		T_CONTENT			,	       // ����
		T_BIGCLASS			,		   // ����
		T_SMALLCLASS		,          // С��
		T_PRIMARY         	,          // �Ƿ���Ҫ
		T_THUMB          	,          // ����ͼ·��
		T_SERVERCOLUMNID	,          // ��������ĿID
		T_SERVERCHANNELID 	,	       // ������Ƶ��ID
		T_RESOURCEID      	,	       // ͼƬID
		T_RESOURCEURL     	,		   // ͼƬURL
		T_RESOURCEEXPLAIN 	,          // ͼƬ˵��
		T_RESOURCENAME    	,          // ͼƬ����
		T_RESOURCEEXT     	,	       // ͼƬ��չ��
		T_RESOURCENUM     	,          // ͼƬ����
		T_RESOURCESIZE		,	       // ͼƬ��С	
		T_ISTHUMB         	,	       // �Ƿ�������ͼ
		T_TIMESTAMP      	,		   // ʱ���
		T_LINKURL        	,          // ת������
		T_EDITOR          	,		   // �༭
		T_WEIGHT   		    ,          // ����Ȩ��
		T_REFERVALUE        ,          // ���û���ֵ
		T_READDATAOFDB                 // ��ȡ��������
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