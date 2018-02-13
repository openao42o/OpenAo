#include "StdAfx.h"
#include "ScreenShotManager.h"
#include "ScreenShotConverter.h"

#include <gdiplus.h>

DWORD CScreenShotConverter::Run()
{
	while (!MustStop())	// 2015-07-08 Future, added proper Shutdown of threads
	{
		// Convert one file once every loop
		m_FilesToConvert.lock();
		auto itr = m_FilesToConvert.begin();
		if (itr != m_FilesToConvert.end())
		{
			string fileNameToConvert = *itr;
			m_FilesToConvert.erase(itr);
			m_FilesToConvert.unlock();

			convertFile(fileNameToConvert);
		}
		else
			m_FilesToConvert.unlock();

		Sleep(1000);	// Check for new files every second
	}

	return 0;
}

void CScreenShotConverter::convertFile(const string& fileName)
{
	// Convert file name to unicode for GDI+
	char curFileDir[MAX_PATH];
	strcpy_s(curFileDir, MAX_PATH, fileName.c_str());
	wchar_t* wa = new wchar_t[MAX_PATH];
	mbstowcs(wa, curFileDir, MAX_PATH);

	// Create GDI+ Image
	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(wa);
	CLSID clsid;

	// Compression Quality
	Gdiplus::EncoderParameters encoderParameters;
	ULONG quality;

	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &quality;

	// Set target file extension and encoder clsid
	size_t len = wcslen(wa);
	switch (m_TargetType)
	{
	case SCREENSHOT_TYPE::JPEG:
		GetEncoderClsid(L"image/jpeg", &clsid);
		wa[len - 3] = L'j';
		wa[len - 2] = L'p';
		wa[len - 1] = L'g';
		break;
	case SCREENSHOT_TYPE::PNG:
		GetEncoderClsid(L"image/png", &clsid);
		wa[len - 3] = L'p';
		wa[len - 2] = L'n';
		wa[len - 1] = L'g';
		break;
	default:
		break;
	}

	// Convert & Save
	for (quality = 90; quality > 0; quality -= 5)
	{
		if (bmp->Save(wa, &clsid, &encoderParameters) == Gdiplus::Ok)
			break;
	}
	delete bmp;

	// Delete the bmp
	DeleteFile(curFileDir);
}

void CScreenShotConverter::addFile(string fileName)
{
	m_FilesToConvert.pushBackLock(fileName);
}

int CScreenShotConverter::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}