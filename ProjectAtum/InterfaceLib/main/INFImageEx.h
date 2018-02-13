// INFImageEx.h: interface for the CINFImageEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGEEX_H__8C17917A_4CA7_4796_8912_F89477139D2D__INCLUDED_)
#define AFX_INFIMAGEEX_H__8C17917A_4CA7_4796_8912_F89477139D2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFImage.h"
#include "GameDataLast.h"
enum
{
	BEGIN,
	MIDDLE,
	END,
};

class CINFImageList;
class CINFImageEx : public CINFImage
{
public:
	CINFImageEx();
	virtual ~CINFImageEx();
	virtual HRESULT InitDeviceObjects( DataHeader* pGameData );
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	virtual	void Render();

	void	SetTransVector(D3DXVECTOR2 v2Trans ) { m_v2Trans = v2Trans; }

	float	GetScaleIsPercent() { return m_fScalePercent; }

	int		GetDataSize() { return m_nDataSize; }
	char*	GetData() { return m_pData; }

	void	SetPixelXofScale( int PixelX );
	void	SetPixelYofScale( int PixelY );
	int		GetPixelXScaleInfo() { return m_nPixelX; }
	int		GetPixelYScaleInfo() { return m_nPixelY; }

	void	SetType(int nType) { m_nType = nType; }
	int		GetType() { return m_nType; }

	void	SetModify( BOOL bflag ) { m_bModify = bflag; }
	BOOL	GetModify() { return m_bModify; }	

	DataHeader*	GetGameData() { return m_pGameData; }
	char*	GetFileName() { return m_strFileName; }	
	void	SetFileName( char* Name ) { strcpy( m_strFileName, Name ); }
 	void	SetCenterVector(D3DXVECTOR2 v2Center ) { m_v2Rcenter = v2Center; }
	void	SetAngle( float fAngle ) { m_fAngle = fAngle; }
 	void	SetScaleIsPercent( float fSize ) { m_fScalePercent = fSize; }
	void	SetUVAnimation( DWORD x, DWORD y, float time );
	void	SetLoop( BOOL flag ) { m_bLoop = flag; }
	void	SetFrame ( DWORD dwFrame );
	DWORD	GetFrame() { return m_dwFrameCurrent; }
	void	SetUVCurrentRect(DWORD dwCurrentFrame);
	void	ReverseRender(BOOL bx, BOOL by);
	int		GetAniType() { return m_nAniType; }

	// 애니메이션
	void	Begin();
	void	End() { m_bAinmation = END; }

	void	SetScaleAnimation(int nType, float fPlayTime, float fScaleX, float fScaleY );
	void SetLine(int nLine);
	D3DXVECTOR2	RenderLineUp( D3DXVECTOR2 vScale );
	void SetAniScale(float fScaleX, float fScaleY);
	D3DXVECTOR2 GetAniScale() { return m_v2AniScaling; }
	void SetTexture(LPDIRECT3DTEXTURE9 lpTexture) { m_pTexture = lpTexture; }
protected:
	DataHeader* m_pGameData;
	CINFImageList*	m_pImageList;

	float	m_fScalePercent;		
	int		m_nType;
	int		m_nPixelX;
	int		m_nPixelY;
	char	m_strFileName[256];
	
	BOOL	m_bModify;
	// UVAni
	POINT	m_ptImgBySize;
	DWORD	m_dwFrameTotal;
	DWORD	m_dwFrameCurrent;

	float	m_fcurrentTime;
	float	m_fGapFrameTime;
	BOOL	m_bLoop;
	
	float	m_fPlayTime;
	float	m_fMaxScaleX;
	float	m_fMaxScaleY;
	float	m_fScaleX;
	float	m_fScaleY;
	int		m_nAniType;

	int		m_bAinmation;
	int     m_nLine;
	D3DXVECTOR2 m_v2AniScaling;
};

#endif // !defined(AFX_INFIMAGEEX_H__8C17917A_4CA7_4796_8912_F89477139D2D__INCLUDED_)
