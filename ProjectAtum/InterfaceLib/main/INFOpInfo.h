// INFOpInfo.h: interface for the CINFOpInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPINFO_H__7AB5C199_64C1_4ABC_A921_06993890D1A4__INCLUDED_)
#define AFX_INFOPINFO_H__7AB5C199_64C1_4ABC_A921_06993890D1A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageListBox.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

#define PET_MASSAGE					0		// 펫이 알려주는 상태이상 정보 메세지
#define PET_MAIN_MESSAGE			1		// 펫이 알려주는 메인 메세지 - 정보 메세지를 무시하는 우선권을 주어야 함( 죽음, 펫 레벨업, 최초 장착 )
#define	MAX_IMAGE					6		// 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현
#define	MAX_MODEL					1		// 2010-06-15 by shcho&hslee 펫시스템 - 팻의 분류

struct OP_INFO
{
	CINFImage* sImage;
	CINFImage* sBackImage;
	OperatorInfo* spOperatorInfo;
	float PlayCurrentTime;
	int x,y;
	BOOL live;
};

// type 1:  연료 부족, 2: 유도미사일 충돌, 3: 쉴드/에너지 다운, 4: 격추, 5: 레벨업, 6: 최초 장착
enum PET_OP
{
	NONE_STATE,
	DEADLINE_FEUL, 
	MISSILE_WARNING, 
	DEADLINE_HPMP, 
	USER_DEAD, 
	PET_LEVELUP, 
	EQUIP_PET 
};

class CINFOpMain;
class CINFOpInfo
{
public: 
	CINFOpInfo(CINFOpMain* pParent); 
	virtual ~CINFOpInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	
	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			OnKeyDown(WPARAM wParam, LPARAM lParam);
	void		SetOperatorAction( int Type, int FunctionIndex );
	OP_INFO*	GetOperatorAction( int Type, int FunctionIndex );
	BOOL		CheckView( int Type, int FunctionIndex, int SourceIndex = NULL );
	void		RenderString();

private:
	CINFOpMain*			m_pParent;
	vector<OP_INFO*>	m_vecOpInfo;
	vector<OP_INFO*>	m_vecCurrentOpInfo;
	int					m_nPetState;
	int					m_PlayTime;
	CD3DHanFont*		m_pFontOpInfo;
};

#endif // !defined(AFX_INFOPINFO_H__7AB5C199_64C1_4ABC_A921_06993890D1A4__INCLUDED_)
