// LetterManager.cpp: implementation of the CLetterManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LetterManager.h"
#include "AtumError.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLetterManager::CLetterManager()
{

}

CLetterManager::~CLetterManager()
{

}

void CLetterManager::InitLetterManager(CFieldIOCPSocket *i_pFISock)
{
	if(NULL == i_pFISock)
	{
		return;
	}

	m_vectLetterInfo.clear();
	m_vectAllLetterInfo.clear();
	m_pFieldIOCPSocket2 = i_pFISock;
}

void CLetterManager::InitLetter(vectSLETTER_INFO * i_pVectLetterInfo)
{
	if(NULL == i_pVectLetterInfo)
	{
		return;
	}

	m_vectLetterInfo.assign(i_pVectLetterInfo->begin(), i_pVectLetterInfo->end());
}

void CLetterManager::InitAllLetter(vectSLETTER_INFO * i_pVectLetterInfo)
{
	if(NULL == i_pVectLetterInfo)
	{
		return;
	}

	m_vectAllLetterInfo.assign(i_pVectLetterInfo->begin(), i_pVectLetterInfo->end());
}

void CLetterManager::InsertLetter(SLETTER_INFO * i_pLetterInfo)
{
	if(NULL == i_pLetterInfo)
	{
		return;
	}
	
	m_vectLetterInfo.push_back(*i_pLetterInfo);
}

void CLetterManager::InsertAllLetter(SLETTER_INFO * i_pLetterInfo)
{
	if(NULL == i_pLetterInfo)
	{
		return;
	}
	
	m_vectAllLetterInfo.push_back(*i_pLetterInfo);
}

BOOL CLetterManager::DeleteLetter(UID64_t i_nLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nLetterUID))
	{
		return FALSE;
	}

	vectSLETTER_INFO::iterator itr = m_vectLetterInfo.begin();
	while(itr != m_vectLetterInfo.end())
	{
		if(i_nLetterUID == itr->LetterUID)
		{
			this->DeleteLetterForDB(itr->LetterUID);
			itr = m_vectLetterInfo.erase(itr);
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

BOOL CLetterManager::DeleteAllLetter(UID64_t i_nAllLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nAllLetterUID))
	{
		return FALSE;
	}

	vectSLETTER_INFO::iterator itr = m_vectAllLetterInfo.begin();
	while(itr != m_vectAllLetterInfo.end())
	{
		if(i_nAllLetterUID == itr->LetterUID)
		{
			this->DeleteAllLetterForDB(itr->LetterUID);
			itr = m_vectAllLetterInfo.erase(itr);
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

BOOL CLetterManager::ReadLetter(UID64_t i_nLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nLetterUID))
	{
		return FALSE;
	}

	vectSLETTER_INFO::iterator itr = m_vectLetterInfo.begin();
	while(itr != m_vectLetterInfo.end())
	{
		if(i_nLetterUID == itr->LetterUID)
		{
			itr->CheckRead = TRUE;
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

BOOL CLetterManager::ReadAllLetter(UID64_t i_nAllLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nAllLetterUID))
	{
		return FALSE;
	}

	vectSLETTER_INFO::iterator itr = m_vectAllLetterInfo.begin();
	while(itr != m_vectAllLetterInfo.end())
	{
		if(i_nAllLetterUID == itr->LetterUID)
		{
			itr->CheckRead = TRUE;
			return TRUE;
		}
		itr++;
	}

	return FALSE;	
}

void CLetterManager::SendLetterList()
{
	this->CheckOldLetter();

	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;
	
	vectSLETTER_INFO::iterator itr = m_vectLetterInfo.begin();
	for (; itr != m_vectLetterInfo.end(); itr++)
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK) > SIZE_MAX_PACKET)
		{
			m_pFieldIOCPSocket2->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_CHAT_LETTER_REQUEST_TITLE_OK;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK *pRMsg	= (MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK*)(SendBuf + nSendBytes);
		pRMsg->LetterUID							= itr->LetterUID;
		pRMsg->CheckRead							= itr->CheckRead;
		pRMsg->SendDate								= itr->SendDate;
		util::strncpy(pRMsg->SendCharacterName, itr->SendCharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(pRMsg->Title, itr->Title, SIZE_MAX_LETTER_TITLE);
		nSendBytes									+= sizeof(MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK);
	}

	if(nSendBytes > 0)
	{
		m_pFieldIOCPSocket2->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	m_pFieldIOCPSocket2->SendAddMessageType(T_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE);	
}

void CLetterManager::SendAllLetterList()
{
	this->CheckOldAllLetter();

	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;
	
	vectSLETTER_INFO::iterator itr = m_vectAllLetterInfo.begin();
	for (; itr != m_vectAllLetterInfo.end(); itr++)
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK) > SIZE_MAX_PACKET)
		{
			m_pFieldIOCPSocket2->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK *pRMsg	= (MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK*)(SendBuf + nSendBytes);
		pRMsg->LetterUID							= itr->LetterUID;
		pRMsg->CheckRead							= itr->CheckRead;
		pRMsg->SendDate								= itr->SendDate;
		util::strncpy(pRMsg->SendCharacterName, itr->SendCharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(pRMsg->Title, itr->Title, SIZE_MAX_LETTER_TITLE);
		nSendBytes									+= sizeof(MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK);
	}

	if(nSendBytes > 0)
	{
		m_pFieldIOCPSocket2->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	m_pFieldIOCPSocket2->SendAddMessageType(T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE);	
}

void CLetterManager::CheckOldLetter()
{
	ATUM_DATE_TIME	CurTime;
	CurTime.SetCurrentDateTime();

	vectSLETTER_INFO::iterator itr = m_vectLetterInfo.begin();
	while(itr != m_vectLetterInfo.end())
	{
		if(LETTER_PASSED_RANGE_DAY * 24 < CurTime.GetTimeDiffTimeInHours(itr->SendDate) )
		{
			this->DeleteLetterForDB(itr->LetterUID);
			itr = m_vectLetterInfo.erase(itr);
			continue;
		}
		itr++;
	}
}
	
void CLetterManager::CheckOldAllLetter()
{
	ATUM_DATE_TIME	CurTime;
	CurTime.SetCurrentDateTime();

	vectSLETTER_INFO::iterator itr = m_vectAllLetterInfo.begin();
	while(itr != m_vectAllLetterInfo.end())
	{
		if(LETTER_PASSED_RANGE_DAY * 24 < CurTime.GetTimeDiffTimeInHours(itr->SendDate) )
		{
			this->DeleteOldAllLetterForDB(itr->LetterUID);
			itr = m_vectAllLetterInfo.erase(itr);
			continue;
		}
		itr++;
	}
}

void CLetterManager::DeleteLetterForDB(UID64_t i_nLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nLetterUID))
	{
		return;
	}

	QPARAM_DELETE_LETTER *pQParam	= new QPARAM_DELETE_LETTER;
	pQParam->LetterUID				= i_nLetterUID;
	pQParam->RecvCharacterUID		= m_pFieldIOCPSocket2->m_character.CharacterUniqueNumber;
	m_pFieldIOCPSocket2->ms_pFieldIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteLetter, m_pFieldIOCPSocket2, m_pFieldIOCPSocket2->m_character.AccountUniqueNumber, pQParam);
}

void CLetterManager::DeleteAllLetterForDB(UID64_t i_nAllLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nAllLetterUID))
	{
		return;
	}
	QPARAM_DELETE_READALLLETTER *pQParam	= new QPARAM_DELETE_READALLLETTER;
	pQParam->LetterUID				= i_nAllLetterUID;
	pQParam->RecvCharacterUID		= m_pFieldIOCPSocket2->m_character.CharacterUniqueNumber;
	m_pFieldIOCPSocket2->ms_pFieldIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteAllLetter, m_pFieldIOCPSocket2, m_pFieldIOCPSocket2->m_character.AccountUniqueNumber, pQParam);

}

void CLetterManager::DeleteOldAllLetterForDB(UID64_t i_nAllLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nAllLetterUID))
	{
		return;
	}
	QPARAM_DELETE_ALL_LETTER *pQParam	= new QPARAM_DELETE_ALL_LETTER;
	pQParam->AllLetterUID				= i_nAllLetterUID;
	m_pFieldIOCPSocket2->ms_pFieldIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteOldAllLetter, m_pFieldIOCPSocket2, m_pFieldIOCPSocket2->m_character.AccountUniqueNumber, pQParam);

}

void CLetterManager::SendLetterInfo(UID64_t i_nLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nLetterUID))
	{
		return;
	}

	vectSLETTER_INFO::iterator itr = m_vectLetterInfo.begin();
	while(itr != m_vectLetterInfo.end())
	{
		if(i_nLetterUID == itr->LetterUID)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_LETTER_READ_OK, T_FC_CHAT_LETTER_READ_OK, pSendData, SendBuf);
			pSendData->LetterUID		= i_nLetterUID;
			util::strncpy(pSendData->Content, itr->Content, SIZE_MAX_LETTER_CONTENT);
			m_pFieldIOCPSocket2->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHAT_LETTER_READ_OK));
			return;
		}
		itr++;
	}	

}

void CLetterManager::SendAllLetterInfo(UID64_t i_nAllLetterUID)
{
	if(FALSE == IS_VALID_UID64(i_nAllLetterUID))
	{
		return;
	}

	vectSLETTER_INFO::iterator itr = m_vectAllLetterInfo.begin();
	while(itr != m_vectAllLetterInfo.end())
	{
		if(i_nAllLetterUID == itr->LetterUID)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_ALLLETTER_READ_OK, T_FC_CHAT_ALLLETTER_READ_OK, pSendData, SendBuf);
			pSendData->LetterUID		= i_nAllLetterUID;
			util::strncpy(pSendData->Content, itr->Content, SIZE_MAX_LETTER_CONTENT);
			m_pFieldIOCPSocket2->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHAT_ALLLETTER_READ_OK));
			return;
		}
		itr++;
	}
}
