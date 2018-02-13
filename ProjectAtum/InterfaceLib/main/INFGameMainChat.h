// INFGameMainChat.h: interface for the CINFGameMainChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMECHAT_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
#define AFX_INFGAMECHAT_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "ChatTab_t.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFGroupImage.h"

// 2007-08-07 by bhsohn 스피커 아이템 추가
//#define CHAT_TAB_NUMBER						6							// 채팅탭 수
#define CHAT_TAB_NUMBER						7							// 채팅탭 수
// 2008-07-01 by dgwoo 스피커 아이템 
#define VOICECHAT_SPK_W						65
#define CHAT_MENULIST_NUMBER				13

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define CHAT_NOT_SHOWBOX_LINE				6							// 채팅 박스가 보이지 않을때 최대보여주는 줄수
#define CHAT_TIME_OF_NOT_SHOW_BOX			10.0f						// 채팅 박스가 보이지 않을때 최대보여주는 시간 공지
#define CHAT_TIME_OF_NOT_SHOW_BOX_GM		20.0f						// 채팅 박스가 보이지 않을때 최대보여주는 시간
#define CHAT_CHATBOX_START_DEFAULT_MIN_X	280							// 채팅 박스 시작시 최소 X 
#define CHAT_CHATBOX_START_DEFAULT_MIN_Y	120							// 채팅 박스 시작시 최소 Y

#define CHAT_CHATBOX_START_DEFAULT_MAX_X	(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)	// 채팅 박스 시작시 최대 X 
#define CHAT_CHATBOX_START_DEFAULT_MAX_Y	(g_pD3dApp->GetBackBufferDesc().Height / 2)			// 채팅 박스 시작시 최대 Yt

#define CHAT_BUTTON_NORMAL					0							// 버튼상태 노말
#define CHAT_BUTTON_DOWN					1							// 버튼상태 다운
#define CHAT_BUTTON_UP						2							// 버튼상태 업

#define CHAT_SELECT_IMAGE_0					0							// 이미지 인덱스 0 번선택 
#define CHAT_SELECT_IMAGE_1					1							// 이미지 인덱스 0 번선택 
#define CHAT_SELECT_IMAGE_2					2							// 이미지 인덱스 0 번선택 

// 2008-08-19 by bhsohn 채팅버퍼 버그 수정
//#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_HELPDESK			// 저장되는 채팅수 (스피커,모든 채팅,주변, 전쟁, 거래, 맵, 여단, 편대, 아레나, 채팅방, 시스템)
#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_NUMBER_ALL			// 저장되는 채팅수 (스피커,모든 채팅,주변, 전쟁, 거래, 맵, 여단, 편대, 아레나, 채팅방, 시스템)

#define CHAT_FONT_LINE_HEIGHT				15							// 한 라인의 높이
#define CHAT_FONT_WIDTH_ENGLISH				6							// 영문 글자 WIDTH

#define CHAT_CHATBOX_START_X				0							// 채팅박스 시작위치 X
#define CHAT_CHATBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - CHAT_CHATBOX_START_DEFAULT_MIN_Y)		// 채팅박스 시작위치 Y

#define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			60				// 필드에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)
#define CHATBOX_CITY_SHOWCHATBOX_INIT_HEIGHT			70				// 도시에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)

#define CHAT_MENUBOX_START_HEIGHT			38							// 채팅아래 메뉴박스 시작시 크기

#define CHAT_BOX_SHOW_IMAGE					0							// 채팅박스 보이기 이미지 인덱스 
#define CHAT_BOX_HIDE_IMAGE					1							// 채팅박스 감추기 이미지 인덱스 

#define CHAT_MENUBOX_START_X				1							// 메뉴박스 시작위치 X
#define CHAT_MENUBOX_WIDTH					48							// 메뉴박스 넒이 
#define CHAT_MENUBOX_HEITHT					15							// 메뉴박스 높이 
#define CHAT_MENUBOX_SELECT_COUNT			5							// 메뉴박스 갯수  
#define CHAT_MENUBOX_SELECT_STATE			2							// 메뉴박스 상태
#define CHAT_MENUBOX_GAB_WIDTH				4							// 메뉴박스 사이 간격
#define CHAT_MENUBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - 20)		// 메뉴박스 시작위치 Y

#define CHAT_FONT_START_Y					(g_pD3dApp->GetBackBufferDesc().Height - 23)//30)		// 맨 아래 라인의 시작점(맨 아래부터 그린다) : Y
#define CHAT_FONT_START_X					55//31							// 채팅 시작 라인 : X
#define CHAT_INPUT_FONT_LENGTH              240//189				// 채팅입력창 길이
#define CHAT_STRING_SIZE_GM					84

// 옵션 버튼.
#define CHAT_OPTION_BUTTON_X				309
#define CHAT_OPTION_BUTTON_Y				(g_pD3dApp->GetBackBufferDesc().Height - 34)

#define CHATBOX_CLOSE_GAB					27							// 채팅박스 감추기 Heith - 27
#define CHATBOX_IMAGE_GAB_WIDTH_TOP			18							// 채팅박스 이미지 크기 W Top
#define CHATBOX_IMAGE_GAB_HEITHT_TOP		18							// 채팅박스 이미지 크기 H Top
#define CHATBOX_IMAGE_GAB_WIDTH_BOTTOM		19							// 채팅박스 이미지 크기 W Bottom
#define CHATBOX_IMAGE_GAB_HEITHT_BOTTOM		5							// 채팅박스 이미지 크기 H Bottom
#define CHATBOX_IMAGE_GAB_WIDTH_MIDDLE		4							// 채팅박스 이미지 크기 W Middle
#define CHATBOX_IMAGE_GAB_HEITHT_MIDDLE		1							// 채팅박스 이미지 크기 H Middle
#define CHATBOX_IMAGE_GAB_WINDOWSIZE_CHANGE	4							// 채팅박스 이미지 크기 조절.
#define CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH	9							// 채팅박스 이미지 크기 최소화 버튼 WIDTH
#define CHATBOX_IMAGE_GAB_MINIMIZE_HEIGHT	12							// 채팅박스 이미지 크기 최소화 버튼 HEIGHT
#define CHATBOX_IMAGE_NUMBER				9							// 채팅박스 이미지 갯수 																				
#define CHATBOX_IMAGE_SCROLL_LINE_NUMBER	3							// 스크롤 라인 이미지 갯수 

#define CHATBOX_SCROLLLINE_IMAGE_GAP_X		7							// 스크롤 라인 X Gap 
#define CHATBOX_SCROLL_IMAGE_GAP_X			5//4						// 스크롤 이미지 X Gap		// 2012-03-29 by mspark, 채팅창 스크롤바 위치 변경 - 기존 3에서 5로 변경
#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y		7							// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y	6							// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y BOTTOM_Gap
#define CHATBOX_SCROLL_IMAGE_SIZE_WIDTH		11							// 스크롤 이미지 넒이 
#define CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT	22							// 스크롤 이미지 높이

#define CHAT_OTHER_MENU_ALL					3							// 기타 메뉴 전체 수

#define CHAT_MACRO_INPUT_COUNT				3							// 매크로 입력 가능한 수.
#define CHAT_MACRO_PRINT_GAP				10.0f		  
#else		   
#define CHAT_NOT_SHOWBOX_LINE				6							// 채팅 박스가 보이지 않을때 최대보여주는 줄수
#define CHAT_TIME_OF_NOT_SHOW_BOX			10.0f						// 채팅 박스가 보이지 않을때 최대보여주는 시간 공지
#define CHAT_TIME_OF_NOT_SHOW_BOX_GM		20.0f						// 채팅 박스가 보이지 않을때 최대보여주는 시간
#define CHAT_CHATBOX_START_DEFAULT_MIN_X	253							// 채팅 박스 시작시 최소 X 
#define CHAT_CHATBOX_START_DEFAULT_MIN_Y	120							// 채팅 박스 시작시 최소 Y

#define CHAT_CHATBOX_START_DEFAULT_MAX_X	(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)	// 채팅 박스 시작시 최대 X 
#define CHAT_CHATBOX_START_DEFAULT_MAX_Y	(g_pD3dApp->GetBackBufferDesc().Height / 2)			// 채팅 박스 시작시 최대 Y

#define CHAT_BUTTON_NORMAL					0							// 버튼상태 노말
#define CHAT_BUTTON_DOWN					1							// 버튼상태 다운
#define CHAT_BUTTON_UP						2							// 버튼상태 업

#define CHAT_SELECT_IMAGE_0					0							// 이미지 인덱스 0 번선택 
#define CHAT_SELECT_IMAGE_1					1							// 이미지 인덱스 0 번선택 
#define CHAT_SELECT_IMAGE_2					2							// 이미지 인덱스 0 번선택 

// 2008-08-19 by bhsohn 채팅버퍼 버그 수정
//#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_HELPDESK			// 저장되는 채팅수 (스피커,모든 채팅,주변, 전쟁, 거래, 맵, 여단, 편대, 아레나, 채팅방, 시스템)
#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_NUMBER_ALL			// 저장되는 채팅수 (스피커,모든 채팅,주변, 전쟁, 거래, 맵, 여단, 편대, 아레나, 채팅방, 시스템)

#define CHAT_FONT_LINE_HEIGHT				15							// 한 라인의 높이
#define CHAT_FONT_WIDTH_ENGLISH				6							// 영문 글자 WIDTH

#define CHAT_CHATBOX_START_X				0							// 채팅박스 시작위치 X
#define CHAT_CHATBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - CHAT_CHATBOX_START_DEFAULT_MIN_Y)		// 채팅박스 시작위치 Y

#define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			44				// 필드에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)
#define CHATBOX_CITY_SHOWCHATBOX_INIT_HEIGHT			89				// 도시에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)

#define CHAT_MENUBOX_START_HEIGHT			38							// 채팅아래 메뉴박스 시작시 크기

#define CHAT_BOX_SHOW_IMAGE					0							// 채팅박스 보이기 이미지 인덱스 
#define CHAT_BOX_HIDE_IMAGE					1							// 채팅박스 감추기 이미지 인덱스 

#define CHAT_MENUBOX_START_X				5							// 메뉴박스 시작위치 X
#define CHAT_MENUBOX_WIDTH					48							// 메뉴박스 넒이 
#define CHAT_MENUBOX_HEITHT					15							// 메뉴박스 높이 
#define CHAT_MENUBOX_SELECT_COUNT			5							// 메뉴박스 갯수  
#define CHAT_MENUBOX_SELECT_STATE			2							// 메뉴박스 상태
#define CHAT_MENUBOX_GAB_WIDTH				4							// 메뉴박스 사이 간격
#define CHAT_MENUBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - 20)		// 메뉴박스 시작위치 Y

#define CHAT_FONT_START_Y					(g_pD3dApp->GetBackBufferDesc().Height - 23)//30)		// 맨 아래 라인의 시작점(맨 아래부터 그린다) : Y
#define CHAT_FONT_START_X					55//31							// 채팅 시작 라인 : X
#define CHAT_INPUT_FONT_LENGTH              240//189				// 채팅입력창 길이
#define CHAT_STRING_SIZE_GM					84

// 옵션 버튼.
#define CHAT_OPTION_BUTTON_X				310
#define CHAT_OPTION_BUTTON_Y				(g_pD3dApp->GetBackBufferDesc().Height - 18)

#define CHATBOX_CLOSE_GAB					27							// 채팅박스 감추기 Heith - 27
#define CHATBOX_IMAGE_GAB_WIDTH_TOP			18							// 채팅박스 이미지 크기 W Top
#define CHATBOX_IMAGE_GAB_HEITHT_TOP		18							// 채팅박스 이미지 크기 H Top
#define CHATBOX_IMAGE_GAB_WIDTH_BOTTOM		19							// 채팅박스 이미지 크기 W Bottom
#define CHATBOX_IMAGE_GAB_HEITHT_BOTTOM		5							// 채팅박스 이미지 크기 H Bottom
#define CHATBOX_IMAGE_GAB_WIDTH_MIDDLE		4							// 채팅박스 이미지 크기 W Middle
#define CHATBOX_IMAGE_GAB_HEITHT_MIDDLE		1							// 채팅박스 이미지 크기 H Middle
#define CHATBOX_IMAGE_GAB_WINDOWSIZE_CHANGE	4							// 채팅박스 이미지 크기 조절.
#define CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH	9							// 채팅박스 이미지 크기 최소화 버튼 WIDTH
#define CHATBOX_IMAGE_GAB_MINIMIZE_HEIGHT	12							// 채팅박스 이미지 크기 최소화 버튼 HEIGHT
#define CHATBOX_IMAGE_NUMBER				9							// 채팅박스 이미지 갯수 																				
#define CHATBOX_IMAGE_SCROLL_LINE_NUMBER	3							// 스크롤 라인 이미지 갯수 

#define CHATBOX_SCROLLLINE_IMAGE_GAP_X		7							// 스크롤 라인 X Gap 
#define CHATBOX_SCROLL_IMAGE_GAP_X			5//4						// 스크롤 이미지 X Gap		// 2012-03-29 by mspark, 채팅창 스크롤바 위치 변경 - 기존 3에서 5로 변경
#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y		7							// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y	6							// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y BOTTOM_Gap
#define CHATBOX_SCROLL_IMAGE_SIZE_WIDTH		11							// 스크롤 이미지 넒이 
#define CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT	38							// 스크롤 이미지 높이

#define CHAT_OTHER_MENU_ALL					3							// 기타 메뉴 전체 수

#define CHAT_MACRO_INPUT_COUNT				3							// 매크로 입력 가능한 수.
#define CHAT_MACRO_PRINT_GAP				10.0f
#endif

struct sCHATMACROSTRING
{
	char		strMacro[SIZE_MAX_CHAT_MESSAGE];
	BOOL		bAppl;
	float		fTime;

	sCHATMACROSTRING()
	{
		memset(strMacro,0x00,SIZE_MAX_CHAT_MESSAGE);
		bAppl		= FALSE;
		fTime		= 0.0f;
	};

};

typedef enum 
{
	NO_CHANGE_SIZE			= 0,
	LEFT_CHANGE_SIZE		= 1,
	RIGHT_CHANGE_SIZE		= 2,
	BOTTOM_CHANGE_SIZE		= 3,
	TOP_CHANGE_SIZE			= 4,
	LEFTTOP_CHANGE_SIZE		= 5,
	RIGHTTOP_CHANGE_SIZE	= 6,
	LEFTBOTTOM_CHANGE_SIZE	= 7,
	RIGHTBOTTOM_CHANGE_SIZE	= 8	
} CHAT_CHANGE_SIZE_TYPE;

typedef enum 
{
	NO_ARROW_IMAGE			= 0,
	LEFT_ARROW_IMAGE		= 8,
	RIGHT_ARROW_IMAGE		= 8,
	BOTTOM_ARROW_IMAGE		= 9,
	TOP_ARROW_IMAGE			= 9,
	LEFTTOP_ARROW_IMAGE		= 6,
	RIGHTTOP_ARROW_IMAGE	= 7,
	LEFTBOTTOM_ARROW_IMAGE	= 7,
	RIGHTBOTTOM_ARROW_IMAGE	= 6,
	MOVE_ARROW_IMAGE		= 10
} CHAT_CHANGE_ARROW_IMAGE;

// 추가 수정 채팅
#define CHAT_BUTTON_NUMBER3			3	
#define CHAT_BUTTON_NUMBER4			4



// 2008. 12. 29 by ckPark 채팅창 최적화
#define MAX_GAME_MAIN_CHAT_LINE		60				// 최대 채팅 라인수
// end 2008. 12. 29 by ckPark 채팅창 최적화

// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
enum
{
	LEFT_BLOCK,
	RIGHT_BLOCK
};

#define IS_BLOCK_TYPE(x1,x2) x1 < x2  ? LEFT_BLOCK : RIGHT_BLOCK
//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화

class CINFImage;
class CD3DHanFont;
class CINFSystemMsgWindow;
class CINFGameMainWisperChat;
class CINFGroupManager;
class CINFGroupImage;

typedef vector<CINFGameMainWisperChat*>	vectorINFGameMainWisperChatPtr;
class CINFGameMainChat : public CINFBase  
{
public:
	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
	//CINFGameMainChat(CAtumNode* pParent);								// 생성자
	CINFGameMainChat(CAtumNode* pParent, int nWidth, int nHeight);		// 생성자


	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장



	virtual ~CINFGameMainChat();										// 소멸자
	void InitChatData();
   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcWhisperChat(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcMacro(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//int			WndProcVoiceOption(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2008-07-10 by bhsohn 시스템 창 우선순위 추가
	int			WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void		RenderSystemMsg();
	
	void Render();
	void RenderWisperChat();
	void ResetHanFontLine();
	void RenderMacro();
	//void RenderVoiceChatOption();
	virtual void Tick();
	void TickMacro();

	void AddBuffer(
		int nType, const char* strBuffer, DWORD dwColorType,
		const char* szWisperToCharacterName = nullptr,
		const char* szWisperFromCharacterName = nullptr,
		BYTE NameColor = 0);

	void DrawChatWindow(int x, int y, int cx, int cy);
	//void OnSoundButtonClick();
	//void OnVoiceChatInviteButtonClick();
	int  ChangeCurrentLineIndex(int CurrentScrollHeight, int ChatBoxHeight, int NumberOfLine, int ScrollbarLength);

	BOOL IsValidTabIndex(int i_nTabIdx);
	void ProcessWisperChatAddLine(const char* strBuffer, DWORD i_dwColorType, const char* szWisperToCharacterName=NULL, const char *szWisperFromCharacterName=NULL, BYTE NameColor=0);// 귓말시 WisperChat 생성이나 기존 WisperChat에 입력
	void SetWisperBoxMinimumStatusInitPosition(int i_nPosX, int i_nPosY);					// 귓말 대화상자 최소화시 포지션 SetUp
	
	// 추가 수정 채팅 메뉴	05.01.21
	void ViewChatMenuList();
	void EnterCashShop();
	void CheckShopInfo();
	
	//void SetVOIPExit();
	void SetCleanChat();

	BOOL CheckChatTypeMsg(WPARAM wParam, LPARAM lParam);

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	void OnClickFAQ();
	
	// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵�?
	void GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER]);

	// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
	ChatTab_t * GetChatTabMode();
	void ActiveChatOption(BOOL i_bActive);
	void SetChatRoomAct(BOOL i_bActive);
	void SetChatArenaAct(BOOL i_bActive);

	INT WhisperChatSmallWindowCountPlus();
	INT WhisperChatSmallWindowCountMinus(INT i_nIndex);
	void SetWisperChatShowInterface(BOOL i_bInterface);
	void StartMacro();
	void EndMacro();
	//void SetVoiceChatVolum(DWORD i_nVolum);						// 음성채팅 볼륨 조절.
	//void ShowVoiceChatControl(BOOL i_bShow);
	//void SendVoiceGuild();
	//void SendVoiceParty();
	//BOOL EndVoiceGuild();
	//BOOL EndVoiceParty();

	// 2008-07-14 by bhsohn EP3 관련 처리
	BOOL IsChatingMode();
	//void UpdateVoiceChatType(BYTE i_bVoiceType);

	// 2008-10-20 by bhsohn VOIP 버그 수정
	//BOOL PopupVOIPError(BYTE byRqVoiceType);
	//void UpdateVoiceVolumScroll();


	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장

	stcuctRateRect GetChatWndRect(void);

	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장

	void InitChatMsgBuff();		// 2013-02-13 by mspark, 채팅창에 쓰던 글이 특정 상황에서 사라지는 문제 해결 - public 함수로 변경

	bool ChatModeChack(char i_Ctr); // 2013-06-20 by bhsohn 특정 상황 채팅 정보 날라가는 버그 수정


private:
	// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
	DWORD ChangeChatMode(DWORD i_nChatMode);

	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	void RenderCursel(int nShowTemp);

	int OnKeyUpFunc(WPARAM wParam, LPARAM lParam);

	int OnKeyDownCursel(WPARAM wParam, LPARAM lParam);
	int OnKeyDownArrow(WPARAM wParam, LPARAM lParam);
	int OnKeyDownFunc(WPARAM wParam, LPARAM lParam);
	
	//void InitChatMsgBuff();		// 2013-02-13 by mspark, 채팅창에 쓰던 글이 특정 상황에서 사라지는 문제 해결 - public 함수로 변경하기 위해 주석
	void SetChatMsgBuff(char* i_pImeChat);	

	// 클립보드 관련 처리
	void SetClipBoard(char* i_pStr);
	void PasteFromClipBoard(char* o_pStrTxt, int nSize);
	void GetCurselString(char* o_pStrTxt);
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화		
	// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 블럭지정 삭제
	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화		
    //void BlockCleanChat(int chLen);
	void BlockCleanChat(int LeftBlockPos,int RightBlockPos);
	//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화		
	// 채팅모드 체크
//	bool ChatModeChack(char i_Ctr);
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안

	void GetStrPara ( char *o_szStrBuff , char *i_szString , int iParamNum = 0 );

public:
//	CINFImage	*	m_pTabButton[CHAT_MENUBOX_SELECT_COUNT][CHAT_MENUBOX_SELECT_STATE];															 // 채팅 버튼상태(일반, 귓속말, 편대, 여단, 음성)
	CINFImageEx	*	m_pChatBox[CHATBOX_IMAGE_NUMBER];					// 채팅 박스 이미지(기본 틀이미지)
	CINFImageEx	*	m_pScrollLine[CHATBOX_IMAGE_SCROLL_LINE_NUMBER];	// 스크롤 라인 이미지
	CINFImageEx	*	m_pChatViewButton[2][BUTTON_BOTH_STATE_NUMBER];		// 채팅박스 보여주기와 감추기
	CINFImageEx	*	m_pChatShowImage;									// 채팅박스 타이틀
	CINFImageEx	*	m_pScrollButton;									// 스크롤 버튼 이미지

	// 추가수정 2005.01.21
	CINFImageEx	*	m_pNChatInput;									// 채팅 입력창
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*	m_pNChatCash[CHAT_BUTTON_NUMBER4];					// 채팅 채쉬충전 버튼
#endif
	CINFImageEx	*	m_pNChatCheck;										// 채팅 채크마크
	CINFImageEx	*	m_pNChatFAQ[CHAT_BUTTON_NUMBER3];					// 채팅 FAQ버튼
	CINFImageEx	*	m_pNChatMenu[CHAT_MENULIST_NUMBER];					// 채팅 메뉴 리스트
	CINFImageEx	*	m_pNChatMenuView[CHAT_BUTTON_NUMBER3];				// 채팅 메뉴 보이기 버튼
	CINFImageEx	*	m_pNChatCashView[CHAT_BUTTON_NUMBER4];				// 채팅 메뉴 보이기 버튼
	// 2006-03-16 by ispark
	CINFImageEx	*	m_pNChatInputLanguage[INPUT_LANGUAGE_END];			// 입력 언어

	// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
	CINFGroupImage* m_pBtnChatBG;										// 채팅 모드 배경
	CINFImageBtn *	m_pBtnChatMode[9];									// 채팅 모드 버튼. 
	
	DWORD			m_nChatMode;										// 현재 활성화된 채팅.
	CINFImageBtn *	m_pBtnPartyQuick;									// 편대 바로가기.
	CINFImageBtn *	m_pBtnGuildQuick;									// 여단 바로가기.
	BOOL			m_bGuildQuick;
	BOOL			m_bPartyQuick;

	CINFImageBtn *	m_pBtnOption;

	// 2008-05-27 by dgwoo 채팅 옵션 매크로 
	BOOL			m_bShowMacro;	
	BOOL			m_bMacroMove;
	INT				m_nMacroOldX;
	INT				m_nMacroOldY;

	INT				m_nMacroX;
	INT				m_nMacroY;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pChatMacroBG;										// 매크로 배경 이미지.
#else
	CINFImageEx	*	m_pChatMacroBG;										// 매크로 배경 이미지.
#endif
	CINFImageBtnBoth* m_pBtnChatMacro[CHAT_MACRO_INPUT_COUNT];			// 적용 버튼.
	CINFImageBtn *	m_pBtnMacroOK;										// 확인 버튼.
	CINFImageBtn *	m_pBtnMacroCancel;									// 취소 버튼.
	sCHATMACROSTRING m_sMacroStr[CHAT_MACRO_INPUT_COUNT];				// 매크로 스트링 저장.
	char			m_strTempMacro[SIZE_MAX_CHAT_MESSAGE];									// 매크로 스트링.
	WORD			m_nActMacro;										// 현재 채팅을 입력한 창의 번호.
	BOOL			m_bMacroIng;
	WORD			m_nSendMacroNum;									// 보낼 매크로 번호.
	WORD			m_nTransChatCount;									// 2008-08-14 by dgwoo 거래채팅 매크로 기호기 변경.

	


	// 2005-10-10 by ispark VOIP 메뉴
	CINFImageEx	*	m_pNChatVOIPStartMenu[CHAT_BUTTON_NUMBER3];			// 채팅 VOIP 시작 버튼
	CINFImageEx	*	m_pNChatVOIPEndMenu[CHAT_BUTTON_NUMBER3];			// 채팅 VOIP 종료 버튼

//	int				m_nSizeMaxButtonstate;								// 최대 크기 버튼 상태
//	int				m_nSizeMinButtonstate;								// 최소 크기 버튼 상태 
	DWORD			m_nChatSizeBtnNormal;								// 채팅 사이즈 조절 버튼 상태.
	int				m_nChatBoxWidth;									// 채팅박스 폭 
	int				m_nChatBoxHeight;									// 채팅박스 높이 	
	int				m_nChatBoxYPos;										// 채팅박스 Y 위치값 

	//BOOL			m_bLockSoundButton;									// 음성채팅 선택
	BOOL			m_bShowChatBox;										// 채팅박스 보이기 선택 
	BOOL			m_bWindowSizeMax;									// 최대 버튼 선택 상태
	BOOL			m_bWindowSizeMin;									// 최소 버튼 선택 상태
	BOOL			m_bLockHandle;										// 채팅박스 사이즈조절 선택 핸들 
	BOOL			m_bScrollLock;										// 스크롤 락관련
	POINT			m_ptMouse;

	int				m_nCurrentTabIndex;									// 채팅 종류 선택 탭	
	ChatTab_t		m_ArrChatTab;
	// 2008-05-14 by dgwoo EP3채팅 기획 변경.(각 채팅, 마다 따로 기억)
	ChatTab_t		m_ArrChatTabMode[CHAT_TAB_SAVE_COUNT];				// 각 채팅정보 저장.
	

	int				m_nCurrentRelScrollPosY;
	int				m_nCurrentRelScrollOldPosY;
													
	CD3DHanFont*	m_pFontInput;										// 한폰트 설정
	vector<CD3DHanFont*> m_vecFontLine;									// 벡터형식의 한폰트 생성								
												
	char			m_strInputMessage[SIZE_MAX_CHAT_MESSAGE];
	BOOL			m_bChatMode;
	float			m_fTimeOfShowChat[CHAT_NOT_SHOWBOX_LINE];
//	int				m_nShowTimeChatType;
	// 2008-05-22 by dgwoo 시스템 창 추가.
	CINFSystemMsgWindow	*		m_pSystemMsgW;
	vectorINFGameMainWisperChatPtr	m_vectorINFiGameMainWisperChatPtr;
	int				m_nClickedWisperChatBoxVectorIndex;
	int				m_nSmallWindowCount;
	CINFImageEx *		m_pImgWisperBtn[2][BUTTON_BOTH_STATE_NUMBER];
	BOOL			m_bShowMiniWisper;
	BOOL			m_bShowMiniWisperUnder;
	int				m_nWisperBoxMinimumStatusInitPositionX;
	int				m_nWisperBoxMinimumStatusInitPositionY;

	// 멘트 관련
	char m_strMyMent[1024];

	// 채팅 변경 2005.01.13 ydkim
	BOOL			m_bChatCurrentSelect[CHAT_MENULIST_NUMBER];
	int				m_nSelectChatTab;
	float			m_fTimeGm;
	float			m_fTimeInfluenceLeader;
	
	CD3DHanFont*	m_pFontGM;											// 공지용 채팅 창	
//	char			m_strInputGm[3][512];
	vector<string>	m_vecInputGM;
	char			m_strInputGm[1024];
	char			m_strInputInfluenceLeader[512];						// 2006-04-21 by ispark, 세력 지도자 공지
	int				m_nMenuListButtonState;								// 메뉴 버튼 상태
	int				m_nOtherMenuList[CHAT_OTHER_MENU_ALL];				// 그외 기타 메뉴
	BOOL			m_bMenuListUse;										// 메뉴리스트 사용 가능
	BOOL			m_bMenuList;
	BOOL			m_bCheckShopInfo;
	BOOL			m_bCashShopOpen;
	int				m_nOtherCashList;
	int				m_nCashButtonGab;
	BOOL			m_bCashButton;
	BOOL			m_bPKOptionFlag;									// 결투옵션
	BOOL			m_bTradeFlag;										// 거래
	BOOL			m_bHelpDeskRender;
	BOOL			m_bWindowMessageFlag;								// 창끄기 창켜기
	BOOL			m_bSelectInputLanguage;								// 입력 언어 선택

	int				m_nVoiceMenu;										// 음성 메뉴
private:
	// voice chat control center
	//BOOL						m_bShowVoiceChatControl;
	//INT							m_nVCCPosX;
	//INT							m_nVCCPosY;
	//BOOL						m_bVCCMoving;
	//POINT						m_ptVCCOldPos;
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//	CINFGroupImage*				m_pVoiceChatBG;
//#else
//	CINFImageEx*				m_pVoiceChatBG;
//#endif
	//CINFImageBtnBoth*			m_pBothParty;
	//CINFImageBtnBoth*			m_pBothGuild;
	//CINFImageBtnBoth*			m_pBothChat;
	//CINFImageBtnBoth*			m_pBothWhisper;
	//CINFImageEx*				m_pImgSpkVolBar;
	//CINFImageEx			*		m_pImgRadioBtn[2];
	//CINFImageBtn		*		m_pBtnVoiceChatInv;
	//CINFImageBtn		*		m_pBtnVoiceChatOk;
	//CINFImageBtn		*		m_pBtnVoiceChatCan;

	//DWORD						m_nVolum;
	//BOOL						m_bVolumCon;							// 볼륨소리 수정중.
	//float						m_fVolumConTimeGap;						// 
	//POINT						m_ptOldVolumPos;						// 기존의 볼륨 위치.
	//INT							m_nPosVolum;							// 기존위치(0) + m_nPosVolum      : m_nPosVolum = (m_nVolum / VOICECHAT_SPK_W) * 100
	//DWORD						m_nVoiceInput;							// 0 : 키 입력방식, 1 : 자유방식.
	
	int				m_nChinaCnt;
	int				m_nMSPYMode;


	// 2008. 12. 29 by ckPark 채팅창 최적화
	// stl, itor 타입 정의
	typedef std::list< vector<string> >				CulledStringList;
	typedef std::list< vector<string> >::iterator	CulledStringListItor;

	CD3DHanFont*		m_pFontLine[MAX_GAME_MAIN_CHAT_LINE];	// 채팅 각 라인당 폰트
	CulledStringList	m_listCulledString;						// 컬링된 스트링을 보관할 변수

	void	BuildCulledString( void );							// 컬링된 스트링을 m_listCulledString에 만든다

	// end 2008. 12. 29 by ckPark 채팅창 최적화

	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가	
	POINT				m_ptCurselPos;
	POINT				m_ptSelCurselPos;
	char				m_strPreBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// 스트링 앞에 버퍼
	char				m_strNextBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// 스트링 뒤에 버퍼
	char				m_strBkInputMessage[SIZE_MAX_CHAT_MESSAGE];		// 빽업 버퍼
	BOOL				m_bShiftClick;
	BOOL				m_bCaretCursel;
	BOOL				m_bControlCursel;
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

};
#endif
