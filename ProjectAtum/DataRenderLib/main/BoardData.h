// BoardData.h: interface for the CBoardData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOARDDATA_H__5C2A8862_C4CE_40BE_B4F5_A035D47A81BE__INCLUDED_)
#define AFX_BOARDDATA_H__5C2A8862_C4CE_40BE_B4F5_A035D47A81BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFImage;
class CD3DHanFont;

#define D3DFVF_BOARDVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

struct BOARDVERTEX
{
    D3DXVECTOR3 p;
    FLOAT       tu, tv;
};

struct stBOARDSTRING
{
	CD3DHanFont	*				pBoardFont;				// 폰트텍스쳐
	int							nBoardFontWidth;		// 폰트 텍스쳐 넓이
	int							nBoardFontHeight;		// 폰트 텍스쳐 높이
	
	INT							StringIndex;			// 전광판 스트링 인덱스

	BOOL						bUse;

	stBOARDSTRING()
	{
		pBoardFont = nullptr;
		nBoardFontWidth = 0;
		nBoardFontHeight = 0;

		StringIndex = 0;
		bUse = FALSE;
	}
};
typedef struct stBoard
{
	int							nBoardStringIndex;	// 보드 내용 인덱스
	BOARDVERTEX					vPos[4];
	LPDIRECT3DVERTEXBUFFER9		pVBBoard;
	vector<stBOARDSTRING *>		vecBoardString;		// 보드 내용
	float						fScrollMove;		// 이동 속도
	float						fCurrentMove;		// 현재 이동 위치
	int							nTexWidth;			// 기본 텍스쳐 넓이
	int							nTexHeight;			// 기본 텍스쳐 높이
	int							nTextCount;			// 보드 내용 갯수
	int							nRenderTextIndex;	// 현재 출력 내용
	BOOL						bBoardMove;			// 움직이는 보드인가

	stBoard()
	{
		for(int i = 0; i < 4; i++)
		{
			vPos[i].p = D3DXVECTOR3(0,0,0);
			vPos[i].tu = 0.0f;
			vPos[i].tv = 0.0f;
		}
		nBoardStringIndex = -1;
		vecBoardString.clear();
		fScrollMove = 0.0f;
		fCurrentMove = 0.0f;
		nTexWidth = 0;
		nTexHeight = 0;
		nTextCount = 0;
		nRenderTextIndex = 0;
		bBoardMove = FALSE;
	}

}ELECTRICBOARD;

class CBoardData  
{
private:
	
public:
	HRESULT						InitDeviceObjects(char * pTexName);
	HRESULT						RestoreDeviceObjects();
	HRESULT						DeleteDeviceObjects();
	HRESULT						InvalidateDeviceObjects();

	void						Tick(float fElapsedTime);
	void						Render(float* fOffset);

	int							GetBoardIndex() { return m_nBoardIndex; }
	void						SetBoardStringUse(int nBoardStringIndex, int nStringIndex, BOOL bUse);

	void						AddBoard(int nBoardStringIndex, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight, float fMoveRate, BOOL bMove = FALSE);
	void						AddBoardNumber(int nNumberCount, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight);
	void						AddBoardString(int nBoardStringIndex, const char* pFontName, int nFontSize, int nStringIndex, char* pStr);
	void						SetBoardNumberString(char* pFontName, int nFontSize, char* pStr);

	BOOL						DelBoardString(int nBoardStringIndex, int nStringIndex);

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	void						AddBoardWorldRank(int nNumberCount, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight);
	void						SetWorldRankBoardString(int i_nBoardStringIndex, int i_nX, const char* i_pFontName, int i_nFontSize, char* i_pStr, DWORD	i_dwColor);
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

private:
	void						CheckBoardStringUse();
	
private:
	BYTE						InfluenceMask0;		// 2006-04-17 by cmkwon, 세력 마스크
	
	int							m_nBoardIndex;		// 보드 인덱스
	BOOL						m_bBothFaces;
	DWORD						m_dwColor;

	BOARDVERTEX					m_vMainPos[4];
	LPDIRECT3DVERTEXBUFFER9		m_pVBMainBoard;
	LPDIRECT3DTEXTURE9			m_pMainBoardTex;	// 기본 바탕 텍스쳐

	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matTrans;
	D3DXMATRIX					m_matRotX;
	D3DXMATRIX					m_matRotY;
	D3DXMATRIX					m_matRotZ;

	vector<ELECTRICBOARD *>		m_vecElectricBoard;
	vector<ELECTRICBOARD *>		m_vecElectricBoardNumber;		// 숫자 전광판 오직 하나로 구성

	char *						m_pBoardTextureName;

public:
	CBoardData(int nBoardIndex, float fWidth, float fHeight, DWORD dwColor, D3DXVECTOR3 vCenter, float fRotX = 0.0f, float fRotY = 0.0f, float fRotZ = 0.0f, BOOL bBothFaces = FALSE);
	virtual ~CBoardData();

};

#endif // !defined(AFX_BOARDDATA_H__5C2A8862_C4CE_40BE_B4F5_A035D47A81BE__INCLUDED_)
