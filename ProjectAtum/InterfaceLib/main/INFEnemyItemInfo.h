// INFEnemyItemInfo.h: interface for the CINFEnemyItemInfo class.
//
//////////////////////////////////////////////////////////////////////
/////////////// 2012-06-14 by isshin 아템미리보기 ////////////////////
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFENEMYITEMINFO_H__EE8904EF_85C9_4027_AEA3_FD123E12082A__INCLUDED_)
#define AFX_INFENEMYITEMINFO_H__EE8904EF_85C9_4027_AEA3_FD123E12082A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFImportantInfo.h"
#include "INFInvenExtend.h"
#include "EffectRender.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"

struct ENEMY_WEAPON_MESH
{
	CSkinnedMesh*	pInvenWeaponMesh;
	int				nMatIndex;
};
typedef vector<ENEMY_WEAPON_MESH>		vectENEMY_WEAPON_MESH;

class CINFEnemyItemInfo  : public CINFBase  
{
public:
	CINFEnemyItemInfo(CAtumNode* pParent);
	virtual ~CINFEnemyItemInfo();

	HRESULT		InitDeviceObjects();
	HRESULT		RestoreDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	
	void		SetBaseEnemyEtc(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID);			// 기본 정보 셋팅
	void		RqCharacterUserItemInfo(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID);	// Enemy 정보 요청

	void		Render();
	void		Tick();
	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL		IsMouseCaps(POINT ptPos);
	BOOL		IsShowWindow();
	void		ShowWindow(BOOL b_show);
	void		UpdateUIPos();
	void		ShowEnemyItemInfo();	

	void		Set_bItemInfoEnable(BOOL Enable){ m_bItemInfoEnable = Enable;}	// 정보 요청 여부

	void		InitEquipPosInfo ( void );					// 장착 슬롯 Pos정보 초기값 설정
	CPosData	*GetEquipPosInfo ( int iKind );				// 장착 슬롯 Pos정보 포인터 리턴
	void		CheckUpdateEquipItem();						// 타유저 장비 업테이트 체크
	BOOL		CheckEnemyState();							// 타유저 상태 체크

	void		Set_bChangeEquipItem(BOOL bChange, ClientIndex_t ClientIndex);
	INT			GetItemIconNum(INT OldItemNum, INT CurrentItemNum, INT CurrentShapeItemNum);
	void		SetEnchantInfo(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg);
	void		SetEnemyEquipItemIconInfo();				// Enmey 아이템 아이콘 정보 셋팅
	void		RenderAttachItem(int nPosX, int nPosY);		// 장착아이템 아이콘
	
	void		InitEnemyEquipItemInfo();
	void		SetEnemyEquipItemInfo();					// 기체 장비아이템 셋팅
	void		SetEnemyClientKind(ClientIndex_t TargetCharcterCID);
	ClientIndex_t	GetEnemyClientKind(){return m_TargetCharcterCID;}
	void		SetEnemyUnitKind(UID32_t TargetCharcterUID);
	USHORT		GetEnemyUnitKind(){return m_usUnitKind;}

	void		RenderMirror(POINT *pMirrorPos);			// 기체.
	
	void		SetByPushButton(BOOL bFlag);	
	int			ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			ProcessRotationUnit(POINT pt, UINT uMsg);

	void		MirrorTurnLeft(){m_fRotationX = m_fRotationX+0.05f;}
	void		MirrorTurnRight(){m_fRotationX = m_fRotationX-0.05f;}
	void		MirrorTurnUp(){m_fRotationZ = m_fRotationZ+0.05f;}
	void		MirrorTurnDown(){m_fRotationZ = m_fRotationZ-0.05f;}
	void		MirrorTurnOrig(){m_fRotationX = SHUTTLE_ROTATION_DEFAULT_X; m_fRotationZ = SHUTTLE_ROTATION_DEFAULT_Z;}

	BOOL		IsRobotArmor();	// 2013-07-08 by bhsohn 아이템 정보보기 기능 상대방 로봇 아머 무기 나오는 버그 수정

private:
	CINFGroupImage*					m_pEnemyItemInfoBackImage;
	CINFGroupImage*					m_pBkImage;			// 닫기	
	CINFImageBtn*					m_pCloseBtn;		// 닫기	

	CINFImageEx*					m_pInvenRotationBase;
	CINFImageEx*					m_pInvenDirection[INVEN_ROTATION_STATE][INVEN_ROTATION_NUM];

	CD3DHanFont*					m_pFontItemNum;
	
	BOOL							m_bMove;
	BOOL							m_bShow;	
	POINT							m_ptCommOpBk;
	POINT							m_ptCommOpMouse;
	POINT							m_ptBkPos;
	
	USHORT							m_usUnitKind;		// 유닛의 종류
	BOOL							m_bItemInfoEnable;
	
	vector<INT>						m_vecItemInfo;
	CPosData						m_cEquipPosInfo[MAX_EQUIP_POS];					

	/////////////////////////////////////////////////////////////////////
	//Shuttle
	D3DXMATRIX						m_pMatInven;
	D3DXMATRIX						m_pMatInvenWeaponSetPosition[4];
	D3DXMATRIX						m_pMatInvenWeaponOrgPosition[4];
	
	vectENEMY_WEAPON_MESH			m_vectEnemyWeaponMesh;
	
	// Rotation
	float				m_fRotationX;
	float				m_fRotationZ;

	int					m_nRotationState;
	int					m_nButtonState;
	
	BOOL				m_bTurnLeft;
	BOOL				m_bTurnRight;
	BOOL				m_bTurnUp;
	BOOL				m_bTurnDown;
	BOOL				m_bByPush[4];	
	
	ClientIndex_t		m_TargetCharcterCID;
	UID32_t				m_TargetCharcterUID;
	int					m_nWeaponCount;
	
	INT					m_nArmerNum;
	INT					m_nRadar;
	INT					m_nCpu;
	INT					m_nBaseWeapon;
	INT					m_nMark;
	INT					m_nSpecialWeapon;	
	INT					m_nEngine;
	INT					m_nAccessoryFuel;
	INT					m_nAccessoryCard;	
	INT					m_nPet;

	BOOL				m_bChangeEquipItem;		// 장비 Update 여부
};

#endif // !defined(AFX_INFENEMYITEMINFO_H__EE8904EF_85C9_4027_AEA3_FD245E12082A__INCLUDED_)