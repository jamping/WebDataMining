//CharseDetector.h

#ifndef _CHARSETDETECTOR_H
#define _CHARSETDETECTOR_H

#include "universalcharset/nsUniversalDetector.h"
#include "universalcharset/Charset.h"

class CCharsetDetector : public nsUniversalDetector
{
public:
	CCharsetDetector(void);
	~CCharsetDetector(void);
public:
	charset_t GetCharset(const char* pBuf, size_t len);

protected:
	void		Report(const char * aCharset);
protected:
	charset_t	m_detectedCharset;
	int					m_ready;
};

#endif
