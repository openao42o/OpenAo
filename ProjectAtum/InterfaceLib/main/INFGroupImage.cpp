// INFGroupImage.cpp: implementation of the CINFGroupImage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "INFGroupImage.h"
#include "AtumApplication.h"
#include "SceneData.h"
#include "INFImageEx.h"
#include "INFImageList.h"
#include "INFControl.h"
#include "d3dx9core.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGroupImage::CINFGroupImage()
{
// 	m_lpRenderSurface = NULL;
// 	m_lpRenderTexture = NULL;

	m_v2MinPos =	D3DXVECTOR2( 0.0f, 0.0f );
	m_v2MaxPos =	D3DXVECTOR2( 0.0f, 0.0f );
	m_v2Trans =		D3DXVECTOR2( 0.0f, 0.0f );
}

CINFGroupImage::~CINFGroupImage()
{

}

HRESULT CINFGroupImage::InitDeviceObjects( CINFImageList* pImageList )
{
	m_pUIImageInfo = pImageList;
	return S_OK;
}

HRESULT CINFGroupImage::RestoreDeviceObjects()
{
	if(g_pD3dApp->GetDirectSprite())
	{
		m_pd3dxSprite = g_pD3dApp->GetDirectSprite();
	}
	if(!m_pd3dxSprite)
	{
		D3DXCreateSprite(g_pD3dDev , &m_pd3dxSprite);
	}
	
	for( int i=0; i < m_vecImageList.size(); i++ )
	{
		if( m_vecImageList[i]->GetTexture() == NULL )
		{
			((CINFImageEx*)m_vecImageList[i])->RestoreDeviceObjects();
		}
	}	

	CalculateMinMax();
// 	UpdateTexture();
// 	
// 	CreateRenderTexture();
	return S_OK;
}

HRESULT CINFGroupImage::InvalidateDeviceObjects()
{
	for(int i=0; i < m_vecImageList.size(); i++)
	{
		((CINFImageEx*)m_vecImageList[i])->InvalidateDeviceObjects();
	}
	return S_OK;
}

HRESULT CINFGroupImage::DeleteDeviceObjects()
{
	for(int i=0; i < m_vecImageList.size(); i++)
	{if(m_vecImageList[i])
	{
		m_vecImageList[i]->DeleteDeviceObjects();
	}
	}
	return S_OK;
}

void CINFGroupImage::CalculateMinMax()
{
	D3DXVECTOR2 MinPos;
	D3DXVECTOR2 MaxPos;

	if(m_vecImageList.size() > 0)
	{
		if( m_vecImageList[0]->GetType() == CONTROL_TYPE )
		{
			MinPos.x =  ((CINFControl*)m_vecImageList[0])->GetMinPos().x;
			MinPos.y =  ((CINFControl*)m_vecImageList[0])->GetMinPos().y;
			
			MaxPos.x =  ((CINFControl*)m_vecImageList[0])->GetMaxPos().x;
			MaxPos.y =  ((CINFControl*)m_vecImageList[0])->GetMaxPos().y;

			D3DXVECTOR2 temp;
			if(m_vecImageList.size() > 1)
			{
				for(int i =1; i < m_vecImageList.size(); i++)
				{
					if( m_vecImageList[i]->GetType() == CONTROL_TYPE )
					{
						if(m_vecImageList[i]->GetPtRect())
							temp = D3DXVECTOR2( ( m_vecImageList[i]->GetPtRect()->right - m_vecImageList[i]->GetPtRect()->left ) * m_vecImageList[i]->GetScale().x  * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent() , ( m_vecImageList[i]->GetPtRect()->bottom - m_vecImageList[i]->GetPtRect()->top ) * m_vecImageList[i]->GetScale().y  * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent() ) ;
						else
							temp = D3DXVECTOR2(  (m_vecImageList[i])->GetSrcInfo().Width * m_vecImageList[i]->GetScale().x * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent(),  ( m_vecImageList[i])->GetSrcInfo().Height * m_vecImageList[i]->GetScale().y  * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent() );

						MinPos.x = min( m_vecImageList[i]->GetTransVector().x, MinPos.x );
						MinPos.y = min( m_vecImageList[i]->GetTransVector().y, MinPos.y );

						MaxPos.x = max( m_vecImageList[i]->GetTransVector().x + temp.x, MaxPos.x );
						MaxPos.y = max( m_vecImageList[i]->GetTransVector().y + temp.y, MaxPos.y );
					}
					else
					{
						MinPos.x = min( ((CINFControl*)m_vecImageList[i])->GetMinPos().x, MinPos.x );
						MinPos.y = min( ((CINFControl*)m_vecImageList[i])->GetMinPos().y, MinPos.y );

						MaxPos.x = max( ((CINFControl*)m_vecImageList[i])->GetMaxPos().x, MaxPos.x );
						MaxPos.y = max( ((CINFControl*)m_vecImageList[i])->GetMaxPos().y, MaxPos.y );
					}
				}
			}
		}
		else
		{
			MinPos.x =  m_vecImageList[0]->GetTransVector().x;
			MinPos.y =  m_vecImageList[0]->GetTransVector().y;

			D3DXVECTOR2 temp;
			if(m_vecImageList[0]->GetPtRect())
				temp = D3DXVECTOR2( ( m_vecImageList[0]->GetPtRect()->right - m_vecImageList[0]->GetPtRect()->left ) * m_vecImageList[0]->GetScale().x  * ((CINFImageEx*)m_vecImageList[0])->GetScaleIsPercent() , ( m_vecImageList[0]->GetPtRect()->bottom - m_vecImageList[0]->GetPtRect()->top ) * m_vecImageList[0]->GetScale().y  * ((CINFImageEx*)m_vecImageList[0])->GetScaleIsPercent() );
			else
				temp = D3DXVECTOR2(  (m_vecImageList[0])->GetSrcInfo().Width * m_vecImageList[0]->GetScale().x  * ((CINFImageEx*)m_vecImageList[0])->GetScaleIsPercent(),  (m_vecImageList[0])->GetSrcInfo().Height * m_vecImageList[0]->GetScale().y  * ((CINFImageEx*)m_vecImageList[0])->GetScaleIsPercent() );

			MaxPos.x =  m_vecImageList[0]->GetTransVector().x + temp.x;
			MaxPos.y =  m_vecImageList[0]->GetTransVector().y + temp.y;

			if(m_vecImageList.size() > 1)
			{
				for(int i =1; i < m_vecImageList.size(); i++)
				{
					if( m_vecImageList[i]->GetType() != CONTROL_TYPE )
					{
						if(m_vecImageList[i]->GetPtRect())
							temp = D3DXVECTOR2( ( m_vecImageList[i]->GetPtRect()->right - m_vecImageList[i]->GetPtRect()->left ) * m_vecImageList[i]->GetScale().x  * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent() , ( m_vecImageList[i]->GetPtRect()->bottom - m_vecImageList[i]->GetPtRect()->top ) * m_vecImageList[i]->GetScale().y  * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent() ) ;
						else
							temp = D3DXVECTOR2(  (m_vecImageList[i])->GetSrcInfo().Width * m_vecImageList[i]->GetScale().x * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent(),  (m_vecImageList[i])->GetSrcInfo().Height * m_vecImageList[i]->GetScale().y  * ((CINFImageEx*)m_vecImageList[i])->GetScaleIsPercent() );

						MinPos.x = min( m_vecImageList[i]->GetTransVector().x, MinPos.x );
						MinPos.y = min( m_vecImageList[i]->GetTransVector().y, MinPos.y );

						MaxPos.x = max( m_vecImageList[i]->GetTransVector().x + temp.x, MaxPos.x );
						MaxPos.y = max( m_vecImageList[i]->GetTransVector().y + temp.y, MaxPos.y );
					}
					else
					{
						MinPos.x = min( ((CINFControl*)m_vecImageList[i])->GetMinPos().x, MinPos.x );
						MinPos.y = min( ((CINFControl*)m_vecImageList[i])->GetMinPos().y, MinPos.y );

						MaxPos.x = max( ((CINFControl*)m_vecImageList[i])->GetMaxPos().x, MaxPos.x );
						MaxPos.y = max( ((CINFControl*)m_vecImageList[i])->GetMaxPos().y, MaxPos.y );
					}
				}
			}
		}			
	}
	m_v2MaxPos = MaxPos;
	m_v2MinPos = MinPos;
}

void CINFGroupImage::Tick()
{
	if( IsImageModify() )
	{
// 		CalculateMinMax();
// 		UpdateTexture();
// 		CreateRenderTexture();
	}	
}

HRESULT CINFGroupImage::CreateRenderTexture()
{
// 	LPDIRECT3DSURFACE9	m_lpMainSurface;
//  	g_pD3dDev->GetRenderTarget( 0, &m_lpMainSurface );
// 	if(m_lpRenderTexture)
// 	{
// 		g_pD3dDev->SetRenderTarget( 0, m_lpRenderSurface );
//  		if( SUCCEEDED( g_pD3dDev->BeginScene() ) )
//  		{
//  			g_pD3dDev->Clear(0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0); 
//  
// 			// 그림자 색 및 텍스쳐 설정
//  			//g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
// // 			g_pD3dDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
// //    			g_pD3dDev->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA );
// // 			g_pD3dDev->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVDESTALPHA );
// 			//g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
// 			g_pD3dDev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
// 			g_pD3dDev->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
// 			g_pD3dDev->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
// 			if( m_vecImageList.size() > 0 )
//  			{	
//  	 			for(int i=0; i < m_vecImageList.size(); i++)
//  	 			{
// 					// 					// texture의 알파값을 증가시켜 2회 블렌딩 후 오차를 줄인다. by dhkwon 2011.11.18
// 					// 					D3DLOCKED_RECT Rect; 
// 					// 					LPDIRECT3DTEXTURE9 pTex = m_vecImageList[i]->GetTexture();
// 					// 					pTex->LockRect( 0, &Rect, NULL, 0 ); 
// 					// 					LPBYTE	pPixel = (LPBYTE) Rect.pBits; 
// 					// 					D3DSURFACE_DESC desc;
// 					// 					pTex->GetLevelDesc( 0, &desc );
// 					// 					
// 					// // 					if(FAILED(D3DXCreateTexture(g_pD3dDev, desc.Width, desc.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex)))
// 					// // 					{
// 					// // 						return ;
// 					// // 					}
// 					// 					D3DLOCKED_RECT DestRect; 
// 					// //					pTex->LockRect(0, &DestRect, NULL,0); 
// 					// 					LPBYTE	pDestPixel = (LPBYTE) Rect.pBits; 
// 					// 					
// 					// 					for( int y = 0 ; y < desc.Height ; ++y )
					// // 					{
// 					// 						for( int x = 0 ; x < desc.Width ; ++x )
// 					// 						{
// 					// 							const BYTE* pSrcColor =  &pPixel[ ( x * 4 ) + y * ( desc.Width * 4 ) ];
// 					// 							float fValue = (float)pSrcColor[3]/2.55f;
// 					// 							BYTE* pDestColor =  &pDestPixel[ ( x * 4 ) + y * ( desc.Width * 4 ) ];
// 					// // 							pDestColor[ 0 ] = pSrcColor[0];
// 					// // 							pDestColor[ 1 ] = pSrcColor[1];
// 					// // 							pDestColor[ 2 ] = pSrcColor[2];
// 					// // 							//pDestColor[ 3 ] = pSrcColor[3];
// 					// 							
// 					// 							pDestColor[3] = (BYTE) (10.0f* sqrtf( fValue ) * 2.55f );
					// // 					}
// 					// 					}
// 					// 					pTex->UnlockRect(0);
// 					// 					//m_vecImageList[i]->GetTexture()->UnlockRect(0);
// 					// 					
// 					// 					//pd3dxSprite->Draw( pTex, m_vecImageList[i]->GetPtRect(), &(m_vecImageList[i]->GetCenteVector()), &(m_vecImageList[i]->GetTransVector()), m_vecImageList[i]->GetColor() );
// 					// 					//SAFE_RELEASE(pTex);
// // 					// end, texture의 알파값을 증가시켜 2회 블렌딩 후 오차를 줄인다. by dhkwon 2011.11.18
//  	 				m_vecImageList[i]->Render();
//  	 				((CINFImageEx*)m_vecImageList[i])->SetModify( FALSE );
					// 						}
					// 					}
// 			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
//  		}
//  		g_pD3dDev->EndScene();
// 	}
// 	g_pD3dDev->SetRenderTarget( 0, m_lpMainSurface );
	return S_OK;
}

void CINFGroupImage::UpdateTexture()
{
// 	if(m_lpRenderTexture && m_lpRenderSurface )
// 	{
// 		m_lpRenderTexture->Release();
// 		m_lpRenderTexture = NULL;
// 		m_lpRenderSurface->Release(); 
// 		m_lpRenderSurface = NULL; 
// 	}
// 	g_pD3dDev->CreateTexture( m_v2MaxPos.x - m_v2MinPos.x, m_v2MaxPos.y - m_v2MinPos.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_lpRenderTexture, NULL );
// 	if(m_lpRenderTexture)
// 			m_lpRenderTexture->GetSurfaceLevel( 0, &m_lpRenderSurface );
}

void CINFGroupImage::Render()
{
	if( m_vecImageList.size() > 0 )
	{
		for(int i=0; i < m_vecImageList.size(); i++)
		{
			D3DXVECTOR2 vec2Temp = ((CINFImageEx*)m_vecImageList[i])->GetTransVector();
			((CINFImageEx*)m_vecImageList[i])->Move( m_v2Trans.x + m_v2MinPos.x + m_vecImageList[i]->GetTransVector().x, m_v2Trans.y + m_v2MinPos.y + m_vecImageList[i]->GetTransVector().y );
			m_vecImageList[i]->Render();
			((CINFImageEx*)m_vecImageList[i])->Move( vec2Temp.x, vec2Temp.y );
			((CINFImageEx*)m_vecImageList[i])->SetModify( FALSE );
		}
	}
// 	if( m_pd3dxSprite && m_lpRenderTexture)
// 	{
// 		m_pd3dxSprite->Draw( m_lpRenderTexture, NULL, NULL, NULL, NULL, &m_v2Trans, 0xffffffff );
// 	}
}

BOOL CINFGroupImage::IsImageModify()
{
	for(int i=0; i < m_vecImageList.size(); i++)
	{
		if( ((CINFImageEx*)m_vecImageList[i])->GetModify() == TRUE )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CINFGroupImage::AttachImage( CINFImage* pImageInfo )
{
// 	if( pImageInfo->GetType() != CONTROL_TYPE )
// 	{
// 		m_vecImageList.push_back( pImageInfo );
// 	}
	m_vecImageList.push_back( pImageInfo );
}

void CINFGroupImage::DetachImage( CINFImage* pImageInfo )
{
}

void CINFGroupImage::Move( float x, float y )
{
	m_v2Trans = D3DXVECTOR2(x, y);
}

POINT CINFGroupImage::GetFindControlTargetofMinPos( char* nStrName )
{
	POINT temp = { 0, 0 };
	for(int i=0; i< m_vecImageList.size(); i++)
	{
		if(!strcmp(((CINFControl*)m_vecImageList[i])->GetTargetName(), nStrName))
		{	
			temp.x = ((CINFControl*)m_vecImageList[i])->GetMinPos().x;
			temp.y = ((CINFControl*)m_vecImageList[i])->GetMinPos().y;
			return temp;
		}
	}
	return temp;
}
POINT CINFGroupImage::GetFindControlTargetofMaxPos( char* nStrName )
{
	POINT temp = { 0, 0 };
	for(int i=0; i< m_vecImageList.size(); i++)
	{
		if(!strcmp(((CINFControl*)m_vecImageList[i])->GetTargetName(), nStrName))
		{	
			temp.x = ((CINFControl*)m_vecImageList[i])->GetMaxPos().x;
			temp.y = ((CINFControl*)m_vecImageList[i])->GetMaxPos().y;
			return temp;
		}
	}
	return temp;

}

POINT CINFGroupImage::GetImgSize()
{
	POINT pt;
	pt.x = GetMaxPos().x - GetMinPos().x;
	pt.y = GetMaxPos().y - GetMinPos().y;

	return pt;
}

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CINFGroupImage::SetColor(D3DCOLOR color)
{
	vector<CINFImage*>::iterator it = m_vecImageList.begin();
	for( ;it != m_vecImageList.end() ; it++)
	{
		(*it)->SetColor(color);
	}
}
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

