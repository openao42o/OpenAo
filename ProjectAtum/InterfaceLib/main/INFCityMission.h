// INFCityMission.h: interface for the CINFCityMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYMISSION_H__13CBEB57_3A36_41F4_9AF8_C01670DA45E0__INCLUDED_)
#define AFX_INFCITYMISSION_H__13CBEB57_3A36_41F4_9AF8_C01670DA45E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define CITY_MISSION_BUTTON_NUMBER	4
#define CITY_MISSION_BUTTON_JOIN	0
#define CITY_MISSION_BUTTON_CANCEL	1

#define CITY_MISSION_LINE_NUMBER	11
#define CITY_MISSION_MAX_NUMBER		30
#define CITY_MISSION_LINE_LENGTH	50

class CAtumNode;
class CQuest;
class CINFImage;
class CINFImageEx;
class CD3DHanFont;
class CINFScrollBar;
class CINFCityMission : public CINFBase  
{
public:
	CINFCityMission(CAtumNode* pParent);
	virtual ~CINFCityMission();

	void Reset();
	void AddMission(CQuest* pQuest);

	void SetMissionDetailInfo(CQuest* pQuest);
	BOOL IsExistMission() { return m_mapCityMission.size() > 0 ? TRUE : FALSE; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonDown(int nButton);

protected:
	// Set Quest Description
	void SetQuestDescString(int index, char* szString);// 일반 스트링을 넣는다.(아래 여러개 함수 수행시 제목 사용에 용이하다)
	void SetQuestDescQuestName(int index, char* szName);
	void SetQuestDescQuestTypeRepeatable(int index);//무한 반복 퀘스트인경우만 표시
	void SetQuestDescRequstLevel(int index, CQuest* pQuest);// 제한레벨이 있을때만 보여준다
	void SetQuestDescNextQuestName(int index, int nNextQuestIndex);//연속성인 경우만 표시
	void SetQuestDescReqUnitKind(int index, USHORT nReqUnitKind);
	void SetQuestDescReqLevel(int index, INT_RANGE nReqLevel);
	//void SetQuestDescReqExperience(int index, Experience_t fExperience);
	void SetQuestDescReqAttackPart(int index, INT_RANGE nReqAttackPart);
	void SetQuestDescReqDefensePart(int index, INT_RANGE nReqDefensePart);
	void SetQuestDescReqFuelPart(int index, INT_RANGE nReqFuelPart);
	void SetQuestDescReqSoulPart(int index, INT_RANGE nReqSoulPart);
	void SetQuestDescReqShieldPart(int index, INT_RANGE nReqShieldPart);
	void SetQuestDescReqDodgePart(int index, INT_RANGE nReqDodgePart);
	void SetQuestDescReqQuest(int index, int QuestIndex);// 퀘스트 개수만큼 반복하여 넣는다.
	void SetQuestDescEndPayBonusStat(int index, BYTE nBonusStat);
	void SetQuestDescEndPayBonusSkillPoint(int index, BYTE nBonusSkillPoint);
	void SetQuestDescEndPayFame(int index, int nFame);
	void SetQuestDescEndPayStatus(int index, int nStatus);
	void SetQuestDescEndPayLevel(int index, BYTE nLevel);
	void SetQuestDescEndPayExperience(int index, int nExperience);
	void SetQuestDescEndPayPropensity(int index, int nPropensity);
	void SetQuestDescEndPayWarpMap(int index, MapIndex_t nWrapMapIndex);
	void SetQuestDescItemCountVector(char * strName, vector<ITEM_W_COUNT> &vecItem);
	void SetQuestDescItemCountVector(char * strName, vector<QUEST_PAY_ITEM> &vecItem);
	void SetQuestDescEndRandomPayItemVector(char* strName, vector<QUEST_RANDOM_PAY_ITEM> &vecItem );
	void SetQuestDescEndPayMixItemVector(char* strName, vector<ITEM_MIXING_INFO> &vecItem );
	void SetQuestDescItemCount(int index, ITEM_W_COUNT itemCount);
	void SetQuestDescItemCount(int & index, QUEST_PAY_ITEM itemCount);
	void SetQuestDescItemCount(int index, ITEM_W_COUNT_PROB itemCount);

protected:
	BOOL			m_bRestored;
	CINFScrollBar*	m_pListScroll;
	CINFScrollBar*	m_pDescScroll;
	CINFImageEx*	m_pJoinButton[CITY_MISSION_BUTTON_NUMBER];
	CINFImageEx*		m_pCompleteButton[CITY_MISSION_BUTTON_NUMBER];
	CINFImageEx*		m_pCancelButton[CITY_MISSION_BUTTON_NUMBER];
	int				m_nButtonState[2];// CITY_MISSION_BUTTON_JOIN, CITY_MISSION_BUTTON_CANCEL

	CINFImageEx*		m_pImgBack;
	CINFImageEx*		m_pImgTitle;
//	CINFImage*		m_pImgScroll;
	CINFImageEx*		m_pImgHighLight;

	CD3DHanFont*	m_pFontQuestList[CITY_MISSION_LINE_NUMBER];
	CD3DHanFont*	m_pFontQuestDesc[CITY_MISSION_LINE_NUMBER];
	char			m_strQuestList[CITY_MISSION_MAX_NUMBER][CITY_MISSION_LINE_LENGTH];
	char			m_strQuestDesc[CITY_MISSION_MAX_NUMBER][CITY_MISSION_LINE_LENGTH];
	int				m_nQuestDescLineNumber;		// 퀘스트 설명 라인 갯수

//	int				m_nCurrentQuestListScroll;
//	BOOL			m_bQuestListLock;
//	int				m_nCurrentQuestDescScroll;
//	BOOL			m_bQuestDescLock;
	int				m_nCurrentSelectQuestList;	//선택된 퀘스트 번호

	CMapQuest		m_mapCityMission;			// map[화면상 번호(m_nCurrentSelectQuestList)] = Quest;

public:
	BOOL			m_bProgressMission[CITY_MISSION_MAX_NUMBER];
	int				m_nCancerSelect;			// 미션 취소

};

#endif // !defined(AFX_INFCITYMISSION_H__13CBEB57_3A36_41F4_9AF8_C01670DA45E0__INCLUDED_)
