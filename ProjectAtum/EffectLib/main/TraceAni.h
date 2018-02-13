#pragma once

// TraceAni.h: interface for the CTraceAni class.
//////////////////////////////////////////////////////////////////////

#include "Effect.h"

#define NUMBER_OF_EFFECT_PLANE		1//(m_nNumberOfCross*4)
#define PLANE_VB_SIZE				16//최소 사이즈 : 8

struct TraceData : public Effect
{
	char		m_strName[20];				//이름
	UINT		m_nNumberOfTrace;			// 생성될 잔상박스의 개수 
	float		m_fCreateTick;				//생성 간격(작을수록 촘촘한 잔상이 생성된다.)
	//D3DXVECTOR3 m_vCreatePos;				//생성 좌표(상대좌표) : CEffectInfo로 넘어감
	float		m_fHalfSize;				// 잔상의 가로 길이의 절반
	UINT		m_nTextureNumber;			// 텍스쳐 개수
	float		m_fTextureAnimationTime;	// 텍스쳐 애니메이션 타임
	char		m_strTextureName[20][20];		// 텍스쳐는 최대 20개만 쓸수 있다.
	int			m_nNumberOfCross;				// 크로스 개수(1 ~ 4)
	
	// todo : m_bAlphaBlendEnable >> 16 is the alpha value for shader (temporary)
	// if alpha value is 0, the default 0.8 is used
	BOOL		m_bAlphaBlendEnable; 

	DWORD		m_dwSrcBlend;
	DWORD		m_dwDestBlend;
	DWORD		m_nTextureRenderState;
	BOOL		m_bZbufferEnable;
	BOOL		m_bZWriteEnable;
	
};

class CTraceAni;

class CEffectPlane : public Effect
{
	friend CTraceAni;
public:
	CEffectPlane(CTraceAni*	pParent, int nIBIndex);
	~CEffectPlane();

//	D3DXVECTOR3 &GetReferencePos() { return m_vPos; }
//	TRACEANI_INDEX &GetReferenceTraceAniIndex() { return m_TraceAniIndex; }

	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void SetFirstIndex(bool bSet);
	void SetEndIndexOriginal();
	void SetEndIndex();
	void SetEndIndex2();
	const D3DXVECTOR3& GetPos() const { return m_vPos; }

public:
	CTraceAni				*m_pParent;				// 부모 객체
protected:
	D3DXVECTOR3				m_vPos ;				// 유닛좌표를 계산한 절대좌표(ZEnable시에 거리계산에 사용), parent의 m_vPos참조
	int						m_nIBIndex;				// 0 ~ 15 사이 값
	//LPDIRECT3DVERTEXBUFFER9 m_pVB;					// 버텍스버퍼(4*32)
	BOOL					m_bIsFirstIndex;
	BOOL					m_bZbufferEnable;
	
public:
	TRACE_INSTANCE			m_TraceInstance;
};

class CEffectInfo;
class CEffectPlane;

class CTraceAni : public TraceData
{
public:
	float					m_fCurrentCreateTick;		// 생성대기 시간
	int						m_nCurrentNumberOfTrace;	// 현재까지 생성된 잔상개수(MAX=m_nNumberOfTrace)
	int						m_nCurrentTextureNumber;	// 현재 세팅된 텍스쳐 번호
//	LPDIRECT3DVERTEXBUFFER8 m_pVB;					// 버텍스버퍼
	float					m_fCurrentTextureTick;		// 텍스쳐 애니메이션 대기시간
	BOOL					m_bCreateTrace;				// 이 값이 FALSE이면 더이상 잔상을 만들지 않는다.
	CEffectInfo				*m_pParent;					// 이펙트 정보 파일
	CEffectPlane			**m_pEffectPlane;		// CEffectPlane 버퍼(16 X m_nNumberOfTrace)
//	LPDIRECT3DINDEXBUFFER8	*m_pIB;					// 인덱스 버퍼, 16 X m_nNumberOfTrace, CEffectPlane와 1:1매치
	int						m_nCurrentCreateIndex;	// 현재 생성할 VB,Plane 인덱스(0 ~ m_nNumberOfTrace)
	int						m_nCurrentRenderIndex;	// 현재 렌더링이 시작되는 인덱스
	int						m_nCurrentDeleteIndex;	// 지워질 경우 지워지는 인덱스
	D3DXVECTOR3				m_vOldPos;
	D3DXVECTOR3				m_vCurrentPos;
	BOOL					m_bRestored;

	float					m_fRadius;
public:
	CTraceAni();
	~CTraceAni();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	bool EnableAlphaBlend() const { return (m_bAlphaBlendEnable & 0xFF) != 0; }
	unsigned char GetAlphaValueByte() const { return (m_bAlphaBlendEnable >> 8) & 0xFF; }
	float GetAlphaValue() const { return GetAlphaValueByte() > 0 ? float(GetAlphaValueByte()) / 255.0f : 0.2f; }

	void TraceEnable(bool bEnable) { m_bCreateTrace = bEnable; }
	//void SetTexture(char* strTexture) {};
	//void SetTexture(LPDIRECT3DTEXTURE9 pTexture) {};
};