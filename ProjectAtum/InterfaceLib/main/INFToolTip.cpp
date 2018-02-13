// INFToolTip.cpp: implementation of the CINFToolTip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFToolTip.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"								   // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGameMain.h"

#define	TOOLTIP_SHOW_TIME			3.0f
#define TOOLTIP_RENDER_Y			150.f
#define POINT_Y_GAB					15
#define POINT_LEFT_GAB				3
#define POINT_TOP_GAB				2


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFToolTip::CINFToolTip()
{
	m_pImgTemp					= NULL;
	m_fLifeTime					= 0;
	m_IsShow					= TRUE;

	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	m_pToolTipFont				= NULL;	
	m_bToolTipState				= FALSE;
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

}

CINFToolTip::~CINFToolTip()
{

}

void CINFToolTip::SetImgType(UINT i_nType)
{
	map<UINT, CINFImageEx*>::iterator it = m_mapToolTip.find(i_nType);				// 2011. 10. 10 by jskim UI시스템 변경
	m_pImgTemp = it->second;
	m_fLifeTime = TOOLTIP_SHOW_TIME;
}
void CINFToolTip::Render()
{
	if(m_fLifeTime > 0 
		&& m_IsShow)
	{
		float fUnLifeTime =  TOOLTIP_SHOW_TIME - m_fLifeTime;
		DWORD dwAlpha = GetCurrentColor(fUnLifeTime, ALPHA_DELAY_WAVE, TOOLTIP_SHOW_TIME,2.0f);
		
		POINT pt = m_pImgTemp->GetImgSize();
		DWORD nX = (int)(g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		DWORD nY = TOOLTIP_RENDER_Y;
		m_pImgTemp->SetColor(dwAlpha);
		m_pImgTemp->Move(nX,nY);
		m_pImgTemp->Render();
	}
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	if( m_bToolTipState == TRUE && strlen(m_pViewName) > 0)
	{
		SIZE Size = m_pToolTipFont->GetStringSize(m_pViewName);
		LONG temp = 0;
		if((int)(g_pD3dApp->GetBackBufferDesc().Width) - (int)(m_nPoint.x + Size.cx + POINT_LEFT_GAB * 2 + 2 ) < 0 ) 
		{
			temp = abs((LONG)((g_pD3dApp->GetBackBufferDesc().Width) - (int)(m_nPoint.x + Size.cx + POINT_LEFT_GAB * 2 + 2 )));
		}
		g_pGameMain->RenderPopUpWindowImage(m_nPoint.x - temp, m_nPoint.y + POINT_TOP_GAB, Size.cx + POINT_LEFT_GAB * 2 + 2, Size.cy);
		m_pToolTipFont->DrawText(m_nPoint.x + POINT_LEFT_GAB - temp, m_nPoint.y,GUI_FONT_COLOR_W,m_pViewName);
	}
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
}
void CINFToolTip::Tick()
{
	if(m_fLifeTime <= 0)
	{
		m_fLifeTime = 0;
	}
	else
	{
		m_fLifeTime -= g_pD3dApp->GetElapsedTime();
	}
}

HRESULT CINFToolTip::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	//char buf[16];

	CINFImageEx * pImage;

	pImage = new CINFImageEx;
	pDataHeader = FindResource("ustelsuc");
	pImage->InitDeviceObjects(pDataHeader ) ;
	m_mapToolTip[TOOLTIP_IMGTYPE_TELEPORT_SUC] = pImage;

	pImage = new CINFImageEx;
	pDataHeader = FindResource("usteldes");
	pImage->InitDeviceObjects(pDataHeader ) ;
	m_mapToolTip[TOOLTIP_IMGTYPE_TELEPORT_DESTROY] = pImage;
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	m_pToolTipFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pToolTipFont->InitDeviceObjects(g_pD3dDev);

	int nCnt = 0;
	while(1)
	{
		char* szTemp = new char[TOOLTIP_FONTSIZE];
		wsprintf( szTemp, "STRTOOLTIP%d", ++nCnt );
		char* strName = new char[TOOLTIP_FONTSIZE];
		
		OutIniString( _T("ToolTipName"), szTemp, strName, TOOLTIP_FONTSIZE);
		
		if( 0 == strlen(strName) )
			break;
		
		
		m_vecToolTipName.push_back( make_pair( szTemp, strName ) );
	}
	m_bToolTipState = FALSE;
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	return S_OK;
}
HRESULT CINFToolTip::RestoreDeviceObjects()
{
	map<UINT, CINFImageEx*>::iterator it = m_mapToolTip.begin();
	while(it != m_mapToolTip.end())
	{
		CINFImageEx* pImage = it->second;
		pImage->RestoreDeviceObjects();
		it++;
	}
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	m_pToolTipFont->RestoreDeviceObjects();
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	return S_OK;
}
HRESULT CINFToolTip::DeleteDeviceObjects()
{
	map<UINT, CINFImageEx*>::iterator it = m_mapToolTip.begin();
	while(it != m_mapToolTip.end())
	{
		CINFImageEx* pImage = it->second;
		pImage->DeleteDeviceObjects();
		util::del(pImage);
		it++;
	}
	m_mapToolTip.clear();

	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	m_pToolTipFont->DeleteDeviceObjects();
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	return S_OK;
}
HRESULT CINFToolTip::InvalidateDeviceObjects()
{
	map<UINT, CINFImageEx*>::iterator it = m_mapToolTip.begin();
	while(it != m_mapToolTip.end())
	{
		CINFImageEx* pImage = it->second;
		pImage->InvalidateDeviceObjects();
		it++;
	}

	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	m_pToolTipFont->InvalidateDeviceObjects();
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
	return S_OK;
}
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
TCHAR* CINFToolTip::ExecuteDirFullName() /* ini파일 경로 */
{
	TCHAR szCurrentDirectory[TOOLTIP_FONTSIZE];
	static TCHAR szExecuteFullName[TOOLTIP_FONTSIZE*2];		
	TCHAR sziniFileName[TOOLTIP_FONTSIZE] = _T("ToolTipList.tex");
	
	GetCurrentDirectory( TOOLTIP_FONTSIZE, szCurrentDirectory );
	wsprintf( szExecuteFullName, _T("%s\\Res-Tex\\%s"), szCurrentDirectory, sziniFileName );
	
	return szExecuteFullName;
}

int CINFToolTip::OutIniInt( TCHAR* _lpAppName, TCHAR* _lpKeyName ) /* ini에서 숫자값 읽기 */
{
	return GetPrivateProfileInt( _lpAppName, _lpKeyName, 0, ExecuteDirFullName() );
}

void CINFToolTip::OutIniString( TCHAR* _lpAppName, TCHAR* _lpKeyName, TCHAR* _lpReturnedString, int _nSize ) /* ini에서 문자열 읽기 */
{
	GetPrivateProfileString( _lpAppName, _lpKeyName, _T(""), _lpReturnedString, _nSize, ExecuteDirFullName() );
}

void CINFToolTip::SetNameToolTip(POINT pt, char* strKey)
{
	m_bToolTipState = TRUE;
	memset(m_pViewName,0x00, TOOLTIP_FONTSIZE);
	m_nPoint.x = pt.x;
	m_nPoint.y = pt.y - POINT_Y_GAB;
	vector< pair< string, string > >::iterator it = m_vecToolTipName.begin();

	for(it; it!=m_vecToolTipName.end(); it++)
	{
		if(!strcmp(strKey,(const char*)it->first.c_str()))
		{
			strcpy(m_pViewName,it->second.c_str());
			break;
		}			
	}
}
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현









































