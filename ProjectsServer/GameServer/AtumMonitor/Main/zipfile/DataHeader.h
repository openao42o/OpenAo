// DataHeader.h: interface for the DataHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAHEADER_H__2DB340FD_5C59_4C5E_BA71_DEF168C2A1FB__INCLUDED_)
#define AFX_DATAHEADER_H__2DB340FD_5C59_4C5E_BA71_DEF168C2A1FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DataHeader  
{
public:
	int m_EncodeNum;
	int m_DataSize;
	int m_Parity;
	char m_FileName[10];
	DataHeader();
	~DataHeader();

};

#endif // !defined(AFX_DATAHEADER_H__2DB340FD_5C59_4C5E_BA71_DEF168C2A1FB__INCLUDED_)

