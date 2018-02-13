// INFCommunityReject.h: interface for the CINFCommunityReject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYREJECT_H__CAB71D99_F963_4DCD_AB22_05510CEE28CC__INCLUDED_)
#define AFX_INFCOMMUNITYREJECT_H__CAB71D99_F963_4DCD_AB22_05510CEE28CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CINFScrollBar;
class CD3DHanFont;
class CINFArenaScrollBar;
class CINFImageBtn;
class CINFEditBox;
class CINFGroupImage;
class CINFImageEx;

typedef struct
{
	char			szRejectName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	atimeRegTime;			// 거부 리스트 등록일자
}Reject_t;

#define MAX_REJECT_NUMBER					9	// 화면상 라인 수
#define MAX_REJECT_LIST						50  // 저장할수 있는 최대 친구 수

class CINFCommunityReject : public CINFBase  
{
public:
	CINFCommunityReject(CAtumNode* pParent);
	virtual ~CINFCommunityReject();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(POINT ptPos);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	void AddRejectList(Reject_t struRej);
	void InitRejectList();
	
	void DeleteRejectList(char *RejectName);
	void SendDeleteRejectList();

	void InitEditBox();
	void UpdateBtnPos();
private:
	void TestDBRejectList();		// Test용 
	void RenderRejectList(POINT ptPos);
	void OnClickReject();			//	거부 등록 
protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*			m_pBack;
#else								
	CINFImageEx*            m_pBack;
#endif
	CINFImageEx*			m_pImgSelect;
	//CINFImage*				m_pImgButton[4];

	CINFArenaScrollBar*			m_pScroll;	
	CD3DHanFont				*m_pFontReject[MAX_REJECT_NUMBER];

	BOOL					m_bAddRejectClick;
	BOOL					m_bDeleteRejectClick;	
	
	int						m_nRejectSelect;
	int						m_nButtonState;	

	int						m_nRejectRenderX;
	int						m_nRejectRenderY;

	CINFImageBtn*				m_pRegisterReject;	// 등록
	CINFImageBtn*				m_pDelReject;		// 삭제
	CINFEditBox*				m_pINFUserName;

public:
	//BOOL					m_bDeleteOK;
	vector<Reject_t>		m_vecReject;
};

#endif // !defined(AFX_INFCOMMUNITYREJECT_H__CAB71D99_F963_4DCD_AB22_05510CEE28CC__INCLUDED_)
