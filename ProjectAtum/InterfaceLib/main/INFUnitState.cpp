// INFUnitState.cpp: implementation of the CINFUnitState class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RangeTime.h"
#include "INFUnitState.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "ShuttleChild.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "TutorialSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define NUM_OF_INFUNITSTATE_BIT		32

#define UNIT_STATE_SIZE_X			311
#define UNIT_STATE_SIZE_Y			60
#define UNIT_STATE_STR_SIZE_X		311
#define UNIT_STATE_STR_SIZE_Y		60

#define UNIT_STATE_START_X			((g_pD3dApp->GetBackBufferDesc().Width)/2)
#define UNIT_STATE_START_Y			(UNIT_STATE_SIZE_Y-7)
#define UNIT_STATE_STR_START_X		((g_pD3dApp->GetBackBufferDesc().Width)/2)
#define UNIT_STATE_STR_START_Y		(UNIT_STATE_SIZE_Y-7)

#define TIME_POSITION_1				0
#define TIME_POSITION_2				1
#define INSERT_STATE_VIEWING_TIME	5
#else
#define NUM_OF_INFUNITSTATE_BIT		32

#define UNIT_STATE_SIZE_X			311
#define UNIT_STATE_SIZE_Y			60
#define UNIT_STATE_STR_SIZE_X		311
#define UNIT_STATE_STR_SIZE_Y		60

#define UNIT_STATE_START_X			((g_pD3dApp->GetBackBufferDesc().Width - UNIT_STATE_SIZE_X)/2)
#define UNIT_STATE_START_Y			(UNIT_STATE_SIZE_Y-7)
#define UNIT_STATE_STR_START_X		((g_pD3dApp->GetBackBufferDesc().Width - UNIT_STATE_SIZE_X)/2)
#define UNIT_STATE_STR_START_Y		(UNIT_STATE_SIZE_Y-7)

#define TIME_POSITION_1				0
#define TIME_POSITION_2				1
#define INSERT_STATE_VIEWING_TIME	5
#endif


CINFUnitState::CINFUnitState()
{
	m_bRestored = FALSE;

}

CINFUnitState::CINFUnitState(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_bRestored = FALSE;
	
	
	// tick
	m_dwState					= 0;//INF_UW_DANGER_MASK;
	m_dwInsertedState			= 0;	// 중간에 삽입됬을때
	m_dwInsertedStateCountSum	= 0;	// 삽입된 상태를 얼마동안 보여주나

	m_fElapsedTime				= 0;	// 어플리케이션 Tick에서의 경과시간
	m_fElapsedTimeSum			= 0;	// m_fElapsedTime의 합

	m_fTimeMiddlePos			= 0.4f;	// 깜박임을 위한 임의의 중간지점
    m_fTimeDirection			= 1.0f;	// 시간의 순회 방향
	memset(m_pImgState, 0x00, sizeof(DWORD)*UNIT_WARN_STATE_MAX);
	memset(m_pImgStateStr, 0x00, sizeof(DWORD)*UNIT_WARN_STATE_MAX);

	m_tInsertedStateRemainTime.Set(INSERT_STATE_VIEWING_TIME);
	m_tBlinkTime.Set( 0.0f, 2.0f, 0.7f );
}

CINFUnitState::~CINFUnitState()
{
	for( int i=0; i<UNIT_WARN_STATE_MAX; i++)
	{
		util::del(m_pImgState[i]);
		util::del(m_pImgStateStr[i]);
	}

}



HRESULT CINFUnitState::InitDeviceObjects()
{
	ASSERT_ASSERT(m_pGameData);
	DataHeader *pDataHeader = NULL;
	char buf[64];
	for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
	{
		wsprintf(buf, "us%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgState[i] = new CINFImageEx;
		m_pImgState[i]->InitDeviceObjects(pDataHeader );

		wsprintf(buf, "us_str%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgStateStr[i] = new CINFImageEx;
		m_pImgStateStr[i]->InitDeviceObjects(pDataHeader );
	}

	return S_OK;
}

HRESULT CINFUnitState::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
		{
			m_pImgState[i]->RestoreDeviceObjects();
			m_pImgStateStr[i]->RestoreDeviceObjects();
		}
		
		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFUnitState::DeleteDeviceObjects()
{
	for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
	{
		m_pImgState[i]->DeleteDeviceObjects();
		m_pImgStateStr[i]->DeleteDeviceObjects();

		util::del(m_pImgState[i]);
		util::del(m_pImgStateStr[i]);
	}
	return S_OK;
}

HRESULT CINFUnitState::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		
		for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
		{
			m_pImgState[i]->InvalidateDeviceObjects();
			m_pImgStateStr[i]->InvalidateDeviceObjects();
		}

		m_bRestored = FALSE;
	}
	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////
/// \fn		void CINFUnitState::Tick()
///
/// \brief		| ---------------|-----------------------|
///			timePos1		timeMiddlePos				timePos2
///
///			시간은 timePos1 부터 ~ timePos2 사이를 반복한다.
///			timePos1 ~ timeMiddlePos  까지는 이미지를 안보여 주고
///			이미지의 깜박임을 처리한다.
/////////////////////////////////////////////////////////////////////////////////////////////
void CINFUnitState::Tick()
{
	m_fElapsedTime = g_pD3dApp->GetElapsedTime();
/*	
	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_1) )
	{
		InsertState( INF_UW_DANGER_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_2) )
	{
		OffState( INF_UW_DANGER_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_3) )
	{
		SetState( INF_UW_OVERHEAT_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_4) )
	{
		OffState( INF_UW_OVERHEAT_MASK );
	}
	
	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_5) )
	{
		SetState( INF_US_TAKEOFF_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_6) )
	{
		OffState( INF_US_TAKEOFF_MASK );
	}
*/
	
	m_tBlinkTime.Tick( m_fElapsedTime );
	m_tInsertedStateRemainTime.Tick( m_fElapsedTime );
	
	if( m_tBlinkTime.IsOverEnd() )
		m_tBlinkTime.Start();

	if( m_dwInsertedState )
	{
		BOOL bRemain = m_tInsertedStateRemainTime.IsRemain();
		if( !bRemain )
		{
			SetState( m_dwInsertedState );
			m_dwInsertedState = 0;
		}
	}

	/*
	m_fElapsedTime = m_fElapsedTime*m_fTimeDirection;

	if( m_fElapsedTimeSum > TIME_POSITION_2 || m_fElapsedTimeSum < TIME_POSITION_1) 
	{
		if( m_fElapsedTimeSum > TIME_POSITION_2 )
		{
			m_fElapsedTimeSum = TIME_POSITION_2;
			m_fTimeDirection = -1.0f;
		}
		else
		{
			m_fElapsedTimeSum = TIME_POSITION_1;
			m_fTimeDirection = 1.0f;
		}
		
		// 삽입된 유닛 상태는 INSERT_STATE_VIEWING_TIME 시간이 지나면 SetState( ) 로 추가 시켜 준다
		if( m_dwInsertedState )
		{
			m_dwInsertedStateCountSum += 1;
			if( m_dwInsertedStateCountSum > INSERT_STATE_VIEWING_TIME )
			{
				m_dwInsertedStateCountSum = 0;
				SetState( m_dwInsertedState );
				m_dwInsertedState = 0;
			}
		}
	}
	
	m_fElapsedTimeSum += m_fElapsedTime;
	*/
	
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \fn		void CINFUnitState::Render()
/// \brief  1. CheckState( TRUE )로 먼저 삽입된상태(m_dwInsertedState)가 있는지 체크한다.
///			2. index는 선택된 상태의 이미지 배열 번호로서 
///				index 번호가 일반적인상태(UNIT_STATE_TAKEOFF이상)들 이면 항상 goto RENDER 한다.
///			3. index가 WARNING 상태면 시간을 체크하여 깜박인다.
/////////////////////////////////////////////////////////////////////////////////////////////
void CINFUnitState::Render()
{
	// 2005-04-19 by jschoi - Tutorial
	// 2005-08-02 by ispark - Character - 캐릭터 상태
	if(g_pTutorial->IsTutorialMode() == TRUE 
		|| g_pD3dApp->m_bCharacter == TRUE
		|| g_pShuttleChild->IsObserverMode())
		return;
	int index = -1;

//	//삽입된 상태가 있는지 첵크	
//	index = CheckState( TRUE );
//	if( index >= UNIT_STATE_TAKEOFF )		// 상태가 STATE_TAKEOFF이상이면 항상 보여주기
//		goto RENDER;
//
//	//일반 상태가 있는지 체크
//	index = CheckState( FALSE );
//	if( index >= UNIT_STATE_TAKEOFF )		// 상태가 STATE_TAKEOFF이상이면 항상 보여주기
//		goto RENDER;
//	
//	if( index < 0 )							// 상태가 없으면 안보여 주기
//		return;
//	
//	if( m_tBlinkTime.IsUnderMiddle() )	// 시간이 Middle 아래에 있을때만 그려주기(깜박임)
//		goto RENDER;
//
//	/*
//	if( m_fElapsedTimeSum > m_fTimeMiddlePos )
//	{
//		goto RENDER;
//	}
//	*/
//	return;
	
	// 2005-07-08 by ispark
	// 위 소스를 수정 goto문을 없앰

	//삽입된 상태가 있는지 첵크	
	index = CheckState(TRUE);
	if(index < UNIT_STATE_TAKEOFF)
	{
		//일반 상태가 있는지 체크
		index = CheckState(FALSE);
		if(index < UNIT_STATE_TAKEOFF)
		{
			if(index <0)
				return;

			if(!m_tBlinkTime.IsUnderMiddle())
				return;
		}
	}
	

//RENDER:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgState[index]->Move(UNIT_STATE_START_X - m_pImgState[index]->GetImgSize().x / 2, UNIT_STATE_START_Y);
	m_pImgState[index]->Render();
	
	if( g_pShuttleChild->m_myShuttleInfo.Level <= 11  )
	{
		m_pImgStateStr[index]->Move(UNIT_STATE_STR_START_X - m_pImgStateStr[index]->GetImgSize().x / 2, UNIT_STATE_STR_START_Y);
		m_pImgStateStr[index]->Render();
	}
#else
	m_pImgState[index]->Move(UNIT_STATE_START_X, UNIT_STATE_START_Y);
	m_pImgState[index]->Render();
	
	if( g_pShuttleChild->m_myShuttleInfo.Level <= 11  )
	{
		m_pImgStateStr[index]->Move(UNIT_STATE_STR_START_X, UNIT_STATE_STR_START_Y);
		m_pImgStateStr[index]->Render();
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////
/// \fn		int CINFUnitState::CheckState( BOOL bInsertedStateCheck )
/// \brief  1. bInsertedStateCheck 에 의한 첵크변수 선택
///			2. dwCheckState 를 #define된 마스크 값과 &연산 해서 참이면 이미지 index를 리턴
////////////////////////////////////////////////////////////////////////////////////

int CINFUnitState::CheckState( BOOL bInsertedStateCheck )
{
	int index = -1;
	DWORD dwCheckState = 0;			//첵크할 스테이트
	
	if( bInsertedStateCheck )
	{
		dwCheckState = m_dwInsertedState;
	}
	else
	{
		dwCheckState = m_dwState;
	}
	
	if( dwCheckState )
	{
		if( dwCheckState & INF_UW_FULLITEM3_MASK )		// 아이템 초과
		{
			index = 8;
			return index;
		}
		if( dwCheckState & INF_UW_FULLITEM2_MASK )		// 아이템 초과
		{
			index = 7;
			return index;
		}
		if( dwCheckState & INF_UW_FULLITEM1_MASK )		// 아이템 초과
		{
			index = 6;
			return index;
		}
		if( dwCheckState & INF_UW_DANGER_MASK )			// 기체 손상 위험
		{
			index = 0;
			return index;
		}
		
		if( dwCheckState & INF_UW_OVERHEAT_MASK)		// 부스터 과열
		{
			index = 1;
			return index;
		}
		
		if( dwCheckState & INF_UW_LOWFUEL_MASK )			// 연료 부족
		{
			index = 2;
			return index;
		}
		if( dwCheckState & INF_UW_FULLITEM_MASK )		// 아이템 초과
		{
			index = 3;
			return index;
		}
		if( dwCheckState & INF_UW_TOOHIGH_MASK )		// 아이템 초과
		{
			index = 4;
			return index;
		}
		if( dwCheckState & INF_UW_LOWSHIELD_MASK )		// 아이템 초과
		{
			index = 5;
			return index;
		}
		
		
		// Inf(인터페이스) Unit State
		if( dwCheckState & INF_US_TAKEOFF_MASK )		// 이륙된 상태(Take Off ) - 다시착륙-0(ins)
		{
			index = 9;
			return index;
		}
		
		if( dwCheckState & INF_US_STOP_MASK )			// 유닛 공중에 정지됨 - 해제 -전진키 W
		{
			index = 10;
			return index;
		}
		if( dwCheckState & INF_US_LOCK_MASK )			// 움직일수 없는 상태(Unit Lock) - R
		{
			index = 11;
			return index;
		}
		if( dwCheckState & INF_US_FORM_MASK )			// formation 편대비행중( 합류키 f2)
		{
			index = 12;
			return index;
		}
		if( dwCheckState & INF_US_LAND_MASK )			// 유닛 Landing, 착륙중 - 다시 이륙 W
		{
			index = 13;
			return index;
		}
	}
	
	return index;
}

void CINFUnitState::InsertState( DWORD dwState )
{
	if( m_dwInsertedState )
	{
		SetState( m_dwInsertedState );
	}
	m_dwInsertedState = dwState;	// 중간에 삽입됬을때
	m_tInsertedStateRemainTime.Set(INSERT_STATE_VIEWING_TIME);// 삽입된 상태를 얼마동안 보여주나
	//m_dwInsertedStateCountSum = 0;	// 삽입된 상태를 얼마동안 보여주나
}

void CINFUnitState::SetState( DWORD dwState )
{
	m_dwState |= dwState;
	m_tBlinkTime.Start();
}

BOOL CINFUnitState::GetState( DWORD dwState )
{
	return ( m_dwState & dwState ) ? TRUE : FALSE;
}

void CINFUnitState::OffState( DWORD dwState )
{
	m_dwState &= ~dwState;
	m_dwInsertedState &= ~dwState;
}
