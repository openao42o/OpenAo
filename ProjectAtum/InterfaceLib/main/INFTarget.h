// INFTarget.h: interface for the CINFTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFTARGET_H__1E2FD209_BB49_4CB7_ADDA_5BE689875601__INCLUDED_)
#define AFX_INFTARGET_H__1E2FD209_BB49_4CB7_ADDA_5BE689875601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"


#define VEL_ATTACK_MOUSE_ROTATION			11
#define VEL_NORMAL_MOUSE_ROTATION			2
#define TARGET_MOUSE_SCALE		((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
#define CENTER_START_X				(g_pD3dApp->GetBackBufferDesc().Width/2 - 15*TARGET_MOUSE_SCALE)
#define CENTER_START_Y				(g_pD3dApp->GetBackBufferDesc().Height/2 - 15*TARGET_MOUSE_SCALE)
#define MOUSE_HALF_SIZE				(25*TARGET_MOUSE_SCALE)
#define MOUSE_START_POS_X			(x - MOUSE_HALF_SIZE)
#define MOUSE_START_POS_Y			(y - MOUSE_HALF_SIZE)
#define SECOND_TARGET_HALF_SIZE_X	(56/2*TARGET_MOUSE_SCALE)
#define SECOND_TARGET_HALF_SIZE_Y	(54/2*TARGET_MOUSE_SCALE)
#define AUTO_TARGET_HALF_SIZE		(42*TARGET_MOUSE_SCALE)
#define HP_START_X_FROM_MOUSE		(AUTO_TARGET_HALF_SIZE - (11*TARGET_MOUSE_SCALE))
#define HP_START_Y_FROM_MOUSE		(AUTO_TARGET_HALF_SIZE + (1*TARGET_MOUSE_SCALE) - 2)				// 2006-11-27 by ispark
#define AUTO_TARGET_HP_SIZE_X		61//*TARGET_MOUSE_SCALE)
#define AUTO_TARGET_HP_SIZE_Y		5//*TARGET_MOUSE_SCALE)
#define TARGET_ARROW_HALF_SIZE		(14*TARGET_MOUSE_SCALE)//28/2
#define NAME_FROM_MOUSE_CENTER_Y	(AUTO_TARGET_HALF_SIZE + 14) // 12 : text height
#define DISTANCE_FROM_MOUSE_CENTER_Y (AUTO_TARGET_HALF_SIZE - 6) // 12 : text height
#define ID_GUILD_MARK_START_X		12
#define ID_GUILD_MARK_START_Y		(AUTO_TARGET_HALF_SIZE+25)
#define TARGET_DIR_ARROW_HALF_SIZE	(19*TARGET_MOUSE_SCALE)//28/2

#define FIRE_ANGLE_START_X			(g_pD3dApp->GetBackBufferDesc().Width/2 - 45*TARGET_MOUSE_SCALE)
#define FIRE_ANGLE_START_Y			(g_pD3dApp->GetBackBufferDesc().Height/2 - 45*TARGET_MOUSE_SCALE)

#define MOUSE_STATE_NORMAL			0
#define MOUSE_STATE_UP				1
#define MOUSE_WISPERCHAT_IMAGE_GAB  10

#define CENTER_NORMAL				0
#define CENTER_LOCKON				1
#define TARGET_TICK_MOVE_POS_X		1

class CINFImage;
class CAppEffectData;
class CD3DHanFont;
class CINFGameMainOtherInfo;
class CAtumData;
class CUnitData;
class CINFImageEx;										  // 2011. 10. 10 by jskim UI시스템 변경
// 2005-08-12 by ispark
struct stSecondTarget
{
	int		nTargetIndexNum;
	int		nTargetAnimationTick;
	BOOL	bUse;
	stSecondTarget()
	{
		nTargetIndexNum = 0;
		nTargetAnimationTick = 0;
		bUse = FALSE;
	}
};

class CINFTarget : public CINFBase  
{
public:
	CINFTarget();
	virtual ~CINFTarget();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderMouse(int x, int y, int type);
	void Render2stTarget(int x, int y, int nTargetAnimationTick);
	void RenderAutoTarget(BOOL bAutoTarget, BOOL bShowAutoTargetHP, CAtumData* pUnit);
	void RenderHP(int x, int y, float fRate);

	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetMouseType(int nMouseType);
	int GetMouseType() { return m_nMouseType; }
	void RenderAttackMeMonster();
	void RenderAutoTargetDrow();
	void RenderAttackMeObject();
	void SetTargetAnimation(){m_bTargetAnimation = TRUE;}
	BOOL GetShowTargetCodition();
	void RenderAttackMePKEnemy();
	void RenderOtherTargetMonster();						// 2006-11-09 by ispark, 몬스터가 다른 상대 타겟 표시
	void RenderInfluenceMonster(CMonsterData * pMon);		// 2006-11-24 by ispark, 같은 세력 몬스터에 대한 표시

protected:
	void RenderGameMainMonsterInfo(CMonsterData* pUnit, int x, int y, float fHPCurrent, float fHPMax);
	void RenderGameMainEnemyInfo(CEnemyData* pUnit, int x, int y);
	// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
	//void RenderTargetArrow(D3DXVECTOR3 vPos );
	POINT RenderTargetArrow(D3DXVECTOR3 vPos );
	void RenderOutSideMonster(CMonsterData* pMonster);		// 내시야에서 벗어난 나를 공격하는 몬스터 정보 표시
	void RenderOutSideEnemy(CEnemyData* pEnemy);
	void RenderOutSideObject(D3DXVECTOR3 vPos);

	// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
	void RenderDistance(CAtumData* pUnit, POINT ptArrow);
	BOOL IsTargetToParty(ClientIndex_t i_nTargetIndex);

public:
	CINFImageEx*		m_pCenter[2];				// "t_center0, t_center1", 가운데 표시
	CINFImageEx*		m_pRedMouse;				// "t_m_red", 마우스 빨간색 타겟
	CINFImageEx*		m_pArrowMouse;				// "t_m_whi", 마우스 하얀색 방향지시
	CINFImageEx*		m_pBlueMouse;				// "t_m_blue", 마우스 파란색 타겟
	CINFImageEx*		m_p2stLockOn;				// "t_2st_l", 2형 무기 락온
	CINFImageEx*		m_pTextLockOn;				// "t_text_l", "LOCKON!"텍스트
	CINFImageEx*		m_pAutoTarget;				// "t_ototar", 자동 타겟 설
	CINFImageEx*		m_pAutoTargetLong;			// "t_oto", 자동 타겟 (사정거리 밖에 있는 경우
	CINFImageEx*		m_pSkillTarget;				// "skTarget", 타겟이 있는 스킬 사용시 타겟
	CINFImageEx*		m_pHPBox;					// "t_hpbox", HP박스
	CINFImageEx*		m_pHP;						// "t_hp", HP정보
	CINFImageEx*		m_pNormalMouse[2];			// "t_m_red", 마우스 빨간색 타겟
	CINFImageEx*		m_pFireAngle[2];			// "t_angle", 화망 (화망이미지,A기어는 안나옴)	

	CINFImageEx*		m_pWisperBoxLeftTRightB;	// 귓말 채팅창 마우스 이미지
	CINFImageEx*		m_pWisperBoxLeftBRightT;	// 귓말 채팅창 마우스 이미지
	CINFImageEx*		m_pWisperBoxLeftRight;		// 귓말 채팅창 마우스 이미지
	CINFImageEx*		m_pWisperBoxTopBottom;		// 귓말 채팅창 마우스 이미지
	CINFImageEx*		m_pWisperBoxCross;			// 귓말 채팅창 마우스 이미지

	CINFImageEx*		m_pMonTargetStateImg;		// 몬스터의 공격대상 상태 표시하는 이미지(몬스터의 이름 양쪽)
	CINFImageEx*		m_pImgTargetMe[2];			// 내가 타겟을 잡은 몬스터

//	CAppEffectData		* m_pTargetDirArrow;	// 화면에 벗어나는 타겟 방향 화살표
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	//CINFImage*		m_pTargetDirArrow[3];		// 화면에 벗어나는 타겟 방향 화살표
	CINFImageEx*		m_pTargetDirArrow[4];		// 화면에 벗어나는 타겟 방향 화살표
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	CINFImageEx*		m_pImgArrowHelp;			// 세력선택 전까지 화살표의 설명을 해준다.
	float			m_fTargetDirVel;
	float			m_fMouseRotationtAngle;		// 마우스 회전 각도
	int				m_nMouseType;				// 일반, 블루타겟, 레드타겟, 회전블루, 회전레드
//	float			m_fAutoTargetHPRate;		// 자동 타겟 설정시(구.강제락온) HP 양
//	BOOL			m_bAutoTargetShow;			// 자동 타겟 설정시(구.강제락온) HP 보여주는지 여부
	CD3DHanFont*	m_pFontAutoTargetName;
	CD3DHanFont*	m_pFontAutoTargetDistance;
	CD3DHanFont*	m_pFontTargetArrow;
	int				m_nMouseState;
	BOOL			m_bRestored;
	BOOL			m_bWisperChatBoxMouseFlag;	// 귓말 채팅창 마우스 포인트 활성화 비활성화 
	CINFGameMainOtherInfo* m_pOtherInfo;

	float			m_fTickCount;
	int				m_nAttackerTargetX;
	int				m_nTargetColor;
	BOOL			m_bReverseTargetMove;

protected:
	CINFImageEx*	m_pImgTargetAnimation[4];
	CINFImageEx*	m_pImgTargetHelper[2];				// 2007-07-27 by dgwoo 저렙을 위한 설명 [0] 1형, [1] 2형.
	CINFImageEx*	m_pImgSelectTarget[2];
	CINFImageEx*	m_pImgPartyTarget[2];				// 2008-06-17 by dgwoo 파티원을 타겟잡은 몬스터의 표시.

	list<stSecondTarget>	m_listTargetOld;		// 2005-08-12 by ispark
	D3DXVECTOR3		m_vecTargetTemp;				// 화망 렌더를 위한 임시 변수
	BOOL			m_bTargetAnimation;
	BOOL			m_bTargetDistance;
	BOOL			m_bTargetDis;					// 마우스 포인터를 보이지 않게 하는코드
//	int				m_nTargetAnimationTick;			// 2005-08-12 by ispark
	int				m_nOrderTargetX;
	int				m_nOrderTargetY;
	float 			m_fTimeDelay;	

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	CINFImageEx*		m_pImgFriendlyTarget[2];
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
};

#endif // !defined(AFX_INFTARGET_H__1E2FD209_BB49_4CB7_ADDA_5BE689875601__INCLUDED_)
