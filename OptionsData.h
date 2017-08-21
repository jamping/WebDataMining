// OptionsData.h: interface for the COptionsData class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class COptionsData  
{
public:
	struct OptionsData
	{
		//General params
		BOOL          m_bAutoRun;
		BOOL          m_bOne;
		BOOL          m_bMiniToTray;
		BOOL          m_bCloseToTray;
		int           m_nOuterNetwork;
		// Network params
		int           m_nMaxMission;
		int           m_nPortNum;
		CString       m_strBroadcastIP;
		// Mysql drive name
		CString       m_strMysqlDrive;
		//Database params
		struct DBServerData
		{
			int       m_nType;									
			CString   m_strServer;  
			CString   m_strDatabase;
			CString   m_strUser;
			CString   m_strPass;
			BOOL      m_bSavePass;
			CString   m_strResourceDir;
			CString   m_strResourcePath;
//			CString   m_strRootValue;
			int       m_nPort;
			CString   m_strCode;
		};
		DBServerData  m_localDatabase;
		DBServerData  m_wdmServerDatabase;
		DBServerData  m_ServerDatabase;
        //Web data mining params
		int			  m_nTopicSaveDays;
		int           m_nPictureWidth;
		int           m_nPictureHeight;
		int           m_nInterpolation;
		int           m_nPictureMaxSize;
		int           m_nTopicTopTime;
		int           m_nTopicKeyWord;
		int           m_nTopicTopKeyWord;		
		CString       m_strTopicViewPath;
		CString       m_strTopicModifyPath;
	};

	COptionsData();
	virtual ~COptionsData();

public:
    OptionsData   m_data;
    
};