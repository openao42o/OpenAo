// DataHeader.cpp: implementation of the DataHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumMonitor.h"
#include "DataHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataHeader::DataHeader()
{
	m_EncodeNum = 0;
	m_DataSize = 0;
	m_Parity = 0;
	memset(m_FileName,0x00,sizeof(m_FileName));
}

DataHeader::~DataHeader()
{

}
