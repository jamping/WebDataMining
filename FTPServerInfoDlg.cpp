// FTPServerInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebDataMining.h"
#include "FTPServerInfoDlg.h"
#include "afxdialogex.h"

// CFTPServerInfoDlg dialog

IMPLEMENT_DYNAMIC(CFTPServerInfoDlg, CDialogEx)

CFTPServerInfoDlg::CFTPServerInfoDlg(CString strServer,CString strUser,CString strPass,UINT uPort,CString strDir,CWnd* pParent /* = NULL */)
	: CDialogEx(CFTPServerInfoDlg::IDD, pParent)
{
	ftp.ConnectFtp(strServer,uPort,strUser,strPass,FALSE);
	m_strDir = strDir;
}

CFTPServerInfoDlg::~CFTPServerInfoDlg()
{
}

void CFTPServerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BACK, m_btnBack);
	DDX_Control(pDX, IDC_LIST_SERVER_CONTENT, m_listContent);
	DDX_Text(pDX, IDC_EDIT_DIR, m_strDir);
}


BEGIN_MESSAGE_MAP(CFTPServerInfoDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CFTPServerInfoDlg::OnBnClickedButtonBack)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_SERVER_CONTENT, &CFTPServerInfoDlg::OnDblclkListServerContent)
END_MESSAGE_MAP()


// CFTPServerInfoDlg message handlers


BOOL CFTPServerInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_listContent.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_hBitmap=::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BACK));
	m_btnBack.SetBitmap(m_hBitmap);
//	SetFileColumns(&m_listContent);	
	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[]={
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("字节数"),LVCFMT_CENTER
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		m_listContent.InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);

	if(m_ctServImageList.Create(IDB_FILE_BITMAP,16,1,RGB(0,255,0)))
		m_listContent.SetImageList(&m_ctServImageList,LVSIL_SMALL);

	ftp.Create(this,&CFTPServerInfoDlg::ListFtpServerFiles);
	ftp.FtpFindFiles(m_strDir,CFtpManager<CFTPServerInfoDlg>::FileTypeDirectory 
		                | CFtpManager<CFTPServerInfoDlg>::FileTypeFile ,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CFTPServerInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if(m_ctServImageList.m_hImageList)
		m_ctServImageList.Detach();

	DeleteObject(m_hBitmap);
}


void CFTPServerInfoDlg::OnBnClickedButtonBack()
{
	// TODO: Add your control notification handler code here
	UpdateData();
		
	if(m_strDir.Right(1)=="/")
		return;

	m_strDir = m_strDir.Left(m_strDir.ReverseFind('/'));
	if( m_strDir.IsEmpty() )
		m_strDir += "/";

	m_listContent.DeleteAllItems();
	ftp.FtpFindFiles(m_strDir,CFtpManager<CFTPServerInfoDlg>::FileTypeDirectory 
		| CFtpManager<CFTPServerInfoDlg>::FileTypeFile ,NULL);

	UpdateData(FALSE);
}
BOOL CFTPServerInfoDlg::ListFtpServerFiles(LPVOID pThis,LPCTSTR lpListedFile,WIN32_FIND_DATA& FileInfo,LPVOID lpParam)
{
	LVITEM	lvIt;
	int nItem;
	CString szFileDate,szFileSize;
	FILETIME fileTime;
	
	lvIt.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;	

	if(FileInfo.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
	{
		nItem=0;
		lvIt.iImage=DIRECTORYICON;
	}
	else
	{
		nItem=m_listContent.GetItemCount();
		DWORD i=FileInfo.nFileSizeLow;
		if(i>1024)
		{
			szFileSize.Format("%ldKB",i/1024);
		}
		else
			szFileSize.Format("%ld",i);
		lvIt.iImage=FILEICON;
	}	
	
	lvIt.lParam=lvIt.iImage;
	lvIt.pszText=FileInfo.cFileName;
	lvIt.iSubItem=0;
	lvIt.iItem=nItem;
	int iPos=m_listContent.InsertItem(&lvIt);

	lvIt.mask=LVIF_TEXT;
	lvIt.iItem=iPos;

	fileTime=FileInfo.ftLastWriteTime;
	CTime time=CTime(fileTime);
	szFileDate=time.Format("%x");
	lvIt.pszText=szFileDate.GetBuffer();
	szFileDate.ReleaseBuffer();

	lvIt.iSubItem=1;
	m_listContent.SetItem(&lvIt);
	lvIt.pszText=szFileSize.GetBuffer();
	szFileSize.ReleaseBuffer();
	lvIt.iSubItem=2;
	m_listContent.SetItem(&lvIt);

	return TRUE;
}


void CFTPServerInfoDlg::OnDblclkListServerContent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int iItem = pNMItemActivate->iItem;
	if( iItem == -1 )
		return;

	DWORD dwFlag = m_listContent.GetItemData(iItem);
	if( dwFlag == FILEICON )
		return;

	if(m_strDir!="/")
		m_strDir += "/";
	m_strDir += m_listContent.GetItemText(iItem,0);

	m_listContent.DeleteAllItems();

	ftp.FtpFindFiles(m_strDir,CFtpManager<CFTPServerInfoDlg>::FileTypeDirectory 
		| CFtpManager<CFTPServerInfoDlg>::FileTypeFile ,NULL);

	UpdateData(FALSE);

	*pResult = 0;
}
