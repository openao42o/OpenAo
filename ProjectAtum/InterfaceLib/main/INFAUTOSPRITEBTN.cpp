// INFAUTOSPRITEBTN.cpp: implementation of the CINFAUTOSPRITEBTN class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "INFLuckyMachine.h"
#include "AtumDatabase.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "INFIcon.h"
#include "ItemInfo.h"
#include "FieldWinSocket.h"
#include "INFCityBase.h"
#include "StoreData.h"
#include "Chat.h"
#include "INFInvenExtend.h"
#include "ShuttleChild.h"
#include "AtumSound.h"
#include "INFAUTOSPRITEBTN.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFAUTOSPRITEBTN::CINFAUTOSPRITEBTN( int nFrameCount, DWORD nTimePerFrame ) : CINFImageBtn(),
																			   m_nFrameCount(nFrameCount),
																			   m_nTimePerFrame(nTimePerFrame),
																			   m_nCurFrame(0),
																			   m_nLastAnimationTime(0)
{

}

CINFAUTOSPRITEBTN::~CINFAUTOSPRITEBTN()
{

}

BOOL	CINFAUTOSPRITEBTN::IsMouseOverlab(POINT ptPos)
{
	CINFImage*	pImage	= GetImg(0);
	POINT		pt		= pImage->GetImgSize();
	
	int			nWidth	= pt.x / m_nFrameCount;

	POINT		pos		= GetBtnPosition();
	
	if( (pos.x <= ptPos.x) && ( ptPos.x <= pos.x + nWidth) )
	{
		if( (pos.y <= ptPos.y) && ( ptPos.y <= (pos.y + pt.y) ) )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

void	CINFAUTOSPRITEBTN::Render()
{
	DWORD dwCurTime = g_pD3dApp->GetCurrentGameTime();
	if( m_nLastAnimationTime + m_nTimePerFrame <= dwCurTime )
	{
		m_nCurFrame = ++m_nCurFrame % m_nFrameCount;
		m_nLastAnimationTime = dwCurTime;
	}

	for(int i=0; i<MAX_BTN_STATUS; ++i)
	{
		CINFImage*	pImage	= GetImg(i);
		POINT		pt		= pImage->GetImgSize();

		int			nWidth	= pt.x / m_nFrameCount;

		pImage->SetRect( m_nCurFrame * nWidth, 0, ( m_nCurFrame + 1 ) * nWidth, pt.y );
	}
	
	CINFImageBtn::Render();
}