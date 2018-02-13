#include "StdAfx.h"
#include "ScreenShotManager.h"

#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "ScreenShotConverter.h"

#include <gdiplus.h>


#define SCREENSHOT_FILE_PATH			".\\ScreenShot\\"

#define SCREENSHOT_FONT_TOP_MARGIN		0.02
#define SCREENSHOT_FONT_RIGHT_MARGIN	0.10

CScreenShotManager::CScreenShotManager(CAtumApplication* mainApp, SCREENSHOT_TYPE type)
{
	m_ScreenShotNumber = 0;
	util::zero(m_PathTemplate, MAX_PATH);
	util::zero(m_NameTemplate, MAX_PATH);
	m_pAtumApp = mainApp;
	m_pScreenShotConverter = new CScreenShotConverter(type);
	m_Type = type;
}


CScreenShotManager::~CScreenShotManager()
{
	util::del(m_pScreenShotConverter);
}

void CScreenShotManager::Init(const char* nameStructure)
{
	// Set Name Structure
	sprintf_s(m_PathTemplate, MAX_PATH, "%s%s.bmp", SCREENSHOT_FILE_PATH, nameStructure);
	util::strncpy(m_NameTemplate, nameStructure, MAX_PATH);

	// Make sure the directory exists
	CreateDirectory(SCREENSHOT_FILE_PATH, NULL);

	// Find the next free Screen Shot Number
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	char checkTemplate[MAX_PATH];
	strcpy_s(checkTemplate, MAX_PATH, m_PathTemplate);
	modifyTypeExtension(checkTemplate);

	for (m_ScreenShotNumber = 1; m_ScreenShotNumber < 10000; m_ScreenShotNumber++)
	{
		char checkFile[MAX_PATH];
		sprintf_s(checkFile, MAX_PATH, checkTemplate, m_ScreenShotNumber);

		// Check if file exists already
		hFind = FindFirstFile(checkFile, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			// File does not exist yet, break
			break;
		}
		else
		{
			// File exists already
			FindClose(hFind);
		}
	}

	// Start Converter
	m_pScreenShotConverter->CreateThread();
}

void CScreenShotManager::TakeScreenShot(IDirect3DDevice9* device, int screenX, int screenY)
{
	// Copy DX Environment to file
	copyDXSurfaceToFile(device, screenX, screenY);

	// Notify the client
	char fileName[MAX_PATH];
	sprintf_s(fileName, MAX_PATH, m_NameTemplate, m_ScreenShotNumber);
	// Add extension
	size_t zeroChar = strlen(fileName);
	memset(&fileName[zeroChar], '.', 4);
	fileName[zeroChar + 4] = 0;
	modifyTypeExtension(fileName);
	// Notify Client
	m_pAtumApp->OnScreenShotCreated(fileName);

	// Convert the bmp if needed
	if (m_Type != SCREENSHOT_TYPE::BMP)
	{
		auto buffer = STRNBUF(MAX_PATH);
		convertScreenShot(getFileName(m_ScreenShotNumber, buffer), screenX, screenY);
	}

	// Increase Number for the next ScreenShot
	m_ScreenShotNumber++;
}

const char* CScreenShotManager::getFileName(UINT index, STRNBUF& buffer) const
{
	sprintf_s(buffer.GetBuffer(), buffer.GetSize(), m_PathTemplate, index);
	return buffer.GetBuffer();
}

void CScreenShotManager::copyDXSurfaceToFile(IDirect3DDevice9* device, int screenX, int screenY)
{
	IDirect3DSurface9* frontbuf; //this is our pointer to the memory location containing our copy of the
	//front buffer

	//now we create the image that our screen shot will be copied into
	//NOTE: Surface format of the front buffer is D3DFMT_A8R8G8B8 when it is returned
	device->CreateOffscreenPlainSurface(screenX, screenY, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &frontbuf, NULL);

	//now we copy the front buffer into our surface
	HRESULT hr = device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &frontbuf);

	//error checking
	if (hr != D3D_OK)
	{
		//do error handling etc...
		frontbuf->Release(); //release the surface so there is no memory leak
		return;
	}

	HDC surfaceDC;
	if (frontbuf->GetDC(&surfaceDC) == D3D_OK)
	{
		auto buffer = STRNBUF(512);

		// Draw the Additional Information to the Screen Shot
		drawInformationToSurface(getAdditionalInfo(buffer), surfaceDC, screenX, screenY);

		// Convert & Save the ScreenShot directly if it needs to be converted
		//if (m_Type != SCREENSHOT_TYPE::BMP)
		//{
		//	convertScreenShot(surfaceDC, screenX, screenY);
		//}

		// Release the device context
		frontbuf->ReleaseDC(surfaceDC);
	}
	else
	{
		DBGOUT("WARNING: Failed to create Screen Shot! Aquiring of the Device Contect of the temp surface was not successful");
	}

	auto buffer = STRNBUF(MAX_PATH);

	//now write our screen shot to a bitmap file
	//the last 2 params are NULL because we want the entire front buffer and no palette
	D3DXSaveSurfaceToFile(getFileName(m_ScreenShotNumber, buffer), D3DXIFF_BMP, frontbuf, NULL, NULL);
	
	// Release the surface buffer
	frontbuf->Release();
}

void CScreenShotManager::drawInformationToSurface(const char* information, HDC surfaceDC, int screenX, int screenY)
{
	// Set Char Set
	DWORD dwCharset = DEFAULT_CHARSET;
#ifdef LANGUAGE_VIETNAM
	dwCharset = VIETNAMESE_CHARSET;
#endif

	// Create Text Font
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_DEMIBOLD, TRUE,
		FALSE, FALSE, dwCharset, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH,
#ifdef LANGUAGE_VIETNAM
		VIETNAM_FONT
#else
		ENGLISH_FONT
#endif
		);

	if (!hFont)
	{
		DBGOUT("WARNING: Failed to create Screenshot Font, Screenshot wont have Additional Information!");
		return;
	}

	// Select Font
	HGDIOBJ oldFont = SelectObject(surfaceDC, hFont);

	// Prepare HDC
	SetBkColor(surfaceDC, 0xff000000);
	SetBkMode(surfaceDC, TRANSPARENT);
	SetTextAlign(surfaceDC, TA_TOP | TA_LEFT);
	SetTextColor(surfaceDC, GUI_FONT_COLOR_C);

	// Get expected Text Size
	SIZE size;
	int  iStringLength = lstrlen(information);
	GetTextExtentPoint32(surfaceDC, information, iStringLength, &size);

	// Calc Text coordinates
	int nPosX = screenX - size.cx - (screenX * SCREENSHOT_FONT_RIGHT_MARGIN);
	int nPosY = screenY * SCREENSHOT_FONT_TOP_MARGIN;

	// Draw the text
	ExtTextOut(surfaceDC,
		nPosX,
		nPosY,
		ETO_CLIPPED,
		NULL,
		information,
		strlen(information), NULL);

	// Delete the font resources
	SelectObject(surfaceDC, oldFont);
	DeleteObject(hFont);
}

void CScreenShotManager::convertScreenShot(const char* fileName, int screenX, int screenY)
{
	if (m_Type == SCREENSHOT_TYPE::BMP)
		return;

	m_pScreenShotConverter->addFile(fileName);
}

void CScreenShotManager::modifyTypeExtension(char* str)
{
	size_t len = strlen(str);

	if (len > 3)
	{
		switch (m_Type)
		{
		case SCREENSHOT_TYPE::BMP:
			str[len - 3] = 'b';
			str[len - 2] = 'm';
			str[len - 1] = 'p';
			break;
		case SCREENSHOT_TYPE::JPEG:
			str[len - 3] = 'j';
			str[len - 2] = 'p';
			str[len - 1] = 'g';
			break;
		case SCREENSHOT_TYPE::PNG:
			str[len - 3] = 'p';
			str[len - 2] = 'n';
			str[len - 1] = 'g';
			break;
		default:
			break;
		}
	}
}

const char* CScreenShotManager::getAdditionalInfo(STRNBUF& buffer)
{
	// Get current Time
	ATUM_DATE_TIME now { true };

	// Get Character Time
	const char* characterName;
	if (g_pShuttleChild)
	{
		characterName = g_pShuttleChild->m_myShuttleInfo.CharacterName;
	}
	else
	{
		characterName = "";
	}

	// Create output
	sprintf_s(buffer.GetBuffer(), 512, "%s %02d:%02d %02d-%02d-%04d %s"
		, STRMSG_WINDOW_TEXT
		, now.Hour
		, now.Minute
		, now.Day
		, now.Month
		, now.Year
		, characterName);

	// Return output
	return buffer.GetBuffer();
}
