// MeshRender.cpp: implementation of the CMeshRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "MeshRender.h"
//#include "dxutil.h"
// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
#include "ShuttleChild.h"	
#include "INFGameMain.h"
// end 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
#include "MeshInitThread.h"
#include "CharacterChild.h" 
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshRender::CMeshRender()
{
}

CMeshRender::~CMeshRender()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
}

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
//CSkinnedMesh* CMeshRender::InitData(int nMeshIndex)
CSkinnedMesh* CMeshRender::InitData(int nMeshIndex, int nType, D3DXVECTOR3 ObjScale)	// 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nMeshIndex);
	if( it != m_mapSkinnedMesh.end() && ObjScale == D3DXVECTOR3(1.0f, 1.0f, 1.0f))		// 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
	{
		return it->second;
	}
	// 2007-08-03 by bhsohn 캐릭터 오브젝트 체크썸 보냄
	// 케릭터에 대한 메쉬 로드
	BOOL bCharacter = g_pD3dApp->SendMeshObjectCheckSum(nMeshIndex);	
	// end 2007-08-03 by bhsohn 캐릭터 오브젝트 체크썸 보냄
	
// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
//	char buf[16];
//	char strPath[MAX_PATH];
//	wsprintf(buf,"%08d.obj",nMeshIndex);
//	g_pD3dApp->LoadPath(strPath, IDS_DIRECTORY_OBJECT, buf);
// 	CGameData gameData;
// 	if(gameData.SetFile(strPath,FALSE, NULL,0)==TRUE)
// 	{
// 		// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
// //#ifdef _DEBUG
// //		// 보안 코드
// //		wsprintf(buf,"%08d",nMeshIndex);
// //		if(gameData.Find(buf) == NULL)
// //		{
// //			g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
// //			DBGOUT("Resource File Error(%d)\n",nMeshIndex);	//리소스 파일 에러
// //			return NULL;
// //		}
// //#endif
// 		CHARACTER myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();		
// 		if(COMPARE_RACE(myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER))
// 		{
// 			// 관리자만 스트링을 찍는다.
//		wsprintf(buf,"%08d",nMeshIndex);
//		if(gameData.Find(buf) == NULL)
//		{
//			DBGOUT("Resource File Error(%d)\n",nMeshIndex);	//리소스 파일 에러
// 				char ErrorMsgMissionList[256];
// 				wsprintf(ErrorMsgMissionList, "Resource File Error(%d)", nMeshIndex);
// 				if(g_pGameMain)
// 				{
// 					g_pGameMain->CreateChatChild_OperationMode(ErrorMsgMissionList, COLOR_ERROR);
//		}
// 			}
// 		}
//	 	CSkinnedMesh* pSkinnedMesh  = new CSkinnedMesh(FALSE);
// 		pSkinnedMesh->InitDeviceObjects();
			// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
//		int nObjectIdx = 0;
//		if(FALSE == bCharacter)
//		{
//			nObjectIdx = nMeshIndex;			
//		}
//		pSkinnedMesh->LoadMeshHierarchyFromMem( gameData );
//		m_mapSkinnedMesh[nMeshIndex] = pSkinnedMesh;
// 		if(g_pShuttleChild)
// 		{
// 			g_pD3dApp->m_bLoading = TRUE;
// 		}

	char buf[16];
	char strPath[MAX_PATH];
	wsprintf(buf,"%08d.obj",nMeshIndex);

	// 2012-04-18 by mspark, 오브젝트 리소스 정리
#ifdef C_OBJECT_RESOURCE_ORGANIZE_MSPARK
	FILE* file = _tfopen( _T("(Obj)MeshRender.txt"), _T("a") );
	fprintf( file, "%08d - [MapIndex : %04d]\n", nMeshIndex, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
	fclose(file);	
#endif
	// end 2012-04-18 by mspark, 오브젝트 리소스 정리	

	CSkinnedMesh* pSkinnedMesh  = new CSkinnedMesh(FALSE);
	pSkinnedMesh->InitDeviceObjects();
	m_mapSkinnedMesh[nMeshIndex] = pSkinnedMesh;

	if( g_pD3dApp->m_dwGameState != _GAME || nType == _OBJECT_TYPE ||
		nMeshIndex == g_pShuttleChild->GetUnitNum() ||
		nMeshIndex == g_pCharacterChild->m_nUnitNum)
	{
		CGameData* gameData = new CGameData;
		g_pD3dApp->LoadPath(strPath, IDS_DIRECTORY_OBJECT, buf);
		if(gameData->SetFile(strPath,FALSE, NULL,0)==TRUE)
		{
			const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
			if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
			{
				// 관리자만 스트링을 찍는다.
				wsprintf(buf,"%08d",nMeshIndex);
					if(gameData->Find(buf) == NULL)
				{
					DBGOUT("Resource File Error(%d)\n",nMeshIndex);	//리소스 파일 에러
					char ErrorMsgMissionList[256];
					wsprintf(ErrorMsgMissionList, "Resource File Error(%d)", nMeshIndex);
					if(g_pGameMain)
					{
						g_pGameMain->CreateChatChild_OperationMode(ErrorMsgMissionList, COLOR_ERROR);
					}				
				}
			}
			int nObjectIdx = 0;
			if(FALSE == bCharacter)
			{
				nObjectIdx = nMeshIndex;			
			}
			pSkinnedMesh->LoadMeshHierarchyFromMem( gameData, nObjectIdx, ObjScale);	// 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
			util::del( gameData );
			return pSkinnedMesh;
		}
		util::del( gameData );
		return NULL;
	}
	pSkinnedMesh->SetIsLoadingFlag(TRUE);
	structLoadingGameInfo* LoadingGameInfo = new structLoadingGameInfo;			
	strcpy(LoadingGameInfo->MeshName, buf);
	LoadingGameInfo->MeshType		 = nType;
	LoadingGameInfo->pSkinnedMesh	 = pSkinnedMesh;
	if(nMeshIndex == g_pCharacterChild->m_nUnitNum || nMeshIndex == g_pShuttleChild->GetUnitNum())
	{
		LoadingGameInfo->LoadingPriority = _MY_CHARACTER_PRIORITY;		
	}
	else
	{
		LoadingGameInfo->LoadingPriority = _MESH_PRIORITY;		
	}
	EnterCriticalSection(&g_pD3dApp->m_cs);
	g_pD3dApp->m_pMeshInitThread->QuePushGameData(LoadingGameInfo);
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	return pSkinnedMesh;
}

void CMeshRender::DeleteData(int nMeshIndex)
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nMeshIndex);
	if( it != m_mapSkinnedMesh.end() )
	{
		it->second->InvalidateDeviceObjects();
		it->second->DeleteDeviceObjects();
		util::del(it->second);
		m_mapSkinnedMesh.erase( it );
	}
}

void CMeshRender::RenderShadow(CAtumNode * pNode)
{

}

void CMeshRender::Render()
{

}

void CMeshRender::Tick(float fElapsedTime)
{

}

HRESULT CMeshRender::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CMeshRender::RestoreDeviceObjects()
{
	return S_OK;
}

HRESULT CMeshRender::InvalidateDeviceObjects()
{
	return S_OK;
}

HRESULT CMeshRender::DeleteDeviceObjects()
{
	return S_OK;
}

