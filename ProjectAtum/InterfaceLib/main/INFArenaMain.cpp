// INFArenaMain.cpp: implementation of the CINFArenaMain class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "INFGameMain.h"
#include "INFArenaMain.h"

#define		ARENA_STATE_NONE		0
#define		ARENA_STATE_CITY		1
#define		ARENA_STATE_GAME		2


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFArenaMain::CINFArenaMain()
{
	m_pINFGameArea = NULL;
	m_nAreanaState = ARENA_STATE_GAME;	
}

CINFArenaMain::~CINFArenaMain()
{
	util::del(m_pINFGameArea);		
}

HRESULT CINFArenaMain::InitDeviceObjects()
{	
	if(NULL == m_pINFGameArea)
	{
		m_pINFGameArea = new CINFGameArea;			
	}
	m_pINFGameArea->InitDeviceObjects();	
	
	return S_OK ;
}
HRESULT CINFArenaMain::RestoreDeviceObjects()
{
	m_pINFGameArea->RestoreDeviceObjects();
	return S_OK ;
}
HRESULT CINFArenaMain::DeleteDeviceObjects()
{
	m_pINFGameArea->DeleteDeviceObjects();	
	return S_OK ;
}
HRESULT CINFArenaMain::InvalidateDeviceObjects()
{
	m_pINFGameArea->InvalidateDeviceObjects();
	return S_OK ;
}
void CINFArenaMain::Render(DWORD dwGameState)
{
	switch(dwGameState)
	{
	case _GAME:
		{
			m_pINFGameArea->Render();
		}
		break;
	}	
}

void CINFArenaMain::TickArena(DWORD dwGameState)
{
	switch(dwGameState)
	{
	case _GAME:
		{
			m_pINFGameArea->Tick();
		}
		break;
	}	

}


int CINFArenaMain::WndProc(DWORD dwGameState, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(dwGameState)
	{
	case _GAME:
		{
			return m_pINFGameArea->WndProc(uMsg, wParam, lParam);
		}
		break;
	}	
	return INF_MSGPROC_NORMAL;
}


// 아레나 인터페이스 보여주기
void CINFArenaMain::ShowArenaInteraface(DWORD dwGameState, int nIdx, BOOL bShow)
{
	switch(dwGameState)
	{
	case _GAME:
		{
			m_pINFGameArea->ShowArenaInteraface(nIdx, bShow);
		}
		break;
	}	


}

