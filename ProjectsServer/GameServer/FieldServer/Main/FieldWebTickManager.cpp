#include "stdafx.h"
#include "FieldWebTickManager.h"
#include "FieldWebIOCP.h"

CFieldWebTickManager::CFieldWebTickManager(CFieldWebIOCP *i_pFieldWebIOCP, DWORD i_nTickInterval /* = 1000 */)
: CTickManager(i_nTickInterval), m_pFieldWebIOCP1(i_pFieldWebIOCP)
{
}

CFieldWebTickManager::~CFieldWebTickManager()
{
}

void CFieldWebTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CFieldWebTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CFieldWebTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CFieldWebTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CFieldWebTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}
