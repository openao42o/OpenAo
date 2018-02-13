// INFEditBox.h: interface for the CINFEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFEDITBOX_H__345C4E04_2C34_4062_9F20_D0086895C39F__INCLUDED_)
#define AFX_INFEDITBOX_H__345C4E04_2C34_4062_9F20_D0086895C39F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CD3DHanFont;
class CINFImageEx;

#define		MAX_SEARCH_ITEM			10					// 2013-02-20 by bhsohn 인게임 조합창 처리

class CINFEditBox  : public CINFBase
{
public:
	CINFEditBox();
	virtual ~CINFEditBox();

	virtual HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		// 2013-02-20 by bhsohn 인게임 조합창 처리
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	virtual void Render();
	virtual void Render(int nScroll, int nOnePageLine); // 2013-02-20 by bhsohn 인게임 조합창 처리
	virtual void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus); // 2013-02-20 by bhsohn 인게임 조합창 처리
	//virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL);
	// 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL, BOOL bAddString=FALSE, BOOL bUseBackSpace=TRUE);
	// END 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리

	// 버튼 클릭 
	virtual BOOL OnLButtonDown(POINT ptPos);	

	virtual void Tick(); // 2013-02-20 by bhsohn 인게임 조합창 처리

	BOOL IsEditMode();
	
	// 스트링 지정
	int  SetString(const char* pTxt, int nBufferCnt, BOOL bChRtn=FALSE);	// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
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
	BOOL ClickBackSpace();	

protected:// 2013-02-20 by bhsohn 인게임 조합창 처리
	void StringCulling(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);
	
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
	POINT					m_ptCurSel;

	BOOL					m_bShowCursel;
	float					m_fShowCurselTick;

	int						m_nMaxBuffLen;

	// 컬링 글씨 여부 판단.
	BOOL					m_bCullText;

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	char*					m_pBackupTxtString;
	BOOL					m_bBackAdd;	
	int						m_nMouseHeight;		// 마우스 인식 범위
	
	// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
	BOOL					m_bManageChRtn;	//	\\n 처리

};

#endif // !defined(AFX_INFEDITBOX_H__345C4E04_2C34_4062_9F20_D0086895C39F__INCLUDED_)
