// SetupConfig.h: interface for the CSetupConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETUPCONFIG_H__D0B6C9A9_7710_4757_BC84_2F3D605D3A64__INCLUDED_)
#define AFX_SETUPCONFIG_H__D0B6C9A9_7710_4757_BC84_2F3D605D3A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CONFIG_SIZE_BUFF	512

typedef struct  {
	char *chTitle;		// 윈도우 인덱스
	char *chString;		// 윈도우 처리 우선순위
} SetupConfig;

class CSetupConfig  
{
public:
	CSetupConfig();
	virtual ~CSetupConfig();

	BOOL LoadSetupInfo(const char *configFile);
	void SaveSetupInfo(char* strconfigFile);
	
	BOOL GetSetupInfo(char* i_pTitle, char* o_pTxt);
	void SetSetupInfo(char* i_pTitle, char* i_pTxt);

	void AddSetupInfo(char* i_pTitle, char* i_pTxt);

	// 2008-11-13 by bhsohn 조이스틱 작업
	vector<SetupConfig*>	*GetSetupInfoIterator();
	char*					GetConfigFile();
	void ResetSetupInfo();
private:
	BOOL GetLineString(FILE* i_fp,char* o_pDst);	
	//void ResetSetupInfo();	

	char						m_strconfigFile[MAX_PATH];

	vector<SetupConfig*>		m_setupInfo;			// 퀘스트 완료를 위해 필요한 아이템 리스트

};

#endif // !defined(AFX_SETUPCONFIG_H__D0B6C9A9_7710_4757_BC84_2F3D605D3A64__INCLUDED_)
