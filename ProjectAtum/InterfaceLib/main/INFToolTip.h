// INFToolTip.h: interface for the CINFToolTip class.
//	2007-09-19 by dgwoo 게임 진행중 화면에 이미지를 띄울 클래스.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFTOOLTIP_H__DC6DA211_4954_4DFC_8831_7850FC864D34__INCLUDED_)
#define AFX_INFTOOLTIP_H__DC6DA211_4954_4DFC_8831_7850FC864D34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#include "D3DHanFont.h"

#define TOOLTIP_FONTSIZE			512

class CINFImage;
class DataHeader;
class CGameData;
class CINFImageEx;								   // 2011. 10. 10 by jskim UI시스템 변경


class CINFToolTip : public CINFBase  
{
public:
	CINFToolTip();
	virtual ~CINFToolTip();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();

	void SetImgType(UINT i_nType);

	map<UINT, CINFImageEx*> m_mapToolTip;				// 툴팁 정보를 가지고있는 map
	CINFImageEx*	m_pImgTemp;						// 현재 등록된 이미지 주소값.
	BOOL			m_IsShow;						// 현재 보여져야 하는가?
	float			m_fLifeTime;					// 보여지는 남은 시간.
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	CD3DHanFont*    m_pToolTipFont;			    	// 버튼 툴팁
	vector< pair< string, string > >		m_vecToolTipName;		// 
	BOOL					m_bToolTipState;
	POINT					m_nPoint;
	char					m_pViewName[TOOLTIP_FONTSIZE];

	void   SetNameToolTip(POINT pt, char* strKey);
	TCHAR* ExecuteDirFullName(); /* ini파일 경로 */
	int OutIniInt( TCHAR* _lpAppName, TCHAR* _lpKeyName ); /* ini에서 숫자값 읽기 */
	void OutIniString( TCHAR* _lpAppName, TCHAR* _lpKeyName, TCHAR* _lpReturnedString, int _nSize );
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
};

#endif // !defined(AFX_INFTOOLTIP_H__DC6DA211_4954_4DFC_8831_7850FC864D34__INCLUDED_)
