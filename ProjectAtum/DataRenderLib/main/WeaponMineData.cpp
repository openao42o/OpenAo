// WeaponMineData.cpp: implementation of the CWeaponMineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponMineData.h"
#include "EnemyData.h"
#include "MonsterData.h"
#include "AtumApplication.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "FieldWinSocket.h"
#include "AtumDatabase.h"
#include "ItemData.h"
#include "Cinema.h"
#include "WeaponItemInfo.h"
#include "dxutil.h"
#include "PkNormalTimer.h"

#define BODYCON_MINE_MOVE_TO_TARGET			BODYCON_DAMAGE1_MASK
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeaponMineData::CWeaponMineData(MSG_FC_ITEM_SHOW_ITEM* pMsg)
{
	FLOG( "CWeaponMineData(MSG_FC_ITEM_SHOW_ITEM* pMsg)" );
	m_pCharacterInfo = NULL;
	m_nClientIndex = 0;
	m_bSetPosition = TRUE;
	m_nTargetItemFieldIndex = 0;
	m_nFieldItemIndex = pMsg->ItemFieldIndex;
	m_vPos = A2DX(pMsg->Position);
	m_vStartPos = m_vPos;
	m_vTargetPos = m_vPos;
	m_fTargetLength = 0.0f;
	m_vVel = D3DXVECTOR3(0,0,1);
	m_vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtLH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_bIsRender = TRUE;
	m_dwWeaponState = _NORMAL;
	m_pTarget = NULL;
	m_nTargetIndex = 0;
	m_fWeaponSpeed = 100.0f;
	m_fObjectSize = 5.0f;
	D3DXMatrixLookAtLH( &m_mMatrix, &(m_vPos), &(m_vPos - m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	char buf[256];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
//	wsprintf(buf,"%08d",pMsg->ItemNum);
	wsprintf(buf,"%08d",pItem->SourceIndex);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}

	m_fSendAttackDataCheckTime = 0.0f;
	m_dwPartType = _MINE;
	m_ptRegion.x = (int)(m_vPos.x/(TILE_SIZE*3));
	m_ptRegion.y = (int)(m_vPos.z/(TILE_SIZE*3));
	
	m_pItemData = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
	if(m_pItemData)
		m_fCheckReactionRange = CAtumSJ::GetReactionRange( m_pItemData, &g_pShuttleChild->m_paramFactor );
	else
		m_fCheckReactionRange = 100.0f;
	SetShuttleChildOrderTarget();

	m_pCinema = NULL;		// 2004.06.30 jschoi

	// 마인은 패턴 사용 안함. 2004.06.30 jschoi
/*

//  시네마 포인트 셋팅/////////////////////////////////////////////////////////////////
	char str[32];
	sprintf(str,"%08d",info->ItemNum);
	m_pCinema = g_pScene->LoadCinemaData(str);
	if(m_pCinema == NULL)
	{
		#ifdef _DEBUG
			DBGOUT( "%s 로켓부분 패턴 파일이 존재하지 않습니다.",str);
		#endif //_DEBUG_endif
		m_pCinema = g_pScene->LoadCinemaData(PATTERN_DEFAULT);
	}
	EVENT_POINT ep;
	ep.vPosition = m_vPos;
	ep.vDirection = m_vVel;
	ep.vTarget = m_vVel;
	ep.vUpVector = m_vUp;
	D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
	ep.fVelocity = m_fWeaponSpeed;	// m_fWeaponSpeed 초기 스피드
	ep.fCurvature = DEFAULT_CURVATURE;	// 초기값 셋팅
	m_pCinema->InitCinemaData(ep);
//  초기화 종료 ///////////////////////////////////////////////////////////////////////
*/
}

CWeaponMineData::CWeaponMineData(MSG_FC_BATTLE_DROP_MINE_OK* pMsg)
{
	m_fWeaponLifeTime = 30.0f;
	m_pCharacterInfo = NULL;
	m_nClientIndex = 0;
	m_pTarget = NULL;
	if(pMsg->AttackIndex < 10000)
	{
		if(pMsg->AttackIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
		{
			m_nClientIndex = pMsg->AttackIndex;
			m_vPos = g_pShuttleChild->m_vPos;
			m_pAttacker = g_pShuttleChild;
			if( pMsg->TargetIndex != 0 )
			{
				m_nTargetIndex = pMsg->TargetIndex;
				m_pTarget = g_pScene->FindUnitDataByClientIndex( pMsg->TargetIndex );
			}
		}
		else
		{
			CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.find(pMsg->AttackIndex);
			if(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
			{
				m_vPos = itEnemy->second->m_vPos;
			}
			else
			{
				m_vPos = A2DX(pMsg->DropPosition);
			}
			m_pAttacker = itEnemy->second;
		}
	}
	else
	{
		CMapMonsterIterator itMonster = g_pD3dApp->m_pScene->m_mapMonsterList.find(pMsg->AttackIndex);
		if(itMonster != g_pD3dApp->m_pScene->m_mapMonsterList.end())
		{
			m_vPos = itMonster->second->m_vPos;
		}
		else
		{
			m_vPos = A2DX(pMsg->DropPosition);
		}
		m_pAttacker = itMonster->second;
	}

	m_bSetPosition = FALSE;
	m_vStartPos = m_vPos;
	m_nFieldItemIndex = pMsg->ItemFieldIndex;
	m_nTargetItemFieldIndex = 0;
	m_vTargetPos = A2DX(pMsg->DropPosition);
	m_fTargetLength = D3DXVec3Length(&(m_vTargetPos - m_vPos));
	if(m_fTargetLength == 0.0f)
		m_vVel = D3DXVECTOR3(0,0,1);
	else
		D3DXVec3Normalize(&m_vVel,&(m_vTargetPos - m_vPos));
	m_vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtLH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_bIsRender = TRUE;
	m_dwWeaponState = _NORMAL;
	m_fWeaponSpeed = 100.0f;
	m_fObjectSize = 5.0f;
	D3DXMatrixLookAtLH( &m_mMatrix, &(m_vPos), &(m_vPos - m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	char buf[256];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
//	wsprintf(buf,"%08d",pMsg->ItemNum);
	wsprintf(buf,"%08d",pItem->SourceIndex);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	m_fSendAttackDataCheckTime = 0.0f;
	m_dwPartType = _MINE;
	m_ptRegion.x = (int)(m_vPos.x/(TILE_SIZE*3));
	m_ptRegion.y = (int)(m_vPos.z/(TILE_SIZE*3));

	m_pItemData = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
	if(m_pItemData)
		m_fCheckReactionRange = CAtumSJ::GetReactionRange( m_pItemData, &g_pShuttleChild->m_paramFactor );
	else
		m_fCheckReactionRange = 100.0f;
	SetShuttleChildOrderTarget();

	m_pCinema = NULL;		// 2004.06.30 jschoi

	// 마인은 패턴 사용 안함. 2004.06.30 jschoi
/*

//  시네마 포인트 셋팅/////////////////////////////////////////////////////////////////
	char str[32];
	sprintf(str,"%08d",info->ItemNum);
	m_pCinema = g_pScene->LoadCinemaData(str);
	if(m_pCinema == NULL)
	{
		#ifdef _DEBUG
			DBGOUT( "%s 로켓부분 패턴 파일이 존재하지 않습니다.",str);
		#endif //_DEBUG_endif
		m_pCinema = g_pScene->LoadCinemaData(PATTERN_DEFAULT);
	}
	EVENT_POINT ep;
	ep.vPosition = m_vPos;
	ep.vDirection = m_vVel;
	ep.vTarget = m_vVel;
	ep.vUpVector = m_vUp;
	D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
	ep.fVelocity = m_fWeaponSpeed;	// m_fWeaponSpeed 초기 스피드
	ep.fCurvature = DEFAULT_CURVATURE;	// 초기값 셋팅
	m_pCinema->InitCinemaData(ep);
//  초기화 종료 ///////////////////////////////////////////////////////////////////////
*/
}

CWeaponMineData::~CWeaponMineData()
{
	FLOG( "~CWeaponMineData()" );
	if(m_pCharacterInfo)//추가
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}

	util::del(m_pCinema);
	if(m_pAttacker == g_pShuttleChild && g_pShuttleChild->m_pSecondaryWeapon)
	{
		g_pShuttleChild->m_pSecondaryWeapon->DeleteMine( this );
	}
}

void CWeaponMineData::CheckDeleteMineSendData()
{
	FLOG( "CWeaponMineData::CheckDeleteMineSendData()" );
	if(m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
	{
		MSG_FC_ITEM_DELETE_DROP_ITEM sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_FC_ITEM_DELETE_DROP_ITEM;
		sMsg.ItemFieldIndex = m_nFieldItemIndex;
		sMsg.DropPosition = m_vPos;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		m_nFieldItemIndex = 0xFFFFFFFF;
	}
}

void CWeaponMineData::Tick()
{
	FLOG( "CWeaponMineData::Tick()" );
	if(m_pAttacker == NULL)
	{
		m_bUsing = FALSE;
		return;
	}
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
//	if(m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
//	{
//		g_pShuttleChild->m_pCurrentMine[g_pShuttleChild->m_nCurrentMineCont] = this;
//		g_pShuttleChild->m_nCurrentMineCont++;
//	}
	if(m_dwWeaponState == _NORMAL)
	{
		if(m_fSendAttackDataCheckTime >= 0.0f)
			m_fSendAttackDataCheckTime -= fElapsedTime;

		if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_BULLET_MASK))
		{
			m_bodyCondition = BODYCON_BULLET_MASK;
			if(m_pCharacterInfo)
				m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
		}
		if(m_bSetPosition == FALSE )
		{
			MoveWeapon(WEAPON_FLYTYPE_STRAIGHT2X);
			if(D3DXVec3Length(&(m_vStartPos-m_vPos)) >= m_fTargetLength)
			{
				m_bSetPosition = TRUE;
				D3DXMATRIX matTemp;
				D3DXMatrixRotationAxis( &matTemp, &m_vUp, -0.5f*fElapsedTime);
				D3DXVec3TransformCoord( &m_vVel, &m_vVel, &matTemp );
			}
		}
		m_ptRegion.x = (int)(m_vPos.x/(TILE_SIZE*3));
		m_ptRegion.y = (int)(m_vPos.z/(TILE_SIZE*3));
//		if(D3DXVec3Length(&(m_vPos - g_pShuttleChild->m_vPos)) > RANGE_OF_VISION)
//		{
//			m_bUsing = FALSE; // 범위 벗어난 마인 제거
//			CheckDeleteMineSendData();
//		}
//		else 
		if(m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
		{// 마인 체크
			if( D3DXVec3Length(&(m_vPos - g_pShuttleChild->m_vPos)) > RANGE_OF_VISION ||
				m_fWeaponLifeTime < 0)
			{
				m_bUsing = FALSE; // 범위 벗어난 마인 제거
				CheckDeleteMineSendData();
			}
			else
			{
				m_fWeaponLifeTime -= g_pD3dApp->GetElapsedTime();
				if( m_pTarget == NULL || m_bSetPosition == TRUE ) // 타겟이 없는 경우, 포지션으로 이동이 완료된 경우
				{
				if(m_fSendAttackDataCheckTime < 0.0f)
				{
					if(g_pShuttleChild->IsPkEnable())
					{
						float fDist = 1000.0f;
//						CEnemyData * pTarget = NULL;

						CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
						while(it != g_pScene->m_mapEnemyList.end())
						{
							CEnemyData * pEnemy = it->second;

							if( ATTACK_AVAILABLE_STATE(pEnemy->m_dwState) && 
								(	pEnemy->IsPkAttackEnable()		||										// 2. 강제 공격이 아니거나
								(	g_pShuttleChild->m_pOrderTarget == pEnemy &&						//    OrderTarget이고
//									g_pShuttleChild->m_pPkNormalTimer->IsPkEnableNormalOrderTarget() ) ))//	  Delay Time 이 없다
									g_pShuttleChild->IsEnemyPKAttackTime(it->first) ) ))		// 2005-11-03 by ispark	  Delay Time 이 없다
							{
							
//							if( (pEnemy->IsPkEnable() || 
//								g_pShuttleChild->m_pOrderTarget == pEnemy ) &&
//								ATTACK_AVAILABLE_STATE(pEnemy->m_dwState) )
//							{
								float fTemp = D3DXVec3Length(&(pEnemy->m_vPos-m_vPos));
								if(fTemp < fDist)
								{
									fDist = fTemp;
//									pTarget = pEnemy;
									if(fDist < m_fCheckReactionRange && pEnemy)
									{
										m_bSetPosition = TRUE;
										CItemData *pTargetItem = g_pD3dApp->m_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pEnemy );
										if(pTargetItem)
										{
											SendBattleMineAttack(pEnemy->m_infoCharacter.CharacterInfo.ClientIndex,pTargetItem->m_vPos, pTargetItem->m_nItemIndex);
											m_fSendAttackDataCheckTime = 2.0f;
											return;
										}
										else
										{
											SendBattleMineAttack(pEnemy->m_infoCharacter.CharacterInfo.ClientIndex, pEnemy->m_vPos, 0);
											m_fSendAttackDataCheckTime = 2.0f;
											return;
										}
									}
								}
							}
							it++;
						}

//						for(int i = m_ptRegion.x - 1;i < m_ptRegion.x + 1;i++)
//						{
//							for(int j = m_ptRegion.y - 1;j < m_ptRegion.y + 1;j++)
//							{
//								if( i >= 0 && 
//									i < g_pD3dApp->m_pScene->m_nBlockSizeX && 
//									j >= 0 && 
//									j < g_pD3dApp->m_pScene->m_nBlockSizeX)
//								{
//									CVecEnemyIterator it = g_pD3dApp->m_pScene->m_vecEnemyBlockList[i*g_pD3dApp->m_pScene->m_nBlockSizeY + j].begin();
//									while(it != g_pD3dApp->m_pScene->m_vecEnemyBlockList[i*g_pD3dApp->m_pScene->m_nBlockSizeY + j].end())
//									{
//										CEnemyData * pEnemy = *it;
//										if( (pEnemy->IsPkEnable() || g_pShuttleChild->m_pOrderTarget == pEnemy)&& 
//											ATTACK_AVAILABLE_STATE(pEnemy->m_dwState) )
//										{
//											float fTemp = D3DXVec3Length(&(pEnemy->m_vPos-m_vPos));
//											if(fTemp < fDist)
//											{
//												fDist = fTemp;
//												pTarget = pEnemy;
//												if(fDist < m_fCheckReactionRange && pTarget)
//												{
//													m_bSetPosition = TRUE;
//													CItemData *pTargetItem = g_pD3dApp->m_pScene->FindFieldItemByPartTypeAndParent( _DUMMY, pTarget );
//													if(pTargetItem)
//													{
//														SendBattleMineAttack(pTarget->m_infoCharacter.CharacterInfo.ClientIndex,pTargetItem->m_vPos, pTargetItem->m_nItemIndex);
//														m_fSendAttackDataCheckTime = 2.0f;
//														return;
//													}
//													else
//													{
//														SendBattleMineAttack(pTarget->m_infoCharacter.CharacterInfo.ClientIndex, pTarget->m_vPos, 0);
//														m_fSendAttackDataCheckTime = 2.0f;
//														return;
//													}
//												}
//											}
//										}
//										it++;
//									}
//								}
//							}
//						}
					}
					float fDist = 1000.0f;
//					CMonsterData * pTarget = NULL;
					CMapMonsterIterator it = g_pScene->m_mapMonsterList.begin();
					while(it != g_pScene->m_mapMonsterList.end())
					{
						CMonsterData * pMonster = it->second;
						if(ATTACK_AVAILABLE_STATE(pMonster->m_dwState))
						{
							float fTemp = D3DXVec3Length(&(pMonster->m_vPos - m_vPos));
							if(fTemp < fDist)
							{
								fDist = fTemp;
//								pTarget = pMonster;
								if(fDist < m_fCheckReactionRange && pMonster)
								{
									m_bSetPosition = TRUE;
									SendBattleMineAttack(pMonster->m_info.MonsterIndex, pMonster->m_vPos, 0);
									m_fSendAttackDataCheckTime = 2.0f;
									return;
								}
							}
						}
						it++;
					}
//					for(int i = m_ptRegion.x - 1;i < m_ptRegion.x + 1;i++)
//					{
//						for(int j = m_ptRegion.y - 1;j < m_ptRegion.y + 1;j++)
//						{
//							if( i >= 0 && 
//								i < g_pD3dApp->m_pScene->m_nBlockSizeX && j >= 0 && 
//								j < g_pD3dApp->m_pScene->m_nBlockSizeX)
//							{
//								CVecMonsterIterator it = g_pD3dApp->m_pScene->m_vecMonsterList[i*g_pD3dApp->m_pScene->m_nBlockSizeX + j].begin();
//								while(it != g_pD3dApp->m_pScene->m_vecMonsterList[i*g_pD3dApp->m_pScene->m_nBlockSizeX + j].end())
//								{
//									CMonsterData * pMonster = *it;
//									if(ATTACK_AVAILABLE_STATE(pMonster->m_dwState))
//									{
//										float fTemp = D3DXVec3Length(&(pMonster->m_vPos-m_vPos));
//										if(fTemp < fDist)
//										{
//											fDist = fTemp;
//											pTarget = pMonster;
//											if(fDist < m_fCheckReactionRange && pTarget)
//											{
//												m_bSetPosition = TRUE;
//												SendBattleMineAttack(pTarget->m_info.MonsterIndex, pTarget->m_vPos, 0);
//												m_fSendAttackDataCheckTime = 2.0f;
//												return;
//											}
//										}
//									}
//									it++;
//								}
//							}
//						}
//					}
				}
			}
				else // if(m_pTarget != NULL) // 타겟이 있는 경우
				{
					if(ATTACK_AVAILABLE_STATE(m_pTarget->m_dwState))
					{
						float fDist = D3DXVec3Length(&(m_pTarget->m_vPos - m_vPos));
						if(fDist < m_fCheckReactionRange)
						{
							m_bSetPosition = TRUE;
							SendBattleMineAttack(m_nTargetIndex, m_pTarget->m_vPos, 0);
							m_fSendAttackDataCheckTime = 2.0f;
							return;
						}
					}
				}
			}
		}
	}
	else if(m_dwWeaponState == _EXPLODING)
	{
		if(m_pTarget)
		{
			if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_MINE_MOVE_TO_TARGET))
			{
				m_bodyCondition = BODYCON_MINE_MOVE_TO_TARGET;
				if(m_pCharacterInfo)
					m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
			}
			if(m_fWeaponSpeed < 300.0f)
				m_fWeaponSpeed += 150.0f*fElapsedTime;
			if(m_fWeaponSpeed > 300.0f)
				m_fWeaponSpeed = 300.0f;
			D3DXVECTOR3 vTargetPos;
			if(m_nTargetItemFieldIndex != 0)
			{
				CItemData *pItem = g_pD3dApp->m_pScene->FindFieldItemByFieldIndex( m_nTargetItemFieldIndex );
				if(pItem)
				{
					vTargetPos = pItem->m_vPos;
					D3DXVec3Normalize(&m_vVel,&(vTargetPos - m_vPos));
				}
				else
				{
					if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_HIT_MASK))
					{
						m_bodyCondition = BODYCON_HIT_MASK;
						if(m_pCharacterInfo)
							m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
					}
					m_dwWeaponState = _EXPLODED;
					return;
				}
			}
			else
			{
				vTargetPos = m_pTarget->m_vPos;
				D3DXVec3Normalize(&m_vVel,&(m_pTarget->m_vPos - m_vPos));
			}
			MoveWeapon(WEAPON_FLYTYPE_STRAIGHT1X);
			/*
			// MoveWeapon 대신 패턴 적용
			BOOL bResult;							// 목표점에 도달했는지 결과..
			bResult=m_pCinema->Tick(m_vTargetPos);	// 결과가 FALSE 면 목표를 지났다.. 즉 도달했다.
			EVENT_POINT ep;
			ep=m_pCinema->GetCurrentCinemaPoint();
			m_vPos = ep.vPosition;			// 위치
			m_vVel = ep.vDirection;			// 방향
			m_fWeaponSpeed = ep.fVelocity;	// 속력
			m_vUp = ep.vUpVector;
			*/
			
//			m_vPos += m_fWeaponSpeed*vVel*fElapsedTime;
			float fLength = D3DXVec3Length(&(m_vPos - vTargetPos));
			if(fLength < m_pItemData->ExplosionRange)//15.0f)
			{

				/* add by jsy */
				if(m_pTarget)
				{
					if(!((CUnitData*)m_pTarget)->m_bShielding)
					{
						// 랜덤하게 위치를 조정한다.
						D3DXVECTOR3 vVel;
						D3DXVec3Normalize(&vVel, &(m_vStartPos - m_pTarget->m_vPos));
						vVel = D3DXVECTOR3( vVel.x*((float)RANDI(0, 4)),vVel.y*((float)RANDI(0, 4)),vVel.z*((float)RANDI(0, 4)));
						m_vPos = m_pTarget->m_vPos + vVel;
					}
					else	// 쉴드 발동 중인경우 좌표는 쉴드크기 만큼으로 조정한다.
					{// 쉴드 발동 중일때 쉴드 Hit 이펙트 추가
						D3DXVECTOR3 vVel;
						D3DXVec3Normalize(&vVel, &(m_vStartPos - m_pTarget->m_vPos));
						m_vPos = m_pTarget->m_vPos + vVel*SIZE_OF_SHIELD_EFFECT;
						((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_vStartPos);
					}
				}
			

				if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_HIT_MASK))
				{
					m_bodyCondition = BODYCON_HIT_MASK;
					if(m_pCharacterInfo)
						m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
				}
				m_dwWeaponState = _EXPLODED;
				if(m_nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
				{
					if(m_pTarget->m_dwPartType == _ENEMY)
					{
						SendBattleMineAttackFind(
							((CEnemyData *)m_pTarget)->m_infoCharacter.CharacterInfo.ClientIndex, 
							((CEnemyData *)m_pTarget)->m_vPos,
							m_nTargetItemFieldIndex );

					}
					else if(m_pTarget->m_dwPartType == _MONSTER)
					{
						SendBattleMineAttackFind(
							((CMonsterData *)m_pTarget)->m_info.MonsterIndex, 
							((CMonsterData *)m_pTarget)->m_vPos,
							m_nTargetItemFieldIndex );

					}
				}
			}
		}
		else
		{
			m_dwWeaponState = _EXPLODED;
		}
	}
	else if(m_dwWeaponState == _EXPLODED)
	{
		if(m_pCharacterInfo)
		{
			if(!m_pCharacterInfo->IsUsing())
			{
				m_bUsing = FALSE;
			}
		}
		else
		{
			m_bUsing = FALSE;
		}
	}
	D3DXMatrixLookAtRH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

	// 스크린상의 좌표
	g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		m_nObjScreenX, m_nObjScreenY, m_nObjScreenW );
	if(m_nObjScreenX > -m_fObjectSize && m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width+m_fObjectSize 
		&& m_nObjScreenY > -m_fObjectSize && m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height+m_fObjectSize 
		&& m_nObjScreenW > -m_fObjectSize && D3DXVec3Length(&(g_pShuttleChild->m_vPos-m_vPos)) < 250.0f)
		m_bIsRender = TRUE;
	else
		m_bIsRender = FALSE;
	// effect matrix & ticking
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix );
//		m_pCharacterInfo->SetSingleBodyConditionMatrix( BODYCON_FIRE_MASK,m_mFireMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}
}


void CWeaponMineData::SendBattleMineAttack(ClientIndex_t nClientIndex,
										   D3DXVECTOR3 vTargetPos,
										   UINT nItemIndex)
{
	MSG_FC_BATTLE_MINE_ATTACK sMsg;
	sMsg.TargetInfo.TargetIndex = nClientIndex;
	sMsg.TargetInfo.TargetItemFieldIndex = nItemIndex;
	sMsg.TargetInfo.TargetPosition = vTargetPos;
	sMsg.ItemFieldIndex = m_nFieldItemIndex;
#pragma omp critical
	g_pFieldWinSocket->SendMsg(T_FC_BATTLE_MINE_ATTACK, (char*)&sMsg, sizeof(sMsg));
}

void CWeaponMineData::SendBattleMineAttackFind(ClientIndex_t nClientIndex,
											   D3DXVECTOR3 vTargetPos,
											   UINT nItemIndex)
{
	MSG_FC_BATTLE_MINE_ATTACK_FIND sMsg;
	sMsg.TargetInfo.TargetIndex = nClientIndex;
	sMsg.TargetInfo.TargetItemFieldIndex = nItemIndex;
	sMsg.TargetInfo.TargetPosition = vTargetPos;
	sMsg.ItemFieldIndex = m_nFieldItemIndex;
#pragma omp critical
	g_pFieldWinSocket->SendMsg(T_FC_BATTLE_MINE_ATTACK_FIND, (char*)&sMsg, sizeof(sMsg));
	m_nFieldItemIndex = 0xFFFFFFFF;
}
