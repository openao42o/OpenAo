// CINFGameMainWisperChat.h: interface for the CINFGameMainWisperChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINFGameMainWisperChat_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
#define AFX_CINFGameMainWisperChat_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "ChatTab_t.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define CHAT_CHATBOX_MAX_X							(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)  // 귓말 메세지박스 최대크기 X
#define CHAT_CHATBOX_MAX_Y							(g_pD3dApp->GetBackBufferDesc().Height / 2)			// 귓말 메세지박스 최대크기 Y

#define WISPERBOX_FIELD_MINIMUM_INIT_HEIGHT			63		// 필드에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)
#define WISPERBOX_CITY_MINIMUM_INIT_HEIGHT			108		// 도시에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)

#define WISPERBOX_MINIMUM_WINDOW_BUTTON_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_BUTTON_Y			2
#define WISPERBOX_MINIMUM_WINDOW_START_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_START_Y			21
#define WISPERBOX_MINIMUM_WINDOW_HEIGHT				22
#define WISPERBOX_MINIMUM_WINDOW_WIDTH				160//140

#define CHAT_BOX_MAX_SIZE_START_X					200
#define CHAT_BOX_MAX_SIZE_START_Y					200

#define WISPERBOX_SYSTEM_MENU_SIZE_X				9
#define WISPERBOX_SYSTEM_MENU_SIZE_Y				9
#define WISPERBOX_SYSTEM_MENU_SPACE					3

#define CHAT_BOX_TOP_IMAGE_SIZE_X					19
#define CHAT_BOX_TOP_IMAGE_SIZE_Y					19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_X				19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_Y				5
#define WISPERBOX_INPUT_IMAGE_SIZE_X				4
#define WISPERBOX_INPUT_IMAGE_SIZE_Y				16
#define WISPERBOX_INPUT_RES_POS_X					19
#define MOVE_RAIL_POSITION_SPACE					30		// 귓말창 이동시 정의한 위치값만큼왔을때 자동으로 옆으로 이동
#define WISPERBOX_NEWMESSAGE_TWINKLE_TIME			1

#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y				7		// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y			6		// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y BOTTOM_Gap

#define CHAT_INPUT_LANGUAGE_X2						224
#define CHAT_INPUT_LANGUAGE_Y2						(g_pD3dApp->GetBackBufferDesc().Height - 27)
#define CHAT_INPUT_LANGUAGE_WH2						22

#define CHAT_BOX_INPUT_WIDTH						279
#define CHAT_BOX_INPUT_X							10
#define CHAT_BOX_MESSAGE_X							6
#else
#define CHAT_CHATBOX_MAX_X							(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)  // 귓말 메세지박스 최대크기 X
#define CHAT_CHATBOX_MAX_Y							(g_pD3dApp->GetBackBufferDesc().Height / 2)			// 귓말 메세지박스 최대크기 Y

#define WISPERBOX_FIELD_MINIMUM_INIT_HEIGHT			63		// 필드에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)
#define WISPERBOX_CITY_MINIMUM_INIT_HEIGHT			108		// 도시에서 귓말 팝업창이 최소화 될때 기준점 y(해상도에서 이값을 뺀것이 y좌표이다)

#define WISPERBOX_MINIMUM_WINDOW_BUTTON_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_BUTTON_Y			2
#define WISPERBOX_MINIMUM_WINDOW_START_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_START_Y			21
#define WISPERBOX_MINIMUM_WINDOW_HEIGHT				22
#define WISPERBOX_MINIMUM_WINDOW_WIDTH				160//140

#define CHAT_BOX_MAX_SIZE_START_X					200
#define CHAT_BOX_MAX_SIZE_START_Y					200

#define WISPERBOX_SYSTEM_MENU_SIZE_X				9
#define WISPERBOX_SYSTEM_MENU_SIZE_Y				9
#define WISPERBOX_SYSTEM_MENU_SPACE					3		// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - (5 -> 3) 변경

#define CHAT_BOX_TOP_IMAGE_SIZE_X					19
#define CHAT_BOX_TOP_IMAGE_SIZE_Y					19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_X				19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_Y				5
#define WISPERBOX_INPUT_IMAGE_SIZE_X				4
#define WISPERBOX_INPUT_IMAGE_SIZE_Y				16
#define WISPERBOX_INPUT_RES_POS_X					19
#define MOVE_RAIL_POSITION_SPACE					30		// 귓말창 이동시 정의한 위치값만큼왔을때 자동으로 옆으로 이동
#define WISPERBOX_NEWMESSAGE_TWINKLE_TIME			1

#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y				7		// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y			6		// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 스크롤 Y BOTTOM_Gap

#define CHAT_INPUT_LANGUAGE_X2						224
#define CHAT_INPUT_LANGUAGE_Y2						(g_pD3dApp->GetBackBufferDesc().Height - 27)
#define CHAT_INPUT_LANGUAGE_WH2						22

#define CHAT_BOX_INPUT_WIDTH						279
#define CHAT_BOX_INPUT_X							10
#define CHAT_BOX_MESSAGE_X							6
#endif



#define SYSTEM_WINDOW_START_X						(g_pD3dApp->GetBackBufferDesc().Width - m_nSystemBoxWidth)//260)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define SYSTEM_WINDOW_START_Y						(g_pD3dApp->GetBackBufferDesc().Height - 183) 
#else 
#define SYSTEM_WINDOW_START_Y						(g_pD3dApp->GetBackBufferDesc().Height - 160)
#endif

#define SYSTEM_SWOW_NOT_SHOWBOX_LINE				6

class CINFImage;
class CD3DHanFont;
class CINFGameMainChat;
class CINFImageBtn;
class CINFImageBtnBoth;
class CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경

class CINFGameMainWisperChat : public CINFBase  
{ 
public:       
	CINFGameMainWisperChat(CAtumNode* pParent);
	virtual ~CINFGameMainWisperChat();

	void SetPresentClickedFlag(BOOL bFlag);

   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void Render();
	void RenderOption();
	virtual void Tick();

	void AddBuffer(const char* strBuffer, DWORD i_dwColorType);
	void ResetHanFontLine();
	void DrawChatWindow(int x, int y, int cx, int cy);
	void DrawMiniChatWindow(int x, int y, int cx, int cy);
	int  ChangeCurrentLineIndex(int CurrentScrollHeight, int NumberOfLine, int ScrollbarLength);
	BOOL SetInterface(BOOL i_bShowInterface);
	BOOL GetInterface();
	void ClearChatTextBox();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CINFGameMainChat			*m_pINFGameMainChat;
	int							m_nVectorIndex;													// CINFGameMainChat에서 관리되는 vector에서의 인덱스
	char						m_szWisperCharacterName[SIZE_MAX_STRING_128];					// 귓말 상대방 CharacterName
	char						m_szWisperCharacterNameForRender[SIZE_MAX_STRING_128];			// 2017-01-13 used for rendering

// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx					*m_pWhisperBox[9];												// 귓말 박스 이미지
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx					*m_pWhisperMiniBox;												// 귓말 박스 이미지
#endif
	CINFImageBtn				*m_pWhisperBoxMin;												// 귓말 최소화 버튼.
	CINFImageBtnBoth			*m_pWhisperBoxVoice;											// 귓말 음성 버튼.
	CINFImageBtn				*m_pWhisperBoxHide;												// 귓말 인터페이스 숨기기.
	CINFImageBtn				*m_pWhisperBoxClose;											// 귓말 클로즈 버튼.
// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx					*m_pWhisperBoxTextInput[3];										// 귓말 박스 이미지

	BOOL						m_bWindowSizeMax;												// 최대 버튼 선택 상태
	BOOL						m_bWindowSizeMin;												// 최소 버튼 선택 상태
	BOOL						m_bShowChatBox;													// 채팅 박스 스위치 
	BOOL						m_bShowInterface;												// 인터페이스 박스 보이는가?
	BOOL						m_bChatMode;
	BOOL						m_bMoveChatBoxHandle;											// 채팅 박스 이동 핸들
	BOOL						m_bPresentClickedFlag;											// 채팅 박스가 선택되었는지의 플래그
	BOOL						m_bScrollLock;													// SCROLL BAR 이동 플래그
	BOOL						m_bShowNameFlag;												// 미니 귓말창에 이름을 보여주는 플래그
	BOOL						m_bNewMassage;													// 새로운 메세지가 왔음을 알려주는 플래그



	int							m_nWisperBoxSizeChangeType;										// 채팅 박스 사이즈 조정 Type
	int							m_nWisperBoxWidth;												// 채팅박스의 넒이 
	int							m_nWisperBoxHeight;												// 채팅박스의 높이
	int							m_nWisperBoxStartX;												// 채팅박스의 시작위치 x 
	int							m_nWisperBoxStartY;												// 채팅박스의 시작위치 y
	int							m_nWisperBoxStartXOld;											// 채팅박스의 시작위치 이전x 
	int							m_nWisperBoxStartYOld;											// 채팅박스의 시작위치 이전y
	int							m_nWisperBoxWidthOld;											// 채팅박스의 넒이 이전 값x 
	int							m_nWisperBoxHeightOld;											// 채팅박스의 높이 이전 값y
	int							m_nWisperBoxSortMiniSizePosY;									// 채팅박스가 최소화일때 정렬
	int							m_nWisperBoxArrowImage;											// 채팅박스의 마우스 포인트 
	int							m_nCurrentRelScrollPosY;
	int							m_nCurrentRelScrollOldPosY;

	float						m_fTwinkleTime;													// 새로운 메세지 깜박이는 타이밍
	
	POINT						m_pointBeforeMousePoints;										// 마우스 이전좌표

	CD3DHanFont*				m_pFontInput;
	vector<CD3DHanFont*>		m_vecWisperFontLine;	
	char						m_strInputMessage[256];	

	ChatTab_t					m_WisperChatTab;
private:
	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	BOOL					IsChatingMode();
	void					SetChatMsgBuff(char* i_pImeChat);
	void					InitChatMsgBuff();
	void					RenderCursel();
	int						OnKeyDownCursel(WPARAM wParam, LPARAM lParam);
	int						OnKeyDownFunc(WPARAM wParam, LPARAM lParam);	
	int						OnKeyDownArrow(WPARAM wParam, LPARAM lParam);
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

private:
	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가	
	POINT				m_ptCurselPos;
	char				m_strPreBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// 스트링 앞에 버퍼
	char				m_strNextBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// 스트링 뒤에 버퍼
	char				m_strBkInputMessage[SIZE_MAX_CHAT_MESSAGE];		// 빽업 버퍼
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	
};




// 2008. 12. 29 by ckPark 채팅창 최적화
#define	MAX_SYSTEM_MSG_LINE			60		// 최대 시스템 메세지 라인
// end 2008. 12. 29 by ckPark 채팅창 최적화



class CINFSystemMsgWindow : public CINFBase  
{ 
public:       
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	//CINFSystemMsgWindow(CAtumNode* pParent);
	CINFSystemMsgWindow(CAtumNode* pParent, int nStartX, int nStartY, int nWidth, int nHeight);
	virtual ~CINFSystemMsgWindow();


   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void SetPresentClickedFlag(BOOL bFlag);
	BOOL SetInterface(BOOL i_bShowInterface);
	BOOL GetInterface();
	void Render();
	virtual void Tick();
	void ResetHanFontLine();
	void AddBuffer(const char* strBuffer, DWORD i_dwColorType);
	void DrawChatWindow(int x, int y, int cx, int cy);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	stcuctRateRect GetSysMsgWndRect();

	// 2009-02-19 by bhsohn 시스템창 위치 게임 안넘어가게 함
	void	RefreshSystemBox();

public:
	CINFGameMainChat			*m_pINFGameMainChat;
	int							m_nVectorIndex;													// CINFGameMainChat에서 관리되는 vector에서의 인덱스
	char						m_szSystemCharacterName[SIZE_MAX_STRING_128];					// 귓말 상대방 CharacterName

// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx					*m_pSystemBox[9];												// 귓말 박스 이미지
	CINFImageEx					*m_pSystemBoxMin;												// 귓말 박스 이미지
	CINFImageEx					*m_pSystemBoxClose;												// 귓말 박스 이미지
	CINFImageEx					*m_pSystemBoxName;												// 시스템 박스 이름. 
// end 2011. 10. 10 by jskim UI시스템 변경

	BOOL						m_bWindowSizeMax;												// 최대 버튼 선택 상태
	BOOL						m_bWindowSizeMin;												// 최소 버튼 선택 상태
	BOOL						m_bShowChatBox;													// 채팅 박스 스위치 
	BOOL						m_bShowInterface;												// 인터페이스가 보이는가?
	BOOL						m_bMoveChatBoxHandle;											// 채팅 박스 이동 핸들
	BOOL						m_bPresentClickedFlag;											// 채팅 박스가 선택되었는지의 플래그
	BOOL						m_bScrollLock;													// SCROLL BAR 이동 플래그
	BOOL						m_bShowNameFlag;												// 미니 귓말창에 이름을 보여주는 플래그
	BOOL						m_bNewMassage;													// 새로운 메세지가 왔음을 알려주는 플래그

	int							m_nSystemBoxSizeChangeType;										// 채팅 박스 사이즈 조정 Type
	int							m_nSystemBoxWidth;												// 채팅박스의 넒이 
	int							m_nSystemBoxHeight;												// 채팅박스의 높이
	int							m_nSystemBoxStartX;												// 채팅박스의 시작위치 x 
	int							m_nSystemBoxStartY;												// 채팅박스의 시작위치 y
	int							m_nSystemBoxStartXOld;											// 채팅박스의 시작위치 이전x 
	int							m_nSystemBoxStartYOld;											// 채팅박스의 시작위치 이전y
	int							m_nSystemBoxWidthOld;											// 채팅박스의 넒이 이전 값x 
	int							m_nSystemBoxHeightOld;											// 채팅박스의 높이 이전 값y
	int							m_nSystemBoxSortMiniSizePosY;									// 채팅박스가 최소화일때 정렬
	int							m_nSystemBoxArrowImage;											// 채팅박스의 마우스 포인트 
	int							m_nCurrentRelScrollPosY;
	int							m_nCurrentRelScrollOldPosY;

	float						m_fTwinkleTime;													// 새로운 메세지 깜박이는 타이밍
	
	POINT						m_pointBeforeMousePoints;										// 마우스 이전좌표

	
	// 2008. 12. 29 by ckPark 채팅창 최적화
	//CD3DHanFont*				m_pFontInput;
	// 각 라인별 폰트
	CD3DHanFont*				m_pFontInput[MAX_SYSTEM_MSG_LINE];
	// end 2008. 12. 29 by ckPark 채팅창 최적화


	vector<CD3DHanFont*>		m_vecSystemFontLine;	
	char						m_strInputMessage[256];	

	ChatTab_t					m_SystemChatTab;
	float						m_fTimeOfShowChat[SYSTEM_SWOW_NOT_SHOWBOX_LINE];


	
	// 2008. 12. 29 by ckPark 채팅창 최적화
	// 컬링된 문자열을 저장할 자료형
	typedef std::list< vector<string> >				CulledStringList;
	typedef std::list< vector<string> >::iterator	CulledStringListItor;
	
	CulledStringList	m_listCulledString;		// 컬링된 문자열을 저장할 변수
	
	
	void	BuildCulledString( void );			// 컬링된 문자열을 만든다

	
	// end 2008. 12. 29 by ckPark 채팅창 최적화


};
#endif
