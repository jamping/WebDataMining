// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

// �˴�Ҫ����С DB ֧�֡�δѡ���κ���ͼ��

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include <atlbase.h>
#include <afxoledb.h>
#include <atlplus.h>
#include <afxsock.h>

#include "ado2.h"
#include "CommonFunction.h"
#include "AFXINET.H"
#include <set>
#include <string>
#include <fstream>
#include "Logs.h"
#include "tinyxml.h"
#include "ChineseCodeLib.h"

#include "..\htmltidy\include\tidy.h"
#include "..\htmltidy\include\buffio.h"

#ifdef _DEBUG
#define _TIDY_LIBNAME_	"tidylibd.lib"
#else
#define _TIDY_LIBNAME_	"tidylib.lib"
#endif

#pragma comment(lib,_TIDY_LIBNAME_)

#include "tagwindow\JUtility.h"
//#include "..\FLib\fool.h"
#include "..\cximage701_full\CxImage/ximage.h"

#pragma comment(lib,"png.lib")
#pragma comment(lib,"libdcr.lib")
#pragma comment(lib,"jpeg.lib")
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"tiff.lib")
#pragma comment(lib,"jasper.lib")
#pragma comment(lib,"mng.lib")
#pragma comment(lib,"jbig.lib")
#pragma comment(lib,"libpsd.lib")
#pragma comment(lib,"cximage.lib")

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif