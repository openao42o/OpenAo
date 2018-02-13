// INFArenaResult.h: interface for the CINFArenaResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENARESULT_H__9124FFDF_D17D_4F50_850D_9C0530B81F55__INCLUDED_)
#define AFX_INFARENARESULT_H__9124FFDF_D17D_4F50_850D_9C0530B81F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

// 라운드 방식
#define	ARENA_RESULTMODE_DEATHMATCH		0
#define	ARENA_RESULTMODE_ROUND			1
#define	MAX_ARENA_RESULTMODE			2

// 아군, 적군
#define	ARENA_RESULT_FRIEND			0		// 아군
#define	ARENA_RESULT_ENEMY			1		// 적군
#define	MAX_ARENA_RESULT			2

// 바이제니유/알링턴
#define	ARENA_RESULTINFLUENCE_BCU				0		// 바이제니유
#define	ARENA_RESULTINFLUENCE_ANI				1		// 알링턴
#define	MAX_ARENA_RESULTINFLUENCE				2

// 승리/패배
#define	ARENA_RESULTSTATE_DRAW					-1		// 무승부-1
#define	ARENA_RESULTSTATE_WIN					0		// 승
#define	ARENA_RESULTSTATE_LOSE					1		// 패
#define	MAX_ARENA_RESULTSTATE					2

// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
struct STRUCT_ARENA_ROOM_WAR_FINISH
{
	BYTE			Influence;				// 2007-04-17 by dhjin, 세력 
	BYTE 			ShootingDown;			// 2007-04-17 by dhjin, 격추수 
	BYTE 			SufferingAttack;		// 2007-04-17 by dhjin, 피격수 
	BYTE			LostPoint;				// 2007-06-01 by dhjin, LostPoint
	USHORT			UnitKind;				// 2007-04-17 by dhjin, 기어 종류 
	CHAR			CharacterLevelRank[SIZE_MAX_LEVELRANK];	// 2007-04-17 by dhjin, 유저 레벨 등급
	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	CHAR			CharacterName[SIZE_MAX_ARENA_FULL_NAME];
	//FLOAT			Average;				// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	INT				TeamAverage;			// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
};


class CINFImage;
class CD3DHanFont;

class CINFArenaResult  : public CINFBase
{
public:
	CINFArenaResult();
	virtual ~CINFArenaResult();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void AddArenaResult(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg);
	void UserInfoClear();
	void SetIssue(int issue);

private:
	void RenderWinLose(float fStartBkX, float fStartBkY, POINT ptArenaBK, int nArenaIdx,CHARACTER myShuttleInfo);
	void RenderScroll(int nArenaIdx);
	
	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	int GetInfluenIndex(BYTE byBelligerence);
	void RenderUserInfo(float fStartBkX, float fStartBkY, POINT ptArenaBK, int nArenaIdx);	
	void GetUnitKindString(USHORT i_nUnitKind,char* pTxt);
	int GetFriendIndex(BYTE byBelligerence);

	void TestDB();

private:
	CINFImageEx*	m_pArenaResultBk[MAX_ARENA_RESULTMODE];	
	CINFImageEx*	m_pArenaResultInfluence[MAX_ARENA_RESULTINFLUENCE];		// 진형 
	CINFImageEx*	m_pArenaResultState[MAX_ARENA_RESULTSTATE];		// 진형 	

	float		m_fBackPosX;
	float		m_fBackPosY;

	// 버튼 
	CINFImageBtn*			m_pGoCiyBtn;

	// 스크롤 바
	CINFArenaScrollBar*		m_pINFArenaScrollBar[MAX_ARENA_RESULT];

	int				m_nArenaResultMode;
	int				m_nArenaResultInfluence[MAX_ARENA_RESULT];		// 진형 	

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//vector<MSG_FC_ARENA_ROOM_WAR_FINISH>		m_vecUserInfo[MAX_ARENA_RESULTINFLUENCE];				
	vector<STRUCT_ARENA_ROOM_WAR_FINISH>		m_vecUserInfo[MAX_ARENA_RESULTINFLUENCE];
	
	CD3DHanFont*			m_pFontUserInfo;
	CD3DHanFont*			m_pFontTotal;

	int						m_nScrollPos;

	float					m_fShowTime;					// 결과값을 보여주는 시간.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pRenewArenaResultBackImage;
#endif

	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	int						m_nBCURanking;
	int						m_nANIRanking;
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	
};

#endif // !defined(AFX_INFARENARESULT_H__9124FFDF_D17D_4F50_850D_9C0530B81F55__INCLUDED_)
