// RainData.h: interface for the CRainData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAINDATA_H__45B12F9B_BC6A_416B_8D94_BD1F5950EB00__INCLUDED_)
#define AFX_RAINDATA_H__45B12F9B_BC6A_416B_8D94_BD1F5950EB00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CRainData : public CAtumNode  
{
public:
	CRainData(D3DXVECTOR3 vPos, float fVelRate,D3DXVECTOR3 vVel);
	virtual ~CRainData();
	virtual void Tick();

	D3DXMATRIX			m_mMatrix;
	D3DXVECTOR3			m_vPos;						// Position
	D3DXVECTOR3			m_vVel;						// πÊ«‚
	D3DXVECTOR3			m_vUp;						// 
	FLOAT				m_fUpDistance;
	FLOAT				m_fVelRate;
};

#endif // !defined(AFX_RAINDATA_H__45B12F9B_BC6A_416B_8D94_BD1F5950EB00__INCLUDED_)
