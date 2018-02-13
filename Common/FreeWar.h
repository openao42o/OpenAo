#pragma once

#ifdef FREEWAR_

inline bool isFreeForAllMap(int idx)
{
	static const int WarMaps[] = 
	{ 
		9020, 
		3082, 
		3083, 
		9017, 
		9018, 
		9019 
	};

	for (auto mapidx : WarMaps)
		
		if (idx == mapidx) return true;

	return false;
}


inline bool isBannedSkillWarMap(int num)
{
	static const int BannedSkillNums[] = 
	{
		7803044,
		7813074,
		7823064,
		7833054,
		7813031,
		7811039,
		7811059,
		7823041,
		7823040,
		7803031,
		7803030
	};

	for (auto skillnum : BannedSkillNums)

		if (num == skillnum) return true;

	return false;
}

#endif