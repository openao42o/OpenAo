#pragma once

#include "CompileUtils.h"

constexpr int BannedSkills[] =
{
	781110,
	782105,
	783204,
	780303,
	780304,
	781308,
	782304,
	782306,
	783301,
	783305,
	784304,
	784305,
};

inline int IsBannedSkill(int skillnum)
{
	skillnum /= 10;

	for (auto skill : BannedSkills) if (skillnum == skill) return true;

	return false;
}