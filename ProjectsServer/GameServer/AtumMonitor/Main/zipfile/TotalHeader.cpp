// TotalHeader.cpp: implementation of the TotalHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumMonitor.h"
#include "TotalHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TotalHeader::TotalHeader()
{
	m_EncodeNum = 1000;
	m_Identification = 1000;
	m_FileSize = 0;
	m_DataNumber = 0;
	m_Parity = 0;
}

TotalHeader::~TotalHeader()
{

}
