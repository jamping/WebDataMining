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
	//下载栏目所有文章
    //////////////////////////////////////////////////////////////////////////
	int i=0;
    if(m_pEntry->IsValid())
	{
		//更改栏目图标颜色
		theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetSelImg(),m_pEntry->GetSelImg());
		//下载栏目文章链接
		char* buf=NULL;
		int   nLen=-1;
		//下载主页
		CString strInfo;
		strInfo.Format("开始下载：%s-%s",m_pEntry->GetParent()->GetDesc(),m_pEntry->GetDesc());
		UpdateOutputInfo(strInfo);

		//解释URL
		//http://www.chinamil.zz/site1/xwpdxw/jrywxw/2008-01/04/jrywxw.htm
		//http://www.chinamil.zz/site1/xwpdxw/jrywxw/##4y-2m/2d##/jrywxw.htm
		CString strURL=CRegexWDM::TimeFormatStrConvert(m_pEntry->m_data.m_strURL);   
				
		m_http.Fetch(strURL,&buf,nLen);

		if(nLen>0)
		{			
			// 处理数据
			CString strBuf(buf);			
			
			free(buf);
			buf=NULL;			
			//提取文章列表有效内容
			CString strContent=CRegex::Search(strBuf,m_pEntry->m_data.m_strMatchRule,TRUE);
			//提取文章所有链接
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
				//下载每一篇文章
				CTopic* pTopic=DownloadTopic(hrefs.GetAt(i)->m_strLink,TRUE);
				if(pTopic!=NULL)
				{
					nTopicNum++;
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
					pTopic->m_data.m_strOldTitle = hrefs.GetAt(i)->m_strContent;
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
					// 自动发布的文章存入缓存
					if(m_pEntry->m_data.m_bAutoPublish)
					{
						COleDateTime time;
						if(time.ParseDateTime(pTopic->m_data.m_strTime))
						{
							//是否在设置的时间阀值内
							COleDateTimeSpan span(0,theApp.GetTopicTopTime(),0,0);
							if((COleDateTime::GetCurrentTime()-time)<span)
							{
								theApp.GetAutoPublishTopicList().Add(pTopic);
							}
						}
					}			
				}
			}
			strInfo.Format("%s-%s:下载完毕，共下载%d篇文章",m_pEntry->GetParent()->GetDesc(),
				m_pEntry->GetDesc(),nTopicNum);
			UpdateOutputInfo(strInfo);
            //清理内存
			for(i=0;i<hrefs.GetSize();i++)
				delete hrefs.GetAt(i);
			hrefs.RemoveAll();			
		}
		//恢复栏目图标颜色
		theApp.GetSourceTree()->SetItemImage(m_pEntry->GetRefItem(),m_pEntry->GetNormImg(),m_pEntry->GetSelImg());
	}
}