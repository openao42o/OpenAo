// INFSelectBack.h: interface for the CINFSelectBack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSELECTBACK_H__B1CA5525_1D44_4C1D_A77C_E179BB1C5C84__INCLUDED_)
#define AFX_INFSELECTBACK_H__B1CA5525_1D44_4C1D_A77C_E179BB1C5C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SELECT_UNIT_NUMBER	4

#define ROTATE_NONE			0
#define ROTATE_LEFT			1
#define ROTATE_RIGHT		2

#include "AtumNode.h"

class CSkinnedMesh;
class CAppEffectData;
class CINFSelectBack : public CAtumNode  
{
public:
	CINFSelectBack(CAtumNode* pParent);
	virtual ~CINFSelectBack();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderCreateUnit();
	void RenderSelectUnit();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ChangeMode(int nMode);
	int HitTest(LPARAM lParam);
	int GetCurrentSelectIndex() { return m_nCurrentSelectUnit; }
	int GetRotateMode() { return m_nRotateMode; }
	USHORT GetCurrentUnitKind();
	void MoveSelect(int nSelect);

	// 2008-07-10 by bhsohn 셀렉트 화면 기체 높이 갱신
	void RefreshUnitMatrix();

protected:
	void InitRenderMatrix();

protected:
	BOOL			m_bRestored;
	CSkinnedMesh 	*m_pTopMesh;
	CSkinnedMesh 	*m_pMiddleMesh;
	CSkinnedMesh 	*m_pLocationMesh;
	CSkinnedMesh 	*m_pBottomMesh;

	D3DXVECTOR3		m_vCurrentCamPos;
	D3DXVECTOR3		m_vTargetCamPos;
	D3DXVECTOR3		m_vCurrentTargetPos;

	D3DXMATRIX		m_mSelectBottomMatrix;
	D3DXMATRIX		m_mSelectTopMatrix[SELECT_UNIT_NUMBER];
	D3DXMATRIX		m_mSelectMiddleMatrix[SELECT_UNIT_NUMBER];
	D3DXMATRIX		m_mSelectLocationMatrix;

	float			m_fCurrentRotationAngle;
	float			m_fTargetRotationAngle;
	float			m_fUnitRotationStartTime;
	int				m_nMode; // rendering될 유닛 개수로 쓰인다.
	int				m_nRotateMode;	// 회전하는 방향
	int				m_nCurrentSelectUnit; // 현재 선택된 유닛의 인덱스
	int				m_nOldSelectUnit;
	float			m_fCurrentSelectUnitRotateAngle;
	float			m_fOldSelectUnitRotateAngle;

	float			m_fTest;

	GUIUnitRenderInfo m_CreateRenderInfo[4];

// 2006-01-12 by ispark, Create 렌더 정보
	GUIUnitRenderInfo m_CreateOrgLWeaponInfo[4];		// 원본
	GUIUnitRenderInfo m_CreateOrgRWeaponInfo[4];
	GUIUnitRenderInfo m_CreateRenderLWeaponInfo[4];		// 계산된
	GUIUnitRenderInfo m_CreateRenderRWeaponInfo[4];

public:
	GUIUnitRenderInfo m_SelectRenderInfo[3];
	// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
	float			m_fTick[3];
	//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정

// 2006-01-12 by ispark, Select 렌더 정보
	GUIUnitRenderInfo m_SelectOrgLWeaponInfo[3][2];
	GUIUnitRenderInfo m_SelectOrgRWeaponInfo[3][2];
	GUIUnitRenderInfo m_SelectRenderLWeaponInfo[3][2];
	GUIUnitRenderInfo m_SelectRenderRWeaponInfo[3][2];

	bool			m_bInitEffectItem;
	CAppEffectData*	m_pBackEffect;
};

#endif // !defined(AFX_INFSELECTBACK_H__B1CA5525_1D44_4C1D_A77C_E179BB1C5C84__INCLUDED_)
