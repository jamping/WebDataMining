/* URL handling
 */

#include "stdafx.h"
#include "WebDataMining.h"
#include "Url.h"

//#include <iostream>
#include <Winsock.h>

/* Is X "."?  */
#define DOTP(x) ((*(x) == '.') && (!*(x + 1)))
/* Is X ".."?  */
#define DDOTP(x) ((*(x) == '.') && (*(x + 1) == '.') && (!*(x + 2)))


struct scheme_data
{
	char *leading_string;
	int default_port;
	int enabled;
};

/* Supported schemes: */
static struct scheme_data supported_schemes[] =
{
	{ "http://",  DEFAULT_HTTP_PORT,  1 },
	{ "ftp://",   DEFAULT_FTP_PORT,   1 },

	/* SCHEME_INVALID */
	{ NULL,       -1,                 0 }
};

/* Returns the scheme type if the scheme is supported, or
   SCHEME_INVALID if not.  */
void CUrl::ParseScheme (const char *url)
{
	int i;

	for (i = 0; supported_schemes[i].leading_string; i++)

		if (0 == _strnicmp (url, supported_schemes[i].leading_string,
                          strlen (supported_schemes[i].leading_string))) {

			if (supported_schemes[i].enabled){
				this->m_eScheme = (enum url_scheme) i;
				return;
			}else{
				this->m_eScheme = SCHEME_INVALID;
				return;
			}
		}

	this->m_eScheme = SCHEME_INVALID;
	return;
}

/************************************************************************
 *  Function name: ParseUrlEx
 *  Input argv:
 *  	-- strUrl: url
 *  Output argv:
 *  	--
 *  Return:
   	true: success
   	false: fail
 *  Fucntion Description: break an URL into scheme, host, port and request.
 *  			result as member variants
 *  Be careful:	release the memory by the client
************************************************************************/
bool CUrl::ParseUrlEx(CString strUrl)
{
	char protocol[10];
	char host[HOST_LEN];
	char request[256];
	int port = -1;

	memset( protocol, 0, sizeof(protocol) );
	memset( host, 0, sizeof(host) );
	memset( request, 0, sizeof(request) );

	this->ParseScheme((LPCSTR)strUrl);
	if( this->m_eScheme != SCHEME_HTTP ){
		return false;
	}

	ParseUrlEx((LPCSTR)strUrl,
			protocol, sizeof(protocol),
			host, sizeof(host),
			request, sizeof(request),
			&port);

	m_sUrl  = strUrl;
	m_sHost = host;
	m_sPath = request;

	if( port > 0 ){
		m_nPort = port;
	}

	return true;
}
CString CUrl::MakeURL(CString strTempURL)
{
	/********************************************************************
	http://www.zzb.kj/zgwNews/list.aspx?cid=205
    show.aspx?id=706871&amp;cid=205
    http://www.zzb.kj/zgwNews/show.aspx?id=706871&amp;cid=205

    http://21.144.241.13/site1/default/yearmonth/date/default.htm
    ../../../xwpdxw/2006-12/14/content_676618.htm
    http://21.144.241.13/site1/xwpdxw/2006-12/14/content_676618.htm
	*********************************************************************/
	if(strTempURL.IsEmpty())
		return _T("");

    if(strTempURL.Find("http://")!=-1)
		return strTempURL;
  
	CString strRet=_T("");    

    if(strTempURL.GetAt(0)=='/')
	{
		strRet.Format("http://%s%s",m_sHost,strTempURL);
		return strRet;
	}

	CString strSubPath,strNewObject;

	strNewObject=m_sPath;

    do
	{
		strSubPath=GetSubPath(strTempURL);

		if(strSubPath.Compare(_T(".."))==0)
		{
			GetSubPath(strNewObject,FALSE);
		}
		else if(strSubPath.Compare(_T("."))==0)
		{
			continue;
		}
		else
		{
            GetSubPath(strNewObject,FALSE);			
			break;
		}
	} 
	while(!strSubPath.IsEmpty());

	if(strSubPath.IsEmpty())
		strRet.Format("http://%s%s/%s",m_sHost,strNewObject,strTempURL);
	else
		strRet.Format("http://%s%s/%s/%s",m_sHost,strNewObject,strSubPath,strTempURL);
	
	CCommonFunction::Replace(strRet,"\\","/");
	
	return strRet;
}
CString CUrl::GetSubPath(CString& strPath,BOOL bDirction/* =TRUE */)
{
// 	 http://21.144.241.13/site1/default/yearmonth/date/default.htm
//     ../../../xwpdxw/2006-12/14/content_676618.htm
//     http://21.144.241.13/site1/xwpdxw/2006-12/14/content_676618.htm
	CString strRet=_T("");

	int i=bDirction?strPath.Find('/'):strPath.ReverseFind('/');
	if(i!=-1)
	{
		strRet=bDirction?strPath.Left(i):strPath.Mid(i+1);
		strPath=bDirction?strPath.Mid(i+1):strPath.Left(i);
	}

	return strRet;
}
/************************************************************************
 *  Function name: ParseUrlEx
 *  Input argv:
 *  	-- url: host name
 *  	-- protocol: result protocol
 *  	-- lprotocol: protocol length
 *  	-- host: result host
 *  	-- lhost: host length
 *  	-- request: result request
 *  	-- lrequest: request length
 *  Output argv:
 *  	--
 *  Return:
   	true: success
   	false: fail
 *  Fucntion Description: break an URL into scheme, host, port and request.
 *  			result as argvs
 *  Be careful:
************************************************************************/
void CUrl::ParseUrlEx(const char *url,
		char *protocol, int lprotocol,
		char *host, int lhost,
		char *request, int lrequest,
		int *port)
{
	char *work,*ptr,*ptr2;

	*protocol = *host = *request = 0;
	*port = 80;

	int len = strlen(url);
	//pthread_mutex_lock(&mutexMemory);
	work = new char[len + 1];
	//pthread_mutex_unlock(&mutexMemory);
	memset(work, 0, len+1);
	strncpy(work, url, len);

	// find protocol if any
	ptr = strchr(work, ':');
	if( ptr != NULL ){
		*(ptr++) = 0;
		strncpy( protocol, work, lprotocol );
	} else {
		strncpy( protocol, "HTTP", lprotocol );
		ptr = work;
	}

	// skip past opening /'s
	if( (*ptr=='/') && (*(ptr+1)=='/') )
		ptr+=2;

	// find host
	ptr2 = ptr;
	while( IsValidHostChar(*ptr2) && *ptr2 )
		ptr2++;
	*ptr2 = 0;
	strncpy( host, ptr, lhost );

	// find the request
	int offset = ptr2 - work;
	const char *pStr = url + offset;
	strncpy( request, pStr, lrequest );

	// find the port number, if any
	ptr = strchr( host, ':' );
	if( ptr != NULL ){
		*ptr = 0;
		*port = atoi(ptr+1);
	}

	//pthread_mutex_lock(&mutexMemory);
	delete [] work;
	//pthread_mutex_unlock(&mutexMemory);
	work = NULL;
}


/* scheme://user:pass@host[:port]... 
 *                    ^              
 * We attempt to break down the URL into the components path,
 * params, query, and fragment.  They are ordered like this:
 * scheme://host[:port][/path][;params][?query][#fragment] 
 */

/*
bool CUrl::ParseUrl(CString strUrl)
{
	CString::size_type idx;

	this->ParseScheme(strUrl.c_str());	
	if( this->m_eScheme != SCHEME_HTTP )
		return false;

	// get host name
	this->m_sHost = strUrl.substr(7);
	idx = m_sHost.find('/');
	if(idx != CString::npos){
		m_sHost = m_sHost.substr(0,idx);
	}

	this->m_sUrl = strUrl;

	return true;
}
*/

CUrl::CUrl()
{
	this->m_sUrl = ""; 
	this->m_eScheme= SCHEME_INVALID;
        
	this->m_sHost = "";  
	this->m_nPort = DEFAULT_HTTP_PORT; 
        
	this->m_sPath = "";
	/*
	this->m_sParams = "";
	this->m_sQuery = "";
	this->m_sFragment = "";

	this->m_sDir = "";
	this->m_sFile = "";
        
        this->m_sUser = "";
	this->m_sPasswd = "";
	*/

}

CUrl::~CUrl()
{

}


/****************************************************************************
 *  Function name: GetIpByHost
 *  Input argv:
 *  	-- host: host name
 *  Output argv:
 *  	--
 *  Return:
   	ip: sucess
   	NULL: fail
 *  Function Description: get the ip address by host name
 *  Be careful: release the memory by the client
****************************************************************************/
char * CUrl::GetIpByHost(const char *host)
{
	if( !host ){	// null pointer
		return NULL;
	}

	if( !IsValidHost(host) ){	// invalid host
		return NULL;
	}

	unsigned long inaddr = 0;
	char *result = NULL;
	int len = 0;


	inaddr = (unsigned long)inet_addr( host );
	if ( inaddr != INADDR_NONE){ // host is just ip
		len = strlen(host);
		//pthread_mutex_lock(&mutexMemory);
		result = new char[len+1];
		//pthread_mutex_unlock(&mutexMemory);
		memset(result, 0, len+1);
		memcpy(result, host, len);

		return result;

        } 
// 	else {
// 		//firt find from cache
// 		
// 		map<CString,CString>::iterator it  = mapCacheHostLookup.find(host);
// 
// 		if( it != mapCacheHostLookup.end() ){	// find in host lookup cache
// 			const char * strHostIp;
// 
// 			strHostIp = (*it).GetBuffer(0);
// 			(*it).ReleaseBuffer();
// 
// 			inaddr = (unsigned long)inet_addr( strHostIp );
// 			if ( inaddr != INADDR_NONE ){ 
// 				len = strlen(strHostIp);
// 				//pthread_mutex_lock(&mutexMemory);
// 				result = new char[len+1];
// 				//pthread_mutex_unlock(&mutexMemory);
// 				memset( result, 0, len+1 );
// 				memcpy( result, strHostIp, len );
// 				
// 				return result;
//         		}
// 		}
//	}
	return result;
}

/**********************************************************************************
 *  Function name: IsValidHostChar
 *  Input argv:
 *  	-- ch: the character for testing
 *  Output argv:
 *  	-- 
 *  Return:
   	true: is valid
   	false: is invalid
 *  Function Description: test the specified character valid
 *  			for a host name, i.e. A-Z or 0-9 or -.:
**********************************************************************************/
bool CUrl::IsValidHostChar(char ch)
{
	return( isalpha(ch) || isdigit(ch)
		|| ch=='-' || ch=='.' || ch==':' || ch=='_');
}

/**********************************************************************************
 *  Function name: IsValidHost
 *  Input argv:
 *  	-- ch: the character for testing
 *  Output argv:
 *  	-- 
 *  Return:
   	true: is valid
   	false: is invalid
 *  Function Description: test the specified character valid
 *  			for a host name, i.e. A-Z or 0-9 or -.:
 *  Be careful:
**********************************************************************************/
bool CUrl::IsValidHost(const char *host)
{
	if( !host ){
		return false;
	}

	if( strlen(host) < 6 ){ // in case host like "www", "pku", etc.
		return false;
	}

	char ch;
	for(unsigned int i=0; i<strlen(host); i++){
		ch = *(host++);
		if( !IsValidHostChar(ch) ){
			return false;
		}
	}

	return true;
}



/**********************************************************************************
 *  Function name: IsValidIp
 *  Input argv:
 *  	-- ip: ip
 *  Output argv:
 *  	-- 
 *  Return:
   	true: inside the ip block
   	false: outside the ip block
 *  Function Description: decide teh ip whether or not inside the ip block
 *  Be careful:
**********************************************************************************/
bool CUrl::IsValidIp(const char *ip)
{
	if( ip == NULL ){
		return false;
	}

	unsigned long inaddr = (unsigned long)inet_addr(ip);
	if( inaddr == INADDR_NONE ){	// invalid ip
		return false;
	}

// 	if( mapIpBlock.size() > 0 ){
// 		map<unsigned long,unsigned long>::iterator pos;
// 		for(pos=mapIpBlock.begin(); pos!=mapIpBlock.end(); ++pos){
// 			unsigned long ret;
// 
// 			ret = inaddr & ~((*pos).second);
// 			if( ret == (*pos).first ){	// inside
// 				return true;
// 			}
// 		}
// 
// 		// outside
// 		return false;
// 	}

	// if block range is not given, we think it inside also
	return true;
}

/*
 * If it is, return true; otherwise false
 * not very precise
 */
bool CUrl::IsForeignHost(CString host)
{
	if( host.IsEmpty() ) return true;
	if( host.GetLength() > HOST_LEN ) return true;

	unsigned long inaddr = 0;

	inaddr = (unsigned long)inet_addr((LPCSTR) host);
	if ( inaddr != INADDR_NONE){ // host is just ip
		return false;
	}

	int idx = host.Find('.');
	CString tmp;
	if( idx != -1 ){
		tmp = host.Mid(idx+1);
	}

	tmp.MakeLower();
	const char *home_host[] ={
		"cn","com","net","org","info",
		"biz","tv","cc", "hk", "tw",
		"zc","zz","zh","zb",
		"sy","bj","lz","jn","nj","gz","cd",
		"hj","kj","ep","wj","jk","mtn"
	};

	int home_host_num = sizeof(home_host)/sizeof(home_host[0]);

	for(int i=0; i<home_host_num; i++){
		if( tmp == home_host[i] )
			return false;
	}

	return true;
}
	
	
bool CUrl::IsImageUrl(CString url)
{
	if( url.IsEmpty() ) return false;
	if( url.GetLength() > HOST_LEN ) return false;

	int idx = url.ReverseFind('.');
	CString tmp;
	if( idx != -1 ){
		tmp = url.Mid(idx+1);
	}

	tmp.MakeLower();
	const char *image_type[] ={"jpg","jpeg","png","bmp"/*,"gif","tif","psd",*/};

	int image_type_num = sizeof(image_type)/sizeof(image_type[0]);

	for (int i=0; i<image_type_num; i++)
	{
		if( tmp == image_type[i] )
			return true;
	}

	return false;
}
