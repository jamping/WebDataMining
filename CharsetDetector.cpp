
//
#include "stdafx.h"
#include "CharsetDetector.h"

CCharsetDetector::CCharsetDetector(void)
	:m_detectedCharset(Charset::UNKNOWN)
	,m_ready(0)
{
}

CCharsetDetector::~CCharsetDetector(void)
{
}

charset_t CCharsetDetector::GetCharset(const char* pBuf, size_t len)
{
	HandleData(pBuf,len);
	DataEnd();
	if (m_ready) 
	{
		return m_detectedCharset;
	}
	//could't find a confident Charset			
	return Charset::UNKNOWN;
}
void CCharsetDetector::Report(const char * aCharset) 
{
	m_detectedCharset = Charset::NameToType(aCharset);
	m_ready = 1;
}
