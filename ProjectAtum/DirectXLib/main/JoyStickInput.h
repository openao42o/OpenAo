// JoyStickInput.h: interface for the CJoyStickInput class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <dinput.h>

//////////// 조이스틱 각키 값 선언 //////////// 
#define		DIRECT_INPUT_JOSTICK_X			0	    /* x-axis position              */
#define		DIRECT_INPUT_JOSTICK_Y			1		/* y-axis position              */
#define		DIRECT_INPUT_JOSTICK_Z			2		/* z-axis position              */
#define		DIRECT_INPUT_JOSTICK_RX			3		/* x-axis rotation              */
#define		DIRECT_INPUT_JOSTICK_RY			4		/* y-axis rotation              */
#define		DIRECT_INPUT_JOSTICK_RZ			5		/* z-axis rotation              */
#define		DIRECT_INPUT_JOSTICK_SLIDER0	6		/* extra axes positions         */
#define		DIRECT_INPUT_JOSTICK_SLIDER1	7		
#define		DIRECT_INPUT_JOSTICK_POW0		8		/* POV directions               */
#define		DIRECT_INPUT_JOSTICK_POW1		9		
#define		DIRECT_INPUT_JOSTICK_POW2		10		
#define		DIRECT_INPUT_JOSTICK_POW3		11		    
#define		DIRECT_INPUT_JOSTICK_BTN0		12		/* 128 buttons                  */
#define		DIRECT_INPUT_JOSTICK_BTN1		13		    
#define		DIRECT_INPUT_JOSTICK_BTN2		14
#define		DIRECT_INPUT_JOSTICK_BTN3		15
#define		DIRECT_INPUT_JOSTICK_BTN4		16
#define		DIRECT_INPUT_JOSTICK_BTN5		17
#define		DIRECT_INPUT_JOSTICK_BTN6		18
#define		DIRECT_INPUT_JOSTICK_BTN7		19
#define		DIRECT_INPUT_JOSTICK_BTN8		20
#define		DIRECT_INPUT_JOSTICK_BTN9		21
#define		DIRECT_INPUT_JOSTICK_BTN10		22
#define		DIRECT_INPUT_JOSTICK_BTN11		23
#define		DIRECT_INPUT_JOSTICK_BTN12		24
#define		DIRECT_INPUT_JOSTICK_BTN13		25
#define		DIRECT_INPUT_JOSTICK_BTN14		26
#define		DIRECT_INPUT_JOSTICK_BTN15		27
#define		DIRECT_INPUT_JOSTICK_BTN127		139		    
#define		MAX_DIRECT_INPUT_JOSTICK		140
//////////// end 조이스틱 선언 //////////// 

// 2008-11-13 by bhsohn 조이스틱 작업
// 조이스틱 셋업창 옵션
#define		JOY_SETUP_XAXIS_MINUS				0		//"X Axis(-)"
#define		JOY_SETUP_XAXIS_PLUS				1		//"X Axis(+)"
#define		JOY_SETUP_YAXIS_MINUS				2		//"Y Axis(-)"
#define		JOY_SETUP_YAXIS_PLUS				3		//"Y Axis(+)"
#define		JOY_SETUP_ZAXIS_MINUS				4		//"Z Axis(-)"
#define		JOY_SETUP_ZAXIS_PLUS				5		//"Z Axis(+)"
#define		JOY_SETUP_RXAXIS_MINUS				6		//"RX Axis(-)"
#define		JOY_SETUP_RXAXIS_PLUS				7		//"RX Axis(+)"
#define		JOY_SETUP_RYAXIS_MINUS				8		//"RY Axis(-)"
#define		JOY_SETUP_RYAXIS_PLUS				9		//"RY Axis(+)"
#define		JOY_SETUP_RZAXIS_MINUS				10		//"RZ Axis(-)"
#define		JOY_SETUP_RZAXIS_PLUS				11		//"RZ Axis(+)"
#define		JOY_SETUP_POW_LEFT					12		//"십자버튼-좌"
#define		JOY_SETUP_POW_RIGHT					13		//"십자버튼-우"
#define		JOY_SETUP_POW_UP					14		//"십자버튼-상"
#define		JOY_SETUP_POW_DOWN					15		//"십자버튼-하"
#define		JOY_SETUP_BTN0						16		//BTN1
#define		JOY_SETUP_BTN1						17		//BTN1
#define		JOY_SETUP_BTN2						18		//BTN1
#define		JOY_SETUP_BTN3						19		//BTN1
#define		JOY_SETUP_BTN4						20		//BTN1
#define		JOY_SETUP_BTN5						21		//BTN1
#define		JOY_SETUP_BTN6						22		//BTN1
#define		JOY_SETUP_BTN7						23		//BTN1
#define		JOY_SETUP_BTN8						24		//BTN1
#define		JOY_SETUP_BTN9						25		//BTN1
#define		JOY_SETUP_BTN10						26		//BTN1
#define		JOY_SETUP_BTN11						27		//BTN1
#define		JOY_SETUP_BTN12						28		//BTN1
#define		JOY_SETUP_BTN13						29		//BTN1
#define		JOY_SETUP_BTN14						30		//BTN1
#define		JOY_SETUP_BTN15						31		//BTN1
#define		JOY_SETUP_BTN16						32		//BTN1
#define		JOY_SETUP_BTN17						33		//BTN1
#define		JOY_SETUP_BTN18						34		//BTN1
#define		JOY_SETUP_BTN19						35		//BTN1
#define		MAX_JOSTICK_KEY_OPTION				40		// 조이스틱 키설정 최대

// 조이스틱 옵션 저장 정보
#define		JOY_SETUP_OP_XAXIS_MINUS			"x(-)"	//"X Axis(-)"
#define		JOY_SETUP_OP_XAXIS_PLUS				"x(+)"	//"X Axis(+)"
#define		JOY_SETUP_OP_YAXIS_MINUS			"y(-)"	//"Y Axis(-)"
#define		JOY_SETUP_OP_YAXIS_PLUS				"y(+)"	//"Y Axis(+)"
#define		JOY_SETUP_OP_ZAXIS_MINUS			"z(-)"	//"Z Axis(-)"
#define		JOY_SETUP_OP_ZAXIS_PLUS				"z(+)"		//"Z Axis(+)"
#define		JOY_SETUP_OP_RXAXIS_MINUS			"rx(-)"		//"RX Axis(-)"
#define		JOY_SETUP_OP_RXAXIS_PLUS			"rx(+)"		//"RX Axis(+)"
#define		JOY_SETUP_OP_RYAXIS_MINUS			"ry(-)"		//"RY Axis(-)"
#define		JOY_SETUP_OP_RYAXIS_PLUS			"ry(+)"			//"RY Axis(+)"
#define		JOY_SETUP_OP_RZAXIS_MINUS			"rz(-)"		//"RZ Axis(-)"
#define		JOY_SETUP_OP_RZAXIS_PLUS			"rz(+)"		//"RZ Axis(+)"
#define		JOY_SETUP_OP_POW_LEFT				"left"		//"십자버튼-좌"
#define		JOY_SETUP_OP_POW_RIGHT				"right"		//"십자버튼-우"
#define		JOY_SETUP_OP_POW_UP					"up"		//"십자버튼-상"
#define		JOY_SETUP_OP_POW_DOWN				"down"		//"십자버튼-하"
#define		JOY_SETUP_OP_BTN0					"btn1"		//BTN1
#define		JOY_SETUP_OP_BTN1					"btn2"		//BTN1
#define		JOY_SETUP_OP_BTN2					"btn3"		//BTN1
#define		JOY_SETUP_OP_BTN3					"btn4"		//BTN1
#define		JOY_SETUP_OP_BTN4					"btn5"		//BTN1
#define		JOY_SETUP_OP_BTN5					"btn6"		//BTN1
#define		JOY_SETUP_OP_BTN6					"btn7"		//BTN1
#define		JOY_SETUP_OP_BTN7					"btn8"		//BTN1
#define		JOY_SETUP_OP_BTN8					"btn9"		//BTN1
#define		JOY_SETUP_OP_BTN9					"btn10"		//BTN1
#define		JOY_SETUP_OP_BTN10					"btn11"		//BTN1
#define		JOY_SETUP_OP_BTN11					"btn12"		//BTN1
#define		JOY_SETUP_OP_BTN12					"btn13"		//BTN1
#define		JOY_SETUP_OP_BTN13					"btn14"		//BTN1
#define		JOY_SETUP_OP_BTN14					"btn15"		//BTN1
#define		JOY_SETUP_OP_BTN15					"btn16"		//BTN1
#define		JOY_SETUP_OP_BTN16					"btn17"		//BTN1
#define		JOY_SETUP_OP_BTN17					"btn18"		//BTN1
#define		JOY_SETUP_OP_BTN18					"btn19"		//BTN1
#define		JOY_SETUP_OP_BTN19					"btn20"		//BTN1


// 기본 조이스틱 방향수
#define		MAX_DEFAULT_JOY_SETUP				16

// 조이스틱 진동 타입
#define		JOY_FEED_COLLISION_DAMAGE			0		// 충돌
#define		JOY_FEED_WEAPON1_DAMAGE				1		// 1형무기 데미지
#define		JOY_FEED_WEAPON2_DAMAGE				2		// 2형무기 데미지
#define		JOY_FEED_SHUTTLE_EXPLODED			3		// 기체 파괴
#define		MAX_JOY_FEED						4		


// end 2008-11-13 by bhsohn 조이스틱 작업

#define		MAX_JOYSTICK_SENSITY			5		// 최대 감도
#define		MAX_JOYSTICK_VALUE				101		// 값 갯수
#define		MAX_JOYSTICK_DEFAULTSENSITY		2		// 기본 감도

struct structJoysticValue 
{
	float fRepresentAtiveValue;		// 대표값
	float fAtiveValue;				// 값	
};

typedef struct 
{
	BOOL bXAxis;	// GUID_XAxis
	BOOL bYAxis;	// GUID_YAxis
	BOOL bZAxis;	// GUID_ZAxis
	
	BOOL bRXAxis;	// GUID_RxAxis
	BOOL bRYAxis;	// GUID_RyAxis
	BOOL bRZAxis;	// GUID_RzAxis
	
	int	nSlideCnt;	// GUID_Slider
	int	nPovCnt;	// GUID_POV
	int	nBtnCnt;	// GUID_Button
	

} structJoyStickStats;

typedef struct 
{		
	int			nKeyIdx;					// 키 인덱스
	int			nKeyValue;					// 키 값
	char		chJoysticInfo[MAX_PATH];	// 키 설명
		
} structJoyStickKeyInfo;


class CJoyStickInput  
{
public:
	CJoyStickInput();
	virtual ~CJoyStickInput();

	HRESULT InitDirectInput( HWND hDlg, char* i_pJoySelDeviceName );

	static BOOL CALLBACK EnumOnlyJoysticksListCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext );

	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext );
	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext );

	HRESULT UpdateInputState();
	VOID FreeDirectInput();

	int GetJoySitckInfo(int nIdx);
	BOOL IsChangeJoySitckInfo(int nIdx);

	BOOL	IsJoystickWheel(int nKeyIndex);
	float	GetSensity(int i_nSensity, float i_fValue);
	BOOL	IsJoystickPOV(int nKeyIndex);

	void	InitChangeDIJ();
	int		GetJoyStickBtnCnt();

	structJoyStickKeyInfo *GetKeyInfomation(int nKeyInfo);
	int		GetKeyJoystickLen();	

	void	GetKeyKeyValue_To_JoysticInfo(int i_nKeyIndex, int* o_pKeyIndex, int* o_pKeyValue);

	static void	UpdateJoysticKeyInfomation();

	char*	GetSelJoystickCtrlDevice();
	char*	GetJoystickCtrlDeviceList(int i_nIdx);
	int		GetJoystickCtrlDeviceListLen();

	
private:
	void InitSensity();
	void UpdateSenstity(int i_nSensity);	

	int				m_nDIJ[MAX_DIRECT_INPUT_JOSTICK];
	BOOL			m_bChangeDIJ[MAX_DIRECT_INPUT_JOSTICK];

	structJoysticValue			m_SensyTable[MAX_JOYSTICK_SENSITY][MAX_JOYSTICK_VALUE];	
	
};