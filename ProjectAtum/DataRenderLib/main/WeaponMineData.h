// WeaponMineData.h: interface for the CWeaponMineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONMINEDATA_H__56B4F3E3_3D5D_4175_8B3C_1CA9B8DC6EDC__INCLUDED_)
#define AFX_WEAPONMINEDATA_H__56B4F3E3_3D5D_4175_8B3C_1CA9B8DC6EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

class CCinema;
class CWeaponMineData : public CWeapon  
{
public:
	CWeaponMineData(MSG_FC_ITEM_SHOW_ITEM* pMsg);
	CWeaponMineData(MSG_FC_BATTLE_DROP_MINE_OK* pMsg);
	virtual ~CWeaponMineData();
	virtual void Tick();
	void CheckDeleteMineSendData();
	void SendBattleMineAttack(ClientIndex_t nClientIndex, D3DXVECTOR3 vTargetPos, UINT nItemIndex);
	void SendBattleMineAttackFind(ClientIndex_t nClientIndex, D3DXVECTOR3 vTargetPos, UINT nItemIndex);

	
	BOOL					m_bSetPosition;
	INT						m_nClientIndex;					// 내가 발사한 마인이면 클라이언트 인덱스를 갖는다. 아니면 0
	INT						m_nTargetItemFieldIndex;
	INT						m_nTargetIndex;
	FLOAT					m_fTargetLength;				// 발사 위치와 마인이 도착 위치 사이의 거리
//	FLOAT					m_fSpeed;			
	INT						m_nFieldItemIndex;
	BOOL					m_bIsRender;
	INT						m_nObjScreenX;			// 스크린상 X 좌표
	INT						m_nObjScreenY;			// 스크린상 Y 좌표
	INT						m_nObjScreenW;			// 스크린상 W 좌표
	FLOAT					m_fObjectSize;
	POINT					m_ptRegion;						// 마인이 위치한 블럭
	FLOAT					m_fSendAttackDataCheckTime;		// 마인의 Attack 프로토콜 전송 체크 시간 - 응답이 없으면 1초마다 충돌 보낸다.
	FLOAT					m_fCheckReactionRange;			// 마인의 반응 반경
	CCinema			*		m_pCinema;
};

#endif // !defined(AFX_WEAPONMINEDATA_H__56B4F3E3_3D5D_4175_8B3C_1CA9B8DC6EDC__INCLUDED_)
