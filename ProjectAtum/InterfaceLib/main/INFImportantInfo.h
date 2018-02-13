// INFImportantInfo.h: interface for the CINFImportantInfo class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_INFIMPORTANTINFO_H__E307CF26_C7FB_48A6_9D5C_FFA20FDF1D0D__INCLUDED_)
#define AFX_INFIMPORTANTINFO_H__E307CF26_C7FB_48A6_9D5C_FFA20FDF1D0D__INCLUDED_



///////////////////////////////////////////////////////////////////////////////
/// \class		class CINFImportantInfo : public CINFBase  
/// \brief		거점정보를 가지고있는 클래스.(피,거점의 상태)
///
/// \author		dgwoo
/// \version	
/// \date		2008-03-24 ~ 2008-03-24
/// \warning	
///////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

enum {
	MAP_STATE_OFF = 0,
	MAP_STATE_NORMAL,
	MAP_STATE_DESTROY,
	MAP_STATE_NUMBER
};
enum {
	TELEPORT_STATE_OFF = 0,				// 없는 상태.
	TELEPORT_STATE_ING,					// 활성화 진행중.
	TELEPORT_STATE_ACTIVE				// 활성화.
};
enum {
	HP_STATE_OFF = 0,					// 없는 상태.
	HP_STATE_ON,						// 켜진 상태.
	HP_STATE_BG							// 피바 배경 이미지만 있는 상태.
};

#define		IMPORTANT_ATT_WARNING_GAP		0.5f
#define		IMPORTANT_ATT_WARNING_TIME		10.f
#define		TELEPORT_ACT_ANIMATION          11
#define		TELEPORT_ACT_COOLTIME			18
#define		TELEPORT_ANI_TIME_GAP			0.15f

class CINFImage;
class CINFImageEx;
class CINFImportantInfo : public CINFBase  
{
public:
	CINFImportantInfo();
	virtual ~CINFImportantInfo();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	DWORD			SetState(DWORD i_nState,float i_HPRate = MS_WAR_HP_RATE);
	void			SetImportantPos(int i_X,int i_Y);
	BOOL			SetMapNum(DWORD i_nMapNum);
	void			Init();
	USHORT			SetOption(USHORT i_nOption);
	void			AttWarning();
	void			InitAllState();
	void			SetActiveTime(ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime);

	USHORT			m_nOption;					// 옵션
	DWORD			m_nState;					// 
	DWORD			m_nMapState;
	DWORD			m_nTelState;
	DWORD			m_nHPState;
	float			m_fHP;						// 현재 자신의 피량.
	int				m_nStartX;					// 시작 좌표.
	int				m_nStartY;

	INT				m_bMapInfluence;					// 맵의 세력.

	float			m_fWarningTime;				// 경고메시지 시간 저장.
	float			m_fWarningGap;				// 
	BOOL			m_bOldWarning;				// 깜박임 효과 저장.

	DWORD			m_nMapNum;
	CINFImageEx*		m_pImgMapState[MAP_STATE_NUMBER];			// 맵의 상태에 따른 이미지.
	CINFImageEx*		m_pImgHP;					// 피 바 이미지.
	CINFImageEx*		m_pImgHPBg;					// 피 배경 이미지.
	CINFImageEx*		m_pImgReadyTel;				// 준비상태의 텔레포트 이미지.
	CINFImageEx*		m_pImgSuccTel;				// 활성화 완료 텔레포트 이미지.
	CINFImageEx*		m_pImgAttWarning;			// 경고 이미지.
	
	CINFImageEx*		m_pActAni[TELEPORT_ACT_ANIMATION];			// 활성화 진행중 애니매이션.
	UINT			m_nAniNum;									// 애니매이션 이미지 번호.
	float			m_fAniTime;									// 시간 누적.
	BOOL			m_bActAni;									// 활성화 진행중인 애니매이션.

	CINFImageEx*		m_pActCoolTime[TELEPORT_ACT_COOLTIME];		// 활성화 진행효과를 칸이 차는듯한 이미지.
	UINT			m_nCoolTimeNum;								// 쿨타임 넘버.
	ATUM_DATE_TIME	m_DTStartTime;								// 테스트 변수.
	float			m_fCooltime;								// 쿨타임 시간 체크.
	BOOL			m_bActCoolTime;								// 활성화 진행중인 쿨타임.
	float			m_fTeleportBuldingTime;						// 텔레포트가 진행시간.


};

#endif // !defined(AFX_INFIMPORTANTINFO_H__E307CF26_C7FB_48A6_9D5C_FFA20FDF1D0D__INCLUDED_)
