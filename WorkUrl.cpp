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
	//������Ŀͼ����ɫ
	theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetSelImg(),m_pEntry->GetSelImg());
	for(int i=0;i<m_arrayUrl.GetSize();i++)
	{
		//����һƪ����
		CTopic* pTopic=DownloadTopic(m_arrayUrl.GetAt(i),TRUE);
		if(pTopic!=NULL)
		{
			//��ȡ���¹ؼ���
			try
			{
				RetriveKeyWord(pTopic);
			}
			catch (...)
			{
				UpdateOutputInfo(_T("��ȡ�ؼ���ʧ��"));
			}					
			//������ӵ���Ŀ��
			pTopic->m_data.m_strOldTitle = pTopic->m_data.m_strTitle;
			pTopic->AddTo(m_pEntry);
			//��������������Դ
			try
			{
				DownloadResource(pTopic);
			}
			catch (...)
			{
				UpdateOutputInfo(_T("������Դʧ��"));
			}

			//�������²���ʾ
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
			//���ص����´��뵽����
			if(theApp.GetNetworkType()==0)
			{
				theApp.GetDownloadTopicsList().Add(pTopic);
			}
		}		
	}
	//�ָ���Ŀͼ����ɫ
	theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetNormImg(),m_pEntry->GetSelImg());
}