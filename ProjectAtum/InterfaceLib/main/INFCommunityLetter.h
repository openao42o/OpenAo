// INFCommunityLetter.h: interface for the CINFCommunityLetter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYLETTER_H__DFF38A04_9D63_44D9_B403_CF87C3A3D868__INCLUDED_)
#define AFX_INFCOMMUNITYLETTER_H__DFF38A04_9D63_44D9_B403_CF87C3A3D868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFEditBox;
class CINFCurselEditBox;
class CINFGroupImage;

// 편지 모드
#define		LETTER_MODE_READ		0	// 편지 읽기
#define		LETTER_MODE_WRITE		1	// 편지 쓰기
#define		MAX_LETTER_MODE			2

// 편지 읽기
#define		LETTER_READ_MODE_LIST	0	// 편지 리스트
#define		LETTER_READ_MODE_INFO	1	// 편지 내용

// 편지 읽었는지 여부
#define		LETTER_READ_NORES		0	// 읽지 않은 메일
#define		LETTER_READ_RES			1	// 읽은 메일
#define		MAX_LETTER_READ			2



class CINFCommunityLetter  : public CINFBase  
{
public:
	CINFCommunityLetter(CAtumNode* pParent);
	virtual ~CINFCommunityLetter();	

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(POINT ptPos);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


	int WndProcLetterRead(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcLetterWrite(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 편지 읽기 정보
	void AddReadLetter(BOOL bAllMail, BOOL bReadMail, UID64_t   LetterUID, char* pSendUser, char* pLetterTitle, ATUM_DATE_TIME	atimeMail);
	void RqDelReadLetter(BOOL bAllMail, UID64_t   LetterUID);
	void DelReadLetter();
	void SetLetterInfo(BOOL bAllMail, UID64_t LetterUID, char* Content); // 편지 내용	
	void RefreshLetterInfo();
	void RqLetterTitle(BOOL bLetterFirstRq);								// 편지 리스트요청
	void RqAllLetterTitle();		// 전체 편지 요청 

	void CheckNotReadMail();		// 읽지않은 메일 체크
	void FieldSocketChatLetterDelOk();

	void ChangeLetterMode(int nLetterMode, int nSubLetterMode, char* pUser, 
											BOOL bAllMail, UID64_t uReadLetterUID);

	void UpdateBtnPos();


private:
	// 
	int OnLButtonDown(POINT pt);

	void InitLetterMode();

	// 편지 읽기
	void RenderLetterRead(POINT ptBkPos);	
	int OnLButtonDown_ReadList(POINT pt);
	int OnLButtonDown_ReadInfo(POINT pt);
	int OnLButtonUp_Read(POINT pt);
	void RenderReadLetter(POINT ptBkPos);
	int GetReadLetterCurselPos(POINT pt);
	void OnClickReadLetter(int nSelReadLetterPos);		// 아이템 클릭	
	void TestReadDb();
	
	void RqLetterRead(BOOL bAllMail, UID64_t uReadLetterUID);								// 편지 내용 요청	
	BOOL GetLetterItemInfo(BOOL bAllMail, UID64_t LetterUID, structReadLetterItemInfo* pItem);	// 편지 정보 얻어옴

	// 편지 쓰기
	void OnClickLetterSend(BOOL bAllMail);
	void RenderLetterWrite(POINT ptBkPos);
	void OnClickWriteEditBox(int nSelLetterWrite, CINFEditBox*	pSelEdit);	


private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	 m_pBack;		// 배경														  
#else 
	CINFImageEx*	m_pBack;		// 배경
#endif
	
	// 읽기/쓰기
	int				m_nLetterMode;
	CINFImageEx*	m_pLetterMode[MAX_LETTER_MODE];		
	
	// 선택
	int						m_nSelReadLetterPos;
	CINFImageEx*			m_pImgSelect;

	// 읽기 관련 버튼
	int				m_nReadLetterMode;
	CINFImageBtn*				m_pReadBtn;			// 읽기
	CINFImageBtn*				m_pReadDelBtn;		// 삭제	
	CINFArenaScrollBar*			m_pScrollRead;			// 스크롤
	vector<structReadLetterItemInfo>		m_vecReadLetter;
	CINFImageEx*				m_pLetterRead[MAX_LETTER_READ];
	
	CINFImageEx*				m_pLetterReadListBk;		
	CINFImageEx*				m_pLetterReadBk;

	// 읽기
	UID64_t						m_uReadLetterUID;
	BOOL						m_bAllMail;					// 전체메일여부

	CINFEditBox*				m_pEditReadReceiveUser;		// 받는 유저
	CINFEditBox*				m_pEditReadTitle;			// 제목
	CINFEditBox*				m_pEditReadInfo;			// 내용
	CINFArenaScrollBar*			m_pScrollReadInfo;			// 스크롤 

	CINFImageBtn*				m_pReadResponseBtn;			// 읽기 답장
	CINFImageBtn*				m_pReadListDelBtn;				// 읽기 삭제
	CINFImageBtn*				m_pReadBackBtn;				// 읽기 뒤로

	// 쓰기 관련 버트
	CINFImageBtn*				m_pSendBtn;			// 보내기
	CINFImageBtn*				m_pAllSendBtn;		// 전체 발송

	// 쓰기
	CINFEditBox*				m_pEditWriteSendUser;		// 보내는 유저
	CINFEditBox*				m_pEditWriteReceiveUser;		// 받는 유저
	CINFEditBox*				m_pEditWriteTitle;			// 제목
	// 2008-06-24 by bhsohn 편지에서 방향키 먹게 적용
	CINFCurselEditBox*				m_pEditWriteInfo;			// 내용
	CINFArenaScrollBar*			m_pScrollWrite;			// 스크롤 

	int							m_nSelLetterWrite;
	

	// 편지 폰트 
	CD3DHanFont*				m_pFontLetter;

	BOOL						m_bLetterFirstRq;			// 처음 요청한 메일이냐?
};

#endif // !defined(AFX_INFCOMMUNITYLETTER_H__DFF38A04_9D63_44D9_B403_CF87C3A3D868__INCLUDED_)
