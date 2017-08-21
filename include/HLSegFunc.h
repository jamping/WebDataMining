//Unix��ֲ û�иĶ�  lili     dawnsun ok song ok
#ifndef __HLSEGFUNC_H__
#define __HLSEGFUNC_H__

#include "HLPubDef.h"

#ifdef WIN32
#define HLDLLIMPORT  _declspec(dllimport)
#else
#define HLDLLIMPORT  extern "C"
#endif

//�����ִ�ϵͳ���Ի�
HLDLLIMPORT bool HLSplitInit(const char* lpszDataFilePath = NULL);

//�����ִ�ϵͳж��
HLDLLIMPORT void HLFreeSplit(void);

//�򿪺����ִʾ��
HLDLLIMPORT HANDLE HLOpenSplit();

//�رպ����ִʾ��
HLDLLIMPORT void HLCloseSplit(HANDLE hHandle);

//��һ���ַ����ִ�
HLDLLIMPORT bool HLSplitWord(HANDLE hHandle , LPCTSTR lpText , int iExtraCalcFlag=0);

//��÷ִʽ������
HLDLLIMPORT int HLGetWordCnt(HANDLE hHandle);

//��ȡָ���ķִʽ��
HLDLLIMPORT SHLSegWord* HLGetWordAt(HANDLE hHandle , int nIndex);


//װ���û��Զ���ʵ�
HLDLLIMPORT bool HLOpenUsrDict(const char* lpUserDictName);

//ж���û��Զ���ʵ�
HLDLLIMPORT bool HLFreeUsrDict();

//��ȡ�ؼ��ʸ���
HLDLLIMPORT int HLGetFileKeyCnt(HANDLE hHandle);

//��ȡָ���Ĺؼ���
HLDLLIMPORT SHLSegWord* HLGetFileKeyAt(HANDLE hHandle , int iIndex);

///�������ָ��
HLDLLIMPORT bool HLGetFingerM(HANDLE hHandle, LPBYTE &rpData, DWORD &rdwLen);


#endif
