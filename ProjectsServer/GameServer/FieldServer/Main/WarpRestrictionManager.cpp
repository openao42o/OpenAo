#include "StdAfx.h"

#ifdef SC_LEADER_WARP_RESTRICTION

#include "WarpRestrictionManager.h"

#include "FieldIOCPSocket.h"


CWarpRestrictionManager::CWarpRestrictionManager()
{
}


CWarpRestrictionManager::~CWarpRestrictionManager()
{
}

// Restricts the current socket for the given minutes from warping
// Returns whether the character is already restricted (FALSE) or not (TRUE)
BOOL CWarpRestrictionManager::SetWarpRestriction(CFieldIOCPSocket* pTarget, DWORD forMillis)
{
	// Check if the user is already restricted
	if (IsWarpRestricted(pTarget))
		return FALSE;

	// Delete any previous restrictions for the char
	m_mtmapWarpRestrictions.deleteLock(pTarget->GetCharacter()->CharacterUniqueNumber);

	// Insert the new char
	m_mtmapWarpRestrictions.insertLock(pTarget->GetCharacter()->CharacterUniqueNumber, timeGetTime() + forMillis);
	return TRUE;
}

// Releases a warping restriction for a given character
// Returns whether there was a restriction (TRUE) or not (FALSE)
BOOL CWarpRestrictionManager::ReleaseWarpRestriction(CFieldIOCPSocket* pTarget)
{
	BOOL wasWarpRestricted = IsWarpRestricted(pTarget);
	m_mtmapWarpRestrictions.deleteLock(pTarget->GetCharacter()->CharacterUniqueNumber);
	return wasWarpRestricted;
}

// Returns whether there exists a warping restriction for the given Character (TRUE) or not (FALSE)
BOOL CWarpRestrictionManager::IsWarpRestricted(CFieldIOCPSocket* pTarget)
{
	return m_mtmapWarpRestrictions.findLock(pTarget->GetCharacter()->CharacterUniqueNumber) > timeGetTime();
}

// Gets the warping restriction time left for a target in milliseconds
DWORD CWarpRestrictionManager::GetWarpRestrictionTimeLeft(CFieldIOCPSocket* pTarget)
{
	if (IsWarpRestricted(pTarget))
	{
		return m_mtmapWarpRestrictions.findLock(pTarget->GetCharacter()->CharacterUniqueNumber) - timeGetTime();
	}
	else
	{
		return 0;
	}
}

#endif // SC_LEADER_WARP_RESTRICTION