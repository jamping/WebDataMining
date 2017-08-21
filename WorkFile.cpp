// WorkFile.cpp: implementation of the CWorkFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WorkFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkFile::CWorkFile(CEntry* pEntry,CRule* pRule,CString strFileDir)
:CWork(pEntry,pRule),m_strFileDir(strFileDir)
{

}

CWorkFile::~CWorkFile()
{
}

void CWorkFile::process()
{
	if(m_pEntry->m_data.m_strRuleID.IsEmpty())
	{
		UpdateOutputInfo("��ȡ������Ч");
		return;
	}
    RecurseRetriveTopic(m_strFileDir);
	//��ʾ���سɹ�������
	m_pEntry->ShowInTreeView(theApp.GetSourceTree(),TRUE);
	//�������ص�����
	try
	{	
		m_pEntry->SaveToDB(*theApp.GetDB());
	}
	catch (...)
	{
		TRACE("Saving data error\n");
	}
}

void CWorkFile::RecurseRetriveTopic(CString strFileDir)
{	
	TCHAR szOldDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szOldDir);
	if (!SetCurrentDirectory(strFileDir))
	{
		UpdateOutputInfo("����Ŀ¼ʧ��:"+strFileDir);
		return ;
	}
	//����Ŀ¼�����ļ�
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(_T("*.*"));
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (!(FileFinder.IsDirectory() || FileFinder.IsDots()))		//	�ļ�
		{
			CString strFileName,strExt;
			strFileName=FileFinder.GetFileName();
			strExt=strFileName.Right(strFileName.GetLength()-strFileName.ReverseFind('.')-1);
			if(strExt.CompareNoCase("htm")==0 || strExt.CompareNoCase("html")==0 ||
				strExt.CompareNoCase("shtml")==0 ||strExt.CompareNoCase("txt")==0 )
			{
				RetriveTopic(FileFinder.GetFilePath());
			}
		}
		else if (FileFinder.IsDirectory() && !FileFinder.IsDots())	//	Ŀ¼
		{
			RecurseRetriveTopic(FileFinder.GetFilePath());
		}
	}

	SetCurrentDirectory(szOldDir);
}