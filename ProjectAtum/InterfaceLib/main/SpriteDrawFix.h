#pragma once

#include "StdAfx.h"

HRESULT SpriteDrawFix(
	LPD3DXSPRITE pSprite,				// The sprite
	LPDIRECT3DTEXTURE9 pSrcTexture,		// The texture to draw.
	CONST RECT* pSrcRect,				// The src rect 
	CONST D3DXVECTOR2* pScaling,		// Scaling
	CONST D3DXVECTOR2* pRotationCenter,	// The rotation center if rotation is used.
	FLOAT Rotation,						// Rotation in radians.
	CONST D3DXVECTOR2* pTranslation,	// Translation, i.e. moving the object to this place.
	D3DCOLOR Color);					// Color to module the image pixels.
	