// INFBase.h: interface for the CINFBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFBASE_H__945C37CC_0842_4739_98D3_F4085A935FCC__INCLUDED_)
#define AFX_INFBASE_H__945C37CC_0842_4739_98D3_F4085A935FCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

// 아레나 모드.
enum{
	ARENA_WAR_MODE_DEATHMATCH,				// 데스매치.
	ARENA_WAR_MODE_ROUND,					// 라운드.
	ARENA_WAR_MODE_END				
};
struct ST_WARINFO_OUTPOST: public SCITY_WARINFO_OUTPOST
{

//	BYTE			Influence;
//	MapIndex_t		MapIndex;
//	UID32_t			GuildUID;
	CINFImage *		pGuildMark;
//	CHAR			GuildCommander[SIZE_MAX_CHARACTER_NAME];
//	CHAR			GuildName[SIZE_MAX_GUILD_NAME];
//	ATUM_DATE_TIME	OutpostDate;

};



// 2010-06-15 by shcho&hslee 펫시스템 - 좌표 처리 관련 클래스 추가.

class CPosData
{

	public :

				 CPosData ();
		virtual ~CPosData ();

		void Reset ( void );

		void InitData ( const float a_fPosx , const float a_fPosy , const float a_fWid = .0f , const float a_fHgt = .0f , const bool a_bInitRect = false );
		void InitRect ( const bool a_bUpdate = false , const float a_fPosx = .0f , const float a_fPosy = .0f , const float a_fWidth = .0f , const float a_fHeight = .0f );

		bool IsRectOn ( POINT pt , const float a_fDstx = .0f , const float a_fDsty = .0f );

	public :  // inline Method - input process.

		inline void SetPosx ( const float a_fPosx ) { _fPosx = a_fPosx; }
		inline void SetPosy ( const float a_fPosy ) { _fPosx = a_fPosy; }

		inline void SetWidth ( const float a_fWidth ) { _fWid = a_fWidth; }
		inline void SetHeight ( const float a_fHeight ) { _fHgt = a_fHeight; }

		inline void SetPos ( const float a_fPosx , const float a_fPosy ) { _fPosx = a_fPosx; _fPosy = a_fPosy; }
		inline void SetSize ( const float a_fWid , const float a_fHgt ) { _fPosx = a_fWid; _fPosy = a_fHgt; }

	public :  // inline Method - return process.

		inline float rtn_Posx ( void ) { return _fPosx; }
		inline float rtn_Posy ( void ) { return _fPosy; }

		inline float rtn_Width ( void ) { return _fWid; }
		inline float rtn_Height ( void ) { return _fHgt; }

	private :

		float	_fPosx;
		float	_fPosy;

		float	_fWid;
		float	_fHgt;

		RECT	_rcRECT;

};

// End 2010-06-15 by shcho&hslee 펫시스템 - 좌표 처리 관련 클래스 추가.


class DataHeader;
class CGameData;
class CINFBase : public CAtumNode  
{
public:
	CINFBase();
	virtual ~CINFBase(); 

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetGameData(CGameData * pData);
	DataHeader* FindResource(char* szRcName) const;
	BOOL GetButtonStateOnMouse( POINT pt, int nX, int nY, int nSizeX, int nSizeY );

	void SetTex(float fTexRate) { m_fTexRate = fTexRate; }
	float GetTex() { return m_fTexRate;	}

public:
	CGameData	*	m_pGameData;
	float			m_fTexRate;			// 상점에서만 사용

};

#endif // !defined(AFX_INFBASE_H__945C37CC_0842_4739_98D3_F4085A935FCC__INCLUDED_)


