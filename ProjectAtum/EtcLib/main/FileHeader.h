// FileHeader.h: interface for the CFileHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEHEADER_H__DB4F19F3_BF29_4BE7_A530_664C99A4BBE4__INCLUDED_)
#define AFX_FILEHEADER_H__DB4F19F3_BF29_4BE7_A530_664C99A4BBE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum FILE_HEADERTYPE
{
	_NOTHING,
	_MAP
};

class CFileHeader  
{
public:
	CFileHeader();
	virtual ~CFileHeader();

	FILE_HEADERTYPE	m_dwFileType;
};

#endif // !defined(AFX_FILEHEADER_H__DB4F19F3_BF29_4BE7_A530_664C99A4BBE4__INCLUDED_)
