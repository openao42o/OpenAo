/// InfinityMapManager.h: interface for the CInfinityMapManager class.
/// \brief		인피니티 - 인피니티에서 생성하는 맵 관리 클래스
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYMAPMANAGER_H__F166EE86_6624_4759_BD78_A6F9B3FEA7F7__INCLUDED_)
#define AFX_INFINITYMAPMANAGER_H__F166EE86_6624_4759_BD78_A6F9B3FEA7F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FieldMapWorkspace.h"
#include "FieldMapProject.h"

struct SInfinityMapInfo {
	MapIndex_t		InfinityMapIdx;			
	ChannelIndex_t	InfinityMapChannel;	
	BOOL			InfinityChannelSet;
};
typedef mt_vector<SInfinityMapInfo>   mtvectInfinityMapInfo;

class CInfinityMapManager
{
public:
	CInfinityMapManager();
	virtual ~CInfinityMapManager();

	void InitInfinityMapManager();
	void SetInfinityMapManager(CFieldMapWorkspace * i_pFieldMapWorkspace);
	void InitInfinityMapInfoList(vectorInfinityModeInfo * i_pVectInfiModeInfo);
	CFieldMapChannel * CreateInfinityMap(MapIndex_t i_MapIndex);
	void ResetInfinityMap(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);

protected:
	CFieldMapWorkspace	  * m_pFieldMapWorkspace;
	mtvectInfinityMapInfo	m_mtInfinityMapInfoList;
};

#endif // !defined(AFX_INFINITYMAPMANAGER_H__F166EE86_6624_4759_BD78_A6F9B3FEA7F7__INCLUDED_)
