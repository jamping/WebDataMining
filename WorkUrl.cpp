// WorkUrl.cpp: implementation of the CWorkUrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WorkUrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkUrl::CWorkUrl(CEntry* pEntry,CRule* pRule,CStringArray& arrayUrl)
:CWork(pEntry,pRule)
{
	for(int i=0;i<arrayUrl.GetSize();i++)
	{
		m_arrayUrl.Add(arrayUrl.GetAt(i));
	}

}

CWorkUrl::~CWorkUrl()
{

}
void CWorkUrl::process()
{
	//更改栏目图标颜色
	theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetSelImg(),m_pEntry->GetSelImg());
	for(int i=0;i<m_arrayUrl.GetSize();i++)
	{
		//下载一篇文章
		CTopic* pTopic=DownloadTopic(m_arrayUrl.GetAt(i),TRUE);
		if(pTopic!=NULL)
		{
			//提取文章关键字
			try
			{
				RetriveKeyWord(pTopic);
			}
			catch (...)
			{
				UpdateOutputInfo(_T("提取关键词失败"));
			}					
			//文章添加到条目上
			pTopic->m_data.m_strOldTitle = pTopic->m_data.m_strTitle;
			pTopic->AddTo(m_pEntry);
			//下载文章所属资源
			try
			{
				DownloadResource(pTopic);
			}
			catch (...)
			{
				UpdateOutputInfo(_T("下载资源失败"));
			}

			//保存文章并显示
			try
			{
				pTopic->SaveToDB(*theApp.GetDB());
			}
			catch(...)
			{
				pTopic->LeaveFrom(m_pEntry);
				delete pTopic;
				pTopic = NULL;

				continue;
			}
			
			pTopic->ShowInTreeView(theApp.GetSourceTree(),TRUE);
			//下载的文章存入到容器
			if(theApp.GetNetworkType()==0)
			{
				theApp.GetDownloadTopicsList().Add(pTopic);
			}
		}		
	}
	//恢复栏目图标颜色
	theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetNormImg(),m_pEntry->GetSelImg());
}