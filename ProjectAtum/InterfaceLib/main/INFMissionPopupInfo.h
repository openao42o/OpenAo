// INFMissionPopupInfo.h: interface for the CINFMissionPopupInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONPOPUPINFO_H__65216759_8DC9_4CA0_A474_603E9D4DAC42__INCLUDED_)
#define AFX_INFMISSIONPOPUPINFO_H__65216759_8DC9_4CA0_A474_603E9D4DAC42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFAniCtrl.h"
#include "INFImageListBox.h"

class CINFMissionMain;

class CINFMissionPopupInfo  
{
public:
	CINFMissionPopupInfo(CINFMissionMain* i_pParent);
	virtual ~CINFMissionPopupInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitPopupInfo(int nPossibleQuestIdx);

private:
	void SetAniCtrlPos(float fBackPosX, float fBackPosY);
	void SetAniStepPosY(int nStep, int nMaxStep);

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void SetQuestNameString(CQuest* pQuest, vector<string>* o_vecMissionName);
	void UpdateListBoxPos(float fBackPosX, float fBackPosY);	// 리스트 박스 위치지정

	void TickPopupMode();
	void TickBriefMode();
	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	void TickInfiMode();
	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템

	void UpdateBtnPos(float fShowPosY);	

private:
	// 애니메이션 컨트롤

	CINFAniCtrl*			m_pINFAniCtrl;


	// 배경 
	CINFImageEx*			m_pMissionBk;

	// 닫기 창
	CINFImageBtn*			m_pCloseBtn;
	// 미션 시작 창
	CINFImageBtn*			m_pStartMissionBtn;

	float					m_fBackPosX;
	float					m_fBackPosY;
		
	float					m_fShowPosY;

	float					m_fBackWidth;
	float					m_fBackHeight;

	float					m_fStepCapY;
	float					m_fStepY;

	float					m_fPopupTime;
	float					m_fStringShowTime;

	BOOL					m_bPopupMode;
	BOOL					m_bBriefMode;
	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	BOOL					m_bInfiMode;
	BOOL					m_bEndPopupMode;
	float					m_bInfiCountDown;

	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템

	// 부모 윈도우
	CINFMissionMain*		m_pParentMissionMain;
	CINFImageListBox*		m_pINFImageListBox; // 미션 설명 에디트 박스

	// 미션 정보
	vector<string>			m_vecMissionName;			// 미션 이름
	vector<string>			m_vecBriefMissionName;			// 미션 설명	
	vector<string>			m_vecPossibleDesc;			// 미션 설명	
	vector<string>			m_vecBriefDesc;				// 미션 설명	

	int						m_nShowStringCnt;

	int						m_nSelPossibleQuestIdx;

};

#endif // !defined(AFX_INFMISSIONPOPUPINFO_H__65216759_8DC9_4CA0_A474_603E9D4DAC42__INCLUDED_)
