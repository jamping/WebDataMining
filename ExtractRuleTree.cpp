// ExtractRuleTree.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ExtractRuleTree.h"
#include "Rule.h"
#include "RuleTree.h"
#include "InfoDlg.h"
#include "DownloadHttpFile.h"
#include "RegexWDM.h"
#include "Topic.h"
#include "NewsPicture.h"
#include <fstream>
#include "MainFrm.h"
#include "progressbar.h"
#include "ExportDlg.h"
#include "CharsetDetector.h"
#include <stack>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtractRuleTree

CExtractRuleTree::CExtractRuleTree():m_objectHelper(HELPER_RULE)
{
	m_objectHelper.CanDelete()=FALSE;
}

CExtractRuleTree::~CExtractRuleTree()
{
}

BOOL CExtractRuleTree::DeleteSelection(BOOL bDelSelf/* =TRUE */)
{
	BOOL bRet=TRUE;
	CBaseObject *pOB=GetSelectedObject();
	
	if(theApp.DBIsOpen())
	{
		if(MessageBox("ȷ��Ҫɾ����ѡ������","��ȷ��",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			bRet=pOB->DeleteFromDB(*theApp.GetDB(),bDelSelf);
		}
		else
			return bRet;
		
		if(!bRet)
		{
			MessageBox("����ɾ��ʧ��");
		}
		else
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	else
	{
		if(MessageBox("���ݿ�������Ͽ�,��������ɾ����ѡ����,������","��ȷ��",MB_YESNO|MB_ICONQUESTION) == IDYES)
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	
	return bRet;
}

void CExtractRuleTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/*=FALSE*/)
{
	CWaitCursor wait;
	//������вɼ�����
	POSITION pos=m_objectHelper.GetFirstChildPosition();
	while(pos)
	{
		CBaseObject *pChild=m_objectHelper.GetNextChild(pos);
		if(pChild != NULL)
		{
			DeleteItem(pChild->GetRefItem());
			if(pChild->CanDelete())
			{
				pChild->LeaveFrom(&m_objectHelper);
				delete pChild;
			}
		}
	}

	if(theApp.DBIsOpen() && bReadOfDB)
	{
		CADORecordset rs;
	    CString strSql,str;
		CProgressBar* pBar=NULL;
		//�����ݿ����ؽ��ɼ�������
		strSql.Format("select * from wdm_rule");
		
		if(rs.Open(theApp.GetDB()->GetActiveConnection(),strSql,CADORecordset::openQuery))
		{
			if(bShowInfo)
			{
				pBar=new CProgressBar(_T("׼����ȡ�ɼ���������"),60,5000);
			}

			CRule *pRule=NULL;
			while(!rs.IsEOF())
			{
				pRule=new CRule;
				pRule->LoadInfo(rs);
				pRule->AddTo(&m_objectHelper);              
							
				pRule->LoadFromDB(*theApp.GetDB(),pBar);
			
				rs.MoveNext();
			}
			rs.Close();
            if(bShowInfo)
			{
				pBar->Finish("���ڳ�ʼ���ɼ�����������");
				delete pBar;
				pBar=NULL;
			}			
		}
	}
     m_objectHelper.ShowInTreeView(this,TRUE);
}
BEGIN_MESSAGE_MAP(CExtractRuleTree, CCustomTree)
	//{{AFX_MSG_MAP(CExtractRuleTree)
	ON_COMMAND(ID_ROOT_NEW, OnRootNew)
	ON_COMMAND(ID_RULE_NEW, OnRuleNew)
	ON_COMMAND(ID_ROOT_SAVE, OnRootSave)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_ROOT_DELETE, OnRootDelete)
	ON_COMMAND(ID_ROOT_FRESH, OnRootFresh)
	ON_COMMAND(ID_RULE_DELETE, OnRuleDelete)
	ON_COMMAND(ID_RULE_EDIT, OnRuleEdit)
	ON_COMMAND(ID_RULE_SAVE, OnRuleSave)
	ON_COMMAND(ID_RULE_TEST, OnRuleTest)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_IMPORT, OnImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtractRuleTree message handlers

void CExtractRuleTree::OnRootNew() 
{
	// TODO: Add your command handler code here
	CRule *pRule=new CRule;
	
	if(pRule->EditObject(this,FALSE) == IDOK)
	{
		pRule->SetID(CCommonFunction::AllocateID());
		pRule->AddTo(&m_objectHelper);
		
		pRule->NewSubRules("����",0,0);
		pRule->NewSubRules("����",0,1);
		pRule->NewSubRules("����",0,2);
		pRule->NewSubRules("����",0,3);
		pRule->NewSubRules("ʱ��",0,4);
		pRule->NewSubRules("����",0,5);
		pRule->NewSubRules("��Դ",0,6);

		pRule->ShowInTreeView(this,TRUE);
		pRule->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
		delete pRule;
	
}

void CExtractRuleTree::OnRuleNew() 
{
	// TODO: Add your command handler code here
	CRuleTree *pSubRule=new CRuleTree;
	
	if(pSubRule->EditObject(this,FALSE) == IDOK)
	{
		pSubRule->SetID(CCommonFunction::AllocateID());
		pSubRule->AddTo(GetSelectedObject());
		pSubRule->ShowInTreeView(this,TRUE);
		pSubRule->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
		delete pSubRule;
}


void CExtractRuleTree::OnRootSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("����ѡ��Ľڵ�������"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("����ѡ��Ľڵ�����"),60,5000);
	CWebDataMiningApp::DBServerType type=CWebDataMiningApp::localServer;

	if(theApp.GetNetworkType()==0 && MessageBox(_T("���浽������������"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
		pBar->UpdateBar(_T("������������,��ȴ�..."),200);
        type=CWebDataMiningApp::wdmServer;

		CCommonFunction::OpenInNet();
		Sleep(5000);
		Sleep(3000);

		theApp.ConnectDB(type);
	}
    
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(type),TRUE,pBar))
	{
		if(type==CWebDataMiningApp::wdmServer)
		{
			theApp.CloseDB(type);
			CCommonFunction::OpenOutNet();
		}

		MessageBox(_T("����ɹ�!"));
	}
	else
		MessageBox(_T("����ʧ��!"));

	pBar->Finish(_T("����ڵ����"));
	delete pBar;
	pBar=NULL;
	
}

void CExtractRuleTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE && HIWORD(GetKeyState(VK_CONTROL)) )
	{
		DeleteSelection();
	}	
	CCustomTree::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExtractRuleTree::OnRootDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection(FALSE);
}

void CExtractRuleTree::OnRootFresh() 
{
	// TODO: Add your command handler code here
	InitUpdate(TRUE,TRUE);
}

void CExtractRuleTree::OnRuleDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CExtractRuleTree::OnRuleEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,!(theApp.GetUserLevel()>0))==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("�����Ѿ����£��Ƿ񱣴浽���ݿ�"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CExtractRuleTree::OnRuleSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("����ѡ��Ľڵ�������"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
    
	CProgressBar* pBar=new CProgressBar(_T("����ѡ��Ľڵ�����"),60,5000);
	
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
	{
		MessageBox(_T("����ɹ�!"));
	}
	else
		MessageBox(_T("����ʧ��!"));

	pBar->Finish(_T("����ڵ����"));
	delete pBar;
	pBar=NULL;
}

void CExtractRuleTree::OnRuleTest() 
{
	// TODO: Add your command handler code here
	UpdateData();
	
	char* buf=NULL;
	int nLen=0;
    CString strURL;

	CInfoDlg dlg;
	dlg.m_strTitle="URL";

	if(dlg.DoModal()==IDOK)
	{
		CWaitCursor wait;
		strURL=dlg.m_strName;

		CDownloadHttpFile download(static_cast<CMainFrame*>(AfxGetMainWnd())->GetOutPutWindowHWND());
		download.Fetch(strURL,&buf,nLen);
	}
	
	if(nLen>0)
	{
		CString strBuf(buf),strResultHtml;	
		CArray<HREF*,HREF*> morePageHrefs;
		CTopic* pTopic=new CTopic;
		pTopic->m_data.m_strURL=strURL;
		CRegexWDM wdm(static_cast<CRule*>(GetSelectedObject()));
		//����ת�� Added at 2019.04.11
		CCharsetDetector detecotr;
		charset_t ct = detecotr.GetCharset(buf,nLen);
		TRACE("The page code is %s\n",Charset::TypeToName(ct).c_str());

		if( ct == Charset::UNKNOWN )
		{
			//���Զ���ȡ����ʧ�ܣ���ʹ���ֶ�����ģʽ�������������
			CRule * pRule=static_cast<CRule*>(GetSelectedObject());
			if( pRule->m_data.m_nRuleEncoding == 1 )
				ct = Charset::UTF_8;
		}

		switch ( ct )
		{
		case Charset::UTF_8:
			strBuf=CChineseCodeLib::Utf8ToGBK(strBuf);
			break;
		case Charset::BIG5:
			strBuf = CChineseCodeLib::Big5ToGBK(strBuf);
			break;
		}
		//����buf
		free(buf);
		buf=NULL;
		// ��ȡ��ҳ��������
		CString strBaseUrl = CRegexWDM::GetWebPageBaseURL(strBuf);
		if( strBaseUrl.IsEmpty() )
			strBaseUrl = (pTopic)->m_data.m_strURL;
		//��ȡ��������			
		wdm.FetchTopic(strBuf,&pTopic,strResultHtml);

		CRegexWDM::FetchPictures(strResultHtml,&pTopic,strBaseUrl);
		CRegexWDM::FetchMorePages(strResultHtml,morePageHrefs,strBaseUrl);

        ofstream file;
	    file.open("result.txt");
		if(!file)
		{
			MessageBox("�����ļ�ʧ��","����",MB_OK|MB_ICONWARNING);			
		}
		else
		{
			file<<"---------------------������Ϣ----------------------"<<endl;
			file<<"��������:"<<pTopic->m_data.m_strHeadTitle<<endl;
			file<<"���±���:"<<pTopic->m_data.m_strTitle<<endl;
			file<<"���¸���:"<<pTopic->m_data.m_strSubTitle<<endl;
			file<<"��������:"<<pTopic->m_data.m_strContent<<endl;
			file<<"����ʱ��:"<<pTopic->m_data.m_strTime<<endl;
			file<<"��������:"<<pTopic->m_data.m_strAuthor<<endl;
			file<<"������Դ:"<<pTopic->m_data.m_strSource<<endl;
			
			POSITION pos=pTopic->GetFirstChildPosition();
			file<<"---------------------ͼƬ��Ϣ------------------------"<<endl;
			while (pos)
			{
				CNewsPicture* pPicture=(CNewsPicture*)pTopic->GetNextChild(pos);
				file<<"ͼƬ���ӣ�"<<pPicture->m_data.m_strURL<<endl;
				file<<"ͼƬ˵����"<<pPicture->m_data.m_strExplain<<endl;
			}
			file<<"----------------��ҳ����������Ϣ---------------------"<<endl;
			for(int i=0;i<morePageHrefs.GetSize();i++)
			{
				file<<"��ҳ����:"<<morePageHrefs[i]->m_strLink<<endl;          
				file<<"��ҳ����:"<<morePageHrefs[i]->m_strContent<<endl;
			}
            file.close();
			
		    ShellExecute(NULL,"open","result.txt",NULL,NULL,SW_SHOWNORMAL);	
		}	
		//Clear the memory		
		for(int i=0;i<morePageHrefs.GetSize();i++)
			delete morePageHrefs[i];
		
		delete pTopic;
        pTopic=NULL;
	}

}

void CExtractRuleTree::OnExport() 
{
	// TODO: Add your command handler code here
	BOOL bRet=FALSE;
	CExportDlg dlg(EXPORT_XML);
	dlg.m_strFileDir=theApp.ExportDir();
	if(dlg.DoModal()==IDOK)
	{   
		CWaitCursor wait;
		CString strDir=dlg.m_strFileDir;
		theApp.ExportDir()=strDir;

		CProgressBar bar(_T("�����ڵ���������"),60,5000);
        //װ�ظ��׽ڵ�
		std::stack<CBaseObject*> s;
		CBaseObject* pOb=GetSelectedObject()->GetParent();
		while (pOb)
		{
			s.push(pOb);
			pOb=pOb->GetParent();
		}
		TiXmlDocument doc;
		TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
		
		doc.LinkEndChild(pDecl);
		TiXmlNode* pNode=(TiXmlNode*)&doc;
		while (s.size())
		{
			pNode=s.top()->StreamOut(pNode,FALSE,&bar);
			s.pop();
		}
		GetSelectedObject()->StreamOut(pNode,TRUE,&bar);

		CString strFile;
		strFile.Format("%s\\%s_��������.xml",strDir,GetSelectedObject()->GetDesc());
		CCommonFunction::CreateDir(strFile);
		bRet=doc.SaveFile(strFile);			
		
		bar.Finish(_T("�����������"));
	}
	MessageBox(bRet?_T("�������ݵ����ɹ�"):_T("�������ݵ���ʧ��"));
}

void CExtractRuleTree::OnImport() 
{
	// TODO: Add your command handler code here
	char szFilters[]="Xml Files (*.xml)|*.xml||";
	CString strFileName;
    BOOL bRet=TRUE;

	CFileDialog fileDlg(TRUE,"","",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		CWaitCursor wait;
		strFileName= fileDlg.GetPathName();
		TiXmlDocument doc(strFileName);
		bRet&=(BOOL)doc.LoadFile();

		if(bRet)
		{
            TiXmlHandle hDoc(&doc);		    
            TiXmlElement* pElement=NULL;

			CProgressBar bar(_T("�����������"),60,5000);
			pElement=hDoc.FirstChildElement(_T("wdm_root")).ToElement();
			if(pElement)
			{
				 int nType=-1;	
                 pElement->QueryIntAttribute(_T("type"),&nType);
				 if(nType==1)
				 {
			         bRet&=GetSelectedObject()->StreamIn(pElement,TRUE,&bar);
				     GetSelectedObject()->ShowInTreeView(this,TRUE);
				 }
				 else
				 {
					 MessageBox(_T("���ݸ�ʽ��ƥ��"));
					 bRet=FALSE;
				 }			
			}
			bar.Finish(_T("�������ݵ������"));
		}
	}
	else
	{
		bRet=FALSE;
	}
	MessageBox(bRet?_T("�������ݵ���ɹ�"):_T("�������ݵ���ʧ��"));
}
