// Interface.h: interface for the CInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACE_H__6912F02E_DF5A_4009_BE9A_D988E4C7386B__INCLUDED_)
#define AFX_INTERFACE_H__6912F02E_DF5A_4009_BE9A_D988E4C7386B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵업
#include "ChatTab_t.h"

// 게임 윈도우창 우선순위 적용 구조체
typedef struct  {
	int		nWindowIndex;		// 윈도우 인덱스
	int		nWindowOrder;		// 윈도우 처리 우선순위
} GameWindowWnd;


class CINFTarget;
class CINFGameMain;
class CINFPilotFace;
class CINFSelect;
class CINFMapLoad;
class CINFImage;
class CINFCityBase;
class CINFChannelList;
class CINFFadeEffect;
class CINFStageEffectData;
class CINFMapName;
class CGlobalImage;
class CINFCityBazaar;
class CINFCityBazaarOpen;
class CINFCityBazaarVisit;
class CINFGameArena;

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
class CINFInfinity;
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
class CINFCityInfinityFieldPopUp;
// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)


// 2007-07-04 by bhsohn 오퍼레이터 추가
class CINFOpMain;
// 2007-07-23 by bhsohn 오퍼레이터 추가작업
class CSetupConfig;

class CINFToolTip;

class DataHeader;	// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가

// 2007-07-04 by bhsohn 오퍼레이터 추가
// 오퍼레이터 모드
#define	OPERATOR_USER_HELPER_NONE			0
#define	OPERATOR_USER_HELPER_FREESKA	1
#define	OPERATOR_USER_TUTORIAL_COMPLETE		2
#define	OPERATOR_USER_TUTORIAL_FAIL			3
#define	OPERATOR_USER_TUTORIAL_HELPER		4
#define	OPERATOR_USER_TUTORIAL_ALL_COMPLETE	5
#define	OPERATOR_USER_SEL_VCU				6
#define	OPERATOR_USER_SEL_ANI				7
#define	OPERATOR_USER_TUTORIAL_NOT_L10		8



class CInterface : public CAtumNode  
{
public:
	CInterface();
	virtual ~CInterface();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void Render();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
//	void OnButtonClick(int nButton);
	
	HRESULT InitGameObjects();
	HRESULT RestoreGameObjects();
	HRESULT InvalidateGameObjects();
	// 2007-11-22 by bhsohn 아레나 통합서버
	//HRESULT DeleteGameObjects();
	HRESULT DeleteGameObjects(BOOL bArenaMode);

	HRESULT InitSelectObjects();
	HRESULT RestoreSelectObjects();
	HRESULT InvalidateSelectObjects();
	HRESULT DeleteSelectObjects();

	HRESULT	InitMapLoadObjects();
	HRESULT RestoreMapLoadObjects();
	HRESULT InvalidateMapLoadObjects();
	HRESULT DeleteMapLoadObjects();
	
	HRESULT	InitCityObjects();
	HRESULT RestoreCityObjects();
	HRESULT InvalidateCityObjects();
	HRESULT DeleteCityObjects();
	
	HRESULT	InitChannelListObjects();
	HRESULT RestoreChannelListObjects();
	HRESULT InvalidateChannelListObjects();
	HRESULT DeleteChannelListObjects();

	void InitCharacterData();
	void InitOptionData();
	BOOL SaveCharacterFile(int SlotPurpose);	// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯 파라미터 추가 ( 0 = 메인, 1 = 아레나 )
	BOOL SaveOptionFile();
	// 2007-11-22 by bhsohn 아레나 통합서버
	//BOOL LoadCharacterFile();
	BOOL LoadCharacterFile(BOOL bArenaRestart);
	
	BOOL LoadOptionFile();

	HRESULT InitSoundScript(char* szFileName, STAGE_EFFECT_DATA * pStageEffectData = NULL, BYTE byEndAct = NOT_ACTION, BOOL bSkip = TRUE);
	HRESULT RestoreSoundScript();
	HRESULT InvalidateSoundScript();
	HRESULT DeleteSoundScript();

	// 2006-07-28 by ispark, 개인상점
	void OpenBazaarShop(ITEM* pSkillItem);
	void CloseBazaarShop();
	void ReadyCloseBazaarShop(BOOL bErr = FALSE);
	void VisitBazaarShop();
	void SetBazaarLogInfo(BYTE byLogState);						// 로그
	void ErrBazaarItemList();
	BOOL SendBazaarRequestItemList(CEnemyData * pEnemy, BOOL bFlag);

	void SetShutDownStart();									// 2006-08-04 by ispark, 게임 강제 종료

	void ScriptEndAct(BYTE byEndAct);							// 2006-09-07 by ispark, 스테이지 이펙트 끝나고 해야하는 행동

	// 2007-05-11 by bhsohn 아레나 버그수정
	BOOL IsArenaStart();

	// 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리
	BOOL IsBazarOpen();

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	DataHeader * FindResource_LoadOp(char* szRcName);
	void ShowOpUserHelper(BOOL bShow, int nHelpMode, char* pTxt);


	// 2009. 01. 12 by ckPark 선전 포고 시스템
	// 선전포고시 오퍼레이터
	void ShowOpWarDeclare(BYTE Influence, ATUM_DATE_TIME MSWarStartTime, BOOL GiveUp, BYTE SelectCount);

	// end 2009. 01. 12 by ckPark 선전 포고 시스템


	void ShowOpMissionComplete(INT i_nExpOfCompensation, BYTE i_byBonusStatOfCompensation, vector<QUEST_PAY_ITEM_INFO> vecQuestPay);
	void AddInflWarMonster(char *pTxt, MapIndex_t	MapIndex, SHORT	MapInfluenceType, ATUM_DATE_TIME		CreateTime);
	void ShowOpBossMonsterSummon(BYTE byBelligerence, int nHour, int nMinute);
	void ShowOpFirstInfluenceQuest(BYTE InfluenceType);

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	int GetOperatorMode();
	void SetOperatorMode(int nMode);
	void CloseFirstUserExplain();
	void DetroySummonMonster(MapIndex_t	MapIndex);

	// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합
	VOID ShowOpWndTxt(char* pTxt);
	BOOL IsBuildingShow();
	void ShowOpWnd(BOOL bShow);

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	int GetSpeakerMode();
	void SetSpeakerMode(int nMode);

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	DataHeader * FindResource_LoadSelect(char* szRcName);

	// 2007-11-01 by bhsohn 상점 이용중에 대한 처리
	void CloseVisitShop();

	// 2007-11-19 by bhsohn 오퍼레이터 여러줄 텍스트 처리
	VOID ShowOpWndMultiTxt(vector<string> *pvecEditText);

	// 2007-12-07 by dgwoo 여단마크가 갱신되었을때 꼭 들어가야하는 함수.
	BOOL SetGuildMark(UID32_t i_nGuildUID);

	// 2007-11-22 by bhsohn 아레나 통합서버
	BOOL IsArenaReady();
	BOOL IsArenaGameState(int nState);

	// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵업
	void BackupChatInfo();		// 뺵업을 받는다.
	void RefreshChatInfo();		// 채팅 정보를 갱신한다.

	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	BOOL IsDoingStrategyWar();

	// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	void	UpdateShoptPlayingSPWarInfo();
	void	RefreshWarSPWarInfo();			// 거점전 
	void	RefreshWarMotherWarInfo();		// 모선전 

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	int		GetEtcOptionInfo(int nIdx);


	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
// 	void	GetEtcOptionInterPosValue(float* fRateX, float* fRateY, 
// 										   float* fRateWidth, float* fRateHeight, 
// 										   float* fGameWidth, float* fGameHeight);
	void	GetEtcOptionInterPosValue(float* fRateX, float* fRateY, 
							          float* fRateWidth, float* fRateHeight, 
									  float* fGameWidth, float* fGameHeight,
									  float* fChatWidth, float* fChatHeight);
	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장


	void	SetEtcOptionInfo(char* pSetupInfo, int nMode);
	void	GetEtcOptionIdx_To_String(int nIdx, char* pDst);
	void	LoadOptionEtcInfo(sOPTION_ETC	*pEtcOption);
	void	SetOptionEtcInfo(sOPTION_ETC	*pEtcOption);
	void	SaveOptionEtcInfo();
	BOOL	GetShowInterface();
	void	SetShowInterface(BOOL bShowInterface);


	// 2008-11-13 by bhsohn 조이스틱 작업
	void	InitJoystickInfo(char* pFilePath);
	void	LoadJoystickInfo(char* pFilePath);
	void	GetJoystickInfo(int i_nOptionIdx, int i_nSlotIdx, int*	o_nKeyIdx, int* o_nKeyValue);
	void	LoadFileJoysticInfo(CSetupConfig*	i_pJoySticConfig, structJoystickSetupKeyTable	*o_pJoystickSetupKeyTable);	// 옵션정보 새로 고침
	void	RefreshJoystickOption();
	void	SetJoysticInfoBehavior(int i_nBehaviorIdx, int i_nJoyKeyIndex, int i_nJoyKeyValue);
	int		GetJoysticInfoString_To_Idx(char* i_pTitle);
	int		GetCullingString(int nPos, char* i_pString, char i_chFind, char* o_pDst);
	BOOL	GetJostickStats(int code);
	void	InitJoystickSetupInfo(structJoystickSetupKeyTable		*o_pJoystickSetupKeyTable);
	BOOL	IsChangeJoySitckInfo(int code);
	void	GetJoysticInfoIdx_To_String(int nIdx, char* o_pDst);
	BOOL	OnJoystickKeyDown(int i_nCode);
	void	LoadJoysticOptionInfo(structJoyStickOptionInfo* pJoysticOp);
	void	SaveJoystickOptionInfo(structJoyStickOptionInfo* pJoysticOp);
	structJoystickSetupKeyTable	 *GetJoystickSetupKeyTable(int nKeyIndex);
	void	SaveJoysticSetupInfo(char* pJoyKeyFile, structJoyStickOptionInfo	*pstruJoyStickOptionInfo, structJoystickSetupKeyTable	*i_pJoystickSetupKeyTable);
	

	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 타입 String값 리턴.
	char *GetString_PetType ( const INT a_nPetType );

	void ResetMapNameList();

	// 2013-06-26 by bhsohn 아머 컬렉션 추가 개발
	BOOL GetSetupInfo(char* i_pTitle, char* o_pTxt);
	void SetSetupInfo(char* i_pTitle, char* o_pTxt);
	// END 2013-06-26 by bhsohn 아머 컬렉션 추가 개발
	
public:
	CINFTarget			*	m_pTarget ;
	CINFGameMain		*	m_pGameMain;
	CINFPilotFace		*	m_pPilotFace;
	CINFMapName			*	m_pMapNameImg;
	BOOL					m_bShowInterface;
	CGlobalImage		*	m_pGameGlobalImage;

	CINFSelect			*	m_pSelect;
	D3DLIGHT9				m_light0;
	D3DLIGHT9				m_light1;

	CINFMapLoad			*	m_pMapLoad;
	CINFImage			*	m_pNormalMouse[2];

	CINFCityBase		*	m_pCityBase;
	CINFChannelList		*	m_pChannelList;
	CINFFadeEffect		*	m_pFadeEffect;
	CINFStageEffectData *	m_pStageEffectData;		// 성우 스크립트
	
	CINFCityBazaar		*	m_pBazaarShop;			// 2006-07-25 by ispark, 개인 상점
	CINFGameArena		*	m_pGameArena;

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	CINFInfinity		*	m_pInfinity;
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	CINFCityInfinityFieldPopUp*		m_pInfinityPopup;
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	CINFOpMain			*	m_pINFOpMain;

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	CSetupConfig*			m_pSetupConfig;	
	CINFToolTip	*			m_pToolTip;

	// 2008-11-13 by bhsohn 조이스틱 작업
	CSetupConfig*			m_pJoySticConfig;	
	structJoystickKey			m_struJoystickOption[MAX_JOSTICK_OPTION][MAX_JOSTICK_OPTION_BEHAVIOR];
	structJoystickSetupKeyTable		*m_pJoystickSetupKeyTable;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CGameData				*	m_pGameData;
	CGameData				*	m_pGameBigIconData;
#endif
	//////////////////////////////////////////////////////////////////////////
	// 이하 윈도우 창 우선 순위 렌더링, 메세지 처리 적용
	// ydkim 2005.10.28
	//
	// 메세지 입력 구조체 사용 우선순위와 메시지 INDEX 입력.
	// 구조체 형식은 Int형 변수 2개사용 
	// 추후 추가 및 변경
	// m_vecGameWindowWnd			정방향 저장(렌더링용)
	// m_vecGameWindowWndReverse	역방향 저장(처리용)
	//////////////////////////////////////////////////////////////////////////
	vector<GameWindowWnd>	m_vecGameWindowWnd;
	vector<GameWindowWnd>	m_vecGameWindowWndReverse;
	void SetWindowOrder(int nWndIndex);
	int  GetWindowFirstOrder();
	void InitWindowsFirstFormat();

	int  WindowsWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void WindowsRender();

	// 게임 전체 Rendering
	void RenderOrderWindows(int nRenderNum);
	void RenderProcMapName();

	// 게임 전체 WndProc
	int WndProcGlob(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcGame(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcCity(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcBaza(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcInfW(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMiss(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcFAQs(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//int WndProcVoIP(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMini(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMp3P(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 2008-07-10 by bhsohn 시스템 창 우선순위 추가
	int WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//
	//////////////////////////////////////////////////////////////////////////
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	int WndProcOperator(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	//////////////////////////////////////////////////////////////////////////	
	// 상점 아이템 착용검사 (추후 수정)
	void SetShopItemUseSerch();
	void SetLeaveShopKind(int nLeaveBuildingIndex);
	//
	//////////////////////////////////////////////////////////////////////////

	// 2007-10-01 by bhsohn 처음 튜토리얼 종료시 프롤로그 안나오는 현상 처리
	void SendTutorialChartEnd();

	// 2007-11-28 by bhsohn 스크린 샷 모드 추가
	BOOL IsScreenShotMode();
	void SetScreenShotMode(BOOL bScreenShotMode);

	// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
	void SetDummyDataHeader(DataHeader *pDataHeader);
	DataHeader *GetDummyDataHeader(char* i_pFileName);

protected:
	void GameShutDownTick();

	// 2007.04.24 by bhsohn China IME Working
	BOOL IsPermissionIME(HKL hkl);	

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	void InitSetupInfo(char* pPath);

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	void	RenderWndIntfaceHideMode();

	// 2008-10-27 by bhsohn 무한으로 보급 가능한 버그 수정
	void	ReleaseBazaarShop();

private:
	// 2007-11-28 by bhsohn 스크린 샷 모드 추가
	BOOL		m_bScreenShotMode;

	// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵업
	ChatLine_t	m_chatBackup[CHAT_BUFFER_NUMBER];

	// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
	DataHeader			*m_pDummyDataHeader;
	// end 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
};


// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
BOOL IsElapsedTime ( DWORD dwTime , DWORD *p_dwElapsedTime );
// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

// 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.
BOOL GetStrPara ( char *pStrBuff , INT iNum , char pKey , char *pGetPara );
// 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.

#endif // !defined(AFX_INTERFACE_H__6912F02E_DF5A_4009_BE9A_D988E4C7386B__INCLUDED_)