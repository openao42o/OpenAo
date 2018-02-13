// UserVariable.cpp: implementation of the CUserVariable class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ATUM_DATA_LOADER
#include "stdafx.h"
#endif // _ATUM_DATA_LOADER

#include "UserVariable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserVariable::CUserVariable()
{

}

CUserVariable::~CUserVariable()
{
	for (const auto &p : m_mapElement) delete p.second;

	m_mapElement.clear();
}

USER_VAR_ELEMENT* CUserVariable::GetUserVariable(char *i_pUserVar)
{
	return m_mapElement.findEZ(i_pUserVar);
}

bool CUserVariable::AddUserVariableInteger(char *i_pUserVar, int i_intValue)
{
	if (m_mapElement.findEZ(i_pUserVar) != nullptr) return false;

	auto pTmp = new USER_VAR_ELEMENT;
	memset(pTmp, 0x00, sizeof(USER_VAR_ELEMENT));

	// 2008-01-09 by cmkwon, 퀘스트 스크립트 전처리기 버퍼 수정 - 수정함(<--SIZE_MAX_STRING_VALUE)
	strncpy(pTmp->UserVariableName, i_pUserVar, SIZE_MAX_STRING_VALUE_NAME-1);
	pTmp->Type = ET_INTEGER;
	pTmp->IntValue = i_intValue;
	pTmp->StringValue[0] = '\0';

	m_mapElement.insertEZ(i_pUserVar, pTmp);

	return true;
}

bool CUserVariable::AddUserVariableString(char *i_pUserVar, char *i_strValue)
{
	if (NULL != m_mapElement.findEZ(i_pUserVar))
	{
		return false;
	}

	USER_VAR_ELEMENT *pTmp = new USER_VAR_ELEMENT;
	memset(pTmp, 0x00, sizeof(USER_VAR_ELEMENT));

	// 2008-01-09 by cmkwon, 퀘스트 스크립트 전처리기 버퍼 수정 - 수정함(<--SIZE_MAX_STRING_VALUE)
	strncpy(pTmp->UserVariableName, i_pUserVar, SIZE_MAX_STRING_VALUE_NAME-1);
	pTmp->Type = ET_STRING;
	pTmp->IntValue = ~0;
	strncpy(pTmp->StringValue, i_strValue, SIZE_MAX_STRING_VALUE-1);

	m_mapElement.insertEZ(i_pUserVar, pTmp);

	return true;
}

void CUserVariable::Print()
{
	for (const auto &p : m_mapElement) p.second->Print();
}
