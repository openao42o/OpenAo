#pragma once

enum class SCREENSHOT_TYPE
{
	BMP,
	JPEG,
	PNG
};

class CAtumApplication;
class CScreenShotConverter;
class CScreenShotManager
{
public:
	CScreenShotManager(CAtumApplication* mainApp, SCREENSHOT_TYPE type);
	~CScreenShotManager();

// Public Actions
public:
	void Init(const char* nameStructure);
	void TakeScreenShot(IDirect3DDevice9* device, int screenX, int screenY);

// Members
private:
	UINT m_ScreenShotNumber;
	SCREENSHOT_TYPE m_Type;

	char m_PathTemplate[MAX_PATH];
	char m_NameTemplate[MAX_PATH];

	CAtumApplication* m_pAtumApp;
	CScreenShotConverter* m_pScreenShotConverter;

// Helpers
private:
	const char* getFileName(UINT index, STRNBUF& buffer) const;
	const char* getAdditionalInfo(STRNBUF& buffer);

	void copyDXSurfaceToFile(IDirect3DDevice9* device, int screenX, int screenY);
	void drawInformationToSurface(const char* information, HDC surfaceDC, int screenX, int screenY);
	void convertScreenShot(const char* fileName, int screenX, int screenY);

	void modifyTypeExtension(char* str);
};

