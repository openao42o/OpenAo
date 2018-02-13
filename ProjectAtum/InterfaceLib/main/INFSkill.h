// INFSkill.h: interface for the CINFSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSKILL_H__86964810_B32C_47B6_B7C0_5C6A8B1CB8B7__INCLUDED_)
#define AFX_INFSKILL_H__86964810_B32C_47B6_B7C0_5C6A8B1CB8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImage.h"
#include "INFImageEx.h"                       // 2011. 10. 10 by jskim UI시스템 변경
// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
#include "INFItemInfo.h"
//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
class CSkillInfo;
class CD3DHanFont;


// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
#define BOX_MARGIN		 6
#define LINE_INTERVAL	 1
//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경

typedef struct  {
	CSkillInfo*		pItemSkill;
	CD3DHanFont*	pSkillFont;
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	vector<string>	m_strSkillInfo;
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
} SkillFontInfo;

typedef struct  {
	ITEM*			pItemInfo;
	UID64_t			ItemUID;
	CD3DHanFont*	pItemFont;
	float			fRemainedTime;
	BOOL			bUseItemFinish;
	// 2008-03-05 by bhsohn 느린 시스템에서 시간제 아이템 시간 계산이 잘못 되는 부분 수정
	BOOL			bFirstTimeCheck;	// 처음 시간 계산함
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	vector<string>	m_strItemInfo;
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
} ItemFontInfo;


class CINFSkill : public CINFBase  
{
	friend class CSkill;
public:
	CINFSkill();
	virtual ~CINFSkill();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void			InsertSkillInfo(CSkillInfo* pItemSkill);
	void			DeleteSkillInfo(CSkillInfo* pItemSkill, UID32_t i_charUID=INVALID_UID32);
	void			InsertItemInfo(ITEM* pItemInfo,UID64_t Uid,int nRemainedTime);
	void			DeleteItemInfo(ITEM* pItemInfo);

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 발동류 아이템 버프 추가
	void			InsertItemBuff( ITEM* pItemInfo, UID64_t nUID, INT nRemainedTime );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	void			Render();
	int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL			IsRenderIcon(CSkillInfo* pSkillInfo);
	CSkillInfo*		FindSkillIcon(int nSkillItemNum);
	ITEM*		FindItemIcon(int nSkillItemNum);				   //2011-10-06 by jhahn 파트너 성장형 시스템

	LPDIRECT3DTEXTURE9 GetGroundTargetTexture() { return m_pGroundTarget != NULL ? m_pGroundTarget->GetTexture() : NULL;}

	void			RenderGroundTarget();
	HRESULT			RestoreGroundTarget();
	HRESULT			InvalidateGroundTarget();
	void			SetGroundTarget(D3DXVECTOR3 vGroundTarget);
	void			SetAirGroundTarget(D3DXVECTOR3 vAirGroundTarget);				// 2006-12-01 by ispark, 공중 폭격

	// 2007-03-22 by bhsohn 아이콘 크기 버그 수정
	BOOL			IsDoingSkill(INT nSourceIndex);

	// 2009-02-25 by bhsohn 버프형 사거리 레이더 안되는 버그 수정
	void			RefreshSkillRader();
	// end 2009-02-24 by bhsohn 버프형 사거리 레이더 안되는 버그 수정
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	void			RenderParameter(ItemFontInfo* pItemInfo);
	vector<string>	SetFunction(ITEM* pItemInfo);

	// 2013-03-27 by bhsohn DestParam 자료형 수정
//	string			SetParameter(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2, FUNCTION_TYPE nFunctionType = FUNCTIONTYPE_NORMAL);
	string			SetParameter(int nParameterIndex, DestParam_t bType1, float fValue1, DestParam_t bType2, float fValue2, FUNCTION_TYPE nFunctionType = FUNCTIONTYPE_NORMAL);
	// END 2013-03-27 by bhsohn DestParam 자료형 수정
	void			RenderInfoWindows(int x, int y, char* name, vector<string> strItemInfo);
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	// 2010. 04. 14 by jskim 아이템 사용시 부스터 기능 추가
	vector<ItemFontInfo*>	GetItemFontInfo() { return m_vecItemFontInfo; }
	//end 2010. 04. 14 by jskim 아이템 사용시 부스터 기능 추가

protected:
	void			SetParamFactor(ITEM* pItemInfo);
	void			ReleaseParamFactor(ITEM* pItemInfo);
	void			SendUseItemFinish(UID64_t UIDItem);

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	void			SetItemSkillInfo(BOOL i_bScan, ITEM* pItemInfo, int i_nRemainedTime);

protected:
	vector<SkillFontInfo*>		m_vecSkillFontInfo;
	vector<ItemFontInfo*>		m_vecItemFontInfo;

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 발동류 아이템 버프
	vector<ItemFontInfo*>		m_vecItemBuffInfo;
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	CINFImageEx				*	m_pGroundTarget;                        // 2011. 10. 10 by jskim UI시스템 변경
	LPDIRECT3DVERTEXBUFFER9		m_pVBGroundTarget;		
	D3DXMATRIX					m_mGroundTargetMatrix;
	
	CD3DHanFont*				m_pToolTipFont;

	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//LPD3DXSPRITE				m_pToolTipSprite;
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	LPDIRECT3DTEXTURE9			m_pToolTipTexture;

	POINT						m_ptMouse;
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	CINFImageEx*					m_pInfoBoxSide[9];                  // 2011. 10. 10 by jskim UI시스템 변경
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
};

#endif // !defined(AFX_INFSKILL_H__86964810_B32C_47B6_B7C0_5C6A8B1CB8B7__INCLUDED_)
