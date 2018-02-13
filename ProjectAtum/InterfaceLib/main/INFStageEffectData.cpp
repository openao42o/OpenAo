// INFStageEffectData.cpp: implementation of the CINFStageEffectData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFStageEffectData.h"
//#include "DXUtil.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "AtumSound.h"
#include "dxutil.h"
#include "INFImageEx.h"									// 2011. 10. 10 by jskim UI시스템 변경

extern LPDIRECT3DDEVICE9		g_pD3dDev;

#define IMAGE_SCALE				((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
// 2008-01-16 by bhsohn 프롤로그 스케일문제 해결
#define IMAGE_SCALE_Y			(((float)g_pD3dApp->GetBackBufferDesc().Height)/768.0f)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFStageEffectData::CINFStageEffectData()
{
	memset((void*)&m_stageEffect, 0x00, sizeof(m_stageEffect) );
	m_dwCurrentColor = 0;
	m_tBlinkTime.Set( 0.0f, 1.5f, 0.5f );
	m_pImage = NULL;
	m_pGameData = NULL;
	m_dwOldGameState = 0;

	m_byEndAct = NOT_ACTION;
	m_bSkip = FALSE;
	m_bSkipEnd = FALSE;

	m_nSceneIndex = -1;
}

CINFStageEffectData::~CINFStageEffectData()
{
	util::del(m_pImage);
	util::del(m_pGameData);
}

void CINFStageEffectData::Reset()
{
//	m_fCurrentSoundPlayTime = 0;
//	m_fCurrentImageShowTime = m_stageEffect.fShowTime;
	m_dwCurrentColor = 0;
	m_tBlinkTime.Start();
//	if(m_pImage)
//	{
//		m_pImage->InvalidateDeviceObjects();
//		m_pImage->DeleteDeviceObjects();
//		util::del(m_pImage);
//		util::del(m_pGameData);
//	}
}

void CINFStageEffectData::SetData( STAGE_EFFECT_DATA * pStageEffectData, BYTE byEndAct, BOOL bSkip )
{
//	m_stageEffect.fShowTime = fShowTime;
//	m_stageEffect.fSoundPlayTime = fSoundPlayTime;
//	m_stageEffect.nQuestNumber = nQuestNumber;
//	strcpy( m_stageEffect.szImageFile, szImageFile );
//	strcpy( m_stageEffect.szSoundFile, szSoundFile );

	if(pStageEffectData)
	{
		memcpy(&m_StageEffectData, pStageEffectData, sizeof(STAGE_EFFECT_DATA));
	}

	m_byEndAct = byEndAct;
	m_bSkip = bSkip;
}

HRESULT CINFStageEffectData::LoadData( char* i_szFileName )
{
	char strPath[MAX_PATH];
	m_pGameData = new CGameData();
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, i_szFileName);
	m_pGameData->SetFile(strPath, FALSE, NULL, 0);
	DataHeader* pHeader = m_pGameData->Find( "info" );
	if( pHeader )
	{
		char* pData = pHeader->m_pData;
		memcpy(&m_stageEffect, pData, sizeof(STAGE_EFFECT));

		pData += sizeof(STAGE_EFFECT);
		// 2006-09-07 by ispark, SECEN_EFFECT
		for(int i = 0 ; i < m_stageEffect.nSceneCount; i++)
		{
			SCENE_EFFECT stScene_Effect;
			memcpy(&stScene_Effect, pData, sizeof(SCENE_EFFECT));
			m_vecScene.push_back(stScene_Effect);

			pData += sizeof(SCENE_EFFECT);
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CINFStageEffectData::InitDeviceObjects()
{
//	if(strlen(m_stageEffect.szSoundNarration)==0)
//	{
//		return E_FAIL;
//	}
	if(m_stageEffect.nSceneCount <= 0)
	{
		return E_FAIL;
	}

	Reset();
	ResetScene();				// 2006-09-07 by ispark
	DataHeader* pHeader;

//	DataHeader* pHeader = m_pGameData->Find ( m_stageEffect.szSoundNarration );
//	if( pHeader )
//	{
//		m_pImage = new CINFImage;
//		m_pImage->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
//	}
	// 기존 부스터 끄기
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_M_A_GEAR );
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_B_I_GEAR );
	g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );

	// 나레이션
	pHeader = m_pGameData->Find ( m_stageEffect.szSoundNarration );
	if( pHeader )
	{
		g_pD3dApp->m_pSound->PlayD3DSound( NARRATIONSOUND, pHeader->m_pData, D3DXVECTOR3(0,0,0), FALSE );
	}

	// BGM
	pHeader = m_pGameData->Find ( m_stageEffect.szSoundBGM );
	if( pHeader )
	{
		g_pD3dApp->m_pSound->PlayD3DSound( BGMSOUNT, pHeader->m_pData, D3DXVECTOR3(0,0,0), FALSE );
	}

//	if( FAILED( D3DXCreateTextureFromFile( g_pD3dDev, m_stageEffect.szImageFile, 
//										   &m_pTexture ) ) )
//	{
//		return E_FAIL;
//	}
	return S_OK;
}

HRESULT CINFStageEffectData::RestoreDeviceObjects()
{
	if(m_pImage)
	{
		m_pImage->RestoreDeviceObjects();
		// 2008-01-16 by bhsohn 프롤로그 스케일문제 해결
		//m_pImage->SetScale( IMAGE_SCALE, IMAGE_SCALE );		
		m_pImage->SetScale( IMAGE_SCALE, IMAGE_SCALE_Y);
	}
	return S_OK;
}

HRESULT CINFStageEffectData::InvalidateDeviceObjects()
{
	if(m_pImage)
	{
		m_pImage->InvalidateDeviceObjects();
	}
	return S_OK;
}

HRESULT CINFStageEffectData::DeleteDeviceObjects()
{
	if(m_pImage)
	{
		m_pImage->DeleteDeviceObjects();
		util::del(m_pImage);
	}
	g_pD3dApp->m_pSound->DelD3DSound( NARRATIONSOUND );
	g_pD3dApp->m_pSound->DelD3DSound( BGMSOUNT );

	return S_OK;
}
BOOL CINFStageEffectData::Tick(float fElapsedTime)
{
	// 2006-09-07 by ispark, 여러 이미지로 가능하게 수정
//	if(m_fCurrentSoundPlayTime < m_stageEffect.fSoundPlayTime)
//	{
//		// 플레이 시간 계산
//		m_fCurrentSoundPlayTime += fElapsedTime;
//		if(m_fCurrentImageShowTime > 0)
//		{
//			// 이미지를 보여주기 위해 기다리는 시간
//			m_fCurrentImageShowTime -= fElapsedTime;
//		}
//		else
//		{
//			// 이미지를 서서히 보여줌
//			m_tBlinkTime.Tick( fElapsedTime );
//			DWORD dwColor = 0x00FFFFFF;
//			DWORD alpha;
//			if( m_tBlinkTime.IsOverMiddle() == FALSE ) // && m_dwCurrentColor != 0xFFFFFFFF)
//			{
//				alpha = (DWORD)(255* (1.0f - m_tBlinkTime.GetPositionRate() ));
//			}
//			else
//			{
//				alpha = 255;///(DWORD)(255* m_tBlinkTime.GetPositionRate() );
//			}
//			alpha = alpha << 24;
//			m_dwCurrentColor = alpha | dwColor;
//
//		}
//		// 서서히 사라지기 시작
//		if(m_fCurrentSoundPlayTime >= m_stageEffect.fSoundPlayTime)
//		{
//			m_tBlinkTime.Start();
//		}
//	}
//	else
//	{
//		// 이미지 서서히 사라짐
//		m_tBlinkTime.Tick( fElapsedTime );
//		DWORD dwColor = 0x00FFFFFF;
//		DWORD alpha;
//		if( m_tBlinkTime.IsOverMiddle() == FALSE ) // && m_dwCurrentColor != 0xFFFFFFFF)
//		{
//			alpha = (DWORD)(255* m_tBlinkTime.GetPositionRate() );
//		}
//		else
//		{
//			alpha = 0;
//			return FALSE;
//		}
//		alpha = alpha << 24;
//		m_dwCurrentColor = alpha | dwColor;
//	}
//	return TRUE;

	// 스킵을 했음
	if(m_bSkipEnd)
	{
		return FALSE;
	}

	if(m_fCurrentPlayTime < m_vecScene[m_nSceneIndex].fScenePlay)
	{
		// 플레이 시간 계산
		m_fCurrentPlayTime += fElapsedTime;
		if(m_fCurrentReadyTime >= 0)
		{
			// 이미지를 보여주기 위해 기다리는 시간
			m_fCurrentReadyTime -= fElapsedTime;
		}
		else
		{
			// 이미지를 서서히 보여줌
			m_tBlinkTime.Tick( fElapsedTime );
			DWORD dwColor = 0x00FFFFFF;
			DWORD alpha;
			if( m_tBlinkTime.IsOverMiddle() == FALSE ) // && m_dwCurrentColor != 0xFFFFFFFF)
			{
				alpha = (DWORD)(255* (1.0f - m_tBlinkTime.GetPositionRate() ));
			}
			else
			{
				alpha = 255;///(DWORD)(255* m_tBlinkTime.GetPositionRate() );
			}
			alpha = alpha << 24;
			m_dwCurrentColor = alpha | dwColor;
			
		}
		// 서서히 사라지기 시작
		if(m_fCurrentPlayTime >= m_vecScene[m_nSceneIndex].fScenePlay)
		{
			m_tBlinkTime.Start();
		}
	}
	else
	{
		// 이미지 서서히 사라짐
		m_tBlinkTime.Tick( fElapsedTime );
		DWORD dwColor = 0x00FFFFFF;
		DWORD alpha;
		if( m_tBlinkTime.IsOverMiddle() == FALSE ) // && m_dwCurrentColor != 0xFFFFFFFF)
		{
			alpha = (DWORD)(255* m_tBlinkTime.GetPositionRate() );
		}
		else
		{
			alpha = 0;
			if(ResetScene() == FALSE)
				return FALSE;
		}
		alpha = alpha << 24;
		m_dwCurrentColor = alpha | dwColor;
	}
	//플레이 시간을 체크한다.
	m_fCurrentAllTime += fElapsedTime;
	if(m_stageEffect.nStageTime < (int)m_fCurrentAllTime)
	{
		m_fCurrentAllTime = 0;
		return FALSE;
	}
	return TRUE;	
}

void CINFStageEffectData::Render()
{
	m_pImage->SetColor( m_dwCurrentColor );
	m_pImage->Render();
}

int CINFStageEffectData::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
			{
				if(m_bSkip)
				{
					m_bSkipEnd = TRUE;
				}
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

BOOL CINFStageEffectData::ResetScene()
{
	
	if(m_nSceneIndex < -1 || 
		m_nSceneIndex+1 >= m_stageEffect.nSceneCount ||
		m_stageEffect.nStageTime < (int)m_fCurrentAllTime)
	{
		m_fCurrentAllTime = 0;
		return FALSE;
	}
	m_nSceneIndex++;
//	DBGOUT("m_nSceneIndex = %d\n",m_nSceneIndex);

	// 이미지 로드
	if(m_nSceneIndex < 0 ||
		strlen(m_vecScene[m_nSceneIndex].szImageFileName)==0)
	{
		return E_FAIL;
	}

	if(m_pImage)
	{
		InvalidateDeviceObjects();
		m_pImage->DeleteDeviceObjects();
	}
	util::del(m_pImage);
	DataHeader* pHeader = m_pGameData->Find ( m_vecScene[m_nSceneIndex].szImageFileName );
	if( pHeader )
	{
		m_pImage = new CINFImageEx;								// 2011. 10. 10 by jskim UI시스템 변경
		m_pImage->InitDeviceObjects( pHeader );
		RestoreDeviceObjects();
	}
	
	m_fCurrentReadyTime = m_vecScene[m_nSceneIndex].fImageReady;
	m_fCurrentPlayTime = 0.0f;
	Reset();
	
	return TRUE;
}