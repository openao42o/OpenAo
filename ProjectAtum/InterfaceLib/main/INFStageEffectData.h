// INFStageEffectData.h: interface for the CINFStageEffectData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSTAGEEFFECTDATA_H__B3508266_078E_4281_B0B6_F9229AEF0B8E__INCLUDED_)
#define AFX_INFSTAGEEFFECTDATA_H__B3508266_078E_4281_B0B6_F9229AEF0B8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <D3dUtil.h>
//#include <d3d8.h>
#include "RangeTime.h"
//#include "INFImage.h"

//class CRangeTime;
class CINFImage;
class CINFImageEx;									  // 2011. 10. 10 by jskim UI시스템 변경
class CGameData;

#define			FILENAME_LEGHT		16
#define			NARRATIONSOUND		5001
#define			BGMSOUNT			5002

//struct STAGE_EFFECT
//{
//	char	szImageFile[16];
//	char	szSoundFile[16];
//	float	fSoundPlayTime;
//	float	fShowTime;		// image가 나타나기 시작하는 시간
//	UINT	nQuestNumber;
//};
struct SCENE_EFFECT
{
	int			num;									//Number.
	char		szImageFileName[FILENAME_LEGHT];		//이미지 파일명.
	float		fImageReady;							//이미지 대기 시간.
	float		fScenePlay;								//한 Scene이보여지는 시간.
};

struct STAGE_EFFECT
{
	int		nSceneCount;								//총 씬의 개수.
	int		nStageTime;									// stage time
	char	szSoundNarration[FILENAME_LEGHT];			//나레이션 파일명.
	char    szSoundBGM[FILENAME_LEGHT];					//배경음악 파일명.
	UINT	nQuestNumber;
};

class CINFStageEffectData  
{
public:
	CINFStageEffectData();
	virtual ~CINFStageEffectData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	BOOL Tick(float fElapsedTime);
	virtual void Render();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Reset();
	HRESULT LoadData( char* i_szFileName );
	void SetData( STAGE_EFFECT_DATA * pStageEffectData, BYTE byEndAct, BOOL bSkip );
	void SetOldGameState(DWORD dwState) { m_dwOldGameState = dwState; }
	DWORD GetOldGameState() { return m_dwOldGameState; }
	BOOL ResetScene();

	BYTE GetEndAct() { return m_byEndAct; }
	BOOL GetStageEffectSkip() { return m_bSkip; }
	STAGE_EFFECT_DATA * GetStageEffectData() { return &m_StageEffectData; } 

protected:
	int					m_nSceneIndex;				//씬의 인덱스 번호.
	std::vector<SCENE_EFFECT>	m_vecScene;			//씬들의 정보.

	STAGE_EFFECT		m_stageEffect;
	CGameData*			m_pGameData;
	CINFImageEx*		m_pImage;					// 2011. 10. 10 by jskim UI시스템 변경
	DWORD				m_dwCurrentColor;
	float				m_fCurrentPlayTime;			// fSoundPlayTime이 되면 Image가 서서히 사라짐
	float				m_fCurrentReadyTime;		// fShowTime 부터 출발해서 0이되면 서서히 보이기 시작함
	float				m_fCurrentAllTime;			// 전체 시간.
//	float				m_fCurrentSoundPlayTime;	// fSoundPlayTime이 되면 Image가 서서히 사라짐
//	float				m_fCurrentImageShowTime;	// fShowTime 부터 출발해서 0이되면 서서히 보이기 시작함
	CRangeTime			m_tBlinkTime;
	DWORD				m_dwOldGameState;

	STAGE_EFFECT_DATA	m_StageEffectData;
	BYTE				m_byEndAct; 
	BOOL				m_bSkip;
	BOOL				m_bSkipEnd;
};

#endif // !defined(AFX_INFSTAGEEFFECTDATA_H__B3508266_078E_4281_B0B6_F9229AEF0B8E__INCLUDED_)
