// INFImageList.h: interface for the CINFImageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGELIST_H__881A3BFD_34F1_4007_B609_F536E6C1AE85__INCLUDED_)
#define AFX_INFIMAGELIST_H__881A3BFD_34F1_4007_B609_F536E6C1AE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include "GameDataLast.h"

struct sTEXTUREINFO
{
	sTEXTUREINFO()
	{
		lpTexture = NULL;
		nCount = 0;
		memset(strPath, 0x00, sizeof(strPath));
	}
	LPDIRECT3DTEXTURE9 lpTexture;
	D3DXIMAGE_INFO		srcInfo;
	int					nCount;
	DataHeader*			pGameData;
	char				strPath[MAX_PATH];
};

class CINFImageList  
{
public:
	CINFImageList();
	virtual ~CINFImageList();

protected:
	vector< pair< string, sTEXTUREINFO* > > m_vecTextureInfo;

public:
	sTEXTUREINFO* AddImage( DataHeader* pGameData, LPDIRECT3DTEXTURE9 lpTexture, D3DXIMAGE_INFO& srcInfo );
	BOOL DelImage( sTEXTUREINFO* pTextureInfo );

	sTEXTUREINFO*	FindImage( DataHeader* pGameData );
	sTEXTUREINFO*	FindImage( char* strName );

	void AttachImage( DataHeader* pGameData );
	void DetachImage( DataHeader* pGameData );
	sTEXTUREINFO* CreateImage( DataHeader* pGameData );

	vector< pair< string, sTEXTUREINFO* > >& GetImgaeInfo() { return m_vecTextureInfo; }

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

#endif // !defined(AFX_INFIMAGELIST_H__881A3BFD_34F1_4007_B609_F536E6C1AE85__INCLUDED_)
