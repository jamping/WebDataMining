// WebDataMiningDoc.h : interface of the CWebDataMiningDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWebDataMiningDoc : public CDocument
{
protected: // create from serialization only
	CWebDataMiningDoc();
	DECLARE_DYNCREATE(CWebDataMiningDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebDataMiningDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWebDataMiningDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
 
// Generated message map functions
protected:
	//{{AFX_MSG(CWebDataMiningDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
