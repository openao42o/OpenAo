// CommuPartyOp.h: interface for the CINFCommuPartyOp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUPARTYOP_H__77B79AB8_C2DD_4782_9683_39D57698D4D3__INCLUDED_)
#define AFX_COMMUPARTYOP_H__77B79AB8_C2DD_4782_9683_39D57698D4D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
class CINFListBox;

class CINFCommuPartyOp  : public CINFBase
{
public:
	CINFCommuPartyOp(CAtumNode* pParent);
	virtual ~CINFCommuPartyOp();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	// 강제 퇴장 시킬 유저
	UID32_t GetPartyBanCharacterUniqueNumber();
	void SetPartyBanCharacterUniqueNumber(UID32_t nPartyBanCharacterUniqueNumber);

	// 위임할 유저
	UID32_t GetPartyTransferMasterCharacterUniqueNumber();
	void SetPartyTransferMasterCharacterUniqueNumber(UID32_t nPartyTransferMasterCharacterUniqueNumber);
	
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	void RenderPartyInfo();
	DWORD GetParyInfoColor(int nIdx, PARTY_TYPE PartyType, char* pUserName);

	void PartyQuestionBanUser();		//추방을 물어본다.
	void PartyQuestionTransferMaster();	// 위임

	void RefreshPartyInfo();
	void ChagePartyFormation(int nFormation);// 편대 진형 변경

	void InitComboInfo();	// 콤보 버튼 초기화

private:
	CINFImageEx*	m_pFormat;	

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;

	CINFImageBtn*			m_pJoinBtn;			// 초대
	CINFImageBtn*			m_pLeaveBtn;			// 탈퇴
	CINFImageBtn*			m_pPurgeBtn;			// 추방
	CINFImageBtn*			m_pChargeBtn;			// 위임

	CINFImageBtn*			m_pCloseBtn;			// 닫기
	CINFImageBtn*			m_pOkBtn;				// 확인

	CD3DHanFont*			m_pPartyList;			// 파티원 목록

	CINFImageEx*				m_pSelImage;			// 파티원선택이미지

	int						m_nPartySel;

	UID32_t					m_nPartyBanCharacterUniqueNumber;				// 추방할 멤버의 유닛 넘버
	UID32_t					m_nPartyTransferMasterCharacterUniqueNumber;	// 위임할 케릭터

	PARTY_TYPE				m_bPartyTypeBtn;
	PARTYINFO				m_partyInfoTmp;

	CINFListBox *			m_pComboForMation;

};

#endif // !defined(AFX_COMMUPARTYOP_H__77B79AB8_C2DD_4782_9683_39D57698D4D3__INCLUDED_)
