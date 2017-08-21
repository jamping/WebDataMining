// WorkEntry.cpp: implementation of the CWorkEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "WorkEntry.h"
#include "Url.h"
#include "Regex.h"
#include "RegexWDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkEntry::CWorkEntry(CEntry* pEntry,CRule* pRule):CWork(pEntry,pRule)
{

}

CWorkEntry::~CWorkEntry()
{

}
void CWorkEntry::process()
{
	//������Ŀ��������
    //////////////////////////////////////////////////////////////////////////
	int i=0;
    if(m_pEntry->IsValid())
	{
		//������Ŀͼ����ɫ
		theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetSelImg(),m_pEntry->GetSelImg());
		//������Ŀ��������
		char* buf=NULL;
		int   nLen=-1;
		//������ҳ
		CString strInfo;
		strInfo.Format("��ʼ���أ�%s-%s",m_pEntry->GetParent()->GetDesc(),m_pEntry->GetDesc());
		UpdateOutputInfo(strInfo);

		//����URL
		//http://www.chinamil.zz/site1/xwpdxw/jrywxw/2008-01/04/jrywxw.htm
		//http://www.chinamil.zz/site1/xwpdxw/jrywxw/##4y-2m/2d##/jrywxw.htm
		CString strURL=CRegexWDM::TimeFormatStrConvert(m_pEntry->m_data.m_strURL);   
				
		m_http.Fetch(strURL,&buf,nLen);

		if(nLen>0)
		{			
			// ��������
			CString strBuf(buf);			
			
			free(buf);
			buf=NULL;			
			//��ȡ�����б���Ч����
			CString strContent=CRegex::Search(strBuf,m_pEntry->m_data.m_strMatchRule,TRUE);
			//��ȡ������������
			CString strBaseURL=CRegexWDM::GetWebPageBaseURL(strBuf);

			if( strBaseURL.IsEmpty() )
				strBaseURL = strURL;

			CArray<HREF*,HREF*> hrefs;
			CRegexWDM::FetchHrefs(strContent,hrefs,strBaseURL);			

            int num=hrefs.GetSize();
			num=(num>m_pEntry->m_data.m_nTopicMax)?m_pEntry->m_data.m_nTopicMax:num;
            
			int nTopicNum=0;
			for( i=0;i<num;i++)
			{
				//����ÿһƪ����
				CTopic* pTopic=DownloadTopic(hrefs.GetAt(i)->m_strLink,TRUE);
				if(pTopic!=NULL)
				{
					nTopicNum++;
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
					pTopic->m_data.m_strOldTitle = hrefs.GetAt(i)->m_strContent;
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
					// �Զ����������´��뻺��
					if(m_pEntry->m_data.m_bAutoPublish)
					{
						COleDateTime time;
						if(time.ParseDateTime(pTopic->m_data.m_strTime))
						{
							//�Ƿ������õ�ʱ�䷧ֵ��
							COleDateTimeSpan span(0,theApp.GetTopicTopTime(),0,0);
							if((COleDateTime::GetCurrentTime()-time)<span)
							{
								theApp.GetAutoPublishTopicList().Add(pTopic);
							}
						}
					}			
				}
			}
			strInfo.Format("%s-%s:������ϣ�������%dƪ����",m_pEntry->GetParent()->GetDesc(),
				m_pEntry->GetDesc(),nTopicNum);
			UpdateOutputInfo(strInfo);
            //�����ڴ�
			for(i=0;i<hrefs.GetSize();i++)
				delete hrefs.GetAt(i);
			hrefs.RemoveAll();			
		}
		//�ָ���Ŀͼ����ɫ
		theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetNormImg(),m_pEntry->GetSelImg());
	}
}