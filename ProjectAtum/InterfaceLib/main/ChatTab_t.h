#ifndef _CHATTAB_T_H_
#define _CHATTAB_T_H_

#include "AtumParam.h"

#define CHAT_BUFFER_NUMBER					200							// 채팅 최대 저장 버퍼수


typedef struct ChatTab
{
	char *		pszChatString;											// 한라인의 채팅 내용을 저장
	DWORD		dwColorType;											// 이라인의 Color
	DWORD		dwType;													// 채팅의 종류(전체, 전쟁, 아레나, 파티등)
	ChatTab()
	{
		pszChatString = NULL;
		dwColorType = 0;
		dwType = 0;
	}
} ChatLine_t;

class ChatTab_t
{
public:
	ChatTab_t(): m_nStoredLineCounts(0), m_nStoreStartIndex(0), m_nStoreEndIndex(0), m_nRenderStartIndex(0)
	{
	}       
	virtual ~ChatTab_t(); 

	BOOL IsValidChatLineIndex(int i_nIdx);
	void PushChatLine(const char *i_szChatLine, DWORD i_dwColorType, DWORD i_dwType = 0);
	int ChatLineLength(int i_nIdx);
	char* GetChatLine(int i_nIdx); 
	DWORD GetColorType(int i_nIdx);
	DWORD GetType(int i_nIdx);
	int IncreaseRenderStartIndex(void);
	int DecreaseRenderStartIndex(void);
	int SetRenderStartIndexWithScrollPositon(int i_nScrMoveRange, int i_nCurScrPosition);
	int GetScrollPositionWithRenderStartIndex(int i_nScrMoveRange, int i_nRanderLine);

	// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵업
	void GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER]);
	

	ChatLine_t		m_ArrChatLine[CHAT_BUFFER_NUMBER];
	int				m_nStoredLineCounts;								// 저장된 채팅의 라인수
	int				m_nStoreStartIndex;									// 저장된 채팅의 시작 인덱스
	int				m_nStoreEndIndex;									// 저장된 채팅의 마지막 인덱스
	int				m_nRenderStartIndex;								// 채팅 창에 Render하기 위한 시작 인덱스
};



#endif// _CHATTAB_T_H_endif