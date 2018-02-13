// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMENULIST_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFMENULIST_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define MENU_LIST_BUTTON_STATE_COUNT		4		// 메뉴 버튼 상태
#define MENU_LIST_BUTTON_STATE_NOM			0
#define MENU_LIST_BUTTON_STATE_OVE			1
#define MENU_LIST_BUTTON_STATE_PUH			2
#define MENU_LIST_BUTTON_STATE_DIS			3

// 추가시 전체카운트와 인덱스 등록
// 2012-06-14 by isshin 아템미리보기 - 메뉴 리스트 전체 수 (8 -> 9)
#ifdef C_ENEMY_INFO_ISSHIN
	#define MENU_LIST_ALL_COUNT					9			
#else
	#define MENU_LIST_ALL_COUNT					8			
#endif
// end 2012-06-14 by isshin 아템미리보기 - 메뉴 리스트 전체 수 (8 -> 9)

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	#define MENU_LIST_WIDTH						113		// 메뉴 리스트 폭
	#define MENU_LIST_HEIGHT					18		// 메뉴 리스트 높이
#else
#define MENU_LIST_WIDTH						77		// 메뉴 리스트 폭
#define MENU_LIST_HEIGHT					18		// 메뉴 리스트 높이
#endif

#define MENU_LIST_MAX_NAME					32

typedef enum 
{
	MENU_LIST_INFR		=				0,		// 친구 등록
	MENU_LIST_INGU		=				1,		// 길드 등록
	MENU_LIST_INPA		=				2,		// 파티 등록
	MENU_LIST_INPV		=				3,		// 1:1
	MENU_LIST_INTR		=				4,		// 거래 등록
	MENU_LIST_ININ		=				5,		// 캐릭 정보
	MENU_LIST_INVO		=				6,		// 음성 채팅
	MENU_LIST_INWI		=				7,		// 귓말	
	MENU_LIST_INIT		=				8		// 2012-06-14 by isshin 아템미리보기 - 타캐릭 아이템 정보
} MENU_LIST_BUTTON_IMAGE;

// 벡터 정렬용 임시 변수
typedef struct
{
	int nIndex;
	int nCIndex;	
} Sort_ChartIndex;

struct sort_Size
{
	bool operator()(Sort_ChartIndex p1, Sort_ChartIndex p2)
	{
		 return p1.nCIndex < p2.nCIndex; 
	}
};

class CINFImage;
class DataHeader;
class CGameData;
class CINFImageEx;

class CINFMenuList : public CINFBase
{

public:
	CINFMenuList();
	virtual ~CINFMenuList();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	char					m_strOtherName[MENU_LIST_MAX_NAME];
	BOOL					m_bFALSEMenu[MENU_LIST_ALL_COUNT];
	
	int						m_nMenuListStartX;
	int						m_nMenuListStartY;
	CEnemyData			*	m_vecpTargetEnemy;
	
	void RenderMenuList(int x, int y);
	void ProcessSelectMenu(int nIndex, char* strName);
	int  IndexListChange(int nIndex);
	void MenuListSort();

	// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
	void SetItemInfoSecret		(BOOL SecretFlag);				
	void SetItemInfoSecretDone	(BOOL SecretDone);				
	BOOL GetItemInfoSecretDone	(){return m_bItemInfoSecretDone;}
	// end 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부

protected:
	CINFImageEx*				m_pImgButton[MENU_LIST_ALL_COUNT][MENU_LIST_BUTTON_STATE_COUNT];
	CINFImageEx*				m_pImgEnemySelect;
	BOOL					m_bUseMenu[MENU_LIST_ALL_COUNT];
	BOOL					m_bItemInfoSecret;								// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
	BOOL					m_bItemInfoSecretDone;							// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부

	int						m_nButtonState[MENU_LIST_ALL_COUNT];
	int						m_nListChart[MENU_LIST_ALL_COUNT];				// 리스트 순서 변환
	
	vector<int>				m_vecRenderList;								// 렌더링 순서 관련 리스트
};

#endif
