// INFMotherShipManager.h: interface for the CINFMotherShipManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMOTHERSHIPMANAGER_H__6585EF9D_3FD5_4DD8_8BE6_E463657B1AFE__INCLUDED_)
#define AFX_INFMOTHERSHIPMANAGER_H__6585EF9D_3FD5_4DD8_8BE6_E463657B1AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFImportantInfo.h"
#include "INFNumEditBox.h"
#include "INFGroupImage.h"


// 2009. 01. 12 by ckPark 선전 포고 시스템
#include "INFWarDeclare.h"
// end 2009. 01. 12 by ckPark 선전 포고 시스템




#define SLIDE_TIME						0.25f

struct stSTRONGHOLD_INFO{
	DWORD			nIndex;
	DWORD			MapIndex;
};

// 2007-02-06 by bhsohn 모선전 진행 사항 체크
#define MOTHERSHIP_INFO_BALON			9		// 정보창 테두리
// 모선전 최대 글씨 라인 수
#define	MOTHERSHIP_MAXLINE				6
// 모선전 정보 한 세력당 표현 해 주는 라인수 
#define	MOTHERSHIP_INFLUENCE_LINE		3


struct MOTHERSHIP_INFO
{ //모선전  정보
	char szInfluence[MAX_PATH];
	char szReMainTime[MAX_PATH];
	char szArea[MAX_PATH];
	BYTE  byBelligerence;
	FLOAT fMotherShipRemainTime;
	
	INT	 nContributionPoint;					// 소환시, 세력포인트 // 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	ATUM_DATE_TIME  SummonMonsterTime;			// 소환시간 // 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	INT SummonMonsterUnitkind;					// 몬스터종류 // 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	
	
	MOTHERSHIP_INFO()
	{
		memset(szInfluence, 0x00, MAX_PATH);
		memset(szReMainTime, 0x00, MAX_PATH);
		memset(szArea, 0x00, MAX_PATH);
		
		byBelligerence = 0;

		memset(&SummonMonsterTime, 0x00, sizeof(ATUM_DATE_TIME));		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
		nContributionPoint = 0;											// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
		SummonMonsterUnitkind = 0;										// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	};
};

struct PREADD_MOTHERSHIP_INFO
{ // 모선전 관련
	BYTE			byBelligerence;
	ATUM_DATE_TIME  SummonMonsterTime;	
	INT	 nContributionPoint;					// 소환시, 세력포인트 // 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	INT SummonMonsterUnitkind;					// 몬스터종류 // 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
};
// end 2007-02-06 by bhsohn 모선전 진행 사항 체크


typedef vector<CINFImportantInfo*>					vecINFImportantInfo;
typedef vector<CINFImportantInfo*>::iterator		vecINFImportantInfoIter;

class CINFImage;
class CD3DHanFont;
class CINFMotherShipManager : public CINFBase  
{
public:
	CINFMotherShipManager();
	virtual ~CINFMotherShipManager();
	CINFMotherShipManager(CGameData * pData);

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	void			TickShowIng();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam);

	USHORT			SetMotherShipOption(USHORT i_Option);
	BOOL			SetButtonPos0(UINT i_nPosX,UINT i_nPosY);
	BOOL			SetButtonPos1(UINT i_nPosX,UINT i_nPosY);
	void			ResetAdventBCU();
	void			ResetAdventANI();

	void			AdventMotherShipBCU(SMSWARINFO_DISPLAY * pInfo);
	void			AdventMotherShipANI(SMSWARINFO_DISPLAY * pInfo);
	void			DestroyMotherShipANI();
	void			DestroyMotherShipBCU();

	void			ShowWindowOption();
	void			HideWindowOption();

	void			RenderOption();
	UINT			OptionWindowEvent(POINT &i_pt);
	
	void			SummonMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, 
												 INT nContributionPoint, 
												 INT SummonMonsterUnitkind); // 2007-02-06 by bhsohn 모선전 진행 사항 체크
	void			DestroyMotherShipInfo(BYTE byBelligerence);// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	int				GetSummonMotherShipCnt();
	BOOL			GetMotherShipInfl(UINT i_nMotherShipInf);
	BOOL			GetMotherShipOptionAct() { return m_bOptionAct;}

	// 2008-03-27 by dgwoo 모선전시 거점맵의 정보를 셋팅.
	BOOL			SetWarPointMapInfo(SMSWARINFO_DISPLAY * pWarInfo);
	void			SetMSWarInfoDisplayOption(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK* pMsg);
	BOOL			SetMotherShipSearchTeleportTime(MSG_FC_EVENT_CLICK_TELEPORT_OK *pMsg);
	// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	void			GetMotherShipInfo(int nIdx, 
										BYTE *o_byBelligerence, 
										ATUM_DATE_TIME  *o_pSummonMonsterTime, 
										int *o_nContributionPoint,
										int *o_SummonMonsterUnitkind);

	// 2009. 01. 12 by ckPark 선전 포고 시스템
	CINFWarDeclare*	GetWarDeclareWindow(void) { return m_pWarDeclareWindow; }
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
	

private:
	void			RenderMothershipInfo();						// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	void			RenderMothershipInfoBalon(int x, int y, int cx, int cy); // 2007-02-06 by bhsohn 모선전 진행 사항 체크	
	void			TickMotherShipInfo();						// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	void			AddPreMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, INT nContributionPoint, INT SummonMonsterUnitkind); // 2007-02-06 by bhsohn 모선전 진행 사항 체크
	void			UpdateMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, 
												INT	 nContributionPoint, INT SummonMonsterUnitkind); // 2007-02-06 by bhsohn 모선전 진행 사항 체크
	SIZE			GetMotherShipInfoFontWidth();					// 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
	SIZE			GetStringSize(char* pTxt);						// 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
	void			SendMotherShipOption();
	
	//--------------------------------------------------------------------------//
	//				BCU : 0,                ANI : 1								//
	//--------------------------------------------------------------------------//
	
private:

	CINFImageBtnBoth*		m_pImgMotherShipBCUButton;
	CINFImageBtnBoth*		m_pImgMotherShipANIButton;

	

	DWORD			m_nButtonState0;
	DWORD			m_nButtonState1;
	
	USHORT			m_nOption;						// 옵션.
	USHORT			m_nTempOption;
	BOOL			m_bFirstOptionSetting;

	vecINFImportantInfo		m_vecImportBCU;
	vecINFImportantInfo		m_vecImportANI;

	DWORD			m_nShowNum0;
	DWORD			m_nShowNum1;

	float			m_fShowTime;

	BOOL			m_bShow0;
	BOOL			m_bShow1;

	DWORD			m_nButtonPosX0;
	DWORD			m_nButtonPosY0;
	DWORD			m_nButtonPosX1;
	DWORD			m_nButtonPosY1;

	//--------------------------------------------------------------------------//
	//							옵션 창.
	//--------------------------------------------------------------------------//

	BOOL				m_bOptionAct;						// 옵션창 활성화여부.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*		m_pImgOptionBG;						// 옵션 배경.
#else
	CINFImageEx*		m_pImgOptionBG;						// 옵션 배경.	
#endif
	
	CINFImageEx*		m_pImgCheckB[RADIOBUTTON_STATE_NUMBER];  // 래디오 버튼.
	CINFImageBtn*		m_pImgOKB;							// 승인.
	CINFImageBtn*		m_pImgCancelB;						// 취소.
	
	float				m_fTimeOptionChange;				// 옵션을 변경한 시간 체크.
	INT					m_nOptionPosX;
	INT					m_nOptionPosY;
	POINT				m_ptOldPoint;
	BOOL				m_bLButtonClick;					// 윈도우 창 무브 상태.
	//--------------------------------------------------------------------------//



	// 2009. 01. 12 by ckPark 선전 포고 시스템

	CINFWarDeclare*		m_pWarDeclareWindow;

	// end 2009. 01. 12 by ckPark 선전 포고 시스템



	CINFImageEx	*	m_pBalonChat[MOTHERSHIP_INFO_BALON];
	// 모선전 관련 정보
	CD3DHanFont*				m_pFontMotherShipInfo[MOTHERSHIP_MAXLINE];
	MOTHERSHIP_INFO				m_struMotherShipInfo[MOTHERSHIP_MAX_INFLUENCE];
	PREADD_MOTHERSHIP_INFO		m_struPreAddMotherShip[MOTHERSHIP_MAX_INFLUENCE];
	int							m_nPreAddMothershipCnt;
	// 현재 떠 있는 모선
	BOOL			m_bSummonMotherShipInfo[MOTHERSHIP_MAX_INFLUENCE];		// 현재 소환돼있는 모선		
	int				m_nSummonMotherShipCnt;		// 현재 나타난 모선의 수	
	
	// 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
	SIZE			m_sizeMotherShipInfo;
	
	
};

#endif // !defined(AFX_INFMOTHERSHIPMANAGER_H__6585EF9D_3FD5_4DD8_8BE6_E463657B1AFE__INCLUDED_)
