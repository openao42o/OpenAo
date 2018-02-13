// TotalHeader.h: interface for the TotalHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOTALHEADER_H__7CA7EBD2_D3C4_4149_B850_1D43FC74BBC2__INCLUDED_)
#define AFX_TOTALHEADER_H__7CA7EBD2_D3C4_4149_B850_1D43FC74BBC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TotalHeader  
{
public:
	int m_EncodeNum;
	int m_Identification;
	int m_FileSize;
	int m_DataNumber;
	int m_Parity;

	TotalHeader();
	~TotalHeader();
};

#endif // !defined(AFX_TOTALHEADER_H__7CA7EBD2_D3C4_4149_B850_1D43FC74BBC2__INCLUDED_)

