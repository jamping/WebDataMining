// NewsPicture.cpp: implementation of the CNewsPicture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebDataMining.h"
#include "NewsPicture.h"
#include "PictureDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewsPicture::CNewsPicture():CBaseObject(OBJECT_TYPE_PICTURE)
{
	SetImg(8,9);
}

CNewsPicture::~CNewsPicture()
{

}
void CNewsPicture::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strPictureID=strID;

	if(strDesc == NULL)
        m_strDesc.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),m_data.m_strURL);
	else
		m_strDesc.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),strDesc);

    CCommonFunction::Replace(m_strDesc,"/","\\");

	CBaseObject::SetID(strID,strDesc);
}

BOOL CNewsPicture::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	return FALSE;
}
void CNewsPicture::DoJoin(CBaseObject *pParent)
{
	m_data.m_strTopicID=pParent->GetID();

	CBaseObject::DoJoin(this);
}		
void CNewsPicture::DoLeave(CBaseObject *pParent)
{
	m_data.m_strTopicID.Empty();
	
	CBaseObject::DoLeave(this);
}
UINT CNewsPicture::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CPictureDlg dlg(this);

	dlg.SetValue(m_data);

	nRet=dlg.DoModal();
	if( nRet== IDOK )
	{
		if(dlg.GetValue(m_data))
			SetModify();
		
		m_strDesc.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),m_data.m_strURL);
		CCommonFunction::Replace(m_strDesc,"/","\\");
	}
	return nRet;
}
BOOL CNewsPicture::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from wdm_picture where picture_id='%s'",m_data.m_strPictureID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("picture_id",m_data.m_strPictureID);
		}
		else
			rs.Edit();

		bRet&=rs.SetFieldValue("topic_id",m_data.m_strTopicID); 
		CString strURL=m_data.m_strURL;
		CCommonFunction::Replace(strURL,"\\","/");
		bRet&=rs.SetFieldValue("url",strURL); 
		bRet&=rs.SetFieldValue("old_url",m_data.m_strOldURL);
		bRet&=rs.SetFieldValue("explain",m_data.m_strExplain);       
		bRet&=rs.SetFieldValue("start_pos",(long)m_data.m_nStartPos);
		bRet&=rs.SetFieldValue("end_pos",(long)m_data.m_nEndPos); 
		bRet&=rs.SetFieldValue("picture_size",(long)m_data.m_nPictureSize);
		bRet&=rs.SetFieldValue("server_auto_id",m_data.m_strServerAutoID); 
		bRet&=rs.SetFieldValue("is_thumb",m_data.m_bThumb);

		bRet&=rs.Update();

		rs.Close();
	}
	//copy picture to wdm server
	if((theApp.GetNetworkType()==0) && theApp.DBIsOpen(CWebDataMiningApp::wdmServer))
	{
		CString strSource,strDst;
		CString strURL(m_data.m_strURL);

		CCommonFunction::Replace(strURL,"/","\\");

		strSource.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::localServer),strURL);
		strDst.Format("%s\\%s",theApp.GetFileSaveDir(CWebDataMiningApp::wdmServer),strURL);
		
		try
		{
			CCommonFunction::CreateDir(strDst);
		    CopyFile(strSource,strDst,FALSE);
		}
		catch (...)
		{
			CString str;
			str.Format("复制文件错误:%s",strSource);
			static_cast<CMainFrame*>(AfxGetMainWnd())->UpdateOutputInfo(str);
		}
		
	}
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	
BOOL CNewsPicture::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
 	return CBaseObject::LoadFromDB(db,pProgressBar);
}
BOOL CNewsPicture::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet &=rs.GetFieldValue("picture_id",m_data.m_strPictureID);
	bRet &=rs.GetFieldValue("topic_id",m_data.m_strTopicID);	
	bRet &=rs.GetFieldValue("url",m_data.m_strURL);
	CCommonFunction::Replace(m_data.m_strURL,"/","\\");
	bRet &=rs.GetFieldValue("old_url",m_data.m_strOldURL);
	bRet &=rs.GetFieldValue("explain",m_data.m_strExplain);
	bRet &=rs.GetFieldValue("start_pos",(long&)m_data.m_nStartPos);
	bRet &=rs.GetFieldValue("end_pos",(long&)m_data.m_nEndPos);
	bRet &=rs.GetFieldValue("picture_size",(long&)m_data.m_nPictureSize);
	bRet &=rs.GetFieldValue("server_auto_id",m_data.m_strServerAutoID);
	bRet &=rs.GetFieldValue("is_thumb",m_data.m_bThumb);

	SetID(m_data.m_strPictureID);

	return TRUE;
}
BOOL CNewsPicture::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from wdm_picture where picture_id='%s'",m_data.m_strPictureID);
		bRet=db.Execute(strSql);
	}
	if(bRet)
	{
		theApp.DeleteVisitedUrlMD5(m_data.m_strOldURL);
	}

	return bRet;
}	

UINT CNewsPicture::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CNewsPicture::PromptMenuPos()
{
	return 5;
}
void CNewsPicture::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,"删除图片");

	ASSERT(delID!=-1);

	int nLevel=theApp.GetUserLevel();
    pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
}
void CNewsPicture::ShowListCtrl(CListCtrlEx* pListCtrl,BOOL bSelf/* =TRUE */)
{
	CBaseObject::ShowListCtrl(pListCtrl,bSelf);

	CString strText;
	CStringArray strList;

	if(bSelf)
	{
		strText=_T("图片路径,150;图片说明,300;是否是缩略图,50");  
		pListCtrl->SetHeadings(strText);		

		strList.Add(GetDesc());
		strList.Add(m_data.m_strExplain);
		strList.Add(m_data.m_bThumb?_T("是"):_T("否"));

		pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)this);
		
	}
	else
	{
 		strText=_T("没有子对象,150");  
		pListCtrl->SetHeadings(strText);	
	}
}
TiXmlNode*  CNewsPicture::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("wdm_picture"));

	pTemp->SetAttribute(_T("picture_id"),m_data.m_strPictureID);
	pTemp->SetAttribute(_T("topic_id"),m_data.m_strTopicID);
	pTemp->SetAttribute(_T("url"),m_data.m_strURL);	
	pTemp->SetAttribute(_T("old_url"),m_data.m_strOldURL);
    pTemp->SetAttribute(_T("explain"),m_data.m_strExplain);
	pTemp->SetAttribute(_T("start_pos"),m_data.m_nStartPos);
    pTemp->SetAttribute(_T("end_pos"),m_data.m_nEndPos);
	pTemp->SetAttribute(_T("picture_size"),m_data.m_nPictureSize);
	pTemp->SetAttribute(_T("server_auto_id"),m_data.m_strServerAutoID);
	pTemp->SetAttribute(_T("is_thumb"),m_data.m_bThumb);

	pNode->LinkEndChild(pTemp);

	return pTemp;

}
