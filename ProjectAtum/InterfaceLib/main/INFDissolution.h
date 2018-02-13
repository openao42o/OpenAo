// INFDissolution.h: interface for the CINFDissolution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFDISSOLUTION_H__98354002_201D_4FAC_AC33_5E6212809401__INCLUDED_)
#define AFX_INFDISSOLUTION_H__98354002_201D_4FAC_AC33_5E6212809401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
// 2011-02-08 by jhahn EP4 용해  UI 변경
#include "INFImageEx.h"
#include "INFImageList.h"
//end 2011-02-08 by jhahn EP4 용해  UI 변경

#define EFFECT_MAX					13
#define EFFECT_BG_MAX				 6
#define TAKE_EFFECT_MAX				 2
#define EFFECT_TIME				    70


#define DIS_START					 0
#define DIS_ING						 1
#define DIS_END						 2

// 2011-02-08 by jhahn EP4 용해  UI 변경
class CINFImageList;
class CINFGroupManager;
class CINFGroupImage;
//end 2011-02-08 by jhahn EP4 용해  UI 변경
struct DIS_ITEM_INFO 
{
	CItemInfo*		i_ItemInfo;
	int				i_count;
};

class CINFDissolution : public CINFBase
{
public:
	CINFDissolution(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFDissolution();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Render();
	virtual void Tick();

	void RenderItem(CItemInfo* nSelectItem);
	void RenderItem(vector<DIS_ITEM_INFO> vecTakeItem);
	void ResetInfo();
	void UpLoadItem(CItemInfo* i_pItem);
	void StartDissolution();
	void PrizeItemGiveMe();
	BOOL IsCloseDissolution();

	void EnterDissolutionShop();
	void OnCloseInfWnd();
	vector<DIS_ITEM_INFO> GetTakeItem() { return m_vecTakeItemInfo; }
	void SetTakeItem( ITEM_GENERAL i_Item );
	void SetTakeItem(MSG_FC_TRADE_UPDATE_ITEM_COUNT* pMsg);
	void SetState( int state ) { m_nState = state; }
	BOOL GetBoom() { return m_bBoom; }
	void SetBoom(BOOL iBoom) { m_bBoom = iBoom; }
	void SetDeleteItem(const UID64_t a_ItemUniqueNumber);


private:

	CD3DHanFont	*			m_TakeItemFont;				//

 	CINFImageEx*			m_pImgDissolutionBG;		// 배경
	CINFImageEx*			m_pImgDissolutionLogo;		// 용해 로고
	CINFImageEx*			m_pImgDissolutionAniBG[EFFECT_BG_MAX];			//용해 로고 애니메이션 

	CINFImageEx*			m_pImgDissolutionEffect[EFFECT_MAX];			// 용해 이팩트
	CINFImageEx*			m_pImgDissolutionTakeEffect[TAKE_EFFECT_MAX];	// 용해 받을 아이템 이팩트 

 	CINFImageBtn*			m_pBtnStart;				// 용해 시작 
	CINFImageBtn*			m_pBtnTake;					// 아이템 받기

	int						m_nState;					// DIS_START  = 대기, DIS_ING = 진행 중, DIS_END = 결과
	int						m_nEffStep;					// 이팩트 스텝
	int						m_nEffBgStep;				// 이펙트 배경 스텝 

	float					m_fTimeEffectBG;			// 배경 프레임 유지 시간
	float					m_fTimeFadeInBG;			// 페이드 인 유지 시간
	float					m_fTimeEffectAniBG;			// 배경 애니 프레임 유지시간 
	
	CItemInfo*				m_pSelectItemInfo;			// 용해할 아이템
	vector<DIS_ITEM_INFO>	m_vecTakeItemInfo;			// 받을 아아템 정보
	BOOL					m_bBoom;					// 꽝
	BOOL					m_bStartClick;				// 버튼 클릭
// 2011-02-08 by jhahn EP4 용해  UI 변경		   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*			m_pDissolutionBK;
	CINFGroupImage*			m_pDissolutionControl;												  
#endif
//end 2011-02-08 by jhahn EP4 용해  UI 변경
	
};

#endif // !defined(AFX_INFDISSOLUTION_H__98354002_201D_4FAC_AC33_5E6212809401__INCLUDED_)
