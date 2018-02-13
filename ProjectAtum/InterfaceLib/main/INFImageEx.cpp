// INFImageEx.cpp: implementation of the CINFImageEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFImageEx.h"
#include "AtumApplication.h"
#include "dxutil.h"
#include "INFImageList.h"
#include "SpriteDrawFix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageEx::CINFImageEx()
{
	m_fScalePercent = 0.0f;
	m_nType		= -1;
	m_nPixelX	=  0;		
	m_nPixelY	=  0;
	memset( m_strFileName, 0x00, sizeof(m_strFileName) );
	
	m_pImageList = NULL;
	m_bModify = FALSE;
	m_fcurrentTime = 0.0f;
	
	m_bAinmation = -1;
	m_fMaxScaleX = 0.0f;
	m_fMaxScaleY = 0.0f;
	m_nAniType = -1;
	m_nLine = LEFT_TOP_LINEUP;
	m_v2AniScaling = D3DXVECTOR2( 1.0f, 1.0f );
}

CINFImageEx::~CINFImageEx(){

}

void CINFImageEx::SetPixelXofScale( int PixelX )
{
	m_nPixelX = PixelX;
}

void CINFImageEx::SetPixelYofScale( int PixelY )
{
	m_nPixelY = PixelY;
}

HRESULT CINFImageEx::InitDeviceObjects( DataHeader* pGameData )
{
	m_pData = pGameData->m_pData;
	m_nDataSize = pGameData->m_DataSize;
	strcpy( m_strFileName, pGameData->m_FileName );
	m_v2Trans = D3DXVECTOR2( 0, 0);
	m_pImageList = g_pD3dApp->m_pImageList;
	m_pGameData = pGameData;
	strcpy( m_strFileName, pGameData->m_FileName );	
	return S_OK;
}

HRESULT CINFImageEx::RestoreDeviceObjects()
{
	if(!m_pData)
		return E_FAIL;
	if(g_pD3dApp->GetDirectSprite())
	{
		m_pd3dxSprite = g_pD3dApp->GetDirectSprite();
	}
	if(!m_pd3dxSprite)
	{ 
		D3DXCreateSprite(g_pD3dDev , &m_pd3dxSprite);
		m_bSpriteCrate = TRUE;
	}

	sTEXTUREINFO* pTextureInfo = m_pImageList->CreateImage( m_pGameData );
	
	if( pTextureInfo )
	{
		m_pTexture = pTextureInfo->lpTexture;
	}
	else
	{
		return E_FAIL;
	}

	m_poImgSize.x = pTextureInfo->srcInfo.Width;
	m_poImgSize.y = pTextureInfo->srcInfo.Height;	
	return S_OK;
}


HRESULT CINFImageEx::InvalidateDeviceObjects()
{
	if(m_bSpriteCrate)
 	{
		SAFE_RELEASE(m_pd3dxSprite);
	}
	m_pd3dxSprite = NULL;
	
	if(m_pImageList)
	{
		m_pImageList->DetachImage( m_pGameData );
		m_pTexture = NULL;
	}
	return S_OK;
}

HRESULT CINFImageEx::DeleteDeviceObjects()
{
	return S_OK;
}


void CINFImageEx::SetUVAnimation( DWORD x, DWORD y, float time )
{
	m_nAniType = UVANI_TYPE;
	m_ptImgBySize.x = x;
	m_ptImgBySize.y = y;
	
	m_dwFrameTotal = x * y;
	m_fGapFrameTime = time;
	m_fcurrentTime = 1;
	SetUVCurrentRect( 1 );
}

void CINFImageEx :: SetFrame ( DWORD dwFrame )
{
	if ( m_dwFrameTotal <= dwFrame )
		return;
	
	m_dwFrameCurrent = dwFrame;	
}

void CINFImageEx::Tick()
{
	if( m_fcurrentTime > 0 )
	{
		m_fcurrentTime -= g_pD3dApp->GetElapsedTime();
		if( (m_dwFrameCurrent == FALSE ) && (m_nAniType == UVANI_TYPE))
		{
			SetUVCurrentRect( 1 );
		}
	}
	else
	{
		
		if( m_nAniType == UVANI_TYPE )
		{
			m_dwFrameCurrent++;
			if(m_bLoop == TRUE && m_dwFrameCurrent > m_dwFrameTotal)
			{
				m_dwFrameCurrent = 0;
			}
			else if( m_bLoop == FALSE &&  m_dwFrameCurrent > m_dwFrameTotal)
			{
				m_dwFrameCurrent = m_dwFrameTotal;
			}
			m_fcurrentTime = m_fGapFrameTime;
			SetUVCurrentRect( m_dwFrameCurrent );
		}
		else if( m_nAniType == SCALE_TYPE )
		{
			if( m_bAinmation == -1 )
			{
				int aa =0;
			}
			if( m_bAinmation == BEGIN )
			{
				m_fcurrentTime = 0;
 				m_fScaleX = GetAniScale().x;
 				m_fScaleY = GetAniScale().y;
				m_bAinmation = MIDDLE;
			}
			if(m_bAinmation == MIDDLE)
			{
				if(m_fScaleY < m_fMaxScaleY && m_fScaleX < m_fMaxScaleX )
				{
					if( m_fScaleX < m_fMaxScaleX )
					{
						m_fScaleX += ( m_fMaxScaleX * m_fScaleX ) * ( g_pD3dApp->GetElapsedTime() / m_fPlayTime );
					}
					if( m_fScaleY < m_fMaxScaleY )
					{
						m_fScaleY += ( m_fMaxScaleY * m_fScaleY ) * ( g_pD3dApp->GetElapsedTime() / m_fPlayTime );
					}					
				}
				else
				{
					m_bAinmation = END;
				}
			}
			else if( m_bAinmation == END )
			{
				if(m_bLoop == TRUE)
				{
					m_bAinmation = BEGIN;
				}
				else
				{
					m_bAinmation = -1;
				}
			}
		}
	}
}

void CINFImageEx::SetUVCurrentRect( DWORD dwCurrentFrame )
{
	POINT pGap;
	pGap.x = m_poImgSize.x / m_ptImgBySize.x;
	pGap.y = m_poImgSize.y / m_ptImgBySize.y;
		
	int Num = 0;
	for( int i = 0; i < m_ptImgBySize.y; i++ )
	{
		for(int j = 0; j < m_ptImgBySize.x; j++)
		{
			Num++;
			if( dwCurrentFrame == Num )
			{
				float tempX = pGap.x * j;
				float tempY = pGap.y * i;
				float tempX1 = pGap.y * i + pGap.x;
				float tempY2 = pGap.y * i + pGap.y;
				SetRect( pGap.x * j, pGap.y * i , pGap.x * j + pGap.x, pGap.y * i + pGap.y );
				break;
			}
			
		}		
	}
}

void CINFImageEx::ReverseRender(BOOL bx, BOOL by)
{
	m_poImgSize.x;
	D3DXVECTOR2 vTemp(1,1);
	if(bx)
	{
		vTemp.x = -1.0f;
	}

	if(by)
	{
		vTemp.y = -1.0f;
	}

	if(m_pd3dxSprite && m_pTexture)
	{
		//m_pd3dxSprite->Draw( m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &m_v2Trans, m_dwColor );
		SpriteDrawFix(m_pd3dxSprite, m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &m_v2Trans, m_dwColor);
	}	
}

void CINFImageEx::SetScaleAnimation(int nType, float fPlayTime, float fScaleX, float fScaleY )
{
	m_nAniType = nType;
	m_fPlayTime = fPlayTime;
	m_fMaxScaleX = fScaleX;
	m_fMaxScaleY = fScaleY;
}

void CINFImageEx::Render()
{
	FLOG( "CINFImageEx::Render()" );
	if(m_pd3dxSprite && m_pTexture)
	{
		D3DXVECTOR2 vTemp(1.0f,1.0f), vTrans(0,0);
		if(m_nAniType == SCALE_TYPE )
		{

			if(m_fPlayTime  == 0)
			{
				vTemp.x = m_fMaxScaleX;
				vTemp.y = m_fMaxScaleY;
			}
			else
			{
				vTemp.x = m_fScaleX;
				vTemp.y = m_fScaleY;
			}
			vTrans = RenderLineUp( vTemp );
	
			//m_pd3dxSprite->Draw( m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &(m_v2Trans - vTrans), m_dwColor );
			SpriteDrawFix(m_pd3dxSprite, m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &m_v2Trans, m_dwColor);
		}
		else if (m_nAniType == UVANI_TYPE)
		{
			if(m_fPlayTime  == 0)
			{
				vTemp.x = m_v2Scaling.x;
				vTemp.y = m_v2Scaling.y;
			}
			else
			{
				vTemp.x = m_v2AniScaling.x;
				vTemp.y = m_v2AniScaling.y;
 			}
	
			//m_pd3dxSprite->Draw(m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &(m_v2Trans - vTrans), m_dwColor);
			SpriteDrawFix(m_pd3dxSprite, m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &m_v2Trans, m_dwColor);
		}
		else
		{	
 			if( m_v2AniScaling.x == 1 && m_v2AniScaling.y == 1 )
 			{
 				vTemp.x = m_v2Scaling.x;
 				vTemp.y = m_v2Scaling.y;
 			}
 			else
 			{
 				vTemp.x = m_v2AniScaling.x;
 				vTemp.y = m_v2AniScaling.y;
 			}
			//m_pd3dxSprite->Draw(m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &m_v2Trans, m_dwColor);
			SpriteDrawFix(m_pd3dxSprite, m_pTexture, m_pRect, &vTemp, &m_v2Rcenter, m_fAngle, &m_v2Trans, m_dwColor);
		}
	}
}

void CINFImageEx::Begin()
{
	m_bAinmation = 0;
}
void CINFImageEx::SetLine(int nLine)
{
	m_nLine = nLine;
}

D3DXVECTOR2 CINFImageEx::RenderLineUp( D3DXVECTOR2 vScale )
{
	D3DXVECTOR2 temp = D3DXVECTOR2( 0.0f, 0.0f );
	switch( m_nLine )
	{
	case LEFT_TOP_LINEUP:
		{
			temp.x = 0;
			temp.y = 0;
		}
		break;
	case LEFT_BOTTOM_LINEUP:
		{
			temp.y = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x));
		}
		break;
	case RIGHT_TOP_LINEUP:
		{
			temp.x = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x));			
		}
		break;
	case RIGHT_BOTTOM_LINEUP:
		{
			temp.x = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x));
			temp.y = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x));
		}
		break;
	case CENTER_TOP_LINEUP:
		{
			temp.x = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x) / 2);			
		}
		break;
	case CENTER_BOTTOM_LINEUP:
		{
			temp.x = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x) / 2);
			temp.y = (float)((vScale.x * m_poImgSize.x - m_v2AniScaling.x * m_poImgSize.x));
		}
		break;
	}
	//SetCenterVector(temp);
	return temp;
}
void CINFImageEx::SetAniScale(float fScaleX, float fScaleY)
{
	m_v2AniScaling.x = fScaleX; 
	m_v2AniScaling.y = fScaleY; 
}