//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: Frustum.cpp
// 
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Frustum.h"
#include "AtumApplication.h"
#include "Camera.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFrustum::CFrustum()
{
	g_pFrustum = this;
}

CFrustum::~CFrustum()
{
	g_pFrustum = NULL;
}


BOOL CFrustum::Construct(IDirect3DDevice9* pdevice, float ZDistance)
{
  D3DXMATRIX Matrix, matView, matProj;
  float      ZMin, Q;

  // Error checking
  if(pdevice == NULL )
    return FALSE;

  
  // Calculate FOV data
  pdevice->GetTransform(D3DTS_PROJECTION, &matProj);

  if(ZDistance != 0.0f) {
    // Calculate new projection matrix based on distance provided
    ZMin = -matProj._43 / matProj._33;
    Q = ZDistance / (ZDistance - ZMin);
    matProj._33 = Q;
    matProj._43 = -Q * ZMin;
  }
  
  pdevice->GetTransform(D3DTS_VIEW, &matView);

  D3DXMatrixMultiply(&Matrix, &matView, &matProj);

  // Calculate the planes
  m_Planes[0].a = Matrix._14 + Matrix._13; // Near
  m_Planes[0].b = Matrix._24 + Matrix._23;
  m_Planes[0].c = Matrix._34 + Matrix._33;
  m_Planes[0].d = Matrix._44 + Matrix._43;
  D3DXPlaneNormalize(&m_Planes[0], &m_Planes[0]);

  m_Planes[1].a = Matrix._14 - Matrix._13; // Far
  m_Planes[1].b = Matrix._24 - Matrix._23;
  m_Planes[1].c = Matrix._34 - Matrix._33;
  m_Planes[1].d = Matrix._44 - Matrix._43;
  D3DXPlaneNormalize(&m_Planes[1], &m_Planes[1]);

  m_Planes[2].a = Matrix._14 + Matrix._11; // Left
  m_Planes[2].b = Matrix._24 + Matrix._21;
  m_Planes[2].c = Matrix._34 + Matrix._31;
  m_Planes[2].d = Matrix._44 + Matrix._41;
  D3DXPlaneNormalize(&m_Planes[2], &m_Planes[2]);

  m_Planes[3].a = Matrix._14 - Matrix._11; // Right
  m_Planes[3].b = Matrix._24 - Matrix._21;
  m_Planes[3].c = Matrix._34 - Matrix._31;
  m_Planes[3].d = Matrix._44 - Matrix._41;
  D3DXPlaneNormalize(&m_Planes[3], &m_Planes[3]);

  m_Planes[4].a = Matrix._14 - Matrix._12; // Top
  m_Planes[4].b = Matrix._24 - Matrix._22;
  m_Planes[4].c = Matrix._34 - Matrix._32;
  m_Planes[4].d = Matrix._44 - Matrix._42;
  D3DXPlaneNormalize(&m_Planes[4], &m_Planes[4]);

  m_Planes[5].a = Matrix._14 + Matrix._12; // Bottom
  m_Planes[5].b = Matrix._24 + Matrix._22;
  m_Planes[5].c = Matrix._34 + Matrix._32;
  m_Planes[5].d = Matrix._44 + Matrix._42;
  D3DXPlaneNormalize(&m_Planes[5], &m_Planes[5]);

  return TRUE;
}

BOOL CFrustum::CheckPoint(float XPos, float YPos, float ZPos)
{
  short i;

  // Make sure point is in frustum
  for(i=0;i<6;i++) {
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XPos, YPos, ZPos)) < 0.0f)
      return FALSE;
  }

  return TRUE;
}

BOOL CFrustum::CheckCube(float XCenter, float YCenter, float ZCenter, float Size)
{
  short i;

  // Make sure at least one point is completely in frustum
  for(i=0;i<6;i++) {
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter-Size, ZCenter-Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter-Size, ZCenter-Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter+Size, ZCenter-Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter+Size, ZCenter-Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter-Size, ZCenter+Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter-Size, ZCenter+Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-Size, YCenter+Size, ZCenter+Size)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+Size, YCenter+Size, ZCenter+Size)) >= 0.0f)
      continue;

    return FALSE;
  }

  return TRUE;
}

BOOL CFrustum::CheckRectangle(float XCenter, float YCenter, float ZCenter, float XSize, float YSize, float ZSize)
{
  short i;

  // Make sure at least one point is in frustum
  for(i=0;i<6;i++) {
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter-YSize, ZCenter-ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter-YSize, ZCenter-ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter+YSize, ZCenter-ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter+YSize, ZCenter-ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter-YSize, ZCenter+ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter-YSize, ZCenter+ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter-XSize, YCenter+YSize, ZCenter+ZSize)) >= 0.0f)
      continue;
    if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter+XSize, YCenter+YSize, ZCenter+ZSize)) >= 0.0f)
      continue;

    return FALSE;
  }

  return TRUE;
}

bool CFrustum::CheckSphere(const D3DXVECTOR3* vCenter, float Radius)
{
	for (int i = 0; i < 6; i++) if (D3DXPlaneDotCoord(&m_Planes[i], vCenter) < -Radius) return false;

	return true;
}

bool CFrustum::CheckSphere(float XCenter, float YCenter, float ZCenter, float Radius)
{
	for (int i = 0; i < 6; i++) if (D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(XCenter, YCenter, ZCenter)) < -Radius) return false;
	
	return true;
}


///////////////////////////////// BOX IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This determines if a BOX is in or around our frustum by it's min and max points
/////
///////////////////////////////// BOX IN FRUSTUM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

#define V_IN 1
#define V_OUT 2
#define V_INTERSECT 3

int CFrustum::BoxInFrustum( float x, float y, float z, float x2, float y2, float z2)
{
	// Detailed explanation on the BSP tutorial 4
	BYTE mode=0;					// set IN and OUT bit to 0
	for(int i = 0; i < 6; i++ )
	{
		mode &= V_OUT;				// clear the IN bit to 0 

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;  // set IN bit to 1
		else 
			mode |= V_OUT; // set OUT bit to 1

		if (mode == V_INTERSECT)
			continue;   // if we found a vertex IN for THIS plane and 
						// a vertex OUT of ANY plane continue ( we have enough information to say: 
						// INTERSECT! IF there is not vertex missing from the FRONT of the remaining planes)
		
		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else 
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		if(m_Planes[i].a * x  + m_Planes[i].b * y  + m_Planes[i].c * z  + m_Planes[i].d >= 0)
			mode |= V_IN;
		else
			mode |= V_OUT;
		if (mode == V_INTERSECT)
			continue;

		// if we arrive to this point, then there are two possibilities:
		// there is not vertices in or there is not intersection till know, if 
		// there is a vertice in, continue (we are not over!) 
		if (mode == V_IN) continue;

		// there is not vertex IN front of this plane, so the box is COMPLETE_OUT
		return COMPLETE_OUT;
	}

	// All planes has a vertex IN FRONT so or the box is intersecting or complete IN
	if (mode == V_INTERSECT)
		return INTERSECT;
	else
		return COMPLETE_IN;
}


