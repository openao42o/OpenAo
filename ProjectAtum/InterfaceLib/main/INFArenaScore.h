// INFArenaScore.h: interface for the CINFArenaScore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENASCORE_H__C12D27BF_2A1A_417E_892A_817925E69AAD__INCLUDED_)
#define AFX_INFARENASCORE_H__C12D27BF_2A1A_417E_892A_817925E69AAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define	ARENA_SCOREMODE_KILLCNT		0
#define	ARENA_SCOREMODE_ROUND		1
#define	MAX_ARENA_SCOREMODE			2

// 아군, 적군
#define	ARENA_NUM_FORM_FRIEND			0		// 아군
#define	ARENA_NUM_FORM_ENEMY			1		// 적군
#define	MAX_ARENA_NUM_FORM			2

// 바이제니유/알링턴
#define	ARENA_INFLUENCE_BCU				0		// 바이제니유
#define	ARENA_INFLUENCE_ANI				1		// 알링턴
#define	MAX_ARENA_INFLUENCE				2

// 2007-05-15 by bhsohn 아레나 스코어 시간 표시 방법 변경
#define	ARENA_TIME_TIME					0
#define	ARENA_TIME_RTIME				1
#define	MAX_ARENA_TIME					2

#define	MAX_ARENA_NUM				10

class CINFImage;
class CINFImageEx;

class CINFArenaScore  : public CINFBase
{
public:
	CINFArenaScore();
	virtual ~CINFArenaScore();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void SetArenaScore(BYTE byInfluenceType, int nScore);
	void SetArenaScoreEx(BYTE bBCU,BYTE bANI);
	void SetArenaTime(float fTime);
	
private:
	void RenderScore(CHARACTER myShuttleInfo, int nForm);
	void RenderTime();
private:
	CINFImageEx*	m_pArenaScoreBk[MAX_ARENA_SCOREMODE];	
	int			m_nScoreMode;

	// 각 진형 숫자
	CINFImageEx*	m_pArenaScoreNum[MAX_ARENA_INFLUENCE][MAX_ARENA_NUM];	

	// 진형 정보
	CINFImageEx*	m_pArenaInfluence[MAX_ARENA_INFLUENCE];

	// 시간 정보 
	CINFImageEx*	m_pArenaTime[MAX_ARENA_TIME][MAX_ARENA_NUM];
	
	int			m_nScore[MAX_ARENA_INFLUENCE];

	// 아레나 시간 
	float		m_fArenaTime;

	ATUM_DATE_TIME		m_timeArenaGamingTime;	

};

#endif // !defined(AFX_INFARENASCORE_H__C12D27BF_2A1A_417E_892A_817925E69AAD__INCLUDED_)
