// Logs.h: interface for the CLogs class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CLogs  
{
public:
	CLogs();
	virtual ~CLogs();
    
	enum Log_type
	{
		log_unknow=0,
		log_login,
		log_logout,
		log_download,
		log_publish_topic,
		log_delete_topic,
		log_open_db,
		log_close_db,
		log_system,
		log_admin
	};
	static void WriteLogs(CString strLogs,Log_type nType=log_unknow,CString strSource = _T(""));
};