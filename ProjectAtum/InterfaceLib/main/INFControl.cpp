// INFControl.cpp: implementation of the CINFControl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "INFControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFControl::CINFControl()
{
	m_bModify = FALSE;
}

CINFControl::~CINFControl()
{

}

HRESULT CINFControl::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CINFControl::RestoreDeviceObjects()
{
	return S_OK;
}

HRESULT CINFControl::DeleteDeviceObjects()
{
	return S_OK;
}

HRESULT CINFControl::InvalidateDeviceObjects()
{
	return S_OK;
}

void CINFControl::Render()
{
//
}