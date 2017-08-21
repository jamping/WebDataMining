// DBServerTree.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "DBServerTree.h"
#include "dbserver.h"
#include "WDMDatabase.h"
#include "WDMTable.h"
#include "WDMOperator.h"
#include "WDMOperatorUnit.h"
#include "progressbar.h"
#include "ExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBServerTree
CDBServerTree::CDBServerTree():m_objectHelper(HELPER_DBSERVER)
{
	m_objectHelper.CanDelete()=FALSE;
}

CDBServerTree::~CDBServerTree()
{
}

BOOL CDBServerTree::DeleteSelection(BOOL bDelSelf/* =TRUE */)
{
	BOOL bRet=TRUE;
	CBaseObject *pOB=GetSelectedObject();
	
	if(theApp.DBIsOpen())
	{
		if(MessageBox("确认要删除所选内容吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			bRet=pOB->DeleteFromDB(*theApp.GetDB(),bDelSelf);
		}
		else
			return bRet;
		
		if(!bRet)
		{
			MessageBox("数据删除失败");
		}
		else
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	else
	{
		if(MessageBox("数据库服务器断开,不能真正删除所选内容,继续吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDYES)
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	
	return bRet;
}

void CDBServerTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/* =FALSE */)
{
	CWaitCursor wait;
	//清空现有数据库服务器列表
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
		CProgressBar *pBar=NULL;
		//从数据库中重建数据库服务器列表
		strSql.Format("select * from wdm_server");
		
		if(rs.Open(theApp.GetDB()->GetActiveConnection(),strSql,CADORecordset::openQuery))
		{
			if(bShowInfo)
			{
			    pBar=new CProgressBar(_T("准备读取数据库服务器数据"),60,5000);
			}

			CDBServer *pDBServer=NULL;
			while(!rs.IsEOF())
			{
				pDBServer=new CDBServer;
				pDBServer->LoadInfo(rs);
				pDBServer->AddTo(&m_objectHelper); 
				
				pDBServer->LoadFromDB(*theApp.GetDB(),pBar);
			
				rs.MoveNext();
			}
			rs.Close();

            if(bShowInfo)
			{
			    pBar->Finish("正在初始化数据库服务器树……");
				delete pBar;
				pBar=NULL;
			}			
		}
	}
     m_objectHelper.ShowInTreeView(this,TRUE);
}


BEGIN_MESSAGE_MAP(CDBServerTree, CCustomTree)
	//{{AFX_MSG_MAP(CDBServerTree)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_ROOT_DELETE, OnRootDelete)
	ON_COMMAND(ID_ROOT_FRESH, OnRootFresh)
	ON_COMMAND(ID_ROOT_SAVE, OnRootSave)
	ON_COMMAND(ID_DBSERVER_DEL, OnDbserverDel)
	ON_COMMAND(ID_DBSERVER_EDIT, OnDbserverEdit)
	ON_COMMAND(ID_DBSERVER_SAVE, OnDbserverSave)
	ON_COMMAND(ID_DBSERVER_NEW_DB, OnDbserverNewDb)
	ON_COMMAND(ID_TABLE_DEL, OnTableDel)
	ON_COMMAND(ID_TABLE_EDIT, OnTableEdit)
	ON_COMMAND(ID_TABLE_NEW_OPERATOR, OnTableNewOperator)
	ON_COMMAND(ID_TABLE_SAVE, OnTableSave)
	ON_COMMAND(ID_DATABASE_DEL, OnDatabaseDel)
	ON_COMMAND(ID_DATABASE_EDIT, OnDatabaseEdit)
	ON_COMMAND(ID_DATABASE_NEW_TABLE, OnDatabaseNewTable)
	ON_COMMAND(ID_DATABASE_SAVE, OnDatabaseSave)
	ON_COMMAND(ID_OPERATOR_DEL, OnOperatorDel)
	ON_COMMAND(ID_OPERATOR_EDIT, OnOperatorEdit)
	ON_COMMAND(ID_OPERATOR_NEW_UNIT, OnOperatorNewUnit)
	ON_COMMAND(ID_OPERATOR_SAVE, OnOperatorSave)
	ON_COMMAND(ID_UNIT_DEL, OnUnitDel)
	ON_COMMAND(ID_UNIT_EDIT, OnUnitEdit)
	ON_COMMAND(ID_UNIT_SAVE, OnUnitSave)
	ON_COMMAND(ID_ROOT_NEW, OnRootNew)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_IMPORT, OnImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBServerTree message handlers

void CDBServerTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE && HIWORD(GetKeyState(VK_CONTROL)) )
	{
		DeleteSelection();
	}	
	CCustomTree::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDBServerTree::OnRootDelete() 
{
	// TODO: Add your command handler code here
	DeleteSelection(FALSE);
}

void CDBServerTree::OnRootFresh() 
{
	// TODO: Add your command handler code here
	InitUpdate(TRUE,TRUE);
}

void CDBServerTree::OnRootSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
		MessageBox(_T("保存成功!"));
	else
        MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CDBServerTree::OnDbserverDel() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CDBServerTree::OnDbserverEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,theApp.GetUserLevel()<1)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CDBServerTree::OnDbserverSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
		MessageBox(_T("保存成功!"));
	else
        MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CDBServerTree::OnDbserverNewDb() 
{
	// TODO: Add your command handler code here
	CWDMDatabase* pDB=new CWDMDatabase;
	//First add to parent node
	pDB->AddTo(GetSelectedObject());

	if(pDB->EditObject(this,FALSE)==IDOK)
	{
		pDB->SetID(CCommonFunction::AllocateID());
		pDB->ShowInTreeView(this,TRUE);
		pDB->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
	{
        GetSelectedObject()->GetRidOf(pDB);
		delete pDB;
	}
}

void CDBServerTree::OnTableDel() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CDBServerTree::OnTableEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,FALSE)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CDBServerTree::OnTableNewOperator() 
{
	// TODO: Add your command handler code here
	CWDMOperator* pOperator=new CWDMOperator;

	if(pOperator->EditObject(this,FALSE)==IDOK)
	{
	    pOperator->AddTo(GetSelectedObject());
		pOperator->SetID(CCommonFunction::AllocateID());
		pOperator->ShowInTreeView(this,TRUE);

		pOperator->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
	{
		delete pOperator;
	}
}

void CDBServerTree::OnTableSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
		MessageBox(_T("保存成功!"));
	else
        MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CDBServerTree::OnDatabaseDel() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CDBServerTree::OnDatabaseEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,FALSE)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CDBServerTree::OnDatabaseNewTable() 
{
	// TODO: Add your command handler code here
	CWDMTable* pTable=new CWDMTable;
	//First add to parent node
    pTable->AddTo(GetSelectedObject());

	if(pTable->EditObject(this,FALSE)==IDOK)
	{
		pTable->SetID(CCommonFunction::AllocateID());
		pTable->ShowInTreeView(this,TRUE);
		pTable->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
	{
        GetSelectedObject()->GetRidOf(pTable);
		delete pTable;
	}
}

void CDBServerTree::OnDatabaseSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
		MessageBox(_T("保存成功!"));
	else
        MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CDBServerTree::OnOperatorDel() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CDBServerTree::OnOperatorEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,FALSE)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CDBServerTree::OnOperatorNewUnit() 
{
	// TODO: Add your command handler code here
	CWDMOperatorUnit* pUnit=new CWDMOperatorUnit;
	//First add to parent node
    pUnit->AddTo(GetSelectedObject());

	if(pUnit->EditObject(this,FALSE)==IDOK)
	{
		pUnit->SetID(CCommonFunction::AllocateID());
		pUnit->ShowInTreeView(this,TRUE);
		pUnit->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
	{
        GetSelectedObject()->GetRidOf(pUnit);
		delete pUnit;
	}
}

void CDBServerTree::OnOperatorSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
		MessageBox(_T("保存成功!"));
	else
        MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CDBServerTree::OnUnitDel() 
{
	// TODO: Add your command handler code here
	DeleteSelection();
}

void CDBServerTree::OnUnitEdit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,FALSE)==IDOK &&
	   GetSelectedObject()->IsModified() &&
       MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);
	}
}

void CDBServerTree::OnUnitSave() 
{
	// TODO: Add your command handler code here
	if(MessageBox(_T("保存选择的节点内容吗"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CProgressBar* pBar=new CProgressBar(_T("保存选择的节点内容"),60,5000);
	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
		MessageBox(_T("保存成功!"));
	else
        MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存节点完毕"));
	delete pBar;
	pBar=NULL;
}

void CDBServerTree::OnRootNew() 
{
	// TODO: Add your command handler code here
	CDBServer* pServer=new CDBServer;

	if(pServer->EditObject(this,FALSE)==IDOK)
	{
		pServer->SetID(CCommonFunction::AllocateID());
        pServer->AddTo(&m_objectHelper);

		pServer->ShowInTreeView(this,TRUE);
		pServer->SaveToDB(*theApp.GetDB(),FALSE);
	}
	else
	{
		delete pServer;
	}
}

void CDBServerTree::OnExport() 
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

		CProgressBar bar(_T("导出节点所有内容"),60,5000);
        
		TiXmlDocument doc;
		TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
		
		doc.LinkEndChild(pDecl);
		TiXmlNode* pNode=(TiXmlNode*)&doc;
		
		GetSelectedObject()->StreamOut(pNode,TRUE,&bar);

		CString strFile;
		strFile.Format("%s\\%s_服务器数据.xml",strDir,GetSelectedObject()->GetDesc());
		CCommonFunction::CreateDir(strFile);
		bRet=doc.SaveFile(strFile);			
		
		bar.Finish(_T("导出数据完毕"));
	}
	MessageBox(bRet?_T("服务器数据导出成功"):_T("服务器数据导出失败"));
}

void CDBServerTree::OnImport() 
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

			CProgressBar bar(_T("导入服务器数据"),60,5000);
			pElement=hDoc.FirstChildElement(_T("wdm_root")).ToElement();
			if(pElement)
			{
				 int nType=-1;	
                 pElement->QueryIntAttribute(_T("type"),&nType);
				 if(nType==3)
				 {
			         bRet&=GetSelectedObject()->StreamIn(pElement,TRUE,&bar);
				     GetSelectedObject()->ShowInTreeView(this,TRUE);
				 }
				 else
				 {
					MessageBox(_T("数据格式不匹配"));
					bRet=FALSE;
				 }
			}
			bar.Finish(_T("服务器数据导入完毕"));
		}
	}
	else
	{
		bRet=FALSE;
	}
	MessageBox(bRet?_T("服务器数据导入成功"):_T("服务器数据导入失败"));
}
