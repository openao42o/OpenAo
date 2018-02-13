// INFGameMainOtherInfo.h: interface for the CINFGameMainOtherInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINOTHERINFO_H__D292DD48_BD40_4CB9_8B4C_107A96F3660F__INCLUDED_)
#define AFX_INFGAMEMAINOTHERINFO_H__D292DD48_BD40_4CB9_8B4C_107A96F3660F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"

/////////////////////////////////////////////////////////////////////////////////
///	\struct		CINFGameMainOtherInfo
/// \brief		타겟에 잡힌 적의 정보(길드,이름,HP)를 보여준다
///				
/////////////////////////////////////////////////////////////////////////////////

class CINFImage;
class CD3DHanFont;
class CINFImageEx;
class CINFGameMainOtherInfo : public CINFBase
{
public:
	CINFGameMainOtherInfo(CAtumNode* pParent);
	virtual ~CINFGameMainOtherInfo();


	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	// main function
	void Tick( float fElapedTime = 0.0f);
	void Render( );

	// sub function
	void SetLockOnTarget( int iLockOnTarget );
	void OffLockOnTarget( );
	BOOL GetLockOnTarget( );
	void SetHpRate( float fHpRate );
	void SetOtherInfoString( char* strGuild, char* strName ); // 적 길드, 이름,
	void SetMonsterInfoString(char* strName);				 // 몬스터의 이름,
	void SetMonsterHPString(char* strHP);
	void DrawText( );										// 적이나 몬스터의 길드, 이름

public:
	// init()
	bool m_bRestored;

//	CINFImage	*m_pImgBack;
//	CINFImage	*m_pImgHpLeft;
//	CINFImage	*m_pImgHpCenter;
//	CINFImage	*m_pImgHpRight;
	
	// 2004-12-09 by jschoi
	CINFImageEx	*m_pImgTargetInfoBack;		// 타겟의 에너지 상태 배경
	CINFImageEx	*m_pImgTargetInfoBar;		// 타겟의 에너지 상태 배경

	CD3DHanFont*	m_pFontOtherInfo[2];
//	char m_strOtherGuiid[64];
	char m_strOtherName[64];
	char m_strMonsterName[64];
	char m_strMonsterHP[64];

	float m_fHpRate;						// 적의 Hp Rate
	int   m_iLockOnTarget;					// 적이나 몬스터를 타켓으로 계속 가지고 있는지

	float m_fImgLeftRate;					// m_pImgHpLeft 의 Rate
	float m_fImgCenterRate;					// m_pImgHpCenter 의 Rate
	float m_fImgRightRate;					// m_pImgHpRight 의 Rate
	
	float m_fImgToLeftRate;					// m_pImgHpLeft 까지의 Rate
	float m_fImgToCenterRate;				// m_pImgHpCenter 까지의 Rate
	float m_fImgToRightRate;				// m_pImgHpRight 까지의 Rate
};

#endif // !defined(AFX_INFGAMEMAINOTHERINFO_H__D292DD48_BD40_4CB9_8B4C_107A96F3660F__INCLUDED_)
