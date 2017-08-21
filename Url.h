#pragma once


const unsigned int URL_LEN	= 256;
const unsigned int HOST_LEN	= 256;

enum url_scheme {
	SCHEME_HTTP,
	SCHEME_FTP,
	SCHEME_INVALID
};

const int DEFAULT_HTTP_PORT = 80;
const int DEFAULT_FTP_PORT  = 21;

class CUrl
{
public:
	CString m_sUrl;			// Original URL
	enum url_scheme m_eScheme;	// URL scheme

	CString	m_sHost;		// Extracted hostname 
	int	m_nPort;		// Port number
	CString	m_sPath;		// Request

	/*
	// URL components (URL-quoted). 
	CString	m_sPath,
		m_sParams,
		m_sQuery,
		m_sFragment;

	// Extracted path info (unquoted). 
	CString	m_sDir,
		m_sFile;

	// Username and password (unquoted). 
	CString	m_sUser,
		m_sPasswd;
	*/

public:
	CUrl();
	~CUrl();

	//bool ParseUrl(CString strUrl);

	// break  an URL into scheme, host, port and request.
	// result as member variants
	bool ParseUrlEx(CString strUrl);
    CString MakeURL(CString strTempURL);
	// break an URL into scheme, host, port and request.
	// result url as argvs
	void ParseUrlEx(const char *url, char *protocol, int lprotocol,
			char *host, int lhost,
			char *request, int lrequest, int *port);

	// get the ip address by host name
	char *GetIpByHost(const char *host);

	bool IsValidHost(const char *ip);
	bool IsForeignHost(CString host);	
	bool IsValidIp(const char *ip);
	bool IsUnReachedUrl(const char *url);
	bool IsValidHostChar(char ch);

	static bool IsImageUrl(CString url);

private:
	CString GetSubPath(CString& strPath,BOOL bDirction=TRUE);
	void ParseScheme (const char *url);

};