///////////////////////////////////////////////////////////////////////////////
// ImageManager Class 2005-04-28 by ydkim
/* Map 형식 기본구조에 모든 이미지 저장

	  * 이미지 타입 종류는 3개로 셀렉트, 게임, 글로벌로 지정한다	
	  * 이미지 지정은 기본으로 define과 IMAGEPUSHBACK으로 한다 (이미지 저장은
	     m_mapImgBank으로 지정
	  * 기타 세부 설정이나 임의 설정 부분은 차후 추가......
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIMAGEMANAGER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_CIMAGEMANAGER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class DataHeader;
class CGameData;

typedef enum
{
	IMAGE_START,						// 셀렉트 화면에서만 쓰이는 이미지
	IMAGE_MAIN,							// 실제 게임에서만 쓰이는 이미지
	IMAGE_GLOBAL						// 셀렉트와 메인에서 다쓰이는 이미지
}ImageType;

typedef struct {
	DWORD		dwImageType;
	CINFImage*	pINFImage;

	float		fAngle;					// 이미지 회전
	float		fImgMagnification;		// 이미지 배율
	int			nGamma;					// 이미지 감마
}ImageInfo;

class CImageManager: public CINFBase
{

public:
	CImageManager();
	virtual ~CImageManager();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects(DWORD dwType);
	virtual HRESULT DeleteDeviceObjects(DWORD dwType);
	virtual HRESULT InvalidateDeviceObjects(DWORD dwType);

	virtual void Render();
//	virtual void Tick();
//	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void		SetSpinImage(float fAng);						// 이미지 회전 설정
	void		SetMagnificationImage(float fImgMag);			// 이미지 배율 설정
	void		SetGamma(int nGam);								// 이미지 감마 설정
	
	int			AddImage(char *strImageName, DWORD dwType);		// 이미지 저장 성공시 TRUE, 실패시 FALSE 리턴
	int			DeleteImage(char *strImageName);				// 이미지 삭제 성공시 TRUE, 실패시 FALSE 리턴
	int			DeleteTypeImage(DWORD dwType);
	
	CINFImage*	SerchImageToFileName(char *strImageName);		// 실패시 FALSE 성공시 이미지 리턴
	int			SetGameStateInit( DWORD dwType );


protected:
	map<DWORD, ImageInfo>		m_mapImgBank;					// 이미지 저장을 위한 저장공간

public:
};

#endif
