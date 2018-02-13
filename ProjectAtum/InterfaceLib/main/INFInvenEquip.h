// INFInvenEquip.h: interface for the CINFInvenEquip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINVENEQUIP_H__EE8904EF_85C9_4027_AEA3_FD245E12082A__INCLUDED_)
#define AFX_INFINVENEQUIP_H__EE8904EF_85C9_4027_AEA3_FD245E12082A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFImportantInfo.h"
#include "INFInvenExtend.h"

#define		MAX_WEIGHT		11
// 2010. 03. 18 by jskim 몬스터변신 카드
#define     UNIT_SCALE		0.011f
//end 2010. 03. 18 by jskim 몬스터변신 카드

class CINFInvenEquip  : public CINFBase  
{
public:
	CINFInvenEquip(CAtumNode* pParent);
	virtual ~CINFInvenEquip();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	
	void Render();
	void Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);

	int OnMouseWhell(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	
	BOOL IsShowWnd();
	void ShowWnd(BOOL bShow, POINT* i_ptPos= NULL);

	void RenderInvenBack(POINT ptPos, SIZE szSize);
	void RenderMirror(POINT *pMirrorPos=NULL);

	D3DXMATRIX	GetInvenMatInven();
	void		SetWearPosition(int iWearPosition);
	POINT	GetBkPos();


	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 관리.
	void InitEquipPosInfo ( void );					// 장착 슬롯 Pos정보 초기값 설정.

	CPosData *GetEquipPosInfo ( int iKind );		// 장착 슬롯 Pos정보 포인터 리턴.
	// End 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 관리.

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	void SetBkPos(POINT ptBkPos);
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

private:
	BOOL IsMouseCaps(POINT ptPos);
	BOOL IsWndRect(POINT ptPos);
	void UpdateBtnPos();
	BOOL GetResolutionPos(POINT ptPixel, float &fPosX,float &fPosY,float &fPosZ,float &fScaling);

	void RenderWearPosition( int iWearPosition );
	void RenderAttachItem(int nPosX, int nPosY);	

	int		ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	int		ProcessRotationUnit(POINT pt, UINT uMsg);
	void	SetByPushButton(BOOL bFlag);
	



private:
	BOOL m_bShowWnd;

	POINT	m_ptBkPos;	
	BOOL						m_bMove;
	POINT						m_ptCommOpMouse;

	LPDIRECT3DVERTEXBUFFER9 m_pMirrorVB;
	LPDIRECT3DTEXTURE9	m_pMirrorTexture;
	D3DXMATRIX			m_pMatInvenBackProj;	// 인벤토리 배경 메트릭스
	
	D3DXMATRIX			m_pMatInven;
	D3DXMATRIX			m_pMatInvenWeaponSetPosition[4];
	D3DXMATRIX			m_pMatInvenWeaponOrgPosition[4];// 2006-01-03 by ispark, 무기 포지션 메트릭스

//	float				m_fUnitPosX;
//	float				m_fUnitPosY;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
	CINFImage*			m_pBkImage;				//배경이미지 
#endif

	CINFImageEx*		m_pWearPositionBlink;//인벤안에 있는 아이템에 마우스를 갖다 댓을때 장착창 위치에서 깜박일 이미지
	int					m_iWearPosition;// 깜박일 장착창의 위치
	CRangeTime			m_tBlinkTime;

	// Rotation
	int					m_nRotationState;
	int					m_nButtonState;
	
	BOOL				m_bTurnLeft;
	BOOL				m_bTurnRight;
	BOOL				m_bTurnUp;
	BOOL				m_bTurnDown;
	BOOL				m_bByPush[4];

	CINFImageEx*		m_pInvenRotationBase;
	CINFImageEx*		m_pInvenDirection[INVEN_ROTATION_STATE][INVEN_ROTATION_NUM];


#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	CINFImageEx*		m_pWeight[MAX_WEIGHT];	// 적재량
	CINFImageBtn*		m_pCloseBtn;
#endif

	CD3DHanFont*		m_pFontItemNum;
	

	
	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 정보.
	CPosData			m_cEquipPosInfo[MAX_EQUIP_POS];


};

#endif // !defined(AFX_INFINVENEQUIP_H__EE8904EF_85C9_4027_AEA3_FD245E12082A__INCLUDED_)
