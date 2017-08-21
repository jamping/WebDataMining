// UnitInfoDlg.h : header file
//
#pragma once

#include "WDMOperatorUnit.h"
#include "BaseObject.h"
#include "sortlistctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CUnitInfoDlg dialog

class CUnitInfoDlg : public CDialogEx
{
// Construction
public:
	CUnitInfoDlg(CBaseObject* pOb,CWnd* pParent = NULL);   // standard constructor

	void SetValue(CWDMOperatorUnit::WDMOperatorUnitData& data);
	BOOL GetValue(CWDMOperatorUnit::WDMOperatorUnitData& data);
// Dialog Data
	//{{AFX_DATA(CUnitInfoDlg)
	enum { IDD = IDD_OPERATOR_UNIT };
	CComboBox	m_ctlUnitName;
	BOOL	m_bKey;
	CString	m_strUnitName;
	int		m_nLen;
	CSortListCtrl m_listUnitCombo;
	int     m_nProcessOrder;
	CString m_strUnitType;
	CString m_strUnitDefault;
	CString m_strUnitKey;
	CString m_strUnitIsNull;
	CString m_strUnitCombos;
	CString m_strDefaultValue;
	BOOL    m_bCache;
	CComboBox m_comboUnitName;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnitInfoDlg)
    public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected: 
    CBaseObject*   m_pOb;
	int m_nCurSelectedItem;

	typedef struct _TagFieldInfo
	{
         int         nLength;
		 CString     strTypeName;
		 CString     strDefault;
		 CString     strPrimaryKey;
		 CString     strIsNull;
	}
	FIELD_INFO,* PFIELD_INFO;

	typedef struct _TagUnitInfo
	{
		int        nUnitValue;
		CString    strDefault;		
		int        nProcess;

		_TagUnitInfo()
		{
			nUnitValue   = 0;
			strDefault   = _T("");
			nProcess     = 0;
		}
	}
	UNIT_INFO,*PUNIT_INFO;
	
	static CString unitCombos[];	
	static CString topicUnits[];	

	void ClearUnitContent();
	void ClearUnitCombos();
	void UpdateList(CString strUnitName,CTopic::TopicUnit nUnitValue,BOOL bSave);
	// Generated message map functions
	//{{AFX_MSG(CUnitInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownUnitName();
	afx_msg void OnSelchangeUnitName();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg LRESULT OnLoadComboboxData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEdit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateDefaultValue(CCmdUI* pCmdUI);
//	afx_msg void OnBnClickedCheck(UINT uID);
	afx_msg void OnUpdateButtonUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDown(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnUpdateButtonDel(CCmdUI* pCmdUI);	
	afx_msg void OnUpdateButtonAdd(CCmdUI* pCmdUI);	
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnItemchangedListUnitCombo(NMHDR *pNMHDR, LRESULT *pResult);
};