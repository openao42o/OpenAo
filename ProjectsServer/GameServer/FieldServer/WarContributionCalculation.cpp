// WarContributionCalculation.cpp: implementation of the CWarContributionCalculation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WarContributionCalculation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWarContributionCalculation::CWarContributionCalculation()
{
	m_nMapIndex = 0;
	m_nTotalContribution = 0;
	m_nBCUCount = 0;
	m_nANICount	= 0;
	m_mtMapCharacterUIDContribution.clear();
	m_nTotalBGearContribution = 0;
	m_nTotalIGearContribution = 0;
	m_nTotalAGearContribution = 0;
	m_nTotalMGearContribution = 0;
	m_nBGearCount = 0;
	m_nIGearCount = 0;
	m_nAGearCount = 0;
	m_nMGearCount = 0;
	m_nBCUDeathCount = 0;
	m_nANIDeathCount = 0;
	m_WarStartTime.SetCurrentDateTime();
}

CWarContributionCalculation::~CWarContributionCalculation()
{
	
}

void CWarContributionCalculation::UpdateCharacterUIDContribution(UID32_t i_nCharacterUID, UID64_t i_nContribution, byte i_byInfl, USHORT i_byGear)
{
	mt_auto_lock mta(&m_mtMapCharacterUIDContribution);
	mtMapCharacterUIDContribution::iterator itr = m_mtMapCharacterUIDContribution.find(i_nCharacterUID);
	if(itr != m_mtMapCharacterUIDContribution.end())
	{
		itr->second.Contribution += i_nContribution;
		if(DAMAGE_CONTRIBUTION == itr->second.CheckDamageContribution)
		{
			this->SetGearContributioninfo(i_byGear, i_nContribution, FALSE);
		}
		else
		{
			this->SetGearContributioninfo(i_byGear, i_nContribution, TRUE);
			itr->second.CheckDamageContribution = DAMAGE_CONTRIBUTION;
			if(INFLUENCE_TYPE_VCN == i_byInfl)
			{
				m_nBCUCount++;
			}
			else if(INFLUENCE_TYPE_ANI == i_byInfl)
			{
				m_nANICount++;
			}
		}
	}
	else
	{
		SWAR_CONTRIBUTION_INFO  WarContributionInfo;
		util::zero(&WarContributionInfo, sizeof(SWAR_CONTRIBUTION_INFO));
		WarContributionInfo.Contribution = i_nContribution;
		WarContributionInfo.CheckDamageContribution = DAMAGE_CONTRIBUTION;
		m_mtMapCharacterUIDContribution.insert(pair<UID32_t, SWAR_CONTRIBUTION_INFO>(i_nCharacterUID, WarContributionInfo));
		if(INFLUENCE_TYPE_VCN == i_byInfl)
		{
			m_nBCUCount++;
		}
		else if(INFLUENCE_TYPE_ANI == i_byInfl)
		{
			m_nANICount++;
		}
		this->SetGearContributioninfo(i_byGear, i_nContribution, TRUE);
	}
	
	m_nTotalContribution += i_nContribution;
}

void CWarContributionCalculation::SetMapIndex(MapIndex_t i_nMapIndex)
{
	m_nMapIndex = i_nMapIndex;
}

MapIndex_t CWarContributionCalculation::GetMapIndex()
{
	return m_nMapIndex;
}

UID64_t	CWarContributionCalculation::GetTotalContribution()
{
	return m_nTotalContribution;
}

INT	CWarContributionCalculation::GetInflUserCount(byte i_byInfl)
{
	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_nBCUCount;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_nANICount;
	}

	return 0;
}

INT	CWarContributionCalculation::GetSizeCharacterUIDContribution()
{
	return m_mtMapCharacterUIDContribution.size();
}

mtMapCharacterUIDContribution * CWarContributionCalculation::GetmtMapCharacterUIDContribution()
{
	return &m_mtMapCharacterUIDContribution;
}

void CWarContributionCalculation::SetGearContributioninfo(USHORT i_byGear, UID64_t i_nContribution, BOOL i_nNew)
{
	if(IS_BGEAR(i_byGear))
	{
		if(i_nNew)
		{
			m_nBGearCount++;
		}
		m_nTotalBGearContribution += i_nContribution;
	}
	else if(IS_IGEAR(i_byGear))
	{
		if(i_nNew)
		{
			m_nIGearCount++;
		}
		m_nTotalIGearContribution += i_nContribution;
	}
	else if(IS_AGEAR(i_byGear))
	{
		if(i_nNew)
		{
			m_nAGearCount++;
		}
		m_nTotalAGearContribution += i_nContribution;
	}
	else if(IS_MGEAR(i_byGear))
	{
		if(i_nNew)
		{
			m_nMGearCount++;
		}
		m_nTotalMGearContribution += i_nContribution;
	}
}

void CWarContributionCalculation::GetGearContributioninfo(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR * o_pContributionGaer)
{
	o_pContributionGaer->nTotalBGearContribution	= m_nTotalBGearContribution;
	o_pContributionGaer->nTotalIGearContribution	= m_nTotalIGearContribution;
	o_pContributionGaer->nTotalAGearContribution	= m_nTotalAGearContribution;
	o_pContributionGaer->nTotalMGearContribution	= m_nTotalMGearContribution;
	o_pContributionGaer->nBGearCount				= m_nBGearCount;
	o_pContributionGaer->nIGearCount				= m_nIGearCount;
	o_pContributionGaer->nAGearCount				= m_nAGearCount;
	o_pContributionGaer->nMGearCount				= m_nMGearCount;
}

void CWarContributionCalculation::UpdateCharacterUIDInflDeathCount(UID32_t i_nCharacterUID, byte i_byInfl)
{
	mt_auto_lock mta(&m_mtMapCharacterUIDContribution);
	mtMapCharacterUIDContribution::iterator itr = m_mtMapCharacterUIDContribution.find(i_nCharacterUID);
	if(itr != m_mtMapCharacterUIDContribution.end())
	{
		itr->second.DeathCount++;
		if(INFLUENCE_TYPE_VCN == i_byInfl)
		{
			m_nBCUDeathCount++;
		}
		else if(INFLUENCE_TYPE_ANI == i_byInfl)
		{
			m_nANIDeathCount++;
		}
	}
	else
	{
		SWAR_CONTRIBUTION_INFO  WarContributionInfo;
		util::zero(&WarContributionInfo, sizeof(SWAR_CONTRIBUTION_INFO));
		WarContributionInfo.DeathCount++;
		m_mtMapCharacterUIDContribution.insert(pair<UID32_t, SWAR_CONTRIBUTION_INFO>(i_nCharacterUID, WarContributionInfo));
		if(INFLUENCE_TYPE_VCN == i_byInfl)
		{
			m_nBCUDeathCount++;
		}
		else if(INFLUENCE_TYPE_ANI == i_byInfl)
		{
			m_nANIDeathCount++;
		}
	}
}

INT	CWarContributionCalculation::GetInflDeathCount(byte i_byInfl)
{
	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_nBCUDeathCount;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_nANIDeathCount;
	}

	return 0;
}

INT	CWarContributionCalculation::GetTotalWarTime()
{
	ATUM_DATE_TIME	WarEndTime;
	WarEndTime.SetCurrentDateTime();

	return	WarEndTime.GetTimeDiffTimeInMinutes(m_WarStartTime);
}

void CWarContributionCalculation::UpdateCharacterUIDOtherInflStayTime(UID32_t i_nCharacterUID, BOOL i_bCheckStart)
{
	mt_auto_lock mta(&m_mtMapCharacterUIDContribution);
	mtMapCharacterUIDContribution::iterator itr = m_mtMapCharacterUIDContribution.find(i_nCharacterUID);
	if(itr != m_mtMapCharacterUIDContribution.end())
	{
		if(i_bCheckStart)
		{
			if(NOT_OTHER_INFL_STAY == itr->second.CheckOtherInflStay)
			{// 상대 세력 맵에 없다가 상대 세력 맵에 왔으므로 값 설정.
				itr->second.OtherInflStayStartTime.SetCurrentDateTime();
				itr->second.CheckOtherInflStay = OTHER_INFL_STAY;
			}
		}
		else
		{
			if(OTHER_INFL_STAY == itr->second.CheckOtherInflStay)
			{// 상대 세력 맵에서 나갔다면 상대 세력에서 머문 시간 업데이트.
				ATUM_DATE_TIME	CurrentTime;
				CurrentTime.SetCurrentDateTime();
				itr->second.OtherInflStayTime += CurrentTime.GetTimeDiffTimeInMinutes(itr->second.OtherInflStayStartTime);
				itr->second.OtherInflStayStartTime.Reset();
				itr->second.CheckOtherInflStay = NOT_OTHER_INFL_STAY;
			}
		}
	}
	else
	{
		SWAR_CONTRIBUTION_INFO  WarContributionInfo;
		util::zero(&WarContributionInfo, sizeof(SWAR_CONTRIBUTION_INFO));
		WarContributionInfo.OtherInflStayStartTime.SetCurrentDateTime();
		WarContributionInfo.CheckOtherInflStay = OTHER_INFL_STAY;
		m_mtMapCharacterUIDContribution.insert(pair<UID32_t, SWAR_CONTRIBUTION_INFO>(i_nCharacterUID, WarContributionInfo));
	}	
}

CBossWarContributionCalculation::CBossWarContributionCalculation(int i_nMapIndex, int i_nStartWarContributionPoint)
{
	this->Init();
	m_nMapIndex = i_nMapIndex;
	m_nStartWarContributionPoint = i_nStartWarContributionPoint;
}

CBossWarContributionCalculation::~CBossWarContributionCalculation()
{

}

void CBossWarContributionCalculation::Init()
{
	m_mtVectWarContributionCalculationGuild.clear();
	m_nStartWarContributionPoint = 0;
}

void CBossWarContributionCalculation::UpdateGuildUIDContribution(SWarContributionCalculationGuild * i_pWarContributionCalculationGuild)
{
	mt_auto_lock mta(&m_mtVectWarContributionCalculationGuild);
	for(mtVectWarContributionCalculationGuild::iterator itr(m_mtVectWarContributionCalculationGuild.begin())
		; itr != m_mtVectWarContributionCalculationGuild.end(); itr++)
	{
		if(i_pWarContributionCalculationGuild->GuildUID == itr->GuildUID)
		{
			itr->Contribution += i_pWarContributionCalculationGuild->Contribution;
			return;
		}
	
	}
	
	SWarContributionCalculationGuild tmWarContributionCalculationGuild;
	util::zero(&tmWarContributionCalculationGuild, sizeof(SWarContributionCalculationGuild));
	tmWarContributionCalculationGuild.GuildUID	= i_pWarContributionCalculationGuild->GuildUID;
	util::strncpy(tmWarContributionCalculationGuild.GuildName, i_pWarContributionCalculationGuild->GuildName, SIZE_MAX_GUILD_NAME);
	tmWarContributionCalculationGuild.GuildInfl = i_pWarContributionCalculationGuild->GuildInfl;
	tmWarContributionCalculationGuild.Contribution	= i_pWarContributionCalculationGuild->Contribution;
	m_mtVectWarContributionCalculationGuild.push_back(tmWarContributionCalculationGuild);
}

INT CBossWarContributionCalculation::GetStartWarContributionPoint()
{
	return m_nStartWarContributionPoint;
}

mtVectWarContributionCalculationGuild * CBossWarContributionCalculation::GetmtVectWarContributionCalculationGuild()
{
	return &m_mtVectWarContributionCalculationGuild;
}

struct sort_GUILD_DAMAGE_By_Contribution
{
	bool operator()(SWarContributionCalculationGuild op1, SWarContributionCalculationGuild op2)
	{
		return op1.Contribution > op2.Contribution;				// 내림차순 정렬
	}
};

void CBossWarContributionCalculation::SortmtVectWarContributionCalculationGuild()
{
	mt_auto_lock mta(&m_mtVectWarContributionCalculationGuild);
	if(0 >= m_mtVectWarContributionCalculationGuild.size())
	{
		return;
	}
	if(1 < m_mtVectWarContributionCalculationGuild.size())
	{
		sort(m_mtVectWarContributionCalculationGuild.begin(), m_mtVectWarContributionCalculationGuild.end(), sort_GUILD_DAMAGE_By_Contribution());
	}
}