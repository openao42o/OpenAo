// BoardData.cpp: implementation of the CBoardData class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "BoardData.h"
#include "AtumApplication.h"
#include "D3dHanFont.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
#ifdef LANGUAGE_VIETNAM
#define DEFAULT_BOLD	D3DFONT_BOLD
#else
#define DEFAULT_BOLD	0
#endif

//////////////////////////////////////////////////////////////////////////
CBoardData::CBoardData(int nBoardIndex, float fWidth, float fHeight, DWORD dwColor, D3DXVECTOR3 vCenter, float fRotX, float fRotY, float fRotZ, BOOL bBothFaces)
{
	m_nBoardIndex = nBoardIndex;
	m_pVBMainBoard = nullptr;
	m_pMainBoardTex = nullptr;
	m_pBoardTextureName = nullptr;
	m_bBothFaces = bBothFaces;
	m_dwColor = dwColor;

	float fBanW = fWidth / 2.0f;
	float fBanH = fHeight / 2.0f;

	m_vMainPos[0].p = D3DXVECTOR3(-fBanW, fBanH, 0.0f);
	m_vMainPos[1].p = D3DXVECTOR3(fBanW, fBanH, 0.0f);
	m_vMainPos[2].p = D3DXVECTOR3(-fBanW, -fBanH, 0.0f);
	m_vMainPos[3].p = D3DXVECTOR3(fBanW, -fBanH, 0.0f);
	m_vMainPos[0].tu = m_vMainPos[0].tv = m_vMainPos[1].tv = m_vMainPos[2].tu = 0;	
	m_vMainPos[1].tu = m_vMainPos[2].tv = m_vMainPos[3].tu = m_vMainPos[3].tv = 1;	

	m_vecElectricBoard.clear();
	m_vecElectricBoardNumber.clear();

	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matRotX);
	D3DXMatrixIdentity(&m_matRotY);
	D3DXMatrixIdentity(&m_matRotZ);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixRotationX(&m_matRotX, fRotX);
	D3DXMatrixRotationY(&m_matRotY, fRotY);
	D3DXMatrixRotationZ(&m_matRotZ, fRotZ);
	D3DXMatrixTranslation(&m_matTrans, vCenter.x, vCenter.y, vCenter.z);
	m_matWorld = m_matRotX * m_matRotY * m_matRotZ * m_matTrans;
}

CBoardData::~CBoardData()
{
	DeleteDeviceObjects();
}

HRESULT CBoardData::InitDeviceObjects(char * pTexName)
{
	m_pBoardTextureName = new char[strlen(pTexName)+1];
	strcpy(m_pBoardTextureName, pTexName);
	return S_OK;
}

HRESULT CBoardData::RestoreDeviceObjects()
{
	//////////////////////////////////////////////////////////////////////////
	// 버텍스 버퍼
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( BOARDVERTEX ),
		0, D3DFVF_BOARDVERTEX, D3DPOOL_MANAGED, &m_pVBMainBoard,NULL ) ) )
	{
		g_pD3dApp->CheckError(CLIENT_ERR_SHUTTLE_EFFECTRESTORE);
		return E_FAIL;
	}
	BOARDVERTEX* v;
	//801.281f
	m_pVBMainBoard->Lock( 0, 0, (void**)&v, 0 );
	for(int i = 0; i < 4; i++)
	{
		v[i].p = m_vMainPos[i].p;
		v[i].tu = m_vMainPos[i].tu;
		v[i].tv = m_vMainPos[i].tv;
	}
	m_pVBMainBoard->Unlock();

	//////////////////////////////////////////////////////////////////////////
	// 텍스쳐
	DataHeader* pDataHeader;
	if(g_pGameMain)
	{
		pDataHeader = g_pGameMain->FindResource(m_pBoardTextureName);
		if(pDataHeader)
		{
			D3DXIMAGE_INFO SrcInfo;
			if(FAILED(D3DXCreateTextureFromFileInMemoryEx( g_pD3dDev, (LPCVOID)pDataHeader->m_pData, pDataHeader->m_DataSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
				0, &SrcInfo, NULL, &m_pMainBoardTex)))
			{
				SAFE_RELEASE(m_pVBMainBoard);
				return E_FAIL;
			}
		}
	}
//	if(FAILED(D3DXCreateTextureFromFile(g_pD3dDev, m_pBoardTextureName, &m_pMainBoardTex)))
//	{
//		SAFE_RELEASE(m_pVBMainBoard);
//		return E_FAIL;
//	}
	//////////////////////////////////////////////////////////////////////////
	// 문자폰트
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( BOARDVERTEX ),
			0, D3DFVF_BOARDVERTEX, D3DPOOL_MANAGED, &(*itBoard)->pVBBoard,NULL ) ) )
		{
			return E_FAIL;
		}
		BOARDVERTEX* v;
		(*itBoard)->pVBBoard->Lock( 0, 0, (void**)&v, 0 );
		for(int i = 0; i < 4; i++)
		{
			v[i].p = (*itBoard)->vPos[i].p;
			v[i].tu = (*itBoard)->vPos[i].tu;
			v[i].tv = (*itBoard)->vPos[i].tv;
		}
		(*itBoard)->pVBBoard->Unlock();

		vector<stBOARDSTRING*>::iterator itBoardString = (*itBoard)->vecBoardString.begin();
		while(itBoardString != (*itBoard)->vecBoardString.end())
		{
			(*itBoardString)->pBoardFont->RestoreDeviceObjects();
			itBoardString++;
		}
		itBoard++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 순자폰트
	vector<ELECTRICBOARD*>::iterator itBoardNumber = m_vecElectricBoardNumber.begin();
	while(itBoardNumber != m_vecElectricBoardNumber.end()) 
	{
		if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( BOARDVERTEX ),
			0, D3DFVF_BOARDVERTEX, D3DPOOL_MANAGED, &(*itBoardNumber)->pVBBoard,NULL ) ) )
		{
			return E_FAIL;
		}
		BOARDVERTEX* v;
		(*itBoardNumber)->pVBBoard->Lock( 0, 0, (void**)&v, 0 );
		for(int i = 0; i < 4; i++)
		{
			v[i].p = (*itBoardNumber)->vPos[i].p;
			v[i].tu = (*itBoardNumber)->vPos[i].tu;
			v[i].tv = (*itBoardNumber)->vPos[i].tv;
		}
		(*itBoardNumber)->pVBBoard->Unlock();

		vector<stBOARDSTRING*>::iterator itBoardString = (*itBoardNumber)->vecBoardString.begin();
		while(itBoardString != (*itBoardNumber)->vecBoardString.end())
		{
			(*itBoardString)->pBoardFont->RestoreDeviceObjects();
			itBoardString++;
		}
		itBoardNumber++;
	}

	return S_OK;
}

HRESULT CBoardData::DeleteDeviceObjects()
{
	// 문자
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		SAFE_RELEASE((*itBoard)->pVBBoard);
		vector<stBOARDSTRING*>::iterator itBoardString = (*itBoard)->vecBoardString.begin();
		while(itBoardString != (*itBoard)->vecBoardString.end())
		{
			(*itBoardString)->pBoardFont->DeleteDeviceObjects();
			util::del((*itBoardString)->pBoardFont);
			itBoardString++;
		}
		(*itBoard)->vecBoardString.clear();
		itBoard++;
	}

	m_vecElectricBoard.clear();

	// 숫자
	vector<ELECTRICBOARD*>::iterator itBoardNumber = m_vecElectricBoardNumber.begin();
	while(itBoardNumber != m_vecElectricBoardNumber.end())
	{
		SAFE_RELEASE((*itBoardNumber)->pVBBoard);
		vector<stBOARDSTRING*>::iterator itBoardString = (*itBoardNumber)->vecBoardString.begin();
		while(itBoardString != (*itBoardNumber)->vecBoardString.end())
		{
			(*itBoardString)->pBoardFont->DeleteDeviceObjects();
			util::del((*itBoardString)->pBoardFont);
			itBoardString++;
		}
		(*itBoardNumber)->vecBoardString.clear();
		itBoardNumber++;
	}
	
	m_vecElectricBoardNumber.clear();

	SAFE_RELEASE(m_pVBMainBoard);
	SAFE_RELEASE(m_pMainBoardTex);
	return S_OK;
}

HRESULT CBoardData::InvalidateDeviceObjects()
{
	// 문자
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		SAFE_RELEASE((*itBoard)->pVBBoard);
		vector<stBOARDSTRING*>::iterator itBoardString = (*itBoard)->vecBoardString.begin();
		while(itBoardString != (*itBoard)->vecBoardString.end())
		{
			(*itBoardString)->pBoardFont->InvalidateDeviceObjects();
			itBoardString++;
		}
		itBoard++;
	}

	// 숫자
	vector<ELECTRICBOARD*>::iterator itBoardNumber = m_vecElectricBoardNumber.begin();
	while(itBoardNumber != m_vecElectricBoardNumber.end()) 
	{
		SAFE_RELEASE((*itBoardNumber)->pVBBoard);
		vector<stBOARDSTRING*>::iterator itBoardString = (*itBoardNumber)->vecBoardString.begin();
		while(itBoardString != (*itBoardNumber)->vecBoardString.end())
		{
			(*itBoardString)->pBoardFont->InvalidateDeviceObjects();
			itBoardString++;
		}
		itBoardNumber++;
	}

	SAFE_RELEASE(m_pVBMainBoard);
	SAFE_RELEASE(m_pMainBoardTex);

	return S_OK;
}

void CBoardData::Tick(float fElapsedTime)
{
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		if(pElectricBoard->nTextCount != 0 && pElectricBoard->vecBoardString.size() != 0 && pElectricBoard->bBoardMove == TRUE)
		{
			BOARDVERTEX* v;
			float fMoveRate = fElapsedTime * pElectricBoard->fScrollMove;
			pElectricBoard->fCurrentMove += fMoveRate;
			float fU1, fU2;
			fU2 = (float)pElectricBoard->nTexWidth / pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->nBoardFontWidth;
			fU1 = pElectricBoard->fCurrentMove - fU2;
			fU2 = fU1 + fU2;
			if(fU2 >= 1.0f)
			{
				fU2 = 1.0f;
			}
			pElectricBoard->pVBBoard->Lock( 0, 0, (void**)&v, 0 );
			v[0].p = pElectricBoard->vPos[0].p;  v[0].tu=fU1; v[0].tv=0;	
			v[1].p = pElectricBoard->vPos[1].p;  v[1].tu=fU2; v[1].tv=0;	
			v[2].p = pElectricBoard->vPos[2].p;  v[2].tu=fU1; v[2].tv=1;	
			v[3].p = pElectricBoard->vPos[3].p;  v[3].tu=fU2; v[3].tv=1;	
			pElectricBoard->pVBBoard->Unlock();

			if(1.0f <= fU2)
			{
				pElectricBoard->nRenderTextIndex++;
				pElectricBoard->nRenderTextIndex = pElectricBoard->nRenderTextIndex % pElectricBoard->nTextCount;
				pElectricBoard->fCurrentMove = 0.0f;
			}
			
		}
		itBoard++;
	}

	CheckBoardStringUse();
}

void CBoardData::Render(float* fOffset)
{
	if(m_bBothFaces)
	{
		g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		
	}
	else
	{
		g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
	}
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE);
	g_pD3dDev->SetFVF( D3DFVF_BOARDVERTEX );

	// 메인 바탕
	if(m_pVBMainBoard)
	{
		g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(0.0f));
		*fOffset += -0.00005f;
		g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(*fOffset));
		g_pD3dDev->SetTexture( 0, m_pMainBoardTex );
		g_pD3dDev->SetTransform( D3DTS_WORLD, &m_matWorld );
		g_pD3dDev->SetStreamSource( 0, m_pVBMainBoard,0, sizeof(BOARDVERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	}

	// 문자 서브
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		if(pElectricBoard->nTextCount != 0)
		{
			if(pElectricBoard->pVBBoard)
			{
				g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(0.0f));
				*fOffset += -0.00005f;
				g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(*fOffset));
				if(pElectricBoard->vecBoardString.size() != 0 &&
					pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->pBoardFont && 
					pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->pBoardFont->m_pTexture)
					g_pD3dDev->SetTexture( 0, pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->pBoardFont->m_pTexture );
				else
					g_pD3dDev->SetTexture( 0, nullptr);
				g_pD3dDev->SetTransform( D3DTS_WORLD, &m_matWorld );
				g_pD3dDev->SetStreamSource( 0, pElectricBoard->pVBBoard,0, sizeof(BOARDVERTEX) );
				g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			}
		}
		itBoard++;
	}

	// 숫자 서브
	*fOffset += -0.00005f;
	vector<ELECTRICBOARD*>::iterator itBoardNumber = m_vecElectricBoardNumber.begin();
	while(itBoardNumber != m_vecElectricBoardNumber.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoardNumber);
		if(pElectricBoard->nTextCount != 0)
		{
			if(pElectricBoard->pVBBoard)
			{
				g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(0.0f));
				g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(*fOffset));
				if(pElectricBoard->vecBoardString.size() != 0 &&
					pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->pBoardFont && 
					pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->pBoardFont->m_pTexture)
					g_pD3dDev->SetTexture( 0, pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->pBoardFont->m_pTexture );
				else
					g_pD3dDev->SetTexture( 0, nullptr);
				g_pD3dDev->SetTransform( D3DTS_WORLD, &m_matWorld );
				g_pD3dDev->SetStreamSource( 0, pElectricBoard->pVBBoard,0, sizeof(BOARDVERTEX) );
				g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 ); // 2016-01-08 exception at this poin, update dirty portion
			}
		}
		itBoardNumber++;
	}
	
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE);
	g_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
}

void CBoardData::AddBoard(int nBoardStringIndex, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight, float fMoveRate, BOOL bMove)
{
	ELECTRICBOARD * pElectricBoard = new ELECTRICBOARD;

	pElectricBoard->nBoardStringIndex = nBoardStringIndex;
	pElectricBoard->nTexWidth = nTexWidth;
	pElectricBoard->nTexHeight = nTexHeight;
	pElectricBoard->fScrollMove = fMoveRate;
	pElectricBoard->bBoardMove = bMove;

	// 메인 바탕보다 넘어가지 않게
	float fVecX, fVecY;
	if(fltx < 0.0f)
		fltx = 0.0f;
	if(flty < 0.0f)
		flty = 0.0f;

	fVecX = m_vMainPos[0].p.x + fltx + fWidth;
	fVecY = m_vMainPos[0].p.y - flty - fHeight;
	if(fVecX > m_vMainPos[1].p.x)
	{
		fVecX = m_vMainPos[1].p.x;
	}
	if(fVecY < m_vMainPos[2].p.y)
	{
		fVecY = m_vMainPos[2].p.y;
	}
	
	pElectricBoard->vPos[0].p = D3DXVECTOR3(m_vMainPos[0].p.x + fltx, m_vMainPos[0].p.y - flty, 0.0f);
	pElectricBoard->vPos[1].p = D3DXVECTOR3(fVecX, m_vMainPos[0].p.y - flty, 0.0f);
	pElectricBoard->vPos[2].p = D3DXVECTOR3(m_vMainPos[2].p.x + fltx, fVecY, 0.0f);
	pElectricBoard->vPos[3].p = D3DXVECTOR3(fVecX, fVecY, 0.0f);
	pElectricBoard->vPos[0].tu = pElectricBoard->vPos[0].tv = pElectricBoard->vPos[1].tv = pElectricBoard->vPos[2].tu = 0;	
	pElectricBoard->vPos[1].tu = pElectricBoard->vPos[2].tv = pElectricBoard->vPos[3].tu = pElectricBoard->vPos[3].tv = 1;	

	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( BOARDVERTEX ),
		0, D3DFVF_BOARDVERTEX, D3DPOOL_MANAGED, &pElectricBoard->pVBBoard,NULL ) ) )
	{
		util::del(pElectricBoard);
		return;
	}
	BOARDVERTEX* v;
	pElectricBoard->pVBBoard->Lock( 0, 0, (void**)&v, 0 );
	for(int i = 0; i < 4; i++)
	{
		v[i].p = pElectricBoard->vPos[i].p;
		v[i].tu = pElectricBoard->vPos[i].tu;
		v[i].tv = pElectricBoard->vPos[i].tv;
	}
	pElectricBoard->pVBBoard->Unlock();

	m_vecElectricBoard.push_back(pElectricBoard);
}

void CBoardData::AddBoardString(int nBoardStringIndex, const char* pFontName, int nFontSize, int nStringIndex, char* pStr)
{
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		if(pElectricBoard->nBoardStringIndex == nBoardStringIndex)
		{
			stBOARDSTRING * pBoardString = new stBOARDSTRING;
			pBoardString->pBoardFont = new CD3DHanFont(pFontName, nFontSize, D3DFONT_BOLD, FALSE, pElectricBoard->nTexWidth*2, pElectricBoard->nTexHeight);
			pBoardString->pBoardFont->InitDeviceObjects(g_pD3dDev);

			if(pElectricBoard->bBoardMove == TRUE)
			{
				SIZE strSize = pBoardString->pBoardFont->GetStringSize(pStr);
				pBoardString->pBoardFont->SetTexWidth(strSize.cx + pElectricBoard->nTexWidth);
				pBoardString->pBoardFont->SetTexHeight(strSize.cy);
				pBoardString->nBoardFontWidth = strSize.cx + pElectricBoard->nTexWidth;
				pBoardString->nBoardFontHeight = strSize.cy;

				pBoardString->StringIndex = nStringIndex;
			}
			pBoardString->pBoardFont->RestoreDeviceObjects();
			char chSTRTemp[SIZE_MAX_SIGN_BOARD_STRING+3] = {0,};
			sprintf(chSTRTemp, "* %s", pStr);
			pBoardString->pBoardFont->SetText(0, 0, chSTRTemp, m_dwColor);
			pBoardString->bUse = TRUE;

			pElectricBoard->vecBoardString.push_back(pBoardString);
			pElectricBoard->nTextCount++;
		}
		itBoard++;
	}
}

void CBoardData::AddBoardNumber(int nNumberCount, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight)
{
	// 가로만 쓴다.
	float fNumWidth = fWidth / nNumberCount;
	int nNumTexWidth = nTexWidth / nNumberCount;
	for(int i = 0; i < nNumberCount; i++)
	{
		ELECTRICBOARD * pElectricBoard = new ELECTRICBOARD;

		pElectricBoard->nBoardStringIndex = i;
		pElectricBoard->nTexWidth = nNumTexWidth;
		pElectricBoard->nTexHeight = nTexHeight;

		// 메인 바탕보다 넘어가지 않게
		float fStVecX, fStVecY;
		float fEdVecX, fEdVecY;
		if(fltx < 0.0f)
			fltx = 0.0f;
		if(flty < 0.0f)
			flty = 0.0f;

		fStVecX = m_vMainPos[0].p.x + fltx + (fNumWidth*i);
		fStVecY = m_vMainPos[0].p.y - flty;
		fEdVecX = m_vMainPos[0].p.x + fltx + (fNumWidth*(i+1));
		fEdVecY = m_vMainPos[0].p.y - flty - fHeight;
		if(fEdVecX > m_vMainPos[1].p.x)
		{
			fEdVecX = m_vMainPos[1].p.x;
		}
		if(fEdVecY < m_vMainPos[2].p.y)
		{
			fEdVecY = m_vMainPos[2].p.y;
		}
		
		pElectricBoard->vPos[0].p = D3DXVECTOR3(fStVecX, fStVecY, 0.0f);
		pElectricBoard->vPos[1].p = D3DXVECTOR3(fEdVecX, fStVecY, 0.0f);
		pElectricBoard->vPos[2].p = D3DXVECTOR3(fStVecX, fEdVecY, 0.0f);
		pElectricBoard->vPos[3].p = D3DXVECTOR3(fEdVecX, fEdVecY, 0.0f);
		pElectricBoard->vPos[0].tu = pElectricBoard->vPos[0].tv = pElectricBoard->vPos[1].tv = pElectricBoard->vPos[2].tu = 0;	
		pElectricBoard->vPos[1].tu = pElectricBoard->vPos[2].tv = pElectricBoard->vPos[3].tu = pElectricBoard->vPos[3].tv = 1;	

		if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( BOARDVERTEX ),
			0, D3DFVF_BOARDVERTEX, D3DPOOL_MANAGED, &pElectricBoard->pVBBoard,NULL ) ) )
		{
			util::del(pElectricBoard);
			return;
		}
		BOARDVERTEX* v;
		pElectricBoard->pVBBoard->Lock( 0, 0, (void**)&v, 0 );
		for(int i = 0; i < 4; i++)
		{
			v[i].p = pElectricBoard->vPos[i].p;
			v[i].tu = pElectricBoard->vPos[i].tu;
			v[i].tv = pElectricBoard->vPos[i].tv;
		}
		pElectricBoard->pVBBoard->Unlock();

		m_vecElectricBoardNumber.push_back(pElectricBoard);		
	}
}

BOOL CBoardData::DelBoardString(int nBoardStringIndex, int nStringIndex)
{
	BOOL bErase = FALSE;
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		if(pElectricBoard->nBoardStringIndex == nBoardStringIndex)
		{
			vector<stBOARDSTRING*>::iterator itBoardString = (*itBoard)->vecBoardString.begin();
			while(itBoardString != (*itBoard)->vecBoardString.end())
			{
				if((*itBoardString)->StringIndex == nStringIndex)
				{
					(*itBoardString)->pBoardFont->DeleteDeviceObjects();
					util::del((*itBoardString)->pBoardFont);
					(*itBoard)->vecBoardString.erase(itBoardString);
					pElectricBoard->nTextCount--;
					bErase = TRUE;
					break;
				}
				itBoardString++;
			}

		}
		itBoard++;
	}
	
	return bErase;
}

void CBoardData::SetBoardStringUse(int nBoardStringIndex, int nStringIndex, BOOL bUse)
{
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		if(pElectricBoard->nBoardStringIndex == nBoardStringIndex)
		{
			vector<stBOARDSTRING*>::iterator itBoardString = (*itBoard)->vecBoardString.begin();
			while(itBoardString != (*itBoard)->vecBoardString.end())
			{
				if((*itBoardString)->StringIndex == nStringIndex)
				{
					(*itBoardString)->bUse = bUse;
					break;
				}
				itBoardString++;
			}

		}
		itBoard++;
	}
}

void CBoardData::CheckBoardStringUse()
{
	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoard.begin();
	while(itBoard != m_vecElectricBoard.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		vector<stBOARDSTRING*>::iterator itBoardString = pElectricBoard->vecBoardString.begin();
		while(itBoardString != pElectricBoard->vecBoardString.end())
		{
			if((*itBoardString)->bUse == FALSE &&
				((pElectricBoard->fCurrentMove == 0.0f && pElectricBoard->nTextCount == 1) ||
				pElectricBoard->vecBoardString[pElectricBoard->nRenderTextIndex]->StringIndex != (*itBoardString)->StringIndex))
			{
				(*itBoardString)->pBoardFont->DeleteDeviceObjects();
				util::del((*itBoardString)->pBoardFont);
				(*itBoard)->vecBoardString.erase(itBoardString);
				pElectricBoard->nTextCount--;
				continue;
			}

			itBoardString++;
		}

		itBoard++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CBoardData::SetBoardNumberString(int nBoardStringIndex, char *pFontName, int nFontSize, char* pStr)
/// \brief		고정되는 메세지 내용 바꾸기 없으면 추가(스트링 인덱스는 항상 0이다.)
/// \author		ispark
/// \date		2006-04-19 ~ 2006-04-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBoardData::SetBoardNumberString(char *pFontName, int nFontSize, char* pStr)
{
	int nStrCount = strlen(pStr);
	int nNumberCount = m_vecElectricBoardNumber.size();
	int nPassCount = nNumberCount - nStrCount;
	int nInputCount = 0;
	if(nPassCount < 0)
	{
		nInputCount = nPassCount * -1;
		nPassCount = 0;
	}

	vector<ELECTRICBOARD*>::iterator itBoardNumber = m_vecElectricBoardNumber.begin();
	while(itBoardNumber != m_vecElectricBoardNumber.end()) 
	{
		if(nPassCount > 0)
		{
			nPassCount--;
			itBoardNumber++;
			continue;
		}
		
		ELECTRICBOARD* pElectricBoard = (*itBoardNumber);
		// 스트링 인덱스는 항상 0이다.
		char chTemp[2] = {0,};
		strncpy(chTemp, &pStr[nInputCount], 1);
		if(pElectricBoard->vecBoardString.size() > 0)
		{
			pElectricBoard->vecBoardString[0]->pBoardFont->SetText(0, 0, chTemp, m_dwColor);
		}
		else
		{
			stBOARDSTRING * pBoardString = new stBOARDSTRING;
			pBoardString->pBoardFont = new CD3DHanFont(pFontName, nFontSize, DEFAULT_BOLD, FALSE, pElectricBoard->nTexWidth*2, pElectricBoard->nTexHeight);
			pBoardString->pBoardFont->InitDeviceObjects(g_pD3dDev);
			pBoardString->pBoardFont->RestoreDeviceObjects();
			pBoardString->pBoardFont->SetText(0, 0, chTemp, m_dwColor);
			pBoardString->bUse = TRUE;

			pElectricBoard->vecBoardString.push_back(pBoardString);
		}
		nInputCount++;
		pElectricBoard->nTextCount++;
		itBoardNumber++;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBoardData::AddBoardWorldRank(int nNumberCount, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight)
{
	// 가로만 쓴다.
// 	float fNumWidth = fWidth / nNumberCount;
// 	int nNumTexWidth = nTexWidth / nNumberCount;
// 	for(int i = 0; i < nNumberCount; i++)
	{
		ELECTRICBOARD * pElectricBoard = new ELECTRICBOARD;
		
		//pElectricBoard->nBoardStringIndex = i;
		//pElectricBoard->nTexWidth = nNumTexWidth;
		pElectricBoard->nBoardStringIndex = nNumberCount;
		pElectricBoard->nTexWidth = nTexWidth;
		pElectricBoard->nTexHeight = nTexHeight;
		
		// 메인 바탕보다 넘어가지 않게
		float fStVecX, fStVecY;
		float fEdVecX, fEdVecY;
		if(fltx < 0.0f)
			fltx = 0.0f;
		if(flty < 0.0f)
			flty = 0.0f;
		
		//fStVecX = m_vMainPos[0].p.x + fltx + (fNumWidth*i);
		fStVecX = m_vMainPos[0].p.x + fltx;
		fStVecY = m_vMainPos[0].p.y - flty;
		//fEdVecX = m_vMainPos[0].p.x + fltx + (fNumWidth*(i+1));
		fEdVecX = m_vMainPos[0].p.x + fltx + (fWidth);
		fEdVecY = m_vMainPos[0].p.y - flty - fHeight;
		if(fEdVecX > m_vMainPos[1].p.x)
		{
			fEdVecX = m_vMainPos[1].p.x;
		}
		if(fEdVecY < m_vMainPos[2].p.y)
		{
			fEdVecY = m_vMainPos[2].p.y;
		}
		
		pElectricBoard->vPos[0].p = D3DXVECTOR3(fStVecX, fStVecY, 0.0f);
		pElectricBoard->vPos[1].p = D3DXVECTOR3(fEdVecX, fStVecY, 0.0f);
		pElectricBoard->vPos[2].p = D3DXVECTOR3(fStVecX, fEdVecY, 0.0f);
		pElectricBoard->vPos[3].p = D3DXVECTOR3(fEdVecX, fEdVecY, 0.0f);
		pElectricBoard->vPos[0].tu = pElectricBoard->vPos[0].tv = pElectricBoard->vPos[1].tv = pElectricBoard->vPos[2].tu = 0;	
		pElectricBoard->vPos[1].tu = pElectricBoard->vPos[2].tv = pElectricBoard->vPos[3].tu = pElectricBoard->vPos[3].tv = 1;	
		
		if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( BOARDVERTEX ),
			0, D3DFVF_BOARDVERTEX, D3DPOOL_MANAGED, &pElectricBoard->pVBBoard,NULL ) ) )
		{
			util::del(pElectricBoard);
			return;
		}
		BOARDVERTEX* v;
		pElectricBoard->pVBBoard->Lock( 0, 0, (void**)&v, 0 );
		for(int i = 0; i < 4; i++)
		{
			v[i].p = pElectricBoard->vPos[i].p;
			v[i].tu = pElectricBoard->vPos[i].tu;
			v[i].tv = pElectricBoard->vPos[i].tv;
		}
		pElectricBoard->pVBBoard->Unlock();
		
		m_vecElectricBoardNumber.push_back(pElectricBoard);		
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBoardData::SetWorldRankBoardString(int i_nBoardStringIndex, int i_nX, const char* i_pFontName, int i_nFontSize, char* i_pStr, DWORD	i_dwColor)
{
	int nBoardStringIndex = i_nBoardStringIndex;
	CD3DHanFont* pD3DHanFont = nullptr;

	vector<ELECTRICBOARD*>::iterator itBoard = m_vecElectricBoardNumber.begin();
	while(itBoard != m_vecElectricBoardNumber.end()) 
	{
		ELECTRICBOARD* pElectricBoard = (*itBoard);
		if(pElectricBoard->nBoardStringIndex == nBoardStringIndex)
		{
			if(pElectricBoard->vecBoardString.size() <= i_nX)
			{
				stBOARDSTRING * pBoardString = new stBOARDSTRING;
				pBoardString->pBoardFont = new CD3DHanFont(i_pFontName, i_nFontSize, 
					D3DFONT_BOLD, FALSE, 
					pElectricBoard->nTexWidth*2, pElectricBoard->nTexHeight);

				pBoardString->pBoardFont->InitDeviceObjects(g_pD3dDev);				
				pBoardString->pBoardFont->RestoreDeviceObjects();	
				
				pBoardString->bUse = TRUE;
				pElectricBoard->nTextCount++;
				
				pElectricBoard->vecBoardString.push_back(pBoardString);
				
			}
			int nSelFont = i_nX;
			if(pElectricBoard->vecBoardString.size() <= i_nX)
			{
				// 폰트를 추가했는데도 크기 이상이면 선택폰트를 0번째 걸로
				nSelFont = 0;
			}
			pD3DHanFont = pElectricBoard->vecBoardString[nSelFont]->pBoardFont;			
			break;
		}
		itBoard++;
	}
	if(NULL == pD3DHanFont)
	{
		return;
	}
	if(strlen(i_pStr) > 0)
	{
		pD3DHanFont->SetText(0, 0, i_pStr, i_dwColor);
	}
	else
	{
		char chTmp[16];
		wsprintf(chTmp, " ");
		pD3DHanFont->SetText(0, 0, chTmp, i_dwColor);
	}
}