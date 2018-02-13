// INFCommuPartyCre.cpp: implementation of the CINFCommuPartyCre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "INFCommunity.h"
#include "INFImageRadioBtn.h"
#include "INFListBox.h"
#include "INFSecuEditBox.h"
#include "INFNumEditBox.h"
#include "INFEditBox.h"
// 2009-04-23 by bhsohn 에디트창 커서 이동
#include "INFSingleCurselEditBox.h"
// end 2009-04-23 by bhsohn 에디트창 커서 이동
#include "ClientParty.h"

#include "INFCommuPartyCre.h"

// 2009. 08. 11 by ckPark 편대 대형 스킬
#include "AtumDatabase.h"
// end 2009. 08. 11 by ckPark 편대 대형 스킬
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"			 // 2011. 10. 10 by jskim UI시스템 변경

#include "INFToolTip.h"					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 커뮤니티 창 옵션
#define	COMMUNITY_OPCREBK_X				230
#define	COMMUNITY_OPCREBK_Y				0
#define	COMMUNITY_OPCREBK_WIDTH			290
#define	COMMUNITY_OPCREBK_HEIGHT		452
#define	COMMUNITY_OPCREBK_CAPS_HEIGHT	20

// 초대
#define	COMMUNITY_OPCRE_INVITE_X			24
#define	COMMUNITY_OPCRE_INVITE_Y			560
// 탈퇴
#define	COMMUNITY_OPCRE_LEAVE_X			85
#define	COMMUNITY_OPCRE_LEAVE_Y			560
// 추방
#define	COMMUNITY_OPCRE_PURGE_X			146
#define	COMMUNITY_OPCRE_PURGE_Y			560
// 위임
#define	COMMUNITY_OPCRE_CHARE_X			207
#define	COMMUNITY_OPCRE_CHARE_Y			560
// 확인 
#define	COMMUNITY_OPCRE_OK_X				82
#define	COMMUNITY_OPCRE_OK_Y				426
// 닫기
#define	COMMUNITY_OPCRE_CLOSE_X			20
#define	COMMUNITY_OPCRE_CLOSE_Y			426

// 참여 방식 라디오
#define	PARTYCRE_JOIN_X				36
#define	PARTYCRE_JOIN_Y				75
#define	PARTYCRE_JOIN_CAP_Y			18

// 음성 옵션 라디오
#define	PARTYCRE_AUDIO_X				39
#define	PARTYCRE_AUDIO_Y				231
#define	PARTYCRE_AUDIO_CAP_Y			18

// 콤보 박스 
#define DIVISION_COMBO_MAIN_X			58
#define DIVISION_COMBO_MAIN_Y			248
#define DIVISION_COMBO_MAIN_WIDTH		110
#define DIVISION_COMBO_MAIN_HEIGHT		17
#define DIVISION_COMBO_ELE_WIDTH		110
#define DIVISION_COMBO_ELE_HEIGHT		15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 13에서 15로 수정

// 편대 포맷 설명
#define	COMMUNITY_FORMATTXT_X				23
#define	COMMUNITY_FORMATTXT_Y				355
#define	COMMUNITY_FORMATTXT_WIDTH			130
#define	COMMUNITY_FORMATTXT_LINE			4
#define	COMMUNITY_FORMATTXT_CAP_Y			18
// 편대 포맷
#define	COMMUNITY_FORMAT_X				171
#define	COMMUNITY_FORMAT_Y				343

// Edit 컨트롤 (Pass)
#define		SECURITY_WND_PASS_EDIT_X		58
#define		SECURITY_WND_PASS_EDIT_Y		138
#define		SECURITY_WND_PASS_EDIT_W		200
#define		SECURITY_WND_PASS_EDIT_H		20
#define		MAX_SECURITY_STRING_LEN			8

// 최소 에디트 컨트롤
#define		MIN_NUM_EDIT_X		(50)
#define		MIN_NUM_EDIT_Y		(174)
#define		MIN_NUM_EDIT_W		(35)
#define		MIN_NUM_EDIT_H		(20)

// 최대 에디트 컨트롤
#define		MAX_NUM_EDIT_X		(110)
#define		MAX_NUM_EDIT_Y		(174)
#define		MAX_NUM_EDIT_W		(35)
#define		MAX_NUM_EDIT_H		(20)

// 여단 소개
#define		PARTY_NAME_EDIT_X						97
#define		PARTY_NAME_EDIT_Y						37
#define		PARTY_NAME_EDIT_W						150
#define		PARTY_NAME_FONT_LINE_HEIGHT			15

#define		MINMAX_LV_CAP			14

#define VOICECHAT_SPK_POS_X			(m_ptCommOpBk.x + 57)
#define VOICECHAT_SPK_POS_Y			(m_ptCommOpBk.y + 294)
#define VOICECHAT_SPK_W				65
#define VOICECHAT_SPK_CONTROL_TIME_GAP		0.25f

#define EDIT_SIZE_MAX_PARTY_NAME		20	  
#else
// 커뮤니티 창 옵션
#define	COMMUNITY_OPCREBK_X				230
#define	COMMUNITY_OPCREBK_Y				0
#define	COMMUNITY_OPCREBK_WIDTH			290
#define	COMMUNITY_OPCREBK_HEIGHT		452
#define	COMMUNITY_OPCREBK_CAPS_HEIGHT	20

// 초대
#define	COMMUNITY_OPCRE_INVITE_X			24
#define	COMMUNITY_OPCRE_INVITE_Y			560
// 탈퇴
#define	COMMUNITY_OPCRE_LEAVE_X			85
#define	COMMUNITY_OPCRE_LEAVE_Y			560
// 추방
#define	COMMUNITY_OPCRE_PURGE_X			146
#define	COMMUNITY_OPCRE_PURGE_Y			560
// 위임
#define	COMMUNITY_OPCRE_CHARE_X			207
#define	COMMUNITY_OPCRE_CHARE_Y			560
// 확인 
#define	COMMUNITY_OPCRE_OK_X				82
#define	COMMUNITY_OPCRE_OK_Y				426
// 닫기
#define	COMMUNITY_OPCRE_CLOSE_X			149
#define	COMMUNITY_OPCRE_CLOSE_Y			426

// 참여 방식 라디오
#define	PARTYCRE_JOIN_X				39
#define	PARTYCRE_JOIN_Y				77
#define	PARTYCRE_JOIN_CAP_Y			18

// 음성 옵션 라디오
#define	PARTYCRE_AUDIO_X				39
#define	PARTYCRE_AUDIO_Y				231
#define	PARTYCRE_AUDIO_CAP_Y			18

// 콤보 박스 
#define DIVISION_COMBO_MAIN_WIDTH			110
#define DIVISION_COMBO_MAIN_HEIGHT		17
#define DIVISION_COMBO_ELE_WIDTH			110
#define DIVISION_COMBO_ELE_HEIGHT			15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 13에서 15로 수정

// 편대 포맷 설명
#define	COMMUNITY_FORMATTXT_X				23
#define	COMMUNITY_FORMATTXT_Y				355
#define	COMMUNITY_FORMATTXT_WIDTH			130
#define	COMMUNITY_FORMATTXT_LINE			4
#define	COMMUNITY_FORMATTXT_CAP_Y			18
// 편대 포맷
#define	COMMUNITY_FORMAT_X				171
#define	COMMUNITY_FORMAT_Y				343

// Edit 컨트롤 (Pass)
#define		SECURITY_WND_PASS_EDIT_X		124
#define		SECURITY_WND_PASS_EDIT_Y		117
#define		SECURITY_WND_PASS_EDIT_W		200
#define		SECURITY_WND_PASS_EDIT_H		20
#define		MAX_SECURITY_STRING_LEN			8

// 최소 에디트 컨트롤
#define		MIN_NUM_EDIT_X		(124)
#define		MIN_NUM_EDIT_Y		(135)
#define		MIN_NUM_EDIT_W		(35)
#define		MIN_NUM_EDIT_H		(20)

// 최대 에디트 컨트롤
#define		MAX_NUM_EDIT_X		(184)
#define		MAX_NUM_EDIT_Y		(135)
#define		MAX_NUM_EDIT_W		(35)
#define		MAX_NUM_EDIT_H		(20)

// 여단 소개
#define		PARTY_NAME_EDIT_X						97
#define		PARTY_NAME_EDIT_Y						37
#define		PARTY_NAME_EDIT_W						150
#define		PARTY_NAME_FONT_LINE_HEIGHT			15

#define		MINMAX_LV_CAP			14

#define VOICECHAT_SPK_POS_X			(m_ptCommOpBk.x + 62)
#define VOICECHAT_SPK_POS_Y			(m_ptCommOpBk.y + 294)
#define VOICECHAT_SPK_W				65
#define VOICECHAT_SPK_CONTROL_TIME_GAP		0.25f

#define EDIT_SIZE_MAX_PARTY_NAME		20

#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommuPartyCre::CINFCommuPartyCre(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pBkImage = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pCloseBtn = NULL;
	m_pOkBtn = NULL;

	// 게임옵션	
	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{
			m_pPartyJoint[nCnt] = NULL;
		}
	}

	{
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			m_pAudioOp[nCnt] = NULL;
		}
	}

	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{
		m_pComboDivision[nCnt] = NULL;
	}
	
	
	m_pComboForm = NULL;

	m_nJointIdx = RADIO_JOINT_COMMAND;
	m_nAduioIdx = RADIO_AUDIO_KEY;

	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{
		m_pFormatImage[nCnt] = NULL;
	}

	m_pFontText = NULL;
	m_pINFSecuEditBox = NULL;
	m_pNumMinEditBox= NULL;
	m_pNumMaxEditBox= NULL;
	m_pEditPartyName = NULL;	

	memset(&m_stSPartyCreate, 0x00, sizeof(SPARTY_INFO));

	m_stSPartyCreate.FormationType = FLIGHT_FORM_2_COLUMN;	// 기본은 이열 종대로
	//m_pImgSpkVolBar = NULL;
// 2008-12-09 by dgwoo 미션마스터.
	m_bMissionMaster = FALSE;
	m_nMissionMasterUID = 0;
}

CINFCommuPartyCre::~CINFCommuPartyCre()
{
	util::del(m_pBkImage);
		
	util::del(m_pCloseBtn);
	util::del(m_pOkBtn);	
	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			util::del(m_pPartyJoint[nCnt]);		
		}
		{
			for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
			{
				util::del(m_pAudioOp[nCnt]);
			}
		}
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{
		util::del(m_pComboDivision[nCnt]);
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{
		util::del(m_pFormatImage[nCnt]);
	}
	util::del(m_pComboForm);

	util::del(m_pFontText);	
	util::del(m_pINFSecuEditBox);
	util::del(m_pNumMinEditBox);
	util::del(m_pNumMaxEditBox);
	util::del(m_pEditPartyName);
}

HRESULT CINFCommuPartyCre::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;
	int nCnt = 0;

	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	// 커뮤니티 옵션배	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPCREBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPCREBK_HEIGHT/2;


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pBkImage == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("F_cre");	
		m_pBkImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBkImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pBkImage->RestoreDeviceObjects();
	}
#else		  
	{
		if(NULL == m_pBkImage)
		{
			m_pBkImage = new CINFImageEx;	
		}		
		pDataHeader = FindResource("p_bk2");
		m_pBkImage->InitDeviceObjects(pDataHeader);
	}
#endif

	
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{		
		char chBuff[16];
		if(NULL == m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt] = new CINFImageEx;	
		}				
		wsprintf(chBuff, "c_pf%d", nCnt);
		pDataHeader = FindResource(chBuff);

		m_pFormatImage[nCnt]->InitDeviceObjects( pDataHeader );
	}
		
	{
		// 확인
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "cres03");
		wsprintf(szDownBtn, "cres01");
		wsprintf(szSelBtn, "cres00");
		wsprintf(szDisBtn, "cres02");															  
#else									
		wsprintf(szUpBtn, "createb3");
		wsprintf(szDownBtn, "createb1");
		wsprintf(szSelBtn, "createb0");
		wsprintf(szDisBtn, "createb2");
#endif
		if(NULL == m_pOkBtn)
		{
			m_pOkBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn, "STRTOOLTIP47");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	{
		// 닫기
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");															  
#else	
		wsprintf(szUpBtn, "shnpc063");
		wsprintf(szDownBtn, "shnpc061");
		wsprintf(szSelBtn, "shnpc060");
		wsprintf(szDisBtn, "shnpc062");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP41");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	
	{
		char chRadioOff[30], chRadioOn[30];
		
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");

		char chRadioOff1[30], chRadioOn1[30];
		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(chRadioOff1, "radio_17b");
		wsprintf(chRadioOn1, "radio_17a");														  
#else			   
		wsprintf(chRadioOff1, "radio_b");
		wsprintf(chRadioOn1, "radio_a");
#endif
				
		
		{
			for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
			{		
				if(NULL == m_pPartyJoint[nCnt])
				{		
					m_pPartyJoint[nCnt] = new CINFImageRadioBtn;
				}				
				m_pPartyJoint[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
			}		
			for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
			{		
				if(NULL == m_pAudioOp[nCnt])
				{		
					m_pAudioOp[nCnt] = new CINFImageRadioBtn;
				}		
				if(RADIO_AUDIO_CHAT == nCnt)
				{
					m_pAudioOp[nCnt]->InitDeviceObjects(chRadioOff1, chRadioOn1);				
				}
				else
				{
					m_pAudioOp[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
				}
			}
			
		}		
	}
	{
		char chTmpBuff[256];
		memset(chTmpBuff, 0x00, 256);

		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			if(NULL == m_pComboDivision[nCnt])
			{
				//m_pComboDivision[nCnt] = new CINFListBox("cbopa","s_combo");				
				m_pComboDivision[nCnt] = new CINFListBox("c_cc","c_ccbk");

				m_pComboDivision[nCnt]->InitDeviceObjects();				
			}			
		}
		
		{
			wsprintf(chTmpBuff, STRMSG_C_080603_0200);		// "개인 습득"
			m_pComboDivision[COMBO_DIVISION_EXP]->AddElement(chTmpBuff);	// 
			wsprintf(chTmpBuff, STRMSG_C_080603_0201);		// "공동 분배"
			m_pComboDivision[COMBO_DIVISION_EXP]->AddElement(chTmpBuff);	// 
		}
		{
			wsprintf(chTmpBuff, STRMSG_C_080603_0200);		// "개인 습득"
			m_pComboDivision[COMBO_DIVISION_ITEM]->AddElement(chTmpBuff);	// 
			wsprintf(chTmpBuff, STRMSG_C_080603_0202);		// "순차 습득"
			m_pComboDivision[COMBO_DIVISION_ITEM]->AddElement(chTmpBuff);	// 
			wsprintf(chTmpBuff, STRMSG_C_080603_0203);		// "랜덤 습득"
			m_pComboDivision[COMBO_DIVISION_ITEM]->AddElement(chTmpBuff);	// 
		}
		
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			m_pComboDivision[nCnt]->UpdateItem();				
		}
	}

	// 편대 비행
	{
		char chTmpBuff[256];
		memset(chTmpBuff, 0x00, 256);

		if(NULL == m_pComboForm)
		{				
			m_pComboForm = new CINFListBox("c_cc","c_ccbk");

			m_pComboForm->InitDeviceObjects();				
		}			

		{
			// 포지션
			sprintf(chTmpBuff, STRMSG_C_PARTY_0033);	// 편대 비행 안 함
			m_pComboForm->AddElement(chTmpBuff);
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0034);	// 이렬 종대, 이렬 종대 모양으로 두 줄로 나란히 선 모양이다
			m_pComboForm->AddElement(chTmpBuff);	
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0035);	// 이렬 횡대, 이렬 횡대 모양으로 두 줄로 나란히 선 모양이다
			m_pComboForm->AddElement(chTmpBuff);	
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0036);	// 삼각 편대, 삼각형 모양으로 상단부터 1, 2, 3개의 유닛이 위치한다
			m_pComboForm->AddElement(chTmpBuff);	
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0037);	// 역삼각 편대, 역 삼각형 모양으로 상단부터 3, 2, 1개의 유닛이 위치한다
			m_pComboForm->AddElement(chTmpBuff);
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0038);	// 종 형태, 종 모양으로 상단부터 1, 3, 2개의 유닛이 위치한다
			m_pComboForm->AddElement(chTmpBuff);	
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0039);	// 역종 형태, 역종 모양으로 상단부터 2, 3, 1개의 유닛이 위치한다
			m_pComboForm->AddElement(chTmpBuff);	
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0040);	// X자 형태
			m_pComboForm->AddElement(chTmpBuff);	
			
			sprintf(chTmpBuff, STRMSG_C_PARTY_0041);	// 별 형태
			m_pComboForm->AddElement(chTmpBuff);		
			
			m_pComboForm->UpdateItem();
		}		
	}

	{
		m_pFontText = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontText->InitDeviceObjects(g_pD3dDev);
	}
	{
		if(NULL == m_pINFSecuEditBox)
		{
			m_pINFSecuEditBox = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_WND_PASS_EDIT_X, SECURITY_WND_PASS_EDIT_Y};
		m_pINFSecuEditBox->InitDeviceObjects(9, ptPos, SECURITY_WND_PASS_EDIT_W, TRUE, SECURITY_WND_PASS_EDIT_H);		

		m_pINFSecuEditBox->SetMaxStringLen(SIZE_MAX_TEAM_PW-1);
		
	}
	{
		if(NULL == m_pNumMinEditBox)
		{
			m_pNumMinEditBox = new CINFNumEditBox;
		}
		char chBuff[32];
		char chMaxMixCnt[64];

		wsprintf(chBuff, "1");
		wsprintf(chMaxMixCnt, "%d", CHARACTER_MAX_LEVEL);

		POINT ptPos = {MIN_NUM_EDIT_X, MIN_NUM_EDIT_Y};
		m_pNumMinEditBox->InitDeviceObjects(9, ptPos, MIN_NUM_EDIT_W, TRUE, MIN_NUM_EDIT_H);				
		m_pNumMinEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pNumMinEditBox->SetString(chBuff, 32);
	}

	{
		if(NULL == m_pNumMaxEditBox)
		{
			m_pNumMaxEditBox = new CINFNumEditBox;
		}
		char chBuff[32];
		char chMaxMixCnt[64];

		wsprintf(chBuff, "100");
		wsprintf(chMaxMixCnt, "%d", CHARACTER_MAX_LEVEL);

		POINT ptPos = {MAX_NUM_EDIT_X, MAX_NUM_EDIT_Y};
		m_pNumMaxEditBox->InitDeviceObjects(9, ptPos, MAX_NUM_EDIT_W, TRUE, MAX_NUM_EDIT_H);				
		m_pNumMaxEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pNumMaxEditBox->SetString(chBuff, 32);
	}
	{
		if(NULL == m_pEditPartyName)
		{
			// 2009-04-23 by bhsohn 에디트창 커서 이동
			//m_pEditPartyName = new CINFEditBox;
			m_pEditPartyName = new CINFSingleCurselEditBox;
			
		}
		POINT ptPos = {PARTY_NAME_EDIT_X, PARTY_NAME_EDIT_Y};		
		m_pEditPartyName->InitDeviceObjects(9, ptPos, PARTY_NAME_EDIT_W, TRUE, PARTY_NAME_FONT_LINE_HEIGHT, 
										TRUE, PARTY_NAME_FONT_LINE_HEIGHT);				
		m_pEditPartyName->SetStringMaxBuff(EDIT_SIZE_MAX_PARTY_NAME);				
	}
	//m_pImgSpkVolBar = new CINFImageEx;
	//pDataHeader = FindResource("scrl_b");
	//m_pImgSpkVolBar->InitDeviceObjects( pDataHeader );

	
	
	return S_OK ;
}

HRESULT CINFCommuPartyCre::RestoreDeviceObjects()
{
	if(m_pBkImage )
	{
		m_pBkImage->RestoreDeviceObjects();
	}

	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();						
	}
	if(m_pOkBtn)
	{
		m_pOkBtn->RestoreDeviceObjects();						
	}

	int nCnt = 0;
	{			
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{	
			if(m_pPartyJoint[nCnt])
			{
				m_pPartyJoint[nCnt]->RestoreDeviceObjects();			
			}
		}
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{				
			if(m_pAudioOp[nCnt])
			{
				m_pAudioOp[nCnt]->RestoreDeviceObjects();			
			}
		}		
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{	
		if(m_pComboDivision[nCnt])
		{
			m_pComboDivision[nCnt]->RestoreDeviceObjects();			
		}
	}
	if(m_pComboForm)
	{
		m_pComboForm->RestoreDeviceObjects();			
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{	
		if(m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt]->RestoreDeviceObjects();
		}		
	}
	if(m_pFontText)
	{
		m_pFontText->RestoreDeviceObjects();
	}
	if(m_pINFSecuEditBox)
	{
		m_pINFSecuEditBox->RestoreDeviceObjects();		
	}
	if(m_pNumMinEditBox)
	{
		m_pNumMinEditBox->RestoreDeviceObjects();		
	}
	if(m_pNumMaxEditBox)
	{
		m_pNumMaxEditBox->RestoreDeviceObjects();		
	}
	if(m_pEditPartyName)
	{
		m_pEditPartyName->RestoreDeviceObjects();		
	}	
	//if(m_pImgSpkVolBar)
	//{
	//	m_pImgSpkVolBar->RestoreDeviceObjects();
	//}
	
	
	// UI유저 지정 
	UpdateUIPos();

	
	return S_OK ;
}

HRESULT CINFCommuPartyCre::DeleteDeviceObjects()
{
	if(m_pBkImage )
	{
		m_pBkImage->DeleteDeviceObjects();
		util::del(m_pBkImage );
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	if(m_pOkBtn)
	{		
		m_pOkBtn->DeleteDeviceObjects();
		util::del(m_pOkBtn);
	}
	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			if(m_pPartyJoint[nCnt])
			{
				m_pPartyJoint[nCnt]->DeleteDeviceObjects();	
				util::del(m_pPartyJoint[nCnt]);
			}
		}
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{				
			if(m_pAudioOp[nCnt])
			{
				m_pAudioOp[nCnt]->DeleteDeviceObjects();	
				util::del(m_pAudioOp[nCnt]);
			}
		}		
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{			
		if(m_pComboDivision[nCnt])
		{
			m_pComboDivision[nCnt]->DeleteDeviceObjects();	
			util::del(m_pComboDivision[nCnt]);
		}
	}	
	if(m_pComboForm)
	{
		m_pComboForm->DeleteDeviceObjects();	
		util::del(m_pComboForm);
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{	
		if(m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt]->DeleteDeviceObjects();	
			util::del(m_pFormatImage[nCnt]);
		}		
	}
	if(m_pFontText)
	{
		m_pFontText->DeleteDeviceObjects();
		util::del(m_pFontText);
	}
	if(m_pINFSecuEditBox)
	{
		m_pINFSecuEditBox->DeleteDeviceObjects();	
		util::del(m_pINFSecuEditBox);
	}
	if(m_pNumMinEditBox)
	{
		m_pNumMinEditBox->DeleteDeviceObjects();	
		util::del(m_pNumMinEditBox);
	}		
	if(m_pNumMaxEditBox)
	{
		m_pNumMaxEditBox->DeleteDeviceObjects();	
		util::del(m_pNumMaxEditBox);
	}
	if(m_pEditPartyName)
	{
		m_pEditPartyName->DeleteDeviceObjects();
		util::del(m_pEditPartyName);		
	}	
	//if(m_pImgSpkVolBar)
	//{
	//	m_pImgSpkVolBar->DeleteDeviceObjects();
	//	util::del(m_pImgSpkVolBar);
	//}



	return S_OK ;
}

HRESULT CINFCommuPartyCre::InvalidateDeviceObjects()
{
	if(m_pBkImage)
	{
		m_pBkImage->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
	if(m_pOkBtn)
	{		
		m_pOkBtn->InvalidateDeviceObjects();		
	}
	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{		
			if(m_pPartyJoint[nCnt])
			{
				m_pPartyJoint[nCnt]->InvalidateDeviceObjects();
			}
		}
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{				
			if(m_pAudioOp[nCnt])
			{
				m_pAudioOp[nCnt]->InvalidateDeviceObjects();				
			}
		}
	}
	for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
	{			
		if(m_pComboDivision[nCnt])
		{
			m_pComboDivision[nCnt]->InvalidateDeviceObjects();
		}
	}
	if(m_pComboForm)
	{
		m_pComboForm->InvalidateDeviceObjects();
	}
	for(nCnt = 0;nCnt < MAX_COMBO_FLIGHT_FORM;nCnt++)
	{	
		if(m_pFormatImage[nCnt])
		{
			m_pFormatImage[nCnt]->InvalidateDeviceObjects();				
		}		
	}
	if(m_pFontText)
	{
		m_pFontText->InvalidateDeviceObjects();
	}
	if(m_pINFSecuEditBox)
	{
		m_pINFSecuEditBox->InvalidateDeviceObjects();			
	}
	if(m_pNumMinEditBox)
	{
		m_pNumMinEditBox->InvalidateDeviceObjects();
	}
	if(m_pNumMaxEditBox)
	{
		m_pNumMaxEditBox->InvalidateDeviceObjects();
	}
	if(m_pEditPartyName)
	{
		m_pEditPartyName->InvalidateDeviceObjects();
	}	
	//if(m_pImgSpkVolBar)
	//{
	//	m_pImgSpkVolBar->InvalidateDeviceObjects();
	//}

	return S_OK ;
}

void CINFCommuPartyCre::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	POINT ptBkPos = m_ptCommOpBk;
	if(m_pBkImage)
	{		
		m_pBkImage->Move(ptBkPos.x, ptBkPos.y);
		m_pBkImage->Render() ;
	}
	
	m_pCloseBtn->Render();		
	m_pOkBtn->Render();		

	int nCnt =0;	
	{
		for(nCnt =0; nCnt < MAX_RADIO_JOINT; nCnt++)
		{		
			m_pPartyJoint[nCnt]->Render();
		}	
		for(nCnt = 0;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{			
			m_pAudioOp[nCnt]->Render();							
		}
	}
	// 콤보박스는 가장 마지막에 랜더링 	
	{
		int nSelCombo = -1;
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			if(m_pComboDivision[nCnt]->IsShowItem())
			{
				nSelCombo = nCnt;
				break;
			}
		}
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{	
			if(nCnt != nSelCombo)
			{
				m_pComboDivision[nCnt]->Render();
			}
		}
		if(nSelCombo != -1)
		{
			m_pComboDivision[nSelCombo]->Render();
		}
	}
	
	{
		char chBuff[512];
		memset(chBuff, 0x00, 512);
		

		// 2009. 08. 11 by ckPark 편대 대형 스킬
		//((CINFCommunity*)m_pParent)->GetFormatString(m_pComboForm->GetSelect(), chBuff);

		// 편대 대형 스킬 설명
		int nSkillNum = CAtumSJ::FindPartyFormationSkillNum( m_pComboForm->GetSelect() );
		if( nSkillNum )
		{
			ITEM* pSkillItem = g_pDatabase->GetServerItemInfo( nSkillNum );
			if( pSkillItem )
				strcpy( chBuff, pSkillItem->Description );
		}
		// end 2009. 08. 11 by ckPark 편대 대형 스킬


		vector<string> vecTmp;

		g_pGameMain->StringCulling(chBuff, COMMUNITY_FORMATTXT_WIDTH, &vecTmp, m_pFontText);
		vector<string>::iterator itTmp = vecTmp.begin();
		for(nCnt = 0; nCnt < COMMUNITY_FORMATTXT_LINE;nCnt++)
		{
			if(itTmp == vecTmp.end())
			{
				break;
			}
			char* pTxt = (char*)(*itTmp).c_str();

			if(pTxt)
			{
				m_pFontText->DrawText(ptBkPos.x+COMMUNITY_FORMATTXT_X, 
					ptBkPos.y+COMMUNITY_FORMATTXT_Y + (COMMUNITY_FORMATTXT_CAP_Y*nCnt), 
					GUI_FONT_COLOR, 
					pTxt);
			}
			itTmp++;
		}
		
		int nSelImage = m_pComboForm->GetSelect() - 1;
		if(nSelImage >= 0 && nSelImage< MAX_COMBO_FLIGHT_FORM)
		{		
			m_pFormatImage[nSelImage]->Move(ptBkPos.x+COMMUNITY_FORMAT_X,ptBkPos.y+COMMUNITY_FORMAT_Y);				
			m_pFormatImage[nSelImage]->Render();
		}
		m_pComboForm->Render();
	}
	//m_pImgSpkVolBar->Move(VOICECHAT_SPK_POS_X + m_nPosVolum,VOICECHAT_SPK_POS_Y);
	//m_pImgSpkVolBar->Render();

	m_pINFSecuEditBox->Render();
	m_pNumMinEditBox->Render();
	m_pNumMaxEditBox->Render();

	m_pEditPartyName->Tick();
	// 2009-04-23 by bhsohn 에디트창 커서 이동
	//m_pEditPartyName->Render();	
	m_pEditPartyName->Render(0, 1);	
	// end 2009-04-23 by bhsohn 에디트창 커서 이동
}

void CINFCommuPartyCre::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}
	m_pINFSecuEditBox->Tick();

}

int CINFCommuPartyCre::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWindow())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommOpBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommOpBk.y;
				m_bMove = TRUE;
				return INF_MSGPROC_BREAK;
			}
						
			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}

			// 콥보박스 아이템이 안보인다.
			if(!m_pComboForm->IsShowItem())
			{
				if(TRUE == m_pOkBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}		

			// 라디오 버튼 처리
			int nCnt = 0;								
			{					
				for(nCnt = 0;nCnt <MAX_RADIO_JOINT;nCnt++)
				{					
					if(TRUE == m_pPartyJoint[nCnt]->OnLButtonDown(pt))
					{						
						OnClickJoint(nCnt);
						return  INF_MSGPROC_BREAK;
					}		
				}
//				if(TRUE == m_pAudioOp[RADIO_AUDIO_CHAT]->OnLButtonDown(pt))
//				{
//					BOOL bCheck = m_pAudioOp[RADIO_AUDIO_CHAT]->GetRadioBtn();
//					bCheck ^= TRUE;
//					m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(bCheck);
//					return  INF_MSGPROC_BREAK;
//				}		
				for(nCnt = RADIO_AUDIO_KEY;nCnt < MAX_RADIO_AUDIO;nCnt++)
				{					
					if(TRUE == m_pAudioOp[nCnt]->OnLButtonDown(pt))
					{
						OnClickAudioOp(nCnt);
						return  INF_MSGPROC_BREAK;
					}		
				}				
			}

			// 
			int nGraphicOptionSel = -1;
			{
				for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
				{
					int nLBtnDown = m_pComboDivision[nCnt]->LButtonDown(pt);
					if(-1 != nLBtnDown)				
					{
						// 안보이다가 보이는 상황
						nGraphicOptionSel = nCnt;
						break;
					}
				}
				// 선택된거를 제외한거 전체를 히든으로
				if(nGraphicOptionSel != -1)
				{
					for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
					{
						if(nCnt != nGraphicOptionSel)
						{
							m_pComboDivision[nCnt]->ShowItem(FALSE);					
						}
					}
					UpdateUIPos();
					return  INF_MSGPROC_BREAK;
				}			
			}
			if(-1 != m_pComboForm->LButtonDown(pt))
			{
				return  INF_MSGPROC_BREAK;
			}

			// 편대장 초대 방식은 비밀 번호가 없다.	
			if(RADIO_JOINT_FREE == m_nJointIdx)	
			{
				if(TRUE == m_pINFSecuEditBox->OnLButtonDown(pt))
				{	
					AllEditBoxDisable();
					m_pINFSecuEditBox->EnableEdit(TRUE);				
					
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}
			}
			{
				if(TRUE == m_pNumMinEditBox->OnLButtonDown(pt))
				{
					AllEditBoxDisable();
					m_pNumMinEditBox->EnableEdit(TRUE);

					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				if(TRUE == m_pNumMaxEditBox->OnLButtonDown(pt))
				{
					AllEditBoxDisable();
					m_pNumMaxEditBox->EnableEdit(TRUE);
					
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}
			}
			if(TRUE == m_pEditPartyName->OnLButtonDown(pt))
			{				
				AllEditBoxDisable();
				m_pEditPartyName->EnableEdit(TRUE, TRUE);
				return INF_MSGPROC_BREAK;
			}
			// 2008-10-20 by bhsohn VOIP 버그 수정
			// 파티 생성시 볼륨조절 막음
//			if(pt.x > VOICECHAT_SPK_POS_X + m_nPosVolum &&
//				pt.x < VOICECHAT_SPK_POS_X + m_nPosVolum + m_pImgSpkVolBar->GetImgSize().x &&
//				pt.y > VOICECHAT_SPK_POS_Y &&
//				pt.y < VOICECHAT_SPK_POS_Y + m_pImgSpkVolBar->GetImgSize().y)
//			{
//				m_bVolumCon = TRUE;
//				m_ptOldVolumPos = pt;
//			}


			
			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+COMMUNITY_OPCREBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+COMMUNITY_OPCREBK_HEIGHT)))
			{
				// 윈도우 영역위에 있다.
				return  INF_MSGPROC_BREAK;
			}
			
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nCnt = 0;								

			if(m_bMove)
			{
				m_ptCommOpBk.x = pt.x - m_ptCommOpMouse.x;
				m_ptCommOpBk.y = pt.y - m_ptCommOpMouse.y;				
				// UI유저 지정 
				UpdateUIPos();
				return INF_MSGPROC_BREAK;
			}
			
			m_pCloseBtn->OnMouseMove(pt);
			m_pOkBtn->OnMouseMove(pt);	

			for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
			{
				m_pComboDivision[nCnt]->MouseMove(pt);
			}	
			m_pComboForm->MouseMove(pt);	
			//if(m_bVolumCon)
			//{// 볼륨 크기 컨트롤중.
			//	m_nPosVolum += (pt.x - m_ptOldVolumPos.x);
			//	if(m_nPosVolum > VOICECHAT_SPK_W)
			//	{
			//		m_nPosVolum = VOICECHAT_SPK_W;
			//	}else if(m_nPosVolum < 0)
			//	{
			//		m_nPosVolum = 0;
			//	}
			//	DWORD nVolum;
			//	nVolum = (DWORD)(m_nPosVolum/VOICECHAT_SPK_W)*100;
			//	m_ptOldVolumPos = pt;
			//	SetVoiceChatVolum(nVolum);

			//}


		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_bMove)
			{
				m_bMove = FALSE;
				return INF_MSGPROC_BREAK;
			}
			//if(m_bVolumCon)
			//{// 볼륨 크기 컨트롤중.				
			//	if(m_nPosVolum > VOICECHAT_SPK_W)
			//	{
			//		m_nPosVolum = VOICECHAT_SPK_W;
			//	}else if(m_nPosVolum < 0)
			//	{
			//		m_nPosVolum = 0;
			//	}
			//	DWORD nVolum;
			//	nVolum = (DWORD)(m_nPosVolum/VOICECHAT_SPK_W)*100;
			//	// 2008-10-20 by bhsohn VOIP 버그 수정
			//	// 볼륨조절 막음
			//	//g_pGameMain->SetVoiceVolum(nVolum);
			//	m_bVolumCon = FALSE;
			//}

						
			{
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{			
					// 버튼위에 마우스가 있다.
					ShowWindow(FALSE);					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				if(TRUE == m_pOkBtn->OnLButtonUp(pt))
				{						
					memset(&m_stSPartyCreate, 0x00, sizeof(SPARTY_INFO));

					// 2009-04-23 by bhsohn 에디트창 커서 이동
					AllEditBoxDisable();
					// end 2009-04-23 by bhsohn 에디트창 커서 이동
					LoadSPartyInfo(&m_stSPartyCreate);				

					((CINFCommunity*)m_pParent)->OnClickCreateParty();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}			
		}
		break;
	case WM_KEYDOWN:
		{
			// 편대장 초대 방식은 비밀 번호가 없다.	
			if(RADIO_JOINT_FREE == m_nJointIdx)	
			{
				if(m_pINFSecuEditBox->WndProc(uMsg, wParam, lParam))
				{
					return INF_MSGPROC_BREAK;
				}
			}
			
			if(m_pNumMinEditBox->WndProc(uMsg, wParam, lParam))
			{				
				return INF_MSGPROC_BREAK;
			}
			if(m_pNumMaxEditBox->WndProc(uMsg, wParam, lParam))
			{				
				return INF_MSGPROC_BREAK;
			}

			// 2009-04-23 by bhsohn 에디트창 커서 이동
			if(INF_MSGPROC_BREAK == WndProcEdit(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}						
			// end 2009-04-23 by bhsohn 에디트창 커서 이동
		}
		break;
	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:	
		{						
			// 2009-04-23 by bhsohn 에디트창 커서 이동
// 			if(!m_pEditPartyName->IsEditMode())
// 			{
// 				return INF_MSGPROC_NORMAL;		
// 			}			
// 			int nLienCnt  =-1;
// 			BOOL bRtn = m_pEditPartyName->WndProc(uMsg, wParam, lParam, &nLienCnt, TRUE);
// 			if(nLienCnt < 0)
// 			{
// 				// 아무 처리도 안했다.
// 				return INF_MSGPROC_NORMAL;
// 			}
// 			return INF_MSGPROC_BREAK;
			if(INF_MSGPROC_BREAK == WndProcEdit(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}						
			// end 2009-04-23 by bhsohn 에디트창 커서 이동
			
		}
		break;
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommuPartyCre::ShowWindow(BOOL bShow)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// 컨트롤이 움직인다.
	if(bShow)
	{
		POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

		// 커뮤니티 옵션배	
		m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPCREBK_X;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPCREBK_HEIGHT/2;
		// UI유저 지정 
		UpdateUIPos();
		
		// 음성 볼륨.
		//m_nPosVolum = (g_pGameMain->GetVoiceVolum()/100)*VOICECHAT_SPK_W;
		
		InitCotrol();		
	}
	else
	{
		AllEditBoxDisable();
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
}

BOOL CINFCommuPartyCre::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommuPartyCre::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_OPCREBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_OPCREBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommuPartyCre::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nCnt = 0;						
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		nPosX = ptBkPos.x + ( m_pBkImage->GetMaxPos().x - m_pBkImage->GetMinPos().x ) / 2 +  COMMUNITY_OPCRE_CLOSE_X;
		nPosY = ptBkPos.y + COMMUNITY_OPCRE_CLOSE_Y;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + ( m_pBkImage->GetMaxPos().x - m_pBkImage->GetMinPos().x ) / 2 -  COMMUNITY_OPCRE_CLOSE_X - m_pOkBtn->GetImgSize().x;
		nPosY = ptBkPos.y + COMMUNITY_OPCRE_OK_Y;		
		m_pOkBtn->SetBtnPosition(nPosX, nPosY);	
	}	

	{		
		// 참여 방식 라디오		
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_JOIN_X;
			nPosY = ptBkPos.y + PARTYCRE_JOIN_Y + ( nCnt * PARTYCRE_JOIN_CAP_Y );		
			m_pPartyJoint[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
	
	/*{		
		nPosX = ptBkPos.x + PARTYCRE_AUDIO_X;
		nPosY = ptBkPos.y + PARTYCRE_AUDIO_Y;

		m_pAudioOp[RADIO_AUDIO_CHAT]->SetPosition(nPosX, nPosY, 60);		
		// 참여 방식 라디오		
		for(nCnt = RADIO_AUDIO_KEY;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_AUDIO_X;
			nPosY = ptBkPos.y + PARTYCRE_AUDIO_Y+ (nCnt*PARTYCRE_AUDIO_CAP_Y);		
			m_pAudioOp[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}*/

	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		nMainWidth = DIVISION_COMBO_MAIN_WIDTH;
		nMainHeight = DIVISION_COMBO_MAIN_HEIGHT;
		nEleWidth = DIVISION_COMBO_ELE_WIDTH;
		nEleHeight = DIVISION_COMBO_ELE_HEIGHT;
		
		POINT	ptMainArena[MAX_COMBO_DIVISION] =
		{
			{ptBkPos.x+ DIVISION_COMBO_MAIN_X, ptBkPos.y+ DIVISION_COMBO_MAIN_Y },
			{ptBkPos.x+ DIVISION_COMBO_MAIN_X, ptBkPos.y+ DIVISION_COMBO_MAIN_Y + DIVISION_COMBO_MAIN_HEIGHT + 20 },
		};
		
		POINT	ptElementArena[MAX_COMBO_DIVISION] =
		{
			{ptMainArena[0].x, ptMainArena[0].y + nMainHeight},
			{ptMainArena[1].x, ptMainArena[1].y + nMainHeight}			
		};
		
		
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			m_pComboDivision[nCnt]->SetMainArea(ptMainArena[nCnt].x, 
												ptMainArena[nCnt].y,
											nMainWidth, nMainHeight);
			m_pComboDivision[nCnt]->SetElementArea(ptElementArena[nCnt].x,
													ptElementArena[nCnt].y + 2,
													nEleWidth, nEleHeight);
			m_pComboDivision[nCnt]->SetBGPos(ptElementArena[nCnt].x+6, ptElementArena[nCnt].y + 3,
												nEleWidth, nEleHeight);

		}
	}
#else
	{
		nPosX = ptBkPos.x + COMMUNITY_OPCRE_CLOSE_X;
		nPosY = ptBkPos.y + COMMUNITY_OPCRE_CLOSE_Y;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + COMMUNITY_OPCRE_OK_X;
		nPosY = ptBkPos.y + COMMUNITY_OPCRE_OK_Y;		
		m_pOkBtn->SetBtnPosition(nPosX, nPosY);	
	}	

	{		
		// 참여 방식 라디오		
		for(nCnt = 0;nCnt < MAX_RADIO_JOINT;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_JOIN_X;
			nPosY = ptBkPos.y + PARTYCRE_JOIN_Y+ (nCnt*PARTYCRE_JOIN_CAP_Y);		
			m_pPartyJoint[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
	
	{		
		nPosX = ptBkPos.x + PARTYCRE_AUDIO_X+2;
		nPosY = ptBkPos.y + PARTYCRE_AUDIO_Y+ 5;		

		m_pAudioOp[RADIO_AUDIO_CHAT]->SetPosition(nPosX, nPosY, 60);		
		// 참여 방식 라디오		
		for(nCnt = RADIO_AUDIO_KEY;nCnt < MAX_RADIO_AUDIO;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYCRE_AUDIO_X;
			nPosY = ptBkPos.y + PARTYCRE_AUDIO_Y+ (nCnt*PARTYCRE_AUDIO_CAP_Y);		
			m_pAudioOp[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}

	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		nMainWidth = DIVISION_COMBO_MAIN_WIDTH;
		nMainHeight = DIVISION_COMBO_MAIN_HEIGHT;
		nEleWidth = DIVISION_COMBO_ELE_WIDTH;
		nEleHeight = DIVISION_COMBO_ELE_HEIGHT;
		
		POINT	ptMainArena[MAX_COMBO_DIVISION] =
		{
			{ptBkPos.x+138, ptBkPos.y+176},
			{ptBkPos.x+138, ptBkPos.y+194},			
		};
		
		POINT	ptElementArena[MAX_COMBO_DIVISION] =
		{
			{ptMainArena[0].x, ptMainArena[0].y + nMainHeight},
			{ptMainArena[1].x, ptMainArena[1].y + nMainHeight}			
		};
		
		
		for(nCnt = 0;nCnt < MAX_COMBO_DIVISION;nCnt++)
		{
			m_pComboDivision[nCnt]->SetMainArea(ptMainArena[nCnt].x, 
												ptMainArena[nCnt].y,
											nMainWidth, nMainHeight);
			m_pComboDivision[nCnt]->SetElementArea(ptElementArena[nCnt].x,
													ptElementArena[nCnt].y,
													nEleWidth, nEleHeight);
			m_pComboDivision[nCnt]->SetBGPos(ptElementArena[nCnt].x+6, ptElementArena[nCnt].y,
												nEleWidth, nEleHeight);
		}
	}
#endif
	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		nMainWidth = DIVISION_COMBO_MAIN_WIDTH;
		nMainHeight = DIVISION_COMBO_MAIN_HEIGHT;
		nEleWidth = DIVISION_COMBO_ELE_WIDTH;
		nEleHeight = DIVISION_COMBO_ELE_HEIGHT;
		
		POINT	ptMainArena = 
		{
			ptBkPos.x+22, ptBkPos.y+335
		};
		
		POINT	ptElementArena =
		{
			ptMainArena.x, ptMainArena.y + nMainHeight
		};
		
		
		m_pComboForm->SetMainArea(ptMainArena.x, 
										ptMainArena.y,
										nMainWidth, nMainHeight);
		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +2 추가
		m_pComboForm->SetElementArea(ptElementArena.x,
										ptElementArena.y+2,
										nEleWidth, nEleHeight);
		// end 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +2 추가

		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +4 추가
		m_pComboForm->SetBGPos(ptElementArena.x+6, ptElementArena.y+4,
												nEleWidth, nEleHeight);
		// end 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +4 추가
	}

	{
		nPosX = ptBkPos.x + SECURITY_WND_PASS_EDIT_X;
		nPosY = ptBkPos.y + SECURITY_WND_PASS_EDIT_Y;
		m_pINFSecuEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + MIN_NUM_EDIT_X;
		nPosY = ptBkPos.y + MIN_NUM_EDIT_Y;		
		m_pNumMinEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + MAX_NUM_EDIT_X;
		nPosY = ptBkPos.y + MAX_NUM_EDIT_Y;		
		m_pNumMaxEditBox->SetPos(nPosX, nPosY);
	}

	{
		nPosX = ptBkPos.x + PARTY_NAME_EDIT_X;
		nPosY = ptBkPos.y + PARTY_NAME_EDIT_Y;		
		m_pEditPartyName->SetPos(nPosX, nPosY);	
	}

	
	UpdateRadioBtn();
}

void CINFCommuPartyCre::OnClickJoint(int nId)
{
	m_nJointIdx = nId;
	switch(nId)
	{
	case RADIO_JOINT_COMMAND:	// 편대장 초대
		{
			char chBlank[SIZE_MAX_TEAM_PW+1];
			memset(chBlank, 0x00, SIZE_MAX_TEAM_PW+1);
			
			m_pINFSecuEditBox->SetString(chBlank, SIZE_MAX_TEAM_PW);	// 비밀번호
			m_pINFSecuEditBox->BackupTxtString();
		}
		break;
	case RADIO_JOINT_FREE:		// 자유 참여
		{
		}
		break;
	}	
	UpdateRadioBtn();
}
void CINFCommuPartyCre::OnClickAudioOp(int nId)
{
	//m_nAduioIdx = nId;
	switch(nId)
	{
	case RADIO_AUDIO_CHAT:	// 음성 채팅방식
		{
		}
		break;
	case RADIO_AUDIO_KEY:	// 키 입력방식
		{
		}
		break;
	case RADIO_AUDIO_FREE:	// 자유방식
		{
		}
		break;
	}	
	UpdateRadioBtn();
}

void CINFCommuPartyCre::UpdateRadioBtn()
{
	// 초대방식
	switch(m_nJointIdx)
	{
	case RADIO_JOINT_COMMAND:	// 편대장 초대
		{
			m_pPartyJoint[RADIO_JOINT_COMMAND]->SetRadioBtn(TRUE);
			m_pPartyJoint[RADIO_JOINT_FREE]->SetRadioBtn(FALSE);
		}
		break;
	case RADIO_JOINT_FREE:		// 자유 참여
		{
			m_pPartyJoint[RADIO_JOINT_COMMAND]->SetRadioBtn(FALSE);
			m_pPartyJoint[RADIO_JOINT_FREE]->SetRadioBtn(TRUE);
		}
		break;
	}	 
	
	//m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(TRUE);
	// 음성 버튼
	switch(m_nAduioIdx)
	{
	case RADIO_AUDIO_CHAT:	// 음성 채팅방식
		{			
			m_pAudioOp[RADIO_AUDIO_KEY]->SetRadioBtn(FALSE);
			m_pAudioOp[RADIO_AUDIO_FREE]->SetRadioBtn(FALSE);
		}
		break;
	case RADIO_AUDIO_KEY:	// 키 입력방식
		{			
			m_pAudioOp[RADIO_AUDIO_KEY]->SetRadioBtn(TRUE);
			m_pAudioOp[RADIO_AUDIO_FREE]->SetRadioBtn(FALSE);
		}
		break;
	case RADIO_AUDIO_FREE:	// 자유방식
		{			
			m_pAudioOp[RADIO_AUDIO_KEY]->SetRadioBtn(FALSE);
			m_pAudioOp[RADIO_AUDIO_FREE]->SetRadioBtn(TRUE);
		}
		break;
	}	
}

void CINFCommuPartyCre::AllEditBoxDisable()
{
	if(m_pEditPartyName->IsEditMode())
	{
		m_pEditPartyName->BackupTxtString();
	}
		
	m_pINFSecuEditBox->EnableEdit(FALSE);
	m_pNumMinEditBox->EnableEdit(FALSE);
	m_pNumMaxEditBox->EnableEdit(FALSE);
	m_pEditPartyName->EnableEdit(FALSE, FALSE);
	
}

void CINFCommuPartyCre::LoadSPartyInfo(SPARTY_INFO *pSPartyInfo)
{
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	pSPartyInfo->PartyInfluence = myShuttleInfo.InfluenceType;		// 세력
	m_pEditPartyName->GetString(pSPartyInfo->PartyName, SIZE_MAX_PARTY_NAME);	// 파티명
	/////////// 파티 참여 방식 /////////// 
	switch(m_nJointIdx)
	{
	case RADIO_JOINT_COMMAND:	// 편대장 초대
		{
			pSPartyInfo->PartyJoinType = PARTY_JOIN_TYPE_INVITE_MASTER;
		}
		break;
	case RADIO_JOINT_FREE:		// 자유 참여
		{
			pSPartyInfo->PartyJoinType = PARTY_JOIN_TYPE_FREE;
		}
		break;
	}
	
	/////////// 경험치 습득 방식 /////////// 
	switch(m_pComboDivision[COMBO_DIVISION_EXP]->GetSelect())
	{
	case 0:		// 개인습득
		{
			pSPartyInfo->ExpDistributeType = EXP_DISTRIBUTE_TYPE_DAMAGE;
		}
		break;
	case 1:		// 공동습득
		{
			pSPartyInfo->ExpDistributeType = EXP_DISTRIBUTE_TYPE_EQUALITY;
		}
		break;
	}

	/////////// 경험치 습득 방식 /////////// 
	switch(m_pComboDivision[COMBO_DIVISION_ITEM]->GetSelect())
	{
	case 0:		// 개인습득
		{
			pSPartyInfo->ItemDistributeType = ITEM_DISTRIBUTE_TYPE_DAMAGE;
		}
		break;
	case 1:		// 순차 습득
		{
			pSPartyInfo->ItemDistributeType = ITEM_DISTRIBUTE_TYPE_ORDER;
		}
		break;
	case 2:		// 랜덤 습득
		{
			pSPartyInfo->ItemDistributeType = ITEM_DISTRIBUTE_TYPE_RANDOM;
		}
		break;
	}

	///////// 파티 암호 ///////// 
	switch(m_nJointIdx)
	{
	case RADIO_JOINT_COMMAND:	// 편대장 초대
		{
			memset(pSPartyInfo->PartyPW, 0x00, SIZE_MAX_TEAM_PW);
		}
		break;
	case RADIO_JOINT_FREE:		// 자유 참여
		{
			m_pINFSecuEditBox->GetString(pSPartyInfo->PartyPW, SIZE_MAX_TEAM_PW);
		}
		break;
	}	

	if(strlen(pSPartyInfo->PartyPW) > 0)
	{
		pSPartyInfo->PartyLock = TRUE;
	}
	else
	{
		pSPartyInfo->PartyLock = FALSE;
	}
	
	//////////////// 파티 참여 레벨//////////////// 
	char chBuffTmp[32];
	memset(chBuffTmp, 0x00, 32);
	m_pNumMinEditBox->GetString(chBuffTmp, 32);
	pSPartyInfo->MinLevel = atoi(chBuffTmp);

	m_pNumMaxEditBox->GetString(chBuffTmp, 32);
	pSPartyInfo->MaxLevel = atoi(chBuffTmp);
		
	// 파티 포맷
	pSPartyInfo->FormationType = m_pComboForm->GetSelect();

}

SPARTY_INFO* CINFCommuPartyCre::GetSPartyCreateInfo()
{
	return &m_stSPartyCreate;
}
// 2008-12-09 by dgwoo 미션마스터.
void CINFCommuPartyCre::MissionMasterParty(char * i_pString)
{
	// 처음 쇼할떄 디폴트 값	
	memset(&m_stSPartyCreate, 0x00, sizeof(SPARTY_INFO));
	m_stSPartyCreate.FormationType= FLIGHT_FORM_NONE;	// 기본은 자유비행
	m_pComboForm->SetSelectItem(m_stSPartyCreate.FormationType);		
	memcpy(&m_stSPartyCreate.PartyName,i_pString,SIZE_MAX_PARTY_NAME);


	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	char chBuff[32];

	{
		int nMinLv = myShuttleInfo.Level - MINMAX_LV_CAP;
		if(nMinLv < 1)
		{
			nMinLv = 1;
		}
		wsprintf(chBuff, "%d", nMinLv );
		m_stSPartyCreate.MinLevel = nMinLv;
		m_pNumMinEditBox->SetString(chBuff, 32);

	}

	{
		int nMaxLv = CHARACTER_MAX_LEVEL;

		wsprintf(chBuff, "%d", nMaxLv);
		m_stSPartyCreate.MaxLevel = nMaxLv;
		m_pNumMaxEditBox->SetString(chBuff, 32);
	}
	
	{

		// 2009-04-23 by bhsohn 에디트창 커서 이동
		//m_pEditPartyName->SetString(i_pString, SIZE_MAX_PARTY_NAME);		
		m_pEditPartyName->SetString(i_pString, SIZE_MAX_PARTY_NAME, 0, TRUE);		
		// end 2009-04-23 by bhsohn 에디트창 커서 이동

		m_pEditPartyName->BackupTxtString();
	}
//	if(g_pGameMain->GetVoiceType() == VOICE_PARTY)
//	{
//		m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(TRUE);
//	}
//	else
	{
		m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(FALSE);
	}
	m_nJointIdx = RADIO_JOINT_FREE;
	m_stSPartyCreate.PartyJoinType = PARTY_JOIN_TYPE_FREE;

	m_stSPartyCreate.PartyLock = FALSE;
	m_stSPartyCreate.PartyInfluence = myShuttleInfo.InfluenceType;
	m_bMissionMaster = TRUE;
	
}
void CINFCommuPartyCre::InitCotrol()
{
	// 처음 쇼할떄 디폴트 값	
	memset(&m_stSPartyCreate, 0x00, sizeof(SPARTY_INFO));
	m_stSPartyCreate.FormationType= FLIGHT_FORM_NONE;	// 기본은 자유비행
	m_pComboForm->SetSelectItem(m_stSPartyCreate.FormationType);		

	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	char chBuff[32];

	{
		int nMinLv = myShuttleInfo.Level - MINMAX_LV_CAP;
		if(nMinLv < 1)
		{
			nMinLv = 1;
		}
		wsprintf(chBuff, "%d", nMinLv );
		m_pNumMinEditBox->SetString(chBuff, 32);

	}

	{
		int nMaxLv = myShuttleInfo.Level + MINMAX_LV_CAP;
		if(nMaxLv > CHARACTER_MAX_LEVEL)
		{
			nMaxLv = CHARACTER_MAX_LEVEL;
		}
		wsprintf(chBuff, "%d", nMaxLv);
		m_pNumMaxEditBox->SetString(chBuff, 32);
	}
	
	{
		char chPartyName[3][256] =
		{
			STRMSG_C_080610_0200,
			STRMSG_C_080610_0201,
			STRMSG_C_080610_0202
		};
		int nRandIdx = RANDI(0, 2);
		// 2009-04-23 by bhsohn 에디트창 커서 이동
		//m_pEditPartyName->SetString(chPartyName[nRandIdx], SIZE_MAX_PARTY_NAME);		
		m_pEditPartyName->SetString(chPartyName[nRandIdx], SIZE_MAX_PARTY_NAME, 0 ,TRUE);		
		// end 2009-04-23 by bhsohn 에디트창 커서 이동
		m_pEditPartyName->BackupTxtString();
	}

	if (false) // g_pGameMain->GetVoiceType() == VOICE_PARTY)
	{
		m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(TRUE);
	}
	else
	{
		m_pAudioOp[RADIO_AUDIO_CHAT]->SetRadioBtn(FALSE);
	}
}
//void CINFCommuPartyCre::SetVoiceChatVolum(DWORD i_nVolum)
//{// 볼륨 조절하는 함수를 계속 호출하면 속도가 많이 느려짐  제어할수있도록 구현.
//	if(m_fVolumConTimeGap > VOICECHAT_SPK_CONTROL_TIME_GAP)
//	{
//		m_fVolumConTimeGap = 0;
//		// 2008-10-20 by bhsohn VOIP 버그 수정
//		// 볼륨조절 막음
//		//g_pGameMain->SetVoiceVolum(i_nVolum);
//	}
//	m_fVolumConTimeGap += g_pD3dApp->GetElapsedTime();
//	m_nVolum = i_nVolum;
//}
// 2008-12-09 by dgwoo 미션마스터.
void CINFCommuPartyCre::SetMissionMaster(BOOL i_bMissionMaster)
{
	m_bMissionMaster = i_bMissionMaster;
}								  
// 2008-12-09 by dgwoo 미션마스터.
void CINFCommuPartyCre::SetMissionMasterUID(UID64_t i_nUID)
{
	m_nMissionMasterUID = i_nUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-23 by bhsohn 에디트창 커서 이동
/// \date		2009-04-23 ~ 2009-04-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCommuPartyCre::WndProcEdit(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nLienCnt = -1;
	int nArrowScroll = 0;
	BOOL bRtn = FALSE;	
	bRtn = m_pEditPartyName->WndProc(uMsg, wParam, lParam);
	
	if(bRtn)
	{
		return INF_MSGPROC_BREAK;
	}		
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CINFCommuPartyCre::SetAutoCreateParty(SPARTY_INFO i_PartyInfo)
/// \brief		/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 자동으로 파티만들기.
/// \author		jhseol
/// \date		2012-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommuPartyCre::SetAutoCreateParty(SPARTY_INFO i_PartyInfo)
{
	m_stSPartyCreate = i_PartyInfo;
}