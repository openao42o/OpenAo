// INFBase.cpp: implementation of the CINFBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFBase.h"
#include "GameDataLast.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFBase::CINFBase()
{
	m_pGameData = NULL;
	m_fTexRate = 0;
}

CINFBase::~CINFBase()
{

}

int CINFBase::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}

void CINFBase::SetGameData(CGameData * pData)
{
	FLOG( "CINFBase::SetGameData(CGameData * pData)" );
	m_pGameData = pData ;
}

DataHeader* CINFBase::FindResource(char* szRcName) const
{
	FLOG( "CINFBase::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pGameData)
	{
		pHeader = m_pGameData->Find(szRcName);
#ifdef _DEBUG
		if(!pHeader)
		{
			DBGOUT("INFBase::FindResource(%s) Can't Find Resource File.\n",szRcName );
		}
#endif // _DEBUG_endif
	}
	return pHeader;
}
BOOL CINFBase::GetButtonStateOnMouse( POINT pt, int nX, int nY, int nSizeX, int nSizeY )
{
	if( pt.x > nX && 
		pt.x < nX+nSizeX &&
		pt.y > nY &&
		pt.y < nY+nSizeY)
	{
		return TRUE;
	}
	return FALSE;
}


// 2010-06-15 by shcho&hslee 펫시스템 - 좌표 처리 관련 클래스 추가.


CPosData :: CPosData ()
{

}


CPosData :: ~CPosData ()
{

}

	
void CPosData :: Reset ( void )
{
	_fPosx = .0f;
	_fPosy = .0f;
	
	_fWid = .0f;
	_fHgt = .0f;
	
	SetRectEmpty ( &_rcRECT );
}


void CPosData :: InitData ( const float a_fPosx , const float a_fPosy , const float a_fWid /*= .0f*/ , const float a_fHgt /*= .0f*/ , const bool a_bInitRect /*= false*/ )
{
	_fPosx = a_fPosx;
	_fPosy = a_fPosy;

	_fWid = a_fWid;
	_fHgt = a_fHgt;

	if ( a_bInitRect )
		InitRect ( false );
}


void CPosData :: InitRect ( const bool a_bUpdate /*= false*/ , const float a_fPosx /*= .0f*/ , const float a_fPosy /*= .0f*/ , const float a_fWidth /*= .0f*/ , const float a_fHeight /*= .0f*/ )
{
	if ( a_bUpdate )
	{
		SetPos ( a_fPosx , a_fPosy );
		SetSize ( a_fWidth , a_fHeight );
	}

	SetRect ( &_rcRECT , _fPosx , _fPosy , _fPosx + _fWid , _fPosy + _fHgt );
}


bool CPosData :: IsRectOn ( POINT pt , const float a_fDstx /*= .0f*/ , const float a_fDsty /*= .0f*/ )
{
	RECT rcCurRect = _rcRECT;
		OffsetRect ( &rcCurRect , a_fDstx , a_fDsty );

	if ( TRUE == PtInRect ( &rcCurRect , pt ) )
		return true;

	return false;
}


// End 2010-06-15 by shcho&hslee 펫시스템 - 좌표 처리 관련 클래스 추가.
