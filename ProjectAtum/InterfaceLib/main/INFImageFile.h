// INFImageFile.h: interface for the CINFImageFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGEFILE_H__52056BAE_2C85_4648_9D34_0F57ADBF4842__INCLUDED_)
#define AFX_INFIMAGEFILE_H__52056BAE_2C85_4648_9D34_0F57ADBF4842__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFImage.h"

class CINFImageFile : public CINFImage  
{
public:
	CINFImageFile();
	virtual ~CINFImageFile();

//D3DXIMAGE_INFO &GetSrcInfo() { return m_srcInfo; }		// 2011. 10. 10 by jskim UI시스템 변경
	virtual HRESULT InitDeviceObjects(char* pData, int nSize);
	virtual HRESULT RestoreDeviceObjects();

protected:
	char m_szFileName[MAX_PATH];
//	D3DXIMAGE_INFO m_srcInfo;		// 2011. 10. 10 by jskim UI시스템 변경
};

#endif // !defined(AFX_INFIMAGEFILE_H__52056BAE_2C85_4648_9D34_0F57ADBF4842__INCLUDED_)
