// INFGroupManager.h: interface for the CINFGroupManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGROUPMANAGER_H__57FA8ECF_D1F7_46B8_86CE_63AB908DAB1E__INCLUDED_)
#define AFX_INFGROUPMANAGER_H__57FA8ECF_D1F7_46B8_86CE_63AB908DAB1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageFileHeader
{
public:
	CImageFileHeader() {}
	~CImageFileHeader() {}
public:
	int nVer;
	int ImageCount;
	int CtrlCount;
};

struct IMAGEFILEMAIN
{
	IMAGEFILEMAIN()
	{
		m_rRect.top = 0;
		m_rRect.left = 0;
		m_rRect.bottom = 0;
		m_rRect.right = 0;
		m_bType = -1;
		memset(m_sTargetName, 0x00, sizeof(m_sTargetName));
		memset(m_strName, 0x00, sizeof(m_strName));
	}
	D3DXVECTOR3			m_v3Trans;
	D3DXVECTOR3			m_v3Center;
	D3DXVECTOR3			m_v3Scaling;
	D3DXVECTOR3			m_v3ConMinPos;
	D3DXVECTOR3			m_v3ConMaxPos;
	
	D3DXVECTOR3			m_v3MaxPos;
	
	float				m_fAngle;
	float				m_fScalePercent;
	
	D3DXMATRIX			m_matMultiply;
	char				m_strName[100];
	char				m_sTargetName[100];
	int					m_bType;	
	
	RECT				m_rRect;
};
class CINFImageList;
class CINFGroupImage; 
class CINFGroupManager
{
public:
	CINFGroupManager( CINFImageList* pUIImageInfo, CGameData*	pInterfaceData );
	virtual ~CINFGroupManager();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	CINFGroupImage*  GetGroupImage( DataHeader* pGameData );
	DataHeader* FindResource(char* szRcName);
protected:
	CGameData*			m_pGameData;
	CGameData*			m_pInterfaceData;
	CINFImageList*		m_pUIImageInfo;
};

#endif // !defined(AFX_INFGROUPMANAGER_H__57FA8ECF_D1F7_46B8_86CE_63AB908DAB1E__INCLUDED_)
