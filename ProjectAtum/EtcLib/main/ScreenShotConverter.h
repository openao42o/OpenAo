#pragma once

#include "AtumThread.h"

class CScreenShotConverter : public CAtumThread
{
public:
	explicit CScreenShotConverter(SCREENSHOT_TYPE targetType) : CAtumThread { }, m_TargetType { targetType } { }

	DWORD Run() override;
	void addFile(string fileName);

private:
	mt_vector<string> m_FilesToConvert;
	SCREENSHOT_TYPE m_TargetType;

	// Helpers
	void convertFile(const string& fileName);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};

