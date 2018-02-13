// FieldODBC.h: interface for the CFieldODBC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDODBC_H__4D17CDC2_9E6E_47A5_89BE_13DD72A864E8__INCLUDED_)
#define AFX_FIELDODBC_H__4D17CDC2_9E6E_47A5_89BE_13DD72A864E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ODBCStatement.h"

class CFieldIOCP;
class CFieldODBC : public CODBCStatement  
{
public:
	CFieldODBC();
	virtual ~CFieldODBC();

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//BOOL InitFieldODBC(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD);
	BOOL InitFieldODBC(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd);
	void CleanFieldODBC(void);

	Err_t ChangeCharacterName(UID32_t i_characterUID, char *i_szOriginName, char *i_szChangeName);
	
	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 횟수를 리턴한다.
	INT CheckLoginEventDBData(UID32_t Account_UID, INT i_eventItemnumber=1);
protected:	

};

#endif // !defined(AFX_FIELDODBC_H__4D17CDC2_9E6E_47A5_89BE_13DD72A864E8__INCLUDED_)
