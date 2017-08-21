#pragma once

// CFTPServerInfoDlg dialog
#include "FtpManager.h"

class CFTPServerInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFTPServerInfoDlg)

public:
	CFTPServerInfoDlg(CString strServer,CString strUser,CString strPass,UINT uPort,CString strDir,CWnd* pParent = NULL);   // standard constructor
	virtual ~CFTPServerInfoDlg();

// Dialog Data
	enum { IDD = IDD_FTP_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    BOOL ListFtpServerFiles(LPVOID pThis,LPCTSTR lpListedFile,WIN32_FIND_DATA& FileInfo,LPVOID lpParam);

	HBITMAP m_hBitmap;
	CImageList m_ctServImageList;
	CButton m_btnBack;
	CListCtrl m_listContent;

	CFtpManager<CFTPServerInfoDlg> ftp;

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonBack();
	CString m_strDir;
//	afx_msg void OnItemdblclickListServerContent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListServerContent(NMHDR *pNMHDR, LRESULT *pResult);
};
