//
//  MODULE:   Ado.h
//
//	AUTHOR: Carlos Antollini 
//
//	Date: 6/28/2001 
//  
//	Version 1.2
//

#ifndef _ADO_H_
#define _ADO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma warning(disable : 4146)
// CG : In order to use this code against a different version of ADO, the appropriate
// ADO library needs to be used in the #import statement
//#import "C:Program Files\Common Files\System\ADO\msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
#import "msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
using namespace ADOCG;
#include "icrsint.h"

struct CADOFieldInfo
{
	char m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};



class CADODatabase
{
public:
	BOOL Execute(LPCTSTR lpstrExec);
	CADODatabase()
	{
		::CoInitialize(NULL);
			
		m_pConnection = NULL;
		m_strConnection = _T("");
		m_pConnection.CreateInstance(__uuidof(Connection));
	}
	
	~CADODatabase()
	{
		Close();
		m_pConnection.Release();
		m_pConnection = NULL;
		::CoUninitialize();
	}
	
	BOOL Open(LPCTSTR lpstrConnection = _T(""));
	_ConnectionPtr GetActiveConnection() {return m_pConnection;};
	DWORD GetRecordCount(_RecordsetPtr m_pRs);
	long BeginTransaction() 
		{return m_pConnection->BeginTrans();};
	long CommitTransaction() 
		{return m_pConnection->CommitTrans();};
	long RollbackTransaction() 
		{return m_pConnection->RollbackTrans();};
	BOOL IsOpen();
	void Close();
	void SetConnectionString(LPCTSTR lpstrConnection)
		{m_strConnection = lpstrConnection;};
	CString GetConnectionString()
		{return m_strConnection;};
	CString GetLastError() 
		{return m_strLastError;};
protected:
	void dump_com_error(_com_error &e);

protected:
	_ConnectionPtr m_pConnection;
	CString m_strConnection;
	CString m_strLastError;
};

class CADORecordset
{
public:
	enum cadoOpenEnum
	{
		openUnknown = 0,
		openQuery = 1,
		openTable = 2,
		openStoredProc = 3
	};

	enum cadoEditEnum
	{
		dbEditNone = 0,
		dbEditNew = 1,
		dbEdit = 2
	};
	
	enum cadoPositionEnum
	{
		positionUnknown = -1,
		positionBOF = -2,
		positionEOF = -3
	};
	
	enum cadoSearchEnum
	{	
		searchForward = 1,
		searchBackward = -1
	};

	
	BOOL SetFieldValue(int nIndex, CString strValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	BOOL SetFieldValue(int nIndex, int nValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue);
	BOOL SetFieldValue(int nIndex, long lValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
	BOOL SetFieldValue(int nIndex, double dblValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	BOOL SetFieldValue(int nIndex, COleDateTime time);
	BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);

	void CancelUpdate();
	BOOL Update();
	void Edit();
	BOOL AddNew();

	BOOL Find(LPCTSTR lpFind, int nSearchDirection = CADORecordset::searchForward);
	BOOL FindFirst(LPCTSTR lpFind);
	BOOL FindNext();

	CADORecordset()
	{
		m_pRecordset = NULL;
		m_pCmd = NULL;
		m_strQuery = _T("");
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pCmd.CreateInstance(__uuidof(Command));
		m_nEditStatus = CADORecordset::dbEditNone;
		m_nSearchDirection = CADORecordset::searchForward;
	}

	CADORecordset(CADODatabase* pAdoDatabase);

	~CADORecordset()
	{
		Close();
		m_pRecordset.Release();
		m_pCmd.Release();
		m_pRecordset = NULL;
		m_pCmd = NULL;
		m_strQuery = _T("");
		m_nEditStatus = dbEditNone;
	}

	CString GetQuery() 
		{return m_strQuery;};
	void SetQuery(LPCSTR strQuery) 
		{m_strQuery = strQuery;};
	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	DWORD GetFieldCount();

	BOOL IsOpen();
	void Close();
	BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	BOOL GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
	BOOL GetFieldValue(int nIndex, double& dbValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, long& lValue);
	BOOL GetFieldValue(int nIndex, long& lValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, int& nValue);
	BOOL GetFieldValue(int nIndex, int& nValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, CString& strValue);
	BOOL GetFieldValue(int nIndex, CString& strValue);
	BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time);
	BOOL GetFieldValue(int nIndex, COleDateTime& time);
	BOOL IsFieldNull(LPCTSTR lpFieldName);
	BOOL IsFieldNull(int nIndex);
	BOOL IsFieldEmpty(LPCTSTR lpFieldName);
	BOOL IsFieldEmpty(int nIndex);	
	BOOL IsEof()
		{return m_pRecordset->EndOfFile == VARIANT_TRUE;};
	BOOL IsEOF()
		{return m_pRecordset->EndOfFile == VARIANT_TRUE;};
	BOOL IsBof()
		{return m_pRecordset->BOF == VARIANT_TRUE;};
	BOOL IsBOF()
		{return m_pRecordset->BOF == VARIANT_TRUE;};
	void MoveFirst() 
		{m_pRecordset->MoveFirst();};
	void MoveNext() 
		{m_pRecordset->MoveNext();};
	void MovePrevious() 
		{m_pRecordset->MovePrevious();};
	void MoveLast() 
		{m_pRecordset->MoveLast();};
	BOOL Move(long lRecords, _variant_t Start);
	long GetAbsolutePage()
		{return m_pRecordset->GetAbsolutePage();};
	void SetAbsolutePage(int nPage)
		{m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);};
	long GetPageCount()
		{return m_pRecordset->GetPageCount();};
	long GetPageSize()
		{return m_pRecordset->GetPageSize();};
	void SetPageSize(int nSize)
		{m_pRecordset->PutPageSize(nSize);};
	long GetAbsolutePosition()
		{return m_pRecordset->GetAbsolutePosition();};
	void SetAbsolutePosition(int nPosition)
		{m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);};
	BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);
	BOOL SetChunk(LPCTSTR lpFieldName, CString strValue);
	
	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	CString GetLastError() {return m_strLastError;};
	void GetBookmark()
		{m_varBookmark = m_pRecordset->Bookmark;};
	BOOL SetBookmark();
	BOOL Delete();
private:
	CString IntToStr(int nVal);
	CString LongToStr(long lVal);
protected:
	_ConnectionPtr m_pConnection;
	int m_nSearchDirection;
	CString m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	CString m_strLastError;
	void dump_com_error(_com_error &e);
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCmd;
	CString m_strQuery;
};

#endif

