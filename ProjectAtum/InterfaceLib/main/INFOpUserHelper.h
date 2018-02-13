// INFOpUserHelper.h: interface for the CINFOpUserHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPUSERHELPER_H__709F5D23_BF85_4BB8_BFE4_7640AA1A294A__INCLUDED_)
#define AFX_INFOPUSERHELPER_H__709F5D23_BF85_4BB8_BFE4_7640AA1A294A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFImageBtn.h"
#include "INFOpSmallHelper.h"
#include "INFOpBigHelper.h"


// 진형
#define HELPER_NPC_IMAGE_VCU		0		// 바이제니유
#define HELPER_NPC_IMAGE_ANI		1		// 알링턴
#define MAX_HELPER_NPC_IMAGE		2

// 오퍼레이터 이미지 크기
#define HELPER_NPCIMAGE_TUTO		0		// 튜토리얼 
#define HELPER_NPCIMAGE_NOR			1		// 마을 
#define	MAX_HELPER_NPCIMAGE			2



class CINFOpMain;

class CINFOpUserHelper  
{
public:
	CINFOpUserHelper(CINFOpMain* pParent);
	virtual ~CINFOpUserHelper();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 글씨를 쓴다.
	void SetHelperString(char* pTxt, int nShowModeBtn);

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int OnKeyDown(WPARAM wParam, LPARAM lParam);

	DataHeader * FindResource(char* szRcName);

	// 프리스카 맵 헬퍼 시작
	void OnClickFreeSkaHelperStart();
	void OnClickNextBtn();		// 다음 버튼 
	void OnClickGoMainBtn();	// 마을로 이동 
	void OnClickCloseBtn();	// 마을로 이동 
	

	void StartFirstUserExplain();
	void EndFirstUserExplain();
	void CloseFirstUserExplain();

	void EndCompleteOpHelper();

	// 튜토리얼시작 
	void TutorialComplete();		// 성공
	void TutorialFail();			// 실패 
	void TutorialHelper(char* pTxt);
	void TutorialAllComplete();		// 모두 완료 
	void TutorialNOTL10();

	// 세력을 선택하여 첫번쨰 퀘스트
	void SelInfluence(int nSelMode);

	void PlayFreeSkaNarrationSound(int nSelctExplain);

	BOOL OnClickEscape();

	int GetHelperMode();

private:
	// 프리스카 헬퍼
	void RenderFreeSkaHelper(int nHelperSubMode);	

private:
	CINFOpMain*		m_pParent;
	CINFImageEx*		m_pNPCImage[MAX_HELPER_NPCIMAGE][MAX_HELPER_NPC_IMAGE];
	int				m_nSelectNpc;	
	int				m_nSelectMode;	
	
	// 현재 보여주고 있는 모드
	int						m_nHelperMode;
	int						m_nHelperSubMode;

	CINFOpSmallHelper*		m_pINFOpSmallHelper;
	CINFOpBigHelper*		m_pINFOpBigHelper;
	
	// 닫을 떄 알파값
	float					m_fNPCViewTime;
	DWORD					m_dwNPCAlpha;

	BOOL					m_bStartAlphaNPC;
};

#endif // !defined(AFX_INFOPUSERHELPER_H__709F5D23_BF85_4BB8_BFE4_7640AA1A294A__INCLUDED_)
