// INFArenaCreate.h: interface for the CINFArenaCreate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENACREATE_H__8ECA0A65_7C15_48B5_8DB4_C91FB72EC2B2__INCLUDED_)
#define AFX_INFARENACREATE_H__8ECA0A65_7C15_48B5_8DB4_C91FB72EC2B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImageEx;
class CINFImageBtn;
class CINFGroupImage;

class CINFArenaCreate : public CINFBase  
{
public:
	CINFArenaCreate(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	CINFArenaCreate();
	virtual ~CINFArenaCreate();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	VOID			SetPos(UINT x, UINT y);
	VOID			AddAllPersons(SARENA_REQUEST_CREATE_TEAMINFO *pTeamInfo);
	VOID			ClearAllPersons();
	VOID			SetPersons(BYTE BMode);
	VOID			ClearPersons();
	VOID			ChangeArenaMode(BYTE BMode);
	VOID			ShowCreateWindow();
	VOID			HideCreateWindow();
	VOID			SendCreateTeam();

	BUILDINGNPC*	m_pBuildingInfo;

	CINFImageEx	*	m_pImgCreateB[BUTTON_STATE_NUMBER];			// 생성 버튼.
	CINFImageEx	*	m_pImgCancelB[BUTTON_STATE_NUMBER];			// 취소 버튼.
	//CINFImage	*	m_pImgCbArr[BUTTON_STATE_NUMBER];			// 콤보 버튼.

	UINT			m_nStateCreateB;							// 생성 버튼 state
	UINT			m_nStateCancelB;							// 취소 버튼 state
//	UINT			m_nStateCbArrCount;							// 인원 버튼 state
	//UINT			m_nStateCbArrMode;							// 모드 버튼 state



	CINFImageEx	*	m_pImgAreCreate;							// 아레나 생성창.
	CINFImageEx	*	m_pImgCbCount;								// 인원 콤보 배경.
	CINFImageEx	*	m_pImgCbAmode;								// 모드 선택 콤보 배경.

	CD3DHanFont	*	m_pFontCount;								// 인원 폰트.
	CD3DHanFont	*	m_pFontMode;								// 모드 폰트.
	CD3DHanFont	*	m_pFontPass;								// 패스워드.
//--------------------------------------------------------------------------//
//						방 생성시 필요한 변수.
	UINT			m_nSelectMode;								// 방 생성시 선택한 모드.
	UINT			m_nSelectCount;								// 방 생성시 선택한 인원수.
	CHAR			m_cTeamPW[SIZE_MAX_TEAM_PW];				// 비밀번호.
//--------------------------------------------------------------------------//
	int				m_ntempMode;								// 임시 모드 번호.
	BYTE			m_nMode;									// 모드 갯수
	BYTE			m_nCount;									// 인원 명수
	
	BOOL			m_bShow;									// 방 생성창을 보이게 할것인가.
	UINT			m_nStartX;									// 렌더링할 위치 값.
	UINT			m_nStartY;									// 렌더링할 위치 값.

	BOOL			m_bCbCount;									// 인원 콤보 창을 뛰울것인가.
	BOOL			m_bCbMode;									// 모드 콤보 창을 뛰울것인가.
	BOOL			m_bTxtPass;									// 비밀번호가 활성화.

	vector<SARENA_REQUEST_CREATE_TEAMINFO>	m_vecAllPersons;		// 모든 모드의 인원 리스트.
	vector<BYTE>							m_vecPersons;		// 인원의 리스트.
												   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pRenewArenaCreateBackImage;
	CINFImageEx	*	m_pImgCbCountBk	;				// 인원 콤보 배경.
	CINFImageEx	*	m_pImgCbAmodeBk	;					// 모드 선택 콤보 배경.

	// 2012-04-13 by mspark, 아레나 UI 작업
	CINFImageEx*	m_pImgArenaMap01;
	CINFImageEx*	m_pImgArenaMap02;
	CINFImageEx*	m_pImgArenaMapRandom;
	CINFImageEx	*	m_pImgArenaMapSelect;
	
	CINFGroupImage*	m_pOkCancelImage;

	CINFImageEx *	m_pImgFadeBG;

	POINT			m_nOkImgMinPos;
	POINT			m_nCancelImgMinPos;
	POINT			m_nOkCancelBaseImgMaxPos;

	MapIndex_t		m_nArenaMapNum;
	MapIndex_t		m_nArenaMapImgTabNum;
	// end 2012-04-13 by mspark, 아레나 UI 작업

#endif




};

#endif // !defined(AFX_INFARENACREATE_H__8ECA0A65_7C15_48B5_8DB4_C91FB72EC2B2__INCLUDED_)
