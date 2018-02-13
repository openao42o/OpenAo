// InflRateManager.h: interface for the CInflRateManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLRATEMANAGER_H__C78952E4_F558_4658_91D9_176163716F59__INCLUDED_)
#define AFX_INFLRATEMANAGER_H__C78952E4_F558_4658_91D9_176163716F59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \version	
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CInflRateManager  
{
public:
	CInflRateManager();
	~CInflRateManager();


	void SetSetForAll(BOOL i_bFlag);
	BOOL GetSetForAll(void);
	BOOL InitInflRateManager(mtvectSINFL_USER_COUNT *i_pInflUserCountList);
	void OnChangeInfluence(BYTE i_byInflTy, BYTE i_byLv);
	SINFL_USER_COUNT *GetInflUserCount(BYTE i_byUserLv);
	BOOL PushUser(BYTE i_byInflTy, BYTE i_byUserLv);
	BOOL PopUser(BYTE i_byInflTy, BYTE i_byUserLv);
	BOOL IsSelectableInfluence(BYTE i_byInflTy, BYTE i_byUserLv);


	mtvectSINFL_USER_COUNT		m_vectInflUserCountList;
	BOOL						m_bSetForAll;				// 레벨과 상관없이 전체 유저의 세력차를 관리
};

#endif // !defined(AFX_INFLRATEMANAGER_H__C78952E4_F558_4658_91D9_176163716F59__INCLUDED_)
