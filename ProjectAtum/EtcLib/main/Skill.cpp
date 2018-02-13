// Skill.cpp: implementation of the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Skill.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "Chat.h"
#include "AtumDatabase.h"
#include "SkillInfo.h"
#include "StoreData.h"
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "INFSkill.h"
#include "SceneData.h"
#include "EnemyData.h"
#include "SkillEffect.h"
#include "ClientParty.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "dxutil.h"
#include "INFInvenExtend.h"

#include "FreeWar.h"

// 2008-12-29 by bhsohn »ç¸Á ½Ã ¹öÇÁ ½ºÅ³ À¯Áö ½Ã½ºÅÛ
#define		TERM_TICK_MUST_SERVER_CHECK_SKILL_DEADSTAT_REATTACK_TIME		60000	// (1000(1ÃÊ)*60ÃÊ(1ºÐ))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill()
{
	m_bSmartSpeed = FALSE;

	// 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö
	m_bMonsterHold	= FALSE;	// Hold»óÅÂ?
	// end 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö

	m_nSkillPatternType = 0;
	m_nTargetIndex = 0;
	m_nCharacterUID = 0;
	m_bSkillTargetState = FALSE;
	m_nChargingShotState = SKILL_STATE_READY;
	m_pSecSkillInfo = NULL;
	m_pPriSkillInfo = NULL;
	m_bScan			= FALSE;
	//m_fScanRange	= 0.f;

	// 2007-04-05 by bhsohn ºòºÕ ¹ö±× Ã³¸®
	m_nPreSkillItemNum = 0;

	// 2007-04-19 by bhsohn ¼­Ä¡¾ÆÀÌ ¾ÆÀÌÅÛ Ãß°¡
	m_nScanState = 0;
	for(int nCnt = 0;nCnt < MAX_SCAN;nCnt++)
	{
		m_fScanRange[nCnt]	= 0.f;
		memset(&m_vScanPosition[nCnt], 0x00, sizeof(D3DXVECTOR3));
	}
	
}

CSkill::~CSkill()
{
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	while(itSkillInfo != m_mapSkill.end())
	{
		util::del(itSkillInfo->second);
		itSkillInfo++;
	}
	m_mapSkill.clear();

	itSkillInfo = m_mapEnemySkillInfo.begin();
	while(itSkillInfo != m_mapEnemySkillInfo.end())
	{
		util::del(itSkillInfo->second);
		itSkillInfo++;
	}
	m_mapEnemySkillInfo.clear();
	
}

CSkillInfo* CSkill::PutSkill(ITEM_SKILL* pItemSkill)
{
	CSkillInfo* pSkillInfo = FindItemSkill(pItemSkill->ItemNum);
	if(pSkillInfo)
	{// 2007-01-05 by dgwoo, ÀÌ¹Ì ½ºÅ³ÀÌ Á¸ÀçÇÏ´ÂÁö Ã¼Å©
		return pSkillInfo;
	}

	pSkillInfo = new CSkillInfo(pItemSkill);
	m_mapSkill[pItemSkill->ItemNum] = pSkillInfo;
	if(pSkillInfo->ItemInfo &&
		pSkillInfo->ItemInfo->SkillType == SKILLTYPE_PERMANENT)
	{	
		for(int i=0;i< SIZE_MAX_POS;i++)
		{
			CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
			if(	pItemSkill->ItemInfo->ReqItemKind == ITEMKIND_ALL_ITEM ||
				(it != g_pStoreData->m_mapItemWindowPosition.end() &&
				CAtumSJ::CheckReqItemKind(pItemSkill->ItemInfo->ReqItemKind,it->second->GetRealItemInfo()->Kind)) )
			{
				UseSkill(pSkillInfo);
				return pSkillInfo;
			}
		}
	}
	return pSkillInfo;
}

void CSkill::DeleteSkill(LONGLONG UniqueNumber)
{
	FLOG( "CSkill::DeleteSkill(LONGLONG UniqueNumber)" );
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	for(; itSkillInfo != m_mapSkill.end(); itSkillInfo++)
	{
		if(itSkillInfo->second->UniqueNumber == UniqueNumber)
		{
			ReleaseSkill(itSkillInfo->second);

			// 2007-01-05 by dgwoo, ½ºÅ³ ±¸ÀÔ½Ã ÀÌ±â ¶§¹®¿¡ ¸ðµç ½ºÅ³À» Ãë¼Ò ÇÑ´Ù
			CancelSkillByBaseNum(itSkillInfo->second->ItemNum, TRUE);

			util::del(itSkillInfo->second);
			m_mapSkill.erase(itSkillInfo);
			break;
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleseSkill()
/// \brief		À§Àå,ÀÎºñÁöºÒ ½ºÅ³ÀÌ ¹ßµ¿ÁÙÀÏ¶§ ÇØÁ¦½ÃÅ²´Ù.
/// \author		dgwoo
/// \date		2006-11-28 ~ 2006-11-28
/// \warning	
///
/// \param		int nType : Ãë¼ÒÇÒ ½ºÅ³
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleseSkill(int nType, INT nAttackSkillItemNum)
{
	if(nType == DES_SKILL_SCANNING)
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
			if(SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_CAMOUFLAGE ||
				SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_INVISIBLE )
			{
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK, nAttackSkillItemNum);
			}
			itvecSkillInfo++;
		}
	}
	else if(nType == DES_SKILL_CAMOUFLAGE)
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
			if(SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_CAMOUFLAGE)
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			itvecSkillInfo++;
		}
	}
	else if(nType == DES_SKILL_CANCELALL)
	{
// 2007-01-29 by dgwoo Á¤È­ ½ºÅ³ »ç¿ë½Ã Àç »ç¿ë½Ã°£±îÁö ÃÊ±âÈ­ ½ÃÅ²±â·Î º¯°æ.
		// ³»°¡ ¾´ ½ºÅ³.
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
//			if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
//			{
//				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//			}
			
			if((*itvecSkillInfo)->GetSkillState() <= SKILL_STATE_USING)
			{
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_USING);
				g_pShuttleChild->m_pSkillEffect->DeleteSkillEffect((*itvecSkillInfo)->ItemInfo->ItemNum);
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				// 2007-03-06 by dgwoo 10ºÐÀº Æ÷ÇÔÇÏÁö ¾ÊÀ½.

				// 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
// 				if((*itvecSkillInfo)->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME
// 					&& !(*itvecSkillInfo)->IsMustOneTargetSkill((*itvecSkillInfo)->ItemInfo->SkillTargetType))
				if( !COMPARE_BIT_FLAG((*itvecSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME)
					&& !(*itvecSkillInfo)->IsMustOneTargetSkill((*itvecSkillInfo)->ItemInfo->SkillTargetType))
				// end 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
					(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			}
			itvecSkillInfo++;
		}
		// ´Ù¸¥À¯Àú°¡ ¾´ ½ºÅ³.
		itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
		while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
		{
			// 2009. 08. 11 by ckPark Æí´ë ´ëÇü ½ºÅ³
			//if((*itvecSkillInfo)->GetSkillState() == SKILL_STATE_USING)
			if( (*itvecSkillInfo)->GetSkillState() == SKILL_STATE_USING
				&& !COMPARE_BIT_FLAG( (*itvecSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_PARTY_FORMATION_SKILL ) 
				&& !IS_INFUENCE_BUFF((*itvecSkillInfo)->ItemInfo)
#ifdef _OUTPOST_BUFFS
				&& !(*itvecSkillInfo)->ItemInfo->IsExistDesParam(DES_SKILLTYPE_OUTPOST_BUFF)
#endif
				) // 2013-05-28 by bhsohn ¼¼·Â¹öÇÁ°¡ [Á¤È­]¿¡ ´Ù ³¯¶ó°¡´Â Çö»ó Ã³¸®
			// end 2009. 08. 11 by ckPark Æí´ë ´ëÇü ½ºÅ³
			{
				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
				{
					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
				}
				// ´Ù¸¥ À¯Àú°¡ ¾´ ½ºÅ³À» »èÁ¦
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);

			}
			itvecSkillInfo++;
		}

// 2007-01-29 by dgwoo Á¤È­ ½ºÅ³ »ç¿ë½Ã Àç»ç¿ë½Ã°£À» À¯Áö.
//			if((*itvecSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
//			{
//				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
//				{
//					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//				}
//				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			}
//			itvecSkillInfo++;
//		}
//		// 2006-12-28 by dgwoo ´Ù¸¥ À¯Àú°¡ ¾´ ½ºÅ³µµ ÇÔ²² »èÁ¦ÇÑ´Ù.
//		itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
//		while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
//		{
//			if((*itvecSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
//			{
//				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
//				{
//					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//				}
//				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//
//			}
//			itvecSkillInfo++;
//		}
// 2007-02-01 by dgwoo ¹ö¼­Ä¿½ºÅ³À» »ç¿ëÇÏ¿©µµ ÇÁ·»Áö½ºÅ³À» Ãë¼Ò½ÃÅ°Áö ¾Ê´Â´Ù.
//	}else if(nType == IGEAR_SKILL_BASENUM_BERSERKER)
//	{
//		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
//		while(itvecSkillInfo != m_vecUsingSkill.end())
//		{
//			if(SKILL_BASE_NUM((*itvecSkillInfo)->ItemNum) == IGEAR_SKILL_BASENUM_FRENZY)
//				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			itvecSkillInfo++;
//		}
	}
	// 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö
	// ¸±¸®Áî½Ã µð¹öÇÁ¸¸ ÇØÁ¦
	else if( nType == DES_SKILL_RELEASE )
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
		while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
		{
			if( IS_ORBIT_SKILL( (*itvecSkillInfo)->ItemInfo->OrbitType )
				&& (*itvecSkillInfo)->GetSkillState() == SKILL_STATE_USING )
			{
				if((*itvecSkillInfo)->ItemInfo->AttackTime > 0)
				{
					g_pShuttleChild->m_pSkillEffect->DeleteCastingStateMonsterSkillEffect();
				}
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
				(*itvecSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			}
			itvecSkillInfo++;
		}
	}
	// end 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö
}

void CSkill::UseSkill(CSkillInfo* pItemSkill)
{
	FLOG( "CSkill::UseSkill(CSkillInfo* pItemSkill)" );
	if( pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT &&
		g_pD3dApp->m_dwGameState == _CITY )
	{
		// µµ½Ã¿¡¼­´Â ÆÐ½Ãºê ½ºÅ³À» Á¦¿ÜÇÏ°í ´Ù¸¥ ½ºÅ³Àº »ç¿ëÇÒ ¼ö ¾ø´Ù.
		return;
	}
	
	// 2004-11-26 by jschoi - ¿öÇÁÁß¿¡´Â ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø´Ù.
	if( g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN)
	{
		return;
	}

	if(pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{
		vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
		while(itvecSkillInfo != m_vecUsingSkill.end())
		{
			if(((*itvecSkillInfo)->ItemNum == pItemSkill->ItemNum) &&
				((*itvecSkillInfo)->GetAttackIndex() == g_pShuttleChild->m_myShuttleInfo.ClientIndex))
			{
				if((*itvecSkillInfo)->IsEnableStateForReUsing())
				{
					// ½Ã°£Çü ½ºÅ³ Àç»ç¿ë : ½Ã°£Çü ½ºÅ³ÀÇ °æ¿ì ReattackTimeÀÌ Áö³ª¸é Àç»ç¿ëÀÌ °¡´ÉÇÏ´Ù.
					//(*itvecSkillInfo)->ReUsingSkill();
					// 2012-08-09 by mspark, ½Ã°£Çü ½ºÅ³ ¹ßµ¿ ½Ã°£ÀÌ 10ÃÊ ÀÌÇÏ·Î ³²¾ÆÀÖÀ» °æ¿ì ÀÎºñÁöºí »óÅÂ¿¡¼­µµ Àç¹ßµ¿µÇ´ø ¹ö±× Àç¹ßµ¿ ¾ÈµÇµµ·Ï ¼öÁ¤
					if(!IsUseSkill(pItemSkill))
					{
					(*itvecSkillInfo)->ReUsingSkill();
					}
					// end 2012-08-09 by mspark, ½Ã°£Çü ½ºÅ³ ¹ßµ¿ ½Ã°£ÀÌ 10ÃÊ ÀÌÇÏ·Î ³²¾ÆÀÖÀ» °æ¿ì ÀÎºñÁöºí »óÅÂ¿¡¼­µµ Àç¹ßµ¿µÇ´ø ¹ö±× Àç¹ßµ¿ ¾ÈµÇµµ·Ï ¼öÁ¤
					// 2006-11-28 by dgwoo ÇöÀç À§Àå½ºÅ³À» ½ºÅ³À» »ç¿ëÁßÀÌ¸é ½ºÅ³À» Ç¬´Ù.
					if(g_pShuttleChild->GetUnitAlphaState())
						ReleseSkill(DES_SKILL_CAMOUFLAGE);
				}
				else
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0008,COLOR_SKILL_CANCEL);//"Àç¹ßµ¿ ½Ã°£ÀÌ ³²¾Ò½À´Ï´Ù."
				}
				return;
			}

			itvecSkillInfo++;
		}
	}

	// 2005-11-26 by ispark, ½ºÅ³ÀÌ »ç¿ë °¡´ÉÇÑ°¡?
	if(IsUseSkill(pItemSkill))
	{
		return;
	}
	// 2006-12-04 by dgwoo À§Àå½Ã ½ºÅ³À» »ç¿ëÇÏ¸é ÀÚµ¿À¸·Î Ç®¸°´Ù.
	if(g_pShuttleChild->GetUnitAlphaState())
		ReleseSkill(DES_SKILL_CAMOUFLAGE);
	// ÇöÀç µ¿ÀÏÇÑ ½ºÅ³ÀÌ »ç¿ëÁßÀÌ ¾Æ´Ï¶ó¸é
	map<int,CSkillInfo*>::iterator itmapSkillInfo = m_mapSkill.find(pItemSkill->ItemNum);
	if(itmapSkillInfo != m_mapSkill.end())
	{
		// icon
		if(NULL == g_pShuttleChild->m_pSkill->FindUsingSkillInfo(itmapSkillInfo->second->ItemNum))
		{
			// 2007-04-05 by bhsohn ºòºÕ ¹ö±× Ã³¸®
			m_nPreSkillItemNum = pItemSkill->ItemNum;
			//DBGOUT("CSkill::UseSkill [%d]\n", pItemSkill->ItemNum);

			itmapSkillInfo->second->Init();
			
			if(itmapSkillInfo->second->ItemInfo->AttackTime > 0)
			{// 2007-01-06 by dgwoo AttackTime ÀÖ´Â°Í¸¸ ÇöÀç »ç¿ëÇÏ´Â vector¿¡ ¹Ì¸® µî·ÏÇÑ´Ù.
				g_pGameMain->m_pInfSkill->InsertSkillInfo(itmapSkillInfo->second);
				
			}
			// 2009-02-17 by bhsohn ½ºÄµ ¹ö±× ¼öÁ¤
			itmapSkillInfo->second->SetSkillEnable(TRUE);
			// end 2009-02-17 by bhsohn ½ºÄµ ¹ö±× ¼öÁ¤
			m_vecUsingSkill.push_back(itmapSkillInfo->second);
			if(itmapSkillInfo->second->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_SKILL_0001,COLOR_SKILL_USE,CHAT_TAB_SYSTEM);	//"½ºÅ³À» ½ÃÀüÇÕ´Ï´Ù."
			}			

			// 2008-03-04 by bhsohn ½ÃÁî¸ðµåÈÄ °ÔÀÓÁ¾·á¸é ¸ØÃß´Â Çö»óÃ³¸®
			if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind)
				&&(SKILL_BASE_NUM(itmapSkillInfo->second->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE 
					||SKILL_BASE_NUM(itmapSkillInfo->second->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE 
					||SKILL_BASE_NUM(itmapSkillInfo->second->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE) )
			{
				// ±â¼ú ½ºÀü½Ã, Ã¢À» ´Ý´Â´Ù.
				g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);				
				g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
				
				// 2010-10-14 by jskim ·Ñ¸µ°ú ¿¡¾î½ÃÁî °°ÀÌ »ç¿ëÇßÀ»½Ã Å¸°Ù ¾ÈÀâÈ÷´Â ¹ö±× ¼öÁ¤
				if( ( g_pShuttleChild->IsMyShuttleRolling() ) )
				{
					g_pShuttleChild->InitRolling();
					g_pShuttleChild->m_bMouseMoveLock = FALSE;				// 2005-08-05 by ispark		
					g_pShuttleChild->m_bIsCameraPattern = FALSE;
				}
				// end 2010-10-14 by jskim ·Ñ¸µ°ú ¿¡¾î½ÃÁî °°ÀÌ »ç¿ëÇßÀ»½Ã Å¸°Ù ¾ÈÀâÈ÷´Â ¹ö±× ¼öÁ¤
			}
			// end 2008-03-04 by bhsohn ½ÃÁî¸ðµåÈÄ °ÔÀÓÁ¾·á¸é ¸ØÃß´Â Çö»óÃ³¸®
			
			if(itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
				itmapSkillInfo->second->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
			{ // Å¸ÄÏÀ» Å¬¸¯ÇØ¾ßÇÏ´Â ÀÖ´Â ½ºÅ³ Å¸ÀÔ
				SetPrimarySkill(itmapSkillInfo->second);
				SetSkillTargetState(TRUE);	// ½ºÅ³ ¸¶¿ì½º Å¸ÀÔ ¼¼ÆÃ
			}
		}
	}
	else
	{
		DBGOUT("Use Skill : Can't Find Skill Information .\n");
	}
}

void CSkill::ReleaseSkill(CSkillInfo* pItemSkill)
{
	FLOG( "CSkill::ReleaseSkill(CSkillInfo* pItemSkill)" );

	CSkillInfo *pSkillInfo = FindUsingSkillInfo(pItemSkill->ItemNum);
	if(NULL == pSkillInfo)
	{
		return;
	}

	pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
}

void CSkill::Tick(float fElaspedTime)
{
	// 2007-02-13 by dgwoo ½ºÅ³Àº ½ÇÁ¦ ½Ã°£À¸·Î °è»ê.
	fElaspedTime = g_pD3dApp->GetCheckElapsedTime();
	if(GetSkillPatternType() != 0 && g_pShuttleChild->m_bSkillMoveIsUse == FALSE)	
	{// ½ºÅ³ ÆÐÅÏÀ» ÇØÁ¦ÇØ Áà¾ßÇÔ
		CSkillInfo* pSkillInfo = FindUsingSkillInfoByBaseNum(GetSkillPatternType());
		if(	pSkillInfo &&
			SKILL_KIND(pSkillInfo->ItemNum) == SKILL_KIND_CONTROL &&
			pSkillInfo->ItemInfo->SkillType == SKILLTYPE_CLICK )	// ÆÐÅÏÀÌ ÀÖ´Â ½ºÅ³ÀÌ´Ù.(SKILL_KIND_CONTROL ÀÌ°í, Å¬¸¯ÇüÀÌ´Ù. == ÆÐÅÏÀÌ ÀÖ´Â ½ºÅ³ÀÌ´Ù.)
		{
			pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
		}
	}
	
	vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
	while(itvecSkillInfo != m_vecUsingSkill.end())
	{
		if((*itvecSkillInfo)->GetSkillState() == SKILL_STATE_RELEASE)
		{
			// 2007-01-29 by dgwoo Á¤È­½ºÅ³ Àç»ç¿ë½Ã°£±îÁö ÃÊ±âÈ­ÇÑ´Ù.
			(*itvecSkillInfo)->SetInitReattackTime();
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itvecSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
			SafeDeleteSkillInfo(*itvecSkillInfo);
			itvecSkillInfo = m_vecUsingSkill.erase(itvecSkillInfo);
		}
		else
		{
			(*itvecSkillInfo)->Tick(fElaspedTime);
			itvecSkillInfo++;
		}
	}
	itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if((*itvecSkillInfo)->GetSkillState() == SKILL_STATE_RELEASE)
		{
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itvecSkillInfo);
			SafeDeleteSkillInfo(*itvecSkillInfo);
			itvecSkillInfo = m_vecEnemyUsingSkillForMe.erase(itvecSkillInfo);
		}
		else
		{
			(*itvecSkillInfo)->Tick(fElaspedTime);
			itvecSkillInfo++;
		}
	}
}

char* CSkill::GetSkillName(int nSkillItemNumber)
{
	FLOG( "CSkill::GetSkillName(int nSkillItemNumber)" );
	ITEM* pItem = g_pDatabase->GetServerItemInfo( nSkillItemNumber );
	if(pItem == NULL)
	{
		return NULL;
	}
	return pItem->ItemName;
}

int	CSkill::FindSkillItemNum(LONGLONG UniqueNumber)
{
	FLOG( "CSkill::FindSkillItemNum(LONGLONG UniqueNumber)" );
	int nSkillItemNum = 0;
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();	
	while(itSkillInfo != m_mapSkill.end())
	{
		if(itSkillInfo->second->UniqueNumber == UniqueNumber)
		{
			nSkillItemNum = itSkillInfo->second->ItemNum;
			break;
		}
		itSkillInfo++;
	}
	return nSkillItemNum;
}

ITEM* CSkill::FindItem(int nSkillItemNumber)
{
	FLOG( "CSkill::FindItem(int nSkillItemNumber)" );
	return g_pDatabase->GetServerItemInfo(nSkillItemNumber);
}

CSkillInfo* CSkill::FindItemSkill(int nSkillItemNumber)
{
	FLOG( "CSkill::FindItemSkill(int nSkillItemNumber)" );
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.find(nSkillItemNumber);
	if(itSkillInfo != m_mapSkill.end())
	{	
		return itSkillInfo->second;
	}
	return NULL;
}

int	CSkill::FindItemSkillLevelByBaseNum(int nSkillBaseNum)
{
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	while(itSkillInfo != m_mapSkill.end())
	{	
		if(SKILL_BASE_NUM(itSkillInfo->second->ItemNum) == nSkillBaseNum)
		{
			return SKILL_LEVEL(itSkillInfo->second->ItemNum);
		}
		itSkillInfo++;
	}
	return NULL;
}

CSkillInfo* CSkill::FindUsingSkillInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;
}

CSkillInfo* CSkill::FindEnemyUsingSkillForMeInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;

}

CSkillInfo* CSkill::FindEnemyUsingSkillForMeInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;

}

CSkillInfo* CSkill::FindUsingSkillInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			if((*itSkillInfo)->GetSkillState() <= SKILL_STATE_USING)
				return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillInfo* CSkill::FindUsingSkillExistInfoByBaseNum(int nSkillItemNum)
/// \brief		m_vecUsingSkill¿¡ Á¸ÀçÇÏ´Â °ÍÀ» Ã£¾ÆÁØ´Ù.(BaseNum)
/// \author		dgwoo
/// \date		2007-01-15 ~ 2007-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSkillInfo* CSkill::FindUsingSkillExistInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			return (*itSkillInfo);
		}
		itSkillInfo++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			IsExistSingleSkill()
/// \brief		ÇöÀç »ç¿ëÁßÀÎ ½ºÅ³Áß ´Üµ¶À¸·Î¸¸ »ç¿ë°¡´ÉÇÑ ½ºÅ³À» »ç¿ëÁßÀÎ°¡?
/// \author		jschoi
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	½ÃÁî ¸ðµå,±×¶ó¿îµå Æø°Ý, ¸ðµç ÄÁÆ®·Ñ ½ºÅ³°ú Â÷Â¡¼¦ Å¸ÀÔ ½ºÅ³Àº ´Üµ¶À¸·Î¸¸ »ç¿ëÇÑ´Ù.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsExistSingleSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(	SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE ||
			(SKILL_KIND((*itSkillInfo)->ItemNum) == SKILL_KIND_CONTROL && (*itSkillInfo)->GetSkillState() <= SKILL_STATE_USING))
		{
			char strMessage[256];
			wsprintf(strMessage,STRERR_C_SKILL_0009,//"[%s] ½ºÅ³°ú µ¿½Ã¿¡ »ç¿ë ÇÒ ¼ö ¾ø´Â ½ºÅ³ÀÔ´Ï´Ù."
				g_pShuttleChild->m_pSkill->GetSkillName((*itSkillInfo)->ItemInfo->ItemNum));
			g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_CANCEL);
			return TRUE;
			
		}
		itSkillInfo++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			IsSingleSkill(int nSkillItemNum)
/// \brief		nSkillItemNumÀÇ ½ºÅ³ÀÌ ´Üµ¶À¸·Î »ç¿ëÇÏ´Â ½ºÅ³ÀÎ°¡?
/// \author		jschoi
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	½ÃÁî ¸ðµå,±×¶ó¿îµå Æø°Ý, ¸ðµç ÄÁÆ®·Ñ ½ºÅ³°ú Â÷Â¡¼¦ Å¸ÀÔ ½ºÅ³Àº ´Üµ¶À¸·Î¸¸ »ç¿ëÇÑ´Ù.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSingleSkill(int nSkillItemNum)
{
	CSkillInfo* pItemSkill = FindItemSkill(nSkillItemNum);
	if( pItemSkill == NULL )
	{
		return FALSE;
	}
	if(	
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE)
//		||SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL)// 2007-06-15 by dgwoo Åä±Û½ºÅ³»ç¿ëÁß ÄÁÆ®·Ñ½ºÅ³»ç¿ë°¡´É.
	{
		return TRUE;
	}
	return FALSE;
}

void CSkill::DeleteSkillFromWearItem(BYTE nItemKind)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		UINT fReAttackTime = (*itSkillInfo)->ItemInfo->ReAttacktime;
		
		// 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
// 		if( (*itSkillInfo)->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
// 			CAtumSJ::CheckReqItemKind((*itSkillInfo)->ItemInfo->ReqItemKind, nItemKind) &&
// 			fReAttackTime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)	// 2006-12-20 by ispark, Æ¯Á¤ ½Ã°£ ÀÌ»óÀº Áö¿ìÁö ¾Ê´Â´Ù.
		if( (*itSkillInfo)->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
			CAtumSJ::CheckReqItemKind((*itSkillInfo)->ItemInfo->ReqItemKind, nItemKind) &&
			!COMPARE_BIT_FLAG((*itSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))	// 2006-12-20 by ispark, Æ¯Á¤ ½Ã°£ ÀÌ»óÀº Áö¿ìÁö ¾Ê´Â´Ù.
		// end 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pShuttleChild->m_pSkillEffect->DeleteSkillEffect((*itSkillInfo)->ItemInfo->ItemNum);
			// 2006-12-21 by dgwoo ¹«±â ÇØÁ¦·Î ÀÎÇÑ ½ºÅ³Àº reattacktimeÀ» 0À¸·Î ¼ÂÆÃÇÑ´Ù.
			(*itSkillInfo)->SetInitReattackTime();
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}

void CSkill::PutSkillFromWearItem(BYTE nItemKind)
{
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapSkill.begin();
	while(itSkillInfo != m_mapSkill.end())
	{
		if(	itSkillInfo->second->ItemInfo->SkillType == SKILLTYPE_PERMANENT &&
			(nItemKind == ITEMKIND_ALL_ITEM ||
			CAtumSJ::CheckReqItemKind(itSkillInfo->second->ItemInfo->ReqItemKind, nItemKind) ) )
		{
			UseSkill(itSkillInfo->second);
		}
		itSkillInfo++;
	}
}

BOOL CSkill::IsSatisfyReqItemKind(BYTE nReqItemKind, BYTE nItemKind)
{
	if(nReqItemKind == ITEMKIND_ALL_ITEM)
	{
		return TRUE;
	}
	
	switch(nReqItemKind)
	{
	case ITEMKIND_ALL_WEAPON:
		{
			return TRUE;
		}
		break;
	case ITEMKIND_PRIMARY_WEAPON_ALL:
		{
			if(IS_PRIMARY_WEAPON(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_PRIMARY_WEAPON_1:
		{
			if(IS_PRIMARY_WEAPON_1(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_PRIMARY_WEAPON_2:
		{
			if(IS_PRIMARY_WEAPON_2(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_SECONDARY_WEAPON_ALL:
		{
			if(IS_SECONDARY_WEAPON(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_SECONDARY_WEAPON_1:
		{
			if(IS_SECONDARY_WEAPON_1(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	case ITEMKIND_SECONDARY_WEAPON_2:
		{
			if(IS_SECONDARY_WEAPON_2(nItemKind))
				return TRUE;
			else
				return FALSE;
		}
		break;
	default:
		{
			if(nReqItemKind == nItemKind)
				return TRUE;
			else
				return FALSE;
		}
		break;
	}
	return FALSE;
}

void CSkill::CancelSkill(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel)
{
	CSkillInfo *pSkillInfo = FindUsingSkillInfo(nSkillItemNum);
	if(pSkillInfo)
	{
		if(0 < pSkillInfo->GetCheckEnableTime())
		{
			pSkillInfo->ChangeSkillState(SKILL_STATE_USING);
		}
		else
		{
			if(pSkillInfo->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
			}

			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
			DeleteUsingSkillInfo(nSkillItemNum);
		}
	}
	
	if(i_bEnemyUsingSkillCancel)
	{
		pSkillInfo = FindEnemyUsingSkillForMeInfo(nSkillItemNum);
		if(pSkillInfo)
		{
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
			DeleteEnemyUsingSkillInfo(nSkillItemNum);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::CancelSkillByBaseNum(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel)
/// \brief		½ºÅ³À» Ãë¼ÒÇÏ¸ç vecUsing¿¡¼­ »èÁ¦½ÃÅ²´Ù.
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::CancelSkillByBaseNum(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel)
{
	CSkillInfo *pSkillInfo = FindUsingSkillExistInfoByBaseNum(nSkillItemNum);
	if(pSkillInfo)
	{
// 2007-01-15 by dgwoo ½ºÅ³À» Ãë¼ÒÇÒ¶§´Â ±×³É »èÁ¦½ÃÅ²´Ù.
//		if(0 < pSkillInfo->GetCheckEnableTime())
//		{
//			pSkillInfo->ChangeSkillState(SKILL_STATE_USING);
//		}
//		else
//		{
//			if(pSkillInfo->ItemInfo->SkillType == SKILLTYPE_CHARGING)
//			{
//				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
//			}
//
//			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
//			DeleteUsingSkillInfoByBaseNum(nSkillItemNum);
//		}
		if(pSkillInfo->ItemInfo->AttackTime > 0)
		{
			g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
		}

		g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo, g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber);
		DeleteUsingSkillInfoByBaseNum(nSkillItemNum);
		
	}
	
	if(i_bEnemyUsingSkillCancel)
	{
		pSkillInfo = FindEnemyUsingSkillForMeInfoByBaseNum(nSkillItemNum);
		if(pSkillInfo)
		{
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
			DeleteEnemyUsingSkillInfoByBaseNum(nSkillItemNum);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleaseAllUsingSkillWithOutPermanent()
/// \brief		À¯Àú°¡ Á×¾úÀ» °æ¿ì ÆÄ¶ó¹ÌÅÍ ¹× ½ºÅ³ »èÁ¦.
/// \author		dgwoo
/// \date		2007-01-10 ~ 2007-01-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseAllUsingSkillWithOutPermanent()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			// 2007-04-12 by dgwoo reattact»óÅÂÀÏ °æ¿ì ´Ù½Ã reattactÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
			if((*itSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
			{
				(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			}
			// 2007-01-24 by dgwoo Ä³½ºÆÃ Å¸ÀÓÀÌ ÀÖ´Â ¸ðµç ÀÌÆåÆ®¸¦ »èÁ¦ÇÑ´Ù.
			//if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
			}


			// 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateMonsterSkillEffect();
			}
			// end 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö

	
			// 2006-12-15 by dgwoo, 10ºÐ ¹Ì¸¸ÀÏ°æ¿ì¸¸ ½ºÅ³ Àç»ç¿ë½Ã°£À» Áö¿î´Ù.
			// 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
			//if((*itSkillInfo)->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)				//5ºÐ ÀÌÇÏÀÏ °æ¿ì
			if(!COMPARE_BIT_FLAG((*itSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))				//5ºÐ ÀÌÇÏÀÏ °æ¿ì
			// end 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
			{
				(*itSkillInfo)->SetCheckEnableTime();
				(*itSkillInfo)->SetInitReattackTime();
				g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
				SafeDeleteSkillInfo(*itSkillInfo);
				itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
			}
			else
			{
				itSkillInfo++;
			}
		}
		else
		{
			itSkillInfo++;
		}
	}

	itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			//(*itSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleaseStatDEAD_AllUsingSkillWithOutPermanent()
/// \brief		À¯Àú°¡ Á×¾úÀ» °æ¿ì ÆÄ¶ó¹ÌÅÍ ¹× ½ºÅ³ »èÁ¦.(»ç¸Á ½Ã ¹öÇÁ ½ºÅ³ À¯Áö ½Ã½ºÅÛ)
/// \author		// 2008-12-29 by bhsohn »ç¸Á ½Ã ¹öÇÁ ½ºÅ³ À¯Áö ½Ã½ºÅÛ
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseStatDEAD_AllUsingSkillWithOutPermanent()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(IsReleaseSkill_StatDEAD((*itSkillInfo))
			&&(*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			// reattact»óÅÂÀÏ °æ¿ì ´Ù½Ã reattactÇÒ ÇÊ¿ä°¡ ¾ø´Ù.
			if((*itSkillInfo)->GetSkillState() != SKILL_STATE_WAIT_REATTACK)
			{
				(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			}
			// Ä³½ºÆÃ Å¸ÀÓÀÌ ÀÖ´Â ¸ðµç ÀÌÆåÆ®¸¦ »èÁ¦ÇÑ´Ù.
			//if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
			}


			// 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö
			if((*itSkillInfo)->ItemInfo->AttackTime > 0)
			{
				g_pShuttleChild->m_pSkillEffect->DeleteCastingStateMonsterSkillEffect();
			}
			// end 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö

			
			// 2006-12-15 by dgwoo, 10ºÐ ¹Ì¸¸ÀÏ°æ¿ì¸¸ ½ºÅ³ Àç»ç¿ë½Ã°£À» Áö¿î´Ù.
			// 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
			//if((*itSkillInfo)->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)				//5ºÐ ÀÌÇÏÀÏ °æ¿ì
			if(!COMPARE_BIT_FLAG((*itSkillInfo)->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))				//5ºÐ ÀÌÇÏÀÏ °æ¿ì
			// end 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
			{
				(*itSkillInfo)->SetCheckEnableTime();
				(*itSkillInfo)->SetInitReattackTime();
				g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
				SafeDeleteSkillInfo(*itSkillInfo);
				itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
			}
			else
			{
				itSkillInfo++;
			}
		}
		else
		{
			itSkillInfo++;
		}
	}
	
	itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		if(IsReleaseSkill_StatDEAD((*itSkillInfo))
			&& (*itSkillInfo)->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
		{
			//(*itSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsReleaseSkill_StatDEAD()
/// \brief		À¯Àú°¡ Á×¾úÀ» °æ¿ì Áö¿ï ½ºÅ³ÀÌ³Ä?
/// \author		// 2008-12-29 by bhsohn »ç¸Á ½Ã ¹öÇÁ ½ºÅ³ À¯Áö ½Ã½ºÅÛ
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsReleaseSkill_StatDEAD(CSkillInfo* i_pSkillInfo)
{
	if(NULL == i_pSkillInfo)
	{
		return FALSE;
	}


	// 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö

	// ¸ó½ºÅÍ ½ºÅ³ Á×¾úÀ»¶§ Ç®¸°´Ù
	if( i_pSkillInfo->ItemInfo->Kind == ITEMKIND_FOR_MON_SKILL )
		return TRUE;

	// end 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö


	switch(i_pSkillInfo->ItemInfo->SkillType)
	{
	case SKILLTYPE_PERMANENT:
		{
			return FALSE;
		}
	case SKILLTYPE_TIMELIMIT:	// ½Ã°£Çü ½ºÅ³Àº Áö¿ì¸é ¾ÈµÈ´Ù.
		{
			// ½ºÅ³ À¯Áö ½Ã°£ 1ºÐ ÀÌ»óÀÎ ½ºÅ³¸¸ Áö¿ìÁö ¾Ê´Â´Ù.(1ºÐ¹Ì¸¸¸¸ »èÁ¦)
			if(i_pSkillInfo->ItemInfo->Time >= TERM_TICK_MUST_SERVER_CHECK_SKILL_DEADSTAT_REATTACK_TIME)				//5ºÐ ÀÌÇÏÀÏ °æ¿ì
			{
				return FALSE;
			}
		}
		break;	
		
	}		
	return TRUE;
}


void CSkill::ReleaseAllUsingToggleSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_TOGGLE)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::ReleaseGroundSkill()
/// \brief		Ground ½ºÅ³ ÇØÁ¦
/// \author		dhkwon
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseGroundSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(	SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == FLASH_ACCELERATOR ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||				// 2006-05-29 by ispark
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_CAMOUFLAGE ||			// 2006-11-16 by dgwoo
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE)		// 2006-05-29 by ispark
		{
			ReleaseSkill(*itSkillInfo);
//			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);// 2005-11-26 by ispark µÎ¹ø Áßº¹
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
			continue;
		}
		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::ReleaseAllChargingSkill()
/// \brief		Â÷Â¡½ºÅ³ ÇØÁ¦
/// \author		jschoi
/// \date		2004-10-25 ~ 2004-10-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseAllChargingSkill()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		//if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
		if((*itSkillInfo)->ItemInfo->AttackTime > 0)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			g_pShuttleChild->m_pSkillEffect->DeleteCastingStateSkillEffect();
		}
		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SendPrepareUseSkill()
/// \brief		¼­¹ö·Î ½ºÅ³ÀÌ ¹ßµ¿µÊÀ» ¾Ë·ÁÁØ´Ù
/// \author		jschoi
/// \date		2004-10-05 ~ 2004-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SendPrepareUseSkill(CSkillInfo* pSkillInfo)
{
	MSG_FC_SKILL_PREPARE_USE sMsg;
	sMsg.SkillItemID.ItemNum = pSkillInfo->ItemNum;
	sMsg.SkillItemID.ItemUID = pSkillInfo->UniqueNumber;
	g_pFieldWinSocket->SendMsg(T_FC_SKILL_PREPARE_USE, (char*)&sMsg, sizeof(sMsg) );

	g_pShuttleChild->m_myShuttleInfo.CurrentSP -= pSkillInfo->ItemInfo->ReqSP;
	char *strSkillName;
	char strMessage[256];
	strSkillName = GetSkillName(pSkillInfo->ItemNum);
	wsprintf(strMessage,STRMSG_C_SKILL_0002,strSkillName);	//"½ºÅ³ [%s] À» ¹ßµ¿ÇÕ´Ï´Ù."
	g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_USE,CHAT_TAB_SYSTEM);	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SendPrepareCancelSkill()
/// \brief		¼­¹ö·Î ¹ßµ¿µÈ ½ºÅ³ÀÌ Ãë¼ÒµÊÀ» ¾Ë·ÁÁØ´Ù.
/// \author		jschoi
/// \date		2004-10-05 ~ 2004-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SendPrepareCancelSkill()
{
	char* strSkillName;
	char strMessage[256];
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CHARGING)
		//if((*itSkillInfo)->ItemInfo->AttackTime > 0)
		{
			MSG_FC_SKILL_CANCEL_PREPARE sMsg;
			sMsg.SkillItemID.ItemNum = (*itSkillInfo)->ItemNum;
			sMsg.SkillItemID.ItemUID = (*itSkillInfo)->UniqueNumber;
			g_pFieldWinSocket->SendMsg(T_FC_SKILL_CANCEL_PREPARE, (char*)&sMsg, sizeof(sMsg) );
			
			strSkillName = GetSkillName((*itSkillInfo)->ItemNum);
			wsprintf(strMessage,STRERR_C_SKILL_0010,strSkillName);//"½ºÅ³ [%s] ÀÇ ¹ßµ¿À» Ãë¼ÒÇÕ´Ï´Ù."	
			g_pD3dApp->m_pChat->CreateChatChild(strMessage,COLOR_SKILL_CANCEL);	
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);	
			continue;
		}
		itSkillInfo++;
	}
}

void CSkill::CheckChargingShotSkill()
{
//	UINT nSkillNum = 0;
	// Â÷Â¡¼¦
	switch(GetChargingShotState())
	{
	case SKILL_STATE_WAITING_PREPARE:
	case SKILL_STATE_PREPARE:
	case SKILL_STATE_WAITING:
		{
			SendPrepareCancelSkill();
			SetChargingShotState(SKILL_STATE_READY);
		}
		break;
// 2007-01-10 by dgwoo 
//	case SKILL_STATE_WAITING:
//		{
//			CSkillInfo* pSkillInfo = GetPrimarySkill();
//			if(pSkillInfo)
//			{
//				pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			}
//			else
//			{
//				SetChargingShotState(SKILL_STATE_READY);
//			}
//		}
//		break;
	case SKILL_STATE_USING:
		{
			CSkillInfo* pSkillInfo = GetPrimarySkill();
			if(pSkillInfo)
			{
				pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			}
			else
			{
				SetChargingShotState(SKILL_STATE_READY);
			}
		}
		break;
	case SKILL_STATE_READY:
		break;
	case SKILL_STATE_RELEASE:
		SetChargingShotState(SKILL_STATE_READY);
	case SKILL_STATE_WAIT_REATTACK:
	default:
		SetPrimarySkill(NULL);
		break;
	}
//	return nSkillNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UINT CSkill::GetChargingShotTypeSkillNum()
/// \brief		
/// \author		ispark
/// \date		2006-12-06 ~ 2006-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UINT CSkill::GetChargingShotTypeSkillNum()
{
	UINT nSkillNum = 0;

	switch(GetChargingShotState())
	{
	case SKILL_STATE_WAITING_PREPARE:
	case SKILL_STATE_PREPARE:
	case SKILL_STATE_WAITING:
		{
			SendPrepareCancelSkill();
		}
		break;
	case SKILL_STATE_USING:

		{
			CSkillInfo* pSkillInfo = GetPrimarySkill();
			if(pSkillInfo)
			{
				nSkillNum = pSkillInfo->ItemNum;
			}
			else
			{
				SetChargingShotState(SKILL_STATE_READY);
			}
		}
		break;
//	case SKILL_STATE_USING:
//		{
//			CSkillInfo* pSkillInfo = GetPrimarySkill();
//			if(pSkillInfo)
//			{
//				pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//			}
//			else
//			{
//				SetChargingShotState(SKILL_STATE_READY);
//			}
//		}
//		break;
	case SKILL_STATE_READY:
		break;
	case SKILL_STATE_WAIT_REATTACK:
	case SKILL_STATE_RELEASE:
	default:
		SetPrimarySkill(NULL);
		break;
	}

	return nSkillNum;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsExistTargetOneSkill(CSkillInfo *pItemSkill)
/// \brief		Å¸ÄÏÀ» Å¬¸¯ÇØ¾ß ¹ßµ¿ÇÏ´Â ½ºÅ³ÀÌ ¹ßµ¿ÁßÀÎ°¡?
/// \author		jschoi
/// \date		2004-10-14 ~ 2004-10-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsExistTargetOneSkill(CSkillInfo *pItemSkill)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		// 2006-10-26 by ispark, ½ºÅ³ Å¸°Ù Å¸ÀÔÀ» ½ºÅ³ Å¸ÀÔÀ¸·Î º¯°æ
		if(((*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS) &&
			//((*itSkillInfo)->ItemInfo->ItemNum == pItemSkill->ItemInfo->ItemNum) &&					// 2006-12-11 by ispark
//			((*itSkillInfo)->GetAttackIndex() == g_pShuttleChild->m_myShuttleInfo.ClientIndex ||
//			GetSkillTargetState() == TRUE))
			// 2006-12-27 by dgwoo À§¿¡ µÎÁÙÀº ¿Ö ±×·¸°Ô ÇßÀ»±î¿ä; 
			((*itSkillInfo)->GetSkillState() < SKILL_STATE_USING ))
		{
			return TRUE;
		}
		itSkillInfo++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindTargetForSkill()
/// \brief		¸¶¿ì½º Æ÷ÀÎÅÍ¸¦ ±âÁØÀ¸·Î °¡Àå °¡±î¿î ³» ÁÖº¯¿¡ UnitÀ» ±¸ÇÑ´Ù.(Å¬¸¯)
/// \author		jschoi
/// \date		2004-10-14 ~ 2004-10-14
/// \warning	
///
/// \param		
/// \return		TRUE : Å¸°ÙÀ» Àâ¾Ò´Ù. FALSE : Å¸°ÙÀ» ¸øÀâ¾Ò´Ù.	
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::FindTargetForSkill()
{
	float fLength = 50.0f;
	POINT pt;
	GetCursorPos(&pt);			
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	SetSkillTargetState(FALSE);		// ½ºÅ³ ¸¶¿ì½º Æ÷ÀÎÅÍ ÇØÁ¦
	
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt.x,pt.y);
	D3DXVECTOR2 vPos2;
	//////////////////////////////////////////////////////////////////////////
	// ¸ÕÀú ³»°¡ Å¸°ÙÀÎÁö °Ë»ç
	if(g_pShuttleChild->m_nObjScreenW > 0 && m_pPriSkillInfo)
	{
		vPos2 = D3DXVECTOR2(g_pShuttleChild->m_nObjScreenX, g_pShuttleChild->m_nObjScreenY);
		if(D3DXVec2Length(&(vPos1 - vPos2)) < fLength)
		{
			// 2005-12-05 by ispark, ÀÚ½Å¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³
			if(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME ||			// 2006-09-20 by ispark, µÎ ÁÙ Æ÷ÇÔ
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_INRANGE_WITHOUT_ME ||
				m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
			{
				// ÀÚ±â ÀÚ½Å Æ÷ÇÔ ¾È‰?
				char strmsg[256];
				wsprintf(strmsg, STRMSG_C_051205_0001, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ½ºÅ³Àº ÀÚ½Å¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³ÀÔ´Ï´Ù."
				g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
				ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
				return FALSE;
			}
			else
			{
				SetTargetIndex(g_pShuttleChild->m_myShuttleInfo.ClientIndex);
				return TRUE;	
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2005-11-24 by ispark
	// Æí´ë ¸®½ºÆ®¿¡¼­ Å¬¸¯
	BOOL bFindTarget = FALSE;
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	if(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		bFindTarget = FindListTargetForSkill(pt);
	}
	// Å¸°ÙÀ» ¸øÀâ¾ÒÀ¸¹Ç·Î ½ºÅ³À» ÇØÁ¦ÇÏ°í, ÃÊ±âÈ­ ÇÑ´Ù.
	// ¾ÆÁ÷ UseSkill»óÅÂµµ ¾Æ´Ï¹Ç·Î ±×³É Áö¿î´Ù.
	if(bFindTarget)
	{
		// 2010. 06. 18 by jskim ÄÝ¿Àºê È÷¾î·Î ½ÃÀü½Ã SP °¨¼ÒÇÏ´Â ¹ö±× ¼öÁ¤
		if(m_pPriSkillInfo->ItemInfo->AttackTime > 0 && 
			bFindTarget &&
			(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME		||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME  ||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME		||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_INRANGE_WITHOUT_ME	||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS))
		{
			m_pPriSkillInfo->ChangeSkillState(SKILL_STATE_WAITING_PREPARE);

			// 2010. 10. 11. µô·¹ÀÌ Å¸°Ù ½ÃÀüÇü ½ºÅ³ ¹ö±× ¼öÁ¤.
			m_pPriSkillInfo->SetClickTagetIndex ( g_pShuttleChild->m_pSkill->GetTargetIndex() );
			m_pPriSkillInfo->SetClickTargetUID ( g_pShuttleChild->m_pSkill->GetCharacterUID() );
			// End 2010. 10. 11. µô·¹ÀÌ Å¸°Ù ½ÃÀüÇü ½ºÅ³ ¹ö±× ¼öÁ¤.

		}
		//end 2010. 06. 18 by jskim ÄÝ¿Àºê È÷¾î·Î ½ÃÀü½Ã SP °¨¼ÒÇÏ´Â ¹ö±× ¼öÁ¤
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	// ÁÖº¯¿¡ Enemy°¡ Å¸°ÙÀÎÁö °Ë»ç
	CEnemyData * pTargetEnemy = NULL;
	map<INT,CEnemyData *>::iterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
// 2007-02-16 by dgwoo ¾Æ·¡¿Í °°Àº ¹æ½ÄÀ¸·Î ºñ±³¹®ÀÌ µé¾î°¥ ÇÊ¿ä°¡ ¾øÀ½.
//		if( itEnemy->second->m_nObjScreenW > 0 &&
//			(itEnemy->second->m_bySkillStateFlag == CL_SKILL_NONE ||					// 2006-12-12 by ispark, À§Àå, ÀÎºñÁöºíÀÌ ¾Æ´Ï°í
//			(itEnemy->second->m_bySkillStateFlag != CL_SKILL_NONE &&					// 2006-12-11 by ispark, À§Àå, ÀÎºñÁöºíÀÌ¶ó¸é
//			&& (IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType) || // 2006-12-12 by ispark, °°Àº ¼¼·Â ÀÌ°Å³ª
//			!(itEnemy->second->IsPkEnable())))))										// 2006-12-12 by ispark, PK°¡ ¾Æ´Ò¶§
		if(itEnemy->second->m_nObjScreenW > 0
			&& itEnemy->second->m_nAlphaValue != SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)	// 2007-02-16 by dgwoo Åõ¸í»óÅÂ°¡ ¾Æ´Ï¶ó¸é.
		{
			vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < fLength)
			{

				// 2011-03-28 by hsson, ÀÌ¹Ì Á×Àº »óÅÂÀÇ ±âÃ¼¿¡°Ô Èú¸µ Å¸°Ù ½áÁö´ø ¹ö±× ¼öÁ¤
				BodyCond_t tEnemyBodyCondition = itEnemy->second->m_pCharacterInfo->m_nCurrentBodyCondition;
				if( COMPARE_BODYCON_BIT( tEnemyBodyCondition, BODYCON_DEAD_MASK ) )
				{
					if( m_pPriSkillInfo )
						ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
					
					return FALSE;
				}
				// end 2011-03-28 by hsson, ÀÌ¹Ì Á×Àº »óÅÂÀÇ ±âÃ¼¿¡°Ô Èú¸µ Å¸°Ù ½áÁö´ø ¹ö±× ¼öÁ¤

				fLength = fLengthTemp;
				pTargetEnemy = itEnemy->second;
			}
		}
		itEnemy++;
	}

	//////////////////////////////////////////////////////////////////////////
	// Å¸°Ù Àâ¾Ò´Ù.
	if(pTargetEnemy && m_pPriSkillInfo)
	{ // Å¸°ÙÀ» Àâ¾Ò´Ù. ½ºÅ³À» »ç¿ëÇÑ´Ù.
		// 2006-11-30 by ispark, ¾Æ±º(ÀÏ¹Ý ¼¼·Â Æ÷ÇÔ)¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³
		if(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS
#ifdef FREEWAR_
			&& (IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTargetEnemy->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)
#else
			&&(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTargetEnemy->m_infoCharacter.CharacterInfo.InfluenceType)
#endif
			&& !(g_pShuttleChild->IsPkEnable() && g_pShuttleChild->GetPVPClientIndex() == pTargetEnemy->m_infoCharacter.CharacterInfo.ClientIndex)
			&& !(g_pShuttleChild->IsPkState(PK_FVF) && g_pShuttleChild->m_pClientParty->IsFVFEnemy(pTargetEnemy->m_infoCharacter.CharacterInfo.CharacterUniqueNumber))
			&& !(g_pShuttleChild->IsPkState(PK_GVG) && pTargetEnemy->m_infoCharacter.CharacterInfo.GuildUniqueNumber == g_pShuttleChild->m_pGuildWarInfo->PeerGuildUID)))

		{
			// ¾Æ±º Æ÷ÇÔ ¾È‰?
			char strmsg[256];
			wsprintf(strmsg, STRMSG_C_061130_0000, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ½ºÅ³Àº ÀÚ½Å ¼¼·Â¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³ÀÔ´Ï´Ù."
			g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
			ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
			return FALSE;
		}

		// 2005-12-05 by ispark, ÆÄÆ¼¿ø¿¡ ÇØ´çÇÏ´Â ½ºÅ³ Á¦¿Ü
		if(	m_pPriSkillInfo &&
			(m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			m_pPriSkillInfo->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME))
		{
			// Æí´ë¿øµé¿¡°Ô¸¸ ¾²´Â Å¸°Ù ½ºÅ³
			char strmsg[256];
			wsprintf(strmsg, STRMSG_C_051205_0002, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ½ºÅ³Àº ÀÚ½Å¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³ÀÔ´Ï´Ù."
			g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
			ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
			return FALSE;
		}

		CSkillInfo* pSkillInfo = NULL;
		if(pSkillInfo = CheckConfirmSkillUse())
		{
			// »ç¿ëÇã°¡°¡ ÇÊ¿äÇÑ ½ºÅ³ÀÌ´Ù.
			SendConfirmSkillUse(pSkillInfo, pTargetEnemy->m_infoCharacter.CharacterInfo.CharacterUniqueNumber);
			return TRUE;
		}

		SetTargetIndex(pTargetEnemy->m_infoCharacter.CharacterInfo.ClientIndex);
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Å¸°Ù ¸ø Àâ¾Ò´Ù.
	g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0011,COLOR_SKILL_CANCEL);//"Å¸°ÙÀ» ÀâÁö¸øÇÏ¿© ½ºÅ³À» Ãë¼ÒÇÕ´Ï´Ù."

	// 2007-05-22 by bhsohn Å¸ÄÏ °ü·Ã ¸Þ¸ð¸® ¹ö±× Ã³¸®
	//ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
	if(m_pPriSkillInfo)
	{
		ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
	}
	// 2007-05-23 by bhsohn ½ºÅ³ ¾È³ª°¡´Â ¹®Á¦ Ã³¸®
	else
	{
		InitTargetState();	// Å¸ÄÏÇü ½ºÅ³ÀÇ º¯¼öµéÀ» ÃÊ±âÈ­ ÇÑ´Ù.
	}
	
	return FALSE;
}

void CSkill::ReleaseAllPrepareTargetOneSkill()
{ // ÇöÀç ¹ßµ¿ÁßÀÎ TargetÀÌ ÀÖ´Â ½ºÅ³À» ¸ðµÎ ÇØÁ¦ÇÑ´Ù.
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if((*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			(*itSkillInfo)->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_RELEASE);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);
			itSkillInfo = m_vecUsingSkill.erase(itSkillInfo);
		}
		else
		{
			itSkillInfo++;
		}
	}	
	SetTargetIndex(0);				// ÃÊ±âÈ­
	SetCharacterUID(0);				// ÃÊ±âÈ­
	SetSkillTargetState(FALSE);		// ½ºÅ³ ¸¶¿ì½º Æ÷ÀÎÅÍ ÇØÁ¦
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleasePrepareTargetOneSkill(int nSkillNumber)
/// \brief		Å¸°ÙÇü ½ºÅ³À» »èÁ¦ÇÑ´Ù.
/// \author		dgwoo
/// \date		2007-01-06 ~ 2007-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleasePrepareTargetOneSkill(int nSkillNumber)
{ // ÇöÀç ¹ßµ¿ÁßÀÎ TargetÀÌ ÀÖ´Â ½ºÅ³À» ÇØÁ¦ÇÑ´Ù.
	CSkillInfo* pSkillInfo = FindUsingSkillInfo(nSkillNumber);
	if(pSkillInfo)
	{
		pSkillInfo->ChangeSkillState(SKILL_STATE_RELEASE);
		g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
		SafeDeleteSkillInfo(pSkillInfo);
		DeleteUsingSkillInfo(nSkillNumber);
	}
	
	// 2007-05-23 by bhsohn ½ºÅ³ ¾È³ª°¡´Â ¹®Á¦ Ã³¸®
//	SetTargetIndex(0);				// ÃÊ±âÈ­
//	SetCharacterUID(0);				// ÃÊ±âÈ­
//	SetSkillTargetState(FALSE);		// ½ºÅ³ ¸¶¿ì½º Æ÷ÀÎÅÍ ÇØÁ¦
	InitTargetState();	// Å¸ÄÏÇü ½ºÅ³ÀÇ º¯¼öµéÀ» ÃÊ±âÈ­ ÇÑ´Ù.
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::InitPrepareTargetOneSkill()
/// \brief		Å¸ÄÏÇü ½ºÅ³ÀÇ º¯¼öµéÀ» ÃÊ±âÈ­ ÇÑ´Ù.
/// \author		// 2007-05-23 by bhsohn ½ºÅ³ ¾È³ª°¡´Â ¹®Á¦ Ã³¸®
/// \date		2007-01-06 ~ 2007-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::InitTargetState()
{
	SetTargetIndex(0);				// ÃÊ±âÈ­
	SetCharacterUID(0);				// ÃÊ±âÈ­
	SetSkillTargetState(FALSE);		// ½ºÅ³ ¸¶¿ì½º Æ÷ÀÎÅÍ ÇØÁ¦
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SafeDeleteSkillInfo(CSkillInfo* pSkillInfo)
/// \brief		³»°¡ »ç¿ë°¡´ÉÇÑ ½ºÅ³¸®½ºÆ®¿¡ ÀÖ´Â ½ºÅ³Á¤º¸ÀÎ°¡ ºñ±³ÇÏ¿© ³»°¡ »ç¿ë°¡´ÉÇÑ ½ºÅ³¸®½ºÆ®°¡ ¾Æ´Ï¸é 
///				ÇØ´ç ½ºÅ³ Á¤º¸¸¦ ¿ÏÀüÈ÷ ClearÇÑ´Ù.
/// \author		jschoi
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SafeDeleteSkillInfo(CSkillInfo* pSkillInfo)
{// ³»°¡ »ç¿ë°¡´ÉÇÑ ½ºÅ³¸®½ºÆ®¿¡ ÀÖ´Â ½ºÅ³Á¤º¸ÀÎ°¡ ºñ±³ÇÏ¿© ³»°¡ »ç¿ë°¡´ÉÇÑ ½ºÅ³¸®½ºÆ®°¡ ¾Æ´Ï¸é ÇØ´ç ½ºÅ³ Á¤º¸¸¦ ¿ÏÀüÈ÷ ClearÇÑ´Ù.

	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapEnemySkillInfo.begin();
	for(; itSkillInfo != m_mapEnemySkillInfo.end(); itSkillInfo++)
	{
		if(pSkillInfo == itSkillInfo->second)
		{ // ÇØ´ç ½ºÅ³ÀÌ ÀÖ´Ù.

			util::del(pSkillInfo);
			m_mapEnemySkillInfo.erase(itSkillInfo);
			return;
		}
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::EraseUsingSkill(CSkillInfo* pSkillInfo)
/// \brief		½ºÅ³ Á¤º¸¸¦ m_vecUsingSkill¸®½ºÆ®¿¡¼­ Áï½Ã Áö¿î´Ù.
/// \author		jschoi
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		pSkillInfo »èÁ¦ÇÒ ½ºÅ³ÀÇ ÁÖ¼Ò. bType FALSE = ÀÚ½Å TRUE = ´Ù¸¥ À¯Àú°¡ ³ª¿¡°Ô.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::EraseUsingSkill(CSkillInfo* pSkillInfo,BOOL bEnemyDelete/* = FALSE*/)
{
	if(!bEnemyDelete)
	{
		vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
		while(itSkillInfo != m_vecUsingSkill.end())
		{
			if((*itSkillInfo)->ItemNum == pSkillInfo->ItemNum)
			{
				SafeDeleteSkillInfo(pSkillInfo);
				m_vecUsingSkill.erase(itSkillInfo);
				return;
			}
			itSkillInfo++;
		}
	}
	else
	{
		vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
		while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
		{
			if((*itSkillInfo)->ItemNum == pSkillInfo->ItemNum)
			{
				SafeDeleteSkillInfo(pSkillInfo);
				m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
				return;
			}
			itSkillInfo++;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		³» ¹öÇÁ º¸´Ù ³ôÀº ·¹º§ÀÇ ½ºÅ³ÀÌ µé¾î¿Â´Ù¸é ³» ¹öÇÁ¸¦ Áö¿î´Ù.
/// \author		// 2008-12-26 by bhsohn ¿¥±â¾î Æí´ë ¹ö±× ¼öÁ¤
/// \date		2004-10-15 ~ 2004-10-15
/// \warning	
///
/// \param		pSkillInfo »èÁ¦ÇÒ ½ºÅ³ÀÇ ÁÖ¼Ò. bType FALSE = ÀÚ½Å TRUE = ´Ù¸¥ À¯Àú°¡ ³ª¿¡°Ô.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::EraseLowLVUsingSkill(ClientIndex_t	i_AttackIndex, INT i_nSkillItemNum)
{
	if(i_AttackIndex == g_pShuttleChild->GetMyShuttleInfo().ClientIndex)
	{
		// ³»°¡ ¾´ ¹öÇÁ´Â Áö¿ï ÇÊ¿ä°¡¾ø´Ù.
		return;
	}
	INT nSkillItemNum = SKILL_BASE_NUM(i_nSkillItemNum);
	INT nSkillLevel = SKILL_LEVEL(i_nSkillItemNum);


	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		CSkillInfo* pSkillInfo = (*itSkillInfo);
		if(NULL == pSkillInfo)
		{			
			itSkillInfo++;
			continue;
		}
		INT nTmpSkillItemNum = SKILL_BASE_NUM(pSkillInfo->ItemNum);
		INT nTmpSkillLevel = SKILL_LEVEL(pSkillInfo->ItemNum);		
		
		if((nTmpSkillItemNum == nSkillItemNum)
			&&(nSkillLevel >= nTmpSkillLevel))
		{			
			pSkillInfo->SetSkillEnable(FALSE);
		}
		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindSkillEffect(CUnitData* pUnit, int nEffectNum)
/// \brief		ÇØ´ç ½ºÅ³ ÀÌÆåÆ®¸¦ ÇØÁ¦ÇÑ´Ù.
/// \author		jschoi
/// \date		2004-12-08 ~ 2004-12-08
/// \warning	g_pD3dApp->m_pEffectList¿¡¼­ ÇØ´ç ÀÌÆåÆ®¸¦ Ã£´Â´Ù.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::DisableSkillEffect(CUnitData* pUnit, int nEffectNum)
{
	CAppEffectData* pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
	while(pEffect)
	{
		if( pEffect->m_pParent == (CAtumNode*)pUnit &&
			pEffect->m_nType == nEffectNum )		
		{
			pEffect->m_bUsing = FALSE;
			break;
		}
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteUsingSkillInfo(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteUsingSkillInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	for(; itSkillInfo != m_vecUsingSkill.end(); itSkillInfo++)
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			m_vecUsingSkill.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteUsingSkillInfoByBaseNum(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteUsingSkillInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	for(; itSkillInfo != m_vecUsingSkill.end(); itSkillInfo++)
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			m_vecUsingSkill.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteEnemyUsingSkillInfo(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteEnemyUsingSkillInfo(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	for(; itSkillInfo != m_vecEnemyUsingSkillForMe.end(); itSkillInfo++)
	{
		if((*itSkillInfo)->ItemNum == nSkillItemNum)
		{
			m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::DeleteEnemyUsingSkillInfoByBaseNum(int nSkillItemNum)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::DeleteEnemyUsingSkillInfoByBaseNum(int nSkillItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	for(; itSkillInfo != m_vecEnemyUsingSkillForMe.end(); itSkillInfo++)
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			m_vecEnemyUsingSkillForMe.erase(itSkillInfo);
			return TRUE;
		}		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindListTargetForSkill(POINT pt)
/// \brief		Æí´ë ¸®½ºÆ®¿¡¼­ Ã£±â(Å¬¸¯Çü¸¸ °¡´É)
/// \author		ispark
/// \date		2005-11-24 ~ 2005-11-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::FindListTargetForSkill(POINT pt)
{
	// Å¬¸¯Çü¸¸ »ç¿ëÇÔ
	int i = 0;
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt.x,pt.y);
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		// ¸®½ºÆ® Å¬¸¯
		// 2008-07-10 by bhsohn Å¸ÄÏÈúÀÌ ¾ÈµÇ´Â ¹®Á¦ ÇØ°á
//		if(pt.x > 0 && pt.x < 130 &&
//			pt.y > 170 + 34 * i &&
//			pt.y < 204 + 34 * i)
		if(pt.x > 0 && pt.x < 130 &&
			pt.y > FRAME_START_Y + PARTY_FRAME_SIZE_Y * i &&
			pt.y < (FRAME_START_Y + PARTY_FRAME_SIZE_Y) + (PARTY_FRAME_SIZE_Y * i))
		{
			// °°Àº¸Ê¿¡ Á¸ÀçÇÔ
			if((*itParty)->m_bPartyType == _PARTYMASTER ||
				(*itParty)->m_bPartyType == _PARTYMEMBER)
			{
				CSkillInfo* pSkillInfo = NULL;
				if(pSkillInfo = CheckConfirmSkillUse())
				{
					// »ç¿ëÇã°¡°¡ ÇÊ¿äÇÑ ½ºÅ³ÀÌ´Ù.

					// 2008. 12. 23 by ckPark ½ºÅ³ »ç¿ë Áö¿¬ »ç¿ë Áö¿¬
					//SendConfirmSkillUse(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					g_pGameMain->PushDelaySkill(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					// end 2008. 12. 23 by ckPark ½ºÅ³ »ç¿ë Áö¿¬ »ç¿ë Áö¿¬



					return TRUE;
				}

				SetTargetIndex(0);
				SetCharacterUID((*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);

				return TRUE;
			}
			// ´Ù¸¥¸Ê¿¡ Á¸ÀçÇÔ
			else if((*itParty)->m_bPartyType == _PARTYOTHERMAPMASTER ||
				(*itParty)->m_bPartyType == _PARTYOTHERMAPMEMBER)
			{
				CSkillInfo* pSkillInfo = NULL;
				if(pSkillInfo = CheckConfirmSkillUse())
				{
					// »ç¿ëÇã°¡°¡ ÇÊ¿äÇÑ ½ºÅ³ÀÌ´Ù.



					// 2008. 12. 23 by ckPark ½ºÅ³ »ç¿ë Áö¿¬ »ç¿ë Áö¿¬
					//SendConfirmSkillUse(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					g_pGameMain->PushDelaySkill(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
					// end 2008. 12. 23 by ckPark ½ºÅ³ »ç¿ë Áö¿¬ »ç¿ë Áö¿¬



					return TRUE;
				}
				// 2010. 06. 18 by jskim ÄÝ¿Àºê È÷¾î·Î ½ÃÀü½Ã SP °¨¼ÒÇÏ´Â ¹ö±× ¼öÁ¤
				SetTargetIndex(0);
				SetCharacterUID((*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
				return TRUE;
				//end 2010. 06. 18 by jskim ÄÝ¿Àºê È÷¾î·Î ½ÃÀü½Ã SP °¨¼ÒÇÏ´Â ¹ö±× ¼öÁ¤
			}
			// ¾ÆÁ÷...
			else if((*itParty)->m_bPartyType == _PARTYRESTARTMEMBER)
			{

			}
		}
		else // Æí´ë¿ø ÀÏ¹Ý Å¸°Ù Å¬¸¯
		{
			// °°Àº¸Ê¿¡ Á¸ÀçÇÔ
			if((*itParty)->m_bPartyType == _PARTYMASTER ||
				(*itParty)->m_bPartyType == _PARTYMEMBER)
			{
			if((*itParty)->m_pEnemyData->m_nObjScreenW > 0)
			{
				float fLength = 50.0f;
				D3DXVECTOR2 vPos2 = D3DXVECTOR2((*itParty)->m_pEnemyData->m_nObjScreenX, (*itParty)->m_pEnemyData->m_nObjScreenY);
				float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
				if(fLengthTemp < fLength)
				{
					CSkillInfo* pSkillInfo = NULL;
					if(pSkillInfo = CheckConfirmSkillUse())
					{
						// »ç¿ëÇã°¡°¡ ÇÊ¿äÇÑ ½ºÅ³ÀÌ´Ù.



						// 2008. 12. 23 by ckPark ½ºÅ³ »ç¿ë Áö¿¬ »ç¿ë Áö¿¬
						//SendConfirmSkillUse(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
						g_pGameMain->PushDelaySkill(pSkillInfo, (*itParty)->m_ImPartyMemberInfo.CharacterUniqueNumber);
						// end 2008. 12. 23 by ckPark ½ºÅ³ »ç¿ë Áö¿¬ »ç¿ë Áö¿¬



						return TRUE;
					}
				}
				}
			}
			// ´Ù¸¥¸Ê¿¡ Á¸ÀçÇÔ
			else if((*itParty)->m_bPartyType == _PARTYOTHERMAPMASTER ||
				(*itParty)->m_bPartyType == _PARTYOTHERMAPMEMBER)
			{
				
			}
			// ¾ÆÁ÷...
			else if((*itParty)->m_bPartyType == _PARTYRESTARTMEMBER)
			{

			}
		}
		
		i++;
		itParty++;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::ReleaseAllUsingControlSkill(int i_nExcludeSkillNum)
/// \brief		ÄÁÆ®·Ñ ½ºÅ³ ¸ðµÎ »èÁ¦, m_vecUsingSkill¿¡¼­´Â Áö¿ìÁö ¾Ê´Â´Ù.
///				Áßº¹ »ç¿ë½Ã Àç¹ßµ¿ ½Ã°£ÀÌ Áß°£¿¡¼­ »èÁ¦µÈ´Ù.
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleaseAllUsingControlSkill(int i_nExcludeSkillNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{

		if((*itSkillInfo)->ItemNum != i_nExcludeSkillNum 
			&& (*itSkillInfo)->ItemInfo->SkillType == SKILLTYPE_CLICK
			&& SKILL_KIND((*itSkillInfo)->ItemNum) == SKILL_KIND_CONTROL)
		{
			(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
			SafeDeleteSkillInfo(*itSkillInfo);

			g_pShuttleChild->DeletePattern();				
			g_pShuttleChild->m_pSkill->SetSkillPatternType(0);	// ÆÐÅÏ ÃÊ±âÈ­
			// 2013-03-13 by bhsohn ½ºÅ³ ¹øÈ£ ±¸Á¶ ¼öÁ¤
//			g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_NUM((*itSkillInfo)->ItemInfo->SourceIndex));
			g_pShuttleChild->m_pSkill->DisableSkillEffect(g_pShuttleChild,SKILL_BASE_SOURCEINDEX((*itSkillInfo)->ItemInfo->SourceIndex));

		}

		itSkillInfo++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsExistMultyNGround()
/// \brief		¸ÖÆ¼Å¸°Ù°ú Áö»óÆø°ÝÀº °°ÀÌ ¾µ ¼ö ¾øÀ½. Á¸ÀçÇÏ´ÂÁö °Ë»ö
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsExistMultyNGround()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
			SKILL_BASE_NUM((*itSkillInfo)->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE ||
			(FindDestParamUseSkill((*itSkillInfo)->ItemInfo, DES_MULTITAGET_01) ||
			FindDestParamUseSkill((*itSkillInfo)->ItemInfo, DES_MULTITAGET_02)))
		{
			return TRUE;
		}

		itSkillInfo++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::FindDestParamUseSkill(ITEM *pItem, BYTE byDestParameter)
/// \brief		¾ÆÅÛ DestParameter¿¡¼­ ½ºÅ³ DestParameter¶û ºñ±³ÇØ¼­ °°Àº°ÍÀÌ Á¸ÀçÇÏ¸é BOOLÇü ¸®ÅÏ
/// \author		ispark
/// \date		2005-11-25 ~ 2005-11-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2013-03-27 by bhsohn DestParam ÀÚ·áÇü ¼öÁ¤
//BOOL CSkill::FindDestParamUseSkill(ITEM *pItem, BYTE byDestParameter)
BOOL CSkill::FindDestParamUseSkill(ITEM *pItem, DestParam_t byDestParameter)
{
	// 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
// 	if(pItem->DestParameter1 == byDestParameter ||
// 	   pItem->DestParameter2 == byDestParameter ||
// 	   pItem->DestParameter3 == byDestParameter ||
// 	   pItem->DestParameter4 == byDestParameter)
// 	{
// 		return TRUE;
// 	}
	int nArrParamCnt = 0;
	for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		if(pItem->ArrDestParameter[nArrParamCnt] == byDestParameter)
		{
			return TRUE;
		}
	}
	// end 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsGroundSkill(CSkillInfo *pItemSkill)
/// \brief		Áö»ó¿¡¼­ »ç¿ëÇÏ´Â ½ºÅ³ÀÌ³Ä?
/// \author		dgwoo
/// \date		2007-01-09 ~ 2007-01-09
/// \warning	
///
/// \param		
/// \return		TRUE Áö»ó¿ë ½ºÅ³.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsGroundSkill(CSkillInfo *pItemSkill)
{
	if((SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_GROUNDACCELERATOR ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == FLASH_ACCELERATOR ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE||
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_SKILL_BARRIER) ||
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_SKILL_CAMOUFLAGE)))
		return TRUE;
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsUseSkill(CSkillInfo *pItemSkill)
/// \brief		¾µ·Á´Â ½ºÅ³ÀÌ »ç¿ë °¡´ÉÇÑ°¡?
/// \author		ispark
/// \date		2005-11-26 ~ 2005-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsUseSkill(CSkillInfo *pItemSkill)
{
	// 2007-02-01 by dgwoo µµ½Ã ¸Ê¿¡¼± ½ºÅ³À» »ç¿ëÇÒ¼ö ¾ø´Ù.
	if(IS_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)
		&& !IS_BAZAAR_SKILL(pItemSkill->ItemInfo))
	{
		return TRUE;
	}
	// 2010. 06. 29 by jskim ±¸ÀÔ»óÁ¡ ¿­¶§ ÀÎº¥ ³²Àº °³¼ö Ã¼Å©
	if(g_pGameMain->m_pInven->GetInvenFreeSize() <= 0 && pItemSkill->ItemInfo->IsExistDesParam(DES_BAZAAR_BUY))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100629_0401,COLOR_ERROR);
		return TRUE;
	}
	//end 2010. 06. 29 by jskim ±¸ÀÔ»óÁ¡ ¿­¶§ ÀÎº¥ ³²Àº °³¼ö Ã¼Å©
	if(((g_pShuttleChild->m_dwState != _NORMAL || g_pShuttleChild->m_bUnitStop == TRUE || g_pShuttleChild->m_bLandedMove == TRUE || g_pShuttleChild->m_bPenaltyUnitStop == TRUE) || // 2005-11-25 by ispark Ä³¸¯ÅÍÀÏ ¶§¿Í À¯´ÖÁ¤Áö ½ºÅ³ ¾²Áö ¾Ê±â
		(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && g_pShuttleChild->m_bIsAir == FALSE)) &&
		pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL)	// ÆÐÅÏÀÌ ÀÖ´Â ½ºÅ³Àº m_dwState°¡ _NORMAL »óÅÂ¿¡¼­ »ç¿ë°¡´ÉÇÏ´Ù.
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0002,COLOR_SKILL_CANCEL);//"½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù."
		return TRUE;
	}

	// 2006-09-27 by ispark ÆÐÅÏ ÁßÀÌ¸é¼­ ÄÁÆ®·Ñ ½ºÅ³Àº »ç¿ë ÇÒ ¼ö ¾ø´Ù.
	if(g_pShuttleChild->m_bSkillMoveIsUse == TRUE && 
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0002,COLOR_SKILL_CANCEL);//"½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù."
		return TRUE;
	}

	if( pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ME &&
		pItemSkill->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
		g_pStoreData->IsWearItem( pItemSkill->ItemInfo->ReqItemKind ) == FALSE &&
		pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0003,COLOR_SKILL_CANCEL);//"½ºÅ³À» Àû¿ëÇÒ ¹«±â°¡ ÀåÂøµÇ¾îÀÖÁö ¾Ê½À´Ï´Ù."
		return TRUE;
	}

	if(	g_pShuttleChild->m_bIsAir == TRUE && IsGroundSkill(pItemSkill))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0004,COLOR_SKILL_CANCEL);//"Áö»ó¿¡¼­¸¸ »ç¿ë°¡´ÉÇÑ ½ºÅ³ÀÔ´Ï´Ù."
		return TRUE;
	}
	// 2006-11-16 by dgwoo Air Siege
	if(	!g_pShuttleChild->m_bIsAir&&
		(SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061206_0100,COLOR_SKILL_CANCEL);//"°øÁß¿¡¼­¸¸ »ç¿ë°¡´ÉÇÑ ½ºÅ³ÀÔ´Ï´Ù."
		return TRUE;
	}
	// 2006-11-27 by dgwoo Á¤Áö »óÅÂ¿¡¼­ »ç¿ëÇÒ¼ö ¾ø´Â ½ºÅ³µé
	// 2009-01-12 by bhsohn A±â¾î, À¯´Ö °íÁ¤ »óÅÂ¿¡¼­ ¿¡¾î ½ÃÁî½Ã °ø°Ý ¾ÈµÇ´Â ¹®Á¦ ÇØ°á
// 	if((SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE) &&
// 		g_pShuttleChild->m_bUnitStop == TRUE)
// 	{
// 		char chMsg[512] = {0,};
// 		wsprintf(chMsg,STRMSG_C_061206_0101, pItemSkill->ItemInfo->ItemName);
// 		g_pD3dApp->m_pChat->CreateChatChild(chMsg,COLOR_SKILL_CANCEL);
// 		return TRUE;
// 	}
	if(SKILL_BASE_NUM(pItemSkill->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE)
	{
		if((g_pShuttleChild->m_bUnitStop == TRUE)
			||(g_pD3dApp->m_bFixUnitDirMode))
		{
			char chMsg[512] = {0,};
			wsprintf(chMsg,STRMSG_C_061206_0101, pItemSkill->ItemInfo->ItemName);
			g_pD3dApp->m_pChat->CreateChatChild(chMsg,COLOR_SKILL_CANCEL);
			return TRUE;
		}
	}
	// end 2009-01-12 by bhsohn A±â¾î, À¯´Ö °íÁ¤ »óÅÂ¿¡¼­ ¿¡¾î ½ÃÁî½Ã °ø°Ý ¾ÈµÇ´Â ¹®Á¦ ÇØ°á

	// ÆÄÆ¼ ½Ã¿¡¸¸ »ç¿ë °¡´ÉÇÑ ½ºÅ³ (³ª¸¦ Á¦¿ÜÇÏ°í ´Ù¸¥ ÆÄÆ¼¿øÀÌ Á¸ÀçÇÒ ¶§ »ç¿ë °¡´É)
#ifdef FREEWAR_
	if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		if ((pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITH_ME ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME) &&
			//g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.size() == 0 )
			!g_pShuttleChild->m_pClientParty->IsParty())
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0005, COLOR_SKILL_CANCEL);//"Æí´ë½Ã¿¡¸¸ »ç¿ë°¡´ÉÇÑ ½ºÅ³ÀÔ´Ï´Ù."
			return TRUE;
		}
	}
#endif	
	if( IsSingleSkill(pItemSkill->ItemNum) && IsExistSingleSkill())
	{
		return TRUE;
	}
	
	// 2006-10-26 by ispark, Å¸°ÙÇü ½ºÅ³Àº ¿ÀÁ÷ ÇÏ³ª¸¸ ÀÛµ¿
	if((pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
			pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS) &&
		IsExistTargetOneSkill(pItemSkill))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0006,COLOR_SKILL_CANCEL);//"´Ù¸¥ Å¸ÄÏÇü ½ºÅ³ÀÌ ¹ßµ¿ÁßÀÔ´Ï´Ù."
		return TRUE;
	}
	// 2006-12-21 by dgwoo Å¸°ÙÇü ½ºÅ³°ú Â÷Â¡Çü ½ºÅ³À» µ¿½Ã¿¡ »ç¿ëÇÒ¼ö¾ø´Ù.
	if(((GetChargingShotState() <= SKILL_STATE_USING) &&
		(GetChargingShotState() > SKILL_STATE_READY)) &&
		pItemSkill->IsMustOneTargetSkill(pItemSkill->ItemInfo->SkillTargetType))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061221_0100,COLOR_SKILL_CANCEL);
		return TRUE;
	}
	
	if( pItemSkill->ItemInfo->ReqSP > g_pShuttleChild->m_myShuttleInfo.CurrentSP )
	{	
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0007,COLOR_SKILL_CANCEL);//"½ºÅ³Æ÷ÀÎÆ®°¡ ºÎÁ·ÇÕ´Ï´Ù."
		return TRUE;
	}

	// Æí´ëºñÇà ÀÌ¸é¼­ Æí´ë¿øÀÎ°æ¿ì¿¡´Â ÆÐÅÏ ½ºÅ³À» »ç¿ëÇÏ¸é Æí´ë´ëÇü¿¡¼­ Å»ÅðµÈ´Ù.
	if( pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL &&
		g_pShuttleChild->m_pClientParty &&
		g_pShuttleChild->m_pClientParty->IsFormationFlightMove())
	{
		g_pShuttleChild->SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
		g_pShuttleChild->m_pClientParty->FormationFlightClear();
	}
	// 2005-12-17 by ispark
	// Æí´ëÀåÀÌ¸é¼­ Æí´ëºñÇàÀÏ ¶§ ÆÐÅÏ ½ºÅ³ »ç¿ë½Ã Æí´ë´ëÇü ÀÚÀ¯ºñÇàÀ¸·Î º¯°æ
	else if(pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL &&
		g_pShuttleChild->m_pClientParty &&
		g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
		g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		// 2008-06-17 by bhsohn Æí´ë °ü·Ã Ã³¸®
		//g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(FLIGHT_FORM_NONE);
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}

	// 2005-11-25 by ispark
	// ¸ÖÆ¼Å¸°Ù°ú Áö»óÆø°ÝÀº °°ÀÌ »ç¿ë ¸øÇÏ°Ô ÇÔ
	if((SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE ||
		SKILL_BASE_NUM(pItemSkill->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE ||
		(FindDestParamUseSkill(pItemSkill->ItemInfo, DES_MULTITAGET_01) ||
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_MULTITAGET_02))) &&
		IsExistMultyNGround())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051125_0002,COLOR_SKILL_CANCEL);// "¸ÖÆ¼Å¸°Ù°ú Áö»óÆø°ÝÀº °°ÀÌ ¾µ ¼ö ¾ø½À´Ï´Ù."
		return TRUE;		
	}

	// 2006-07-28 by ispark
	// °³ÀÎ»óÁ¡ »ç¿ëÀÌ °¡´ÉÇÑ Áö¿ªÀÎÁö
// 2007-06-01 by dgwoo ¹ÙÀÚ¸¦¸¶À»¿¡¼­µµ »ç¿ë°¡´É ÇÏ¸ç ¿µ¿ªÀÇ Á¦ÇÑµµ Ç®µµ·Ï º¯°æ.
	if(!g_pD3dApp->m_bCharacter && 
		IS_BAZAAR_SKILL(pItemSkill->ItemInfo))
//		&& !g_pCharacterChild->GetbBazaarEvent())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060728_0002,COLOR_SKILL_CANCEL);// "ÀÌ°÷¿¡¼­´Â »óÁ¡À» °³¼³ ÇÒ ¼ö ¾ø½À´Ï´Ù."
		return TRUE;
	}

	// 2006-08-01 by ispark
	// Æí´ë ºñÇàÁßÀÌ¸é °³ÀÎ»óÁ¡¿¡¼­´Â ÀÚµ¿ Å»Åð
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY && 
		g_pD3dApp->m_bCharacter && 
		IS_BAZAAR_SKILL(pItemSkill->ItemInfo) &&
		g_pCharacterChild->GetbBazaarEvent())
	{
		g_pShuttleChild->m_pClientParty->ISendPartyLeave();
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0007, COLOR_SKILL_CANCEL);//"Æí´ë¿¡¼­ Å»Åð ÇÏ¿´½À´Ï´Ù."
	}

	// 2006-11-17 by ispark
	// ÀÌº¥Æ®¸Ê¿¡¼­´Â »ç¿ë ÇÒ ¼ö ¾ø´Â ½ºÅ³
	if(!IsUseEventMap(pItemSkill))
	{
		char chMsg[512] = {0,};
		wsprintf(chMsg, STRMSG_C_061117_0001, pItemSkill->ItemInfo->ItemName);	// "ÀÌº¥Æ®¸Ê¿¡¼­´Â %s ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."
		g_pD3dApp->m_pChat->CreateChatChild(chMsg, COLOR_SKILL_CANCEL);
		return TRUE;
	}
	if(!IsSkillPossibility())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061206_0102, COLOR_SKILL_CANCEL);	// "[ÀÎºñÁöºí]½ºÅ³ »ç¿ëÁß¿¡´Â ½ºÅ³ ¹ßµ¿À» ÇÒ¼ö ¾ø½À´Ï´Ù."
		return TRUE;
	}
// 2011-10110 by jhahn EP4 Ä§¹¬ ½ºÅ³ Ãß°¡
	if(!IsSkillPossibilitySlience())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061206_0102, COLOR_SKILL_CANCEL);	// "Ä§¹¬ ½ºÅ³ »ç¿ëÁß¿¡´Â ½ºÅ³ ¹ßµ¿À» ÇÒ¼ö ¾ø½À´Ï´Ù."
		return TRUE;
	}									 
//end 2011-10110 by jhahn EP4 Ä§¹¬ ½ºÅ³ Ãß°¡
// 2007-02-01 by dgwoo ÇÁ·»Áö½ºÅ³°ú ÆøÁÖ½ºÅ³ Áßº¹»ç¿ëÀÌ °¡´ÉÇÏ´Ù.
//	if(	FindUsingSkillInfoByBaseNum(IGEAR_SKILL_BASENUM_BERSERKER) && 
//		SKILL_BASE_NUM(pItemSkill->ItemNum) == IGEAR_SKILL_BASENUM_FRENZY)
//	{
//		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061212_0100, COLOR_SKILL_CANCEL);	//"ÇØ´ç½ºÅ³Àº ÇÁ·»Áî¿Í Áßº¹»ç¿ëÀÌ ºÒ°¡´ÉÇÕ´Ï´Ù."
//		return TRUE;
//	}

	// 2009. 08. 20 by ckPark Á×Àº »óÅÂ¿¡¼­ ¿¡¾î½ÃÁî ½ºÅ³ ½áÁö´ø ¹®Á¦
	if( g_pShuttleChild->m_dwState == _FALLING || g_pShuttleChild->m_dwState == _FALLEN)
	{
		g_pD3dApp->m_pChat->CreateChatChild( STRERR_C_SKILL_0002, COLOR_SKILL_CANCEL );	//"½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù."
		return TRUE;
	}
	// end 2009. 08. 20 by ckPark Á×Àº »óÅÂ¿¡¼­ ¿¡¾î½ÃÁî ½ºÅ³ ½áÁö´ø ¹®Á¦

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillPossibility()
/// \brief		½ºÅ³À» »ç¿ëÇÒ¼ö ÀÖ´Â°¡? ¿ä¼Òµé¿¡ ÀÇÇØ ½ºÅ³ »ç¿ëÀÇ À¯¹«¸¦ ÆÇ´Ü.
/// \author		dgwoo
/// \date		2006-11-28 ~ 2006-11-28
/// \warning	Ãß°¡ µÇ´Â ³»¿ëÀ» °è¼Ó »ðÀÔ.
///
/// \param		
/// \return		TRUE: ½ºÅ³»ç¿ë°¡´É FALSE : ½ºÅ³ »ç¿ëÀ» ¸øÇÔ.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillPossibility()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(BGEAR_SKILL_BASENUM_INVISIBLE == SKILL_BASE_NUM((*itSkillInfo)->ItemNum))
		{
			return FALSE;
		}
		itSkillInfo++;
	}	
	return TRUE;
}
// 2011-10110 by jhahn EP4 Ä§¹¬ ½ºÅ³ Ãß°¡
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillPossibilitySlience()
/// \brief		
/// \author		jhahn
/// \date		2011-11-28 ~ 2011-11-28
/// \warning	
///
/// \param		
/// \return		TRUE: ½ºÅ³»ç¿ë°¡´É FALSE : ½ºÅ³ »ç¿ëÀ» ¸øÇÔ.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillPossibilitySlience()
{
	if(g_pShuttleChild->GetSkillMissileWarning() == TRUE)
	{
		 return FALSE;
	}
	 return TRUE;
}										 
//end 2011-10110 by jhahn EP4 Ä§¹¬ ½ºÅ³ Ãß°¡
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillPossibility()
/// \brief		
/// \author		dgwoo
/// \date		2008-01-08 ~ 2008-01-08
/// \warning	
///
/// \param		
/// \return		TRUE: ½ºÅ³»ç¿ë°¡´É FALSE : ½ºÅ³ »ç¿ëÀ» ¸øÇÔ.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsAttackPossibility()
{
	// ÀÎºñÁöºí »óÅÂ´Â °ø°ÝÀÌ ºÒ°¡´ÉÇÑ »óÅÂ´Ù
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(BGEAR_SKILL_BASENUM_INVISIBLE == SKILL_BASE_NUM((*itSkillInfo)->ItemNum))
		{
			return FALSE;
		}
		itSkillInfo++;
	}	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::IsSkillOverBooster()
/// \brief		¿À¹öºÎ½ºÅÍ¸¦ ¾²´ÂÁö Ã£´Â´Ù. ÀÌÀ¯´Â Æ¯º°È÷ ÀÌÆåÆ®¸¦ È¿°ú¸¦ ³»±â À§ÇØ¼­
/// \author		ispark
/// \date		2005-11-29 ~ 2005-11-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillOverBooster()
{
	for (auto pSkillInfo : m_vecUsingSkill)
	{
		if(
			SKILL_KIND(pSkillInfo->ItemNum) == SKILL_KIND_CONTROL
			&& (pSkillInfo->ItemNum / 10) == 783203						  // ¿À¹öºÎ½ºÅÍ

// 2010-12-28 by hsson ¿À¹öºÎ½ºÅÍ ½ºÅ³ ¹ö±×
			&& pSkillInfo->GetSkillState() == SKILL_STATE_USING       // ½ºÅ³ÀÌ »ç¿ëÁßÀÌ¸é
// end 2010-12-28 by hsson ¿À¹öºÎ½ºÅÍ ½ºÅ³ ¹ö±×

			)
		{
			return TRUE;
		}
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::UseSkillConfirm(int i)
/// \brief		»ç¿ë Çã°¡ È®ÀÎ
/// \author		ispark
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::UseSkillConfirm(int i)
{
	MSG_FC_SKILL_CONFIRM_USE_ACK sMsg;
	if(i == 0)
	{
		sMsg.bYesOrNo = TRUE;				// ½ÂÀÎ
		sMsg.AttackCharacterUID = m_stSkillConfirm.AttackCharacterUID;		// ½ºÅ³ »ç¿ëÀÚ CharacterUID
		sMsg.TargetCharacterUID = m_stSkillConfirm.TargetCharacterUID;		// ½ºÅ³ Å¸°Ù CharacterUID
		sMsg.UsingSkillItemNum = m_stSkillConfirm.UsingSkillItemNum;		// »ç¿ë ½ºÅ³ ItemNum
	}
	else
	{
		sMsg.bYesOrNo = FALSE;				// °ÅºÎ
		sMsg.AttackCharacterUID = m_stSkillConfirm.AttackCharacterUID;		// ½ºÅ³ »ç¿ëÀÚ CharacterUID
		sMsg.TargetCharacterUID = m_stSkillConfirm.TargetCharacterUID;		// ½ºÅ³ Å¸°Ù CharacterUID
		sMsg.UsingSkillItemNum = m_stSkillConfirm.UsingSkillItemNum;		// »ç¿ë ½ºÅ³ ItemNum
	}

	// 2009. 04. 06 by ckPark ÄÝ¿ÀºêÈ÷¾î·Î½Ã ´Ù¸¥ Å¸°ÙÆÃ ½ºÅ³ ¸ø¾²´Â ¹®Á¦
	sMsg.SkillConfirmUseUID	= m_stSkillConfirm.SkillConfirmUseUID;
	// end 2009. 04. 06 by ckPark ÄÝ¿ÀºêÈ÷¾î·Î½Ã ´Ù¸¥ Å¸°ÙÆÃ ½ºÅ³ ¸ø¾²´Â ¹®Á¦

	g_pFieldWinSocket->SendMsg(T_FC_SKILL_CONFIRM_USE_ACK, (char*)&sMsg, sizeof(sMsg));
	memset(&m_stSkillConfirm, 0, sizeof(MSG_FC_SKILL_CONFIRM_USE));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SetSkillConfirmData(MSG_FC_SKILL_CONFIRM_USE *pMsg)
/// \brief		½ºÅ³ »ç¿ë ¿©ºÎ µ¥ÀÌÅ¸ ÀÓ½Ã ÀúÀå
/// \author		ispark
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SetSkillConfirmData(MSG_FC_SKILL_CONFIRM_USE *pMsg)
{
	memcpy(&m_stSkillConfirm, pMsg, sizeof(MSG_FC_SKILL_CONFIRM_USE));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::CheckConfirmSkillUse()
/// \brief		»ç¿ëÇã°¡°¡ ÇÊ¿äÇÑ ½ºÅ³ÀÎÁö Ã¼Å©, ±×·¸´Ù¸é µû·Î Ã³¸®
/// \author		ispark
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSkillInfo* CSkill::CheckConfirmSkillUse()
{
	vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecUsingSkill.begin();
	while(itvecSkillInfo != m_vecUsingSkill.end())
	{
		// 2006-10-26 by ispark
		// ÄÝ¿ÀºêÈ÷¾î·Î
//		if(FindDestParamUseSkill((*itvecSkillInfo)->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER) &&
//			(*itvecSkillInfo)->GetCheckAckWaitingTime() == 0)
		// 2007-12-11 by dgwoo ÄÝ¿Àºê È÷¾î·Î ½ºÅ³ÀÌ »ç¿ë ÇÒ¼ö ¾øÀ»¶§ ´Ù¸¥ Å¸°ÙÇü ½ºÅ³À» 
		if(FindDestParamUseSkill((*itvecSkillInfo)->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER) &&
			(*itvecSkillInfo)->GetSkillState() == SKILL_STATE_PREPARE)
		{
			//DbgOut("CheckConfirmSkillUse() %d\n",(*itvecSkillInfo)->GetSkillState());
			return (*itvecSkillInfo);
		}
		itvecSkillInfo++;
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillInfo* CSkill::FindItemEnemySkillInfo(int nSkillItemNumber)
/// \brief		
/// \author		dgwoo
/// \date		2007-01-05 ~ 2007-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSkillInfo* CSkill::FindItemEnemySkillInfo(int nSkillItemNumber)
{
	FLOG( "CSkill::FindItemSkill(int nSkillItemNumber)" );
	map<int, CSkillInfo*>::iterator itSkillInfo = m_mapEnemySkillInfo.find(nSkillItemNumber);
	if(itSkillInfo != m_mapEnemySkillInfo.end())
	{	
		if(nSkillItemNumber == itSkillInfo->second->ItemNum)
		{
			return itSkillInfo->second;
		}
	}
	return NULL;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkill::SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber)
/// \brief		»ç¿ëÇã°¡ ½ºÅ³ ¸Þ¼¼Áö º¸³»±â
/// \author		ispark
/// \date		2005-12-06 ~ 2005-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber)
{
	MSG_FC_SKILL_CONFIRM_USE sMsg;
	wsprintf(sMsg.szAttackCharacterName, "%s", g_pShuttleChild->m_myShuttleInfo.CharacterName);		// ½ºÅ³ »ç¿ëÀÚÀÇ CharacterName;
	sMsg.AttackCharacterUID = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;				// ½ºÅ³ »ç¿ëÀÚ CharacterUID
	sMsg.TargetCharacterUID = TargetCharacterUniqueNumber;											// ½ºÅ³ Å¸°Ù CharacterUID
	sMsg.UsingSkillItemNum = pSkillInfo->ItemInfo->ItemNum;											// »ç¿ë ½ºÅ³ ItemNum
	sMsg.MapChannelIndex = g_pShuttleChild->m_myShuttleInfo.MapChannelIndex;						// ½ºÅ³ »ç¿ëÀÚÀÇ MapChannelIndex
	g_pFieldWinSocket->SendMsg(T_FC_SKILL_CONFIRM_USE, (char*)&sMsg, sizeof(sMsg));
	pSkillInfo->SetCheckAckWaitingTime(12);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsUseEventMap(CSkillInfo* pItemSkill)
/// \brief		ÀÌº¥Æ® ¸Ê¿¡¼­ »ç¿ëÀ» ÇÒ ¼ö ÀÖ´Â ½ºÅ³ÀÎ°¡?
/// \author		ispark
/// \date		2006-11-17 ~ 2006-11-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsUseEventMap(CSkillInfo* pItemSkill)
{
	MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	
	// 2006-11-17 by ispark, ÀÌº¥Æ® ¸Ê¿¡¼­´Â Á¦¿Ü
	if(IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType) &&
		FindDestParamUseSkill(pItemSkill->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER))
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsCancelSkill(CSkillInfo* pSkillInfo)
/// \brief		Á¤È­ ½ºÅ³Áß¿¡ Ãë¼ÒµÉ ½ºÅ³ÀÎ°¡.
/// \author		dgwoo
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		Á¤È­ ½ºÅ³·Î ÀÎÇÑ Ãë¼ÒµÉ ½ºÅ³ÀÎÁö ¾Ë¾Æ¿Â´Ù.
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsCancelSkill(CSkillInfo* pSkillInfo)
{
	// 2007-01-29 by dgwoo 10ºÐ ¹Ì¸¸ÀÎ ½ºÅ³Àº Àç¹ßµ¿½Ã°£±îÁö ÃÊ±âÈ­ÇÑ´Ù.
	// 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
// 	if(pSkillInfo->GetSkillState() == SKILL_STATE_USING
// 		&& pSkillInfo->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)
	if(pSkillInfo->GetSkillState() == SKILL_STATE_USING
		&& !COMPARE_BIT_FLAG(pSkillInfo->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))
	// end 2009. 04. 06 by ckPark Àç»ç¿ë ½ºÅ³ ÀúÀå ¼Ó¼º Ãß°¡
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CSkill::GetPreSkillItemNum()
/// \brief		½ÃÀüÀ» ¿äÃ»ÇÏ´Â ÁßÀÎ ±â¼ú
/// \author		// 2007-04-05 by bhsohn ºòºÕ ¹ö±× Ã³¸®
/// \date		2007-04-05 ~ 2007-04-05
/// \warning	
///
/// \param		
/// \return		Á¤È­ ½ºÅ³·Î ÀÎÇÑ Ãë¼ÒµÉ ½ºÅ³ÀÎÁö ¾Ë¾Æ¿Â´Ù.
///////////////////////////////////////////////////////////////////////////////
INT	CSkill::GetPreSkillItemNum()
{	
	return m_nPreSkillItemNum;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::InitPreSkillItemNum()
/// \brief		±â¼úÁ¤º¸ ÃÊ±âÈ­
/// \author		// 2007-04-05 by bhsohn ºòºÕ ¹ö±× Ã³¸®
/// \date		2007-04-05 ~ 2007-04-05
/// \warning	
///
/// \param		
/// \return		Á¤È­ ½ºÅ³·Î ÀÎÇÑ Ãë¼ÒµÉ ½ºÅ³ÀÎÁö ¾Ë¾Æ¿Â´Ù.
///////////////////////////////////////////////////////////////////////////////
void CSkill::InitPreSkillItemNum()
{
	m_nPreSkillItemNum = 0;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::SetScanSkill(int nSkillIdx, BOOL bScan)
/// \brief		±â¼úÁ¤º¸ ÃÊ±âÈ­
/// \author		// 2007-04-19 by bhsohn ¼­Ä¡¾ÆÀÌ ¾ÆÀÌÅÛ Ãß°¡
/// \date		2007-04-19 ~ 2007-04-19
/// \warning	
///
/// \param		
/// \return		Á¤È­ ½ºÅ³·Î ÀÎÇÑ Ãë¼ÒµÉ ½ºÅ³ÀÎÁö ¾Ë¾Æ¿Â´Ù.
///////////////////////////////////////////////////////////////////////////////
void CSkill::SetScanSkill(int nIdx, BOOL bScan)
{
	if(nIdx >= MAX_SCAN )
	{
		return;
	}
	if(bScan)
	{
		m_nScanState |= (1<< nIdx);		
	}
	else
	{
		m_nScanState &= ~(1<< nIdx);
	}

	// ÇöÀç ½ºÄµµÈ ¹öÇÁ°¡ ÀÖ³Ä?
	if(m_nScanState)
	{
		m_bScan	= TRUE;
	}
	else
	{
		m_bScan	= FALSE;
	}
}

void CSkill::SetScanPosition(int nIdx, D3DXVECTOR3	vScanPosition)
{
	if(nIdx >= MAX_SCAN )
	{
		return;
	}
	m_vScanPosition[nIdx] = vScanPosition;
}

float CSkill::GetScanRange(int nIdx)								
{
	if(nIdx >= MAX_SCAN )
	{
		return 0;
	}
	return m_fScanRange[nIdx];
}
void CSkill::SetScanRange(int nIdx, float ScanRange)				
{
	if(nIdx >= MAX_SCAN )
	{
		return;
	}
	m_fScanRange[nIdx] = ScanRange;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkillInfo::PrepareSkillFromServer()
/// \brief		
/// \author		// 2007-04-19 by bhsohn ¼­Ä¡¾ÆÀÌ ¾ÆÀÌÅÛ Ãß°¡
/// \date		2007-04-20 ~ 2007-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsCanSeeInvisible(D3DXVECTOR3 vPos)
{
	BOOL bCanSeeInvisible = FALSE;
	if(FALSE == ISScanSkill())
	{
		// ½ºÄµ »óÅÂ°¡ ¾Æ´Ï¸é ¸ø º»´Ù. 
		return bCanSeeInvisible;
	}
	FLOAT fLength = 0;
	float fScanRange =0;
	for(int nIdx = 0;nIdx < MAX_SCAN;nIdx++)
	{
		fScanRange = GetScanRange(nIdx);
// 2012-11-29 by mspark, ¼­Ä¡¾ÆÀÌ À§Ä¡°¡ ÀÌµ¿ÇÏ´Â Ä³¸¯ÅÍÀÇ À§Ä¡¿Í µ¿ÀÏÇÏµµ·Ï ¼öÁ¤
#ifdef C_SEARCHEYE_SHUTTLE_POSITION_MSPARK
		if(SCAN_ITEM == nIdx || SCAN_ITEM2 == nIdx)
		{
			fLength = D3DXVec3Length(&(g_pShuttleChild->m_vPos - vPos));
		}
		else
		{
		fLength = D3DXVec3Length(&(m_vScanPosition[nIdx] - vPos));
		}		
#else
		fLength = D3DXVec3Length(&(m_vScanPosition[nIdx] - vPos));
#endif
// end 2012-11-29 by mspark, ¼­Ä¡¾ÆÀÌ À§Ä¡°¡ ÀÌµ¿ÇÏ´Â Ä³¸¯ÅÍÀÇ À§Ä¡¿Í µ¿ÀÏÇÏµµ·Ï ¼öÁ¤
		if(fLength <= fScanRange)
		{
			// ½ºÄµ °Å¸® ¾È¿¡ µé¾î¿Ô´Ù
			bCanSeeInvisible= TRUE;
			return bCanSeeInvisible;
		}
	}
	return bCanSeeInvisible;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSkill::ReleasePrePareSkill()
/// \brief		
/// \author		// 2007-10-22 by bhsohn Å¸ÄÏÇü ½ºÅ³ ¿À·ù¿¡ ´ëÇÑ Ã³¸®
/// \date		2007-10-22 ~ 2007-10-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::ReleasePrePareSkill()
{
	if(NULL == m_pPriSkillInfo)
	{
		return;
	}
	ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		½¯µå ¸¶ºñ »óÅÂÀÎÁö Ã¼Å©
/// \author		// 2008-08-27 by bhsohn HP/DPµ¿½Ã¿¡ Â÷´Â ¾ÆÀÌÅÛ Ãß°¡
/// \date		2008-08-27 ~ 2008-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsShieldParalyze()
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		CSkillInfo *pSkillInfo = (*itSkillInfo);		
		// 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
		//if(IS_EXIST_DES_PARAM(pSkillInfo->ItemInfo,DES_SKILL_SHIELD_PARALYZE))
		if(pSkillInfo->ItemInfo->IsExistDesParam(DES_SKILL_SHIELD_PARALYZE))
		{
			return TRUE;
		}
		itSkillInfo++;
	}	
	return FALSE;

}


// 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö
// ÇØ´ç µ¥½º ÆÄ¶÷ÀÌ Á¸ÀçÇÏ´Â ½ºÅ³ÀÌ °É·Á ÀÖ´Â°¡
BOOL	CSkill::IsExistDesParamSKill( BYTE nDesParam )
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		CSkillInfo *pSkillInfo = (*itSkillInfo);		
		if(pSkillInfo->ItemInfo->IsExistDesParam( nDesParam ) )
			return TRUE;
		itSkillInfo++;
	}

	itSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		CSkillInfo *pSkillInfo = (*itSkillInfo);		
		if(pSkillInfo->ItemInfo->IsExistDesParam( nDesParam ) )
			return TRUE;
		itSkillInfo++;
	}

	return FALSE;
}
// end 2009. 09. 21 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ¸ó½ºÅÍ ½ºÅ³ ±¸Çö


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		ÀÚ±â ÀÚ½Å¿¡°Ô ¾µ¼ö ÀÖ´Â ½ºÅ³ÀÌ³Ä?
/// \author		// 2008-10-23 by bhsohn ÀÚ±â ÀÚ½ÅÇÑÅ× Èú ´ÜÃàÅ° Ãß°¡
/// \date		2008-10-23 ~ 2008-10-23
/// \warning	
///
/// \param		BYTE		SkillTargetType;				// ½ºÅ³ Å¸ÄÏ Å¸ÀÔ, SKILLTARGETTYPE_XXX
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsTargetToMyShuttle(BYTE	i_bySkillTargetType)
{
	// ÀÚ½Å¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³
	if(i_bySkillTargetType == SKILLTARGETTYPE_ONE_EXCLUDE_ME ||
		i_bySkillTargetType == SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME ||
		i_bySkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME ||			// µÎ ÁÙ Æ÷ÇÔ
		i_bySkillTargetType == SKILLTARGETTYPE_INRANGE_WITHOUT_ME ||
		i_bySkillTargetType == SKILLTARGETTYPE_ONE_EXCEPT_OURS)
	{
		return FALSE;
	}
	return TRUE;		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		ÀÚ±â ÀÚ½Å¿¡°Ô ¾µ¼ö ÀÖ´Â ½ºÅ³ÀÌ³Ä?
/// \author		// 2008-10-23 by bhsohn ÀÚ±â ÀÚ½ÅÇÑÅ× Èú ´ÜÃàÅ° Ãß°¡
/// \date		2008-10-23 ~ 2008-10-23
/// \warning	
///
/// \param		BYTE		SkillTargetType;				// ½ºÅ³ Å¸ÄÏ Å¸ÀÔ, SKILLTARGETTYPE_XXX
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::FindTargetForSkill_ToMe()
{
	SetSkillTargetState(FALSE);		// ½ºÅ³ ¸¶¿ì½º Æ÷ÀÎÅÍ ÇØÁ¦

	if(NULL == m_pPriSkillInfo)
	{
		return FALSE;
	}
	if(!IsTargetToMyShuttle(m_pPriSkillInfo->ItemInfo->SkillTargetType))
	{
		// ÀÚ±â ÀÚ½Å Æ÷ÇÔ ¾È‰?
		char strmsg[256];
		wsprintf(strmsg, STRMSG_C_051205_0001, m_pPriSkillInfo->ItemInfo->ItemName);// "[%s] ½ºÅ³Àº ÀÚ½Å¿¡°Ô ¾µ ¼ö ¾ø´Â ½ºÅ³ÀÔ´Ï´Ù."
		g_pD3dApp->m_pChat->CreateChatChild(strmsg, COLOR_SKILL_CANCEL);
		ReleasePrepareTargetOneSkill(m_pPriSkillInfo->ItemNum);
		return FALSE;		
	}
	else
	{
		SetTargetIndex(g_pShuttleChild->m_myShuttleInfo.ClientIndex);
		return TRUE;	
	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkill::IsSkillUse(INT	ItemNum)
/// \brief		
/// \author		// 2009-03-30 by bhsohn Â÷Â¡¼¦ ÀÌÆåÆ® ¹ö±× ¼öÁ¤
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkill::IsSkillUse(INT	ItemNum)
{
	vector<CSkillInfo*>::iterator itSkillInfo = m_vecUsingSkill.begin();
	while(itSkillInfo != m_vecUsingSkill.end())
	{
		if(ItemNum == SKILL_BASE_NUM((*itSkillInfo)->ItemNum))
		{
			return TRUE;
		}
		itSkillInfo++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È ½Ã½ºÅÛ
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkill::DisableTimeLimitSkill(INT ItemNum)
{
	vector<CSkillInfo*>::iterator itvecSkillInfo = m_vecEnemyUsingSkillForMe.begin();
	while(itvecSkillInfo != m_vecEnemyUsingSkillForMe.end())
	{
		CSkillInfo* pSkillInfo = (*itvecSkillInfo);
		if(pSkillInfo && ItemNum == pSkillInfo->ItemNum)
		{
			pSkillInfo->DisableTimelimitSkill();
		}
		itvecSkillInfo++;
	}
}