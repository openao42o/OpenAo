// INFGameMainOutPost.h: interface for the CINFGameMainOutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINOUTPOST_H__D633B4F2_F06B_402C_A828_44E731AA0C97__INCLUDED_)
#define AFX_INFGAMEMAINOUTPOST_H__D633B4F2_F06B_402C_A828_44E731AA0C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CAtumNode;
class CINFImage;
class CINFImageEx;
class CD3DHanFont;
typedef struct _OutPost
{
	_OutPost()
	{
		m_BState						= 0;
		m_fLifeTime						= 0.0f;
		
		// 2007-10-02 by dgwoo 기획 추가 사항 리셋 바.
		m_fResetBarLife					= 0.0f;
		m_fResetBarTotalLife			= 0.0f;
		m_bResetBarRender				= FALSE;
		m_bAttackerInf					= 0;
		
		m_b10Min						= TRUE;


	}
	float			m_fLifeTime;									// 이미지가 사라지기까지 남은시간.
	BYTE			m_BState;										// 전진기지 상태.
	ATUM_DATE_TIME	m_timeWarStartTime;								// 전쟁 시작 시간.
	ATUM_DATE_TIME	m_timeWarEndTime;								// 전쟁 끝나는 시간.
	// 2007-10-02 by dgwoo 리셋시간.
	BOOL			m_bResetBarRender;								// 리셋 바를 그릴것인가?
	float			m_fResetBarLife;								// 리셋 완료까지의 시간.
	float			m_fResetBarTotalLife;							// 리셋 완료까지의 총시간.
	BYTE			m_bAttackerInf;									// 공격자 세력.
	MapIndex_t		m_MapIndex;										// 진행되고 있는 맵번호.
	BOOL			m_bOutPostImgShow;								// 전진기지 도움말(이미지)를 출력할것인가?
	BOOL			m_b10Min;										// 10분남았을경우 스트링 출력.
	CINFImage*		m_pImgTemp;

} STOUTPOST;

class CINFGameMainOutPost : public CINFBase  
{
public:
	CINFGameMainOutPost();
	virtual ~CINFGameMainOutPost();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void SetWarTime(ATUM_DATE_TIME i_DateTimeStart,ATUM_DATE_TIME i_DateTimeEnd);
	void SetResetTime(MapIndex_t i_MapIndex,ATUM_DATE_TIME i_DateTimeStart,ATUM_DATE_TIME i_DateTimeEnd);
	void SetResetTime(MapIndex_t i_MapIndex,INT i_OutPostWarResetRamainSecondTime);
	void SetAttInfluence(MapIndex_t i_MapIndex,BYTE i_AttInfluence);
	void SetResetBarRender(BOOL i_bResetBarRender);
	void SetOutPostState(BYTE BState,MapIndex_t i_MapIndex);
	BOOL SetOutPostImgShow();
	void AddOutPostMapIndex(MapIndex_t MapIndex);
	void DeleteOutPostMapIndex(MapIndex_t MapIndex);
	void DeleteAllOutPostMapIndex();
	BYTE GetOutPostState(MapIndex_t MapIndex);

	// 2007-11-27 by dgwoo 여러개의 전진기지 발발. 
	void AddOutPost(MapIndex_t i_MapIndex,BYTE i_State,BYTE i_Influence,ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime,INT i_ResetTime = 0);
	STOUTPOST * FindOutPost(MapIndex_t i_MapIndex);
	BOOL IsOutPosting();

	

	CINFImageEx*		m_pImgProDestroy;								// 방어막 파괴.
	CINFImageEx*		m_pImgResetStart;								// 각인 시작.
	CINFImageEx*		m_pImgResetSucc;								// 각인 성공.
	CINFImageEx*		m_pImgResetDestroy;								// 각인 파괴.
	CINFImageEx*		m_pImgResetBarBack;								// 리셋바 배경.
	CINFImageEx*		m_pImgResetBarBar1;								// 리셋바1(빨강:공격)
	CINFImageEx*		m_pImgResetBarBar2;								// 리셋바2(파랑:방어)
	CINFImageEx*		m_pImgResetBarInf1;								// 리셋바세력 1.(바이제니유)
	CINFImageEx*		m_pImgResetBarInf2;								// 리셋바세력 2.(알링턴)
	
	
	vector<STOUTPOST>		m_vecOutPost;							// 전진기지 정보등.
	BOOL					m_bOutPostImgShow;						// 정보창이 보여야 하는가?
	STOUTPOST*				m_pOutPostInfo;							// 자신의 있는 맵과 관련이 있는 전진기지 정보.
	
	
//	float			m_fLifeTime;									// 이미지가 사라지기까지 남은시간.
//	
//	BYTE			m_BState;										// 전진기지 상태.
//
//	ATUM_DATE_TIME	m_timeWarStartTime;								// 전쟁 시작 시간.
//	ATUM_DATE_TIME	m_timeWarEndTime;								// 전쟁 끝나는 시간.
//	
//	// 2007-10-02 by dgwoo 리셋시간.
//	BOOL			m_bResetBarRender;								// 리셋 바를 그릴것인가?
//	float			m_fResetBarLife;								// 리셋 완료까지의 시간.
//	float			m_fResetBarTotalLife;							// 리셋 완료까지의 총시간.
//
//	BYTE			m_bAttackerInf;									// 공격자 세력.
//
//
//	MapIndex_t		m_MapIndex;										// 
//	vector<MapIndex_t>		m_vecMapIndex;							// 현재 전진기지전이 진행중인 MapIndex
//	BOOL			m_bOutPostImgShow;								// 전진기지 도움말(이미지)를 출력할것인가?
//
//	BOOL			m_b10Min;										// 10분남았을경우 스트링 출력.
};

#endif // !defined(AFX_INFGAMEMAINOUTPOST_H__D633B4F2_F06B_402C_A828_44E731AA0C97__INCLUDED_)
