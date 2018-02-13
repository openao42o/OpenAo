// LetterManager.h: interface for the CLetterManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LETTERMANAGER_H__49CE6B1C_95DC_45C9_8308_AF6D223922B8__INCLUDED_)
#define AFX_LETTERMANAGER_H__49CE6B1C_95DC_45C9_8308_AF6D223922B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;
class CLetterManager  
{
public:
	CLetterManager();
	virtual ~CLetterManager();

	void InitLetterManager(CFieldIOCPSocket *i_pFISock);

	void InitLetter(vectSLETTER_INFO * i_pVectLetterInfo);
	void InitAllLetter(vectSLETTER_INFO * i_pVectLetterInfo);
	void InsertLetter(SLETTER_INFO * i_pLetterInfo);
	void InsertAllLetter(SLETTER_INFO * i_pLetterInfo);
	
	BOOL DeleteLetter(UID64_t i_nLetterUID);
	BOOL DeleteAllLetter(UID64_t i_nAllLetterUID);
	BOOL ReadLetter(UID64_t i_nLetterUID);
	BOOL ReadAllLetter(UID64_t i_nAllLetterUID);
	void SendLetterList();
	void SendAllLetterList();
	void CheckOldLetter();
	void CheckOldAllLetter();
	void DeleteLetterForDB(UID64_t i_nLetterUID);
	void DeleteAllLetterForDB(UID64_t i_nAllLetterUID);
	void DeleteOldAllLetterForDB(UID64_t i_nAllLetterUID);
	void SendLetterInfo(UID64_t i_nLetterUID);
	void SendAllLetterInfo(UID64_t i_nAllLetterUID);


private:
	vectSLETTER_INFO		m_vectLetterInfo;
	vectSLETTER_INFO		m_vectAllLetterInfo;
	CFieldIOCPSocket		*m_pFieldIOCPSocket2;

};

#endif // !defined(AFX_LETTERMANAGER_H__49CE6B1C_95DC_45C9_8308_AF6D223922B8__INCLUDED_)
