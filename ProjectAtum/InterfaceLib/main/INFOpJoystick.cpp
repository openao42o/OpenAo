// INFOpJoystick.cpp: implementation of the CINFOpJoystick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "INFImageRadioBtn.h"
#include "INFListBox.h"
#include "JoyStickInput.h"
#include "INFImageBtn.h"
#include "AtumSound.h"
#include "INFArenaScrollBar.h"
#include "Interface.h"
#include "INFImageComboBoxEx.h"
#include "INFWindow.h"
#include "INFGameMain.h"
#include "SetupConfig.h"

#include "INFOpJoystick.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFToolTip.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

// 캡스 영역
#define OPJOYSTIC_CAPS_HEIGHT		20

// 라디오 버튼

// 조이스틱 사용 여부
#define OPTION_RADIO_JOYSTIC_ONOFF_X		232
#define OPTION_RADIO_JOYSTIC_ONOFF_Y		43
#define OPTION_RADIO_JOYSTIC_ONOFF_CAP_X	76
#define OPTION_RADIO_JOYSTIC_ONOFF_CAP_Y	27

// 진동
#define OPTION_RADIO_JOYSTIC_FEED_X			232
#define OPTION_RADIO_JOYSTIC_FEED_Y			83

// 감도 조절
#define	JOY_COMBO_SENSE_X				234
#define	JOY_COMBO_SENSE_Y				105
#define JOY_COMBO_SENSE_MAIN_WIDTH			110
#define JOY_COMBO_SENSE_MAIN_HEIGHT			20
#define JOY_COMBO_SENSE_ELE_WIDTH			110
#define JOY_COMBO_SENSE_ELE_HEIGHT			15

// 키 설정 가지고 오기
#define	JOY_COMBO_KEY_X				234
#define	JOY_COMBO_KEY_Y				126
#define JOY_COMBO_KEY_MAIN_WIDTH			110
#define JOY_COMBO_KEY_MAIN_HEIGHT			20
#define JOY_COMBO_KEY_ELE_WIDTH			110
#define JOY_COMBO_KEY_ELE_HEIGHT			15

// 조이스틱선택
#define	JOY_COMBO_DEVICE_X				234
#define	JOY_COMBO_DEVICE_Y				66
#define JOY_COMBO_DEVICE_MAIN_WIDTH			110
#define JOY_COMBO_DEVICE_MAIN_HEIGHT			20
#define JOY_COMBO_DEVICE_ELE_WIDTH			110
#define JOY_COMBO_DEVICE_ELE_HEIGHT			15


// 조이스틱 키
#define JOY_SETUP_KEY_CONFIG_X			38
#define JOY_SETUP_KEY_CONFIG_Y			190
#define JOY_SETUP_KEY_CONFIG_HEIGHT		26

//저장
#define	JOY_BTN_SAVE_X				702
#define	JOY_BTN_SAVE_Y				458
//닫기
#define	JOY_BTN_CLOSE_X				742
#define	JOY_BTN_CLOSE_Y				458
//새이름으로 저장
#define	JOY_BTN_SAVEAS_X				782
#define	JOY_BTN_SAVEAS_Y				458


// 키 설정 콤보 박스 
#define	JOY_COMBO_SETUP_X				111
#define	JOY_COMBO_SETUP_Y				191
#define	JOY_COMBO_SETUP_CAP_X			138
#define	JOY_COMBO_SETUP_CAP_Y			26
#define JOY_COMBO_SETUP_MAIN_WIDTH			70
#define JOY_COMBO_SETUP_MAIN_HEIGHT			17
#define JOY_COMBO_SETUP_ELE_WIDTH			70
#define JOY_COMBO_SETUP_ELE_HEIGHT			13

// 키 콤보 박스 크기
#define KEY_SETUP_COMBO_SIZE_Y				15
#define KEY_SETUP_COMBO_SIZE_X				132
#else
// 캡스 영역
#define OPJOYSTIC_CAPS_HEIGHT		20

// 라디오 버튼

// 조이스틱 사용 여부
#define OPTION_RADIO_JOYSTIC_ONOFF_X		245
#define OPTION_RADIO_JOYSTIC_ONOFF_Y		32
#define OPTION_RADIO_JOYSTIC_ONOFF_CAP_X	70
#define OPTION_RADIO_JOYSTIC_ONOFF_CAP_Y	16

// 진동
#define OPTION_RADIO_JOYSTIC_FEED_X			245
#define OPTION_RADIO_JOYSTIC_FEED_Y			72

// 감도 조절
#define	JOY_COMBO_SENSE_X				244
#define	JOY_COMBO_SENSE_Y				89
#define JOY_COMBO_SENSE_MAIN_WIDTH			110
#define JOY_COMBO_SENSE_MAIN_HEIGHT			17
#define JOY_COMBO_SENSE_ELE_WIDTH			110
#define JOY_COMBO_SENSE_ELE_HEIGHT			13

// 키 설정 가지고 오기
#define	JOY_COMBO_KEY_X				244
#define	JOY_COMBO_KEY_Y				109
#define JOY_COMBO_KEY_MAIN_WIDTH			110
#define JOY_COMBO_KEY_MAIN_HEIGHT			17
#define JOY_COMBO_KEY_ELE_WIDTH			110
#define JOY_COMBO_KEY_ELE_HEIGHT			13

// 조이스틱선택
#define	JOY_COMBO_DEVICE_X				244
#define	JOY_COMBO_DEVICE_Y				49
#define JOY_COMBO_DEVICE_MAIN_WIDTH			110
#define JOY_COMBO_DEVICE_MAIN_HEIGHT			17
#define JOY_COMBO_DEVICE_ELE_WIDTH			110
#define JOY_COMBO_DEVICE_ELE_HEIGHT			13


// 조이스틱 키
#define JOY_SETUP_KEY_CONFIG_X			51
#define JOY_SETUP_KEY_CONFIG_Y			173
#define JOY_SETUP_KEY_CONFIG_HEIGHT		26

//저장
#define	JOY_BTN_SAVE_X				322
#define	JOY_BTN_SAVE_Y				437
//닫기
#define	JOY_BTN_CLOSE_X				388
#define	JOY_BTN_CLOSE_Y				437
//새이름으로 저장
#define	JOY_BTN_SAVEAS_X				454
#define	JOY_BTN_SAVEAS_Y				437


// 키 설정 콤보 박스 
#define	JOY_COMBO_SETUP_X				122
#define	JOY_COMBO_SETUP_Y				173
#define	JOY_COMBO_SETUP_CAP_X			138
#define	JOY_COMBO_SETUP_CAP_Y			26
#define JOY_COMBO_SETUP_MAIN_WIDTH			70
#define JOY_COMBO_SETUP_MAIN_HEIGHT			17
#define JOY_COMBO_SETUP_ELE_WIDTH			70
#define JOY_COMBO_SETUP_ELE_HEIGHT			13

// 키 콤보 박스 크기
#define KEY_SETUP_COMBO_SIZE_Y				15
#define KEY_SETUP_COMBO_SIZE_X				132
#endif

CINFOpJoystick::CINFOpJoystick(CAtumNode* pParent)
{	
	m_pBoxImage = NULL;
	m_pSelImage = NULL;

	m_pFontTxt = NULL;
	
	m_bShowWnd = FALSE;

	m_ptBkPos.x = m_ptBkPos.y = 0;

	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_nWidth = m_nHeight = 0;
	m_nMaxLine = 0;

	int nCnt = 0;
	int nMainId = 0;	

	for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{
			m_pJoyOpRadioBtn[nMainId][nCnt] = NULL;
		}
	}
	for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
	{
		for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
		{
			m_pComboJoySetup[nMainId][nCnt] = NULL;
		}
	}
	int i = 0;
	m_pJoystickSetupKeyTable = new structJoystickSetupKeyTable[MAX_JOSTICK_KEY_OPTION];
	for(i=0; i< MAX_JOSTICK_KEY_OPTION;i++)
	{					
		memset(&m_pJoystickSetupKeyTable[i], 0x00, sizeof(structJoystickSetupKeyTable));
		m_pJoystickSetupKeyTable[i].nKeySetup = -1;
		for(nCnt=0; nCnt< MAX_JOSTICK_OPTION_SLOT;nCnt++)
		{
			m_pJoystickSetupKeyTable[i].nBehavior[nCnt] = -1;
		}
	}
	

	memset(&m_struJoyStickOptionInfo, 0x00, sizeof(structJoyStickOptionInfo));

	m_pComboJoySense = NULL;		// 콤보 박스
	m_pComboJoyFile = NULL;
	m_pComboJoyDevice = NULL;		// 장치 

	m_pOptionJoySave = NULL;		// 저장
	m_pOptionClose = NULL;			// 닫기
	m_pOptionJoySaveAs = NULL;		// 새이름으로 저장

	m_pScroll = NULL;
	
	m_nShowItemComboX = m_nShowItemComboY = -1;

	m_pJoySticTmpConfig = new CSetupConfig;	

}

CINFOpJoystick::~CINFOpJoystick()
{		
	util::del(m_pScroll);

	util::del(m_pBoxImage);
	util::del(m_pSelImage);

	util::del(m_pFontTxt);

	int nCnt = 0;
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{		
			util::del(m_pJoyOpRadioBtn[nMainId][nCnt]);		
		}
	}
	for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
	{
		for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
		{
			util::del(m_pComboJoySetup[nMainId][nCnt]);
		}
	}

	util::del(m_pComboJoySense);	
	util::del(m_pComboJoyFile);
	util::del(m_pComboJoyDevice);		// 장치 

	util::del(m_pOptionJoySave);		// 저장
	util::del(m_pOptionClose);			// 닫기
	util::del(m_pOptionJoySaveAs);		// 새이름으로 저장

	util::del_array(m_pJoystickSetupKeyTable);

	util::del(m_pJoySticTmpConfig);
}

HRESULT CINFOpJoystick::InitDeviceObjects()
{	
	char buf[64];
	
	//CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("joystick");	
		m_pBoxImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBoxImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
#else
	{
		wsprintf(buf, "jobk");
		m_pBoxImage = new CINFImageEx;
		DataHeader* pDataHeader = FindResource(buf);
		m_pBoxImage->InitDeviceObjects(pDataHeader);
	}
#endif	
	{
		wsprintf(buf, "joblk");
		m_pSelImage = new CINFImageEx;
		DataHeader* pDataHeader = FindResource(buf);
		m_pSelImage->InitDeviceObjects(pDataHeader);
	}

	if(NULL == m_pFontTxt)
	{
		m_pFontTxt = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
		m_pFontTxt->InitDeviceObjects(g_pD3dDev);
	}

	{
		char chRadioOff[30], chRadioOn[30];
		
		wsprintf(chRadioOff, "radio_b");
		wsprintf(chRadioOn, "radio_a");
		
		int nMainId = 0;	
		int nCnt = 0;
		for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
			{		
				if(NULL == m_pJoyOpRadioBtn[nMainId][nCnt])
				{		
					m_pJoyOpRadioBtn[nMainId][nCnt] = new CINFImageRadioBtn;
				}				
				m_pJoyOpRadioBtn[nMainId][nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
			}		
		}		
	}
	{
		int nMainId = 0;	
		int nCnt = 0;
		
		for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
		{
			for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
			{
				if(NULL == m_pComboJoySetup[nMainId][nCnt])
				{
					POINT ptPos  = {0,0};
					int nPos = -1;
					m_pComboJoySetup[nMainId][nCnt] = new CINFImageComboBoxEx();
					m_pComboJoySetup[nMainId][nCnt]->SetGameData(m_pGameData);
					m_pComboJoySetup[nMainId][nCnt]->InitDeviceObjects(ptPos, 
						KEY_SETUP_COMBO_SIZE_X, KEY_SETUP_COMBO_SIZE_Y,
						"jcar", "jcbk", "jcsel", "c_scrlb");	

					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, " ", NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0200, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0201, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0202, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0203, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0204, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0205, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0206, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0207, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0208, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0209, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0210, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0211, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0212, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0213, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0214, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0215, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0216, NULL, TRUE);
					m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_081127_0217, NULL, TRUE);

					//if(pJoyStick)
					{
						char chBuff[MAX_PATH];
						//int nJoyBtnLen = pJoyStick->GetKeyJoystickLen() - MAX_DEFAULT_JOY_SETUP;
						int nCntTmp = 0;
						for(nCntTmp = 0;nCntTmp < QSLOT_NUMBER;nCntTmp++)
						{
							wsprintf(chBuff, STRMSG_C_081127_0218, nCntTmp+1);
							m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, chBuff, NULL, TRUE);
						}								
					}					
					// 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
					{
						m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_090116_0200, NULL, TRUE);
						m_pComboJoySetup[nMainId][nCnt]->AddItem(nPos++, STRMSG_C_090116_0201, NULL, TRUE);
					}
					// end 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
				}
			}
		}
	}
	{
		if(NULL == m_pComboJoySense)
		{			
			m_pComboJoySense = new CINFListBox("cbarena","cbarenab");
			m_pComboJoySense->InitDeviceObjects();				
		}			
	}

	{
		if(NULL == m_pComboJoyFile)
		{			
			m_pComboJoyFile = new CINFListBox("cbarena","cbarenab");
			m_pComboJoyFile->InitDeviceObjects();				
		}			
	}
	// 장치 
	{
		if(NULL == m_pComboJoyDevice)
		{			
			m_pComboJoyDevice = new CINFListBox("cbarena","cbarenab");
			m_pComboJoyDevice->SetUseCulling(TRUE); //글씨 컬링 사용
			m_pComboJoyDevice->InitDeviceObjects();				
		}			
	}
	// 저장
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
#else
		wsprintf(szUpBtn, "josbtn3");
		wsprintf(szDownBtn, "josbtn1");
		wsprintf(szSelBtn, "josbtn0");
		wsprintf(szDisBtn, "josbtn2");
#endif

		if(NULL == m_pOptionJoySave)
		{
			m_pOptionJoySave = new CINFImageBtn;
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM		
		m_pOptionJoySave->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP11");
#else
		m_pOptionJoySave->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
#endif
	}
	// 닫기
	{	
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
		if(NULL == m_pOptionClose)
		{
			m_pOptionClose = new CINFImageBtn;
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pOptionClose->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP41");
#else
		m_pOptionClose->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
#endif
	}
	// 새이름으로 저장
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "jonsbtn3");
		wsprintf(szDownBtn, "jonsbtn1");
		wsprintf(szSelBtn, "jonsbtn0");
		wsprintf(szDisBtn, "jonsbtn2");
		if(NULL == m_pOptionJoySaveAs)
		{
			m_pOptionJoySaveAs = new CINFImageBtn;
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM		
		m_pOptionJoySaveAs->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP10");	
#else
		m_pOptionJoySaveAs->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
#endif

	}

	{
		char  szScBall[30];	
		if(NULL == m_pScroll)
		{
			m_pScroll = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pScroll->InitDeviceObjects(ONEPAGE_JOY_SETUP_Y, szScBall);
	}
	
	InitJoysticSense();

	FindJoyFile();	// .Joy 파일 찾기
	return S_OK;
}
HRESULT CINFOpJoystick::RestoreDeviceObjects()
{	
	if(m_pBoxImage)
	{
		m_pBoxImage->RestoreDeviceObjects();	
		POINT ptSize = m_pBoxImage->GetImgSize();
		m_nWidth = ptSize.x;
		m_nHeight = ptSize.y;
	}
	if(m_pSelImage)
	{
		m_pSelImage->RestoreDeviceObjects();	
	}

	if(m_pFontTxt)
	{
		m_pFontTxt->RestoreDeviceObjects();
	}

	{		
		int nMainId = 0;	
		int nCnt = 0;
		for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
		{
			for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
			{	
				if(m_pJoyOpRadioBtn[nMainId][nCnt])
				{
					m_pJoyOpRadioBtn[nMainId][nCnt]->RestoreDeviceObjects();			
				}
			}
		}		
	}
	{
		int nMainId = 0;	
		int nCnt = 0;
		for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
		{
			for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
			{
				if(m_pComboJoySetup[nMainId][nCnt])
				{
					m_pComboJoySetup[nMainId][nCnt]->RestoreDeviceObjects();
				}
			}
		}
	}
	if(m_pComboJoySense)
	{
		m_pComboJoySense->RestoreDeviceObjects();						
	}
	if(m_pComboJoyFile)
	{
		m_pComboJoyFile->RestoreDeviceObjects();						
	}
	
	if(m_pComboJoyDevice)
	{
		m_pComboJoyDevice->RestoreDeviceObjects();						
	}

	// 저장
	{			
		if(m_pOptionJoySave)
		{
			m_pOptionJoySave->RestoreDeviceObjects();
		}
		
	}
	// 닫기
	{	
		if(m_pOptionClose)
		{
			m_pOptionClose->RestoreDeviceObjects();
		}		
	}
	// 새이름으로 저장
	{	
		if(m_pOptionJoySaveAs)
		{
			m_pOptionJoySaveAs->RestoreDeviceObjects();
		}		
	}
	// 스크롤
	if(m_pScroll)
	{
		m_pScroll->RestoreDeviceObjects();
	}

	UpdateBtnPos(m_ptBkPos.x, m_ptBkPos.y);
	
	return S_OK;
}
HRESULT CINFOpJoystick::DeleteDeviceObjects()
{	
	if(m_pBoxImage)
	{
		m_pBoxImage->DeleteDeviceObjects();
		util::del(m_pBoxImage) ;
	}
	if(m_pSelImage)
	{
		m_pSelImage->DeleteDeviceObjects();
		util::del(m_pSelImage) ;
	}

	if(m_pFontTxt ) 
	{
		m_pFontTxt->DeleteDeviceObjects() ;
		util::del(m_pFontTxt ) ; 
	}

	int nCnt = 0;
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{		
			if(m_pJoyOpRadioBtn[nMainId][nCnt])
			{
				m_pJoyOpRadioBtn[nMainId][nCnt]->DeleteDeviceObjects();	
				util::del(m_pJoyOpRadioBtn[nMainId][nCnt]);
			}
		}
	}
	{		
		for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
		{
			for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
			{
				if(m_pComboJoySetup[nMainId][nCnt])
				{
					m_pComboJoySetup[nMainId][nCnt]->DeleteDeviceObjects();	
					util::del(m_pComboJoySetup[nMainId][nCnt]);
				}
			}
		}
	}
	if(m_pComboJoySense)
	{		
		m_pComboJoySense->DeleteDeviceObjects();
		util::del(m_pComboJoySense);
	}
	if(m_pComboJoyFile)
	{		
		m_pComboJoyFile->DeleteDeviceObjects();
		util::del(m_pComboJoyFile);
	}
	if(m_pComboJoyDevice)
	{
		m_pComboJoyDevice->DeleteDeviceObjects();
		util::del(m_pComboJoyDevice);
	}
	// 저장
	{			
		if(m_pOptionJoySave)
		{
			m_pOptionJoySave->DeleteDeviceObjects();
			util::del(m_pOptionJoySave);
		}
		
	}
	// 닫기
	{	
		if(m_pOptionClose)
		{			
			m_pOptionClose->DeleteDeviceObjects();
			util::del(m_pOptionClose);
		}		
	}
	// 새이름으로 저장
	{	
		if(m_pOptionJoySaveAs)
		{
			m_pOptionJoySaveAs->DeleteDeviceObjects();
			util::del(m_pOptionJoySaveAs);			
		}		
	}
	if(m_pScroll)
	{
		m_pScroll->DeleteDeviceObjects();
		util::del(m_pScroll);
	}

	return S_OK;
}
HRESULT CINFOpJoystick::InvalidateDeviceObjects()
{	
	if(m_pBoxImage)
	{
		m_pBoxImage->InvalidateDeviceObjects();
	}
	if(m_pSelImage)
	{
		m_pSelImage->InvalidateDeviceObjects();
	}

	if(m_pFontTxt )
	{
		m_pFontTxt->InvalidateDeviceObjects() ;
	}

	int nCnt = 0;
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{		
			if(m_pJoyOpRadioBtn[nMainId][nCnt])
			{
				m_pJoyOpRadioBtn[nMainId][nCnt]->InvalidateDeviceObjects();
			}
		}
	}
	{		
		for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
		{
			for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
			{
				if(m_pComboJoySetup[nMainId][nCnt])
				{
					m_pComboJoySetup[nMainId][nCnt]->InvalidateDeviceObjects();					
				}
			}
		}
	}
	if(m_pComboJoySense)
	{
		m_pComboJoySense->InvalidateDeviceObjects();
	}
	if(m_pComboJoyFile)
	{
		m_pComboJoyFile->InvalidateDeviceObjects();
	}
	if(m_pComboJoyDevice)
	{
		m_pComboJoyDevice->InvalidateDeviceObjects();		
	}
	// 저장
	{			
		if(m_pOptionJoySave)
		{
			m_pOptionJoySave->InvalidateDeviceObjects();
		}
		
	}
	// 닫기
	{	
		if(m_pOptionClose)
		{			
			m_pOptionClose->InvalidateDeviceObjects();
		}		
	}
	// 새이름으로 저장
	{	
		if(m_pOptionJoySaveAs)
		{
			m_pOptionJoySaveAs->InvalidateDeviceObjects();
		}		
	}

	if(m_pScroll)
	{
		m_pScroll->InvalidateDeviceObjects();
	}

	return S_OK;
}
void CINFOpJoystick::Render()
{
	if(!IsShowWnd())
	{
		return;
	}
	RenderCenterWindow(m_ptBkPos.x, m_ptBkPos.y, m_nWidth, m_nHeight);

	int nCheckJoy = CheckJoySticBtn();
	if(nCheckJoy != -1)
	{
		int nOldStep = m_pScroll->GetScrollStep();
		int nScrollMax = m_pScroll->GetMaxStepCnt() - ONEPAGE_JOY_SETUP_Y;
		if(nCheckJoy > nScrollMax)
		{
			nCheckJoy = nScrollMax;
		}
		
		// 스크롤 위치 갱신
		m_pScroll->SetScrollStep(nCheckJoy);
		if(nOldStep != nCheckJoy)
		{
			// 콤보 박스 정보 갱신
			UpdateKeyComboControls();
		}
	}

	int nCnt =0;
	int nMainId = 0;	
	for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
	{
		for(nCnt =0; nCnt < MAX_OPTION_SYS_CNT; nCnt++)
		{		
			m_pJoyOpRadioBtn[nMainId][nCnt]->Render();
		}	
	}
		
	m_pComboJoyFile->Render();
	m_pComboJoySense->Render();
	m_pComboJoyDevice->Render();		

	// 저장
	{			
		if(m_pOptionJoySave)
		{
			m_pOptionJoySave->Render();
		}
		
	}
	// 닫기
	{	
		if(m_pOptionClose)
		{			
			m_pOptionClose->Render();
		}		
	}
	// 새이름으로 저장
	{	
		if(m_pOptionJoySaveAs)
		{
			m_pOptionJoySaveAs->Render();
		}		
	}

	m_pScroll->Render();	// 스크롤

	RenderKeyInfomation();
	RenderClickKey();	// 클릭한 컨트롤 랜더링

	{		
		// 콤보 박스
		for(nMainId = ONEPAGE_JOY_SETUP_Y-1;nMainId >= 0 ;nMainId--)
		{
			for(nCnt = ONEPAGE_JOY_SETUP_X-1;nCnt >= 0;nCnt--)
			{
				if(m_pComboJoySetup[nMainId][nCnt])
				{
					m_pComboJoySetup[nMainId][nCnt]->Render();
				}
			}
		}
	}

}

void CINFOpJoystick::RenderCenterWindow(int x, int y, int cx, int cy)
{
	FLOG( "CINFWindow::RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose)" );	
	if(m_pBoxImage)
	{
		m_pBoxImage->Move(x, y);
		m_pBoxImage->Render();
	}
}

int CINFOpJoystick::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(uMsg, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(uMsg, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(uMsg, wParam, lParam);
		}
		break;	
	case WM_KEYDOWN:
		{
			return OnKeyDown(uMsg, wParam, lParam);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			{
				int nPosX, nPosY;
				nPosX = nPosY = 0;
				for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
				{		
					for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
					{
						if(m_pComboJoySetup[nPosY][nPosX]->OnMouseWheel(pt, wParam, lParam))
						{
							return INF_MSGPROC_BREAK;
						}
					}
				}
			}
			{
				BOOL bClick = m_pScroll->IsMouseWhellPos(pt);
				if(bClick)		
				{			
					int nOldStep = m_pScroll->GetScrollStep();
					m_pScroll->OnMouseWheel(wParam, lParam);	
					if(nOldStep != m_pScroll->GetScrollStep())
					{
						// 스텝이 다르다.
						UpdateKeyComboControls();
					}
					return INF_MSGPROC_BREAK;
				}
			}
		}
		break;	
	}
	return INF_MSGPROC_NORMAL;
}

int CINFOpJoystick::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		int nPosX, nPosY;
		nPosX = nPosY = 0;
		
		for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
		{			
			for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
			{
				BOOL bClick = m_pComboJoySetup[nPosY][nPosX]->OnLButtonUp(pt);
				if(bClick)
				{
					// 스텝이 다르다.
					UpdateKeyComboControls();
					return INF_MSGPROC_BREAK;
				}	
				
			}
		}
	}
	{
		BOOL bClick = m_pScroll->GetMouseMoveMode();
		if(bClick)
		{
			int nOldStep = m_pScroll->GetScrollStep();								
			m_pScroll->SetMouseMoveMode(FALSE);
			if(nOldStep != m_pScroll->GetScrollStep())
			{
				// 스텝이 다르다.
				UpdateKeyComboControls();
			}
			return INF_MSGPROC_BREAK;
		}				
	}
	

	{
		if(m_bMove)
		{
			m_bMove = FALSE;
			return INF_MSGPROC_BREAK;
		}		
	}	
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}


	// 저장	
	{
		if(TRUE == m_pOptionJoySave->OnLButtonUp(pt))
		{		
			OnClickSaveBtn();
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			return  INF_MSGPROC_BREAK;
		}		
	}
	// 닫기	
	{
		if(TRUE == m_pOptionClose->OnLButtonUp(pt))
		{			
			ShowWnd(FALSE, NULL);
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			return  INF_MSGPROC_BREAK;
		}		
	}
	// 새이름으로 저장
	{
		if(TRUE == m_pOptionJoySaveAs->OnLButtonUp(pt))
		{			
			OnClickSaveAsBtn();
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			return  INF_MSGPROC_BREAK;
		}		
	}

	

	return INF_MSGPROC_BREAK;
}

int CINFOpJoystick::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	

	if(!m_bMove)
	{		
		{		
			int nOldCursel = m_pComboJoyDevice->GetSelect() - 1;
			int nLBtnDown = m_pComboJoyDevice->LButtonDown(pt);
			if(1 == nLBtnDown)				
			{
				OnDeviceChange(nOldCursel); // 장치 변경				
				// 보이다가 안보이는 상황			
				return  INF_MSGPROC_BREAK;
			}
		}
		{		
			int nLBtnDown = m_pComboJoySense->LButtonDown(pt);
			if(1 == nLBtnDown)				
			{
				int nCursel = m_pComboJoySense->GetSelect();			
				if(nCursel < 0)
				{
					nCursel = 0;
				}
				else if(nCursel >= MAX_JOYSTICK_SENSITY)
				{
					nCursel = MAX_JOYSTICK_SENSITY-1;
				}
				m_struJoyStickOptionInfo.nJoysticSense = nCursel;
				// 보이다가 안보이는 상황			
				return  INF_MSGPROC_BREAK;
			}
		}
		
		{		
			int nLBtnDown = m_pComboJoyFile->LButtonDown(pt);
			if(1 == nLBtnDown)				
			{
				int nCursel = m_pComboJoyFile->GetSelect();		
				char* pFilePath = m_pComboJoyFile->GetItem(nCursel);
				if(strlen(pFilePath) > 0)
				{
					// 초기화
					m_pJoySticTmpConfig->ResetSetupInfo();	
					InitSetupKeyTable();
					if(m_pJoySticTmpConfig->LoadSetupInfo(pFilePath))
					{
						g_pInterface->LoadFileJoysticInfo(m_pJoySticTmpConfig, m_pJoystickSetupKeyTable);
						
						UpdateKeyInfomation();
						UpdateKeyComboControls();
					}
				}			
				// 보이다가 안보이는 상황			
				return  INF_MSGPROC_BREAK;
			}
		}
		if(OnLButtonDownUpdateComoboBox(pt))
		{
			return  INF_MSGPROC_BREAK;
		}
	}

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}

	{
		if(IsMouseCaps(pt))
		{
			m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
			m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
			m_bMove = TRUE;			
						
			return INF_MSGPROC_BREAK;
		}
	}	
	
	{	
		// 라디오 버튼
		int nMainId = 0;	
		int nCnt = 0;
		BOOL bBreak = FALSE;
		for(nMainId = 0;nMainId < MAX_JOY_OPTION_RADIO;nMainId++)
		{
			if(bBreak )
			{
				break;
			}
			for(nCnt = 0;nCnt <MAX_OPTION_SYS_CNT;nCnt++)
			{
				if(bBreak )
				{
					break;
				}
				if(TRUE == m_pJoyOpRadioBtn[nMainId][nCnt]->OnLButtonDown(pt))
				{
					BOOL bRadioCheck = TRUE;
					if(OPTION_SYS_OFF == nCnt)
					{
						bRadioCheck = FALSE;
					}
					SetRadioInfo(nMainId, bRadioCheck);
					bBreak = TRUE;
				}		
			}	
		}
	}
	
	 
	// 저장
	if(TRUE == m_pOptionJoySave->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	// 닫기
	if(TRUE == m_pOptionClose->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		
	// 새이름으로 저장
	if(TRUE == m_pOptionJoySaveAs->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}		

	{
		BOOL bClick = m_pScroll->IsMouseBallPos(pt);
		if(bClick)
		{
			int nOldStep = m_pScroll->GetScrollStep();								
			m_pScroll->SetMouseMoveMode(TRUE);
			if(nOldStep != m_pScroll->GetScrollStep())
			{
				// 스텝이 다르다.
				UpdateKeyComboControls();
			}
			
			return INF_MSGPROC_BREAK;
		}
	}
	
	return INF_MSGPROC_BREAK;
}

int CINFOpJoystick::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	if(!m_bMove)
	{
		BOOL bComboSel = FALSE;
		int nPosX, nPosY;
		nPosX = nPosY = 0;
		
		for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
		{		
			for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
			{
				bComboSel |= m_pComboJoySetup[nPosY][nPosX]->OnMouseMove(pt);
			}
		}

		if(bComboSel)
		{
			return INF_MSGPROC_BREAK;
		}
	}

//	if(!IsWndRect(pt) && !m_bMove)
//	{
//		return INF_MSGPROC_NORMAL;
//	}	

	{
		if(m_bMove)
		{
			m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
			m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
			// UI유저 지정 
			UpdateBtnPos(m_ptBkPos.x, m_ptBkPos.y);
			return INF_MSGPROC_BREAK;
		}
	}		

	m_pComboJoySense->MouseMove(pt);
	m_pComboJoyFile->MouseMove(pt);
	m_pComboJoyDevice->MouseMove(pt);

	// 저장
	m_pOptionJoySave->OnMouseMove(pt);	
	// 닫기
	m_pOptionClose->OnMouseMove(pt);		
	// 새이름으로 저장
	m_pOptionJoySaveAs->OnMouseMove(pt);	
	{
		if(m_pScroll->GetMouseMoveMode())
		{
			if(FALSE == m_pScroll->IsMouseScrollPos(pt))
			{
				int nOldStep = m_pScroll->GetScrollStep();								
				m_pScroll->SetMouseMoveMode(FALSE);
				if(nOldStep != m_pScroll->GetScrollStep())
				{
					// 스텝이 다르다.
					UpdateKeyComboControls();
				}			
			}
			else
			{
				int nOldStep = m_pScroll->GetScrollStep();								
				m_pScroll->SetScrollPos(pt);
				if(nOldStep != m_pScroll->GetScrollStep())
				{
					// 스텝이 다르다.
					UpdateKeyComboControls();
				}			
				
				return INF_MSGPROC_BREAK;
			}
		}
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFOpJoystick::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		{
			ShowWnd(FALSE, NULL);
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			return INF_MSGPROC_BREAK;
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

BOOL CINFOpJoystick::IsWndRect(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;		

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+m_nWidth))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+m_nHeight)))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CINFOpJoystick::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;		

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+m_nWidth))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+OPJOYSTIC_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CINFOpJoystick::IsShowWnd()
{
	return m_bShowWnd;
}

void CINFOpJoystick::InitString()
{
	m_nMaxLine = 0;
	// memset(m_szStringDesc, 0x00, MAX_STRING_LINE*MAX_PATH);
}

void CINFOpJoystick::SetString(char* i_vecDesc)
{
//	if(m_nMaxLine >= MAX_STRING_LINE)
//	{
//		return;
//	}
//	strncpy(m_szStringDesc[m_nMaxLine], i_vecDesc, MAX_PATH);
	
	m_nMaxLine++;	
}

void CINFOpJoystick::ShowWnd(BOOL bShowWnd, POINT *ptPos/*=NULL*/)
{
	m_bShowWnd = bShowWnd;
	if(bShowWnd && ptPos)
	{		
		m_ptBkPos = (*ptPos);

		memcpy(&m_struJoyStickOptionInfo, g_pJoysticOp, sizeof(structJoyStickOptionInfo));

		int nCnt = 0;
		for(nCnt =0; nCnt < MAX_JOSTICK_KEY_OPTION;nCnt++)
		{
			// 저장된 정보에서 가져온다.
			structJoystickSetupKeyTable	 *pJoystickSetupKeyTable = g_pInterface->GetJoystickSetupKeyTable(nCnt);
			if(NULL == pJoystickSetupKeyTable)
			{
				continue;
			}
			memcpy(&m_pJoystickSetupKeyTable[nCnt], pJoystickSetupKeyTable, sizeof(structJoystickSetupKeyTable));
		}		

		UpdateOptionInterface(&m_struJoyStickOptionInfo);	// 옵션 라디오 버튼 초기화

		UpdateBtnPos(m_ptBkPos.x, m_ptBkPos.y);

		UpdateKeyInfomation();
		UpdateKeyComboControls();
	}
}

POINT CINFOpJoystick::GetWndPos()
{
	return m_ptBkPos;
}

void CINFOpJoystick::UpdateString(int nIdx, char* pStr)
{
//	if(nIdx <0 || nIdx >=MAX_STRING_LINE)
//	{
//		return;
//	}
//	strncpy(m_szStringDesc[nIdx], pStr, MAX_PATH-1);

}

void CINFOpJoystick::UpdateBtnPos(int nBackPosX, int nBackPosY)
{
	int nCnt = 0;
 	int nPosX, nPosY;

	POINT ptBkPos = m_ptBkPos;
	
	int nMainId = 0;	
	
	{
		// 조이스틱 사용 여부
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{
			nPosX = nBackPosX + OPTION_RADIO_JOYSTIC_ONOFF_X + (nCnt*OPTION_RADIO_JOYSTIC_ONOFF_CAP_X);
			nPosY = nBackPosY + OPTION_RADIO_JOYSTIC_ONOFF_Y;		
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_ONOFF][nCnt]->SetPosition(nPosX, nPosY, 60);					
		}
		// 진동 사용 여부
		for(nCnt = 0;nCnt < MAX_OPTION_SYS_CNT;nCnt++)
		{
			nPosX = nBackPosX + OPTION_RADIO_JOYSTIC_FEED_X + (nCnt*OPTION_RADIO_JOYSTIC_ONOFF_CAP_X);
			nPosY = nBackPosY + OPTION_RADIO_JOYSTIC_FEED_Y;		
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_FEED][nCnt]->SetPosition(nPosX, nPosY, 60);					
		}
	}

	{		
		// 감도
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = JOY_COMBO_SENSE_MAIN_WIDTH;
		nMainHeight = JOY_COMBO_SENSE_MAIN_HEIGHT;
		nEleWidth = JOY_COMBO_SENSE_ELE_WIDTH;
		nEleHeight = JOY_COMBO_SENSE_ELE_HEIGHT;		
	
		nPosX = ptBkPos.x + JOY_COMBO_SENSE_X;
		nPosY = ptBkPos.y + JOY_COMBO_SENSE_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		m_pComboJoySense->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		m_pComboJoySense->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
		m_pComboJoySense->SetBGPos(nElePosX+6, nElePosY,
												nEleWidth, nEleHeight);
	}
	
	{		
		// 키 설정
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = JOY_COMBO_KEY_MAIN_WIDTH;
		nMainHeight = JOY_COMBO_KEY_MAIN_HEIGHT;
		nEleWidth = JOY_COMBO_KEY_ELE_WIDTH;
		nEleHeight = JOY_COMBO_KEY_ELE_HEIGHT;		
		
		nPosX = ptBkPos.x + JOY_COMBO_KEY_X;
		nPosY = ptBkPos.y + JOY_COMBO_KEY_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		m_pComboJoyFile->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		m_pComboJoyFile->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
		m_pComboJoyFile->SetBGPos(nElePosX+6, nElePosY,
												nEleWidth, nEleHeight);
	}
	
	{		
		// 장치
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = JOY_COMBO_DEVICE_MAIN_WIDTH;
		nMainHeight = JOY_COMBO_DEVICE_MAIN_HEIGHT;
		nEleWidth = JOY_COMBO_DEVICE_ELE_WIDTH;
		nEleHeight = JOY_COMBO_DEVICE_ELE_HEIGHT;		
		
		nPosX = ptBkPos.x + JOY_COMBO_DEVICE_X;
		nPosY = ptBkPos.y + JOY_COMBO_DEVICE_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		m_pComboJoyDevice->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		m_pComboJoyDevice->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
		m_pComboJoyDevice->SetBGPos(nElePosX+6, nElePosY,
												nEleWidth, nEleHeight);
	}
	{
		nPosX = ptBkPos.x + JOY_BTN_SAVE_X;
		nPosY = ptBkPos.y + JOY_BTN_SAVE_Y;		
		m_pOptionJoySave->SetBtnPosition(nPosX, nPosY);
	}
	{
		nPosX = ptBkPos.x + JOY_BTN_CLOSE_X;
		nPosY = ptBkPos.y + JOY_BTN_CLOSE_Y;		
		m_pOptionClose->SetBtnPosition(nPosX, nPosY);
	}
	
	{
		nPosX = ptBkPos.x + JOY_BTN_SAVEAS_X;
		nPosY = ptBkPos.y + JOY_BTN_SAVEAS_Y;		
		m_pOptionJoySaveAs->SetBtnPosition(nPosX, nPosY);
	}

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		ptScrollPos.x += 802;
		ptScrollPos.y += 194;
#else
		ptScrollPos.x += 813;
		ptScrollPos.y += 176;
#endif

		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,210);
		rcMouseWhell.left		= ptScrollPos.x - 790;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 270;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 300;
		m_pScroll->SetMouseBallRect(rcMousePos);

	}	
	{			
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = JOY_COMBO_SETUP_MAIN_WIDTH;
		nMainHeight = JOY_COMBO_SETUP_MAIN_HEIGHT;
		nEleWidth = JOY_COMBO_SETUP_ELE_WIDTH;
		nEleHeight = JOY_COMBO_SETUP_ELE_HEIGHT;		
		
		nPosX = ptBkPos.x + JOY_COMBO_KEY_X;
		nPosY = ptBkPos.y + JOY_COMBO_KEY_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		for(nMainId = 0;nMainId < ONEPAGE_JOY_SETUP_Y;nMainId++)
		{
			for(nCnt = 0;nCnt < ONEPAGE_JOY_SETUP_X;nCnt++)
			{				
				nPosX = ptBkPos.x + JOY_COMBO_SETUP_X + (nCnt*JOY_COMBO_SETUP_CAP_X);
				nPosY = ptBkPos.y + JOY_COMBO_SETUP_Y + (nMainId*JOY_COMBO_SETUP_CAP_Y);		
				
				nElePosX = nPosX;
				nElePosY = nPosY + nMainHeight;

				POINT ptPosTmp;
				ptPosTmp.x = ptBkPos.x + JOY_COMBO_SETUP_X + (nCnt*JOY_COMBO_SETUP_CAP_X);
				ptPosTmp.y = ptBkPos.y + JOY_COMBO_SETUP_Y + (nMainId*JOY_COMBO_SETUP_CAP_Y);		
				
				if(m_pComboJoySetup[nMainId][nCnt])
				{
					m_pComboJoySetup[nMainId][nCnt]->SetPos(ptPosTmp);

//					m_pComboJoySetup[nMainId][nCnt]->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
//					m_pComboJoySetup[nMainId][nCnt]->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
//					m_pComboJoySetup[nMainId][nCnt]->SetBGPos(nElePosX+6, nElePosY, nEleWidth, nEleHeight);
				}
			}
		}

	}

}

void CINFOpJoystick::UpdateOptionInterface(structJoyStickOptionInfo* pOptionInfo)
{
	int nMainId = 0;
	
	// 조이스틱 사용 유무
	{
		if(pOptionInfo->bUseJoystick)
		{
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_ONOFF][OPTION_SYS_ON]->SetRadioBtn(TRUE);
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_ONOFF][OPTION_SYS_OFF]->SetRadioBtn(FALSE);
		}
		else
		{
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_ONOFF][OPTION_SYS_ON]->SetRadioBtn(FALSE);
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_ONOFF][OPTION_SYS_OFF]->SetRadioBtn(TRUE);
		}		
	}	

	// 조이스틱 진동 유무 
	{
		if(pOptionInfo->bUseFeedJoyStick)
		{
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_FEED][OPTION_SYS_ON]->SetRadioBtn(TRUE);
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_FEED][OPTION_SYS_OFF]->SetRadioBtn(FALSE);
		}
		else
		{
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_FEED][OPTION_SYS_ON]->SetRadioBtn(FALSE);
			m_pJoyOpRadioBtn[JOY_OPTION_RADIO_FEED][OPTION_SYS_OFF]->SetRadioBtn(TRUE);
		}		
	}

	// 감도 
	{
		m_pComboJoySense->SetSelectItem(m_struJoyStickOptionInfo.nJoysticSense);
	}
	// 키 설정값
	{
		FindJoyFile();		
	}
	// 조이스틱 갯수 설정
	{
		InifJoystickList();
	}
}

void CINFOpJoystick::InifJoystickList()
{
	m_pComboJoyDevice->ItemClear();
	m_pComboJoyDevice->AddElement(" ");	// 첫칸은 공백
	
	int nCnt = 0;
	int nCurSel = 0;
	for(nCnt = 0;nCnt < g_pD3dApp->GetJoystickCtrlDeviceListLen();nCnt++)
	{
		char* pDevice = g_pD3dApp->GetJoystickCtrlDeviceList(nCnt);
		if(pDevice)
		{
			if(0 == stricmp(pDevice, m_struJoyStickOptionInfo.chJoySelJoy))
			{
				nCurSel = nCnt+1;
			}
			m_pComboJoyDevice->AddElement(pDevice);
		}
	}
	m_pComboJoyDevice->SetSelectItem(nCurSel);	
}

void CINFOpJoystick::SetRadioInfo(int nMainId, BOOL bRadioCheck)
{
	switch(nMainId)
	{
	case JOY_OPTION_RADIO_ONOFF:	// 조이스틱 사용유무
		{
			m_struJoyStickOptionInfo.bUseJoystick=bRadioCheck;
		}
		break;
	case JOY_OPTION_RADIO_FEED:		// 조이스틱 진동유무
		{
			m_struJoyStickOptionInfo.bUseFeedJoyStick = bRadioCheck;
		}
		break;
	}

	UpdateOptionInterface(&m_struJoyStickOptionInfo);
}


void CINFOpJoystick::InitJoysticSense()
{
	char chBuff[256];	
	memset(chBuff, 0x00, 256);

	int nCnt = 0;
	for(nCnt =0;nCnt <MAX_JOYSTICK_SENSITY;nCnt++)
	{
		// 마우스 포인터 감도
		sprintf(chBuff, "%d", nCnt-MAX_JOYSTICK_DEFAULTSENSITY);
		m_pComboJoySense->AddElement(chBuff);
	}
	m_pComboJoySense->UpdateItem();
	m_pComboJoySense->SetSelectItem(m_struJoyStickOptionInfo.nJoysticSense);
}

void CINFOpJoystick::FindJoyFile()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	char fname[MAX_PATH];
	BOOL bResult=TRUE;
	char chFullPath[MAX_PATH];
	int  nCnt=0;
	memset(chFullPath, 0x00, MAX_PATH);
	
	GetCurrentDirectory( MAX_PATH, chFullPath );
	strcat(chFullPath, "\\*.joy");	

	m_pComboJoyFile->ItemClear();
	m_pComboJoyFile->AddElement("");
	
	hSrch = FindFirstFile(chFullPath, &wfd);
	if(hSrch == INVALID_HANDLE_VALUE)
	{
		m_pComboJoyDevice->SetSelectItem(0);	
		DBGOUT("FindFirstFile : INVALID_HANDLE_VALUE = %d\n",GetLastError());
		FindClose(hSrch);
		
		return ;
	}
	
	int nCursel = 0;		

	while(bResult)
	{		
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//wsprintf(fname, "[ %s ]", wfd.cFileName);
		}
		else
		{
			wsprintf(fname, "%s", wfd.cFileName);			
			m_pComboJoyFile->AddElement(fname);
			if(!stricmp(fname, m_struJoyStickOptionInfo.chJoyKeyFile))
			{
				nCursel = nCnt+1;
			}
			nCnt++;
		}		
		bResult = FindNextFile(hSrch, &wfd);
	}	
	FindClose(hSrch);	
	
	m_pComboJoyFile->SetSelectItem(nCursel);
}

void CINFOpJoystick::RenderKeyInfomation()
{
	
	char chBuff[MAX_PATH];
	memset(chBuff, 0x00 ,MAX_PATH);
	
	int nCnt =0;
	int nPos =m_pScroll->GetScrollStep();

	int sizeY = JOY_SETUP_KEY_CONFIG_HEIGHT;
	POINT ptBakPos = m_ptBkPos;		
	int nPosX, nPosY;
	nPosX = ptBakPos.x + JOY_SETUP_KEY_CONFIG_X;
	nPosY = ptBakPos.y + JOY_SETUP_KEY_CONFIG_Y;
	for(nCnt =0; nCnt < ONEPAGE_JOY_SETUP_Y;nCnt++)
	{
		if( nPos >=MAX_JOSTICK_KEY_OPTION)
		{
			break;
		}		
		CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
		if(pJoyStick)
		{
			// 장치가 있다.
			structJoyStickKeyInfo *pJoyStickKeyInfo = pJoyStick->GetKeyInfomation(nPos);
			if(pJoyStickKeyInfo)
			{
				m_pFontTxt->DrawText(nPosX, nPosY, GUI_FONT_COLOR, pJoyStickKeyInfo->chJoysticInfo,0L);
			}
		}
		else
		{
			// 장치가 없다.
			m_pFontTxt->DrawText(nPosX, nPosY, GUI_FONT_COLOR, "",0L);
		}
		
		nPosY += sizeY;
		nPos++;
		
	}	
}

void CINFOpJoystick::RenderClickKey()
{
	CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
	if(!pJoyStick)
	{
		// 조이스틱 장치 없음
		return;
	}
	int sizeY = JOY_SETUP_KEY_CONFIG_HEIGHT;
	POINT ptBakPos = m_ptBkPos;		
	int nPosX, nPosY;
	nPosX = ptBakPos.x + JOY_SETUP_KEY_CONFIG_X-3;
	nPosY = ptBakPos.y + JOY_SETUP_KEY_CONFIG_Y;

	int nKeyValue = 0;
	int nPos =m_pScroll->GetScrollStep();
	int nCnt =0;
	for(nCnt =0; nCnt < ONEPAGE_JOY_SETUP_Y;nCnt++)
	{
		if(nPos >= MAX_JOSTICK_KEY_OPTION)
		{
			break;
		}
		structJoyStickKeyInfo *pJoyStickKeyInfo = pJoyStick->GetKeyInfomation(nPos);
		if(pJoyStickKeyInfo)
		{
			nKeyValue = pJoyStick->GetJoySitckInfo(pJoyStickKeyInfo->nKeyIdx);
			if(nKeyValue == pJoyStickKeyInfo->nKeyValue)
			{
				m_pSelImage->Move(nPosX, nPosY);
				m_pSelImage->Render();
				
			}
		}		
		nPos++;
		nPosY += sizeY;		
	}


}
int CINFOpJoystick::CheckJoySticBtn()
{
	CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
	if(!pJoyStick)
	{
		// 조이스틱 장치 없음
		return -1;
	}
	int nKeyValue = 0;
	int nPos =m_pScroll->GetScrollStep();	
	for(nPos =0; nPos < MAX_JOSTICK_KEY_OPTION;nPos++)
	{		
		structJoyStickKeyInfo *pJoyStickKeyInfo = pJoyStick->GetKeyInfomation(nPos);
		if(pJoyStickKeyInfo)
		{
			nKeyValue = pJoyStick->GetJoySitckInfo(pJoyStickKeyInfo->nKeyIdx);
			if(nKeyValue == pJoyStickKeyInfo->nKeyValue)
			{
				return nPos;				
			}
		}
	}
	return -1;
}

void CINFOpJoystick::UpdateKeyInfomation()
{
	CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
	int nKeyLen = ONEPAGE_JOY_SETUP_Y;
	if(pJoyStick)
	{
		// 조이스틱 장치 있음
		nKeyLen = pJoyStick->GetKeyJoystickLen();
	}
	m_pScroll->SetMaxItem(nKeyLen);
}


void CINFOpJoystick::UpdateKeyComboControls()
{
	int nPosX,nPosY;	
	nPosX = nPosY = 0;

	CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();	
	if(NULL == pJoyStick)
	{
		// 장치가 없다.
		AllComboBoxHide(-1, -1);	// 모두 숨기기
		for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
		{		
			for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
			{
				// 아무것도 없는 상태로
				m_pComboJoySetup[nPosY][nPosX]->SetCurSel(0);
			}
		}
		return;
	}

	AllComboBoxHide(-1, -1);	// 모두 숨기기

	int nMaxItemCnt = (ONEPAGE_JOY_SETUP_Y*ONEPAGE_JOY_SETUP_X);
	BOOL bBreak = FALSE;
	int nPos = m_pScroll->GetScrollStep();	
	
	for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
	{		
		for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
		{
			nPos = nPosY + m_pScroll->GetScrollStep();
			structJoystickSetupKeyTable	 *pJoystickSetupKeyTable = GetJoystickSetupKeyTable(nPos);
			if(!pJoystickSetupKeyTable)
			{				
				continue;
			}
			UpdateKeyCombo(pJoystickSetupKeyTable, nPosY, nPosX);	
			
		}
	}
}

void CINFOpJoystick::UpdateKeyCombo(structJoystickSetupKeyTable	*i_pstruKeyTable, int i_nPosY, int i_nPosX)
{
	int nSlot = i_nPosX;
	int nCursel = i_pstruKeyTable->nBehavior[nSlot] +1;
	m_pComboJoySetup[i_nPosY][i_nPosX]->SetCurSel(nCursel);
}

structJoystickSetupKeyTable	 *CINFOpJoystick::GetJoystickSetupKeyTable(int nKeyIndex)
{
	if(nKeyIndex < 0 || nKeyIndex >= MAX_JOSTICK_KEY_OPTION)
	{
		return NULL;
	}
	return &m_pJoystickSetupKeyTable[nKeyIndex];
}

// 선택한 콤보가 아닌 박스 다 숨기기
void CINFOpJoystick::AllComboBoxHide(int i_nPosY, int i_nPosX)
{
	int nPosX, nPosY;
	nPosX = nPosY = 0;

	m_nShowItemComboX = i_nPosX;
	m_nShowItemComboY = i_nPosY;
	
	for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
	{		
		for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
		{
			if(i_nPosY == nPosY && i_nPosX == nPosX)
			{
				continue;
			}
			m_pComboJoySetup[nPosY][nPosX]->ShowItem(FALSE);
		}
	}
}

// 
BOOL CINFOpJoystick::OnLButtonDownUpdateComoboBox(POINT pt)
{
	int nSelectComboX, nSelectComboY;
	nSelectComboX = nSelectComboY = 0;
	int nSelectEvent = LBTN_COMBOEX_NONE;
	
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	BOOL bContinue = FALSE;
		
	for(nPosY = 0;nPosY< ONEPAGE_JOY_SETUP_Y;nPosY++)
	{		
		if(nSelectEvent != LBTN_COMBOEX_NONE)
		{
			break;
		}
		for(nPosX = 0;nPosX< ONEPAGE_JOY_SETUP_X;nPosX++)
		{			
			bContinue = FALSE;
			if(nSelectEvent != LBTN_COMBOEX_NONE)
			{
				break;
			}
			if(nPosX >= 1)
			{
				int nTmpPosX = nPosX -1;
				if(0 == m_pComboJoySetup[nPosY][nTmpPosX]->GetCurSel())
				{
					// 내 앞에 항목이 체크한것이 없다.
					bContinue = TRUE;
				}
			}
			if(bContinue)
			{
				// 다음 체크한다.
				continue;
			}
			
			int nTmpEvent = m_pComboJoySetup[nPosY][nPosX]->OnLButtonDown(pt);
			if(LBTN_COMBOEX_NONE != nTmpEvent)
			{
				nSelectEvent = nTmpEvent;				
				nSelectComboX = nPosX;	
				nSelectComboY = nPosY;
				break;
			}
		}
	}	
	switch(nSelectEvent)
	{
	case LBTN_COMBOEX_SHOWITEM:
		{
			SelComboBoxRefresh(nSelectComboY, nSelectComboX); //선택한 콤보박스 아이템

			// 안보이다가 보이는 상황이다.
			AllComboBoxHide(nSelectComboY, nSelectComboX);	// 선택한 콤보가 아닌 박스 다 숨기기
			m_pComboJoySetup[nSelectComboY][nSelectComboX]->ShowItem(TRUE);			
			
			return  TRUE;
		}
		break;
	case LBTN_COMBOEX_HIDEITEM:
		{
			AllComboBoxHide(-1, -1);	// 선택한 콤보가 아닌 박스 다 숨기기
			return  TRUE;
		}
		break;
	case LBTN_COMBOEX_CHANGEITEM:
		{
			AllComboBoxHide(-1, -1);	// 선택한 콤보가 아닌 박스 다 숨기기
			
			int nCursel = m_pComboJoySetup[nSelectComboY][nSelectComboX]->GetCurSel();
			int nPos = nSelectComboY + m_pScroll->GetScrollStep();

			structJoystickSetupKeyTable	 *pJoystickSetupKeyTable = GetJoystickSetupKeyTable(nPos);
			
			if(pJoystickSetupKeyTable && nCursel >= 0)
			{
				int nSlot = nPosX;
				pJoystickSetupKeyTable->nBehavior[nSlot] = nCursel-1;

				if(0 == nCursel)
				{
					// 커서가 0이면 다음 항목들은 다 초기화
					int nTmpCurselX = nSelectComboX;
					int nTmpCnt = 0;
					for(nTmpCnt = nTmpCurselX+1;nTmpCnt<ONEPAGE_JOY_SETUP_X;nTmpCnt++)
					{
						m_pComboJoySetup[nSelectComboY][nTmpCnt]->SetCurSel(0);
						pJoystickSetupKeyTable->nBehavior[nTmpCnt] = -1;
					}
				}
				return  TRUE;
			}
		}
		break;
	case LBTN_COMBOEX_SELSCROLL:
		{
		}
		break;
	}		
	return  FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		파일저장
/// \author		
/// \date		2008-11-28 ~ 2008-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOpJoystick::OnClickSaveBtn()
{	
	INT	nCurSel = m_pComboJoyFile->GetSelect();
	strncpy(m_struJoyStickOptionInfo.chJoyKeyFile, m_pComboJoyFile->GetItem(nCurSel), MAX_PATH);	

	nCurSel = m_pComboJoyDevice->GetSelect()-1;
	if(nCurSel >= 0)
	{
		char * pDeviceName = g_pD3dApp->GetJoystickCtrlDeviceList(nCurSel);
		if(pDeviceName)
		{
			strncpy(m_struJoyStickOptionInfo.chJoySelJoy, pDeviceName, MAX_PATH);
		}
	}
	else
	{
		wsprintf(m_struJoyStickOptionInfo.chJoySelJoy, "");
	}
	

	// 실제 저장
	g_pInterface->SaveJoysticSetupInfo(m_struJoyStickOptionInfo.chJoyKeyFile, &m_struJoyStickOptionInfo, m_pJoystickSetupKeyTable);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		파일저장
/// \author		
/// \date		2008-11-28 ~ 2008-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOpJoystick::OnClickSaveAsBtn()
{
	g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_081127_0219, _Q_JOYSTICK_SETUP, (DWORD)this);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		파일저장
/// \author		
/// \date		2008-11-28 ~ 2008-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOpJoystick::SaveAsJoysticSetupInfo(char* pSaveAsFilename)
{	
	strncpy(m_struJoyStickOptionInfo.chJoyKeyFile, pSaveAsFilename, MAX_PATH);
	strcat(m_struJoyStickOptionInfo.chJoyKeyFile, ".joy");

//	INT	nCurSel = m_pComboJoyFile->GetSelect();
//	strncpy(m_struJoyStickOptionInfo.chJoyKeyFile, m_pComboJoyFile->GetItem(nCurSel), MAX_PATH);	


	int nCurSel = m_pComboJoyDevice->GetSelect()-1;
	if(nCurSel >= 0)
	{
		char * pDeviceName = g_pD3dApp->GetJoystickCtrlDeviceList(nCurSel);
		if(pDeviceName)
		{
			strncpy(m_struJoyStickOptionInfo.chJoySelJoy, pDeviceName, MAX_PATH);	
		}
	}
	else
	{
		wsprintf(m_struJoyStickOptionInfo.chJoySelJoy, "");
	}

	// 실제 저장
	g_pInterface->SaveJoysticSetupInfo(m_struJoyStickOptionInfo.chJoyKeyFile, &m_struJoyStickOptionInfo, m_pJoystickSetupKeyTable);

	FindJoyFile(); // 갱신

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아이템 선택
/// \author		
/// \date		2008-11-28 ~ 2008-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOpJoystick::SelComboBoxRefresh(int i_nSelectComboY, int i_nSelectComboX)
{
	int nRealSelectComboY = i_nSelectComboY + m_pScroll->GetScrollStep();
	int nCnt = 0;
	int nSlot = 0;
	POINT ptPos[MAX_JOSTICK_KEY_OPTION];
	BOOL bShowItem[MAX_JOSTICK_OPTION];
	for(nCnt =0; nCnt < MAX_JOSTICK_OPTION;nCnt++)
	{
		bShowItem[nCnt]= TRUE;
	}

	for(nCnt =0; nCnt < MAX_JOSTICK_KEY_OPTION;nCnt++)
	{
		ptPos[nCnt].x = ptPos[nCnt].y = -1;	

		structJoystickSetupKeyTable	 *pJoystickSetupKeyTable = GetJoystickSetupKeyTable(nCnt);
		if(pJoystickSetupKeyTable)
		{
			for(nSlot =0; nSlot < MAX_JOSTICK_OPTION_SLOT;nSlot++)
			{
				int nCurSel = pJoystickSetupKeyTable->nBehavior[nSlot];
				if((JOSTICK_OPTION_MOUSE_UP <= nCurSel) 
					&& (nCurSel <= JOSTICK_OPTION_UNIT_RIGHT))
				{
					if((nCnt == nRealSelectComboY) && (nSlot == i_nSelectComboX))
					{
					}
					else
					{
						bShowItem[nCurSel]= FALSE;
					}

				}
				
			}
		}
	}
	for(nCnt =0; nCnt < MAX_JOSTICK_OPTION;nCnt++)
	{
		m_pComboJoySetup[i_nSelectComboY][i_nSelectComboX]->UpdateItemInfo(nCnt, bShowItem[nCnt]);
	}


}


void CINFOpJoystick::InitSetupKeyTable()
{
	int i = 0;
	int nCnt = 0;
	for(i=0; i< MAX_JOSTICK_KEY_OPTION;i++)
	{					
		memset(&m_pJoystickSetupKeyTable[i], 0x00, sizeof(structJoystickSetupKeyTable));
		m_pJoystickSetupKeyTable[i].nKeySetup = -1;
		for(nCnt=0; nCnt< MAX_JOSTICK_OPTION_SLOT;nCnt++)
		{
			m_pJoystickSetupKeyTable[i].nBehavior[nCnt] = -1;
		}
	}
}
// 장치 변경
void CINFOpJoystick::OnDeviceChange(int nOldCursel) 
{
	int nCursel = m_pComboJoyDevice->GetSelect()-1;	
	int nCnt =0;	
	
	if(nCursel >= 0 && nOldCursel != nCursel )
	{
		char * pDeviceName = g_pD3dApp->GetJoystickCtrlDeviceList(nCursel);
		if(pDeviceName)
		{
			strncpy(m_struJoyStickOptionInfo.chJoySelJoy, pDeviceName, MAX_PATH);
			// 장치 변경 
			g_pD3dApp->InitJoyStickDevice(TRUE, TRUE, m_struJoyStickOptionInfo.chJoySelJoy);
			
			for(nCnt =0; nCnt < MAX_JOSTICK_KEY_OPTION;nCnt++)
			{
				// 저장된 정보에서 가져온다.
				structJoystickSetupKeyTable	 *pJoystickSetupKeyTable = g_pInterface->GetJoystickSetupKeyTable(nCnt);
				if(NULL == pJoystickSetupKeyTable)
				{
					continue;
				}
				memcpy(&m_pJoystickSetupKeyTable[nCnt], pJoystickSetupKeyTable, sizeof(structJoystickSetupKeyTable));
			}	
			
			UpdateKeyInfomation();
			UpdateKeyComboControls();
		}
		
	}
	
}