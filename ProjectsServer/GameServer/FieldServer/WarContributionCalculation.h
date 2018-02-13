// WarContributionCalculation.h: interface for the CWarContributionCalculation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARCONTRIBUTIONCALCULATION_H__4E4479AE_9B66_4217_9B59_01CA87836866__INCLUDED_)
#define AFX_WARCONTRIBUTIONCALCULATION_H__4E4479AE_9B66_4217_9B59_01CA87836866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SWarContributionCalculationGuild
{
	UID32_t		GuildUID;
	char		GuildName[SIZE_MAX_GUILD_NAME];
	BYTE		GuildInfl;
	UID64_t		Contribution;
};

typedef mt_map<UID32_t, SWAR_CONTRIBUTION_INFO>	mtMapCharacterUIDContribution;
typedef mt_vector<SWarContributionCalculationGuild>	mtVectWarContributionCalculationGuild;

class CWarContributionCalculation
{
public:
	CWarContributionCalculation();
	virtual ~CWarContributionCalculation();

	void UpdateCharacterUIDContribution(UID32_t i_nCharacterUID, UID64_t i_nContribution, byte i_byInfl, USHORT i_byGear);
	void		SetMapIndex(MapIndex_t i_nMapIndex);
	MapIndex_t	GetMapIndex();
	UID64_t		GetTotalContribution();
	INT			GetInflUserCount(byte i_byInfl);
	INT			GetSizeCharacterUIDContribution();
	mtMapCharacterUIDContribution * GetmtMapCharacterUIDContribution();
	void		SetGearContributioninfo(USHORT i_byGear, UID64_t i_nContribution, BOOL i_nNew);
	void		GetGearContributioninfo(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR * o_pContributionGaer);
	void		UpdateCharacterUIDInflDeathCount(UID32_t i_nCharacterUID, byte i_byInfl);
	INT			GetInflDeathCount(byte i_byInfl);
	INT			GetTotalWarTime();
	void		UpdateCharacterUIDOtherInflStayTime(UID32_t i_nCharacterUID, BOOL i_bCheckStart);
	ATUM_DATE_TIME * GetWarStartTime() {return &m_WarStartTime;}		// 2010-06-01 by shcho, GLogDB ฐüทร -

protected:
	MapIndex_t		m_nMapIndex;
	UID64_t			m_nTotalContribution;
	INT				m_nBCUCount;
	INT				m_nANICount;
	UID64_t			m_nTotalBGearContribution;
	UID64_t			m_nTotalIGearContribution;
	UID64_t			m_nTotalAGearContribution;
	UID64_t			m_nTotalMGearContribution;
	INT				m_nBGearCount;
	INT				m_nIGearCount;
	INT				m_nAGearCount;
	INT				m_nMGearCount;
	INT				m_nBCUDeathCount;
	INT				m_nANIDeathCount;
	ATUM_DATE_TIME	m_WarStartTime;
	mtMapCharacterUIDContribution m_mtMapCharacterUIDContribution;
};

class CBossWarContributionCalculation: public CWarContributionCalculation
{
public:
	CBossWarContributionCalculation(int i_nMapIndex, int i_nStartWarContributionPoint);
	virtual ~CBossWarContributionCalculation();

	void Init();
	void UpdateGuildUIDContribution(SWarContributionCalculationGuild * i_pWarContributionCalculationGuild);
	INT GetStartWarContributionPoint();
	mtVectWarContributionCalculationGuild * GetmtVectWarContributionCalculationGuild();
	void SortmtVectWarContributionCalculationGuild();

	UID32_t		m_nFirstGuild;
	UID32_t		m_nSecondGuild;
	UID32_t		m_nThirdGuild;
private:
	mtVectWarContributionCalculationGuild m_mtVectWarContributionCalculationGuild;
	INT			m_nStartWarContributionPoint;
};


#endif // !defined(AFX_WARCONTRIBUTIONCALCULATION_H__4E4479AE_9B66_4217_9B59_01CA87836866__INCLUDED_)
