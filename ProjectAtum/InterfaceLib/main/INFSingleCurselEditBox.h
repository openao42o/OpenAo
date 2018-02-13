// INFSingleCurselEditBox.h: interface for the CINFSingleCurselEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSINGLECURSELEDITBOX_H__A7F38416_E734_4EB2_8193_D7A285F0A0CA__INCLUDED_)
#define AFX_INFSINGLECURSELEDITBOX_H__A7F38416_E734_4EB2_8193_D7A285F0A0CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define		MAX_EDIT_SINGLE_LINE_BUFF		512

class CD3DHanFont;

class CINFSingleCurselEditBox  : public CINFBase
{
public:
	CINFSingleCurselEditBox();
	virtual ~CINFSingleCurselEditBox();

	HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	//virtual void Render();
	void Render(int nScroll, int nOnePageLine);
	void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus);
	//virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL);
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 버튼 클릭 
	virtual BOOL OnLButtonDown(POINT ptPos);	

	void Tick();

	BOOL IsEditMode();
	
	// 스트링 지정
	int  SetString(char* pTxt, int nBufferCnt, int nScrollStep, BOOL bCurselInit);
	void GetString(char* pTxt, int nBufferCnt);	// 스트링 얻어옴.

	// 스트링 초기와
	void InitString();	

	// 위치 변경 
	void SetPos(int nPosX, int nPosY);

	// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.
	// String 출력 Width사이즈값 대입.
	inline void SetStringWidth ( const int a_iWidth ) { m_nStrWidth = a_iWidth; }
	inline int GetStringWidth ( void ) { return m_nStrWidth; }

	// String 입력 제한 활성/비활성화.
	inline void SetEnable_InputLimit ( const bool a_bEnable ) { m_bEnable_InputLimit = a_bEnable; }
	inline bool GetEnable_InputLimit ( void ) { return m_bEnable_InputLimit; }

	// String 입력 제한 버퍼 사이즈 대입.
	inline void SetBuffLen_InputLimit ( const int a_nLimit_BuffLen ) { m_iLimit_InputBuffLen = a_nLimit_BuffLen; }
	inline int GetBuffLen_InputLimit ( void ) { return m_iLimit_InputBuffLen; }

	inline void SetInputNumMode ( const bool a_bInputNumMode ) { m_bInputNumMode = a_bInputNumMode; }
	inline bool GetInputNumMode ( void ) { return m_bInputNumMode; }

	// 출력 좌표 가져오기.
	inline const POINT *rtnPos ( void ) { return &m_ptEditPos; }
	// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.

	// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.

	// 2007-11-05 by bhsohn 공약 창에 최대 글씨 못넘게 수정
	int GetImeStringLen();
	void SetStringMaxBuff(int nMaxBuffLen);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	void BackupTxtString();
	BOOL ClickBackSpace();		
	BOOL IsLastPos();	
	
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경	
	void SetFirstLine();

	// 2009-04-23 by bhsohn 에디트창 커서 이동	
	BOOL ClickDelBtn();
	void SetChatMsgBuff(char* i_pImeChat);

	// 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)
	void InitSetChatMsgBuff ( char *i_pText );
	// End 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)

	int OnKeyDownCursel(WPARAM wParam, LPARAM lParam);
	int OnKeyDownArrow(WPARAM wParam, LPARAM lParam);
	void InitChatMsgBuff();
	void RenderCursel();
	POINT RefreshEndCurselPos();
	int OnKeyDownFunc(WPARAM wParam, LPARAM lParam);
	// end 2009-04-23 by bhsohn 에디트창 커서 이동

private:	
	int  UpdateCurselPos(int nScrollStep, int nMaxScrollStep);
	int OnArrowKeyDown(int nArrow, int nScrollStep);
	int GetStringPos(char* pTxtString, int nStringLen, int nLinePos);	

	int GetStringLen(char* pTxt);
	int GetCharLen(char* pTxt, int nNextChatLen);

	int  UpdateCurselX();	
	
protected:

	// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.
	bool	m_bEnable_InputLimit;
	bool	m_bInputNumMode;

	int		m_iLimit_InputBuffLen;
	// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.

	BOOL	m_bEnableEdit;

	CD3DHanFont	*			m_pTxtFont;
	
	char*					m_pTxtString;	
	int						m_nBuffLen;

	vector<string>			m_vecStringCulling;

	// 에디트 박스 위치
	POINT					m_ptEditPos;
	// 컨트롤 크기
	int						m_nStrWidth;
	// 글씨간 간격
	int						m_nStringHeightCap;
	
	// 커서 이미지
	CINFImageEx*			m_pImgCurSel;								// 2011. 10. 10 by jskim UI시스템 변경
	POINT					m_ptCurSelPixel;

	BOOL					m_bShowCursel;
	float					m_fShowCurselTick;

	int						m_nMaxBuffLen;

	// 컬링 글씨 여부 판단.
	BOOL					m_bCullText;

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	char*					m_pBackupTxtString;
	char*					m_pTmpTxtString;	
	int						m_nMouseHeight;		// 마우스 인식 범위

	POINT					m_ptCurSelPos;		// 커서의 위치
	POINT					m_ptEndCurSelPos;	// 커서 마지막의 위치

	int						m_nStrOverFlowCntY;		// 스트링인 초과됐는지 여부 판다.

	int						m_nOnePageItemCnt;

	char					m_strIMEFullTmp[SIZE_MAX_STRING_1024];
	char					m_strIMETmp[SIZE_MAX_STRING_1024];

	// 2009-04-23 by bhsohn 에디트창 커서 이동	
	char						m_strInputMessage[MAX_EDIT_SINGLE_LINE_BUFF];	
	char				m_strPreBackupMessage[MAX_EDIT_SINGLE_LINE_BUFF];	// 스트링 앞에 버퍼
	char				m_strNextBackupMessage[MAX_EDIT_SINGLE_LINE_BUFF];	// 스트링 뒤에 버퍼
	char				m_strBkInputMessage[MAX_EDIT_SINGLE_LINE_BUFF];		// 빽업 버퍼
	// end 2009-04-23 by bhsohn 에디트창 커서 이동

};

#endif // !defined(AFX_INFSINGLECURSELEDITBOX_H__A7F38416_E734_4EB2_8193_D7A285F0A0CA__INCLUDED_)
