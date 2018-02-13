// INFCommuPartyCre.h: interface for the CINFCommuPartyCre class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUPARTYCRE_H__63861C02_6AC5_4D66_8FC1_21BBD65AA0BE__INCLUDED_)
#define AFX_INFCOMMUPARTYCRE_H__63861C02_6AC5_4D66_8FC1_21BBD65AA0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 라디오버튼 갯수
#define		RADIO_JOINT_COMMAND				0	// 편대장 초대
#define		RADIO_JOINT_FREE				1	// 자유 참여
#define		MAX_RADIO_JOINT					2

// 라디오버튼 갯수
#define		RADIO_AUDIO_CHAT				0	// 음성 채팅방식
#define		RADIO_AUDIO_KEY					1	// 키 입력방식
#define		RADIO_AUDIO_FREE				2	// 자유방식
#define		MAX_RADIO_AUDIO					3

// 경험치 분배 콤보 박스
#define		COMBO_DIVISION_EXP			0	// 경험치
#define		COMBO_DIVISION_ITEM			1	// 아이템 분배
#define		MAX_COMBO_DIVISION			2

// 편대 비행 갯수
//자유 비행은 이미지 없음 
#define COMBO_FLIGHT_FORM_2_COLUMN			0	// 이렬 종대, 이렬 종대 모양으로 두 줄로 나란히 선 모양이다
#define COMBO_FLIGHT_FORM_2_LINE			1	// 이렬 횡대, 이렬 횡대 모양으로 두 줄로 나란히 선 모양이다
#define COMBO_FLIGHT_FORM_TRIANGLE			2	// 삼각 편대, 삼각형 모양으로 상단부터 1, 2, 3개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_INVERTED_TRIANGLE	3	// 역삼각 편대, 역 삼각형 모양으로 상단부터 3, 2, 1개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_BELL				4	// 종 형태, 종 모양으로 상단부터 1, 3, 2개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_INVERTED_BELL		5	// 역종 형태, 역종 모양으로 상단부터 2, 3, 1개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_X					6 // X자 형태
#define COMBO_FLIGHT_FORM_STAR				7	// 별 형태
#define MAX_COMBO_FLIGHT_FORM				8

class CINFImageRadioBtn;
class CINFListBox;
class CINFSecuEditBox;
class CINFNumEditBox;
class CINFEditBox;
// 2009-04-23 by bhsohn 에디트창 커서 이동
class CINFSingleCurselEditBox;
// end 2009-04-23 by bhsohn 에디트창 커서 이동
class CINFGroupImage;
class CINFImageEx;

class CINFCommuPartyCre  : public CINFBase
{
public:
	CINFCommuPartyCre(CAtumNode* pParent);
	virtual ~CINFCommuPartyCre();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();
	SPARTY_INFO* GetSPartyCreateInfo();
	// 2008-12-09 by dgwoo 미션마스터.
	void MissionMasterParty(char * i_pString);
	BOOL IsMissionMaster()		{			return m_bMissionMaster;	}
	void SetMissionMaster(BOOL i_bMissionMaster);
	void SetMissionMasterUID(UID64_t i_nUID);
	UID64_t GetMissionMasterUID() {return m_nMissionMasterUID;	}
	void SetAutoCreateParty(SPARTY_INFO i_PartyInfo);		/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 자동으로 파티만들기.
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();

	void OnClickJoint(int nId);
	void OnClickAudioOp(int nId);
	
	void UpdateRadioBtn();	
	void AllEditBoxDisable();

	void LoadSPartyInfo(SPARTY_INFO *pSPartyInfo);
	void InitCotrol();
	void SetVoiceChatVolum(DWORD i_nVolum);

	// 2009-04-23 by bhsohn 에디트창 커서 이동
	int WndProcEdit(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// end 2009-04-23 by bhsohn 에디트창 커서 이동
private:
	//CINFImage*	m_pBkImage;		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pBkImage;																	  
#else								
    CINFImageEx*	m_pBkImage;
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pCloseBtn;			// 닫기
	CINFImageBtn*			m_pOkBtn;				// 생성

	// 참여방식
	CINFImageRadioBtn*		m_pPartyJoint[MAX_RADIO_JOINT];
	int						m_nJointIdx;

	// 음성 옵션
	CINFImageRadioBtn*		m_pAudioOp[MAX_RADIO_AUDIO];
	int						m_nAduioIdx;

	// 스피커 볼륨 조절.
	//CINFImageEx*				m_pImgSpkVolBar;

	//DWORD						m_nVolum;
	//BOOL						m_bVolumCon;							// 볼륨소리 수정중.
	//float						m_fVolumConTimeGap;						// 
	//POINT						m_ptOldVolumPos;						// 기존의 볼륨 위치.
	//INT							m_nPosVolum;							// 기존위치(0) + m_nPosVolum      : m_nPosVolum = (m_nVolum / VOICECHAT_SPK_W) * 100
	// 스피커 볼륨 조절.

	CINFListBox*			m_pComboDivision[MAX_COMBO_DIVISION];	// 분배 방식

	CINFListBox*			m_pComboForm;							// 편대 비행
	CINFImageEx*			m_pFormatImage[MAX_COMBO_FLIGHT_FORM];		

	CD3DHanFont*			m_pFontText;	
	
	CINFSecuEditBox*		m_pINFSecuEditBox;
	CINFNumEditBox*			m_pNumMinEditBox;	// 최소 레벨
	CINFNumEditBox*			m_pNumMaxEditBox;	// 최대 레벨
	// 2009-04-23 by bhsohn 에디트창 커서 이동
	//CINFEditBox*			m_pEditPartyName;
	CINFSingleCurselEditBox*			m_pEditPartyName;
	// end 2009-04-23 by bhsohn 에디트창 커서 이동

	SPARTY_INFO				m_stSPartyCreate;

// 2008-12-09 by dgwoo 미션마스터.
	BOOL					m_bMissionMaster;
	UID64_t					m_nMissionMasterUID;
};

#endif // !defined(AFX_INFCOMMUPARTYCRE_H__63861C02_6AC5_4D66_8FC1_21BBD65AA0BE__INCLUDED_)
