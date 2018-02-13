// Revision.h: interface for the CRevision class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REVISION_H__F3BAF9A8_B5A6_45FC_98DF_924EB72F56C4__INCLUDED_)
#define AFX_REVISION_H__F3BAF9A8_B5A6_45FC_98DF_924EB72F56C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRevision  
{
public:
	CRevision();
	virtual ~CRevision();

	void InitRevision();
	void SetRevisionInfo(vectorRevisionInfo * i_pVectRevisionInfo);
	BOOL GetRevisionInfo(REVISIONINFO * o_pRevisionInfo, RevisionNum_t i_RevisionNum, INT i_nUnitKind);
	
protected:
	vectorRevisionInfo	m_vectRevisionInfo;
};

#endif // !defined(AFX_REVISION_H__F3BAF9A8_B5A6_45FC_98DF_924EB72F56C4__INCLUDED_)
