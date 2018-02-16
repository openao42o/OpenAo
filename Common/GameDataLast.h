//Copyright[2002] MasangSoft
#pragma once

#ifdef bool
#error hhhhh
#endif

#define  MAXBUFF  65536

struct TotalHeader  
{
	int m_EncodeNum;
	int m_Identification;
	int m_FileSize;
	int m_DataNumber;
	int m_Parity;

	TotalHeader() : m_EncodeNum { 1000 }, m_Identification { 1000 }, m_FileSize { 0 }, m_DataNumber { 0 }, m_Parity { 0 } { }
};

struct DataHeader  
{
	int m_EncodeNum;
	int m_DataSize;
	int m_Parity;
	char m_FileName[10];
	char* m_pData;

	DataHeader() : m_EncodeNum { 0 }, m_DataSize { 0 }, m_Parity { 0 }, m_FileName { }, m_pData { nullptr } { }
	~DataHeader() { delete[] m_pData; m_pData = nullptr; }
};

class CGameData  
{
public:

	CGameData() :
		pTotal_header { nullptr }, m_ZipFilePath { }, m_EncodeStrFilePath { }, m_EncodeString { },
		m_bEncode { false }, m_mapDataHeader { }, m_it { m_mapDataHeader.begin() } { }

	virtual ~CGameData() { for (auto data : m_mapDataHeader) util::del(data.second); util::del(pTotal_header); }


	bool SetFile(char *filename, bool encode, char* encodeString, int encodeSize, bool bAllLoading = true);

	int GetTotalNumber() const { return pTotal_header->m_DataNumber; }
	
	const char *CGameData::GetZipFilePath() { return m_ZipFilePath; }

	// todo : (CGameData) replace usages of GetStartPosition and GetNext with AsEnumerable

	// Deprecated, these methods are ineffective, use CGameData::AsEnumerable() instead.
	[[deprecated("Use CGameData::AsEnumerable() instead.")]]
	DataHeader* GetStartPosition() { return (m_it = m_mapDataHeader.begin())->second; }

	// Deprecated, these methods are ineffective, use CGameData::AsEnumerable() instead.
	[[deprecated("Use CGameData::AsEnumerable() instead.")]]
	DataHeader* GetNext() { return (++m_it == m_mapDataHeader.end()) ? nullptr : m_it->second; }

	DataHeader* Find(const char* strName) { auto it = m_mapDataHeader.find(strName); return (it == m_mapDataHeader.end()) ? nullptr : it->second; }
	
	void SetEncode(bool encode) { m_bEncode = encode; }
	void SetEncodeString(char* szEncode, int size) { memcpy( m_EncodeString, szEncode, size); }
	
	DataHeader* FindFromFile(char* strName);

	static bool GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath);

	const map<string, DataHeader*>& AsEnumerable() const { return m_mapDataHeader; }

protected:
	TotalHeader* pTotal_header;
	char m_ZipFilePath[256];
	char m_EncodeStrFilePath[256];
	char m_EncodeString[256];
	bool m_bEncode;

	map<string, DataHeader*> m_mapDataHeader;
	map<string, DataHeader*>::iterator m_it;
	
	virtual bool make_parse_file_ext();
};


class CGameDataOpt : public CGameData
{
public:
	DataHeader* find_by_number(unsigned number);

protected:
	map<unsigned, DataHeader*> m_mapDataHeaderOpt;

	bool make_parse_file_ext() override;
};
