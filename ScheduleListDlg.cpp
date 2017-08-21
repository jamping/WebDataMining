// ScheduleListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "webdatamining.h"
#include "ScheduleListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScheduleListDlg dialog


CScheduleListDlg::CScheduleListDlg(CBaseObject* pOb,CWnd* pParent /* = NULL */)
	: CDialogEx(CScheduleListDlg::IDD, pParent),m_pSelOb(pOb)
{
	//{{AFX_DATA_INIT(CScheduleListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScheduleListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScheduleListDlg)
	DDX_Control(pDX, IDC_LIST_SCHEDULE, m_listSchedule);
	//}}AFX_DATA_MAP
}

CString CScheduleListDlg::GetScheduleDesc(const CSchedule::ScheduleData& data)
{
    CString str1,str2;

	switch(data.m_nFreqType)
	{
	case CSchedule::every_day:
        str1.Format("ÿ%d�죬",data.m_nFreqInterval);
		break;
	case CSchedule::every_week:
		str1.Format("ÿ%d�ܣ���",data.m_nFreqRecurrenceFactor);
		if ((data.m_nFreqInterval & CSchedule::sunday) ==CSchedule::sunday)
		{
			str1+="�����գ�";
		}
		if ((data.m_nFreqInterval & CSchedule::monday) ==CSchedule::monday)
		{
			str1+="����һ��";
		}
		if ((data.m_nFreqInterval & CSchedule::tuesday) ==CSchedule::tuesday)
		{
			str1+="���ڶ���";
		}
		if ((data.m_nFreqInterval & CSchedule::wednsday) ==CSchedule::wednsday)
		{
			str1+="��������";
		}
		if ((data.m_nFreqInterval & CSchedule::thursday) ==CSchedule::thursday)
		{
			str1+="�����ģ�";
		}
		if ((data.m_nFreqInterval & CSchedule::friday) ==CSchedule::friday)
		{
			str1+="�����壬";
		}
		if ((data.m_nFreqInterval & CSchedule::saturday) ==CSchedule::saturday)
		{
			str1+="��������";
		}
		break;
	case CSchedule::every_month1:
		str1.Format("ÿ%d�£��ڵ�%d�죬",data.m_nFreqRecurrenceFactor,data.m_nFreqInterval);
		break;
	case CSchedule::every_month2:
		CString weeks[]={"����һ","���ڶ�","������","������","������","������","������"};
		str1.Format("ÿ%d�£��ڵ�%d��%s��",data.m_nFreqRecurrenceFactor,data.m_nFreqRelativeInterval,weeks[data.m_nFreqInterval-1]);
		break;
	}
    switch(data.m_nFreqSubdayType)
    {
	case CSchedule::subday_once:
		str2.Format("��%sʱ�䷢��",CCommonFunction::Int2OleDateTime(data.m_nActiveStartTime,FALSE).Format("%H:%M:%S"));
		break;
	case CSchedule::subday_minute:
		str2.Format("ÿ%d����,��%s��%sʱ�䷢��",data.m_nFreqSubdayInterval,
			CCommonFunction::Int2OleDateTime(data.m_nActiveStartTime,FALSE).Format("%H:%M:%S"),
			CCommonFunction::Int2OleDateTime(data.m_nActiveEndTime,FALSE).Format("%H:%M:%S"));
		break;
	case CSchedule::subday_hour:
		str2.Format("ÿ%dСʱ,��%s��%sʱ�䷢��",data.m_nFreqSubdayInterval,
			CCommonFunction::Int2OleDateTime(data.m_nActiveStartTime,FALSE).Format("%H:%M:%S"),
			CCommonFunction::Int2OleDateTime(data.m_nActiveEndTime,FALSE).Format("%H:%M:%S"));
		break;
    }
	return str1+str2;
}

BEGIN_MESSAGE_MAP(CScheduleListDlg, CDialogEx)
	//{{AFX_MSG_MAP(CScheduleListDlg)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_NEW,OnUpdateNew)
	ON_UPDATE_COMMAND_UI(IDC_EDIT,OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(IDC_DEL,OnUpdateDel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScheduleListDlg message handlers

void CScheduleListDlg::OnNew() 
{
	// TODO: Add your control notification handler code here
	CSchedule* pSchedule=NULL;
	pSchedule=new CSchedule;

	if(pSchedule->EditObject(this,FALSE)==IDOK)
	{
		pSchedule->SetID(CCommonFunction::AllocateID(),pSchedule->m_data.m_strScheduleName);
		pSchedule->m_data.m_strObjectID=m_pSelOb->GetID();
		pSchedule->m_data.m_nObjectType=m_pSelOb->GetType();
		pSchedule->m_data.m_timeCreated=COleDateTime::GetCurrentTime();

		pSchedule->AddTo(&theApp.GetScheduleTree()->m_objectHelper);
		pSchedule->ShowInTreeView(theApp.GetScheduleTree());

		int i=m_listSchedule.GetItemCount();
	
		m_listSchedule.InsertItem(i,pSchedule->m_data.m_strScheduleName);
        m_listSchedule.SetItemText(i,1,pSchedule->GetNextRunTime().Format("%Y-%m-%d %H:%M:%S"));
		m_listSchedule.SetItemText(i,2,GetScheduleDesc(pSchedule->m_data));

		m_listSchedule.SetItemData(i,(DWORD)pSchedule);
	}
	else
		delete pSchedule;
}

void CScheduleListDlg::OnEdit() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_listSchedule.GetFirstSelectedItemPosition();
	int i=m_listSchedule.GetNextSelectedItem(pos);

	CSchedule * pSchedule=(CSchedule*)m_listSchedule.GetItemData(i);
	if(pSchedule->EditObject(this,theApp.GetUserLevel()<1)==IDOK)
	{
		m_listSchedule.SetItemText(i,0,pSchedule->m_data.m_strScheduleName);
		m_listSchedule.SetItemText(i,1,pSchedule->GetNextRunTime().Format("%Y-%m-%d %H:%M:%S"));
		m_listSchedule.SetItemText(i,2,GetScheduleDesc(pSchedule->m_data));
	}
}

void CScheduleListDlg::OnDel() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("ɾ��ѡ��ĵ��ȼƻ�,ȷ����","��ע��",MB_YESNO|MB_ICONWARNING)==IDNO)
		return ;
    
	CArray<int,int> selectedItems;
	POSITION pos=m_listSchedule.GetFirstSelectedItemPosition();
	while (pos)
	{
		int i=m_listSchedule.GetNextSelectedItem(pos);
		CSchedule * pSchedule=(CSchedule*)m_listSchedule.GetItemData(i);
		//Delete the data from database
		if(theApp.DBIsOpen())
            pSchedule->DeleteFromDB(*theApp.GetDB());
		//Delete the data from the tree
		theApp.GetScheduleTree()->DeleteItem(pSchedule->GetRefItem());
		//Delete the data from the memory
		theApp.GetScheduleTree()->m_objectHelper.GetRidOf(pSchedule);
		delete pSchedule; 	
        //save the index value in order to delete data from list ctrl
		selectedItems.Add(i);
	}

	for(int i=0;i<selectedItems.GetSize();i++)
	{
    	m_listSchedule.DeleteItem(selectedItems.GetAt(i));
		//update the other pos
		for(int j=0;j<selectedItems.GetSize();j++)
		{
			if(selectedItems.GetAt(j)>selectedItems.GetAt(i))
				selectedItems.SetAt(j,selectedItems.GetAt(j)-1);
		}
	}
}

void CScheduleListDlg::OnOK() 
{
	// TODO: Add extra validation here
	//save the data to database
	if(theApp.DBIsOpen())
	{
		for(int i=0;i<m_listSchedule.GetItemCount();i++)
		{
			CSchedule * pSchedule=(CSchedule*)m_listSchedule.GetItemData(i);			
			pSchedule->SaveToDB(*theApp.GetDB());
		}
	}
	CDialogEx::OnOK();
}

BOOL CScheduleListDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listSchedule.SetExtendedStyle(m_listSchedule.GetExtendedStyle()
		  | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT /*|LVS_EX_ONECLICKACTIVATE*/);
    
	m_listSchedule.InsertColumn(0,_T("������"),LVCFMT_LEFT,150);
	m_listSchedule.InsertColumn(1,_T("�´�����ʱ��"),LVCFMT_LEFT,150);
	m_listSchedule.InsertColumn(2,_T("��������"),LVCFMT_LEFT,300);
	CBaseObject *pOb=NULL;
	int i=0;
	POSITION pos =theApp.GetScheduleTree()->m_objectHelper.GetFirstChildPosition();
	while (pos)
	{
		pOb=theApp.GetScheduleTree()->m_objectHelper.GetNextChild(pos);
		if(pOb!=NULL)
		{
			CSchedule* pSchedule=(CSchedule*)pOb;
			if(pSchedule->m_data.m_strObjectID==m_pSelOb->GetID())
			{
				m_listSchedule.InsertItem(i,pSchedule->m_data.m_strScheduleName);
				m_listSchedule.SetItemText(i,1,pSchedule->GetNextRunTime().Format("%Y-%m-%d %H:%M:%S"));
			    m_listSchedule.SetItemText(i,2,GetScheduleDesc(pSchedule->m_data));
				m_listSchedule.SetItemData(i++,(DWORD)pSchedule);
			}
		}        
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CScheduleListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CScheduleListDlg::OnUpdateDel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_listSchedule.GetSelectedCount()>0 && theApp.GetUserLevel()>0);
}
void CScheduleListDlg::OnUpdateEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_listSchedule.GetSelectedCount()==1);
}
void CScheduleListDlg::OnUpdateNew(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(theApp.GetUserLevel()>0);
}