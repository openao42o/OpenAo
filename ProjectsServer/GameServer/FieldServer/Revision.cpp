// Revision.cpp: implementation of the CRevision class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Revision.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRevision::CRevision()
{
	this->InitRevision();
}

CRevision::~CRevision()
{

}

void CRevision::InitRevision() {
	m_vectRevisionInfo.clear();
}

void CRevision::SetRevisionInfo(vectorRevisionInfo * i_pVectRevisionInfo) {
	if(i_pVectRevisionInfo) {
		m_vectRevisionInfo.clear();
		m_vectRevisionInfo.assign(i_pVectRevisionInfo->begin(), i_pVectRevisionInfo->end());		
	}
}

BOOL CRevision::GetRevisionInfo(REVISIONINFO * o_pRevisionInfo, RevisionNum_t i_RevisionNum, INT i_nUnitKind) {
	if(NULL == o_pRevisionInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	vectorRevisionInfo::iterator itr = m_vectRevisionInfo.begin();
	for(; itr != m_vectRevisionInfo.end(); itr++) {
		if(i_RevisionNum == itr->RevisionNum
			&& IS_SAME_UNITKIND(itr->UnitKind, i_nUnitKind)) {
			o_pRevisionInfo->RevisionNum	= itr->RevisionNum;
			o_pRevisionInfo->UnitKind		= itr->UnitKind;
			o_pRevisionInfo->RevisionLevel	= itr->RevisionLevel;
			o_pRevisionInfo->RevisionHP	= itr->RevisionHP;
			o_pRevisionInfo->RevisionDP	= itr->RevisionDP;
			o_pRevisionInfo->RevisionStandardWeapon	= itr->RevisionStandardWeapon;
			o_pRevisionInfo->RevisionAdvancedWeapon	= itr->RevisionAdvancedWeapon;
			return TRUE;
		}
	}

	return FALSE;
}
