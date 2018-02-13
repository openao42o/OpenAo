#ifndef _SC_GRID_HEADER_H_
#define _SC_GRID_HEADER_H_

#include "AtumParam.h"
#include "GridCtrl/GridCtrl.h"

// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
//#define GRID_CELL_FORMAT	((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE)

///////////////////////////////////////////////////////////////////////////////
// GRID_BAD_USER
///////////////////////////////////////////////////////////////////////////////
#define NUM_COLUMNS_BAD_USER_GRID	7

#define COL_BAD_USER_ACCOUNT_NAME	0
#define COL_BAD_USER_BLOCK_TYPE		1
#define COL_BAD_USER_PREV_BLOCK		2
#define COL_BAD_USER_START_TIME		3
#define COL_BAD_USER_END_TIME		4
#define COL_BAD_USER_ADMIN_NAME		5
#define COL_BAD_USER_REASON			6

extern const char *g_arrBadUserColumnNames[];

struct BAD_USER_INFO
{
	CString			AccountName;
	INT				BlockType;
	CString			PrevBlock;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	CString			AdminName;
	CString			Reason;
};

#define BLOCK_TYPE_NONE				0
#define BLOCK_TYPE_ACCOUNT_BLOCK	1
#define BLOCK_TYPE_CHATTING_BLOCK	2

#define BLOCK_TYPE_STRING_ACCOUNT_BLOCK		STRMSG_S_SCADMINTOOL_0029
#define BLOCK_TYPE_STRING_CHATTING_BLOCK	STRMSG_S_SCADMINTOOL_0030

///////////////////////////////////////////////////////////////////////////////
// GRID_CONNECTION_LOG
///////////////////////////////////////////////////////////////////////////////
#define NUM_COLUMNS_CONNECTION_LOG_GRID	6	// 2015-09-23 Future, added logging of MAC Address

#define COL_CONN_LOG_ACCOUNT_NAME	0
#define COL_CONN_LOG_DATE_TIME		1
#define COL_CONN_LOG_TYPE			2
#define COL_CONN_LOG_IPADDRESS		3
#define COL_CONN_LOG_SERVER_NAME	4
#define COL_CONN_LOG_MAC_ADDRESS	5	// 2015-09-23 Future, added logging of MAC Address

extern const char *g_arrConnectionLogColumnNames[];

struct CONNECTION_LOG_INFO
{
	CString			AccountName;
	ATUM_DATE_TIME	Time;
	CString			LogType;
	CString			IPAddress;
	CString			ServerName;
	CString			MACAddress;	// 2015-09-23 Future, added logging of MAC Address
};

///////////////////////////////////////////////////////////////////////////////
// GRID_USER_LOG
///////////////////////////////////////////////////////////////////////////////
#define NUM_COLUMNS_USER_LOG_GRID	6

#define COL_USER_LOG_ACCOUNT_NAME	0
#define COL_USER_LOG_CHARACTER_NAME	1
#define COL_USER_LOG_POSITION		2
#define COL_USER_LOG_DATE_TIME		3
#define COL_USER_LOG_TYPE			4
#define COL_USER_LOG_CONTENTS		5

extern const char *g_arrUserLogColumnNames[];

struct USER_LOG_INFO
{
	CString			AccountName;
	CString			CharacterName;
	CString			Position;
	ATUM_DATE_TIME	Time;
	CString			LogType;
	CString			Contents;
};

///////////////////////////////////////////////////////////////////////////////
// GRID_ITEM_LOG
///////////////////////////////////////////////////////////////////////////////
#define NUM_COLUMNS_ITEM_LOG_GRID	7

#define COL_ITEM_LOG_ACCOUNT_NAME	0
#define COL_ITEM_LOG_CHARACTER_NAME	1
#define COL_ITEM_LOG_DATE_TIME		2
#define COL_ITEM_LOG_TYPE			3
#define COL_ITEM_LOG_ITEM			4
#define COL_ITEM_LOG_ITEM_UID		5
#define COL_ITEM_LOG_CONTENTS		6

extern const char *g_arrItemLogColumnNames[];

struct ITEM_LOG_INFO
{
	CString			AccountName;
	CString			CharacterName;
	ATUM_DATE_TIME	Time;
	CString			LogType;
	CString			ItemString;
	int				ItemUID;
	CString			Contents;
};


// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
///////////////////////////////////////////////////////////////////////////////
// GRID_STAISICSMONEY
///////////////////////////////////////////////////////////////////////////////
#define NUM_COLUMNS_STAISTICSMONEY	16

#define COL_ITEM_STAISTICSMONEY_COUNT			0
#define COL_ITEM_STAISTICSMONEY_TIME			1
#define COL_ITEM_STAISTICSMONEY_SPI_GAP			2
#define COL_ITEM_STAISTICSMONEY_SPI_CURRENT		3
#define COL_ITEM_STAISTICSMONEY_WP_GAP			4
#define COL_ITEM_STAISTICSMONEY_WP_CURRENT		5
#define COL_ITEM_STAISTICSMONEY_SORB_GAP		6
#define COL_ITEM_STAISTICSMONEY_SORB_CURRENT	7
#define COL_ITEM_STAISTICSMONEY_QORB_GAP		8
#define COL_ITEM_STAISTICSMONEY_QORB_CURRENT	9
#define COL_ITEM_STAISTICSMONEY_CORB_GAP		10
#define COL_ITEM_STAISTICSMONEY_CORB_CURRENT	11
#define COL_ITEM_STAISTICSMONEY_GORB_GAP		12
#define COL_ITEM_STAISTICSMONEY_GORB_CURRENT	13
#define COL_ITEM_STAISTICSMONEY_UORB_GAP		14
#define COL_ITEM_STAISTICSMONEY_UORB_CURRENT	15

extern const char *g_arrStaisicsMoneyColumnNames[];

struct SSTAISTICSMONEY_INFO
{
	int				Function;
	int				Count;
	ATUM_DATE_TIME	Time;
	INT64			SPICurrent;
	INT64			SPIGap;
	INT64			WPCurrent;
	INT64			WPGap;
	INT64			SOrbCurrent;
	INT64			SOrbGap;
	INT64			QOrbCurrent;
	INT64			QOrbGap;
	INT64			COrbCurrent;
	INT64			COrbGap;
	INT64			GOrbCurrent;
	INT64			GOrbGap;
	INT64			UOrbCurrent;
	INT64			UOrbGap;
	BOOL			Sync;
};
// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

struct SERVERINTEGRATION_LOG_INFO
{// 2007-01-29 by dhjin, 서버 통합 로그를 위한 구조체
	INT				AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	INT				SourceDBNum;
	INT				SourceCharacterUID;
	CHAR			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				TargetDBNum;
	INT				TargetCharacterUID;
	CHAR			TargetCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	Time;
	T1<T0::FL_LOG>	byLogTy;	// 2008-03-03 by cmkwon, 서버군 통합 로그 타입 보여주기 - SERVERINTEGRATION_LOG_INFO 에 추가
};

///////////////////////////////////////////////////////////////////////////////
// CSCGridCtrl
///////////////////////////////////////////////////////////////////////////////
class CSCGridCtrl : public CGridCtrl
{
public:
	void ArrangeGrid();
	void InitGrid(int i_nColumns, const char *i_arrColName[]);

	// GRID_BAD_USER
	void InsertGridBadUser(BAD_USER_INFO *i_pBadUserInfo);
	BAD_USER_INFO GetGridBadUser(INT i_nRow);

	// GRID_CONNECTION_LOG
	void InsertGridConnectionLog(CONNECTION_LOG_INFO *i_pConnLogInfo);
	CONNECTION_LOG_INFO GetGridConnectionLog(INT i_nRow);

	// GRID_USER_LOG
	void InsertGridUserLog(USER_LOG_INFO *i_pUserLogInfo);
	USER_LOG_INFO GetGridUserLog(INT i_nRow);

	// GRID_ITEM_LOG
	void InsertGridItemLog(ITEM_LOG_INFO *i_pItemLogInfo);
	ITEM_LOG_INFO GetGridItemLog(INT i_nRow);

	// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
	// GRID_STAISTICSMONEY
	void InsertGridItemStaisticsMoney(SSTAISTICSMONEY_INFO *i_pItemLogInfo);
	SSTAISTICSMONEY_INFO GetGridStaisticsMoney(INT i_nRow);
	// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

};

#endif // _SC_GRID_HEADER_H_