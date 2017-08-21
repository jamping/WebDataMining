//Unix移植 没有改动  lili     dawnsun ok song ok
#ifndef __HLSEGFUNC_H__
#define __HLSEGFUNC_H__

#include "HLPubDef.h"

#ifdef WIN32
#define HLDLLIMPORT  _declspec(dllimport)
#else
#define HLDLLIMPORT  extern "C"
#endif

//海量分词系统初试化
HLDLLIMPORT bool HLSplitInit(const char* lpszDataFilePath = NULL);

//海量分词系统卸载
HLDLLIMPORT void HLFreeSplit(void);

//打开海量分词句柄
HLDLLIMPORT HANDLE HLOpenSplit();

//关闭海量分词句柄
HLDLLIMPORT void HLCloseSplit(HANDLE hHandle);

//对一段字符串分词
HLDLLIMPORT bool HLSplitWord(HANDLE hHandle , LPCTSTR lpText , int iExtraCalcFlag=0);

//获得分词结果个数
HLDLLIMPORT int HLGetWordCnt(HANDLE hHandle);

//获取指定的分词结果
HLDLLIMPORT SHLSegWord* HLGetWordAt(HANDLE hHandle , int nIndex);


//装载用户自定义词典
HLDLLIMPORT bool HLOpenUsrDict(const char* lpUserDictName);

//卸载用户自定义词典
HLDLLIMPORT bool HLFreeUsrDict();

//获取关键词个数
HLDLLIMPORT int HLGetFileKeyCnt(HANDLE hHandle);

//获取指定的关键词
HLDLLIMPORT SHLSegWord* HLGetFileKeyAt(HANDLE hHandle , int iIndex);

///获得语义指纹
HLDLLIMPORT bool HLGetFingerM(HANDLE hHandle, LPBYTE &rpData, DWORD &rdwLen);


#endif
