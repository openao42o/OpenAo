#include "SpriteDrawFix.h"

HRESULT SpriteDrawFix(
	LPD3DXSPRITE pSprite,				// The sprite
	LPDIRECT3DTEXTURE9 pSrcTexture,		// The texture to draw.
	CONST RECT* pSrcRect,				// The src rect 
	CONST D3DXVECTOR2* pScaling,		// Scaling
	CONST D3DXVECTOR2* pRotationCenter,	// The rotation center if rotation is used.
	FLOAT Rotation,						// Rotation in radians.
	CONST D3DXVECTOR2* pTranslation,	// Translation, i.e. moving the object to this place.
	D3DCOLOR Color)						// Color to module the image pixels.
{
	D3DXMATRIX m;

	D3DXMatrixTransformation2D(&m, nullptr, 0, pScaling, pRotationCenter, -Rotation, pTranslation);

	pSprite->SetTransform(&m);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	auto hr = pSprite->Draw(pSrcTexture, pSrcRect, nullptr, nullptr, Color); // 2016-01-08 exception at this point, GetIBaseTexture (7)
	pSprite->End();

	return hr;
}