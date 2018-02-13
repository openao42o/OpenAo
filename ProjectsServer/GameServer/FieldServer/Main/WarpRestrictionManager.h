#pragma once
#ifdef SC_LEADER_WARP_RESTRICTION

class CFieldIOCPSocket;

class CWarpRestrictionManager
{
public:
	CWarpRestrictionManager();
	~CWarpRestrictionManager();

public:
	BOOL	SetWarpRestriction(CFieldIOCPSocket* pTarget, DWORD forMillis);
	BOOL	ReleaseWarpRestriction(CFieldIOCPSocket* pTarget);
	BOOL	IsWarpRestricted(CFieldIOCPSocket* pTarget);
	DWORD	GetWarpRestrictionTimeLeft(CFieldIOCPSocket* pTarget);

private:
	mt_map<UID32_t, DWORD>	m_mtmapWarpRestrictions;
};

#endif // SC_LEADER_WARP_RESTRICTION