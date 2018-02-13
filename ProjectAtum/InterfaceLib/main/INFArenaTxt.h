// INFArenaTxt.h: interface for the CINFArenaTxt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENATXT_H__898FDAB2_9755_49E4_956D_4DC82A6B7065__INCLUDED_)
#define AFX_INFARENATXT_H__898FDAB2_9755_49E4_956D_4DC82A6B7065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 아레나 카운트
#define		MAX_ARENA_CNT				6

// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
// 팀 갯수
#define		ARENA_TEAM_BLUE				0	// 블루팀
#define		ARENA_TEAM_RED				1	// 레드팀
#define		MAX_ARENA_TEAM_CNT			2

#define	MAX_ARENA_NUM					10	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

class CINFImage;
class CINFImageEx;										// 2011. 10. 10 by jskim UI시스템 변경

class CINFArenaTxt  : public CINFBase
{
public:
	CINFArenaTxt(CAtumNode* pParent);
	CINFArenaTxt();
	virtual ~CINFArenaTxt();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void Tick();

	void StartArenaCnt();
	void SetIssue(int issue,float fArenaTick = 0);
	void SetShowState(int ShowState);

	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	void ShowMyTeamLogo(BYTE byInfluence);

	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	void CheckScore(int VCNDeathCount, int ANIDeathCount);
	void AddArenaTxt(MSG_FC_ARENA_ROOM_WAR_FINISH*  pArenaInfo);
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
private:
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	void HidingMyTeamLogo();	// 숨기고 있는 과정
	void HideMyTeamLogo();		// 완전히 숨김
	void TickFadeMode();	
	// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	

public:
private:
	CINFImageEx*	m_pArenaCnt[MAX_ARENA_CNT];					// 숫자 
	CINFImageEx*	m_pArenaTxt[ARENA_ISSUE_DRAW];				// 아레나 승패.
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	CINFImageEx*	m_pArenaTeamTxt[MAX_ARENA_TEAM_CNT];				// 아레나 팀

	
	int			m_nArenaIssue;								// 
	int			m_nArenaShowState;

	// 카운트 숫자 
	int			m_nArenaCnt;
	float		m_fArenaCntTick;

	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	// 페이드 값
	int			m_nFadeMode;
	float		m_fFadeInTime;
	DWORD		m_dwBkAlpha;
	float		m_fTeamInfoShowTime;
	// 진형
	int			m_nInflIdx;

	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	CINFImageEx*	m_pArenaGageBaseTxt;
	CINFImageEx*	m_pArenaGageBlueTxt;
	CINFImageEx*	m_pArenaGageRedTxt;
		
	CINFImageEx*	m_pArenaTeamAverageTxt[MAX_ARENA_TEAM_CNT][MAX_ARENA_NUM];
	CINFImageEx*	m_pArenaPercentTxt[MAX_ARENA_TEAM_CNT];

	int			m_nArenaTeamAverage[MAX_ARENA_TEAM_CNT];
	int			m_nStartPosX[MAX_ARENA_TEAM_CNT];

	int			m_nBlueScore;
	int			m_nRedScore;
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
};

#endif // !defined(AFX_INFARENATXT_H__898FDAB2_9755_49E4_956D_4DC82A6B7065__INCLUDED_)
