// INFGameMainChat.h: interface for the CINFGameMainChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEHELPDESK_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
#define AFX_INFGAMEHELPDESK_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "RangeTime.h"

#define HELPDESK_ALL_LINE						50			// 도움말 모든줄수
#define HELPDESK_ALL_CHAR						256			// 도움말 한라인에  들어갈수있는 최대 글자수 
#define HELPDESK_ALL_MISSIONCOUNT				10			// 받을수 있는 미션의 갯수 						
#define HELPDESK_FONT_HEIGHT					15			// 한폰트의 높이 
#define HELPDESK_FONT_WIDTH_ENGLISH				6			// 영문 글자 폭

#define HELPDESK_WINDOW_POSITION_X				110			// 처음시작시 윈도우 포지션 X
#define HELPDESK_WINDOW_POSITION_Y				140			// 처음시작시 윈도우 포지션 Y
#define HELPDESK_WINDOW_WIDTH					300			// 처음시작시 윈도우 폭
#define HELPDESK_WINDOW_HEIGHT					200			// 처음시작시 윈도우 높이
#define HELPDESK_WINDOW_TEXT_VIEW_GAP			20			// TextView시 공백 
#define HELPDESK_WINDOW_IMAGE_VIEW_GAP			20			// TextView시 공백 

//g_pD3dApp->GetBackBufferDesc().Width
//g_pD3dApp->GetBackBufferDesc().Height


#define HELPDESK_BUTTON_TWINKLE_TIME			1			// 버튼 깜박이는 타이밍
#define HELPDESK_TEXT_TWINKLE_TIME				0.05f		// 버튼 깜박이는 타이밍
#define HELPDESK_REMOVE_BOX_TIME				5.0f		// 박스가 사라지는 타이밍
#define HELPDESK_VIEW_TIME						60.f		// 박스가 보여지는 시간.

#define HELPDESK_IMAGE_INDEX_0					0			// 이미지 인덱스
#define HELPDESK_IMAGE_INDEX_1					1
#define HELPDESK_IMAGE_INDEX_2					2
#define HELPDESK_IMAGE_INDEX_3					3
#define HELPDESK_IMAGE_INDEX_4					4
#define HELPDESK_IMAGE_INDEX_5					5
#define HELPDESK_IMAGE_INDEX_6					6
#define HELPDESK_IMAGE_INDEX_7					7
#define HELPDESK_IMAGE_INDEX_8					8

#define HELPDESK_KEY_STATE_W					0x00000001
#define HELPDESK_KEY_STATE_A					0x00000002
#define HELPDESK_KEY_STATE_S					0x00000004
#define HELPDESK_KEY_STATE_D					0x00000008

#define HELPDESK_KEY_STATE_CTRL					0x00000001
#define HELPDESK_KEY_STATE_C					0x00000002
#define HELPDESK_KEY_STATE_TAB					0x00000004
#define HELPDESK_KEY_STATE_SPACE				0x00000008
#define HELPDESK_KEY_STATE_Z					0x00000064

#define HELPDESK_MOUSE_STATE_NORMAL				0x00000000
#define HELPDESK_MOUSE_STATE_UP					0x00000004
#define HELPDESK_MOUSE_STATE_DOWN				0x00000008
#define HELPDESK_MOUSE_STATE_LEFT				0x00000001
#define HELPDESK_MOUSE_STATE_RIGHT				0x00000002
#define HELPDESK_MOUSE_STATE_MDOWN				0x00000064

#define HELPDESK_MOUSE_STATE_RBDOWN				0x00000002
#define HELPDESK_MOUSE_STATE_LBDOWN				0x00000001
#define HELPDESK_MOUSE_STATE_WHEEL				0x00000004

#define HELPDESK_QUICK_REPAIR					7002600
#define HELPDESK_QUICK_SHIELD					7002830
#define HELPDESK_QUICK_FUEL						7000790

#define HELPDESK_TALK_SIZE						1024
#define HELPDESK_TEXT_COLOR						RGB(255,255,0) // 도움말 글씨색 (노랑)



// 2007-01-29 by bhsohn 신규유저 미션 인터페이스
#define	MAX_STEP_HELPDESK_RENDER				10
#define	STEP_HELPDESK_TIME						0.1f
// end 2007-01-29 by bhsohn 신규유저 미션 인터페이스

class CINFImage;
class CD3DHanFont;
class CINFScrollBarEX;
class CINFImageEx; 

typedef struct HelpLowLevel
{
	CINFScrollBarEX *		pScrollBarEx;				// 스크롤

	char					msgLowLevelHelp[1024];
	BOOL					bLowLevelHelp;				// 도움말 사용 하는가?
	BOOL					bLowLevelHelpRender;		// 도움말 렌더링 가능한가?

	int						nStartX;
	int						nStartY;

	HelpLowLevel()
	{
		pScrollBarEx		= NULL;
		bLowLevelHelp		= FALSE;
		bLowLevelHelpRender = FALSE;

		nStartX				= 0;
		nStartY				= 0;

		memset(msgLowLevelHelp, 0x00, 1024);
	}

	~HelpLowLevel()
	{
		pScrollBarEx	= NULL;
		memset(msgLowLevelHelp, 0x00, 1024);
	}
	
} HELP_LOW_LEVEL;

class CINFGameHelpDesk : public CINFBase
{
public:
	CINFGameHelpDesk(CAtumNode* pParent);
	virtual ~CINFGameHelpDesk();

	int GetHelpDeskState() { return m_nCharacterStatePos; }

   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void Tick();
	void Render();
	void DrawHelpWindow(int x, int y, int cx, int cy);		// 도움말 창
	void ResetHanFontLine();

	void RenderNewCharacter(int x, int y);					// 새로운 캐릭터를 만들었을때 보여주는 도움말
	void RenderCityHelpDesk(int x, int y);					// 시티내에서 도움말 보이기	
	void RenderFieldHelpDesk(int x, int y);					// 필드상에서 도움말 보이기	
	void RenderMissionHelpDesk(int x, int y);				// 미션 진행시 도움말 보이기
	
	void RenderMousePressHelpDesk(int mouseinput);			// Mouse입력시 도움말

	void ZoomInHelpDesk(int x, int y, int cx, int cy);		// HelpDesk 상태를 점점크게 만든다
	void RenderKeyPressHelpDesk(int selectkey);				// Key입력시 도움말
	int  SetHelpDeskString(int HWidth, int indexString, char* szString);
	void SetHelpDeskBox(int nPosX, int nPosY, int nWidth, int nHelpIndex, BOOL bTypingText, float fRemoveBoxTime, char* szString=NULL);
	int  SetQuickSlotItem();
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void StartHelpDesk();

	void TransferOneCharTemp2Talk(void);
	void SetStringMissionItem(char *strItem){strcpy(m_strMissionItem, strItem);m_bRenderMissionItemInfo=TRUE;}
	void SetLowLevelStringbyType(int nType, int nData1 = 0);

	void ReShowHelpDesk();		

protected:
	void InitTextBox();
	void RenderOnInfo();	

public:
	float						m_fHelpTextCheckTime;
	int							m_nHelpDeskWidth;
	int							m_nHelpDeskHeight;
	int							m_nHelpDeskIndex;

	BOOL						m_bRenderMissionItemInfo;
	float						m_fHelpTextMission;
	char						m_strMissionItem[512];

	// 스탯과 미션 도움말 (선택미션을 완료시점 전까지 출력한다.)
	CINFImageEx*					m_pLowLevelHelpflash;					// 2006-08-22 by ispark
	HELP_LOW_LEVEL				m_stLowLevelHelp[HELP_SPECIAL_COUNT];

protected:
	BOOL						m_bRemoveBoxTextView;		// 도움말 글 지우기
	BOOL						m_bButtonTwinkle;			// 버튼 깜박이기
	BOOL						m_bButtonView;				// 버튼 보이기
	BOOL						m_bRemoveBox;				// 도움말 박스 지우기
	BOOL						m_bQuestAlarmFlag;			// 퀘스트 도움말 박스 보이기

	BOOL						m_bHelpDeskBoxView;			// 풍선도우말 보이기 (Render)	
	BOOL						m_bButtonViewNPC;			// NPC 버튼 보이기	
	BOOL						m_bMouseHelpDesk;			// 마우스 도움말
	BOOL						m_bKeyHelpDesk;				// 키보드 도움말
	BOOL						m_bKeyHelpDeskTAB;			// Tab 도움말 
	BOOL						m_bKeyHelpDeskC;			// C 도움말 
	BOOL						m_bKeyHelpDeskCTRL;			// 컨트롤 키도움말 
	BOOL						m_bKeyHelpDeskSPACE;		// SPACE 도움말 
	BOOL						m_bKeyHelpDeskZ;			// Z키 도움말
	BOOL						m_bKeyTypingFlag;			// 타이핑 형태 텍스트 뷰
	BOOL						m_bDrawBoxType;				// 박스 드로우시 페이드인 페이드 아웃효과
	BOOL						m_bShowCityMouse;			// 마우스 우드래그 보여줌 : TRUE --> 더이상 안보여줌

	CINFImageEx*					m_pHelpBox[9];				// 도움말 상자
	CINFImageEx*					m_pHelpButton[8];			// 도움말 버튼
	CINFImageEx*					m_pHelpButtonNPC[2];		// 미션과 닫기 버튼	
	CINFImageEx*					m_pHelpKeyImage[5];			// 도움말 키보드 이미지
	CINFImageEx*					m_pHelpMouseImage[8];		// 도움말 마우스 이미지
	CINFImageEx*					m_pHelpKeyImage2[8];		// 도움말 키보드 이미지

	int							m_nCharacterStatePos; 	
	int							m_nHelpDeskStartXPos;
	int							m_nHelpDeskStartYPos;
	int							m_nIndexCount;
	int							m_nkeyHelpDesk;				// 키보드 도움말
	int							m_nMouseHelpDeskMove;		// 마우스 도움말
	int							m_nMouseHelpDeskState;		// 마우스 도움말
	int							m_nKeyHelpDeskKEY;			// Kye State 저장
	int							m_nCityButtonNumber;		// 도시 버튼 수

	float						m_fTwinkleTime;												// 버튼 깜박이는 타이밍
	float						m_fRemoveBoxTime;											// 박스가 지워지는 타이밍
	float						m_fTextTwinkleTime;
	float						m_fTextUntilTime;
	
	vector<CD3DHanFont*>		m_vecHelpDeskFontLine;
	vector<string>				m_vecHelpDeskTalk;
	vector<string>				m_vecHelpDeskString;

	vector<string>				m_vecStringTemp;			// 한글자씩 렌더링하기위한 임시 변수
	char						m_szTransferString[512];
	int							m_nTransferVectorIndex;
	char						*m_szTransferPointer;
	char*						m_strPrev;	

	CRangeTime					m_tBlinkTime;
	CRangeTime					m_tBlinkTimeStart;	
	int							m_nBlinkButtonIndex;	// 깜빡이고 있는 버튼 인덱스(하단)
	BOOL						m_bShowMiniMapIndex;

	BOOL						m_bKeyDown[4];
	int							m_nHelpCloseCount;
	BOOL						m_bShowCityEnd;
	// 2005-05-17 by ydkim	
	CD3DHanFont*				m_pFontMissionItem;

	// 스탯과 미션 도움말 (선택미션을 완료 시킬때까지.)
	CINFImageEx*					m_pLowLevelHelpBox;			// 2006-08-22 by ispark
	CD3DHanFont*				m_pLowLevelFont;

	int							m_nShowStep;									// 도움말을 출력할때 부드럽게 하기위한 변수.
	float						m_fHelpPopupTime;								// 도움말을 보이게 할때시간 체크.
};

#endif // !defined(AFX_INFGAMEMAINCHAT_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
