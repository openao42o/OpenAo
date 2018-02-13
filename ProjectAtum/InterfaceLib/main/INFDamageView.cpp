// INFImageAnimation.cpp: implementation of the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFDamageView.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "SceneData.h"
#include "MonsterData.h"
#include "dxutil.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DAMAGE_VIEW_STEP	10	// 이미지를 언제까지 보여줄것인가
#define DAMAGE_VIEW_TIMER	0.1f

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
#define DAMAGE_X_MOVE_MIN	50.0f
#define DAMAGE_X_MOVE_MAX	70.0f
#define DAMAGE_Y_MOVE_MIN	80.0f
#define DAMAGE_Y_MOVE_MAX	130.0f
#define DAMAGE_Y_GRAVITY	80.0f
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

CINFDamageView::CINFDamageView()
{
	m_pNormal		= NULL;
	m_pCritical		= NULL;
	m_pMiss			= NULL;
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	m_pPet			= NULL;
	// ebd 2010-11-01 by jskim, 펫 데미지 View 변경		
	m_fTickTimer	= DAMAGE_VIEW_TIMER;
}

CINFDamageView::~CINFDamageView()
{
	util::del(m_pNormal);		
	util::del(m_pCritical);		
	util::del(m_pMiss);
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	util::del(m_pPet);
	// ebd 2010-11-01 by jskim, 펫 데미지 View 변경		
}

HRESULT CINFDamageView::InitDeviceObjects()
{
	DataHeader * pDataHeader = NULL;
	pDataHeader = m_pGameData->Find( "DgNormal" );
	if(pDataHeader)
	{
		m_pNormal = new CINFImageEx;
		m_pNormal->InitDeviceObjects( pDataHeader  );
	}
	pDataHeader = m_pGameData->Find( "DgCriti" );
	if(pDataHeader)
	{
		m_pCritical = new CINFImageEx;
		m_pCritical->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = m_pGameData->Find( "DgMiss" );
	if(pDataHeader)
	{
		m_pMiss = new CINFImageEx;
		m_pMiss->InitDeviceObjects( pDataHeader );
	}
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	pDataHeader = m_pGameData->Find( "P_DgNormal" );
	if(pDataHeader)
	{
		m_pPet = new CINFImageEx;
		m_pPet->InitDeviceObjects( pDataHeader);
	}
	// ebd 2010-11-01 by jskim, 펫 데미지 View 변경		
	
	return S_OK;
}

HRESULT CINFDamageView::RestoreDeviceObjects()
{
	m_pNormal->RestoreDeviceObjects();
	m_pCritical->RestoreDeviceObjects();
	m_pMiss->RestoreDeviceObjects();
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	m_pPet->RestoreDeviceObjects();
	// end 2010-11-01 by jskim, 펫 데미지 View 변경

	return S_OK;
}

HRESULT CINFDamageView::InvalidateDeviceObjects()
{
	m_pNormal->InvalidateDeviceObjects();
	m_pCritical->InvalidateDeviceObjects();
	m_pMiss->InvalidateDeviceObjects();
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	m_pPet->InvalidateDeviceObjects();
	// end 2010-11-01 by jskim, 펫 데미지 View 변경
	return S_OK;
}

HRESULT CINFDamageView::DeleteDeviceObjects()
{
	FLOG( "CINFDamageView::DeleteDeviceObjects()" );
	m_pNormal->DeleteDeviceObjects();
	m_pCritical->DeleteDeviceObjects();
	m_pMiss->DeleteDeviceObjects();
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	m_pPet->DeleteDeviceObjects();
	// end 2010-11-01 by jskim, 펫 데미지 View 변경
	util::del(m_pNormal);
	util::del(m_pCritical);
	util::del(m_pMiss);
	// 2010-11-01 by jskim, 펫 데미지 View 변경
	util::del(m_pPet);
	// end 2010-11-01 by jskim, 펫 데미지 View 변경
	return S_OK;
}

#define CRITICAL_SIZE_X			22
#define CRITICAL_SIZE_Y			19
#define NORMAL_SIZE_X			18
#define NORMAL_SIZE_Y			15
#define MISS_SIZE_X				40
#define MISS_SIZE_Y				10
#define IMAGE_SCALE				((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
#define SHOW_SPEED				60
#define Y_START_POS				72
void CINFDamageView::Render()
{
	FLOG( "CINFDamageView::Render()" );
    // 2011. 03. 08 by jskim 인피3차 구현
	int x, y, w;
    // end 2011. 03. 08 by jskim 인피3차 구현
	
	int wideTemp = 0;
	
	vector<DamageView_t>::iterator it = vecDamage.begin();
	while(it != vecDamage.end())
	{	
		CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( it->TargetIndex );

		
		// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
		// (pUnit &&
		//	pUnit->m_nObjScreenW > 0.0f)
		//{
		//x = pUnit->m_nObjScreenX;
		//y = pUnit->m_nObjScreenY;
		if( pUnit )
		{
			// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
			if(pUnit &&
				pUnit->m_dwPartType == _MONSTER &&
				((CMonsterData*)pUnit)->m_vecvmultiData.size() > 0)
			// end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
			{
				D3DXVECTOR3 temp = ((CMonsterData*)pUnit)->GetMultiPos((*it).nMultiIndex);
				((CMonsterData*)pUnit)->GetScreenPos(temp, x, y, w);
			}
			else
			{
				x = pUnit->m_nObjScreenX;
				y = pUnit->m_nObjScreenY;
			}
			// end . 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

			DWORD dwColor = 0x00FFFFFF;
			DWORD alpha = 0;
			alpha = 255* (1.0f - ((float)(*it).nDamageViewStep)/DAMAGE_VIEW_STEP );
			alpha = alpha << 24;
			dwColor = dwColor | alpha;
			switch((*it).nDamageKind)
			{
			case DAMAGEKIND_NO_DAMAGE:
				{
					m_pMiss->Move( x-(MISS_SIZE_X/2), y-((*it).fTimer*SHOW_SPEED)-Y_START_POS);
					m_pMiss->SetScale( IMAGE_SCALE, IMAGE_SCALE );
					m_pMiss->SetColor( dwColor );
					m_pMiss->Render();
				}
				break;
			case DAMAGEKIND_NORMAL:
				{
					vector<int>::iterator itl = it->vecDamageView.begin();
					while(itl != it->vecDamageView.end())
					{
						m_pNormal->Move( x+wideTemp, y-((*it).fTimer*SHOW_SPEED)-Y_START_POS);
						m_pNormal->SetScale( IMAGE_SCALE, IMAGE_SCALE );
						m_pNormal->SetColor( dwColor );
						m_pNormal->SetRect( NORMAL_SIZE_X*(*itl), 0, NORMAL_SIZE_X*(*itl+1), NORMAL_SIZE_Y );
						m_pNormal->Render();
						wideTemp += NORMAL_SIZE_X*IMAGE_SCALE;
						itl++;
					}
				}
				break;
			// 2010-11-01 by jskim, 펫 데미지 View 변경
			case DAMAGEKIND_PET:
				{
					vector<int>::iterator itl = it->vecDamageView.begin();
					while(itl != it->vecDamageView.end())
					{
						m_pPet->Move( x+wideTemp, y-((*it).fTimer*SHOW_SPEED)-Y_START_POS);
						m_pPet->SetScale( IMAGE_SCALE, IMAGE_SCALE );
						m_pPet->SetColor( dwColor );
						m_pPet->SetRect( NORMAL_SIZE_X*(*itl), 0, NORMAL_SIZE_X*(*itl+1), NORMAL_SIZE_Y );
						m_pPet->Render();
						wideTemp += NORMAL_SIZE_X*IMAGE_SCALE;
						itl++;
					}
				}
				break;
			// end 2010-11-01 by jskim, 펫 데미지 View 변경

			// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			// 추가 데미지 타입
			case DAMAGEKIND_ADD_DAMAGE:
				{
					float fElapseTime = g_pD3dApp->GetElapsedTime();

					// x축 이동
					(*it).x						+= (*it).fHorizontallySpeed * fElapseTime;
					// 추락 가속도 계산
					(*it).fVerticalSpeed		-= DAMAGE_Y_GRAVITY * fElapseTime;
					// y축 이동
					(*it).y						+= (*it).fVerticalSpeed * fElapseTime;

					vector<int>::iterator itl = it->vecDamageView.begin();
					while(itl != it->vecDamageView.end())
					{
						m_pCritical->Move( x + (*it).x + wideTemp, y + -(*it).y - Y_START_POS );
						m_pCritical->SetScale( IMAGE_SCALE, IMAGE_SCALE );
						dwColor = 0xFF00FF | alpha;			//2013-05-20 by ssjung 추가 데미지 색상 변경 
						m_pCritical->SetColor( dwColor );
						m_pCritical->SetRect( CRITICAL_SIZE_X*(*itl), 0, CRITICAL_SIZE_X*(*itl+1), CRITICAL_SIZE_Y );
						m_pCritical->Render();
						wideTemp += CRITICAL_SIZE_X*IMAGE_SCALE;
						itl++;
					}
				}
				break;
			// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			case DAMAGEKIND_REFLECTION:
				{
					vector<int>::iterator itl = it->vecDamageView.begin();
					while(itl != it->vecDamageView.end())
					{
						m_pNormal->Move( x+wideTemp, y-((*it).fTimer*SHOW_SPEED)-Y_START_POS);
						m_pNormal->SetScale( IMAGE_SCALE, IMAGE_SCALE );
						m_pNormal->SetColor( dwColor );
						m_pNormal->SetRect( NORMAL_SIZE_X*(*itl), 0, NORMAL_SIZE_X*(*itl+1), NORMAL_SIZE_Y );
						m_pNormal->Render();
						wideTemp += NORMAL_SIZE_X*IMAGE_SCALE;
						itl++;
					}
				}
				break;
			// end 2010. 02. 11 by ckPark 발동류 장착아이템

			case DAMAGEKIND_CRITICAL:
				{
					vector<int>::iterator itl = it->vecDamageView.begin();
					while(itl != it->vecDamageView.end())
					{
						m_pCritical->Move( x+wideTemp, y-((*it).fTimer*SHOW_SPEED)-Y_START_POS);
						m_pCritical->SetScale( IMAGE_SCALE, IMAGE_SCALE );
						m_pCritical->SetColor( dwColor );
						m_pCritical->SetRect( CRITICAL_SIZE_X*(*itl), 0, CRITICAL_SIZE_X*(*itl+1), CRITICAL_SIZE_Y );
						m_pCritical->Render();
						wideTemp += CRITICAL_SIZE_X*IMAGE_SCALE;
						itl++;
					}
//					// 느낌표
//					m_pCritical->Move( x+wideTemp, y-((*it).fTimer*SHOW_SPEED)-Y_START_POS);
//					m_pCritical->SetScale( IMAGE_SCALE, IMAGE_SCALE );
//					m_pCritical->SetColor( dwColor );
//					m_pCritical->SetRect( CRITICAL_SIZE_X*10, 0, CRITICAL_SIZE_X*11, CRITICAL_SIZE_Y );
//					m_pCritical->Render();
				}
				break;
			}
			wideTemp = 0;
		}
		it++;
	}





}


void CINFDamageView::Tick()
{
	FLOG( "CINFDamageView::Tick()" );

	vector<DamageView_t>::iterator it = vecDamage.begin();
	while(it != vecDamage.end())
	{
		m_fTickTimer -= g_pD3dApp->GetElapsedTime();
		it->fTimer += g_pD3dApp->GetElapsedTime();
		if(m_fTickTimer < 0)
		{
			m_fTickTimer = DAMAGE_VIEW_TIMER;
			it->nDamageViewStep++;
		}

		if(it->nDamageViewStep > DAMAGE_VIEW_STEP)
		{
			it = vecDamage.erase(it);
			continue;
		}

		it++;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFDamageView::RecvBattleShowDamage(ClientIndex_t	TIndex, int nDamage, int nDKind)
/// \brief		화면에 데미지 량 표시
/// \author		dhkwon
/// \date		2004-11-17 ~ 2004-11-17
/// \warning	현재 Critical Damage는 ShuttleChild의 데미지를 보여주는 것으로 대체되어 있다.
///				나중에 바꿀것.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
//void CINFDamageView::RecvBattleShowDamage(ClientIndex_t	TIndex, int nDamage, int nDKind)
void CINFDamageView::RecvBattleShowDamage(ClientIndex_t TIndex, int nDamage, int nDKind, USHORT MultiTargetIndex /* =/* = NULL */)
// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
{
	char buf[32];
	char cTemp[2];
	int  nCountBuff = 0;

	DamageView_t tempView;
	tempView.nDamageViewStep	= 0;
	tempView.nDamageKind		= nDKind;
	tempView.TargetIndex		= TIndex;
	tempView.fTimer				= 0;

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	tempView.fVerticalSpeed		= 0.0f;
	tempView.fHorizontallySpeed	= 0.0f;
	tempView.x					= 0.0f;
	tempView.y					= 0.0f;

	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	tempView.nMultiIndex		= MultiTargetIndex;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

	// 추가 데미지 타입일 경우
	if( nDKind == DAMAGEKIND_ADD_DAMAGE )
	{
		// x축 이동 방향(좌, 우)
		static bool bLeft = true;
		if( bLeft )
			bLeft = false;
		else
			bLeft = true;

		// x축 이동 속도
		tempView.fHorizontallySpeed	= RANDF( DAMAGE_X_MOVE_MIN, DAMAGE_X_MOVE_MAX );
		// 좌측 이동일 경우 속도 마이너스
		if( bLeft )
			tempView.fHorizontallySpeed	= -tempView.fHorizontallySpeed;
		// y축 이동 속도
		tempView.fVerticalSpeed		= RANDF( DAMAGE_Y_MOVE_MIN, DAMAGE_Y_MOVE_MAX );
	}
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	wsprintf(buf, "%d", nDamage);
	int loopcount = strlen(buf);

	for(int i=0; i<loopcount; i++)
	{	
		cTemp[0] = (char)buf[i];
		cTemp[1] = '\0';
		nCountBuff = atoi(cTemp);
		tempView.vecDamageView.push_back(nCountBuff);
	}

	vecDamage.push_back(tempView);
}