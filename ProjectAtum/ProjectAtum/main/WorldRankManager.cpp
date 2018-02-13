// WorldRankManager.cpp: implementation of the CWorldRankManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "Interface.h"
#include "INFBase.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"

#include "WorldRankManager.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 서비스사 이미지 크기
#define			SERVICE_IMG_WIDTH				48
#define			SERVICE_IMG_HEIGHT				28

#define			MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE	(SIZE_MAX_SERVICE_SYMBOL_IMAGE+10)

CWorldRankManager::CWorldRankManager() :
	m_ServiceInfo { },
	m_ServiceInfoStruct { nullptr }
{
	int nCnt = 0;
	int nGear = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
	{
		for(nGear = 0;nGear < MAX_WORLDRANK_GEAR;nGear++)
		{
			m_vecLocalRank_LV[nCnt][nGear].clear();			// 로컬 랭킹
			m_vecLocalRank_Fame[nCnt][nGear].clear();			// 로컬 랭킹
			m_vecLocalRank_PVP[nCnt][nGear].clear();			// 로컬 랭킹

			m_vecWorldRank_LV[nCnt][nGear].clear();			// 월드 랭킹
			m_vecWorldRank_Fame[nCnt][nGear].clear();			// 월드 랭킹
			m_vecWorldRank_PVP[nCnt][nGear].clear();			// 월드 랭킹
		}
	}	
	
	// 요청할 기어 인덱스
	m_nRqGearIdx = WORLDRANK_GEAR_ALL;

	memset(&m_structMyRankInfo, 0x00, sizeof(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK));

	// 월드 랭킹 시스템 사용 유무
	m_bUseWorldRanking = FALSE;

}

CWorldRankManager::~CWorldRankManager()
{
	DeleteServerImageInfo();
	int nCnt = 0;
	int nGear = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
	{
		for(nGear = 0;nGear < MAX_WORLDRANK_GEAR;nGear++)
		{
			m_vecLocalRank_LV[nCnt][nGear].clear();			// 로컬 랭킹
			m_vecLocalRank_Fame[nCnt][nGear].clear();			// 로컬 랭킹
			m_vecLocalRank_PVP[nCnt][nGear].clear();			// 로컬 랭킹

			m_vecWorldRank_LV[nCnt][nGear].clear();			// 월드 랭킹
			m_vecWorldRank_Fame[nCnt][nGear].clear();			// 월드 랭킹
			m_vecWorldRank_PVP[nCnt][nGear].clear();			// 월드 랭킹
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		정보를 추가한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
									 int i_nInfoIdx,			// 레벨, 명성치, PVP
									 int i_nGearIdx,			// 기어 인덱스
										structRankInfo*		pstruRankInfo)			// 서버명
{
	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR))
	{
		return;
	}

	/*
	structRankInfo struRankInfo;
	memcpy(&struRankInfo, pstruRankInfo, sizeof(structRankInfo));	

	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			m_vecLocalRank_LV[i_nInfoIdx][i_nGearIdx].push_back(struRankInfo);
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			m_vecWorldRank[i_nInfoIdx][i_nGearIdx].push_back(struRankInfo);
		}
		break;
	}		
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		전체 사이즈를 알아온다
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWorldRankManager::GetWorldRankSize(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
										int i_nInfoIdx,			// 레벨, 명성치, PVP
										int i_nGearIdx,			// 기어 인덱스
										int i_nPage)			// 페이지 위치
{
	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR)
		|| (i_nPage >= MAX_WORLDRANK_PAGE))
	{
		return 0;
	}
	
	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// 레벨
				{
					return m_vecLocalRank_LV[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_HONOR:	// 명성치
				{
					return m_vecLocalRank_Fame[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					return m_vecLocalRank_PVP[i_nPage][i_nGearIdx].size();
				}
				break;
			}			
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// 레벨
				{
					return m_vecWorldRank_LV[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_HONOR:	// 명성치
				{
					return m_vecWorldRank_Fame[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					return m_vecWorldRank_PVP[i_nPage][i_nGearIdx].size();
				}
				break;
			}		
		}
		break;
	}	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		랭킹 정보를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
structRankInfo *CWorldRankManager::GetWorldRankInfo(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
										int i_nInfoIdx,			// 레벨, 명성치, PVP
										int i_nGearIdx,			// 기어 인덱스
										int i_nSelectPage,
										int i_nIdx)			
{
	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR)
		||(i_nSelectPage >= MAX_WORLDRANK_PAGE))
	{
		return NULL;
	}
	vector<structRankInfo>*	pSelRank = NULL;
	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// 레벨
				{
					if(m_vecLocalRank_LV[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecLocalRank_LV[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_HONOR:	// 명성치
				{
					if(m_vecLocalRank_Fame[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecLocalRank_Fame[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					if(m_vecLocalRank_PVP[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecLocalRank_PVP[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			}
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// 레벨
				{
					if(m_vecWorldRank_LV[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecWorldRank_LV[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_HONOR:	// 명성치
				{
					if(m_vecWorldRank_Fame[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecWorldRank_Fame[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					if(m_vecWorldRank_PVP[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecWorldRank_PVP[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			}
		}
		break;	
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		나의 랭킹 정보를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWorldRankManager::GetMyRankInfo(int i_nInfoIdx)// 레벨, 명성치, PVP)
{
	switch(i_nInfoIdx)
	{
	case WORLDRANK_INFO_LV:
		{
			return m_structMyRankInfo.LevelRanking;
		}
		break;
	case WORLDRANK_INFO_HONOR:
		{
			return m_structMyRankInfo.FameRanking;
		}
		break;
	case WORLDRANK_INFO_PVP:
		{
			return m_structMyRankInfo.PvPRanking;
		}
		break;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		나의 랭킹 정보를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::RqGetServiceInfo()
{
	if(!GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return FALSE;
	}
	if(m_ServiceInfo.ServiceName[0] == '\0')
	{
		// 어떤 정보가 있으면 리턴
		return FALSE;
	}
	g_pFieldWinSocket->SendMsg(T_FC_INFO_WRK_GET_SERVICE_INFO,NULL,NULL);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		나의 랭킹 정보를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::RqGetMyRankInfo()
{
	if(!GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return FALSE;
	}

// 	if(m_structMyRankInfo.CharacUID != 0)
// 	{
// 		// 어떤 정보가 있으면 리턴
// 		return FALSE;
// 	}
	g_pFieldWinSocket->SendMsg(T_FC_INFO_WRK_GET_SELF_RANKING,NULL,NULL);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		나의 랭킹 정보를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::SetMyRankInfo(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK	*pstructMyRankInfo)
{
	memcpy(&m_structMyRankInfo, pstructMyRankInfo, sizeof(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddServiceInfo(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK* pMsg)
{
	MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK sMsg;

	util::zero(sMsg);

	m_ServiceInfo = sMsg;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK CWorldRankManager::GetServiceInfo()
{
	return m_ServiceInfo;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::GetServiceStringInfo(char* o_pServer)
{
	util::strncpy(o_pServer, m_ServiceInfo.ServiceName, SIZE_MAX_SERVICE_NAME);

	return m_ServiceInfo.ServiceName[0] != '\0';
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사 이미지를 지정한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddServiceInfoImage(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE* pMsg)
{	
	auto pServiceInfo = m_ServiceInfoStruct;

	if (!pServiceInfo)
	{
		// 없으면 새로 생성
		pServiceInfo = new structServiceInfo;		
		pServiceInfo->nDataPos = 0;		
		pServiceInfo->bImageLoadDone = FALSE;		// 이미지 로드 성공		
		pServiceInfo->pImage = NULL;		
		pServiceInfo->bRestored = FALSE;	// 초기화 아직 안했음

		// 4096이 올꺼지만 그래도 약간 더 잡자
		pServiceInfo->pData = new char[MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE];
		memset(pServiceInfo->pData, 0x00, MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE);		
	}

	// 이미지는 4096이 올꺼다 그러므로 메모리 오버플로가 날수있다.
	// 가장 마지막 인덱스는 96이다 ->1000+1000+1000+1000+96 = 4096
	BOOL bReceiveImageDone = FALSE;
	int nCopySize = 1000;// BYTE	SymbolImage[1000]; 
	if((1 == pMsg->SymbolImageIndex)
		||((pServiceInfo->nDataPos + nCopySize) >= MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE))
	{
		bReceiveImageDone = TRUE;		
	}
	
	if((pServiceInfo->nDataPos+nCopySize) < MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE)
	{
		memcpy(&pServiceInfo->pData[pServiceInfo->nDataPos], pMsg->SymbolImage, nCopySize);		
	}

	if(bReceiveImageDone && !pServiceInfo->bImageLoadDone)
	{		
		pServiceInfo->bImageLoadDone = TRUE;
// 		pServiceInfo->pImage = new CINFImage;		
// 		pServiceInfo->pImage->InitDeviceObjects( pServiceInfo->pData, 4096 );		
// 		pServiceInfo->pImage->RestoreDeviceObjects();
// 		pServiceInfo->bRestored = TRUE;
	}		
	
	pServiceInfo->nDataPos = pServiceInfo->nDataPos + nCopySize;
	
	m_ServiceInfoStruct = pServiceInfo;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사 이미지를 가져온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImage* CWorldRankManager::GetServiceInfoImage()
{	 
	if (m_ServiceInfoStruct) return m_ServiceInfoStruct->pImage;
	
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사 이미지를 지정한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::InitDeviceObjects_ServiceInfoImage()
{	
	auto pServiceInfo = m_ServiceInfoStruct;
	if(pServiceInfo && !pServiceInfo->pImage && pServiceInfo->bImageLoadDone)
	{
		MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK  stServerInfo = GetServiceInfo();
        // 2011. 10. 10 by jskim UI시스템 변경
		pServiceInfo->pImage = new CINFImageEx;	
		DataHeader* pHeader = new DataHeader;
		pHeader->m_pData = pServiceInfo->pData;
		pHeader->m_DataSize = stServerInfo.SymbolImageSize;

		pServiceInfo->pImage->InitDeviceObjects( pHeader );	
        // end 2011. 10. 10 by jskim UI시스템 변경

		pServiceInfo->bRestored = FALSE;
	}				
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사 이미지를 지정한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::RestoreDeviceObjects_ServiceInfoImage()
{
	auto pServiceInfo = m_ServiceInfoStruct;

	if(pServiceInfo && pServiceInfo->pImage && pServiceInfo->bImageLoadDone && !pServiceInfo->bRestored)
	{
		pServiceInfo->pImage->RestoreDeviceObjects();
		pServiceInfo->pImage->SetRect(0,0,SERVICE_IMG_WIDTH,SERVICE_IMG_HEIGHT);

		pServiceInfo->bRestored = TRUE;
	}				
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사 이미지를 지정한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::InvalidateDeviceObjects_ServiceInfoImage()
{
	auto pServiceInfo = m_ServiceInfoStruct;

	if(pServiceInfo && pServiceInfo->pImage && pServiceInfo->bImageLoadDone && pServiceInfo->bRestored)
	{
		pServiceInfo->pImage->InvalidateDeviceObjects();
		pServiceInfo->bRestored = FALSE;
	}				
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사 이미지를 지정한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::DeleteDeviceObjects_ServiceInfoImage()
{
	auto pServiceInfo = m_ServiceInfoStruct;

	if (m_ServiceInfoStruct)
	{
		pServiceInfo->bRestored = FALSE;

		if (pServiceInfo->pImage)
		{
			pServiceInfo->pImage->DeleteDeviceObjects();
			util::del(pServiceInfo->pImage);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서비스 사를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::DeleteServerImageInfo()
{
	auto it = m_ServiceInfoStruct;

	if (it)
	{
		if (it->pImage)
		{
			it->pImage->InvalidateDeviceObjects();
			it->pImage->DeleteDeviceObjects();
			util::del(it->pImage);
		}
		util::del(it->pData);
		util::del(it);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		울더 랭킹 정보를 얻어온다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::RqWorldRankInfo(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
										int i_nInfoIdx,			// 레벨, 명성치, PVP
										int i_nGearIdx,			// 기어 인덱스
										int i_nIdx,
										int i_nRqItemCnt,
										BOOL bCtrlRq)	// 클릭해서 요청한거냐?
{
	if(!GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return TRUE;
	}

	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR))
	{
		return TRUE;
	}

	if(!IsRqWorldRankInfo(i_nLocalWorldIdx, i_nInfoIdx, i_nGearIdx, i_nIdx))
	{
		return TRUE;
	}
	
	MSG_FC_INFO_WRK_GET_RANKER_LIST sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_INFO_WRK_GET_RANKER_LIST));

	// 지역? 지구?
	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			sMsg.byScope = RANKING_SCOPE_LOCAL;
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			sMsg.byScope = RANKING_SCOPE_WORLD;
		}
		break;
	}
	// 기어 인덱스
	m_nRqGearIdx = i_nGearIdx;	// 요청할 기어 인덱스
	switch(i_nGearIdx)
	{
	case WORLDRANK_GEAR_ALL:
		{
			// 전체 기어 요청
			sMsg.UnitKind = UNITKIND_ALL_MASK;			
		}
		break;
	case WORLDRANK_GEAR_B:
		{
			sMsg.UnitKind = UNITKIND_BGEAR_MASK;
		}
		break;
	case WORLDRANK_GEAR_M:
		{
			sMsg.UnitKind = UNITKIND_MGEAR_MASK;
		}
		break;
	case WORLDRANK_GEAR_A:
		{
			sMsg.UnitKind = UNITKIND_AGEAR_MASK;
		}
		break;
	case WORLDRANK_GEAR_I:
		{
			sMsg.UnitKind = UNITKIND_IGEAR_MASK;
		}
		break;
	}
	// 레벨, 명성치, PVP
	switch(i_nInfoIdx)
	{
	case WORLDRANK_INFO_LV:
		{
			sMsg.byRankingType = RANKING_TYPE_LEVEL;
		}
		break;
	case WORLDRANK_INFO_HONOR:
		{
			sMsg.byRankingType = RANKING_TYPE_FAME;
		}
		break;
	case WORLDRANK_INFO_PVP:
		{
			sMsg.byRankingType = RANKING_TYPE_PVP;
		}
		break;
	}

	// 시작 인덱스
	sMsg.byStartRank = i_nIdx;
	
	// 값갯수
	sMsg.byReqRankerCnt = i_nRqItemCnt;
	
	g_pFieldWinSocket->SendMsg(T_FC_INFO_WRK_GET_RANKER_LIST, (char*)&sMsg, sizeof(sMsg));

	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		이미 있는 정보 인지 체크해서 요청할지 말지 판단
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::IsRqWorldRankInfo(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
										int i_nInfoIdx,			// 레벨, 명성치, PVP
										int i_nGearIdx,			// 기어 인덱스
										int i_nStartIdx)		// 시작 페이지
{
	int nIdx = (i_nStartIdx /MAX_WORLDRANK_PAGE);
	// 이미 있는 정보 인지 체크
	if(GetWorldRankSize(i_nLocalWorldIdx,i_nInfoIdx, i_nGearIdx, nIdx) > 0)
	{
		return FALSE;
	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		레벨 랭킹 정보 추가
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank_Lv(int nStartRank, SWRK_LEVEL* pSwrk)
{
	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));	

	int nPage = (nStartRank-1)/MAX_WORLDRANK_ONEPAGE;	// 한페이지 최대수에서 뺴�?

	int nGear = m_nRqGearIdx;

	if(nPage < 0)
	{
		nPage = 0;
	}
	else if(nPage >= MAX_WORLDRANK_PAGE)
	{
		nPage = MAX_WORLDRANK_PAGE-1;
	}

	// 서비스명
	GetServiceStringInfo(struRankInfo.chServiceName);
	
	strcpy(struRankInfo.chGearType, AtumSJ::GetUnitKindString(pSwrk->UnitKind));	
	

	// 서버명
	util::strncpy(struRankInfo.chServerName, pSwrk->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
	// 닉네임
	util::strncpy(struRankInfo.chNickName, pSwrk->NameforRK, SIZE_MAX_NAME_FOR_RANKING);

	// 세력명
	if(IS_VCN_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_BCU);
	}	
	else if(IS_ANI_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);
	}
	//레벨
	struRankInfo.nLevel = pSwrk->Level;
	
	// 어떤 랭킹이냐?
	switch(pSwrk->RankingScope)		// RANKING_SCOPE_XXX
	{
	case RANKING_SCOPE_LOCAL:		// 현재 서비스 로컬 랭킹, Name 이 NickName
		{
			m_vecLocalRank_LV[nPage][nGear].push_back(struRankInfo);			// 로컬 랭킹_레벨

		}
		break;
	case RANKING_SCOPE_WORLD:		// 월드 랭킹 정보, Name 이 CharacterName
		{
			m_vecWorldRank_LV[nPage][nGear].push_back(struRankInfo);			// 로컬 랭킹_레벨			
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		명성치 랭킹 정보 추가
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank_Fame(int nStartRank, SWRK_FAME* pSwrk)
{
	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));	
	
	int nPage = (nStartRank-1)/MAX_WORLDRANK_ONEPAGE;	// 한페이지 최대수에서 뺴�?
	
	int nGear = m_nRqGearIdx;
	
	if(nPage < 0)
	{
		nPage = 0;
	}
	else if(nPage >= MAX_WORLDRANK_PAGE)
	{
		nPage = MAX_WORLDRANK_PAGE-1;
	}
	
	// 서비스명
	GetServiceStringInfo(struRankInfo.chServiceName);
	// 유닛명
	{
		strcpy(struRankInfo.chGearType, AtumSJ::GetUnitKindString(pSwrk->UnitKind));	
	}
	
	// 서버명
	util::strncpy(struRankInfo.chServerName, pSwrk->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
	// 닉네임
	util::strncpy(struRankInfo.chNickName, pSwrk->NameforRK, SIZE_MAX_NAME_FOR_RANKING);
	
	// 세력명
	if(IS_VCN_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_BCU);
	}	
	else if(IS_ANI_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);
	}
	//레벨
	struRankInfo.nLevel = pSwrk->Level;
	// 개인 명성
	struRankInfo.nHonor = pSwrk->Fame;
	
	// 어떤 랭킹이냐?
	switch(pSwrk->RankingScope)		// RANKING_SCOPE_XXX
	{
	case RANKING_SCOPE_LOCAL:		// 현재 서비스 로컬 랭킹, Name 이 NickName
		{
			m_vecLocalRank_Fame[nPage][nGear].push_back(struRankInfo);			// 로컬 랭킹_레벨
			
		}
		break;
	case RANKING_SCOPE_WORLD:		// 월드 랭킹 정보, Name 이 CharacterName
		{
			m_vecWorldRank_Fame[nPage][nGear].push_back(struRankInfo);			// 로컬 랭킹_레벨			
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		PVP 랭킹 정보 추가
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank_PVP(int nStartRank, SWRK_PVP* pSwrk)
{
	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));	
	
	int nPage = (nStartRank-1)/MAX_WORLDRANK_ONEPAGE;	// 한페이지 최대수에서 뺴�?
	
	int nGear = m_nRqGearIdx;
	
	if(nPage < 0)
	{
		nPage = 0;
	}
	else if(nPage >= MAX_WORLDRANK_PAGE)
	{
		nPage = MAX_WORLDRANK_PAGE-1;
	}
	
	// 서비스명
	GetServiceStringInfo(struRankInfo.chServiceName);
	// 유닛명
	{
		strcpy(struRankInfo.chGearType, AtumSJ::GetUnitKindString(pSwrk->UnitKind));	
	}
	
	// 서버명
	util::strncpy(struRankInfo.chServerName, pSwrk->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
	// 닉네임
	util::strncpy(struRankInfo.chNickName, pSwrk->NameforRK, SIZE_MAX_NAME_FOR_RANKING);
	
	// 세력명
	if(IS_VCN_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_BCU);
	}	
	else if(IS_ANI_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);
	}
	//레벨
	struRankInfo.nLevel = pSwrk->Level;
	// 스코어
	struRankInfo.nScore = pSwrk->Score;
	// 승
	struRankInfo.nWinNum = pSwrk->WinPoint;
	// 패
	struRankInfo.nLoseNum = pSwrk->LossPoint;
	
	// 어떤 랭킹이냐?
	switch(pSwrk->RankingScope)		// RANKING_SCOPE_XXX
	{
	case RANKING_SCOPE_LOCAL:		// 현재 서비스 로컬 랭킹, Name 이 NickName
		{
			m_vecLocalRank_PVP[nPage][nGear].push_back(struRankInfo);			// 로컬 랭킹_레벨
			
		}
		break;
	case RANKING_SCOPE_WORLD:		// 월드 랭킹 정보, Name 이 CharacterName
		{
			m_vecWorldRank_PVP[nPage][nGear].push_back(struRankInfo);			// 로컬 랭킹_레벨			
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		닉네임 추가
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::RqCreateMyNickName(char* i_pNickName, UID32_t	i_myCharacterUniqueNumber)
{
	if(!GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return;
	}
	MSG_FC_CHARACTER_CHANGE_NICKNAME sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_CHARACTER_CHANGE_NICKNAME));

	sMsg.CharacUID  = i_myCharacterUniqueNumber;
	strncpy(sMsg.NickName, i_pNickName, 10);	// 닉네임은 10글자로 
	
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_CHARACTER_CHANGE_NICKNAME);		
		vecUnLockMsg.push_back(T_FC_CHARACTER_CHANGE_NICKNAME_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_CHARACTER_CHANGE_NICKNAME, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	g_pFieldWinSocket->SendMsg(T_FC_CHARACTER_CHANGE_NICKNAME, (char*)&sMsg, sizeof(sMsg));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		닉네임 추가
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::IsValidNickName(char* i_pCharater)
{
	if(0 == strlen(i_pCharater))
	{
		return FALSE;
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < strlen(i_pCharater)+1;nCnt++)
	{
		char	*pStr = CharNext(&i_pCharater[nCnt]);
		if(NULL == pStr)
		{
			break;
		}
		int nStrPointGap = pStr - &i_pCharater[nCnt];
		if(nStrPointGap > 1)
		{
			// 영문 숫자가 아닌 글씨가 있다.
			return FALSE;
		}
		if( ('a' <= i_pCharater[nCnt]) &&(i_pCharater[nCnt] <= 'z'))
		{
			int a=1;

		}
		else if( ('A' <= i_pCharater[nCnt]) &&(i_pCharater[nCnt] <= 'Z'))
		{
			int a=2;

		}
		else if( ('0' <= i_pCharater[nCnt]) &&(i_pCharater[nCnt] <= '9' ))
		{
			int a=3;
		}
		else if(NULL == i_pCharater[nCnt])
		{
			int a=4;
		}
		else
		{
			// 에러되는 문자가있다.
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		처음 월드 랭킹 정보를 요청한다.
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::RqInitWorldRank()
{
	if(!GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return;
	}
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if(IS_CITY_MAP_INDEX(myShuttleInfo.MapChannelIndex.MapIndex))
	{
		// 마을일떄만 요청		
		if(!RqGetServiceInfo())	// 서비스 리스트 요청
		{
			// 전체 1순위 랭커들이 정보를 요청한다.
			g_pD3dApp->RqAllWorldRankInfo();
		}
		RqGetMyRankInfo(); // 나의 랭킹 정보 요청
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::SetUseWorldRanking(BOOL bUseWorldRanking)
{
	m_bUseWorldRanking = bUseWorldRanking;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 월드 랭킹 시스템 사용 유무
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::GetUseWorldRanking()
{
	return m_bUseWorldRanking;		

}