#pragma once
#include <oledb.h>


// #define STR_SOURCEDB_CNN L"Provider='sqloledb'; Data Source='localhost'; Initial Catalog='GLog'; Integrated Security='SSPI';"

class CGLogSourceDB
{
public:
	CGLogSourceDB(void);
	~CGLogSourceDB(void);

public:
	void Init();
	bool DBCnn(TCHAR * i_strSourceDBCnn);
	void DBDisCnn();
	bool GetIsDBCnn()						{return m_bIsDBCnn;}

	_RecordsetPtr 	GetRecordsetPtr()		{return m_pRecordSet;}
	_CommandPtr		GetCommandPtr()			{return m_pCommand;}

private:
	_RecordsetPtr 	m_pRecordSet;
	_ConnectionPtr	m_pConnection;
	_CommandPtr		m_pCommand;

	bool			m_bIsDBCnn;

inline void TESTHR(HRESULT x) { if FAILED(x) _com_issue_error(x); };

};
