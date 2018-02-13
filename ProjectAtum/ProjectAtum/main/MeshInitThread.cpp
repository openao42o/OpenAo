// MeshInitThread.cpp: implementation of the CMeshInitThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshInitThread.h"
#include "ShuttleChild.h"
#include "INFGameMain.h"
#include "GameDataLast.h"

// virtual destructor
CMeshInitThread::~CMeshInitThread()
{
	ForceStop();

	_cv.notify_one();
}

DWORD CMeshInitThread::Run()
{
	// lock
	unique_lock<mutex> lk { _mutex };

	while (!MustStop())
	{
		while (!m_queLoadingGameInfo.empty())
		{
			auto inf = m_queLoadingGameInfo.front();
			m_queLoadingGameInfo.pop();
			lk.unlock(); 
			// unlock

			// do work
			CreateGameData(inf); 

			if (MustStop()) return 0;

			// and relock
			lk.lock(); 
		}

		// unlock, wait and relock
		_cv.wait_for(lk, seconds(1));
	}

	lk.unlock();
	// final unlock

	return 0;
}

void CMeshInitThread::QuePushGameData(structLoadingGameInfo * GameInfo)
{
	if (!MustStop())
	{
		{
			lock_guard<mutex> { _mutex };
			m_queLoadingGameInfo.push(GameInfo);
		}
		_cv.notify_one();
	}
}

void CMeshInitThread::CreateGameData(structLoadingGameInfo* GameInfo)
{
	char strPath[MAX_PATH];

	auto pMeshData = new CGameData;

	auto LoadingType = GameInfo->MeshType == _EFFECT_TYPE ?
		IDS_DIRECTORY_EFFECT : IDS_DIRECTORY_OBJECT;
	
	g_pD3dApp->LoadPath(strPath, LoadingType, GameInfo->MeshName);	
	if(pMeshData->SetFile(strPath, false, nullptr, 0))
	{
		const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();	
		
		if (COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR) && LoadingType == IDS_DIRECTORY_OBJECT)
		{
			char buf[16];

			wsprintf(buf,"%08d",atoi(GameInfo->MeshName));		
			
			if(pMeshData->Find(buf) == nullptr)
			{
				char msg[256];
				
				wsprintf(msg, "Resource File Error(%d)", atoi(GameInfo->MeshName));
				
				if(g_pGameMain)
				{
					g_pGameMain->CreateChatChild_OperationMode(msg, COLOR_ERROR);
					util::del(pMeshData);
					return;
				}				
			}
		}
		
		auto LoadingData = new structLoadingGameData;
		
		LoadingData->MeshIndex		= atoi(GameInfo->MeshName);
		LoadingData->MeshType		= GameInfo->MeshType;
		LoadingData->pGameData		= pMeshData;
		LoadingData->Step			= _RESOURCE_LOADING_START;
		LoadingData->Text_Cnt		= 0;
		LoadingData->pSkinnedMesh	= GameInfo->pSkinnedMesh;
		LoadingData->LoadingPriority = GameInfo->LoadingPriority;
				
		EnterCriticalSection(&g_pD3dApp->m_cs);
		g_pD3dApp->vecPushGameData(LoadingData);
		LeaveCriticalSection(&g_pD3dApp->m_cs);
	}
	else util::del(pMeshData);
}

