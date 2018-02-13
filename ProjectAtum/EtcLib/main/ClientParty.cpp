// ClientParty.cpp: implementation of the CClientParty class.
//
//////////////////////////////////////////////////////////////////////

#include "ClientParty.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "Chat.h"
#include "EnemyData.h"
#include "FieldWinSocket.h"
// 2007-11-22 by bhsohn 아레나 통합서버
//#include "IMSocket.h"
#include "IMSocketManager.h"
#include "SceneData.h"
#include "INFGameMainChat.h"
#include "INFGameMain.h"
#include "INFCommunity.h"
#include "INFCommunityParty.h"
#include "INFCommunityReject.h"
#include "AtumDatabase.h"
#include "ObjectChild.h"
#include "ChatMoveData.h"
#include "dxutil.h"

#include "InfinityManager.h"
#include "TimeMgr.h" // 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientParty::CClientParty()
{
	m_bParty = FALSE;
	m_bFormationFlight = FALSE;
	memset(&m_infoParty,0x00,sizeof(PARTYINFO));
	memset(&m_infoMaster,0x00,sizeof(MASTERINFO));
	//m_bNtoNCallState = FALSE;

	// 2008-06-03 by bhsohn EP3 편대 관련 처리
	memset(&m_stPartyInfo, 0x00, sizeof(SPARTY_INFO));
	m_nOldFormationFlyingType = -1;

	m_bFirstPartySetupInfo = TRUE;

	m_nTempMMasterPartyID = 0;// 2008-12-09 by dgwoo 미션마스터.
}

CClientParty::~CClientParty()
{	
	m_vecPeerPartyEnemyInfo.clear();
	PartyDeleteAll();
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// 일반 함수들 
/// 2004-08-19 jschoi
///////////////////////////////////////////////////////////////////////////////////////////////

void CClientParty::SetMasterInfo()
{
	if(m_infoParty.bPartyType == _PARTYMASTER)
	{
		m_infoMaster.m_nUniqueNumber = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
		m_infoMaster.m_vPos		= g_pShuttleChild->m_vPos;
		m_infoMaster.m_vVel		= g_pShuttleChild->m_vVel;
//		m_infoMaster.m_vSide	= -g_pShuttleChild->m_vSideVel;
		m_infoMaster.m_vSide.x	= g_pShuttleChild->m_mMatrix._11; 
		m_infoMaster.m_vSide.y	= g_pShuttleChild->m_mMatrix._12;
		m_infoMaster.m_vSide.z	= g_pShuttleChild->m_mMatrix._13;
		m_infoMaster.m_vUp.x	= g_pShuttleChild->m_mMatrix._21;
		m_infoMaster.m_vUp.y	= g_pShuttleChild->m_mMatrix._22;
		m_infoMaster.m_vUp.z	= g_pShuttleChild->m_mMatrix._23;
	}
	else
	{
		vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
		while(itParty != m_vecPartyEnemyInfo.end())
		{
			if((*itParty)->m_bPartyType == _PARTYMASTER)
			{
				m_infoMaster.m_nUniqueNumber = (*itParty)->m_nUniqueNumber;
				m_infoMaster.m_vPos		= (*itParty)->m_pEnemyData->m_vPos;
				m_infoMaster.m_vVel		= (*itParty)->m_pEnemyData->m_vVel;
				m_infoMaster.m_vSide	= (*itParty)->m_pEnemyData->m_vSideVel;
				m_infoMaster.m_vUp		= (*itParty)->m_pEnemyData->m_vUp;
				break;
			}
			itParty++;
		}		
	}
}

MASTERINFO CClientParty::GetMasterInfo()
{
	return m_infoMaster;
}

BOOL CClientParty::IsFormationFlightMove()
{
	if(m_bFormationFlight && m_infoParty.bPartyType == _PARTYMEMBER)
		return TRUE;
	return FALSE;
}

void CClientParty::FormationFlightDismember()
{
	// 필요하나?
}

void CClientParty::FormationFlightClear()
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		(*itParty)->m_bFormationFlight = FALSE;
		(*itParty)->m_bFormationFlyingPositionType = 0;
		(*itParty)->m_bFormationFlyingType = FLIGHT_FORM_NONE;
		itParty++;
	}

	m_bFormationFlight = FALSE;
	m_infoParty.bFormationFlyingPositionType = 0;
}

PARTYENEMYINFO*	CClientParty::PartyFindMemberByClientIndex(ClientIndex_t nClientIndex)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if(	(*itParty)->m_pEnemyData &&
			(*itParty)->m_pEnemyData->m_infoCharacter.CharacterInfo.ClientIndex == nClientIndex)
		{
			return (*itParty);
		}
		itParty++;
	}
	return NULL;
}

PARTYENEMYINFO*	CClientParty::PartyFindMemberByUniqueNumber(LONGLONG nUniqueNumber)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_nUniqueNumber == nUniqueNumber)
		{
			return (*itParty);
		}
		itParty++;
	}
	return NULL;
}

PARTYENEMYINFO* CClientParty::PartyFindMemberByUnitName(char* strName)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if(!strcmp((*itParty)->m_ImPartyMemberInfo.CharacterName, strName))
		{
			return (*itParty);
		}
		itParty++;
	}
	return NULL;
}

PARTYENEMYINFO*	CClientParty::FormationFlightFindMemberByClientIndex(ClientIndex_t nClientIndex)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if(	(*itParty)->m_pEnemyData &&
			(*itParty)->m_pEnemyData->m_infoCharacter.CharacterInfo.ClientIndex == nClientIndex &&
			(*itParty)->m_bFormationFlight)
		{
			return (*itParty);
		}
		itParty++;
	}
	return NULL;
}

PARTYENEMYINFO* CClientParty::FormationFlightFindMemberByUniqueNumber(LONGLONG nUniqueNumber)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if(	(*itParty)->m_nUniqueNumber == nUniqueNumber &&
			(*itParty)->m_bFormationFlight)
		{
			return (*itParty);
		}
		itParty++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// 내부 함수들 
/// 2004-08-19 jschoi
///////////////////////////////////////////////////////////////////////////////////////////////

void CClientParty::PartyCreate(PartyID_t PartyID) //내가 파티마스터일 때만?
{
	m_infoParty.nMasterUniqueNumber = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
	m_infoParty.bPartyType = _PARTYMASTER;
	m_infoParty.PartyID = PartyID;	
	m_infoParty.bFormationFlyingPositionType = 0;
	m_infoParty.bFormationFlyingType = FLIGHT_FORM_NONE;
	m_bFormationFlight = FALSE;
	g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0002,COLOR_PARTY);//"편대 그룹을 생성 하였습니다"
	//m_bParty = FALSE;
	m_bParty = TRUE;
}

void CClientParty::PartyDeleteAll()
{
	FormationFlightClear();

	for (auto info : m_vecPartyEnemyInfo)
	{
		if (info->m_pEnemyData) info->m_pEnemyData->m_nPartyID = 0;

		delete info;
	}

	m_vecPartyEnemyInfo.clear();

	m_infoParty.bPartyType = _NOPARTY;

	m_bParty = false;
}

void CClientParty::PartyInsertMember(IM_PARTY_MEMBER_INFO& ImPartyMemberInfo)
{	
	CEnemyData* pEnemyData = NULL;
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		if(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber == ImPartyMemberInfo.CharacterUniqueNumber)
		{
			pEnemyData = itEnemy->second;
			break;
		}
		itEnemy++;
	}

	BOOL bIsNew = FALSE;
	PARTYENEMYINFO* pPartyEnemyInfo = PartyFindMemberByUniqueNumber(ImPartyMemberInfo.CharacterUniqueNumber);

	if(pPartyEnemyInfo == NULL)
	{
		bIsNew = TRUE;
		pPartyEnemyInfo = new PARTYENEMYINFO;
	}

	pPartyEnemyInfo->m_nUniqueNumber = ImPartyMemberInfo.CharacterUniqueNumber;
	memcpy(&pPartyEnemyInfo->m_ImPartyMemberInfo ,(char*)&ImPartyMemberInfo,sizeof(IM_PARTY_MEMBER_INFO));
	pPartyEnemyInfo->m_pEnemyData = pEnemyData;	

	pPartyEnemyInfo->m_bFormationFlight = FALSE;
	pPartyEnemyInfo->m_bFormationFlyingType = FLIGHT_FORM_NONE;
	pPartyEnemyInfo->m_bFormationFlyingPositionType = 0;
	pPartyEnemyInfo->m_bSpeakingRender = FALSE;
	// 2008-12-02 by bhsohn 편대원 온오프라인 상태체크
	pPartyEnemyInfo->m_bUserLogOn = TRUE;
	// end 2008-12-02 by bhsohn 편대원 온오프라인 상태체크

	if(pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_nPartyID = m_infoParty.PartyID;
	}	
	
	if( pPartyEnemyInfo->m_pEnemyData &&
		pPartyEnemyInfo->m_ImPartyMemberInfo.MapChannelIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex)
	{	
		if(pPartyEnemyInfo->m_nUniqueNumber == m_infoParty.nMasterUniqueNumber)
		{
			pPartyEnemyInfo->m_bPartyType = _PARTYMASTER;
		}
		else
		{
			pPartyEnemyInfo->m_bPartyType = _PARTYMEMBER;
		}
		MSG_FC_PARTY_GET_MEMBER sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		sMsg.CharacterUniqueNumber = pPartyEnemyInfo->m_nUniqueNumber;
		int nType = T_FC_PARTY_GET_MEMBER;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	}
	else
	{
		if(pPartyEnemyInfo->m_nUniqueNumber == m_infoParty.nMasterUniqueNumber)
		{
			pPartyEnemyInfo->m_bPartyType = _PARTYOTHERMAPMASTER;
		}
		else
		{
			pPartyEnemyInfo->m_bPartyType = _PARTYOTHERMAPMEMBER;
		}
	}
	pPartyEnemyInfo->m_bFormationFlight = FALSE;
	pPartyEnemyInfo->m_bFormationFlyingPositionType = 0;
	pPartyEnemyInfo->m_bSpeakingRender	= FALSE;
	pPartyEnemyInfo->VoipType			= ImPartyMemberInfo.VoipType;

	// 2008-12-02 by bhsohn 편대원 온오프라인 상태체크
	pPartyEnemyInfo->m_bUserLogOn = TRUE;
	if(0 == pPartyEnemyInfo->m_ImPartyMemberInfo.MapChannelIndex.MapIndex)
	{
		pPartyEnemyInfo->m_bUserLogOn = FALSE;
	}
	// end 2008-12-02 by bhsohn 편대원 온오프라인 상태체크

	if(bIsNew)
	{
		m_vecPartyEnemyInfo.push_back(pPartyEnemyInfo);
	}
	
	if(m_infoParty.bPartyType == _PARTYMASTER)
	{
		MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_IC_PARTY_CHANGE_FLIGHT_FORMATION;
		sMsg.PartyID = m_infoParty.PartyID;
		sMsg.Formation = m_infoParty.bFormationFlyingType;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	}


	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	{
		// 파티원 추가시 파티원의 디버프 정보 요청
		MSG_FC_CHARACTER_DEBUFF_DOT_INFO sMsg;
		memset( &sMsg, 0, sizeof( sMsg ) );
		sMsg.CharacterUID = ImPartyMemberInfo.CharacterUniqueNumber;
		g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_DEBUFF_DOT_INFO, (char*)&sMsg, sizeof(sMsg) );
	}
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

	
	m_bParty = TRUE;
}

void CClientParty::PartyDeleteMember(LONGLONG nUniqueNumber,char* pName)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_nUniqueNumber == nUniqueNumber)
		{
			memcpy(pName, &(*itParty)->m_ImPartyMemberInfo.CharacterName, sizeof((*itParty)->m_ImPartyMemberInfo.CharacterName));
			if((*itParty)->m_pEnemyData)
			{
				(*itParty)->m_pEnemyData->m_nPartyID = 0;
			}
			util::del(*itParty);
			m_vecPartyEnemyInfo.erase(itParty);
			break;
		}
		itParty++;
	}
// 2008-12-17 by dgwoo 파티원들이 다 나가도 나혼자 파티는 유지된다.
//	if(m_vecPartyEnemyInfo.size() == 0)
//	{
//		m_bParty = FALSE;
//	}
}

void CClientParty::PartyChangeMaster(LONGLONG nNewMasterUniqueNumber, LONGLONG nOldMasterUniqueNumber ,char* pName)
{
	if(g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber == nNewMasterUniqueNumber)
	{// 내가 새로운 마스터 라면
		m_infoParty.bPartyType = _PARTYMASTER;
		memcpy(pName, &g_pShuttleChild->m_myShuttleInfo.CharacterName, sizeof(g_pShuttleChild->m_myShuttleInfo.CharacterName));
	}
	else
	{
		m_infoParty.bPartyType = _PARTYMEMBER;
	}
	
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_nUniqueNumber == nNewMasterUniqueNumber)
		{	// 파티원이 마스터 라면
			memcpy(pName, &(*itParty)->m_ImPartyMemberInfo.CharacterName, sizeof((*itParty)->m_ImPartyMemberInfo.CharacterName));
			if(	(*itParty)->m_pEnemyData &&
				(*itParty)->m_ImPartyMemberInfo.MapChannelIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex)
			{
				(*itParty)->m_bPartyType = _PARTYMASTER;
			}
			else
			{
				(*itParty)->m_bPartyType = _PARTYOTHERMAPMASTER;
			}
		}
		else
		{	// 파티원이 마스터가 아니라면
			if(	(*itParty)->m_pEnemyData &&
				(*itParty)->m_ImPartyMemberInfo.MapChannelIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex)
			{
				(*itParty)->m_bPartyType = _PARTYMEMBER;
			}
			else
			{
				(*itParty)->m_bPartyType = _PARTYOTHERMAPMEMBER;
			}
		}
		
		// 편대비행 초기화
		(*itParty)->m_bFormationFlight = FALSE;
		(*itParty)->m_bFormationFlyingPositionType = 0;
		(*itParty)->m_bFormationFlyingType = FLIGHT_FORM_NONE;
		itParty++;
	}
	
	m_infoParty.nMasterUniqueNumber = nNewMasterUniqueNumber;
	m_infoParty.bFormationFlyingType = FLIGHT_FORM_NONE;
	m_infoParty.bFormationFlyingPositionType = 0;
	m_bFormationFlight = FALSE;
}

void CClientParty::FormationFlightInsertMember(LONGLONG nUniqueNumber)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_nUniqueNumber == nUniqueNumber)
		{
			(*itParty)->m_bFormationFlight = TRUE;
			(*itParty)->m_bFormationFlyingType = m_infoParty.bFormationFlyingType;
			break;
		}
		itParty++;
	}
	BOOL bResult = FormationFlightSortPosition();
	if(bResult == FALSE)
	{
		DBGOUT("Failed FormationFlight Sort Position\n");
	}
}

void CClientParty::FormationFlightDeleteMember(LONGLONG nUniqueNumber)
{
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_nUniqueNumber == nUniqueNumber)
		{
			(*itParty)->m_bFormationFlight = FALSE;
			(*itParty)->m_bFormationFlyingPositionType = 0;
			(*itParty)->m_bFormationFlyingType = FLIGHT_FORM_NONE;
			break;
		}
		itParty++;
	}	
	BOOL bResult = FormationFlightSortPosition();
	if(bResult == FALSE)
	{
		DBGOUT("Failed FormationFlight Sort Position\n");
	}	
}

void CClientParty::FormationFlightChangeType(BYTE nFormationFlightType)
{
	m_infoParty.bFormationFlyingType = nFormationFlightType;
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_bFormationFlight == TRUE)
		{
			(*itParty)->m_bFormationFlyingType = nFormationFlightType;
		}
		itParty++;
	}
}

BOOL CClientParty::FormationFlightSortPosition()
{
	int index = 0;
	UINT nPartyMemberUniqueNumber[SIZE_MAX_PARTY_MEMBER-1];

	// 2009. 08. 11 by ckPark 편대 대형 스킬
	// 편대원 위치 초기화
	memset(nPartyMemberUniqueNumber,0x00,sizeof(UINT)*(SIZE_MAX_PARTY_MEMBER-1));
	// end 2009. 08. 11 by ckPark 편대 대형 스킬

	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_bFormationFlight == TRUE)
		{
			(*itParty)->m_bFormationFlyingPositionType = index + 1;
			nPartyMemberUniqueNumber[index] = (*itParty)->m_nUniqueNumber;
			index++;
		}
		itParty++;
	}
	if(index > 0)
	{
		m_bFormationFlight = TRUE;
	}


	// 2009. 08. 11 by ckPark 편대 대형 스킬
	//if(index < SIZE_MAX_PARTY_MEMBER)
	// 편대대형이 자유 비행일때는 보내지 않는다
	if( index < SIZE_MAX_PARTY_MEMBER && m_infoParty.bFormationFlyingType != FLIGHT_FORM_NONE )
	// end 2009. 08. 11 by ckPark 편대 대형 스킬

	{
		MSG_IC_PARTY_ALL_FLIGHT_POSITION sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_IC_PARTY_ALL_FLIGHT_POSITION;
		for (index = 0 ; index < SIZE_MAX_PARTY_MEMBER - 1 ; index++)
		{
			sMsg.AllPartyMemberCharacterUniqueNumber[index] = nPartyMemberUniqueNumber[index];
		}
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// 파티 관련 프로토콜 받았을 때 처리 함수들(I->C)
/// 2004-08-19 jschoi
///////////////////////////////////////////////////////////////////////////////////////////////

void CClientParty::IPartyPutMember(MSG_IC_PARTY_PUT_MEMBER* pMsg)
{// 파티원 한명을 추가하였을 때					MSG_IC_PARTY_PUT_MEMBER	
	PartyInsertMember(pMsg->IMPartyMemberInfo);
}

void CClientParty::IPartyAcceptInviteOk(MSG_IC_PARTY_ACCEPT_INVITE_OK* pMsg)
{// 파티 초대를 수락했을 때						MSG_IC_PARTY_ACCEPT_INVITE_OK
// 2008-12-09 by dgwoo 미션마스터.
	if(m_infoParty.PartyID != pMsg->PartyID &&
		m_nTempMMasterPartyID != pMsg->PartyID)
		return ;
	char message[256];
	if(g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber == pMsg->IMPartyMemberInfo.CharacterUniqueNumber)
	{
		wsprintf(message,STRMSG_C_PARTY_0003);//"편대에 합류 하였습니다."
		g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY,CHAT_TAB_PARTY);
		m_bFormationFlight = FALSE;
		m_infoParty.bPartyType = _PARTYMEMBER;
		MSG_IC_PARTY_GET_ALL_MEMBER sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_IC_PARTY_GET_ALL_MEMBER;
		sMsg.PartyID = m_infoParty.PartyID;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		// 2007-12-10 by dgwoo 캐릭터 상태이거나 비행기가 멈춰(b+enter)있는 상태에서만 인터페이스를 띄운다.
		if(g_pShuttleChild->m_bUnitStop ||
			g_pD3dApp->IsMyShuttleCharacter())
			g_pGameMain->OnGameMainButtonClicked(GAMEMAIN_BUTTON_COMMUNITY);
	}
	else
	{
		if(m_infoParty.bPartyType == _PARTYMASTER)
		{
			MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION sMsg;
			memset(&sMsg,0x00,sizeof(sMsg));
			char buffer[SIZE_MAX_PACKET];
			int nType = T_IC_PARTY_CHANGE_FLIGHT_FORMATION;
			sMsg.PartyID = m_infoParty.PartyID;
			sMsg.Formation = m_infoParty.bFormationFlyingType;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
			g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

			// 2008-10-20 by bhsohn VOIP 버그 수정
			// VOIP 연결 뿐만 아니라 나 자신도 VOIP가 켜져있냐?
			//if(m_bNtoNCallState)
			//if(m_bNtoNCallState && g_pGameMain->GetVoiceType() == VOICE_PARTY)
			//{
			//	INIT_MSG(MSG_IC_VOIP_NtoN_INVITE_USER, T_IC_VOIP_NtoN_INVITE_USER, pS2Msg, buffer);
			//	pS2Msg->nCharacterUniqueNumber = pMsg->IMPartyMemberInfo.CharacterUniqueNumber;
			//	pS2Msg->byIsPartyCall = TRUE;
			//	g_pIMSocket->Write(buffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_USER));
			//}
		}
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		//wsprintf(message,STRMSG_C_PARTY_0004,pMsg->IMPartyMemberInfo.CharacterName);//"[ %s ] 님이 편대에 합류 하였습니다."
		char szCharName[SIZE_MAX_ARENA_FULL_NAME];
		util::strncpy(szCharName, pMsg->IMPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
		// 서버 이름 제거
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
		wsprintf(message,STRMSG_C_PARTY_0004,szCharName);//"[ %s ] 님이 편대에 합류 하였습니다."		
		
		g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
		
		PartyInsertMember(pMsg->IMPartyMemberInfo);
	}
}

void CClientParty::IPartyBanMemberOk(MSG_IC_PARTY_BAN_MEMBER_OK* pMsg)
{// 파티원 한명을 추방하였을 때					MSG_IC_PARTY_BAN_MEMBER_OK
// 2008-12-09 by dgwoo 미션마스터.
	if(pMsg->PartyID != m_infoParty.PartyID	&&
		m_nTempMMasterPartyID != pMsg->PartyID)
		return ;
	char message[256];
	if(pMsg->CharacterUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
	{
		PartyDeleteAll();
		wsprintf(message,STRMSG_C_PARTY_0005);//"편대에서 추방 당하였습니다."
		g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
	}
	else
	{
		char pName[256];
		PartyDeleteMember(pMsg->CharacterUniqueNumber,pName);		
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		// 서버 이름 제거
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), pName);

		wsprintf(message,STRMSG_C_PARTY_0006,pName);//"%s님을 편대에서 추방 하였습니다."
		g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
	}
}


/*******************************************************************************************************************
**
**	파티 탈퇴 메시지 처리.
**
**	Create Info : ??.??.??.
**
**	Update Info : - 파티원이 탈퇴 할때 인피니티 입장에 대한 예외 처리 추가.		2010. 09. 03. by hsLee.
					(인피니티 진입 준비 중이었고 같은 인피니티 방에 맴버라면 파티 탈퇴 메시지 출력 안함.)
**
********************************************************************************************************************/
void CClientParty::IPartyLeaveOk(MSG_IC_PARTY_LEAVE_OK* pMsg)
{// 파티원 한명이 나갔을 때						MSG_IC_PARTY_LEAVE_OK
// 2008-12-09 by dgwoo 미션마스터.

	if(pMsg->PartyID != m_infoParty.PartyID &&
		m_nTempMMasterPartyID != pMsg->PartyID)
		return ;

	char szMessage[256];

	if(pMsg->CharacterUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
	{
		PartyDeleteAll();

		wsprintf(szMessage,STRMSG_C_PARTY_0007);//"편대에서 탈퇴 하였습니다."
		g_pD3dApp->m_pChat->CreateChatChild(szMessage,COLOR_PARTY);

		if( g_pShuttleChild->IsPkState(PK_FVF) )
		{// 전투 중이라면....
			wsprintf(szMessage,STRMSG_C_PARTY_0008);//"전투가 종료 되었습니다."
			g_pD3dApp->m_pChat->CreateChatChild(szMessage,COLOR_PARTY);

			g_pShuttleChild->SetPkState( PK_FVF, FALSE );

			m_vecPeerPartyEnemyInfo.clear();

			CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();

			while(itEnemy != g_pScene->m_mapEnemyList.end())
			{
				itEnemy->second->SetPkState( PK_FVF, FALSE );
				if(g_pShuttleChild->m_pOrderTarget == itEnemy->second )
				{
					g_pShuttleChild->m_pOrderTarget = NULL;
				}
				itEnemy++;
			}
		}

	}
	else
	{
		char pName[256];
			PartyDeleteMember(pMsg->CharacterUniqueNumber,pName);
		
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		// 서버 이름 제거.
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), pName);

		if ( !(pMsg->bMoveToArena 
				&& g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID 
				&& g_pD3dApp->GetInfinityManager()->GetMemberInfoByCName( pName ) ) )
		{
			wsprintf(szMessage, STRMSG_C_PARTY_0009, pName); // 2015-11-03 DevX //"[ %s ] 님이 편대에서 탈퇴 하였습니다."
			g_pD3dApp->m_pChat->CreateChatChild(szMessage,COLOR_PARTY);
		}
	}
}

void CClientParty::IPartyDismemberOk(MSG_IC_PARTY_DISMEMBER_OK* pMsg)
{// 파티가 해산되었을 때							MSG_IC_PARTY_DISMEMBER_OK
// 2008-12-09 by dgwoo 미션마스터.
	if(pMsg->PartyID != m_infoParty.PartyID&&
		m_nTempMMasterPartyID != pMsg->PartyID)
		return ;

	char message[256];
	PartyDeleteAll();
	wsprintf(message,STRMSG_C_PARTY_0010);//"편대가 해산 되었습니다."
	g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
}

void CClientParty::IPartyPutAllMember(MSG_IC_PARTY_PUT_ALL_MEMBER* pMsg)
{// 모든 파티원 리스트를 받았을 때				MSG_IC_PARTY_PUT_ALL_MEMBER
	char *p = (char *)pMsg;
	p += sizeof(MSG_IC_PARTY_PUT_ALL_MEMBER);
	int index =0;
	m_infoParty.PartyID = pMsg->PartyID;
	m_infoParty.nMasterUniqueNumber = pMsg->MasterUniqueNumber;
	for(int i = 0;i < pMsg->nNumOfPartyMembers; i++)
	{
		IM_PARTY_MEMBER_INFO partyinfo;
		memcpy(&partyinfo,p,sizeof(IM_PARTY_MEMBER_INFO));

		PartyInsertMember(partyinfo);

		p += sizeof(IM_PARTY_MEMBER_INFO);
	}
}

void CClientParty::IPartyPutLastPartyInfo(MSG_IC_PARTY_PUT_LAST_PARTY_INFO* pMsg)
{// 기존의 파티가 있을 때						MSG_IC_PARTY_PUT_LAST_PARTY_INFO
	char message[256];
	m_infoParty.PartyID = pMsg->PartyID;
	m_infoParty.bPartyType = _PARTYMEMBER;
	wsprintf(message,STRMSG_C_PARTY_0011);//"기존 편대에 합류 하였습니다."
	g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
	MSG_IC_PARTY_GET_ALL_MEMBER sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_GET_ALL_MEMBER;
	sMsg.PartyID = m_infoParty.PartyID;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CClientParty::IPartyTransferMasterOk(MSG_IC_PARTY_TRANSFER_MASTER_OK* pMsg)
{// 파티 마스터가 변경되었을 때					MSG_IC_PARTY_TRANSFER_MASTER_OK
	if(NULL == g_pShuttleChild)
		return;

	char message[256];

	// 2011-08-30 by hsson 인파3차에 나오면서 파티 마스터가 변경 될때 예외처리
	char pName[256] = {0, };
	// end 2011-08-30 by hsson 인파3차에 나오면서 파티 마스터가 변경 될때 예외처리

	PartyChangeMaster(pMsg->NewMasterCharacterUniqueNumber, pMsg->OldMasterCharacterUniqueNumber, pName);

	// 2011-08-30 by hsson 인파3차에 나오면서 파티 마스터가 변경 될때 예외처리
	if( NULL == pName )
	{
		const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{
			g_pGameMain->CreateChatChild_OperationMode(
				"ERR:파티마스터를 변경하려는데 정보가 없음(I try to change the party, no information on the master)",
				COLOR_ERROR);
		}
	}
	else
	{
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		// 서버 이름 제거
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), pName);
		wsprintf(message,STRMSG_C_PARTY_0012,pName);//"편대장이 %s 님으로 변경 되었습니다."
		g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
		
		// 2008-06-17 by bhsohn 편대 관련 처리
		if(m_infoParty.bPartyType == _PARTYMASTER)
		{
			// 내가 위임된 편대장이면 편대 정보를 갱신시킨다.
			SPARTY_INFO* pPartyInfo = GetSPartyInfo();
			ChagePartyFormation(pPartyInfo->FormationType);
		}
	}
	// end 2011-08-30 by hsson 인파3차에 나오면서 파티 마스터가 변경 될때 예외처리

}

void CClientParty::IPartyMemberInvalidated(MSG_IC_PARTY_MEMBER_INVALIDATED* pMsg)
{// 잘못된 파티원이 추가되었을 때?				MSG_IC_PARTY_MEMBER_INVALIDATED
//	char pName[256];
//	PartyDeleteMember(pMsg->CharacterUniqueNumber,pName);
	
	// 2008-12-02 by bhsohn 편대원 온오프라인 상태체크
	PARTYENEMYINFO* pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo)
	{
		// 유저 로그오프
		pPartyEnemyInfo->m_bUserLogOn = FALSE;

		// 2009. 08. 11 by ckPark 편대 대형 스킬
		// 유저가 나가면 항상 편대대형에서 나가게 된다
		pPartyEnemyInfo->m_bFormationFlight = FALSE;
		// end 2009. 08. 11 by ckPark 편대 대형 스킬

		// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
		// 파티원이 나가면 디버프 정보 초기화
		pPartyEnemyInfo->m_vecDebuffDotInfo.clear();
		// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	}
	// end 2008-12-02 by bhsohn 편대원 온오프라인 상태체크
}

void CClientParty::IPartyMemberRejoined(MSG_IC_PARTY_MEMBER_REJOINED* pMsg)
{// 파티원이 다시 접속했을 때?					MSG_IC_PARTY_MEMBER_REJOINED
	PARTYENEMYINFO* pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo)
	{
		pPartyEnemyInfo->m_bFormationFlight = FALSE;
		if( pPartyEnemyInfo->m_pEnemyData &&
			pMsg->MapChannelIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex)
		{
			pPartyEnemyInfo->m_bPartyType = _PARTYMEMBER;
		}
		else
		{
			pPartyEnemyInfo->m_bPartyType = _PARTYOTHERMAPMEMBER;
		}
		
		// 2008-12-02 by bhsohn 편대원 온오프라인 상태체크
		pPartyEnemyInfo->m_bUserLogOn = TRUE;
		// end 2008-12-02 by bhsohn 편대원 온오프라인 상태체크

		char buf[256];
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		//wsprintf(buf,STRMSG_C_PARTY_0013,pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName);//"%s 님이 접속 하였습니다."
		char szCharName[SIZE_MAX_ARENA_FULL_NAME];
		util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
		// 서버 이름 제거
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
		wsprintf(buf,STRMSG_C_PARTY_0013,szCharName);//"%s 님이 접속 하였습니다."
		
		g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ITEM);


		// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
		{
			// 파티원이 재접속했을 때, 디버프 정보 요청
			MSG_FC_CHARACTER_DEBUFF_DOT_INFO sMsg;
			memset( &sMsg, 0, sizeof( sMsg ) );
			sMsg.CharacterUID = pMsg->CharacterUniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_DEBUFF_DOT_INFO, (char*)&sMsg, sizeof(sMsg) );
		}
		// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

	}
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
	{// 파티장이면 재접속한 파티원에게 편대 포메이션 정보 전송
		MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_IC_PARTY_CHANGE_FLIGHT_FORMATION;
		sMsg.PartyID = m_infoParty.PartyID;
		sMsg.Formation = m_infoParty.bFormationFlyingType;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		//if(m_bNtoNCallState)
		//{
		//	INIT_MSG(MSG_IC_VOIP_NtoN_INVITE_USER, T_IC_VOIP_NtoN_INVITE_USER, pS2Msg, buffer);
		//	pS2Msg->nCharacterUniqueNumber = pMsg->CharacterUniqueNumber;
		//	pS2Msg->byIsPartyCall = TRUE;
		//	g_pIMSocket->Write(buffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_USER));
		//}
	}
}

void CClientParty::IPartyUpdateMemberInfoMapname(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME* pMsg)
{// ?											MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME
	PARTYENEMYINFO* pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	// 2013-02-06 by bhsohn 파티 관련 메모리 버그 수정
	if(pPartyEnemyInfo)
	{
		pPartyEnemyInfo->m_ImPartyMemberInfo.MapChannelIndex = pMsg->MapChannelIndex;
	}
	/*
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		if(itEnemy->second->m_infoCharacter.CharacterUniqueNumber == pMsg->CharacterUniqueNumber)
		{
			itEnemy->second->m_fLifeTime = 2.0f;
			itEnemy->second->m_infoCharacter.MapChannelIndex = pMsg->MapChannelIndex;
			if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY)
			{
				if( pMsg->MapChannelIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex )
				{// 같은 맵
					// 같은 맵은 정보를 요청한다.
					MSG_FC_PARTY_GET_MEMBER sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					char buffer[SIZE_MAX_PACKET];
					sMsg.CharacterUniqueNumber = itEnemy->second->m_infoCharacter.CharacterUniqueNumber;
					int nType = T_FC_PARTY_GET_MEMBER;
					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
					g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
				}
				else
				{// 다른 맵
					
				}
			}
			if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
			{// 파티장이면 재접속한 파티원에게 편대 포메이션 정보 전송
				MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				char buffer[SIZE_MAX_PACKET];
				int nType = T_IC_PARTY_CHANGE_FLIGHT_FORMATION;
				sMsg.PartyID = m_infoParty.PartyID;
				sMsg.Formation = m_infoParty.bFormationFlyingType;
				memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
				memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
				g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
			}
			break;
		}
		itEnemy++;
	}*/
}

void CClientParty::IPartyUpdateItemPos(MSG_IC_PARTY_UPDATE_ITEM_POS* pMsg)
{
	PARTYENEMYINFO * pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);	
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		
		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		//pPartyEnemyInfo->m_pEnemyData->ChangeItemPoint(pMsg->ItemPosition,pMsg->ItemNum, pMsg->ColorCode);
		pPartyEnemyInfo->m_pEnemyData->ChangeItemPoint( pMsg->ItemPosition,pMsg->ItemNum, pMsg->nShapeItemNum, pMsg->nEffectItemNum );
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CharacterRenderInfo.SetRenderInfoWithPOS( pMsg->ItemPosition,pMsg->ItemNum, pMsg->nShapeItemNum, pMsg->nEffectItemNum );
		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	}	
}

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
void	CClientParty::IPartyGetAutoPartyInfoOK( MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK* pMsg )
{
	if( m_infoParty.PartyID	== pMsg->PartyID )
	{
		m_infoParty.nMasterUniqueNumber	= pMsg->MasterUniqueNumber;
		m_infoMaster.m_nUniqueNumber	= pMsg->MasterUniqueNumber;

		// 자신이 파티장인가?
		if( g_pShuttleChild->GetShuttleInfo()->CharacterUniqueNumber == m_infoParty.nMasterUniqueNumber )
			m_infoParty.bPartyType	= _PARTYMASTER;
		else
			m_infoParty.bPartyType	= _PARTYMEMBER;

		memcpy( &m_stPartyInfo, &(pMsg->PartyInfo), sizeof( SPARTY_INFO ) );

		// 파티멤버수만큼 추가
		char* pTemp = ((char*)pMsg) + sizeof( MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK );
		for( int i=0; i<pMsg->nNumOfPartyMembers; ++i )
		{
			PartyInsertMember( *((IM_PARTY_MEMBER_INFO*)(pTemp)) );
			pTemp += sizeof( IM_PARTY_MEMBER_INFO );
		}
	}
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

///////////////////////////////////////////////////////////////////////////////////////////////
/// 파티 관련 프로토콜 받았을 때 처리 함수들(F->C)
/// 2004-08-19 jschoi
///////////////////////////////////////////////////////////////////////////////////////////////

void CClientParty::FPartyBattleEnd(MSG_FC_PARTY_BATTLE_END* pMsg)
{// 파티전이 끝났을 때							MSG_FC_PARTY_BATTLE_END

	// 2004-12-15 by jschoi - 우리 편대와 파티전 중인 다른 파티 리스트를 초기화
	m_vecPeerPartyEnemyInfo.clear();

	char message[256];
	switch(pMsg->EndType)
	{
	case BATTLE_END_WIN:
		{
			wsprintf(message,STRMSG_C_PARTY_0014);//"편대 전투에서 승리하였습니다."
		}
		break;
	case BATTLE_END_DEFEAT:
		{
			wsprintf(message,STRMSG_C_PARTY_0015);//"편대 전투에서 패배하였습니다."
		}
		break;
	case BATTLE_END_TIE:
		{
			wsprintf(message,STRMSG_C_PARTY_0016);//"편대 전투에서 승부를 가리지 못하였습니다."
		}
		break;
	case BATTLE_END_END:
		{
			wsprintf(message,STRMSG_C_PARTY_0017);//"편대 전투가 종료 되었습니다."
		}
		break;
	}
	g_pShuttleChild->SetPkState( PK_FVF, FALSE );
	g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		itEnemy->second->SetPkState( PK_FVF, FALSE );
		if(g_pShuttleChild->m_pOrderTarget == itEnemy->second)
		{
			g_pShuttleChild->m_pOrderTarget = NULL;
		}
		itEnemy++;
	}
	g_pGameMain->m_pCommunity->GetParty()->SetEnablePartyMenu(TRUE);
}

void CClientParty::FPartyBattleReady(MSG_FC_PARTY_BATTLE_START* pMsg)
{// 파티전이 시작되었을 때						MSG_FC_PARTY_BATTLE_READY
	PEER_PARTY_MEMBER *pPeerMember = (PEER_PARTY_MEMBER *)((char *)pMsg + sizeof(MSG_FC_PARTY_BATTLE_START));
	// 편대전 시 메뉴 사용 불가 세팅
	g_pGameMain->m_pCommunity->GetParty()->SetEnablePartyMenu(FALSE);
	for(int i=0;i<pMsg->nPeerPartyMemberToBattle;i++, pPeerMember++)
	{
		// 2004-12-15 by jschoi - 우리 편대와 파티전 중인 다른 파티 리스트를 작성
		m_vecPeerPartyEnemyInfo.push_back(*pPeerMember);

		CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.find(pPeerMember->ClientIndex);
		if(itEnemy != g_pScene->m_mapEnemyList.end())
		{
			itEnemy->second->SetPkState( PK_FVF, TRUE );
		}
//		else
//		{
//			CEnemyData * pEnemy = new CEnemyData();
//			g_pScene->InsertToBlockData(pEnemy);
//			pEnemy->m_infoCharacter.CharacterInfo.ClientIndex = pPeerMember->ClientIndex;
//			pEnemy->m_infoCharacter.CharacterInfo.CharacterUniqueNumber = pPeerMember->CharacterUniqueNumber;
//			pEnemy->SetPkState( PK_FVF, TRUE );
//			g_pScene->m_mapEnemyList[pEnemy->m_infoCharacter.CharacterInfo.ClientIndex] = pEnemy;
//
//			MSG_FC_CHARACTER_GET_OTHER_INFO sMsg; 
//			memset(&sMsg,0x00,sizeof(sMsg));
//			char buffer[SIZE_MAX_PACKET];
//			sMsg.ClientIndex = pEnemy->m_infoCharacter.CharacterInfo.ClientIndex;
//			int nType = T_FC_CHARACTER_GET_OTHER_INFO;
//			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//			g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		}
	}
	g_pShuttleChild->SetPkState(PK_FVF, TRUE);
// 2004-10-20 by ydkim, 카운트 다운으로 처리
//	g_pD3dApp->m_pChat->CreateChatChild("상대방이 편대 전투를 승낙하였습니다.",COLOR_PARTY);
	g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0018,COLOR_PARTY);//"편대전투가 시작 되었습니다."
}

void CClientParty::FPartyCreateOk(MSG_FC_PARTY_CREATE_OK* pMsg)
{// 파티를 생성했을 때							MSG_FC_PARTY_CREATE_OK
	m_infoParty.nMasterUniqueNumber = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
	m_infoParty.bPartyType = _PARTYMASTER;
	// 편대 비행 초기형태 종형
	m_infoParty.bFormationFlyingType = FLIGHT_FORM_BELL;
	m_infoParty.PartyID = pMsg->PartyID;
	// 2008-06-16 by dgwoo
	m_bParty = TRUE;
	g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0019,COLOR_PARTY);//"편대 그룹을 생성 하였습니다"
}

void CClientParty::FPartyPutItemOther(MSG_FC_PARTY_PUT_ITEM_OTHER* pMsg)
{// 파티원이 아이템을 획득했을 때				MSG_FC_PARTY_PUT_ITEM_OTHER
	if(IS_VALID_CLIENT_INDEX(pMsg->ClientIndex))
	{
		PARTYENEMYINFO * pPartyEnemyInfo = PartyFindMemberByClientIndex(pMsg->ClientIndex);
		if(pPartyEnemyInfo)
		{
			ITEM *item = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			char buf[256];
			if(item)
			{
				if(IS_COUNTABLE_ITEM(item->Kind))// == ITEMKIND_ENERGY)
				{
					// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
					//wsprintf(buf,STRMSG_C_PARTY_0020,pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName,item->ItemName,pMsg->Amount);//"%s 님이 %s(%d)을 습득 하였습니다."
					char szCharName[SIZE_MAX_ARENA_FULL_NAME];
					util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
					// 서버 이름 제거
					g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
					wsprintf(buf,STRMSG_C_PARTY_0020,szCharName,item->ItemName,pMsg->Amount);//"%s 님이 %s(%d)을 습득 하였습니다."
					
					g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ITEM);
				}
				else
				{
					// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
					//wsprintf(buf,STRMSG_C_PARTY_0021,pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName,item->ItemName);//"%s 님이 %s을 습득 하였습니다."
					char szCharName[SIZE_MAX_ARENA_FULL_NAME];
					util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
					// 서버 이름 제거
					g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);

					wsprintf(buf,STRMSG_C_PARTY_0021,szCharName,item->ItemName);//"%s 님이 %s을 습득 하였습니다."
					
					g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ITEM);
				}
			}
		}
	}
}

void CClientParty::FPartyPutMember(MSG_FC_PARTY_PUT_MEMBER* pMsg)
{// 파티원이 추가됐을 때	? 위에는?				MSG_FC_PARTY_PUT_MEMBER
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->FieldMemberInfo.CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CharacterInfo.ClientIndex = pMsg->FieldMemberInfo.ClientIndex;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.HP = pMsg->FieldMemberInfo.HP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentHP = pMsg->FieldMemberInfo.CurrentHP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.DP = pMsg->FieldMemberInfo.DP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentDP = pMsg->FieldMemberInfo.CurrentDP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.EP = pMsg->FieldMemberInfo.EP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentEP = pMsg->FieldMemberInfo.CurrentEP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.SP = pMsg->FieldMemberInfo.SP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentSP = pMsg->FieldMemberInfo.CurrentSP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.MapChannelIndex = pMsg->FieldMemberInfo.MapChannelIndex;
	}
}

void CClientParty::FPartyRejectInviteOk(MSG_FC_PARTY_REJECT_INVITE_OK* pMsg)
{// 파티 초대를 거절했을 때						MSG_FC_PARTY_REJECT_INVITE_OK
	char message[256];
	// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
	//wsprintf(message,STRMSG_C_PARTY_0022,pMsg->CharacterName);//"[ %s ] 님이 편대 그룹 초대를 거절 하였습니다"
	char szCharName[SIZE_MAX_ARENA_FULL_NAME];
	util::strncpy(szCharName, pMsg->CharacterName, SIZE_MAX_ARENA_FULL_NAME);
	// 서버 이름 제거
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
	wsprintf(message,STRMSG_C_PARTY_0022,szCharName);//"[ %s ] 님이 편대 그룹 초대를 거절 하였습니다"

	g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_PARTY);
}

void CClientParty::FPartyRequestInviteQuestion(MSG_FC_PARTY_REQUEST_INVITE_QUESTION* pMsg)
{// ?											MSG_FC_PARTY_REQUEST_INVITE_QUESTION
	// 2006-11-07 by ispark, 거절 리스트에 있는 사람이 나한테 걸었을 때
	vector<Reject_t>::iterator itReject = g_pGameMain->m_pCommunity->GetReject()->m_vecReject.begin();
	while(itReject != g_pGameMain->m_pCommunity->GetReject()->m_vecReject.end())
	{
		if(0 == stricmp(pMsg->MasterCharacterName, itReject->szRejectName))
		{
			g_pShuttleChild->m_pClientParty->FSendPartyRejectInvite(pMsg->PartyID);
			return;
		}
		itReject++;			
	}
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	if(!g_pD3dApp->IsOptionEtc(OPTION_RADIO_FORMATION_INVITE))
	{
		// 편대 초대 사용안함
		g_pShuttleChild->m_pClientParty->FSendPartyRejectInvite(pMsg->PartyID);
		return;
	}
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리
	
	if(m_infoParty.bPartyType == _NOPARTY)
	{
		g_pGameMain->m_pCommunity->GetParty()->PartyQuestionJoin(pMsg->MasterCharacterName, pMsg->PartyID);
	}
}

void CClientParty::FPartyUpdateMemberInfoAll(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL* pMsg)
{// 파티원 한명의 모든 정보를 받았을 때			MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.Level = pMsg->Level;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.HP = pMsg->HP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentHP = pMsg->CurrentHP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.DP = pMsg->DP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentDP = pMsg->CurrentDP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.EP = pMsg->EP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentEP = pMsg->CurrentEP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.SP = pMsg->SP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentSP = pMsg->CurrentSP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CharacterInfo.BodyCondition = pMsg->BodyCondition;
	}
}

void CClientParty::FPartyUpdateMemberInfoBodyCondition(MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION* pMsg)
{// 파티원 한명의 바디컨디션을 받았을 때			MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
	//	pPartyEnemyInfo->m_pEnemyData->ChangeBodyConditionFromServer(pMsg->BodyCondition);
		DBGOUT("Update Party Member Bodycondition \n");
	}
}

void CClientParty::FPartyUpdateMemberInfoCurrentDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentDP = pMsg->CurrentDP;
		if(pMsg->CurrentDP > 0)
		{
			pPartyEnemyInfo->m_pEnemyData->m_bShielding = TRUE;
		}
		else
		{
			pPartyEnemyInfo->m_pEnemyData->m_bShielding = FALSE;
		}
	}
}

void CClientParty::FPartyUpdateMemberInfoCurrentEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentEP = pMsg->CurrentEP;
	}
}

void CClientParty::FPartyUpdateMemberInfoCurrentHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentHP = pMsg->CurrentHP;
	}
}

void CClientParty::FPartyUpdateMemberInfoCurrentSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentSP = pMsg->CurrentSP;
	}
}

void CClientParty::FPartyUpdateMemberInfoDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.DP = pMsg->DP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentDP = pMsg->CurrentDP;
	}
}

void CClientParty::FPartyUpdateMemberInfoEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.EP = pMsg->EP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentEP = pMsg->CurrentEP;
	}
}

void CClientParty::FPartyUpdateMemberInfoHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.HP = pMsg->HP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentHP = pMsg->CurrentHP;
	}
}

void CClientParty::FPartyUpdateMemberInfoSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP* pMsg)
{//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.SP = pMsg->SP;
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.CurrentSP = pMsg->CurrentSP;
	}
}

void CClientParty::FPartyUpdateMemberInfoLevel(MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL* pMsg)
{// 파티원 한명의 레벨정보를 받았을 때			MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL
	PARTYENEMYINFO * pPartyEnemyInfo;
	pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->CharacterUniqueNumber);
	if(pPartyEnemyInfo && pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_infoCharacter.Level = pMsg->Level;
	}
}


// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
void	CClientParty::FPartyUpdateMemberInfoDebuffDotInfo( MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK* pMsg )
{
	// 파티원을 찾는다
	PARTYENEMYINFO* pPartyEnemyinfo = PartyFindMemberByUniqueNumber( pMsg->CharacterUID );
	if( pPartyEnemyinfo )
	{
		// 파티원 디버프 정보 초기화
		pPartyEnemyinfo->m_vecDebuffDotInfo.clear();

		// 디버프 정보 추가
		int i;
		char* pTemp = (char*)(pMsg) + sizeof( MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK );
		for( i=0; i<pMsg->DebuffCount; ++i )
		{
			MSG_DEBUFF_INFO* pDeBuff = (MSG_DEBUFF_INFO*)(pTemp + ( sizeof( MSG_DEBUFF_INFO ) * i ));

			ITEM* pItem = g_pDatabase->GetServerItemInfo( pDeBuff->ItemNum );
			if( pItem )
				pPartyEnemyinfo->m_vecDebuffDotInfo.push_back( sDebuffDotInfo( pItem ) );
		}

		// 도트 정보 추가
		pTemp = (char*)(pMsg) + sizeof( MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK ) + ( sizeof( MSG_DEBUFF_INFO ) * pMsg->DebuffCount );
		for( i=0; i<pMsg->DotCount; ++i )
		{
			MSG_DOT_INFO* PDot = (MSG_DOT_INFO*)(pTemp + ( sizeof( MSG_DOT_INFO ) * i ));

			ITEM* pItem = g_pDatabase->GetServerItemInfo( PDot->ItemNum );
			if( pItem )
				pPartyEnemyinfo->m_vecDebuffDotInfo.push_back( sDebuffDotInfo( pItem ) );
		}
	}
}

void	CClientParty::FPartyUpdateMemberInfoDebuffDotApply( MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING* pMsg )
{
	PARTYENEMYINFO* pPartyEnemyinfo = PartyFindMemberByUniqueNumber( pMsg->CharacterUID );
	if( pPartyEnemyinfo )
	{
		ITEM* pItem = g_pDatabase->GetServerItemInfo( pMsg->SkillItemNum );
		if( pItem )
		{
			std::vector<sDebuffDotInfo>::iterator it;
			for( it = pPartyEnemyinfo->m_vecDebuffDotInfo.begin();
				 it != pPartyEnemyinfo->m_vecDebuffDotInfo.end();
				 ++it )
			{
				if( (*it).nSkillItemNum == pMsg->SkillItemNum )
					break;
			}

			if( it == pPartyEnemyinfo->m_vecDebuffDotInfo.end() )
				pPartyEnemyinfo->m_vecDebuffDotInfo.push_back( sDebuffDotInfo( pItem ) );
		}
	}
}

void	CClientParty::FPartyUpdateMemberInfoDebuffDotRelease( MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE* pMsg )
{
	PARTYENEMYINFO* pPartyEnemyinfo = PartyFindMemberByUniqueNumber( pMsg->CharacterUID );
	if( pPartyEnemyinfo )
	{
		ITEM* pItem = g_pDatabase->GetServerItemInfo( pMsg->SkillItemNum );
		if( pItem )
		{
			std::vector<sDebuffDotInfo>::iterator it;
			for( it = pPartyEnemyinfo->m_vecDebuffDotInfo.begin();
				 it != pPartyEnemyinfo->m_vecDebuffDotInfo.end();
				 ++it )
			{
				if( (*it).nSkillItemNum == pMsg->SkillItemNum )
					break;
			}

			if( it != pPartyEnemyinfo->m_vecDebuffDotInfo.end() )
				pPartyEnemyinfo->m_vecDebuffDotInfo.erase( it );
		}
	}
}
// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
void	CClientParty::FPartyAutoCreateOK( MSG_FC_PARTY_AUTO_CREATED* pMsg )
{
	m_bParty			= TRUE;
	m_bFormationFlight	= FALSE;
	m_infoParty.PartyID	= pMsg->PartyID;
	m_infoParty.bFormationFlyingPositionType	= 0;
	m_infoParty.bFormationFlyingType			= FLIGHT_FORM_NONE;

	// 파티원 정보 요청
	MSG_IC_PARTY_GET_AUTO_PARTY_INFO msg;
	msg.PartyID	= m_infoParty.PartyID;

	g_pIMSocket->SendMsg( T_IC_PARTY_GET_AUTO_PARTY_INFO, (char*)(&msg), sizeof( MSG_IC_PARTY_GET_AUTO_PARTY_INFO ) );
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


///////////////////////////////////////////////////////////////////////////////////////////////
/// 편대비행 관련 프로토콜 받았을 때 처리 함수들(I->C)
/// 2004-08-19 jschoi
///////////////////////////////////////////////////////////////////////////////////////////////

void CClientParty::IPartyAllFlightPosition(MSG_IC_PARTY_ALL_FLIGHT_POSITION* pMsg)
{// 모든 편대원의 편대비행 포지션을 받을 때		MSG_IC_PARTY_ALL_FLIGHT_POSITION
	if(m_bFormationFlight == FALSE)
	{// 혹시라도 내가 편대비행중이 아니라면
		return;
	}
	FormationFlightClear();
	PARTYENEMYINFO* pPartyEnemyInfo;
	for ( int index = 0 ; index < SIZE_MAX_PARTY_MEMBER-1 ; index++)
	{
		if(g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber == pMsg->AllPartyMemberCharacterUniqueNumber[index])
		{
			m_bFormationFlight = TRUE;
			m_infoParty.bFormationFlyingPositionType = index + 1;
		}
		else
		{
			pPartyEnemyInfo = PartyFindMemberByUniqueNumber(pMsg->AllPartyMemberCharacterUniqueNumber[index]);
			if(pPartyEnemyInfo)
			{
				pPartyEnemyInfo->m_bFormationFlight = TRUE;
				pPartyEnemyInfo->m_bFormationFlyingPositionType = index + 1;
			}
		}
	}
}

void CClientParty::IPartyGetFlightPosition(MSG_IC_PARTY_GET_FLIGHT_POSITION* pMsg)
{// 편대원이 추가되었을 때						MSG_IC_PARTY_GET_FLIGHT_POSITION
	FormationFlightInsertMember(pMsg->CharacterUniqueNumber);
}

void CClientParty::IPartyCancelFlightPosition(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION* pMsg)
{// 편대원이 나갔을 때							MSG_IC_PARTY_CANCEL_FLIGHT_POSITION
	if(m_infoParty.bPartyType == _PARTYMASTER)
	{
		FormationFlightDeleteMember(pMsg->CharacterUniqueNumber);
	}

	// 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
	if( FORMATION_SKILL_OFF == pMsg->Formation_On_Off )
	{
		m_bFormationFlight = FALSE;
		m_infoParty.bFormationFlyingPositionType = 0;
	}
	//end 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
}

void CClientParty::IPartyChangeFlightFormationOk(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK* pMsg)
{// 편대비행 타입이 변경되었을 때				MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK
	m_infoParty.bFormationFlyingType = pMsg->Formation;
}

void CClientParty::IPartyChangeFlightPosition(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION* pMsg)
{// ?											MSG_IC_PARTY_CHANGE_FLIGHT_POSITION
//	DBGOUT("이 프로토콜을 사용하는지 확인하는 코드임 보이면 사용됨.\n");
}


///////////////////////////////////////////////////////////////////////////////////////////////
/// 편대비행 관련 프로토콜 받았을 때 처리 함수들(I->C)
/// 2004-08-19 jschoi
///////////////////////////////////////////////////////////////////////////////////////////////

void CClientParty::FSendPartyRequestPartyWarp()
{// 파티 워프를 서버로 요청
	int index = 0;
	UINT nPartyMemberUniqueNumber[SIZE_MAX_PARTY_MEMBER-1];
	memset(nPartyMemberUniqueNumber,0x00,sizeof(UINT)*(SIZE_MAX_PARTY_MEMBER-1));
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_bFormationFlight == TRUE)
		{
			nPartyMemberUniqueNumber[index] = (*itParty)->m_nUniqueNumber;
			index++;
		}
		itParty++;
	}

	char buffer[SIZE_MAX_PACKET];
	MSG_FC_PARTY_REQUEST_PARTY_WARP sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	memset(buffer,0x00,SIZE_MAX_PACKET);
	int nType = T_FC_PARTY_REQUEST_PARTY_WARP;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	int cont = 0;
	for(int i = 0; i < MAX_PARTY_NUMBER - 1; i++)
	{
		if(nPartyMemberUniqueNumber[i])
		{
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER+sizeof(sMsg)+(cont)*sizeof(UINT), &nPartyMemberUniqueNumber[i], sizeof(UINT));
			cont++;
		}
	}
	sMsg.nPartyMembers = cont;
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + cont*sizeof(UINT));		
}

void CClientParty::FSendPartyRequestPartyObjectEvent(CObjectChild * pObj)
{// 파티 오브젝트 이벤트를 서버로 요청
	int index = 0;
	UINT nPartyMemberUniqueNumber[SIZE_MAX_PARTY_MEMBER-1];
	memset(nPartyMemberUniqueNumber,0x00,sizeof(UINT)*(SIZE_MAX_PARTY_MEMBER-1));
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_bFormationFlight == TRUE)
		{
			nPartyMemberUniqueNumber[index] = (*itParty)->m_nUniqueNumber;
			index++;
		}
		itParty++;
	}
	
	char buffer[SIZE_MAX_PACKET];
	MSG_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	memset(buffer,0x00,SIZE_MAX_PACKET);
	sMsg.ObjectType = pObj->m_nCode;
	sMsg.ObjectPosition = pObj->m_vPos;
	int nType = T_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	int cont = 0;
	for(int i = 0; i < MAX_PARTY_NUMBER - 1; i++)
	{
		if(nPartyMemberUniqueNumber[i])
		{
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER+sizeof(sMsg)+(cont)*sizeof(UINT), &nPartyMemberUniqueNumber[i], sizeof(UINT));
			cont++;
		}
	}
	sMsg.nPartyMembers = cont;
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + cont*sizeof(UINT));
}

void CClientParty::ISendPartyChangeFlightFormation(int nFormationFlightType)
{
	MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_CHANGE_FLIGHT_FORMATION;
	sMsg.PartyID = m_infoParty.PartyID;
	sMsg.Formation = nFormationFlightType;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

	//FormationFlightSortPosition();
}

void CClientParty::FSendEventSelectChannelWithParty(MSG_FC_EVENT_SELECT_CHANNEL msgEventSelectChannel)
{// 편대워프 이벤트시 편대원과함께 서버로 요청
	int index = 0;
	UINT nPartyMemberUniqueNumber[SIZE_MAX_PARTY_MEMBER-1];
	memset(nPartyMemberUniqueNumber,0x00,sizeof(UINT)*(SIZE_MAX_PARTY_MEMBER-1));
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_bFormationFlight == TRUE)
		{
			nPartyMemberUniqueNumber[index] = (*itParty)->m_nUniqueNumber;
			index++;
		}
		itParty++;
	}
	INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY, T_FC_EVENT_SELECT_CHANNEL_WITH_PARTY, sMsg, buffer);
	sMsg->ClientIndex		= msgEventSelectChannel.ClientIndex;
	sMsg->WarpTargetIndex	= msgEventSelectChannel.WarpTargetIndex;
	sMsg->MapChannelIndex	= msgEventSelectChannel.MapChannelIndex;
	int cont = 0;
	for(int i=0;i<MAX_PARTY_NUMBER - 1;i++)
	{
		if(nPartyMemberUniqueNumber[i])
		{
			memcpy(buffer+MSG_SIZE(MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY)+i*sizeof(UINT), &nPartyMemberUniqueNumber[i], sizeof(UINT));
			cont++;
		}
	}
	sMsg->nPartyMembers = cont;	
	g_pFieldWinSocket->Write(buffer, MSG_SIZE(MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY) + cont*sizeof(UINT));
	
	FormationFlightClear();	
}

void CClientParty::FSendPartyRequestPartyWarpWithMapName(char* pName)
{
	int index = 0;
	UINT nPartyMemberUniqueNumber[SIZE_MAX_PARTY_MEMBER-1];
	memset(nPartyMemberUniqueNumber,0x00,sizeof(UINT)*(SIZE_MAX_PARTY_MEMBER-1));
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_bFormationFlight == TRUE)
		{
			nPartyMemberUniqueNumber[index] = (*itParty)->m_nUniqueNumber;
			index++;
		}
		itParty++;
	}
	MSG_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	memset(buffer,0x00,SIZE_MAX_PACKET);
	int nType = T_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	int cont = 0;
	for(int i=0;i<MAX_PARTY_NUMBER - 1;i++)
	{
		if(nPartyMemberUniqueNumber[i])
		{
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER+sizeof(sMsg)+(cont)*sizeof(UINT), &nPartyMemberUniqueNumber[i], sizeof(UINT));
			cont++;
		}
	}
	sMsg.MapChannelIndex.MapIndex = atoi(pName);
	sMsg.nPartyMembers = cont;
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + cont*sizeof(UINT));	
}

void CClientParty::ISendPartyGetFlightPosition()
{
	// 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)
	//if( g_cTimeMgr.CheckTime( 1, 200 ) )
	{
		MSG_IC_PARTY_GET_FLIGHT_POSITION sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_IC_PARTY_GET_FLIGHT_POSITION;
		sMsg.CharacterUniqueNumber = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		m_infoParty.bFormationFlyingPositionType = 9;
		m_bFormationFlight = TRUE;
	}
	// end 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)
}

void CClientParty::FSendPartyAcceptInvite(PartyID_t	nPartyRequestID)
{
	MSG_FC_PARTY_ACCEPT_INVITE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_PARTY_ACCEPT_INVITE;
	sMsg.PartyID = nPartyRequestID;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	m_infoParty.PartyID = nPartyRequestID;	
}

void CClientParty::FSendPartyRejectInvite(PartyID_t nPartyRequestID)
{
	MSG_FC_PARTY_REJECT_INVITE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_PARTY_REJECT_INVITE;
	sMsg.PartyID = nPartyRequestID;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	m_infoParty.PartyID = 0;	
}

void CClientParty::ISendPartyLeave() const
{
	MSG_IC_PARTY_LEAVE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_LEAVE;
	sMsg.PartyID = m_infoParty.PartyID;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CClientParty::ISendPartyBanMember(LONGLONG nUniqueNumber) const
{
	MSG_IC_PARTY_BAN_MEMBER sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_BAN_MEMBER;
	sMsg.PartyID = m_infoParty.PartyID;
	sMsg.CharacterUniqueNumber = nUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CClientParty::ISendPartyTransferMaster(LONGLONG nUniqueNumber) const
{
	MSG_IC_PARTY_TRANSFER_MASTER sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_TRANSFER_MASTER;
	sMsg.PartyID = m_infoParty.PartyID;
	sMsg.NewMasterCharacterUniqueNumber = nUniqueNumber;
	sMsg.OldMasterCharacterUniqueNumber = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CClientParty::ISendPartyCreate()
{
	MSG_IC_PARTY_CREATE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_IC_PARTY_CREATE;
	sMsg.CharacterUniqueNumber = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CClientParty::FSendPartyRequestInvite(const char* pName)
{
	MSG_FC_PARTY_REQUEST_INVITE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	strcpy(sMsg.CharacterName,pName);
	int nType = T_FC_PARTY_REQUEST_INVITE;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CClientParty::FormationFlightMoveForShuttleChild()
{
	switch(m_infoParty.bFormationFlyingType)
	{
	case FLIGHT_FORM_2_COLUMN:// 이열 종대 편대
			FormationFlightForShuttleChildType1();
		break;
	case FLIGHT_FORM_2_LINE:// 이열 횡대 편대
			FormationFlightForShuttleChildType2();
		break;
	case FLIGHT_FORM_TRIANGLE:// 삼각 편대
			FormationFlightForShuttleChildType3();
		break;
	case FLIGHT_FORM_INVERTED_TRIANGLE:// 역삼각 편대
			FormationFlightForShuttleChildType4();
		break;
	case FLIGHT_FORM_BELL:// 종 편대
			FormationFlightForShuttleChildType5();
		break;
	case FLIGHT_FORM_INVERTED_BELL:// 역종 편대
			FormationFlightForShuttleChildType6();
		break;
	case FLIGHT_FORM_X:// X-형 편대
			FormationFlightForShuttleChildType7();
		break;
	case FLIGHT_FORM_STAR:// Star-형 편대
			FormationFlightForShuttleChildType8();
		break;
	}
}

void CClientParty::FormationFlightForShuttleChildType1()// 이열종대형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;
	switch(m_infoParty.bFormationFlyingPositionType) //heres bug
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 15.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;//1
		}
		break;
	case 2:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel;
		}
		break;
	case 3:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 45.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;//3
		}
		break;
	case 4:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
		}
		break;
	case 5:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;//5
		}
		break;

	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 15.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide; // 6
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 45.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;//3
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;//5
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 90.0f*m_infoMaster.m_vVel;
	}
	break;

	default:
		{
			// 에러!
			FormationFlightClear();
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;	// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType2()// 이열횡대형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
//	D3DXVec3Normalize(&g_pShuttleChild->m_vUp,&(m_infoMaster.m_vUp));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;

	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 30.0f*m_infoMaster.m_vSide;
		}
		break;
	case 2:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vSide;
		}
		break;
	case 3:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel + 15.0f*m_infoMaster.m_vSide;
		}
		break;
	case 4:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel + 45.0f*m_infoMaster.m_vSide;
		}
		break;
	case 5:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel - 15.0f*m_infoMaster.m_vSide;
		}
		break;
	case 6:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel - 45.0f*m_infoMaster.m_vSide;
		}
	break;

	case 7:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 30.0f*m_infoMaster.m_vSide;
		}
	break;
	case 8:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
		}
	break;
	case 9:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel - 30.0f*m_infoMaster.m_vSide;
		}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();	
		}
	}	
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType3()// 삼각 편대 형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
//	D3DXVec3Normalize(&g_pShuttleChild->m_vUp,&(m_infoMaster.m_vUp));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;

	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 2:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 3:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
		}
		break;
	case 4:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
		}
		break;
	case 5:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel - 40.0f*m_infoMaster.m_vSide;
		}
		break;
	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 60.0f*m_infoMaster.m_vSide;
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel - 60.0f*m_infoMaster.m_vSide;
	}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();	
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType4()// 역삼각 형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
//	D3DXVec3Normalize(&g_pShuttleChild->m_vUp,&(m_infoMaster.m_vUp));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;
	
	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 40.0f*m_infoMaster.m_vSide;
		}
		break;
	case 2:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vSide;
		}
		break;
	case 3:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 4:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 5:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
		}
		break;
	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel + 60.0f*m_infoMaster.m_vSide;
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel - 60.0f*m_infoMaster.m_vSide;
	}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType5()// 종 형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
//	D3DXVec3Normalize(&g_pShuttleChild->m_vUp,&(m_infoMaster.m_vUp));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;

	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
	}
	break;
	case 2:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 3:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
	}
	break;
	case 4:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 40.0f*m_infoMaster.m_vSide;
	}
	break;
	case 5:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
	}
	break;
	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType6()// 역종 형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
//	D3DXVec3Normalize(&g_pShuttleChild->m_vUp,&(m_infoMaster.m_vUp));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;

	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 2:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 3:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 4:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
	}
	break;
	case 5:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType7()// X-형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;

	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 2:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 3:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 10.0f*m_infoMaster.m_vUp - 10.0f*m_infoMaster.m_vSide;
		}
		break;
	case 4:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vUp;
		}
		break;
	case 5:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 10.0f*m_infoMaster.m_vUp + 10.0f*m_infoMaster.m_vSide;
		}
		break;
	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 10.0f*m_infoMaster.m_vUp + 10.0f*m_infoMaster.m_vSide;
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vUp;
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos + 10.0f*m_infoMaster.m_vUp - 10.0f*m_infoMaster.m_vSide;
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
	}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightForShuttleChildType8()// Star-형
{
	D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&(g_pShuttleChild->m_vVel + m_infoMaster.m_vVel));
	g_pShuttleChild->m_vUp = m_infoMaster.m_vUp;
	g_pShuttleChild->m_fOldUpRate = 0;

	switch(m_infoParty.bFormationFlyingPositionType)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
		}
		break;
	case 2:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 35.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 3:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 35.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 4:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
		}
		break;
	case 5:
		{
			g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 55.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
	case 6:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 55.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;

	case 7:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
	}
	break;
	case 8:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
	}
	break;
	case 9:
	{
		g_pShuttleChild->m_vPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
	}
	break;
	default:
		{
			// 에러!
			FormationFlightClear();
		}
	}
	g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//	g_pShuttleChild->m_vMoveVel = g_pShuttleChild->m_vVel;		// 2006-06-30 by ispark
}

void CClientParty::FormationFlightMoveForEnemy()
{

	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{	
		// 2013-02-06 by bhsohn 파티 관련 메모리 버그 수정
		if(	(*itParty) &&
			(*itParty)->m_bFormationFlight == TRUE && 
			(*itParty)->m_bPartyType == _PARTYMEMBER)
		{
			switch(m_infoParty.bFormationFlyingType)
			{
			case FLIGHT_FORM_2_COLUMN:// 이열 종대 편대
				FormationFlightForEnemyType1(*itParty);
				break;
			case FLIGHT_FORM_2_LINE:// 이열 횡대 편대
				FormationFlightForEnemyType2(*itParty);
				break;
			case FLIGHT_FORM_TRIANGLE:// 삼각 편대
				FormationFlightForEnemyType3(*itParty);
				break;
			case FLIGHT_FORM_INVERTED_TRIANGLE:// 역삼각 편대
				FormationFlightForEnemyType4(*itParty);
				break;
			case FLIGHT_FORM_BELL:// 종 편대
				FormationFlightForEnemyType5(*itParty);
				break;
			case FLIGHT_FORM_INVERTED_BELL:// 역종 편대
				FormationFlightForEnemyType6(*itParty);
				break;
			case FLIGHT_FORM_X:// X-형 편대
				FormationFlightForEnemyType7(*itParty);
				break;
			case FLIGHT_FORM_STAR:// Star-형 편대
				FormationFlightForEnemyType8(*itParty);
				break;
			}
		}
		itParty++;
	}
}

void CClientParty::FormationFlightForEnemyType1(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 15.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 2:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel;
			}
			break;
		case 3:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 45.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 4:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
			}
			break;
		case 5:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 15.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 45.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 90.0f*m_infoMaster.m_vVel;
		}
		break;

		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType2(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 30.0f*m_infoMaster.m_vSide;
			}
			break;
		case 2:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vSide;
			}
			break;
		case 3:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel + 15.0f*m_infoMaster.m_vSide;
			}
			break;
		case 4:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel + 45.0f*m_infoMaster.m_vSide;
			}
			break;
		case 5:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel - 15.0f*m_infoMaster.m_vSide;
			}
			break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 30.0f*m_infoMaster.m_vVel - 45.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 30.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel - 30.0f*m_infoMaster.m_vSide;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType3(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 2:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 3:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
			}
			break;
		case 4:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
			}
			break;
		case 5:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel - 40.0f*m_infoMaster.m_vSide;
			}
			break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 60.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel - 60.0f*m_infoMaster.m_vSide;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType4(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 40.0f*m_infoMaster.m_vSide;
			}
			break;
		case 2:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vSide;
			}
			break;
		case 3:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 4:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
			}
			break;
		case 5:
			{
				pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
			}
			break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel + 60.0f*m_infoMaster.m_vSide;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vVel - 60.0f*m_infoMaster.m_vSide;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType5(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
		}
		break;
		case 2:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 3:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
		}
		break;
		case 4:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 40.0f*m_infoMaster.m_vSide;
		}
		break;
		case 5:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
		}
		break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 40.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType6(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{	
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 2:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 3:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos - 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 4:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos - 20.0f*m_infoMaster.m_vVel;
		}
		break;
		case 5:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos - 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos + 20.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos + 20.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos - 40.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos - 40.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType7(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 2:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 3:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 10.0f*m_infoMaster.m_vUp - 10.0f*m_infoMaster.m_vSide;
		}
		break;
		case 4:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vUp;
		}
		break;
		case 5:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 10.0f*m_infoMaster.m_vUp + 10.0f*m_infoMaster.m_vSide;
		}
		break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 10.0f*m_infoMaster.m_vUp + 10.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 20.0f*m_infoMaster.m_vUp;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos + 10.0f*m_infoMaster.m_vUp - 10.0f*m_infoMaster.m_vSide;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemyType8(PARTYENEMYINFO* pPartyEnemyInfo)
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		switch(pPartyEnemyInfo->m_bFormationFlyingPositionType)
		{
		case 0:
			{
				return;
			}
			break;
		case 1:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 20.0f*m_infoMaster.m_vVel;
		}
		break;
		case 2:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 35.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 3:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 35.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 4:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 40.0f*m_infoMaster.m_vVel;
		}
		break;
		case 5:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 55.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 6:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 55.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;

		case 7:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel - 20.0f*m_infoMaster.m_vSide;
		}
		break;
		case 8:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 60.0f*m_infoMaster.m_vVel;
		}
		break;
		case 9:
		{
			pPartyEnemyInfo->m_pEnemyData->m_vNextPos = m_infoMaster.m_vPos - 75.0f*m_infoMaster.m_vVel + 20.0f*m_infoMaster.m_vSide;
		}
		break;
		default:
			{
				// 에러!
				FormationFlightClear();
			}
		}
		FormationFlightForEnemySetPos(pPartyEnemyInfo);
	}
}

void CClientParty::FormationFlightForEnemySetPos(PARTYENEMYINFO* pPartyEnemyInfo) const
{
	if(pPartyEnemyInfo->m_pEnemyData)
	{
		pPartyEnemyInfo->m_pEnemyData->m_vPos = pPartyEnemyInfo->m_pEnemyData->m_vNextPos;
		pPartyEnemyInfo->m_pEnemyData->m_vVel = m_infoMaster.m_vVel;
		pPartyEnemyInfo->m_pEnemyData->m_vNextVel = m_infoMaster.m_vVel;
		pPartyEnemyInfo->m_pEnemyData->m_vOriNextVel = m_infoMaster.m_vVel;
		pPartyEnemyInfo->m_pEnemyData->m_vMoveVel = D3DXVECTOR3(0,0,0);
		pPartyEnemyInfo->m_pEnemyData->m_vUp = m_infoMaster.m_vUp;
		pPartyEnemyInfo->m_pEnemyData->m_vNextUp = m_infoMaster.m_vUp;
		pPartyEnemyInfo->m_pEnemyData->m_vOriNextUp = m_infoMaster.m_vUp;
		pPartyEnemyInfo->m_pEnemyData->m_vMoveUp = D3DXVECTOR3(0,0,0);
		pPartyEnemyInfo->m_pEnemyData->m_fLifeTime = 2.0f;	
	}
}

BOOL CClientParty::IsFVFEnemy(UID32_t nUniqueNum)
{
	vector<PEER_PARTY_MEMBER>::iterator it = m_vecPeerPartyEnemyInfo.begin();
	while(it != m_vecPeerPartyEnemyInfo.end())
	{
		PEER_PARTY_MEMBER sOtherPartyMember;
		sOtherPartyMember.CharacterUniqueNumber = it->CharacterUniqueNumber;
		sOtherPartyMember.ClientIndex = it->ClientIndex;
		if(sOtherPartyMember.CharacterUniqueNumber == nUniqueNum)
		{
			return TRUE;
		}
		it++;
	}
	return FALSE;
}

void CClientParty::Tick(float fElapsedTime) const
{
	if(IsParty()) 
	{ 
		g_pGameMain->m_pCommunity->GetParty()->m_fSpeakingTimers -= fElapsedTime; 
	} 
} 


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CClientParty::ISendPartyLeaveArena()
/// \brief		아레나에서 메인으로 갈떄 파티 탈퇴
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2007-12-17 ~ 2007-12-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CClientParty::ISendPartyLeaveArena()
{
	DBGOUT("void CClientParty::ISendPartyLeaveArena() \n");	
	g_pIMSocket->SendMsg(T_IC_PARTY_LEAVE_FROM_A_TO_M, NULL, NULL);		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		파티 참여 버튼 클릭
/// \author		// 2008-06-03 by bhsohn EP3 편대 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CClientParty::FSendPartyJoinFree(PartyID_t	nPartyRequestID, char* pPass)
{
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_PARTY_JOIN_FREE);		
		vecUnLockMsg.push_back(T_IC_PARTY_ACCEPT_INVITE_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_PARTY_JOIN_FREE, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	MSG_IC_PARTY_JOIN_FREE sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_PARTY_JOIN_FREE));

	sMsg.PartyNum = nPartyRequestID;
	if(pPass)
	{
		strncpy(sMsg.PartyPW, pPass, SIZE_MAX_TEAM_PW);
	}	
	g_pIMSocket->SendMsg( T_IC_PARTY_JOIN_FREE, (char*)&sMsg, sizeof(sMsg));		
	m_infoParty.PartyID = nPartyRequestID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		편대 진형 설정
/// \author		// 2008-06-17 by bhsohn 편대 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CClientParty::ChagePartyFormation(int nFormation)
{	
	m_stPartyInfo.FormationType = nFormation;
	m_infoParty.bFormationFlyingType = nFormation;
	ISendPartyChangeFlightFormation(nFormation);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		임시 편대 진형 설정
/// \author		// 2008-06-17 by bhsohn 편대 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CClientParty::TempPartyFormation(int nFormation)
{
	if(m_infoParty.bPartyType != _PARTYMASTER)
	{
		return;
	}	
	if(m_nOldFormationFlyingType == -1)
	{
		m_nOldFormationFlyingType = m_infoParty.bFormationFlyingType;
	}
	ChagePartyFormation(nFormation);	

	DBGOUT("CClientParty::TempPartyFormation OLD[%d] Temp[%d]\n", m_nOldFormationFlyingType, nFormation);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		편대 진형 설정
/// \author		// 2008-06-17 by bhsohn 편대 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CClientParty::RefreshFormationType()
{
	DBGOUT("CClientParty::RefreshFormationType OLD[%d] \n", m_nOldFormationFlyingType);
	if(m_nOldFormationFlyingType >= 0)
	{
		ChagePartyFormation(m_nOldFormationFlyingType);
	}
	m_nOldFormationFlyingType = -1;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		편대 진형 설정
/// \author		// 2008-06-17 by bhsohn 편대 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CClientParty::SetSPartyInfo(SPARTY_INFO *pSPartyInfo)
{ 
	memcpy(&m_stPartyInfo,pSPartyInfo,sizeof(SPARTY_INFO)); 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 과거 편대가있었다.
/// \author		// 2008-06-17 by bhsohn 편대 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CClientParty::GetOldFormationFlyingType()			
{
	return m_nOldFormationFlyingType;
}

void CClientParty::SetFirstPartySetupInfo(BOOL bFirstPartySetupInfo)
{
	m_bFirstPartySetupInfo = bFirstPartySetupInfo;
}
BOOL CClientParty::GetFirstPartySetupInfo()
{
	return m_bFirstPartySetupInfo;
}

// 2008-12-09 by dgwoo 미션마스터.
void CClientParty::SetMissionMasterPartyID(PartyID_t i_nID)
{
	m_nTempMMasterPartyID = i_nID;
}								  
// 2008-12-09 by dgwoo 미션마스터.
PartyID_t CClientParty::GetMissionMasterPartyID()
{
	return m_nTempMMasterPartyID;
}

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
INT CClientParty::GetInSameMapPartyMemberCount()
{
	INT nCount = 0;
	vector<PARTYENEMYINFO*>::iterator itParty = m_vecPartyEnemyInfo.begin();
	while(itParty != m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_nUniqueNumber == g_pShuttleChild->GetCharacterUniqueNumber())
		{
 			itParty++;
			continue;
		}

		if(g_pShuttleChild
			&& (*itParty)->m_ImPartyMemberInfo.MapChannelIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex)
		{
			nCount++;
		}
 		itParty++;
	}
	return nCount;
}
// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
