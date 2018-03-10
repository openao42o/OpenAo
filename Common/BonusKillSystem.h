//Copyright[2002] MasangSoft
#pragma once

// 2015-06-12 Future, Kill Count System Properties
#if S_BONUS_KILL_SYSTEM_RENEWAL

// The Amount of Kills needed to reward the bonus Items
enum
{
	KILLCOUNT_MAX_BG = 150,
	KILLCOUNT_MAX_MG = 100,
	KILLCOUNT_MAX_IG = 150,
	KILLCOUNT_MAX_AG = 150,
};

template<typename GearType_t>
constexpr unsigned KILLCOUNT_MAX(const GearType_t& GearType)
{
	return

		IS_BGEAR(GearType) ? KILLCOUNT_MAX_BG :
		IS_MGEAR(GearType) ? KILLCOUNT_MAX_MG :
		IS_IGEAR(GearType) ? KILLCOUNT_MAX_IG :
		IS_AGEAR(GearType) ? KILLCOUNT_MAX_AG :
		
		~0;
}

// The Area in which the kills will be highlighted until the User reaches the KILLCOUNT_MAX
constexpr auto HIGHLIGHT_COUNT_AREA = 10;

// Struct for Bonus Item Rewards
struct KILL_REWARD_ITEM
{
	INT ItemNum;
	INT Count;
};

// The time in minutes while the Killcount of a Character should be cached
constexpr auto KILLCOUNT_CACHE_TIME_IN_MINUTES = 30;

struct KILLCOUNT_CACHE_DATA
{
	UID32_t			CharacterUniqueNumber;
	INT				KillCount;
	ATUM_DATE_TIME	CacheTime;
};

// 2015-06-12 Future, Bonus Kill Count Reward Items
// Format:
// { ItemNum, ItemCount }
constexpr KILL_REWARD_ITEM BonusKillSystemRewardItemNUMs[] =
{
	{ 7006380, 3 },
	{ 7026570, 5 },
	{ 7026600, 5 },
	{ 7020640, 5 },
	{ 7020650, 5 },
	{ 7020660, 5 },
	{ 7002840, 100 },
	{ 7004680, 50 },
	{ 9935200, 1 },
	{ 9935230, 1 },
	{ 9944400, 1 },
};


#endif // S_BONUS_KILL_SYSTEM_RENEWAL
