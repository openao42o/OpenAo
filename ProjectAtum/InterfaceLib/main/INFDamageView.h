// INFDamageView.h: interface for the INFDamageView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFDAMAGEVIEW_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFDAMAGEVIEW_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define NUMBER_PICTURE_DAMAGE	10

class CINFImage;
class CINFImageEx;
class DataHeader;
class CGameData;

typedef struct
{
	ClientIndex_t	TargetIndex;
	int				nDamageViewStep;	// 데미지 뷰단계
	float			fTimer;				// 생성후 지나간 시간
	int				nDamageKind;
	vector<int>		vecDamageView;

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	float			fVerticalSpeed;
	float			fHorizontallySpeed;
	float			x, y;
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	int				nMultiIndex;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

} DamageView_t;

class CINFDamageView : public CINFBase
{
public:
	CINFDamageView();
	virtual ~CINFDamageView();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();

public:
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	//void RecvBattleShowDamage(ClientIndex_t	TIndex, int nDamage, int nDKind);
	void RecvBattleShowDamage(ClientIndex_t	TIndex, int nDamage, int nDKind, USHORT	MultiTargetIndex = NULL);
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	
	vector<DamageView_t>	vecDamage;	

protected:	

	CINFImageEx			*	m_pNormal;
	CINFImageEx			*	m_pCritical;
	CINFImageEx			*	m_pMiss;
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	CINFImageEx			*	m_pPet;
	// end 2010-11-01 by jskim, 펫 데미지 View 변경
	
	float					m_fTickTimer;						// 시간이 지남에따라 데미지가 위로 올라간다 5단계가 되면 지워진다
//	D3DCAPS8				m_d3dCapsTest;
};

#endif
