// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINFAQI_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFGAMEMAINFAQI_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define  INF_FAQ_QUESTION_SIZE	1024
#define  INF_FAQ_ANSWER_SIZE	1024
#define  INF_FAQ_SEARCH_ANSWER_RENDER	8	// 검색 후 찾은 답변 렌더링 수
#define  INF_FAQ_SEARCH_QUESTION_RENDER	8	// 검색 후 찾은 질문 렌더링 수
#define  INF_FAQ_BUTTON_IMG				4	// 버튼 이미지 수

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CINFImageEx;
class CINFGroupImage;
class CINFImageBtn;

typedef struct
{
	char sFAQQuestion[INF_FAQ_QUESTION_SIZE];
	char sFAQAnswer[INF_FAQ_ANSWER_SIZE];

}SFAQINF;

class CINFGameMainFAQ : public CINFBase
{

public:
	CINFGameMainFAQ();
	virtual ~CINFGameMainFAQ();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	vector<SFAQINF>				m_vecFAQInf;		// 전체 FAQ 저장
	vector<SFAQINF>				m_vecFAQAnswer;		// 검색된 FAQ 저장
	vector<string>				m_vecAnswer;
	vector<string>				m_vecQuestion;

	BOOL LoadFAQFile(char *chrTxtName);
	void SearchQuestionTxt(char *strWord);
	void SetNPCTalkInfo( char* strNPCTalk, int nType=0);
	void SetInit();
	void InitBaseString();
private:
	void ToLower(char* pSrc, char* pDst);	// 2008-08-22 by bhsohn FAQ 영문 대/소문자 구별없게 처리

	BYTE ToLowerDeu(BYTE i_ch);				// 2008-10-23 by bhsohn 독일어 대문자 소문자로 변경

public:
	char m_strQuestionWord[INF_FAQ_QUESTION_SIZE];
	char m_strInputMessage[INF_FAQ_QUESTION_SIZE];
	BOOL m_bChatMode;								// 검색 텍스트 입력 가능, 불가능

protected:
	CD3DHanFont			*   m_pFontInput;
	CD3DHanFont			*   m_pFontSearchAnswer[INF_FAQ_SEARCH_ANSWER_RENDER];
	CD3DHanFont			*   m_pFontSearchQusetion[INF_FAQ_SEARCH_QUESTION_RENDER];
	CD3DHanFont			*   m_pFontQuestion[2];

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*			m_pImgBackg;
	//CINFImage*				m_pImgBackg;						// 백그라운드 이미지
	CINFImageEx*			m_pImgOper[2];						// 오퍼레이터 이미지.
	INT						m_nOperatorInfl;					// 오퍼레이터 세력.

	CINFImageBtn*			m_pcloseBtn;						// 닫기
	//CINFImageEx*			m_pImgSearch;						// 서치 이미지.
// 	CINFImageEx*			m_pImgResult;						// 결과 배경.
 	CINFImageEx*			m_pImgSelect;						// 제목 선택 이미지
// 	CINFImageEx*			m_pImgSelectName;					// 타이틀 선택 이미지

	CINFImageEx*			m_pImgButton[INF_FAQ_BUTTON_IMG];	// 버튼 이미지					  
#else
	CINFImageEx*				m_pImgBackg;						// 백그라운드 이미지
	CINFImageEx*				m_pImgOper[2];						// 오퍼레이터 이미지.
	INT						m_nOperatorInfl;					// 오퍼레이터 세력.

	CINFImageEx*			m_pImgSearch;						// 서치 이미지.
	CINFImageEx*			m_pImgResult;						// 결과 배경.
	CINFImageEx*			m_pImgSelect;						// 제목 선택 이미지
	CINFImageEx*			m_pImgSelectName;					// 타이틀 선택 이미지

	CINFImageEx*			m_pImgButton[INF_FAQ_BUTTON_IMG];	// 버튼 이미지
#endif

	CINFScrollBar*			m_pScroll;
	CINFScrollBar*			m_pScrollUnder;

	int						m_nStartFAQPositionX;
	int						m_nStartFAQPositionY;
	int						m_nSelectindexQuestion;
	int						m_nMouseButtonState;

	int						m_nFAQBoxStartXOld;
	int						m_nFAQBoxStartYOld;
	POINT					m_pointBeforeMousePoints;			// 마우스 이전좌표

	BOOL					m_bMoveWindow;
	BOOL					m_bToolTibQuestion;
	float					m_fQuestionScrollText;
};

#endif
