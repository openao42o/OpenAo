// ItemData.cpp: implementation of the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemData.h"
#include "AtumData.h"
#include "ChatMoveData.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "dxutil.h"
//#include "INFNotice.h"
#include "TutorialSystem.h"
#include "StoreData.h"
#include "Chat.h"

#define BODYCON_FIXER_MOVE_TO_TARGET			BODYCON_DAMAGE1_MASK


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define NUM_ITEMTYPE_ITEMFIELD		50000
#define ITEMDATA_DEFAULT_SPEED		50.0f
#define ITEMDATA_DEFAULT_SIZE		5.0f
#define CHAFFGAP					7.0f
#define CHAFF_POS_Y					-3.75f
#define CHAFF_POS_Z					17.52f

CItemData::CItemData(MSG_FC_ITEM_SHOW_ITEM* pMsg)
{
	FLOG( "CItemData(MSG_FC_ITEM_SHOW_ITEM* pMsg)" );
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	m_bInfinityEffect = FALSE;

	m_pCharacterInfo = NULL;

	m_dwPartType = _ITEMFIELD;
	m_pParent = NULL;
	m_nItemNum = pMsg->ItemNum;
	m_nItemIndex = pMsg->ItemFieldIndex;
	m_nFirstCharacterUID = pMsg->FirstCharacterUID;
	m_vPos = A2DX(pMsg->Position);
	m_vVel = D3DXVECTOR3(0,0,1);
	m_vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtLH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_fCheckPacketTime = 0.0f;
	m_dwState = _NORMAL;
	m_pTarget = NULL;
	m_fSpeed = ITEMDATA_DEFAULT_SPEED;
	m_fObjectSize = ITEMDATA_DEFAULT_SIZE;
	m_bIsRender = TRUE;
	char buf[256];
	// 2005-04-26 by jschoi
	m_byKind = 0;
	ITEM* pItem = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
	if(pItem)
	{
		wsprintf(buf,"0120%04d",pItem->Kind);
		LoadCharacterEffect(buf);
		// 2008-07-24 by dgwoo ItemKind에 따라 습득여부 결정.
		m_byKind = pItem->Kind;
	}
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}
	else
	{
		// temporary item
		LoadCharacterEffect("01200016");
		if(m_pCharacterInfo)
		{
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
		}
		else
		{
			g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_050512_0001);
			return;
		}
	}
	m_nCase = 0;
	m_bTurnCheck = FALSE;
	m_fItemCheckTime = 0.0f;
	ITEM* pItemInfo = g_pDatabase->GetServerItemInfo( pMsg->ItemNum );
	if(pItemInfo)
	{
		if(IS_COUNTABLE_ITEM(pItemInfo->Kind) && pMsg->Amount > 1)
		{
			wsprintf(buf,STRMSG_C_ITEM_0001,pItemInfo->ItemName, pMsg->Amount);
		}
		else
		{
			wsprintf(buf,"%s",pItemInfo->ItemName);
		}
		m_pMoveChat = new CChatMoveData(nullptr, buf, COLOR_ITEM, 9, 1024, 32);
		m_pMoveChat->InitDeviceObjects();
		m_pMoveChat->RestoreDeviceObjects();
		AddChild(m_pMoveChat);
	}
	else
	{
		DBGOUT("GetServerItemInfo() is NULL (Drop Item:%d)\n",pMsg->ItemNum );//아이템 정보 없음(바닥에 떨어진 아이템:%d)\n
		m_pMoveChat = NULL;
	}
}

//CItemData::CItemData(CAtumNode * pParent,
//					 MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg)
CItemData::CItemData(CAtumNode * pParent,INT nItemNum, UINT nFieldIndex)
{
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	m_bInfinityEffect = FALSE;

	m_pCharacterInfo = NULL;

	m_dwPartType = _DUMMY;
	m_pParent = pParent;
	if(!m_pParent)
	{
		m_bUsing = FALSE;
		return;
	}

//	D3DXVECTOR3 vTempPos;
//	vTempPos.x = rand()%30 - 15;
//	vTempPos.y = rand()%30 - 15;
//	vTempPos.z = rand()%30 - 15;
//	D3DXVec3Normalize(&vTempPos,&vTempPos);
//	m_vTargetPos = 15.0f*vTempPos;

//	int nCount = g_pScene->GetDummyCountOfUnit((CAtumData*)m_pParent);
	int nCount = ((CUnitData*)m_pParent)->GetChaffDataCound();
	// 2006-12-05 by dgwoo 체프의 위치를 자동으로 셋팅.
	ChaffPos(nCount,CHAFFGAP);

	D3DXVec3TransformCoord(&m_vPos, &m_vTargetPos, &((CAtumData*)m_pParent)->m_mMatrix);
	m_nItemNum = nItemNum;
	m_nItemIndex = nFieldIndex;
	m_vVel = ((CAtumData*)m_pParent)->m_vVel;
	m_vUp = ((CAtumData*)m_pParent)->m_vUp;
	D3DXMatrixLookAtLH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_fCheckPacketTime = 0.0f;
	m_dwState = _NORMAL;
	m_pTarget = NULL;
	m_fSpeed = ITEMDATA_DEFAULT_SPEED;
	m_fObjectSize = ITEMDATA_DEFAULT_SIZE;
	m_bIsRender = TRUE;
	char buf[256];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(m_nItemNum);
//	wsprintf(buf,"%08d",pMsg->ItemNum);
// 2006-12-04 by ispark, Chaff용 이펙트 번호
	wsprintf(buf,"%08d",RC_EFF_CHAFF_SKILL);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}

//	m_nCase = rand()%3;
	m_bTurnCheck = FALSE;
	m_fItemCheckTime = 0.0f;
	m_pMoveChat= NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData
/// \brief		_LAY 류 아이템을 생성한다.
/// \author		dgwoo
/// \date		2007-02-08 ~ 2007-02-08
/// \warning	
///
/// \param		부모의 주소값, 존재하는 시간.
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData::CItemData(CAtumNode * pParent, float fCheckTime, UINT uObjId, DWORD dwPartType, 
							D3DXVECTOR3	vVel, D3DXVECTOR3	vUp,
							D3DXVECTOR3	*i_pPos /*= NULL*/)
{
	m_pCharacterInfo = NULL;

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//m_dwPartType = _LAY;
	m_dwPartType = dwPartType;	
	m_bInfinityEffect = FALSE;	
	// end 2007-04-19 by bhsohn 서치아이 아이템 추가

	m_pParent = pParent;
	if(!m_pParent)
	{
		m_bUsing = FALSE;
		return;
	}
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//D3DXVec3TransformCoord(&m_vPos, &m_vTargetPos, &((CAtumData*)m_pParent)->m_mMatrix);
	if(NULL == i_pPos)
	{
	D3DXVec3TransformCoord(&m_vPos, &m_vTargetPos, &((CAtumData*)m_pParent)->m_mMatrix);
	}
	else
	{
		m_vPos = (*i_pPos);
	}

//	m_nItemNum = pMsg->ItemNum;
//	m_nItemIndex = pMsg->ItemFieldIndex;
	m_vVel = ((CAtumData*)m_pParent)->m_vVel;
	m_vUp = ((CAtumData*)m_pParent)->m_vUp;

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
//	m_vVel = D3DXVECTOR3(0,0,1);
//	m_vUp = D3DXVECTOR3(0,1,0);
	m_vVel = vVel;
	m_vUp = vUp;	
	if(fCheckTime <= 0)
	{
		// 체크 타임이 0보다 작거나 같으면 무한 돌기
		m_bInfinityEffect = TRUE;
		fCheckTime  = 1000.0f;
	}
	// end 2007-04-19 by bhsohn 서치아이 아이템 추가	
	
	D3DXMatrixLookAtLH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_fCheckPacketTime = 0.0f;
	m_fItemCheckTime = fCheckTime;
	m_dwState = _NORMAL;
	m_pTarget = NULL;
	m_fSpeed = ITEMDATA_DEFAULT_SPEED;
	m_fObjectSize = ITEMDATA_DEFAULT_SIZE;
	m_bIsRender = TRUE;
	char buf[256];
//	wsprintf(buf,"%08d",pMsg->ItemNum);

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//wsprintf(buf,"%08d",RC_EFF_SCAN_SKILL);
	wsprintf(buf,"%08d",uObjId);	
	// end 2007-04-19 by bhsohn 서치아이 아이템 추가
	

	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}

//	m_nCase = rand()%3;
	m_bTurnCheck = FALSE;
	
	m_pMoveChat= NULL;
}

CItemData::CItemData(CAtumNode * pParent,// target
					 CAtumData * pAttacker,
					 MSG_FC_BATTLE_DROP_FIXER_OK* pMsg)
{
	FLOG( "CItemData(CAtumNode * pParent,CAtumData * pAttacker,MSG_FC_BATTLE_DROP_FIXER_OK* pMsg)" );
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	m_bInfinityEffect = FALSE;

	m_pCharacterInfo = NULL;

	m_dwPartType = _FIXER;

	D3DXVECTOR3 vTempPos;
	vTempPos.x = RANDI(-15, 15);
	vTempPos.y = RANDI(-15, 15);
	vTempPos.z = RANDI(-15, 15);
	// todo : this vector could be (0,0,0)
	D3DXVec3Normalize(&vTempPos,&vTempPos);
	m_vTargetPos = 15.0f*vTempPos;

//	m_vTargetPos = D3DXVECTOR3(5,5,5);
	m_pParent = pParent;
	m_nItemNum = pMsg->ItemNum;
	m_nItemIndex = pMsg->ItemFieldIndex;
	if(pAttacker)
		m_vPos = pAttacker->m_vPos;
	else
		m_vPos = ((CAtumData *)pParent)->m_vPos + m_vTargetPos;
	if(m_pParent)
		D3DXVec3Normalize(&m_vVel,&((CAtumData *)m_pParent)->m_vVel);
	else
	{
		if(pAttacker)
		{
			m_vVel = pAttacker->m_vVel;
		}
		else
		{
			m_vVel = D3DXVECTOR3(0,0,1);
		}
	}
	m_vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtLH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_fCheckPacketTime = 0.0f;
	m_dwState = _NORMAL;
	m_pTarget = NULL;
	m_fSpeed = ITEMDATA_DEFAULT_SPEED;
	m_fObjectSize = ITEMDATA_DEFAULT_SIZE;
	m_bIsRender = TRUE;
	char buf[256];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
//	wsprintf(buf,"%08d",pMsg->ItemNum);//+50000);
	wsprintf(buf,"%08d",pItem->SourceIndex);//+50000);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_FIXER_MOVE_TO_TARGET;
	if(m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}

	m_nCase = 0;
	m_bTurnCheck = FALSE;
	m_fItemCheckTime = 0.0f;
	m_pMoveChat = NULL;
}

CItemData::CItemData(int nItemNum)
{
	FLOG( "CItemData(int nItemNum)" );
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	m_bInfinityEffect = FALSE;

	m_pCharacterInfo = NULL;

	m_dwPartType = _ITEMFIELD;
	m_pParent = NULL;
	m_nItemNum = nItemNum;
	m_nItemIndex = nItemNum;			// 임시
	m_dwState = _NORMAL;
	char buf[256];
	// 2005-04-26 by jschoi
	ITEM* pItem = g_pDatabase->GetServerItemInfo(nItemNum);
	wsprintf(buf,"0120%04d",pItem->Kind);
//	wsprintf(buf,"%08d",nItemNum + NUM_ITEMTYPE_ITEMFIELD);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}

	m_nCase = 0;
	m_bTurnCheck = FALSE;
	m_fItemCheckTime = 0.0f;
	m_pMoveChat = NULL;
}

CItemData::~CItemData()
{
	FLOG( "~CItemData()" );
	if(m_pCharacterInfo)//추가
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CItemData::ChaffPos(nCount,CHAFFGAP)
/// \brief		체프의 위치를 동적으로 잡아줌.
/// \author		dgwoo
/// \date		2006-12-05 ~ 2006-12-05
/// \warning	
///
/// \param		nCount : 생성전의 체프의 갯수. fGap : 체프의 간격.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemData::ChaffPos(int nCount,float fGap)
{
	bool btemp = nCount & 1;		
	int	nHalf = 0;					// 한쪽의 카운터 값.
	float fOldPos = 0.0f;
	if(btemp)
	{// 짝수의 체프 좌표를 설정.
		nHalf = (nCount+1) / 2;
		fOldPos = -(nHalf * fGap) + (fGap/2);
		while(nCount > 0)
		{
			((CUnitData*)m_pParent)->m_pChaffData[nCount-1]->m_vTargetPos = D3DXVECTOR3( fOldPos,CHAFF_POS_Y,CHAFF_POS_Z);
			fOldPos += fGap;
			nCount--;
		}

	}
	else
	{// 홀수의 체프 좌표를 설정.
		nHalf = nCount / 2;
		fOldPos = -(nHalf * fGap);
		while(nCount > 0)
		{
			((CUnitData*)m_pParent)->m_pChaffData[nCount-1]->m_vTargetPos = D3DXVECTOR3( fOldPos,CHAFF_POS_Y,CHAFF_POS_Z);
			fOldPos += fGap;
			nCount--;
		}

	}
	m_vTargetPos = D3DXVECTOR3( fOldPos,CHAFF_POS_Y,CHAFF_POS_Z);

	return ;
}
void CItemData::Tick()
{
	FLOG( "CItemData::Tick()" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//m_fItemCheckTime -= fElapsedTime * 1000;
	if(FALSE == m_bInfinityEffect)
	{
	m_fItemCheckTime -= fElapsedTime * 1000;
	}
	if(m_dwState == _NORMAL)
	{
		if(m_dwPartType == _ITEMFIELD)
		{
			if(m_fCheckPacketTime >= 0.0f)
				m_fCheckPacketTime -= g_pD3dApp->GetElapsedTime();
			float fLength = D3DXVec3Length(&(m_vPos - g_pShuttleChild->m_vPos));

			// 2005-03-28 by jschoi - 프리미엄 시 아이템 자동 습득
//			if(fLength < GET_ITEM_FROM_FIELD_DISTANCE)//50
//			{
			if( fLength < GET_ITEM_FROM_FIELD_DISTANCE ||														// 아무나 줏어 먹을 수 있는 거리이거나.
				g_pTutorial->IsTutorialMode() == TRUE ||		// 튜토리얼 시 아이템 자동 습득
//				(g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1 &&			// 2006-07-05 by ispark, 프리미엄 개념 변경, 프리미엄 상태에서 나의 아이템이라면 아이템을 자동 습득한다.
				  m_nFirstCharacterUID == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber) 
			{
				if(g_pShuttleChild->m_myShuttleInfo.Level < 6)
				{
//					g_pGameMain->m_pNotice->SetNotice( "떨어진 아이템은 아이템 근처로 이동한 후 키보드 'Q'를 누르시면 습득됩니다." );
					g_pGameMain->SetNotice( 13 );
				}
				
				// 2008-07-24 by dgwoo 인벤토리에 여유가 있거나 카운터블 아이템으로 기존에 인벤에 있을경우 
				//                        서버로부터 아이템을 요청한다.
				// 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
				//if(g_pStoreData->GetTotalUseInven() < CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1)
				// 2009. 12. 17 by jskim 캐쉬(인벤/창고 확장) 재수정 
				//CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
				//if(g_pStoreData->GetTotalUseInven() < CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1)
				int nMyInventory = g_pStoreData->GetTotalUseInven();
				int nMaxMyInventory = CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1;
				if( nMyInventory < nMaxMyInventory )
				//end 2009. 12. 17 by jskim 캐쉬(인벤/창고 확장) 재수정 
				//end 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
				{// 인벤토리에 여유가 있을경우는 서버로 바로 요청.
					if(m_fCheckPacketTime <= 0.0f)
					{
						SendItemGetItem(nMyInventory, nMaxMyInventory);
					}
				}
				else if(IS_COUNTABLE_ITEM(m_byKind))
				{// 카운터블 아이템일경우 현재 인벤에 있는지 체크후 요청.

					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(m_nItemNum);
					if(pItemInfo)
					{
						if(m_fCheckPacketTime <= 0.0f)
						{
							SendItemGetItem(nMyInventory, nMaxMyInventory);
						}
					}
					// 2008-08-13 by dgwoo 인벤토리에 없을경우에도 서버로 보냄 에러 메시지가 찍히도록..
					else
					{
						g_pScene->InvectoryFullMessage();
					}

				}
				else
				{
 					g_pScene->InvectoryFullMessage();
				}
			}
//			else if(fLength > DETETE_FIELD_ITEM_DISTANCE)
			else if(fLength > GetMapItemVisibleDistance(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) + 100)
			{
				// 2006-01-13 by ispark, 예전에는 DETETE_FIELD_ITEM_DISTANCE(750)만큼 지나면 지웠다.
				// 그리고나서 다시 가면 없다. 왜냐면 서버에서는 일정거리 안에 들어야 데이타를 넘긴다.
				// 그 일정거리 보다 100 넓게 잡았다. 그전에는 데이타를 삭제 하지 않는다.
				m_bUsing = FALSE;
			}
		}
		else if(m_dwPartType == _DUMMY)
		{
			if(!m_pParent)
			{
				m_bUsing = FALSE;
				return;
			}
			D3DXVec3TransformCoord(&m_vPos, &m_vTargetPos, &((CAtumData*)m_pParent)->m_mMatrix);
			m_vVel = ((CAtumData*)m_pParent)->m_vVel;
			m_vUp = ((CAtumData*)m_pParent)->m_vUp;
		
		}
		// 2007-02-08 by dgwoo 스캔관련 오브젝트 생성과 고정형으로 한다.
		else if(m_dwPartType == _LAY)
		{
			if(!m_pParent)
			{
				m_bUsing = FALSE;
				return;
			}
			
		}
		else if(m_dwPartType == _FIXER)
		{
			if(m_pParent)
			{
				if(!m_bTurnCheck && 
					D3DXVec3Length(&(((CAtumData *)m_pParent)->m_vPos - m_vPos)) > D3DXVec3Length(&m_vTargetPos))
				{
					D3DXVECTOR3 vVel;
					m_vPos += 0.5f*m_fSpeed*(((CAtumData *)m_pParent)->m_vPos - m_vPos)*fElapsedTime;
					if(D3DXVec3Length(&(((CAtumData *)m_pParent)->m_vPos - m_vPos)) < D3DXVec3Length(&m_vTargetPos))
					{
/*						if(g_pShuttleChild == m_pParent)
						{
							ITEM *pItem = g_pDatabase->GetServerItemInfo(m_nItemNum);
							if(pItem)
							{
								SetParamFactor_DesParam( g_pShuttleChild->m_paramFactor , pItem->DestParameter1, pItem->ParameterValue1);	
								SetParamFactor_DesParam( g_pShuttleChild->m_paramFactor , pItem->DestParameter2, pItem->ParameterValue2);
								SetParamFactor_DesParam( g_pShuttleChild->m_paramFactor , pItem->DestParameter3, pItem->ParameterValue3);
								SetParamFactor_DesParam( g_pShuttleChild->m_paramFactor , pItem->DestParameter4, pItem->ParameterValue4);
								if( pItem->DestParameter1 == DES_SHOTNUM_01 || 
									pItem->DestParameter1 == DES_MULTINUM_01 ||
									pItem->DestParameter1 == DES_REATTACKTIME_01 ||
									pItem->DestParameter2 == DES_SHOTNUM_01 || 
									pItem->DestParameter2 == DES_MULTINUM_01 || 
									pItem->DestParameter2 == DES_REATTACKTIME_01 ||
									pItem->DestParameter3 == DES_SHOTNUM_01 || 
									pItem->DestParameter3 == DES_MULTINUM_01 || 
									pItem->DestParameter3 == DES_REATTACKTIME_01 ||
									pItem->DestParameter4 == DES_SHOTNUM_01 || 
									pItem->DestParameter4 == DES_MULTINUM_01 || 
									pItem->DestParameter4 == DES_REATTACKTIME_01)
								{
									g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
								}
								else if( pItem->DestParameter1 == DES_SHOTNUM_02 || 
									pItem->DestParameter1 == DES_MULTINUM_02 ||
									pItem->DestParameter1 == DES_REATTACKTIME_02 ||
									pItem->DestParameter2 == DES_SHOTNUM_02 || 
									pItem->DestParameter2 == DES_MULTINUM_02 || 
									pItem->DestParameter2 == DES_REATTACKTIME_02 ||
									pItem->DestParameter3 == DES_SHOTNUM_02 || 
									pItem->DestParameter3 == DES_MULTINUM_02 || 
									pItem->DestParameter3 == DES_REATTACKTIME_02 ||
									pItem->DestParameter4 == DES_SHOTNUM_02 || 
									pItem->DestParameter4 == DES_MULTINUM_02 || 
									pItem->DestParameter4 == DES_REATTACKTIME_02)
								{
									g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
								}
							}
						}
*/						m_bTurnCheck = TRUE;
						// 상대방에 붙었을때..BULLET으로 Change
						m_bodyCondition = BODYCON_BULLET_MASK;
						if(m_pCharacterInfo)
							m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
						m_nCase = RANDI(0, 2);
					}
				}
				else
				{
					D3DXMATRIX mat;
					D3DXVECTOR3 vAxis;
//					if(((int)m_fItemCheckTime)/3 == 0)
//						m_nCase = rand()%3;
					if(m_nCase == 0)
					{// x 축으로 회전
						vAxis = D3DXVECTOR3(1,0,0);
						D3DXMatrixRotationAxis( &mat, &vAxis, D3DX_PI*fElapsedTime );
					}
					else if(m_nCase == 1)
					{// y 축으로 회전
						vAxis = D3DXVECTOR3(0,1,0);
						D3DXMatrixRotationAxis( &mat, &vAxis, D3DX_PI*fElapsedTime );
					}
					else
					{// z 축으로 회전
						vAxis = D3DXVECTOR3(0,0,1);
						D3DXMatrixRotationAxis( &mat, &vAxis, D3DX_PI*fElapsedTime );
					}
					D3DXVec3TransformCoord(&m_vTargetPos, &m_vTargetPos, &mat);
					m_vPos = ((CAtumData *)m_pParent)->m_vPos + 0.9f*m_vTargetPos;
				}
			}
			else
			{
				m_vPos += 3.0f*m_fSpeed*m_vVel*fElapsedTime;
				if(D3DXVec3Length(&(m_vPos - g_pShuttleChild->m_vPos)) > 500.0f)
					m_bUsing = FALSE;
			}
		}
	}
	else if(m_dwState == _EXPLODING)
	{
		if(m_dwPartType == _ITEMFIELD)
		{
			if(m_pTarget)
			{
				if(m_fSpeed < 300.0f)
					m_fSpeed += 120.0f*fElapsedTime;

				// 2006-09-20 by ispark, 목표와의 거리가 계산된 거리보다 짧으면 먹은걸로 계산
				D3DXVECTOR3 vVel;
				float fRealLen = 0.0f;
				fRealLen = D3DXVec3Length(&(m_vPos  - m_pTarget->m_vPos));		// 목표와의 거리

				D3DXVec3Normalize(&vVel,&(m_pTarget->m_vPos - m_vPos));
				m_vPos += m_fSpeed*vVel*fElapsedTime;
				float fLength = D3DXVec3Length(&(m_vPos  - m_pTarget->m_vPos));	// 계산된 거리
				if( fLength < 10.0f || (fRealLen < fLength))
				{
					if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_HIT_MASK))
					{
						m_bodyCondition = BODYCON_HIT_MASK;
						if(m_pCharacterInfo)
							m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
					}
					m_dwState = _EXPLODED;
				}
			}
			else
			{
				m_dwState = _EXPLODED;
			}
		}
		else if(m_dwPartType == _DUMMY)
		{
			if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_HIT_MASK))
			{
				m_bodyCondition = BODYCON_HIT_MASK;
				if(m_pCharacterInfo)
					m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
			}
			m_dwState = _EXPLODED;
		}
		else if(m_dwPartType == _LAY)
		{
			if(!COMPARE_BODYCON_BIT(m_bodyCondition,BODYCON_HIT_MASK))
			{
				m_bodyCondition = BODYCON_HIT_MASK;
				if(m_pCharacterInfo)
					m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
			}
			m_dwState = _EXPLODED;
		}
	}
	else if(m_dwState == _EXPLODED)
	{
		m_bUsing = FALSE;
	}
	D3DXMatrixLookAtRH(&m_mMatrix,&m_vPos,&(m_vPos+m_vVel),&m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	// 스크린상의 좌표
	g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		m_nObjScreenX, m_nObjScreenY, m_nObjScreenW );
	if(m_nObjScreenX > -m_fObjectSize && m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width+m_fObjectSize 
		&& m_nObjScreenY > -m_fObjectSize && m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height+m_fObjectSize 
		&& m_nObjScreenW > -m_fObjectSize )//&& D3DXVec3Length(&(g_pShuttleChild->m_vPos-m_vPos)) < 250.0f)
		m_bIsRender = TRUE;
	else
		m_bIsRender = FALSE;
	// effect matrix & ticking
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}
}

void CItemData::LoadCharacterEffect(char* strFileName)
{
	FLOG( "CItemData::LoadCharacterEffect(char* strFileName)" );
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	m_pCharacterInfo = new CCharacterInfo();
	if(m_pCharacterInfo->Load(strFileName))
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->RestoreDeviceObjects();
	}
	else
	{
		util::del(m_pCharacterInfo);
		DBGOUT("CItemData::LoadCharacterEffect,  Can't find effect File .\n");//캐릭터 파일이 없습니다.\n
	}
}

void CItemData::RenderItemName()
{
	if(m_pMoveChat && m_bIsRender)
	{
		m_pMoveChat->Render( m_nObjScreenX, m_nObjScreenY );
	}
}

void CItemData::SendItemGetItem( const int& _nMyInventory, const int& nMaxMyInventory )
{
	m_fCheckPacketTime = 1.0f;
	MSG_FC_ITEM_GET_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.ItemFieldIndex = m_nItemIndex;
	int nType = T_FC_ITEM_GET_ITEM;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}
