// PCBangIPManager.h: interface for the CPCBangIPManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBANGIPMANAGER_H__272B6144_5E92_413D_94B1_D31E0DEBE0C8__INCLUDED_)
#define AFX_PCBANGIPMANAGER_H__272B6144_5E92_413D_94B1_D31E0DEBE0C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPCBangIPManager  
{
public:
	CPCBangIPManager();
	virtual ~CPCBangIPManager();

	BOOL InitPCBangIPList();			// 2007-08-06 by dhjin, PC방 IP리스트 초기화 
	BOOL InsertPCBangIPList(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD);	// 2007-08-06 by dhjin, PC방 IP 추가
	BOOL IsPCBangIPClassCCheck(DWORD i_dwIPClassC);				// 2007-08-06 by dhjin, PC방 
	void InsertPCBangIPClassD(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD);		// 2007-08-06 by dhjin, PC방 IP D클래스 추가
	void SetPCBangIPList(mtmapDWORDvectorSIPClassD * i_pPCBangIPList);			// 2007-08-07 by dhjin, PC방 IP를 설정 요청 처리
	BOOL IsPCBangIPCheck(char *i_szClientIP, UID32_t *o_PCBangUID);
	mtmapDWORDvectorSIPClassD* GetPCBangIPList();

protected:
	mtmapDWORDvectorSIPClassD m_mtmapPCBangIPList;

};

#endif // !defined(AFX_PCBANGIPMANAGER_H__272B6144_5E92_413D_94B1_D31E0DEBE0C8__INCLUDED_)
