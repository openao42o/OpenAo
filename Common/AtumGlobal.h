#if !defined(ATUMGLOBAL_H__017A08F0_77F1_4553_AF1A_FDFE85643262__INCLUDED_)
#define ATUMGLOBAL_H__017A08F0_77F1_4553_AF1A_FDFE85643262__INCLUDED_

#include "stdafx.h"
#include "math.h"
//#include "AtumParam.h"
//#include "AtumDataLoader.h"
#include <FieldWinSocket.h>
// 2007-11-22 by bhsohn 아레나 통합서버
#include <FieldWinSocketManager.h>

class CParamFactor;
class CD3DHanFont;
// 2007-11-22 by bhsohn 아레나 통합서버
//extern CFieldWinSocket			*g_pFieldWinSocket;
extern CFieldWinSocketManager	*g_pFieldWinSocket;

// 벡터 곱하기 ( (x1,y1,z1) * (x2,y2,z2) = (x1*x2, y1*y2, z1*z2)
#define MULTI_VECTOR(X,Y)		(D3DXVECTOR3(X.x*Y.x, X.y*Y.y, X.z*Y.z)) 

inline DWORD FtoDW( FLOAT f )	{ return *((DWORD*)&f); }

void	FAR PASCAL BMP_To_JPG(char *Source, char *Target);// AtumJPG.LIB

void SetParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
float GetParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam);
void ReleaseParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
void ParamFactor_plus_ParamFactor( CParamFactor &destParamFactor, CParamFactor &srcParamFactor );
void SetItem_ParamFactor( ITEM & item, CParamFactor &paramFactor );
BOOL CompareIntRange( INT_RANGE &range, INT nValue );
BOOL CompareBitFlag( USHORT nSource, USHORT nValue );
BOOL CompareItemCount( ITEM_W_COUNT &itemCount);
unsigned short GGetUnitUpgradeLevel(unsigned short i_nUnitKind);
unsigned short GGetUnitKindHexToDeimal(unsigned short i_nUnitKind);
int GetUnitTypeDecimal(unsigned short nUnitKind); 
float Random(float max, float min);
int GetCreateUnitWeapon11(int nUnitKind);
int GetCreateUnitWeapon12(int nUnitkind);	// 2006-01-02 by ispark
int GetCreateUnitEngine(int nUnitKind);
char* GetGuildRankName( BYTE nGuildRank );
int GetGearTrace(UINT nUnitKind);
void MakeCurrencySeparator(char *pDest, char *pSrc, int nSepInterval, char cSepChar);

// nDeegree is level of detail, nDefenseItemNum is SourceIndex
UINT GetUnitNum(int nDegree, int nDefenseItemNum, int nUnitKind, BOOL bCharacter);

char* GetWaterTypeAtMap( USHORT nMapIndex );
BOOL IsPkEnableMap( USHORT nMapIndex);
BOOL IsTileMapRenderEnable( USHORT nMapIndex );
// 2007-04-19 by bhsohn 충돌 처리 문제
BOOL IsTileMapTickEnable(USHORT nMapIndex );
BOOL IsSkyRenderEnable( USHORT nMapIndex );
BOOL IsSunRenderEnable( USHORT nMapIndex );
BOOL IsFogEnableMap(USHORT nMapIndex);
int GetQuestIndexForWarp(USHORT nMapIndex);
void SetFogLevel(USHORT nMapIndex,BOOL bDay);
void SetOptionFogDistance(USHORT nMapIndex,BOOL bDay, float fFogStart, float fFogEnd);
DWORD GetFogColor(USHORT nMapIndex, BOOL bDay);
D3DXVECTOR3 GetMapDirection(USHORT nMapIndex,BOOL bDay);
void CheckMouseReverse(POINT* pt);

// 2007-04-13 by bhsohn A기어 포대문제
float GetRadianVectorBetweenVector(D3DXVECTOR3 i_vecDir, D3DXVECTOR3 i_vecUp);
D3DXVECTOR3 GetRadianVector(D3DXVECTOR3 i_vecOld, D3DXVECTOR3 i_vecUp, FLOAT i_fRadianMove);

// 2005-07-29 by ispark
float GetCharacterHeight(int nPilotNum);
float GetCharacterSteps(int nPilotNum);
BOOL GetCharactorName(char* o_szCharatorName,INT i_nPilotNum);
void GetBaseChange3D(POINT pt, D3DXVECTOR3 *vOutPos, D3DXVECTOR3 *vOutDir);
// 2005-11-30 by ispark
int GetPositionInvenScreenMode();
BOOL EnableRevivalMap(USHORT nInfluenceType, USHORT nMapIndex);									// 2006-01-02 by ispark
BOOL IsNotInfluenceSameMap(USHORT nInfluenceType, USHORT nMapIndex);							// 2006-01-02 by ispark
#ifdef FREEWAR_
BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType = 1, UINT WarZone = 0);
#else
BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType = 1);	// 2006-01-07 by ispark
#endif
int GetMapItemVisibleDistance(USHORT nMapIndex);												// 2006-01-13 by ispark
int GetInvenUseEngineEffectNum(int nUnitKind);													// 2006-01-23 by ispark
int GetInvenUseEngineObjectNum(int nUnitKind);													// 2006-01-24 by ispark
int GetMusicVolume(int nRate);																	// 2006-03-22 by ispark
DWORD GetFontColor( char chr );																	// 2006-04-17 by ispark
DWORD GetFontColor(char* str);																	// 2015-03-22 by killburne
BOOL ReSetRemoveString(int nType, char *pChar);													// 2006-04-17 by ispark
float GetCollReflect(D3DXVECTOR3 *pvOut, D3DXVECTOR3 *pvVel, D3DXVECTOR3 *pvNor, BOOL	bRight = TRUE);	// 2006-05-09 by ispark
void GetInfluenceString(char* pOutStr, USHORT nMyInfluenceType);								// 2006-05-24 by ispark

// 2006-08-09 by ispark, 스트링
void StringCullingUserData(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont = NULL);	// 전체 스트링 절단내기
// 2007-11-15 by bhsohn 퀘스트설정할떄만 \n되게 변경
//BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);		// 2006-08-07 by ispark
//BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont, BOOL bChRt = FALSE);		// 2006-08-07 by ispark
BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont, BOOL bChRt = FALSE, BOOL bShop = FALSE);		// 2013-04-05 by ssjung 샾 일때 아이템 이름 글자 수 넘어 갈때 처리 보정

//BOOL StringCullingUserData2( char* strTalk, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);
//void StringCullingUserData3(char *pLineString, int nState, int nCullStringNum, vector<string> *vecStorage); // 특정문자 앞에 자르기

// 2008-01-23 by bhsohn 공백으로 글씨 짜르는 함수 생성
BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecDstString, CD3DHanFont* pFont, BOOL bChRt = FALSE); // 특정문자 앞에 자르기
char StringCullingAddVector(char chColorSave, char* pString, vector<string> *vecDstString);
// 2008-07-07 by dgwoo 아머의 높이를 리턴한다.
FLOAT GetAmorGearHeight(INT i_nAmmor, BOOL i_bSelect = FALSE);



// 2007-07-27 by bhsohn 글씨 컬링 버그 수정
// 2008-01-23 by bhsohn 공백으로 글씨 짜르는 함수 생성
char GetLineFindColor(char* pTxtTmp);				
char GetFindColorOnTxt(char* pTxtTmp);// 2008-07-24 by bhsohn 컬링 관련 버그 수정

ATUM_DATE_TIME GetServerDateTime();																// 2006-10-11 by ispark
// 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
ATUM_DATE_TIME GetArenaServerDateTime();														
// end 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
// 2009. 06. 10 by ckPark 255일 넘는 아이템 사용 시간 제대로 나오지 않는 오류 수정

//ATUM_DATE_TIME GetAtumDateTimeFormSecond(int nSecond);											// 2006-10-11 by ispark
void	GetAtumDateTimeFormSecond( const int nSecond, int* pDay, int* pHour, int* pMin, int* Sec );

// end 2009. 06. 10 by ckPark 255일 넘는 아이템 사용 시간 제대로 나오지 않는 오류 수정


// 2010. 02. 11 by ckPark 발동류 장착아이템
void	GetString_TimeLeft( DWORD nSec, char* pBuff );			// 남은 시간 nSec를 시간문자열로 pBuff
BOOL	GetString_CoolTime( CItemInfo* pItem, char* pBuff );	// pItem의 쿨타임을 시간문자열로 pBuff
// end 2010. 02. 11 by ckPark 발동류 장착아이템


char * GetStringDateTimeFormSecond(char * o_strDateTime, int nSecond);							// 2006-10-11 by ispark
BOOL CheckFontColor( char chr );																// 2006-10-23 by ispark
BOOL CheckFontColor(char* str);																	// 2015-03-22 by killburne
void GetScale(float& fXScale, float& fYScale);													// 2007-07-03 by dgwoo
MapIndex_t GetMapIndexBGM(USHORT nMapIndex);													// 2007-08-02 by dgwoo BGM의 인덱스값을 얻어온다.
DWORD GetCurrentColor(float fTime, DWORD dwType, float fTotal, float fDelay);					// 2007-09-19 by dgwoo이미지의 패이드 인 아웃등을 계산해준다.
BOOL IsMousePosClientArea();																	// 2008-01-14 by dgwoo 마우스 위치가 클라이언트 영역에 있는가?
// 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용
float FloatRangeDecimal(float fValue, int nDecimalPoint);
INT ChangeChatButtonNumber(INT i_nMenuIndex);

// 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용
float FloatSecRangeSharp(float fValue);

// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
int GetStringBuffPos(const char* pTxt, int i_nCurSelPos);
int GetStringBuffLen(const char* pTxt);		// 글씨 길이를 알아온다.
int NormalizeRect(RECT *prt);
// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

int NumDigits(int x);	// 2015-12-21 Future, int32 optimized method of determinating the number of digits in a number

/*--------------------------------------------------------------------------*/
// 2006-06-15 by ispark
// 템플릿
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::SetIntDataByVector(vector<VECTYPE> &vecInfo, int nIndex)
/// \brief		INT 자료형 입력
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
template<class VECTYPE> 
void SetIntDataByVector(vector<VECTYPE> &vecInfo, int nIndex)
{
	vector<VECTYPE>::iterator it = vecInfo.begin();

	while(it != vecInfo.end())
	{
		if(*(it) == nIndex)
		{
			return;
		}

		it++;
	}

	vecInfo.push_back(nIndex);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::SetClientIndexDataByVector(ClientIndex_t nClientIndex)
/// \brief		ClientIndex_t 자료형 입력
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
template<class VECTYPE> 
void SetClientIndexDataByVector(vector<VECTYPE> &vecInfo, ClientIndex_t nClientIndex)
{
	vector<VECTYPE>::iterator it = vecInfo.begin();

	while(it != vecInfo.end())
	{
		if(*(it) == nClientIndex)
		{
			return;
		}

		it++;
	}

	vecInfo.push_back(nClientIndex);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::SendPacketDataByVector(MessageType_t MsgType, VECTYPE &vecInfo)
/// \brief		메세지 뭉쳐서 보내기 정보 요청 보내기
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
template<class VECTYPE> 
void SendPacketDataByVector(MessageType_t MsgType, vector<VECTYPE> &vecInfo)
{
	char buffer[SIZE_MAX_PACKET];
	int nBytes = 0;
	MessageType_t *pMsgType = NULL;
	VECTYPE *pOtherInfo = NULL;

	memset(buffer, 0x00, SIZE_MAX_PACKET);

	pMsgType	= (MessageType_t*)buffer;
	pOtherInfo	= (VECTYPE*)(buffer + SIZE_FIELD_TYPE_HEADER);

	for(int i=0; i < vecInfo.size(); i++)
	{
		if(nBytes + MSG_SIZE(VECTYPE) > SIZE_MAX_PACKET)
		{
			g_pD3dApp->m_pFieldWinSocket->Write(buffer, nBytes);
			nBytes = 0;
			pMsgType	= (MessageType_t*)buffer;
			pOtherInfo	= (VECTYPE*)(buffer + SIZE_FIELD_TYPE_HEADER);
		}

		*pMsgType				= MsgType;		
		*pOtherInfo				= vecInfo[i];
		nBytes					+= MSG_SIZE(VECTYPE);		
		pMsgType				= (MessageType_t*)(buffer + nBytes);
		pOtherInfo				= (VECTYPE*)(buffer + SIZE_FIELD_TYPE_HEADER + nBytes);
	}

	if(nBytes > 0)
	{
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, nBytes);
//		DBGOUT("SendPacketDataByVector Size (%d)\n", vecInfo.size());
	}
	
	vecInfo.clear();
}
// 템플릿
/*--------------------------------------------------------------------------*/

typedef enum 
{
	APPSTATE_LOGO_LOADING,		// LOGO 로딩
	APPSTATE_LOGO,				// 최초 게임 실행시 Logo를 보여주는 상태
	APPSTATE_CONNECTING_SERVER,	// Field server로의 접속,로그인 대기 상태
	APPSTATE_GAME_LOADING,		// 최초 게임 실행시 초기화 작업
	APPSTATE_SELECT,			// select menu
	APPSTATE_CREATE,			// create menu
	APPSTATE_MAP_LOADING,		// 맵 로딩 상태
	APPSTATE_GAME,				// 게임
	APPSTATE_GAME_END_LOCK,		// 게임 종료 준비 도중
	APPSTATE_GAME_END,			// 게임 종료 준비 완료
	APPSTATE_CLOSE,				// 서버와의 연결 종료 상태
	APPSTATE_ERROR				// 게임 에러 상태	
} EnumAppState;
	// APPSTATE_GAME
	typedef enum
	{
		GEARSTATE_FLYING,			// 비행 상태
		GEARSTATE_LAND,				// 착륙 상태
		GEARSTATE_DEAD,				// 폭발 상태
		GEARSTATE_WARF,				// 워프 상태
		GEARSTATE_SHOP				// 도시/상점 메뉴 상태
	} EnumGearState;
		// GEARSTATE_FLYING
		typedef enum
		{
			FLYINGSTATE_NORMAL_FLYING,	// 보통 비행 상태
			FLYINGSTATE_ACCELATING,		// 속도 높이는 상태
			FLYINGSTATE_BOOSTER,		// 부스터 사용 상태
			FLYINGSTATE_BREAK,			// 속도 줄이는 상태
			FLYINGSTATE_AIR_BREAKING,	// 공중 정지 시도 상태
			FLYINGSTATE_AIR_BREAK		// 공중 정지 상태
		} EnumFlyingState;	
		// GEARSTATE_LAND
		typedef enum
		{
			LANDSTATE_LANDING,			// 착륙 시도 상태
			LANDSTATE_LANDED,			// 착륙 상태
			LANDSTATE_TAKEOFF,			// 이륙 상태
			LANDSTATE_MOVE,				// 움직이는 상태(A기어)
			LANDSTATE_ACCELATING,		// 속도 높이는 상태(A기어)
			LANDSTATE_BREAK				// 속도 줄이는 상태(A기어)
		} EnumLandState;
		// GEARSTATE_DEAD
		typedef enum
		{
			GAMESTATE_FALLING,			// 폭발중, 기어는 폭발,공중폭발 둘중에 선택해서 한다.
			GAMESTATE_FALLEN,			// 폭발 완료
			GAMESTATE_EXPLODING,		// 공중 폭발 중
			GAMESTATE_EXPLODED,			// 공중 폭발 완료
		} EnumDeadState;

// 2005-10-10 by ispark
// 나에게 강제 PK 건 유저 타겟
typedef struct PK_TARGET
{
	int			nTargetIndex;
} PK_TARGET;

#endif // !defined(ATUMGLOBAL_H__017A08F0_77F1_4553_AF1A_FDFE85643262__INCLUDED_)
