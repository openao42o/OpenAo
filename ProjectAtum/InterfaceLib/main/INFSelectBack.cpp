// INFSelectBack.cpp: implementation of the CINFSelectBackBack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RangeTime.h"
#include "INFUnitCreateInfo.h"
#include "INFSelectBack.h"
#include "AtumApplication.h"
#include "DbgOut_C.h"
#include "INFCreateMenu.h"
#include "SkinnedMesh.h"
#include "Camera.h"
#include "FieldWinSocket.h"
#include "INFSelect.h"
#include "UnitRender.h"
#include "UnitData.h"
#include "AtumDatabase.h"
#include "AppEffectData.h"
#include "AtumSound.h"
#include "dxutil.h"

#define VEL_CAMERA_MOVE				g_pD3dApp->GetElapsedTime()*120*1.0f	// 카메라 속도
#define VEL_CREATE_UNIT_ROTATE_MOVE	g_pD3dApp->GetElapsedTime()*1.5f*1.0f	// CREATE 유닛 회전 속도
#define VEL_SELECT_UNIT_ROTATE_MOVE	g_pD3dApp->GetElapsedTime()*2.0f*1.0f	// SELECT 유닛 회전 속도
//#define CAMERA_UP_POSITION		0.0f,-105.536f,65.844f					
#define CAMERA_UP_POSITION			0.0f,-105.536f,65.844f					// 카메라가 위에 올라갔을 경우 좌표
#define CAMERA_DOWN_POSITION		0.0f,-90.536f,55.844f					// 카메라가 정상 상태에서의 좌표
//#define CAMERA_DOWN_POSITION			0.0f,-90.536f,55.844f				
#define CAMERA_TARGET_POSITION		0.0f,-36.411f,16.802f//0.0f,0.0f,0.0f	// 카메라 타겟 좌표
#define UNIT_POSITION				0.0f,-36.411f,21.802f+1.087f			// 유닛의 맨 앞 좌표
#define ROTATE_RATTLING				0.0f									// 회전시 덜커덕 거리는 양
#define UNIT_ROTATE_START_TIME		0.5f									// 유닛 회전 시작 시간
#define SELECT_TOP_HEIGHT			(5.802f+3.087f)							// 유닛과 TOP과의 거리 차이
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSelectBack::CINFSelectBack(CAtumNode* pParent)
	:m_vCurrentCamPos(CAMERA_UP_POSITION), m_vTargetCamPos(CAMERA_DOWN_POSITION),
	m_vCurrentTargetPos(CAMERA_TARGET_POSITION)
{
	FLOG( "CINFSelectBack(CAtumNode* pParent)" );
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_pTopMesh = NULL;
	m_pMiddleMesh = NULL;
	m_pLocationMesh = NULL;
	m_pBottomMesh = NULL;

	m_fCurrentRotationAngle = 0;
	m_fTargetRotationAngle = 0;
	m_fUnitRotationStartTime = 0;
	m_nMode = CREATE_MODE;
	memset(m_CreateRenderInfo, 0x00, sizeof(GUIUnitRenderInfo)*4);
	memset(m_SelectRenderInfo, 0x00, sizeof(GUIUnitRenderInfo)*3);

	m_nRotateMode = ROTATE_NONE;
	m_nCurrentSelectUnit = 0;
	m_nOldSelectUnit = -1;
	m_fCurrentSelectUnitRotateAngle = 0;
	m_fOldSelectUnitRotateAngle = 0;

	m_bInitEffectItem = false;
	m_pBackEffect = NULL;

	// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
	for(int i=0; i<3; i++)
	{
		m_fTick[i] = NULL;
	}
	//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
	InitRenderMatrix();
}

CINFSelectBack::~CINFSelectBack()
{
	FLOG( "~CINFSelectBack()" );
	util::del(m_pTopMesh);
	util::del(m_pMiddleMesh);
	util::del(m_pLocationMesh);
	util::del(m_pBottomMesh);
}
/*
CCharacterInfo* CINFSelectBack::LoadCharacterEffect(char* strFileName)
{
	FLOG( "CINFSelectBack::LoadCharacterEffect(char* strFileName)" );
	CCharacterInfo* pChar;
	pChar = new CCharacterInfo();
	if(pChar->Load(strFileName))
	{
		pChar->InitDeviceObjects();
//		pChar->RestoreDeviceObjects();
	}
	else
	{
		util::del(pChar);
	}
	return pChar;

}

void CINFSelectBack::DeleteCharacterEffect(CCharacterInfo* pChar)
{
	FLOG( "CINFSelectBack::LoadCharacterEffect(char* strFileName)" );
	ASSERT_ASSERT(pChar);
	if( pChar->Load(strFileName) )
	{
		pChar->InvalidateDeviceObjects();
		pChar->DeleteDeviceObjects();
	}
	else
	{
		util::del(pChar);
	}
	return pChar;

}
*/

void CINFSelectBack::ChangeMode(int nMode)
{
	FLOG( "CINFSelectBack::ChangeMode(int nMode)" );
	m_nCurrentSelectUnit = 0;
	m_nMode = nMode;
	m_vCurrentCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
	m_vTargetCamPos = D3DXVECTOR3(CAMERA_DOWN_POSITION);
	m_vCurrentTargetPos = D3DXVECTOR3(CAMERA_TARGET_POSITION);
	m_fTargetRotationAngle = 0;
	m_fCurrentRotationAngle = 0;
	m_fCurrentSelectUnitRotateAngle = 0;
	m_fOldSelectUnitRotateAngle = 0;

	// 2008-07-10 by bhsohn 셀렉트 화면 기체 높이 갱신
	if(NULL == m_pParent)
	{
		return;
	}
	// end 2008-07-10 by bhsohn 셀렉트 화면 기체 높이 갱신

	float fDevide = m_nMode/2.0f;
	int i;
	for(i=0;i<SELECT_UNIT_NUMBER;i++)
	{
		//D3DXMatrixRotationZ( &m_mSelectTopMatrix[i],D3DX_PI/fDevide*i);
		//m_mSelectMiddleMatrix[i] = m_mSelectTopMatrix[i];
		D3DXMatrixRotationZ( &m_mSelectMiddleMatrix[i],D3DX_PI/fDevide*i);

		D3DXVECTOR3 vcurrentpos = D3DXVECTOR3(0,0,0);//UNIT_POSITION);
		D3DXVECTOR3 vtarget = D3DXVECTOR3(0,-1.0f,0);
		D3DXVECTOR3 vup = D3DXVECTOR3(0,0,1);
		D3DXMATRIX matTemp;
		D3DXMatrixLookAtRH( &matTemp, &vcurrentpos, &(vcurrentpos+vtarget), &vup);
		D3DXMatrixInverse( &m_CreateRenderInfo[i].matrix, NULL, &matTemp );

		D3DXMatrixTranslation( &matTemp, UNIT_POSITION);
		matTemp *= m_mSelectMiddleMatrix[i];
		m_CreateRenderInfo[i].matrix._41 = matTemp._41;
		m_CreateRenderInfo[i].matrix._42 = matTemp._42;
		m_CreateRenderInfo[i].matrix._43 = matTemp._43;
		//m_CreateRenderInfo[i].matrix *= m_mSelectTopMatrix[i];
		D3DXMatrixTranslation( &m_mSelectTopMatrix[i], 0.0f ,0.0f ,0.0f);
		m_mSelectTopMatrix[i]._41 = matTemp._41;//0.0        -36.411      13.802
		m_mSelectTopMatrix[i]._42 = matTemp._42;
		m_mSelectTopMatrix[i]._43 = matTemp._43-SELECT_TOP_HEIGHT;

		//m_CreateRenderInfo[i].rendertype = UNITKIND_BT01 << 4*i;
	}
	fDevide = 3.0f/2.0f;
	for(i=0;i<3;i++)
	{
		D3DXMATRIX matTemp, matTemp2;
		D3DXMatrixRotationZ( &matTemp,D3DX_PI/fDevide*i);

		D3DXVECTOR3 vcurrentpos = D3DXVECTOR3(UNIT_POSITION);
		D3DXVECTOR3 vtarget = D3DXVECTOR3(0,-1.0f,0);
		D3DXVECTOR3 vup = D3DXVECTOR3(0,0,1);
		D3DXMatrixLookAtRH( &m_SelectRenderInfo[i].matrix, &vcurrentpos, &(vcurrentpos+vtarget), &vup);
		D3DXMatrixInverse( &m_SelectRenderInfo[i].matrix, NULL, &m_SelectRenderInfo[i].matrix );

		D3DXMatrixTranslation( &matTemp2, UNIT_POSITION);
		matTemp2 *= matTemp;
		m_SelectRenderInfo[i].matrix._41 = matTemp2._41;
		m_SelectRenderInfo[i].matrix._42 = matTemp2._42;
		// 2008-07-08 by dgwoo 아머 키에 맞춰 출력.
		// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
		if(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center_ShapeItemNum)
			m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center_ShapeItemNum,TRUE);
		else
		m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center,TRUE);
		// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
		//m_SelectRenderInfo[i].matrix *= matTemp;

		//m_SelectRenderInfo[i].rendertype = UNITKIND_BT01 << 4*i;
	}
	D3DXMatrixTranslation( &m_mSelectLocationMatrix, 0.0f ,0.0f ,0.0f);
	D3DXMatrixTranslation( &m_mSelectBottomMatrix, 0.0f, 0.0f, 0.0f);
	if(m_nMode == CREATE_MODE)
	{
		((CINFSelect*)m_pParent)->ChangeCreateUnit(GetCurrentUnitKind());
		// 2009-02-10 by bhsohn Japan Charcter Create
		//memset(((CINFSelect*)m_pParent)->m_pCreateMenu->m_strCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
		ZERO_MEMORY(((CINFSelect*)m_pParent)->m_pCreateMenu->m_strCharacterName);
		// end 2009-02-10 by bhsohn Japan Charcter Create
		g_pD3dApp->CleanText();	
	}
/*		for(i=0;i<SELECT_UNIT_NUMBER;i++)
		{
			char buf[64];
			wsprintf( buf, "%08d", EFFECT_UNIT_EFFECT_INDEX(i));
			m_pCharacterInfo[i] = LoadCharacterEffect(buf)
		}
	}
	else if(m_nMode == SELECT_MODE)
	{
	}
	*/
}
HRESULT CINFSelectBack::InitDeviceObjects()
{
	FLOG( "CINFSelectBack::InitDeviceObjects()" );
	CGameData* pMeshData = new CGameData();

	char strPath[MAX_PATH];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_OBJECT, "select_t.obj");
	pMeshData->SetFile(strPath, FALSE, NULL, 0);
	m_pTopMesh = new CSkinnedMesh(FALSE);
	m_pTopMesh->InitDeviceObjects();
	m_pTopMesh->LoadMeshHierarchyFromMem( pMeshData );
	m_pTopMesh->m_nRenderCount = 2;
	util::del(pMeshData);

	pMeshData = new CGameData();
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_OBJECT, "select_m.obj");
	pMeshData->SetFile(strPath, FALSE, NULL, 0);
	m_pMiddleMesh = new CSkinnedMesh(FALSE);
	m_pMiddleMesh->InitDeviceObjects();
	m_pMiddleMesh->LoadMeshHierarchyFromMem( pMeshData );
	m_pMiddleMesh->m_nRenderCount = 2;
	util::del(pMeshData);

	pMeshData = new CGameData();
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_OBJECT, "select_l.obj");
	pMeshData->SetFile(strPath, FALSE, NULL, 0);
	m_pLocationMesh = new CSkinnedMesh(FALSE);
	m_pLocationMesh->InitDeviceObjects();
	m_pLocationMesh->LoadMeshHierarchyFromMem( pMeshData );
	m_pLocationMesh->m_nRenderCount = 2;
	util::del(pMeshData);

	pMeshData = new CGameData();
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_OBJECT, "select_b.obj");
	pMeshData->SetFile(strPath, FALSE, NULL, 0);
	m_pBottomMesh = new CSkinnedMesh(FALSE);
	m_pBottomMesh->InitDeviceObjects();
	m_pBottomMesh->LoadMeshHierarchyFromMem( pMeshData );
	m_pBottomMesh->m_nRenderCount = 2;
	util::del(pMeshData);

	float fDevide = m_nMode/2.0f;
	for(int i=0;i<SELECT_UNIT_NUMBER;i++)
	{
 		// 2005-07-21 by ispark
 		m_CreateRenderInfo[i].rendertype = GetUnitNum(2, 0, UNITKIND_BT01 << 4*i, FALSE);
	}
	D3DXMatrixTranslation( &m_mSelectLocationMatrix, 0.0f ,0.0f ,0.0f);
	D3DXMatrixTranslation( &m_mSelectBottomMatrix, 0.0f, 0.0f, 0.0f);

//	GUIUnitRenderInfo info;
//	info.rendertype = 0;
//	info.matrix = m_mSelectBottomMatrix;
//	m_pBackEffect = CUnitData::CreateWearItemEffectAtSelectMenu( &info, 
//																1100013,
//																BODYCON_FLY_MASK );
	return S_OK ;
}

HRESULT CINFSelectBack::RestoreDeviceObjects()
{
	FLOG( "CINFSelectBack::RestoreDeviceObjects()" );
	if(m_pTopMesh)
		m_pTopMesh->RestoreDeviceObjects();
	if(m_pMiddleMesh)
		m_pMiddleMesh->RestoreDeviceObjects();
	if(m_pLocationMesh)
		m_pLocationMesh->RestoreDeviceObjects();
	if(m_pBottomMesh)
		m_pBottomMesh->RestoreDeviceObjects();
	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFSelectBack::InvalidateDeviceObjects()
{
	FLOG( "CINFSelectBack::InvalidateDeviceObjects()" );
	if(m_pTopMesh)
		m_pTopMesh->InvalidateDeviceObjects();
	if(m_pMiddleMesh)
		m_pMiddleMesh->InvalidateDeviceObjects();
	if(m_pLocationMesh)
		m_pLocationMesh->InvalidateDeviceObjects();
	if(m_pBottomMesh)
		m_pBottomMesh->InvalidateDeviceObjects();
	m_bRestored = FALSE;
	return S_OK ;
}

HRESULT CINFSelectBack::DeleteDeviceObjects()
{
	FLOG( "CINFSelectBack::DeleteDeviceObjects()" );
	if(m_pTopMesh)
		m_pTopMesh->DeleteDeviceObjects();
	util::del(m_pTopMesh);
	if(m_pMiddleMesh)
		m_pMiddleMesh->DeleteDeviceObjects();
	util::del(m_pMiddleMesh);
	if(m_pLocationMesh)
		m_pLocationMesh->DeleteDeviceObjects();
	util::del(m_pLocationMesh);
	if(m_pBottomMesh)
		m_pBottomMesh->DeleteDeviceObjects();
	util::del(m_pBottomMesh);
	return S_OK ;
}

void CINFSelectBack::Tick()
{
	FLOG( "CINFSelectBack::Tick()" );
	if(VEL_CAMERA_MOVE>50)
		return;
	if( D3DXVec3Length(&(m_vCurrentCamPos - m_vTargetCamPos))>VEL_CAMERA_MOVE )
	{
		D3DXVECTOR3 v;
		D3DXVec3Normalize(&v,&(m_vTargetCamPos - m_vCurrentCamPos));
		m_vCurrentCamPos += v*(VEL_CAMERA_MOVE);
	}
	else
	{
		m_vCurrentCamPos = m_vTargetCamPos;
/*		if(m_vCurrentCamPos == D3DXVECTOR3(CAMERA_UP_POSITION))
		{
			m_vTargetCamPos = D3DXVECTOR3(CAMERA_DOWN_POSITION);
		}
*/	}
	if(m_nRotateMode != ROTATE_NONE)
	{
		float fOldUnitRotationStartTime = m_fUnitRotationStartTime;
		m_fUnitRotationStartTime -= g_pD3dApp->GetElapsedTime();
		if( m_fUnitRotationStartTime < 0 &&
			fOldUnitRotationStartTime >= 0)
		{
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_ROTATE, D3DXVECTOR3(0,0,0), FALSE);
		}
		if(m_fUnitRotationStartTime < 0)
		{
			D3DXMATRIX	matRotate, matTemp;
			float fRotateVel;
			if(m_nMode == SELECT_MODE)
				fRotateVel = VEL_SELECT_UNIT_ROTATE_MOVE;
			else if(m_nMode == CREATE_MODE)
				fRotateVel = VEL_CREATE_UNIT_ROTATE_MOVE;
			// 오른쪽 회전
			if(m_nRotateMode == ROTATE_RIGHT)
			{
				if(m_fTargetRotationAngle - m_fCurrentRotationAngle > 0 )
				{
					m_fCurrentRotationAngle += fRotateVel;
					if(m_fTargetRotationAngle- m_fCurrentRotationAngle < 0)
						m_fCurrentRotationAngle = m_fTargetRotationAngle;

				}
				else
				{
					// 회전 종료
					m_fCurrentRotationAngle = m_fTargetRotationAngle;
					m_nRotateMode = ROTATE_NONE;
					if(m_vCurrentCamPos == D3DXVECTOR3(CAMERA_UP_POSITION))
					{
						m_vTargetCamPos = D3DXVECTOR3(CAMERA_DOWN_POSITION);
					}
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_UNIT_STOP, D3DXVECTOR3(0,0,0), FALSE);
				}
			}
			// 왼쪽 회전
			if(m_nRotateMode == ROTATE_LEFT)
			{
				if(m_fTargetRotationAngle - m_fCurrentRotationAngle < 0 )
				{
					m_fCurrentRotationAngle -= fRotateVel;
					if(m_fTargetRotationAngle - m_fCurrentRotationAngle > 0)
						m_fCurrentRotationAngle = m_fTargetRotationAngle;
				}
				else
				{
					// 회전 종료
					m_fCurrentRotationAngle = m_fTargetRotationAngle;
					m_nRotateMode = ROTATE_NONE;
					if(m_vCurrentCamPos == D3DXVECTOR3(CAMERA_UP_POSITION))
					{
						m_vTargetCamPos = D3DXVECTOR3(CAMERA_DOWN_POSITION);
					}
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_UNIT_STOP, D3DXVECTOR3(0,0,0), FALSE);
				}
			}
			D3DXMatrixRotationZ( &matRotate,m_fCurrentRotationAngle);
			m_mSelectLocationMatrix = matRotate;
			float fDevide = m_nMode/2.0f;
			for(int i=0;i<m_nMode;i++)
			{
				D3DXMatrixRotationZ( &matTemp,D3DX_PI/fDevide*i);
				//m_mSelectTopMatrix[i] = matRotate * matTemp;
				//m_mSelectMiddleMatrix[i] = m_mSelectTopMatrix[i];
				m_mSelectMiddleMatrix[i] = matRotate * matTemp;


				if(m_nMode == CREATE_MODE)
				{
					D3DXVECTOR3 vcurrentpos = D3DXVECTOR3(UNIT_POSITION);
					D3DXVECTOR3 vtarget = D3DXVECTOR3(0,-1.0f,0);
					D3DXVECTOR3 vup = D3DXVECTOR3(0,0,1);
					D3DXMATRIX matTemp, matTemp2, matCreateRenderInfo;
					D3DXMatrixLookAtRH( &matTemp, &vcurrentpos, &(vcurrentpos+vtarget), &vup);
					if(m_nOldSelectUnit != i)
						D3DXMatrixInverse( &m_CreateRenderInfo[i].matrix, NULL, &matTemp );

					D3DXMatrixTranslation( &matTemp2, UNIT_POSITION);
					matTemp2 *= m_mSelectMiddleMatrix[i];
					m_CreateRenderInfo[i].matrix._41 = matTemp2._41;
					m_CreateRenderInfo[i].matrix._42 = matTemp2._42;
					m_CreateRenderInfo[i].matrix._43 =  matTemp2._43;
					D3DXMatrixTranslation( &m_mSelectTopMatrix[i], 0.0f ,0.0f ,0.0f);
					m_mSelectTopMatrix[i]._41 = matTemp2._41;
					m_mSelectTopMatrix[i]._42 = matTemp2._42;
					m_mSelectTopMatrix[i]._43 = matTemp2._43-SELECT_TOP_HEIGHT;
				}
				if(m_nMode == SELECT_MODE)
				{
					D3DXVECTOR3 vcurrentpos = D3DXVECTOR3(UNIT_POSITION);
					D3DXVECTOR3 vtarget = D3DXVECTOR3(0,-1.0f,0);
					D3DXVECTOR3 vup = D3DXVECTOR3(0,0,1);
					D3DXMATRIX matTemp, matTemp2;
					D3DXMatrixLookAtRH( &matTemp, &vcurrentpos, &(vcurrentpos+vtarget), &vup );
					if(m_nOldSelectUnit != i)
						D3DXMatrixInverse( &m_SelectRenderInfo[i].matrix, NULL, &matTemp );

					D3DXMatrixTranslation( &matTemp2, UNIT_POSITION);
					matTemp2 *= m_mSelectMiddleMatrix[i];
					m_SelectRenderInfo[i].matrix._41 = matTemp2._41;
					m_SelectRenderInfo[i].matrix._42 = matTemp2._42;
					// 2008-07-08 by dgwoo 아머 키에 맞춰 출력.
					//m_SelectRenderInfo[i].matrix._43 = matTemp2._43;
					// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
					if(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center_ShapeItemNum)
						m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center_ShapeItemNum,TRUE);
					else
					m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center,TRUE);
					// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
					D3DXMatrixTranslation( &m_mSelectTopMatrix[i], 0.0f ,0.0f ,0.0f);
					m_mSelectTopMatrix[i]._41 = matTemp2._41;
					m_mSelectTopMatrix[i]._42 = matTemp2._42;
					m_mSelectTopMatrix[i]._43 = matTemp2._43-SELECT_TOP_HEIGHT;
				}
			}
		}
	}
	else
	{
		if(m_nMode == CREATE_MODE)
		{
			D3DXMATRIX matRotate;
			m_fCurrentSelectUnitRotateAngle += g_pD3dApp->GetElapsedTime();
			D3DXMatrixRotationY( &matRotate,g_pD3dApp->GetElapsedTime());
			m_CreateRenderInfo[m_nCurrentSelectUnit].matrix = matRotate * m_CreateRenderInfo[m_nCurrentSelectUnit].matrix;
			D3DXMatrixRotationZ( &matRotate,g_pD3dApp->GetElapsedTime());
			m_mSelectTopMatrix[m_nCurrentSelectUnit] = matRotate * m_mSelectTopMatrix[m_nCurrentSelectUnit];
		}
		if(m_nMode == SELECT_MODE)
		{
			D3DXMATRIX matRotate;
			m_fCurrentSelectUnitRotateAngle += g_pD3dApp->GetElapsedTime();
			D3DXMatrixRotationY( &matRotate,g_pD3dApp->GetElapsedTime());
			m_SelectRenderInfo[m_nCurrentSelectUnit].matrix = matRotate * m_SelectRenderInfo[m_nCurrentSelectUnit].matrix;
			D3DXMatrixRotationZ( &matRotate,g_pD3dApp->GetElapsedTime());
			m_mSelectTopMatrix[m_nCurrentSelectUnit] = matRotate * m_mSelectTopMatrix[m_nCurrentSelectUnit];
		}
	
	}
	if(	m_nOldSelectUnit != -1 )
	{
		D3DXMATRIX matRotate2,matRotate3;
		m_fOldSelectUnitRotateAngle -= g_pD3dApp->GetElapsedTime()*3;
		D3DXMatrixRotationY( &matRotate2,-g_pD3dApp->GetElapsedTime()*3);
		D3DXMatrixRotationZ( &matRotate3,-g_pD3dApp->GetElapsedTime()*3);
		if(m_nMode == CREATE_MODE)
		{
			m_CreateRenderInfo[m_nOldSelectUnit].matrix = matRotate2 * m_CreateRenderInfo[m_nOldSelectUnit].matrix;
			m_mSelectTopMatrix[m_nOldSelectUnit] = matRotate3 * m_mSelectTopMatrix[m_nOldSelectUnit];
		}
		if(m_nMode == SELECT_MODE)
		{
			m_SelectRenderInfo[m_nOldSelectUnit].matrix = matRotate2 * m_SelectRenderInfo[m_nOldSelectUnit].matrix;
			m_mSelectTopMatrix[m_nOldSelectUnit] = matRotate3 * m_mSelectTopMatrix[m_nOldSelectUnit];
		}
		if(m_fOldSelectUnitRotateAngle <0)
		{
			m_nOldSelectUnit = -1;
			m_fOldSelectUnitRotateAngle = 0;
		}
	}

//	D3DXVECTOR3 vUp;
//	D3DXVec3Cross(&vUp, &D3DXVECTOR3(, &m_vCurrentTargetPos );

	g_pD3dApp->m_pCamera->SetViewParams( m_vCurrentCamPos, m_vCurrentTargetPos, D3DXVECTOR3{ 0, 0, 1 });
	g_pD3dApp->m_pCamera->SetProjParams( D3DX_PI/4, 1.333f, 1.0f, 100000.0f );
}

void CINFSelectBack::Render()
{
	FLOG( "CINFSelectBack::Render()" );
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING,	TRUE );
	g_pD3dDev->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

	for(int i=0;i<m_nMode;i++)
	{
		if(m_pTopMesh)
		{
			m_pTopMesh->Tick(0);
			m_pTopMesh->SetWorldMatrix(m_mSelectTopMatrix[i]);
			m_pTopMesh->AnotherTexture(1);
			m_pTopMesh->Render();
		}
		if(m_pMiddleMesh)
		{
			m_pMiddleMesh->Tick(0);
			m_pMiddleMesh->SetWorldMatrix(m_mSelectMiddleMatrix[i]);
			m_pMiddleMesh->AnotherTexture(1);
			m_pMiddleMesh->Render();
		}
	}
	if(m_pLocationMesh)
	{
		m_pLocationMesh->Tick(0);
		m_pLocationMesh->SetWorldMatrix(m_mSelectLocationMatrix);
		m_pLocationMesh->AnotherTexture(1);
		m_pLocationMesh->Render();
	}
	if(m_pBottomMesh)
	{
		m_pBottomMesh->Tick(0);
		m_pBottomMesh->SetWorldMatrix(m_mSelectBottomMatrix);
		m_pBottomMesh->AnotherTexture(1);
		m_pBottomMesh->Render();
	}
}



#define SELECT_MENU_ITEM_SHIFT(x)		(WEAPON_BODYCON_LOADING << (x*10))//10 = 기어가 증가하면서 bodycondition이 증가하는 값
void CINFSelectBack::RenderCreateUnit()
{
	FLOG( "CINFSelectBack::RenderCreateUnit()" );
	if(!m_bInitEffectItem)
	{
		GUIUnitRenderInfo info;
		info.rendertype = 0;
		info.matrix = m_mSelectBottomMatrix;
		m_pBackEffect = CUnitData::CreateWearItemEffectAtSelectMenu( &info, RC_EFF_SELECT, BODYCON_FLY_MASK );
		for(int i=0;i<SELECT_UNIT_NUMBER;i++)
		{
			// 생성화면에서 기본 무기 장작된 것들과, 기본 엔진을 보여주기 위함
			int nItemNum1, nItemNum2;
			USHORT nUnitKind = 0;
			if( i==0 )
				nUnitKind = UNITKIND_BT01;
			else if(i==1)
				nUnitKind = UNITKIND_OT01;
			else if(i==2)
				nUnitKind = UNITKIND_DT01;
			else if(i==3)
				nUnitKind = UNITKIND_ST01;

			GUIUnitRenderInfo * pInfo = &m_CreateRenderInfo[i];
			nItemNum1 = GetCreateUnitWeapon11(nUnitKind);
			nItemNum2 = GetCreateUnitEngine(nUnitKind);
			CAppEffectData	*pCreateEffect;

			pCreateEffect = CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, UNIT_EFFECT_INDEX(2,i,1), BODYCON_LANDED_MASK);
			m_CreateOrgRWeaponInfo[i].matrix = pCreateEffect->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_RIGHT_POSITION);
			m_CreateOrgLWeaponInfo[i].matrix = pCreateEffect->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_LEFT_POSITION);
			CUnitData::CreateWearItemEffectAtSelectMenu( &m_CreateRenderRWeaponInfo[i],nItemNum1+EFFECT_WEAR_WEAPON_1,SELECT_MENU_ITEM_SHIFT(i) );
			CUnitData::CreateWearItemEffectAtSelectMenu( &m_CreateRenderLWeaponInfo[i],nItemNum1+1+EFFECT_WEAR_WEAPON_1,SELECT_MENU_ITEM_SHIFT(i) );
			CUnitData::CreateWearItemEffectAtSelectMenu( pInfo,	nItemNum2, BODYCON_LANDED_MASK );//착륙 바디컨디션
		}
		m_bInitEffectItem = true;
	}

	for(int i=0;i<CREATE_MODE;i++)
	{	
		g_pD3dApp->m_pUnitRender->m_nSelectStateUnitColor = 1;
		g_pD3dApp->m_pUnitRender->Render(m_CreateRenderInfo[i]);

		// 2006-01-02 by ispark
		m_CreateRenderRWeaponInfo[i].matrix = m_CreateOrgRWeaponInfo[i].matrix * m_CreateRenderInfo[i].matrix;
		m_CreateRenderLWeaponInfo[i].matrix = m_CreateOrgLWeaponInfo[i].matrix * m_CreateRenderInfo[i].matrix;
	}

}

void CINFSelectBack::RenderSelectUnit()
{
	FLOG( "CINFSelectBack::RenderSelectUnit()" );
	if(!m_bInitEffectItem)
	{
		CAppEffectData	*pCreateEffect = NULL;
		// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
		GUIUnitRenderInfo info;
		info.rendertype = 0;
		info.matrix = m_mSelectBottomMatrix;
		m_pBackEffect = CUnitData::CreateWearItemEffectAtSelectMenu( &info, 
																RC_EFF_SELECT,
																BODYCON_FLY_MASK );
		//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
		DBGOUT_EFFECT("------------------- Select Window Setting -------------------\n");
		for(int i=0;i<SELECT_MODE;i++)
		{
			if(m_SelectRenderInfo[i].rendertype != 0)
			{
				DBGOUT_EFFECT(" Unit type : %08d\n",m_SelectRenderInfo[i].rendertype);
				CHARACTER_RENDER_INFO info = ((CINFSelect *)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo;
				USHORT nUnitKind = ((CINFSelect *)m_pParent)->m_guiUnitInfo[i].UnitKind;
				GUIUnitRenderInfo * pInfo = &m_SelectRenderInfo[i];
				// 2006-01-02 by ispark, 아머 먼저 로딩을 해야함
				if(info.RI_Center)
				{
					DBGOUT_EFFECT(" Center (Armor)[effect:%d][bodycon:%I64X]\n",info.RI_Center,WEAPON_BODYCON_LOADING);

					// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
					//ITEM * pItem = g_pDatabase->GetServerItemInfo(info.RI_Center);

					int nItemNum = 0;
					if( info.RI_Center_ShapeItemNum )
						nItemNum = info.RI_Center_ShapeItemNum;
					else
						nItemNum = info.RI_Center;

					ITEM* pItem = g_pDatabase->GetServerItemInfo( nItemNum );
					// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

					if(pItem)
					{						
						// 2007-07-30 by dgwoo 저 사양모드에서 이펙트가 제대로 그려지지 않는다.
						if(g_pD3dApp->m_bDegree == 0)
							g_pD3dApp->m_bDegree = 1;

						// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
						//	pCreateEffect = CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
						//											::GetUnitNum(g_pD3dApp->m_bDegree, pItem->SourceIndex, 
						//											nUnitKind, FALSE), 
		//				//											SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
						//											BODYCON_BOOSTER3_MASK);
						if(COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ))
						{
							pCreateEffect = CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
																	::GetUnitNum(g_pD3dApp->m_bDegree, pItem->SourceIndex, 
																	nUnitKind, FALSE), 
		//															SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
																	BODYCON_BOOSTER1_MASK);
							pCreateEffect->m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_BOOSTER1_MASK);
							pCreateEffect->m_pCharacterInfo->SetBodyCondition(BODYCON_BOOSTER1_MASK);
							pCreateEffect->m_pCharacterInfo->Tick(0.0f);
 							m_fTick[i] = pCreateEffect->m_pCharacterInfo->GetCurrentCharacterAnimationTime();
						}
						else
						{
							pCreateEffect = CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
																	::GetUnitNum(g_pD3dApp->m_bDegree, pItem->SourceIndex, 
																	nUnitKind, FALSE), 
		//															SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
																	BODYCON_LANDED_MASK);
							pCreateEffect->m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_LANDED_MASK);
							pCreateEffect->m_pCharacterInfo->SetBodyCondition(BODYCON_LANDED_MASK);
							pCreateEffect->m_pCharacterInfo->Tick(0.0f);
							m_fTick[i] = pCreateEffect->m_pCharacterInfo->GetCurrentCharacterAnimationTime();
						}
						//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
					}
				}
				else
				{
					// 2006-01-24 by ispark, info.RI_Center가 없을 때(아머가 장착되어 있지 않음) 기본유닛으로 셋팅
					pCreateEffect = CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, m_SelectRenderInfo[i].rendertype, BODYCON_LANDED_MASK);
				}	

				if(pCreateEffect)
				{
					m_SelectOrgRWeaponInfo[i][0].matrix = pCreateEffect->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_RIGHT_POSITION);
					m_SelectOrgLWeaponInfo[i][0].matrix = pCreateEffect->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_LEFT_POSITION);
					m_SelectOrgRWeaponInfo[i][1].matrix = pCreateEffect->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_RIGHT_POSITION);
					m_SelectOrgLWeaponInfo[i][1].matrix = pCreateEffect->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_LEFT_POSITION);
				}

				// 1형
				if(info.RI_ProwOut)
				{
					ITEM * item = g_pDatabase->GetServerItemInfo(info.RI_ProwOut);

					// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
					ITEM*	pShapeItem = g_pDatabase->GetServerItemInfo( info.RI_ProwOut_ShapeItemNum );
					// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

					if(item)
					{

						// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

// 						if(IS_PRIMARY_WEAPON_1(item->Kind))
// 						{
// 							DBGOUT_EFFECT(" 1-1 Type Weapon [effect:%d][bodycon:%I64X]\n",
// 								info.RI_ProwOut+EFFECT_WEAR_WEAPON_1,SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 
// 							CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderRWeaponInfo[i][0], 
// 									item->SourceIndex + EFFECT_WEAR_WEAPON_1,
// 									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 							if(nUnitKind != UNITKIND_DT01)
// 							{
// 								CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderLWeaponInfo[i][0], 
// 										item->SourceIndex+1+EFFECT_WEAR_WEAPON_1,
// 										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 							}
// 						}
// 						else if(IS_PRIMARY_WEAPON_2(item->Kind))
// 						{
// 							DBGOUT_EFFECT(" 1-2 Type Weapon [effect:%d][bodycon:%I64X]\n",
// 								info.RI_ProwOut+EFFECT_WEAR_WEAPON_1,SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
// 							CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
// 									item->SourceIndex + EFFECT_WEAR_WEAPON_1,
// 									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 						}

						// 선택창에서의 무기 렌더링 처리
						// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
						ITEM* pArmor = NULL;
						if(info.RI_Center_ShapeItemNum)
						{
							pArmor = g_pDatabase->GetServerItemInfo( info.RI_Center_ShapeItemNum );
						}
						else
						{
							pArmor = g_pDatabase->GetServerItemInfo( info.RI_Center );
						}
						// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정

						if( !pArmor )
						{
							// 아머를 착용하지 않았다면 기존과 같이 무기 이펙트 추가
							if(IS_PRIMARY_WEAPON_1(item->Kind))
							{
								// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
								if( pShapeItem )
									item = pShapeItem;
								// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
								
								
								CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderRWeaponInfo[i][0], 
									item->SourceIndex + EFFECT_WEAR_WEAPON_1,
									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
								if(nUnitKind != UNITKIND_DT01)
								{
									CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderLWeaponInfo[i][0], 
										item->SourceIndex+1+EFFECT_WEAR_WEAPON_1,
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
								}
							}
						}
						else
						{
							// 아머를 착용했는데 로봇아머일 경우는 무기 이펙트 렌더링 추가가 안됨							
							if( COMPARE_BIT_FLAG( pArmor->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) )
							{
								if(IS_PRIMARY_WEAPON_1(item->Kind) && nUnitKind == UNITKIND_DT01 )
								{
									ITEM* pAGearWeapon = g_pDatabase->GetServerItemInfo( A_GEAR_ROBOT_PRIMARY_WEAPON_ITEM_NUM );
									if( pAGearWeapon )
									{
										CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderRWeaponInfo[i][0], 
																					 EFFECT_NUMBER_WEAR_WEAPON_11( pAGearWeapon, GetUnitTypeDecimal(nUnitKind) ),
																					 SELECT_MENU_ITEM_SHIFT( GetUnitTypeDecimal( nUnitKind ) ) );
									}
								}
							}
							else
							{
								// 아머를 착용했는데 로봇아머가 아닐경우 기존과 같이 무기 이펙트 추가
								if(IS_PRIMARY_WEAPON_1(item->Kind))
								{
									// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
									if( pShapeItem )
										item = pShapeItem;
									// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
									
									CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderRWeaponInfo[i][0], 
										item->SourceIndex + EFFECT_WEAR_WEAPON_1,
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
									if(nUnitKind != UNITKIND_DT01)
									{
										CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderLWeaponInfo[i][0], 
											item->SourceIndex+1+EFFECT_WEAR_WEAPON_1,
											SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
									}
								}
							}
						}
						// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
						
					}
				}
				else
				{
				}
				// 2형
				if(info.RI_WingOut)
				{
					ITEM * item = g_pDatabase->GetServerItemInfo(info.RI_WingOut);

					// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
					ITEM*	pShapeItem	= g_pDatabase->GetServerItemInfo( info.RI_WingOut_ShapeItemNum );
					// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

					if(item)
					{
						
						// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

// 						if(IS_SECONDARY_WEAPON_1(item->Kind))
// 						{
// 							DBGOUT_EFFECT(" 2-1 Type Weapon [effect:%d][bodycon:%I64X]\n",
// 								info.RI_WingOut-EFFECT_WEAR_WEAPON_2,SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
// //							CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
// //									item->SourceIndex - EFFECT_WEAR_WEAPON_2,
// //									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 							CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderRWeaponInfo[i][1], 
// 									item->SourceIndex - EFFECT_WEAR_WEAPON_2,
// 									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 							CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderLWeaponInfo[i][1], 
// 									item->SourceIndex + 1 - EFFECT_WEAR_WEAPON_2,
// 									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 
// 						}
// 						else if(IS_SECONDARY_WEAPON_2(item->Kind))
// 						{
// 							DBGOUT_EFFECT(" 2-2 Type Weapon [effect:%d][bodycon:%I64X]\n",
// 								info.RI_WingOut-EFFECT_WEAR_WEAPON_2,SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
// 							CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
// 									item->SourceIndex - EFFECT_WEAR_WEAPON_2,
// 									SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
// 
// 						}

						// 선택창에서의 무기 렌더링 처리
						// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
						//ITEM* pArmor = g_pDatabase->GetServerItemInfo( info.RI_Center );
						ITEM* pArmor = NULL;
						if(info.RI_Center_ShapeItemNum)
						{
							pArmor = g_pDatabase->GetServerItemInfo( info.RI_Center_ShapeItemNum );
						}
						else
						{
							pArmor = g_pDatabase->GetServerItemInfo( info.RI_Center );
						}
						// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
						if( !pArmor
							|| !COMPARE_BIT_FLAG( pArmor->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) )
						{
							// 아머를 착용하고 있지 않거나,
							// 아머를 착용했는데 로봇아머가 아닐경우 기존과 같이 무기 이펙트 추가
							// (아머를 착용했는데 로봇아머일 경우는 무기 이펙트 렌더링 추가가 안됨)

							if(IS_SECONDARY_WEAPON_1(item->Kind))
							{
								// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
								if( pShapeItem )
									item = pShapeItem;
								// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
								
								CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderRWeaponInfo[i][1], 
										item->SourceIndex - EFFECT_WEAR_WEAPON_2,
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
								CUnitData::CreateWearItemEffectAtSelectMenu( &m_SelectRenderLWeaponInfo[i][1], 
										item->SourceIndex + 1 - EFFECT_WEAR_WEAPON_2,
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );

							}
							else if(IS_SECONDARY_WEAPON_2(item->Kind))
							{
								// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
								if( pShapeItem )
									item = pShapeItem;
								// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
								
								CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
										item->SourceIndex - EFFECT_WEAR_WEAPON_2,
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );

							}
						}

						// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대

					}
				}
				else
				{
				}
				// 엔진
				// 2006-01-23 by ispark, 이펙트 없다.
				if(info.RI_Rear)
				{
					DBGOUT_EFFECT(" Back (Engine) [effect:%d][bodycon:%I64X]\n",info.RI_Rear,WEAPON_BODYCON_LOADING);
//					ITEM * pItem = g_pDatabase->GetServerItemInfo(info.RI_Rear);
//					if(pItem)
//					{
//						CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
//										pItem->SourceIndex,
//										BODYCON_LANDED_MASK );//착륙 바디컨디션
//					}
				}
				// 선두
				if(info.RI_Prow)
				{
					DBGOUT_EFFECT(" Front (Rader)[effect:%d][bodycon:%I64X]\n",info.RI_Prow,WEAPON_BODYCON_LOADING);
					// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
					//ITEM * pItem = g_pDatabase->GetServerItemInfo(info.RI_Center);
					ITEM * pItem = NULL;
					if(info.RI_Center_ShapeItemNum)
						pItem = g_pDatabase->GetServerItemInfo(info.RI_Center_ShapeItemNum);
					else
						pItem = g_pDatabase->GetServerItemInfo(info.RI_Center);
					// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
					if(pItem)
					{
						CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
										EFFECT_NUMBER_WEAR_RADAR(pItem->SourceIndex), 
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
					}
				}
				if(info.RI_AccessoryUnLimited)
				{
					DBGOUT_EFFECT(" Option (ATTACH)[effect:%d][bodycon:%I64X]\n",info.RI_AccessoryUnLimited,WEAPON_BODYCON_LOADING);
					// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
					//ITEM * pItem = g_pDatabase->GetServerItemInfo(info.RI_Center);
					ITEM * pItem = NULL;
					if(info.RI_Center_ShapeItemNum)
						pItem = g_pDatabase->GetServerItemInfo(info.RI_Center_ShapeItemNum);
					else
						pItem = g_pDatabase->GetServerItemInfo(info.RI_Center);
					// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정

					if(pItem)
					{
//						CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
//										info.RI_Attachment, 
//										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
						CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
										pItem->SourceIndex, 
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)));
					}
				}
				if ( info.RI_Pet )
				{
					DBGOUT_EFFECT(" Option (ATTACH)[effect:%d][bodycon:%I64X]\n",info.RI_Pet,WEAPON_BODYCON_LOADING);
					ITEM * pItem = g_pDatabase->GetServerItemInfo ( info.RI_Pet );
					if ( pItem )
					{
						CUnitData::CreateWearItemEffectAtSelectMenu ( pInfo , 
										info.RI_Pet_ShapeItemNum,
										SELECT_MENU_ITEM_SHIFT(GetUnitTypeDecimal(nUnitKind)) );
					}

				}
				DBGOUT_EFFECT(" Landing [effect:%d][bodycon:%I64X]\n",
					UNIT_EFFECT_INDEX(2/*g_pD3dApp->m_bDegree*/,GetUnitTypeDecimal(nUnitKind),GGetUnitUpgradeLevel(nUnitKind)), 
					BODYCON_LANDED_MASK);
//				CUnitData::CreateWearItemEffectAtSelectMenu( pInfo, 
//					UNIT_EFFECT_INDEX(2/*g_pD3dApp->m_bDegree*/,GetUnitTypeDecimal(nUnitKind),GGetUnitUpgradeLevel(nUnitKind)), 
//					BODYCON_LANDED_MASK);
			}
		}
		m_bInitEffectItem = true;
	}
	for(int i=0;i<SELECT_MODE;i++)
	{
		if(m_SelectRenderInfo[i].rendertype != 0)
		{
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

			//g_pD3dApp->m_pUnitRender->m_nSelectStateUnitColor = max(1, g_pSelect->m_guiUnitInfo[i].CharacterRenderInfo.RI_ArmorColorCode%100);
			g_pD3dApp->m_pUnitRender->m_nSelectStateUnitColor = 1;

			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			
			
			// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
			//g_pD3dApp->m_pUnitRender->Render(m_SelectRenderInfo[i]);
			// 선택창에서의 무기 렌더링 처리
			CHARACTER_RENDER_INFO info = ((CINFSelect *)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo;
			if( !info.RI_Center )
				g_pD3dApp->m_pUnitRender->Render( m_SelectRenderInfo[i] );
			else
			{
				// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
				//ITEM* pItem = g_pDatabase->GetServerItemInfo( info.RI_Center );
				ITEM* pItem = NULL;
				if(info.RI_Center_ShapeItemNum)
				{
					pItem = g_pDatabase->GetServerItemInfo( info.RI_Center_ShapeItemNum );
				}
				else
				{
					pItem = g_pDatabase->GetServerItemInfo( info.RI_Center );
				}
				// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정

				if( !pItem || !COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) )
					// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
					//g_pD3dApp->m_pUnitRender->Render( m_SelectRenderInfo[i] );
					g_pD3dApp->m_pUnitRender->Render( m_SelectRenderInfo[i], m_fTick[i] );
					//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
				else
					// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
					//g_pD3dApp->m_pUnitRender->Render( m_SelectRenderInfo[i], TRUE );
					g_pD3dApp->m_pUnitRender->Render( m_SelectRenderInfo[i], m_fTick[i], TRUE );
					//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
			}
			// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

			// 2006-01-02 by ispark
			m_SelectRenderRWeaponInfo[i][0].matrix = m_SelectOrgRWeaponInfo[i][0].matrix * m_SelectRenderInfo[i].matrix;
			m_SelectRenderLWeaponInfo[i][0].matrix = m_SelectOrgLWeaponInfo[i][0].matrix * m_SelectRenderInfo[i].matrix;
			m_SelectRenderRWeaponInfo[i][1].matrix = m_SelectOrgRWeaponInfo[i][1].matrix * m_SelectRenderInfo[i].matrix;
			m_SelectRenderLWeaponInfo[i][1].matrix = m_SelectOrgLWeaponInfo[i][1].matrix * m_SelectRenderInfo[i].matrix;
		}
	}
}

int CINFSelectBack::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSelectBack::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_LEFT:
				{						
					if(m_nRotateMode == ROTATE_NONE)
					{
						// 2005-03-09 by jschoi
						// 2005-07-04 by ispark 자동 스탯 분배 삭제
//						g_pSelect->m_pCreateMenu->m_nCreateUnitStat = -1;
//						g_pSelect->m_pCreateMenu->m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//						g_pSelect->m_pCreateMenu->m_nRenderToolTipIndex = -1;
						g_pSelect->m_pCreateMenu->m_nFocusStat = -1;
						g_pSelect->m_pCreateMenu->m_nFocus = -1;
						
						int nTempSelectUnit = m_nCurrentSelectUnit;
						if( ++nTempSelectUnit == m_nMode )
						{
							nTempSelectUnit = 0;
						}
						if(	m_nMode == SELECT_MODE &&
							((CINFSelect*)m_pParent)->m_guiUnitInfo[nTempSelectUnit].CharacterUniqueNumber == 0)
						{
							break;
						}

						m_fTargetRotationAngle -= D3DX_PI*2.0f/m_nMode;
						m_vTargetCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
						m_nRotateMode = ROTATE_LEFT;
						m_fUnitRotationStartTime = UNIT_ROTATE_START_TIME;
						m_nOldSelectUnit = m_nCurrentSelectUnit;
						int fTemp = (int)(m_fCurrentSelectUnitRotateAngle/(D3DX_PI*2.0f));
						m_fOldSelectUnitRotateAngle = m_fCurrentSelectUnitRotateAngle - fTemp*D3DX_PI*2.0f;
						m_fCurrentSelectUnitRotateAngle = 0;
						if(++m_nCurrentSelectUnit == m_nMode)
						{
							m_nCurrentSelectUnit = 0;
						}
						if(m_nMode == CREATE_MODE)
						{
							((CINFSelect*)m_pParent)->ChangeCreateUnit(GetCurrentUnitKind());
							g_pSelect->m_pUnitCreateInfo->StartView();
						}
						else
						{
							if(((CINFSelect*)m_pParent)->m_character[m_nCurrentSelectUnit].CharacterUniqueNumber == 0)
							{
								MSG_FC_CHARACTER_GET_CHARACTER sMsg;
								memset(&sMsg,0x00,sizeof(sMsg));
								char buffer[SIZE_MAX_PACKET];
								sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
								sMsg.CharacterUniqueNumber = ((CINFSelect*)m_pParent)->m_guiUnitInfo[m_nCurrentSelectUnit].CharacterUniqueNumber;
								int nType = T_FC_CHARACTER_GET_CHARACTER;
								memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
								memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
								g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

								g_pD3dApp->m_bRequestEnable = FALSE;
							}					
						}
						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_UNIT_CLICK, D3DXVECTOR3(0,0,0), FALSE);
					}
				}
				break;
			case VK_RIGHT:
				{
					if(m_nRotateMode == ROTATE_NONE)
					{
						// 2005-03-09 by jschoi
						// 2005-07-04 by ispark 자동 스탯 분배 삭제
//						g_pSelect->m_pCreateMenu->m_nCreateUnitStat = -1;
//						g_pSelect->m_pCreateMenu->m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//						g_pSelect->m_pCreateMenu->m_nRenderToolTipIndex = -1;
						g_pSelect->m_pCreateMenu->m_nFocusStat = -1;
						g_pSelect->m_pCreateMenu->m_nFocus = -1;

						int nTempSelectUnit = m_nCurrentSelectUnit;
						if( --nTempSelectUnit == -1 )
						{
							nTempSelectUnit = m_nMode-1;
						}
						if(	m_nMode == SELECT_MODE &&
							((CINFSelect*)m_pParent)->m_guiUnitInfo[nTempSelectUnit].CharacterUniqueNumber == 0)
						{
							break;
						}

						m_fTargetRotationAngle += D3DX_PI*2.0f/m_nMode;
						m_vTargetCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
						m_nRotateMode = ROTATE_RIGHT;
						m_fUnitRotationStartTime = UNIT_ROTATE_START_TIME;
						m_nOldSelectUnit = m_nCurrentSelectUnit;
						int fTemp = (int)(m_fCurrentSelectUnitRotateAngle/(D3DX_PI*2.0f));
						m_fOldSelectUnitRotateAngle = m_fCurrentSelectUnitRotateAngle - fTemp*D3DX_PI*2.0f;
						m_fCurrentSelectUnitRotateAngle = 0;
						if(--m_nCurrentSelectUnit == -1)
						{
							m_nCurrentSelectUnit = m_nMode-1;
						}
						if(m_nMode == CREATE_MODE)
						{
							((CINFSelect*)m_pParent)->ChangeCreateUnit(GetCurrentUnitKind());
							g_pSelect->m_pUnitCreateInfo->StartView();
						}
						else
						{
							if(((CINFSelect*)m_pParent)->m_character[m_nCurrentSelectUnit].CharacterUniqueNumber == 0)
							{
								MSG_FC_CHARACTER_GET_CHARACTER sMsg;
								memset(&sMsg,0x00,sizeof(sMsg));
								char buffer[SIZE_MAX_PACKET];
								sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
								sMsg.CharacterUniqueNumber = ((CINFSelect*)m_pParent)->m_guiUnitInfo[m_nCurrentSelectUnit].CharacterUniqueNumber;
								int nType = T_FC_CHARACTER_GET_CHARACTER;
								memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
								memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
								g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

								g_pD3dApp->m_bRequestEnable = FALSE;
							}					
						}
						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_UNIT_CLICK, D3DXVECTOR3(0,0,0), FALSE);
					}
				}
				break;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(m_nRotateMode == ROTATE_NONE)
			{
				int nCurrentSelectUnit = HitTest(lParam);
				if(m_nCurrentSelectUnit != nCurrentSelectUnit && nCurrentSelectUnit >= 0)
				{
					m_nCurrentSelectUnit = nCurrentSelectUnit;
					
					// 2005-07-04 by ispark 자동 스탯 분배 삭제
//					g_pSelect->m_pCreateMenu->m_nCreateUnitStat = -1;
//					g_pSelect->m_pCreateMenu->m_nCreateUnitStat = AUTOSTAT_TYPE_FREESTYLE;
//					g_pSelect->m_pCreateMenu->m_nRenderToolTipIndex = -1;
					g_pSelect->m_pCreateMenu->m_nFocusStat = -1;
					g_pSelect->m_pCreateMenu->m_nFocus = -1;

					if(m_nMode == CREATE_MODE)
					{						
						((CINFSelect*)m_pParent)->ChangeCreateUnit(GetCurrentUnitKind());
					}
					else
					{
						if(((CINFSelect*)m_pParent)->m_character[m_nCurrentSelectUnit].CharacterUniqueNumber == 0)
						{
							MSG_FC_CHARACTER_GET_CHARACTER sMsg;
							memset(&sMsg,0x00,sizeof(sMsg));

							char buffer[SIZE_MAX_PACKET];
							sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
							sMsg.CharacterUniqueNumber = ((CINFSelect*)m_pParent)->m_guiUnitInfo[m_nCurrentSelectUnit].CharacterUniqueNumber;
							int nType = T_FC_CHARACTER_GET_CHARACTER;
							memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
							memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
							g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

							g_pD3dApp->m_bRequestEnable = FALSE;
						}					
					}
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_UNIT_CLICK, D3DXVECTOR3(0,0,0), FALSE);
				}
			}

		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if(m_nMode == SELECT_MODE && !g_pSelect->m_bDelete)	
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);

				CheckMouseReverse(&pt);
				
				D3DXVECTOR2 v2((float)g_pD3dApp->GetBackBufferDesc().Width/2 - pt.x, (float)g_pD3dApp->GetBackBufferDesc().Height/2 - pt.y);
				float fLen = D3DXVec2Length(&v2);
				if(fLen < g_pD3dApp->GetBackBufferDesc().Width/4)
				{
					// 2009. 10. 14 by jskim 프리스카 제거
					((CINFSelect*)m_pParent)->GameStart(((CINFSelect*)m_pParent)->m_character[GetCurrentSelectIndex()].Material);
				}	//end 2009. 10. 14 by jskim 프리스카 제거
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			D3DXVECTOR2 v2((float)g_pD3dApp->GetBackBufferDesc().Width/2 - pt.x, (float)g_pD3dApp->GetBackBufferDesc().Height/2 - pt.y);
			float fLen = D3DXVec2Length(&v2);
			if(fLen < g_pD3dApp->GetBackBufferDesc().Width/4)
			{
				((CINFSelect*)m_pParent)->SetMouseState(SELECT_MOUSE_STATE_UP);
				return INF_MSGPROC_NORMAL;
			}
			else
			{
				((CINFSelect*)m_pParent)->SetMouseState(SELECT_MOUSE_STATE_NORMAL);
			}
		}
		break;
	}
			
	return INF_MSGPROC_NORMAL;
}

USHORT CINFSelectBack::GetCurrentUnitKind()
{
	FLOG( "CINFSelectBack::GetCurrentUnitKind()" );
	return UNITKIND_BT01 << (4*m_nCurrentSelectUnit);
}

int CINFSelectBack::HitTest(LPARAM lParam)
{
	FLOG( "CINFSelectBack::HitTest(LPARAM lParam)" );
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;
	D3DXMATRIX matProj = g_pD3dApp->m_pCamera->GetProjMatrix();
	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * pt.x ) / g_pD3dApp->GetBackBufferDesc().Width  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * pt.y ) / g_pD3dApp->GetBackBufferDesc().Height ) - 1 ) / matProj._22;
	v.z =  1.0f;

	// Get the inverse view matrix
	D3DXMATRIX matView, m;
	matView = g_pD3dApp->m_pCamera->GetViewMatrix();
	D3DXMatrixInverse( &m, NULL, &matView );

	// Transform the screen space pick ray into 3D space
	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
	D3DXVECTOR3 vTemp(0, vPickRayOrig.y+3, 0);
	D3DXVec3Normalize(&vTemp, &vTemp);
	D3DXVec3Normalize(&vPickRayDir, &vPickRayDir);
	float fDot = D3DXVec3Dot(&vPickRayDir, &vTemp);
	float fAngle = ACOS(fDot);

	float fDis = abs((m_vCurrentCamPos.y+3) / cos(fAngle));
	v = vPickRayDir * fDis + vPickRayOrig;
	for(int i=0;i<m_nMode;i++)
	{
		D3DXVECTOR3 vPos;
		int nRenderType = 0;
		if(m_nMode == CREATE_MODE)
		{
			vPos = D3DXVECTOR3(m_CreateRenderInfo[i].matrix._41, m_CreateRenderInfo[i].matrix._42, m_CreateRenderInfo[i].matrix._43);
			nRenderType = m_CreateRenderInfo[i].rendertype;
		}
		else
		{
			vPos = D3DXVECTOR3(m_SelectRenderInfo[i].matrix._41, m_SelectRenderInfo[i].matrix._42, m_SelectRenderInfo[i].matrix._43)-D3DXVECTOR3(0,19,0);
			nRenderType = m_SelectRenderInfo[i].rendertype;
		}
		if((fDis = D3DXVec3Length(&(vPos-v)))<14)
		{
			if(m_nCurrentSelectUnit == i)
			{
				return m_nCurrentSelectUnit;
			}
			if(m_nCurrentSelectUnit != i && nRenderType != 0 )
			{
				int nNext = m_nCurrentSelectUnit - i;
				if( (m_nMode == SELECT_MODE && (nNext == -2 || nNext == 1)) ||
					(m_nMode == CREATE_MODE && (nNext == -3 || nNext == 1)))
				{
					if(m_nRotateMode == ROTATE_NONE)
					{
						m_fTargetRotationAngle += D3DX_PI*2.0f/m_nMode;
						m_vTargetCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
						m_nRotateMode = ROTATE_RIGHT;
						m_fUnitRotationStartTime = UNIT_ROTATE_START_TIME;
						if(m_nMode == CREATE_MODE)
						{
							g_pSelect->m_pUnitCreateInfo->StartView();
						}
						m_nOldSelectUnit = m_nCurrentSelectUnit;
						int fTemp = (int)(m_fCurrentSelectUnitRotateAngle/(D3DX_PI*2.0f));
						m_fOldSelectUnitRotateAngle = m_fCurrentSelectUnitRotateAngle - fTemp*D3DX_PI*2.0f;
						m_fCurrentSelectUnitRotateAngle = 0;
						//m_nCurrentSelectUnit = i;
						//return i;
					}
				}
				else
				{
					if(m_nRotateMode == ROTATE_NONE)
					{
						m_fTargetRotationAngle -= D3DX_PI*2.0f/m_nMode;
						m_vTargetCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
						m_nRotateMode = ROTATE_LEFT;
						m_fUnitRotationStartTime = UNIT_ROTATE_START_TIME;
						if(m_nMode == CREATE_MODE)
						{
							g_pSelect->m_pUnitCreateInfo->StartView();
						}
						m_nOldSelectUnit = m_nCurrentSelectUnit;
						int fTemp = (int)(m_fCurrentSelectUnitRotateAngle/(D3DX_PI*2.0f));
						m_fOldSelectUnitRotateAngle = m_fCurrentSelectUnitRotateAngle - fTemp*D3DX_PI*2.0f;
						m_fCurrentSelectUnitRotateAngle = 0;
						//m_nCurrentSelectUnit = i;
						//return i;
					}
				}
				return i;
			}
		}
	}
	return -1;
}

void CINFSelectBack::MoveSelect(int nSelect)
{
	FLOG( "CINFSelectBack::MoveSelect(int nSelect)" );
	int nNext = m_nCurrentSelectUnit - nSelect;
	if( (m_nMode == SELECT_MODE && (nNext == -2 || nNext == 1)) ||
		(m_nMode == CREATE_MODE && (nNext == -3 || nNext == 1)))
	{
		if(m_nRotateMode == ROTATE_NONE)
		{
			m_fTargetRotationAngle += D3DX_PI*2.0f/m_nMode;
			m_vTargetCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
			m_nRotateMode = ROTATE_RIGHT;
			m_fUnitRotationStartTime = UNIT_ROTATE_START_TIME;
			m_nOldSelectUnit = m_nCurrentSelectUnit;
			int fTemp = (int)(m_fCurrentSelectUnitRotateAngle/(D3DX_PI*2.0f));
			m_fOldSelectUnitRotateAngle = m_fCurrentSelectUnitRotateAngle - fTemp*D3DX_PI*2.0f;
			m_fCurrentSelectUnitRotateAngle = 0;
		}
	}
	else
	{
		if(m_nRotateMode == ROTATE_NONE)
		{
			m_fTargetRotationAngle -= D3DX_PI*2.0f/m_nMode;
			m_vTargetCamPos = D3DXVECTOR3(CAMERA_UP_POSITION);
			m_nRotateMode = ROTATE_LEFT;
			m_fUnitRotationStartTime = UNIT_ROTATE_START_TIME;
			m_nOldSelectUnit = m_nCurrentSelectUnit;
			int fTemp = (int)(m_fCurrentSelectUnitRotateAngle/(D3DX_PI*2.0f));
			m_fOldSelectUnitRotateAngle = m_fCurrentSelectUnitRotateAngle - fTemp*D3DX_PI*2.0f;
			m_fCurrentSelectUnitRotateAngle = 0;
		}
	}
	m_nCurrentSelectUnit = nSelect;
	if(((CINFSelect*)m_pParent)->m_character[m_nCurrentSelectUnit].CharacterUniqueNumber == 0)
	{
		MSG_FC_CHARACTER_GET_CHARACTER sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
		sMsg.CharacterUniqueNumber = ((CINFSelect*)m_pParent)->m_guiUnitInfo[m_nCurrentSelectUnit].CharacterUniqueNumber;
		int nType = T_FC_CHARACTER_GET_CHARACTER;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

		g_pD3dApp->m_bRequestEnable = FALSE;
	}					
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_UNIT_CLICK, D3DXVECTOR3(0,0,0), FALSE);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFSelectBack::InitRenderMatrix()
/// \brief		렌더 메트릭스 초기화
/// \author		ispark
/// \date		2006-01-12 ~ 2006-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelectBack::InitRenderMatrix()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		D3DXMatrixIdentity(&m_CreateOrgLWeaponInfo[i].matrix);
		D3DXMatrixIdentity(&m_CreateOrgRWeaponInfo[i].matrix);
		D3DXMatrixIdentity(&m_CreateRenderLWeaponInfo[i].matrix);
		D3DXMatrixIdentity(&m_CreateRenderRWeaponInfo[i].matrix);
		
		m_CreateOrgLWeaponInfo[i].rendertype = 0;
		m_CreateOrgRWeaponInfo[i].rendertype = 0;
		m_CreateRenderLWeaponInfo[i].rendertype = 0;
		m_CreateRenderRWeaponInfo[i].rendertype = 0;
	}

	for(i = 0; i < 3; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			D3DXMatrixIdentity(&m_SelectOrgLWeaponInfo[i][j].matrix);
			D3DXMatrixIdentity(&m_SelectOrgRWeaponInfo[i][j].matrix);
			D3DXMatrixIdentity(&m_SelectRenderLWeaponInfo[i][j].matrix);
			D3DXMatrixIdentity(&m_SelectRenderLWeaponInfo[i][j].matrix);

			m_SelectOrgLWeaponInfo[i][j].rendertype = 0;
			m_SelectOrgRWeaponInfo[i][j].rendertype = 0;
			m_SelectRenderLWeaponInfo[i][j].rendertype = 0;
			m_SelectRenderLWeaponInfo[i][j].rendertype = 0;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-10 by bhsohn 셀렉트 화면 기체 높이 갱신
/// \date		2008-07-08 ~ 2008-07-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSelectBack::RefreshUnitMatrix()
{
	float fDevide = m_nMode/2.0f;
	int i;
	for(i=0;i<SELECT_UNIT_NUMBER;i++)
	{
		//D3DXMatrixRotationZ( &m_mSelectTopMatrix[i],D3DX_PI/fDevide*i);
		//m_mSelectMiddleMatrix[i] = m_mSelectTopMatrix[i];
		D3DXMatrixRotationZ( &m_mSelectMiddleMatrix[i],D3DX_PI/fDevide*i);

		D3DXVECTOR3 vcurrentpos = D3DXVECTOR3(0,0,0);//UNIT_POSITION);
		D3DXVECTOR3 vtarget = D3DXVECTOR3(0,-1.0f,0);
		D3DXVECTOR3 vup = D3DXVECTOR3(0,0,1);
		D3DXMATRIX matTemp;
		D3DXMatrixLookAtRH( &matTemp, &vcurrentpos, &(vcurrentpos+vtarget), &vup);
		D3DXMatrixInverse( &m_CreateRenderInfo[i].matrix, NULL, &matTemp );

		D3DXMatrixTranslation( &matTemp, UNIT_POSITION);
		matTemp *= m_mSelectMiddleMatrix[i];
		m_CreateRenderInfo[i].matrix._41 = matTemp._41;
		m_CreateRenderInfo[i].matrix._42 = matTemp._42;
		m_CreateRenderInfo[i].matrix._43 = matTemp._43;
		//m_CreateRenderInfo[i].matrix *= m_mSelectTopMatrix[i];
		D3DXMatrixTranslation( &m_mSelectTopMatrix[i], 0.0f ,0.0f ,0.0f);
		m_mSelectTopMatrix[i]._41 = matTemp._41;//0.0        -36.411      13.802
		m_mSelectTopMatrix[i]._42 = matTemp._42;
		m_mSelectTopMatrix[i]._43 = matTemp._43-SELECT_TOP_HEIGHT;

		//m_CreateRenderInfo[i].rendertype = UNITKIND_BT01 << 4*i;
	}
	fDevide = 3.0f/2.0f;
	for(i=0;i<3;i++)
	{
		D3DXMATRIX matTemp, matTemp2;
		D3DXMatrixRotationZ( &matTemp,D3DX_PI/fDevide*i);

		D3DXVECTOR3 vcurrentpos = D3DXVECTOR3(UNIT_POSITION);
		D3DXVECTOR3 vtarget = D3DXVECTOR3(0,-1.0f,0);
		D3DXVECTOR3 vup = D3DXVECTOR3(0,0,1);
		D3DXMatrixLookAtRH( &m_SelectRenderInfo[i].matrix, &vcurrentpos, &(vcurrentpos+vtarget), &vup);
		D3DXMatrixInverse( &m_SelectRenderInfo[i].matrix, NULL, &m_SelectRenderInfo[i].matrix );

		D3DXMatrixTranslation( &matTemp2, UNIT_POSITION);
		matTemp2 *= matTemp;
		m_SelectRenderInfo[i].matrix._41 = matTemp2._41;
		m_SelectRenderInfo[i].matrix._42 = matTemp2._42;
		// 2008-07-08 by dgwoo 아머 키에 맞춰 출력.
		// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
		//m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center,TRUE);
		if(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center_ShapeItemNum)
			m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center_ShapeItemNum,TRUE);
		else
		m_SelectRenderInfo[i].matrix._43 = matTemp2._43 - 7.087f + GetAmorGearHeight(((CINFSelect*)m_pParent)->m_guiUnitInfo[i].CharacterRenderInfo.RI_Center,TRUE);
		// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
		//m_SelectRenderInfo[i].matrix *= matTemp;

		//m_SelectRenderInfo[i].rendertype = UNITKIND_BT01 << 4*i;
	}
	D3DXMatrixTranslation( &m_mSelectLocationMatrix, 0.0f ,0.0f ,0.0f);
	D3DXMatrixTranslation( &m_mSelectBottomMatrix, 0.0f, 0.0f, 0.0f);
	if(m_nMode == CREATE_MODE)
	{
		((CINFSelect*)m_pParent)->ChangeCreateUnit(GetCurrentUnitKind());
		// 2009-02-10 by bhsohn Japan Charcter Create
		//memset(((CINFSelect*)m_pParent)->m_pCreateMenu->m_strCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
		ZERO_MEMORY(((CINFSelect*)m_pParent)->m_pCreateMenu->m_strCharacterName);
		// end 2009-02-10 by bhsohn Japan Charcter Create
		g_pD3dApp->CleanText();	
	}

}