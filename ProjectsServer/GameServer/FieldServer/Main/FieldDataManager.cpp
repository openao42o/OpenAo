#include "stdafx.h"
#include "FieldDataManager.h"
#include "FieldIOCPSocket.h"

CFieldIOCP	*CFieldDataManager::ms_pFieldIOCP2 = NULL;



CFieldDataManager::CFieldDataManager()
{
	m_pFieldIOCPSocket	= NULL;
	m_pFieldDBManager	= NULL;
	m_pCharacter		= NULL;
}

// DBManager 생성 이후 호출해야 함
void CFieldDataManager::InitFieldDataManager(CFieldIOCPSocket *i_pFieldIOCPSocket)
{
	m_pFieldIOCPSocket	= i_pFieldIOCPSocket;
	m_pFieldDBManager	= ms_pFieldIOCP2->m_pAtumDBManager;
	m_pCharacter		= &i_pFieldIOCPSocket->m_character;
}
