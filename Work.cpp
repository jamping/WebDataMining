// Work.cpp: implementation of the CWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "webdatamining.h"
#include "MainFrm.h"
#include "Work.h"
#include "NewsPicture.h"
#include "RegexWDM.h"
#include "Regex.h"
#include "UniversalWDM.h"


#pragma comment(lib,"HLSSplit.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWork::CWork(CEntry* pEntry,CRule* pRule ):m_pEntry(pEntry)
{
	CMainFrame * pMainFrm=static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT_VALID(pMainFrm);
    
	m_hWnd=pMainFrm->GetOutPutWindowHWND();
	m_http.SetHWND(m_hWnd);
	
	if( pRule == NULL )
	    m_pWDM = new CUniversalWDM;
	else
		m_pWDM = new CRegexWDM(pRule);
}

CWork::~CWork()
{
	delete m_pWDM;
}
CTopic* CWork::DownloadTopic(CString strTopicURL,BOOL bMore /* =TRUE */)
{
	char* buf=NULL;
	int nLen=-1;
    CTopic* pTopic=NULL;
	CArray<HREF*,HREF*> morePageHrefs;
	
	try
	{
		//��������
		if(theApp.IsVisitedUrl(strTopicURL))
		{
			CString strInfo;
			strInfo.Format("%s �Ѿ����أ�����Ҫ��������!",strTopicURL);
            UpdateOutputInfo(strInfo);

			return NULL;
		}		
			
		for(int i=0;i<m_pEntry->m_data.m_nTryTimes;i++)
		{
			if(m_http.Fetch(strTopicURL,&buf,nLen))
				break;
			else
			{
				if(buf)
				{
					free(buf);
					buf=NULL;
				}
			}
		}		
		if(nLen>0)
		{
			CString strBuf(buf),strResultHtml;			
			//����buf
			free(buf);
			buf=NULL;
			// ��ȡ��ҳ��������
			CString strBaseUrl = CRegexWDM::GetWebPageBaseURL(strBuf);
			if( strBaseUrl.IsEmpty() )
				strBaseUrl = strTopicURL;
			//��ȡ��������			
			pTopic=new CTopic;
			pTopic->m_data.m_strURL=strTopicURL;

			BOOL bFailed = FALSE;
			
			bFailed = !m_pWDM->FetchTopic(strBuf,&pTopic,strResultHtml);
			if( bFailed && m_pWDM->GetWDMEngineType() == REGEX_WDM_ENGINE )
			{				
				// ����ʧ�ܣ����²���ͨ�ù���������ȡ
				CUniversalWDM wdm;
				wdm.FetchTopic(strBuf,&pTopic,strResultHtml);
			}			
			// ������µ���Ч��
			if( pTopic->m_data.m_strTitle.IsEmpty() )
			{
				delete pTopic;
				pTopic = NULL;
				return NULL;
			}
			// ��ȡͼƬ�ͷ�ҳ����
			CRegexWDM::FetchPictures(strResultHtml,&pTopic,strBaseUrl);
			if( bMore )
			{
				CRegexWDM::FetchMorePages(strResultHtml,morePageHrefs,strBaseUrl);
			}			
			//����ID
			pTopic->SetID(CCommonFunction::AllocateID(),pTopic->m_data.m_strTitle);					
			pTopic->m_data.m_timeDownload=COleDateTime::GetCurrentTime();
			//�������ص�ַMD5ֵ			
			theApp.SaveVisitedUrlMD5(strTopicURL);
			//���ط�ҳ����
			if(bMore)
			{			
				for(int i=0;i<morePageHrefs.GetSize();i++)
				{
					CTopic* pMorePageTopic=DownloadTopic(morePageHrefs.GetAt(i)->m_strLink,TRUE);
					if(pMorePageTopic!=NULL)
					{
						//�ƶ�ͼƬ
						POSITION pos=pMorePageTopic->GetFirstChildPosition();
						while (pos)
						{
							CBaseObject* pPicture=pMorePageTopic->GetNextChild(pos);
							if(pPicture!=NULL)
							{
								pMorePageTopic->GetRidOf(pPicture);
								pTopic->Add(pPicture);
							}
						}
						CString strTopic = CCommonFunction::GetPureText(pTopic->m_data.m_strContent);
						CString strMoreTopic = CCommonFunction::GetPureText(pMorePageTopic->m_data.m_strContent);

						BOOL bDel=FALSE;
						if(strTopic.GetLength()>0)
						{
							if(strTopic.Find(strMoreTopic)!=-1)
								bDel=TRUE;
						}
						if(bDel)
							CCommonFunction::Replace(pMorePageTopic->m_data.m_strContent,strMoreTopic,"");

						pTopic->m_data.m_strContent+=_T("\r\n")+pMorePageTopic->m_data.m_strContent;
						if(pTopic->m_data.m_strTitle.Find(_T("[��ҳ]"))==-1)
						{
							pTopic->m_data.m_strTitle+=_T("[��ҳ]");
							pTopic->ChangeDesc(pTopic->m_data.m_strTitle);
						}

						delete pMorePageTopic;
						pMorePageTopic=NULL;
					}
				}
			}				
			//�����ڴ�
			for(int i=0;i<morePageHrefs.GetSize();i++)
			{
				delete morePageHrefs.GetAt(i);
			}
			morePageHrefs.RemoveAll();
			// �ٴμ�����µ���Ч�ԣ���Ҫ�����¼�û��ͼƬ��������Ϊ�յ����
			if( pTopic->m_data.m_strContent.IsEmpty() && pTopic->IsHasChildrens() == FALSE )
			{
				delete pTopic;
				pTopic = NULL;
				return NULL;
			}
		}	
	}
	catch (...)
	{
		if(buf)
		{
			free(buf);
			buf=NULL;
		}
		for(int i=0;i<morePageHrefs.GetSize();i++)
		{
			delete morePageHrefs.GetAt(i);
		}
		morePageHrefs.RemoveAll();

	    if(pTopic!=NULL)
		{
			delete pTopic;
			pTopic=NULL;
		}
		return NULL;
	}

	return  pTopic;	
}
int CWork::DownloadResource(CTopic* pTopic)
{
	CStringArray strList;
	CUIntArray   delArray;
	
	CString str,strContent=pTopic->m_data.m_strContent;		
	
	CRegex::Search(strContent,_T("(<\\s*IMG\\s+[^>]*?src\\s*=\\s*.*?>)"),strList);
	int i=0;
	for( i=0;i<strList.GetSize();i++)
	{
		delArray.Add(1);
	}	

	BOOL bFirstPic = TRUE;
	POSITION pos=pTopic->GetFirstChildPosition();
	while (pos)
	{
        CNewsPicture * pPicture=(CNewsPicture*)pTopic->GetNextChild(pos);
		if(pPicture!=NULL)
		{
			char* buf=NULL;
			int nLen=-1;
			
			CString strPictureUrl=pPicture->m_data.m_strURL;
            //����ͼƬ
			try
			{
				if(theApp.IsVisitedUrl(strPictureUrl))
				{
					CString strInfo;
					strInfo.Format("%s �Ѿ����أ�����Ҫ��������!",strPictureUrl);
					UpdateOutputInfo(strInfo);
				}
				else
				{
					m_http.Fetch(strPictureUrl,&buf,nLen);
				}
				
				if( nLen > (theApp.GetPictureMaxSize()*1024) )
				{					
                    //����ͼƬ����
					CString strFileName,strFileExtName,strFilePath,strImg; 
					
					strFileExtName=pPicture->m_data.m_strURL.Mid(pPicture->m_data.m_strURL.ReverseFind(TCHAR('.')));
					strFileName.Format("%s%s",CCommonFunction::AllocateID(),strFileExtName);					
					
					CTime time=CTime::GetCurrentTime();	
									
					strFilePath.Format("%s\\%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),
						time.Format("%Y\\%m%d"),strFileName);

					CCommonFunction::CreateDir(strFilePath);
					
					BOOL bImageOK = FALSE;
					CFile file;
					if(file.Open(strFilePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
					{
						file.Write(buf,nLen);	
						file.Close();

						bImageOK = TRUE;	
					}					

					if(bImageOK)
					{
						// ����ͼƬ��С
						//pPicture->m_data.m_nPictureSize = nLen;
						// ����ͼƬ·��ת��
						for( i=0;i<strList.GetSize();i++)
						{
							if(strList.GetAt(i).Find(pPicture->m_data.m_strURL)!=-1)
							{
								strImg.Format("<img src=\"%s/%s\" border=0>",time.Format("%Y/%m%d"),strFileName);
								CCommonFunction::Replace(pTopic->m_data.m_strContent,strList.GetAt(i),strImg);
								delArray.SetAt(i,0);
							}
						}	
						pPicture->m_data.m_strURL.Format("%s\\%s",time.Format("%Y\\%m%d"),strFileName);
						//�������ص�ַMD5ֵ
						theApp.SaveVisitedUrlMD5(strPictureUrl);
						pPicture->m_data.m_strOldURL=strPictureUrl;
						//�ı���������
						pPicture->ChangeDesc(strFilePath);
						//��һ��ͼƬ��������ͼ800*600	
						if( bFirstPic )
						{
							bFirstPic = FALSE;
							// ����ͼ�ļ�������������������º�ͼƬ��Ϣ
							CString strNewPath,strThumbPath;

							strThumbPath.Format("%s/%s%s",time.Format("%Y/%m%d"),CCommonFunction::AllocateID(),strFileExtName);
							strNewPath.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),strThumbPath);
							CCommonFunction::Replace(strNewPath,"/","\\");

							if ( CCommonFunction::ResamplePicture(strFilePath,strNewPath,800,600,theApp.GetPictureInterpolation()) > 0 )
							{
								pTopic->m_data.m_strThumb = strThumbPath;
								pPicture->m_data.m_bThumb = TRUE;
							}
							
						}
						// ����Ԥ��ֵ����ͼƬ��С
						int nSize = CCommonFunction::ResamplePicture(strFilePath,strFilePath,
							theApp.GetPictureWidth(),theApp.GetPictureHeight(),theApp.GetPictureInterpolation());
						// save the picture size
						if( nSize > 0 )
						{
						    pPicture->m_data.m_nPictureSize = nSize;
						}
					
					}
					else
					{
						//����ʧ�ܣ�ɾ��ͼƬ��Դ
						pTopic->GetRidOf(pPicture);
						delete pPicture;
						pPicture = NULL;
					}					
					//����ڴ�
					free(buf);
					buf=NULL;						
				}
				else
				{
					if(buf)
					{
						free(buf);
						buf=NULL;
					}						
					//����ʧ�ܣ�ɾ��ͼƬ��Դ
					pTopic->GetRidOf(pPicture);
					delete pPicture;
					pPicture = NULL;
				}
			}
			catch (...)
			{
				if(buf)
				{
					free(buf);
					buf=NULL;
				
				}
				for( i=0;i<strList.GetSize();i++)
				{
					if(strList.GetAt(i).Find(pPicture->m_data.m_strURL)!=-1)
					{
						CCommonFunction::Replace(pTopic->m_data.m_strContent,strList.GetAt(i),"");
					}
				}		
				//����ʧ�ܣ�ɾ��ͼƬ��Դ
				pTopic->GetRidOf(pPicture);
				delete pPicture;	
				pPicture = NULL;
			}
		}
	}
	BOOL bAdjust=FALSE;
	for(i=0;i<delArray.GetSize();i++)
	{
		if(delArray.GetAt(i))
		{
			bAdjust=TRUE;
			CCommonFunction::Replace(pTopic->m_data.m_strContent,strList.GetAt(i),"");
		}
	}	
	if( bAdjust )
	{
		pTopic->m_data.m_strContent=CCommonFunction::GetPureText(pTopic->m_data.m_strContent,TRUE);
	}
	return 0;
}
void CWork::UpdateOutputInfo(CString strInfo)
{
    ::SendMessage(m_hWnd,WM_PROCESS_INFO,0,(LPARAM)strInfo.GetBuffer(0));
	strInfo.ReleaseBuffer();
}
void CWork::RetriveKeyWord(CTopic* pTopic)
{
    //��ȡ���¹ؼ���
	//�򿪷ִʾ��
	HANDLE hHandle = HLOpenSplit();
	if(hHandle != INVALID_HANDLE_VALUE)
	{
		int nExtra = 0;
		nExtra |= HL_CAL_OPT_KEYWORD;
		//���зִ�
		HLSplitWord(hHandle,(LPCTSTR)(pTopic->m_data.m_strTitle+pTopic->m_data.m_strContent),nExtra);			
		//��ùؼ��ʸ���
		int nKeyCount = HLGetFileKeyCnt(hHandle);
		
        if( nKeyCount > 0 )
		{			
			if(theApp.GetTopicKeyWordCount() < nKeyCount )
				nKeyCount=theApp.GetTopicKeyWordCount();
			
			SHLSegWord* pKey ;
			CString strKeyWords;
			
			for(int i=0;i<nKeyCount;i++)
			{
				//���ָ���Ĺؼ���
				pKey= HLGetFileKeyAt(hHandle,i);
				strKeyWords+=pKey->s_szWord;
				strKeyWords+=_T(" ");
			}
			strKeyWords.TrimLeft();
			strKeyWords.TrimRight();
			pTopic->m_data.m_strKeyWord=strKeyWords;
		}
		HLCloseSplit(hHandle);//�رշִʾ��
	}			
}
void CWork::RetriveTopic(CString strFilePath)
{
	char* buf=NULL;
	int nLen=-1;

	UpdateOutputInfo("��ȡ "+strFilePath);

	CFile file;
	int i=0;
	if(file.Open(strFilePath,CFile::modeRead|CFile::typeBinary))
	{
		nLen=file.GetLength();
		if(nLen<=0)
		{
			file.Close();
			return;
		}
		buf=new char[nLen+1];

		file.Read(buf,nLen);
		buf[nLen]='\0';

		CTopic* pTopic=new CTopic;
		CArray<HREF*,HREF*> morePageHrefs;

		try
		{
			CString strBuf(buf),strResultHtml;			
			//�������ڴ�
			delete [] buf;
			buf = NULL;
			// ��ȡ��ҳ��������
			CString strBaseUrl = CRegexWDM::GetWebPageBaseURL(strBuf);
			if( strBaseUrl.IsEmpty() )
				strBaseUrl = (pTopic)->m_data.m_strURL;
			//��ȡ��������			
			m_pWDM->FetchTopic(strBuf,&pTopic,strResultHtml);

			CRegexWDM::FetchPictures(strResultHtml,&pTopic,strBaseUrl);
		}
		catch (...)
		{
			delete pTopic;
			pTopic=NULL;
			if(buf)
			{
				delete []buf;
				buf=NULL;
			}			 
			file.Close();	
			UpdateOutputInfo(strFilePath+"��ȡʧ��!");

			return ;
		}
		//��������
		if(pTopic->m_data.m_strTitle.IsEmpty())
		{
			delete pTopic;
			pTopic=NULL;

			file.Close();
			UpdateOutputInfo(strFilePath+"��ȡʧ��!");

			return ;
		}
		pTopic->SetID(CCommonFunction::AllocateID(),pTopic->m_data.m_strTitle);
		pTopic->m_data.m_timeDownload=COleDateTime::GetCurrentTime();
		pTopic->AddTo(m_pEntry);
		//��ȡ����ͼƬ��Ϣ
		CStringArray strList;
		CUIntArray   delArray;
		CString str,strContent=pTopic->m_data.m_strContent;

		CRegex::Search(strContent,_T("(<\\s*IMG\\s+[^>]*?src\\s*=\\s*.*?>)"),strList);
		for( i=0;i<strList.GetSize();i++)
		{
			delArray.Add(1);
		}

		//����ͼƬ������Ŀ¼
		POSITION pos=pTopic->GetFirstChildPosition();
		while (pos)
		{
			CNewsPicture* pPicture=(CNewsPicture*)pTopic->GetNextChild(pos);
			if(pPicture!=NULL)
			{
				CString fileName,fileExt,filePath,strTemp,strImg;                
				strTemp=pPicture->m_data.m_strURL;
				fileExt=strTemp.Mid(strTemp.ReverseFind('.'));
				fileName.Format("%s%s",CCommonFunction::AllocateID(),fileExt);
				//��д������ͼƬ��Ϣ
				CTime time=CTime::GetCurrentTime();
				for( i=0;i<strList.GetSize();i++)
				{
					if(strList.GetAt(i).Find(pPicture->m_data.m_strURL)!=-1)
					{
						strImg.Format("<img src=\"%s/%s\" border=0>",time.Format("%Y/%m%d"),fileName);
						CCommonFunction::Replace(pTopic->m_data.m_strContent,strList.GetAt(i),strImg);
						delArray.SetAt(i,0);
					}
				}		 

				pPicture->m_data.m_strURL.Format("%s\\%s",time.Format("%Y\\%m%d"),fileName);
				filePath.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),pPicture->m_data.m_strURL);
				pPicture->ChangeDesc(filePath);

				CCommonFunction::CreateDir(filePath);

				CCommonFunction::Replace(strTemp,"/","\\");
				CopyFile(strFilePath.Left(strFilePath.ReverseFind('\\')+1)+strTemp,filePath,FALSE);

			}

		}
		for( i=0;i<delArray.GetSize();i++)
		{
			if(delArray.GetAt(i))
			{
				CCommonFunction::Replace(pTopic->m_data.m_strContent,strList.GetAt(i),"");
			}
		}	
		if( delArray.GetSize() > 0 )
		{
			pTopic->m_data.m_strContent=CCommonFunction::GetPureText(pTopic->m_data.m_strContent,TRUE);
		}         
		file.Close();
	}	
	else
	{
		UpdateOutputInfo(strFilePath+"��ʧ��!");
	}
}