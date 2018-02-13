// INFCurselEditBox.h: interface for the CINFCurselEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCURSELEDITBOX_H__717961A7_345B_4320_A19D_D058D7EF87D2__INCLUDED_)
#define AFX_INFCURSELEDITBOX_H__717961A7_345B_4320_A19D_D058D7EF87D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CD3DHanFont;

class CINFCurselEditBox  : public CINFBase
{
public:
	CINFCurselEditBox();
	virtual ~CINFCurselEditBox();

	HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	//virtual void Render();
	void Render(int nScroll, int nOnePageLine);
	void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus);
	//virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL);
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int nScrollStep, 
							int *pLienStep = NULL, int *o_pScrollStep = NULL, BOOL *o_pStrcat=NULL);

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

	// 2007-11-05 by bhsohn 공약 창에 최대 글씨 못넘게 수정
	int GetImeStringLen();
	void SetStringMaxBuff(int nMaxBuffLen);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	void BackupTxtString();
	// 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
	//BOOL ClickBackSpace();
	BOOL ClickBackSpace( int nScrollStep );
	// end 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선

	// 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
	BOOL	OnDeleteKeyDown( int nScrollStep );
	void	OnHomeKeyDown( int nScrollStep );
	void	OnEndKeyDown( int nScrollStep );
	void	OnShiftKeyDown( void );
	void	OnShiftKeyUp( void );
	void	OnCtrlKeyDown( void );
	void	OnCtrlKeyUp( void );

	inline	BOOL	IsBlockMode( void )	{ return m_bBlockMode; }
	void	ResetBlockPosition( void );
	BOOL	IsValidBlockPostion( POINT pt );
	void	DetermineBlockFrontBack( POINT* ptBlockStart, POINT* ptBlockEnd );

	inline	BOOL	IsControlKeyDown( void ) { return m_bCtrlKeyState; }
	void	GetBlockedString( std::string* pStr );
	void	StringToClipBoard( const char* pStr );
	void	GetStringFromClipBoard( std::string* pStr );
	void	PasteString( const char* pStr, int nScrollStep );
	// end 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선

	void SetOnePageItemCnt(int nOnePageItemCnt);
	BOOL IsLastPos();
	POINT RefreshEndCurselPos();
	BOOL ClickEnter(int nScrollStep);

	// 2008-10-29 by bhsohn 에디트 박스 형태 변경	
	void SetFirstLine();

private:	
	int  UpdateCurselPos(int nScrollStep, int nMaxScrollStep);
	int OnArrowKeyDown(int nArrow, int nScrollStep);
	int GetStringPos(char* pTxtString, int nStringLen, int nLinePos);
	int  SetStrcat(char* pTxt, int nBufferCnt, int nIMEBufLen, int nScrollStep);	// 뒤에 스트링 붙이기

	int GetStringLen(char* pTxt);
	int GetCharLen(char* pTxt, int nNextChatLen);

	int  UpdateCurselX();	
	
protected:
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
	CINFImageEx*			m_pImgCurSel;
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

	// 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
	BOOL					m_bBlockMode;
	POINT					m_ptBlockStartPos;
	POINT					m_ptBlockEndPos;

	BOOL					m_bCtrlKeyState;
	// end 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
};

#endif // !defined(AFX_INFCURSELEDITBOX_H__717961A7_345B_4320_A19D_D058D7EF87D2__INCLUDED_)
