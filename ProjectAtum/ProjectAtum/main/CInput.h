
#if !defined(_C_INPUT_H_)
#define _C_INPUT_H_


/*----------------------------------------------------------------------------*/ 
int GetCharsetFromLang(LANGID);
int	GetCodePageFromLang( LANGID langid );

/*----------------------------------------------------------------------------*/ 
// 03/03/2006 by ispark
class CTelexKey  
{
private:
	char				m_ChatMsg[256];
	UINT32				m_ChatMsgIndex[256];

	int					m_ChatMsgCount;
	BOOL				m_bWCharacter;
protected:
	int					IsKeyPass(UINT32 nIsKey);
	void				SetVietKey(UINT nKey, UINT KeyValue);
	int					SetVietKeyPass1(UINT32 nKey1, UINT32 nKey2, UINT KeyValue);
	int					SetVietKeyPass2(UINT32 nKey2);
	int					SetVietKeyPass3(int nKeyIndex, UINT32 nKey2, UINT32 nMASK);
	void				SetDefaultkey(UINT KeyValue);

	char *				FindChar(int index);
	
	void				KeyEncoder(UINT KeyValue, int nLanguageConversion);

public:
	void				KeyDecoder(wchar_t wChar);

	void				ResetTelexString();
	int					ConvertStringVietnam(char* text, int len);
	void				Init();
	int					TelexKeyProc(HWND hWnd, WPARAM wParam, LPARAM lParam, int nLanguageConversion);

	CTelexKey();
	virtual ~CTelexKey();
	
};

// 2007-03-02 by dgwoo Telex 모드 and VNI 모드 추가.
class CVNIKey  
{
private:
	char				m_ChatMsg[256];
	UINT32				m_ChatMsgIndex[256];

	int					m_ChatMsgCount;
	BOOL				m_bWCharacter;
protected:
	int					IsKeyPass(UINT32 nIsKey);
	void				SetVietKey(UINT nKey, UINT KeyValue);
	int					SetVietKeyPass1(UINT32 nKey1, UINT32 nKey2, UINT KeyValue);
	int					SetVietKeyPass2(UINT32 nKey2);
	int					SetVietKeyPass3(int nKeyIndex, UINT32 nKey2, UINT32 nMASK);
	void				SetDefaultkey(UINT KeyValue);

	char *				FindChar(int index);
	
	void				KeyEncoder(UINT KeyValue, int nLanguageConversion);

public:
	void				KeyDecoder(wchar_t wChar);

	void				ResetTelexString();
	int					ConvertStringVietnam(char* text, int len);
	void				Init();
	int					TelexKeyProc(HWND hWnd, WPARAM wParam, LPARAM lParam, int nLanguageConversion);

	CVNIKey();
	virtual ~CVNIKey();
	
};
/*----------------------------------------------------------------------------*/ 
class CInput
{
public:
	CInput();
	~CInput();

	void		InitInput();

	/* messages */ 
	void		OnInputLanguageChange(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	bool		OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam, int nLanguageConversion = 0);

	int			GetInput(char* text, int len, int nLanguageConversion = 0);
	int			GetComp(char* text, int len);
	
	int			GetReading(char* text, int len);

	int			GetCandidate(DWORD index, char* text, int len);
	int			GetCandidateCount();
	int			GetCandidateSelection();
	int			GetCandidatePageSize();
	int			GetCandidatePageStart();
	void		GetUnderLine(int* start, int* end);

	/* property */ 
	bool		IsVerticalReading(){ return m_bVerticalReading; }
	bool		IsVerticalCandidate(){ return m_bVerticalCandidate; }
	WORD		GetCodePage() { return m_codePage; }
	LANGID		GetLangId() { return m_langId; }
	int			GetCharSet() { return GetCharsetFromLang(m_langId); }
	wchar_t*	GetIndicator() { return m_wszCurrIndicator; }
	int			GetImeState() { return m_ImeState; }
	void		SetImeState(int nState) { m_ImeState = nState; }
	void		ResetFontBuffer();									// 03/03/2006 by ispark

	void		SetInputWideChar(wchar_t wChar);

	HKL			GetHKL() { return m_hkl; }
	void		CheckToggleState(HWND hWnd);

	// 2007.04.24 by bhsohn China IME Working	
	void		UpdateReadingString();

	// 2007-06-11 by bhsohn China IME Working
	int 		SetCandidatePage(BOOL bAdd);
	int			SetCandidate(int nSelCandidate);	
	int			OnIMEKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 2007-09-05 by bhsohn China IME Working
	void		SetImeChangeTime(DWORD dwTime);
	DWORD		GetImeChangeTime();

	// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
	void SetImeRtnValue(BOOL bValie);
	BOOL	GetImeRtnValue();

protected:
	void		SetupImeApi(HWND hWnd);
	void		GetImeId();
	bool		GetReadingWindowOrientation();
	void		GetPrivateReadingString(HWND hWnd);

	// by bhsohn 2007-03-12 China Working
	wchar_t		ConvertChinaString(wchar_t temp);
	void		CheckOneChinaString(wchar_t* temp, int tempSize);
	void		CheckAllChinaString(wchar_t* temp, int tempSize);
	

protected:
	wstring				m_input;
	wstring				m_comp;
	wstring				m_reading;
	vector<BYTE>		m_candidate;

	int					m_ulStart;
	int					m_ulEnd;

	HKL					m_hkl;
	LANGID				m_langId;
	WORD				m_codePage;

	bool				m_bUnicodeIME;
	bool				m_bVerticalReading;
	bool				m_bVerticalCandidate;
	int					m_ImeState;
	wchar_t*			m_wszCurrIndicator;

	DWORD				m_dwId[2];

	HINSTANCE			m_hDllIme;

	UINT				(WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	BOOL				(WINAPI * _ShowReadingWindow)( HIMC, BOOL );

	// 03/03/2006 by ispark
	CTelexKey			m_VietNam;//CTelexKey
	CVNIKey				m_VietVNI;//CVNIKey

	// 2007-06-11 by bhsohn China IME Working
	int					m_nCandidatePage;			
	BOOL				m_bShowCandidatePage;			
	BOOL				m_bCandidateException;			
	
	// 2007-06-13 by bhsohn China IME Working
	BOOL				m_bABC50Exception;
	char				m_szCompExceptiontext[256];

	// 2007-09-05 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
	DWORD			m_dwImeChangeTime;
#endif

	// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
	BOOL			m_bZeroRtn;
};

/*----------------------------------------------------------------------------*/ 


#endif //_C_INPUT_H_