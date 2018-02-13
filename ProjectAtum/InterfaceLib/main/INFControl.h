// INFControl.h: interface for the CINFControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCONTROL_H__33EFB1C3_8BB2_4572_A1BC_76A7D804D7AB__INCLUDED_)
#define AFX_INFCONTROL_H__33EFB1C3_8BB2_4572_A1BC_76A7D804D7AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFImage.h"

class CINFImage;
class CINFControl : public CINFImage
{
public:
	CINFControl();
	virtual ~CINFControl();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
 	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();

	void SetMinPos(D3DXVECTOR3 Pos) { m_vMinPos = Pos; }
	D3DXVECTOR3 GetMinPos() { return m_vMinPos;}
	
	void SetMaxPos(D3DXVECTOR3 Pos) { m_vMaxPos = Pos; }
	D3DXVECTOR3 GetMaxPos() { return m_vMaxPos;}
	
	void SetControlName(char* Name) { strncpy( m_chControlName, Name, sizeof(m_chControlName) ); }
	char* GetControlName() { return m_chControlName; }
	
	void SetTargetName(char* Name) { strncpy( m_chTargetName, Name, sizeof(m_chTargetName) ); }
	char* GetTargetName() { return m_chTargetName; }

	BOOL GetModify() { return m_bModify; }
protected:
	D3DXVECTOR3 m_vMinPos;
	D3DXVECTOR3 m_vMaxPos;
	
	char m_chControlName[256];
	char m_chTargetName[256];
	BOOL	m_bModify;
};

#endif // !defined(AFX_INFCONTROL_H__33EFB1C3_8BB2_4572_A1BC_76A7D804D7AB__INCLUDED_)
