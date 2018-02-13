// IMEKey.h: interface for the IMEKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMEKEY_H__4811A5EE_D657_41B0_AA03_E3207DC63ECC__INCLUDED_)
#define AFX_IMEKEY_H__4811A5EE_D657_41B0_AA03_E3207DC63ECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>
#include <imm.h>
#include "AtumParam.h"
#include "CInput.h"

// by bhsohn 2007-03-12 China Working
#define	MAX_IMELIST		64

extern CInput		g_input;


#define WM_ENTERKEY		WM_USER+1

class IMEKey  
{
public:
	IMEKey();
	virtual ~IMEKey();
	int m_str_pos;
	//char m_full_str[SIZE_MAX_CHAT_MESSAGE];
	char m_full_str[SIZE_MAX_STRING_1024];
	BOOL m_ime_composing;
	int m_max_length;

	// 2008-07-24 by bhsohn 에디트창 Enter처리
	char* GetIMEFullStr();
	
	
	void On_IMEChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void ON_IMEComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	BOOL KeyMapping(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetMaxLength(int nLength) { m_max_length = nLength; }
	void ToggleHanEng(HWND hWnd, DWORD fFlag ) ;
	void ToggleChinaEng(HWND hWnd, DWORD fFlag );
	void UpdateChinaToggle(DWORD fFlag );
	
	void SetGameText(int nLanguageConversion);

	int GetInputLanguage() const { return m_nInputLanguage; }
	void SetInputLanguage(int nInputLanguage);
	void ChangeInputLanguage(HWND hWnd);

	// by bhsohn 2007-03-12 China Working
	HKL	GetMspyHKL();
	HKL	GetDefaultHKL();
	void OnlyChangeLangToggleInfo(int nInputLanguage);
	void UpdateToggleIcon(int nInputLanguage);
	int  GetToggleIcon();

	// 2007.04.24 by bhsohn China IME Working
	void EnableIMEControl(BOOL bEnable);

	// 2007-05-21 by bhsohn China IME Working
	void SetMSPYIME();

	// 2007-06-11 by bhsohn China IME Working
	int OnIMEKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 2007-06-13 by bhsohn China IME Working
	BOOL SetPermissionIME(HKL hkl);

	// 2007-09-05 by bhsohn China IME Working
	void NextImeSelect();

	// 2007-11-05 by bhsohn 공약 창에 최대 글씨 못넘게 수정
	int GetImeStringLen();

	// 2009-02-11 by bhsohn Japan IME Fix
	BOOL IsEnableIME() const { return m_bEnableIME; }

private:
	// by bhsohn 2007-03-12 China Working
	void InitImeList();

	// 2007.04.24 by bhsohn China IME Working	
	void SetCompositionWindowPos();

	// 2007-06-13 by bhsohn China IME Working	
	BOOL IsChinaIME(char* pImeType);

private:
	int m_nInputLanguage;
	
	// by bhsohn 2007-03-12 China Working	
	HKL		m_hkl[MAX_IMELIST];
	int		m_nChinaMSPYIdx;
	int		m_nEngIdx;
	int		m_nToggleIconInputLanguage;

	// 2007.04.24 by bhsohn China IME Working
	HIMC	m_hEnableImc;

	// 2007-06-13 by bhsohn China IME Working
	HKL		m_hklPermissionIME[MAX_IMELIST];
	HKL		m_hkActive;
	int		m_nPerMissionIMELen;
	int		m_nPerMissionIMEPos;
	BOOL	m_bFlagOnOff;

	// 2009-02-11 by bhsohn Japan IME Fix
	BOOL	m_bEnableIME;
};

#endif // !defined(AFX_IMEKEY_H__4811A5EE_D657_41B0_AA03_E3207DC63ECC__INCLUDED_)
