// UserVariable.h: interface for the CUserVariable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERVARIABLE_H__3D9D1260_795C_4A28_9F6D_F735BEBEA697__INCLUDED_)
#define AFX_USERVARIABLE_H__3D9D1260_795C_4A28_9F6D_F735BEBEA697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mt_stl.h"

#define SIZE_MAX_STRING_VALUE_NAME		256			// 2008-01-09 by cmkwon, 퀘스트 스크립트 전처리기 버퍼 수정 - 추가 정의 함 
#define SIZE_MAX_STRING_VALUE			2000		// 2008-01-09 by cmkwon, 퀘스트 스크립트 전처리기 버퍼 수정 - 수정함, SIZE_MAX_NPC_TALK_STRING 보다 크게 해야 한다.

enum EnumElementType
{
	ET_INTEGER = 0,
	ET_STRING = 1,
};

struct USER_VAR_ELEMENT
{
	char			UserVariableName[SIZE_MAX_STRING_VALUE_NAME];		// 2008-01-09 by cmkwon, 퀘스트 스크립트 전처리기 버퍼 수정 - 수정함(<--SIZE_MAX_STRING_VALUE)
	EnumElementType	Type;
	int				IntValue;
	char			StringValue[SIZE_MAX_STRING_VALUE];

	void Print()
	{
		printf("%s %d %d \"%s\"\n", UserVariableName, Type, IntValue, StringValue);
	}
};

class CUserVariable  
{
public:
	CUserVariable();
	virtual ~CUserVariable();

	USER_VAR_ELEMENT*	GetUserVariable(char *i_pUserVar);
	bool				AddUserVariableInteger(char *i_pUserVar, int i_intValue);
	bool				AddUserVariableString(char *i_pUserVar, char *i_strValue);

	// 디버깅용
	void Print();

private:
	ez_map<string, USER_VAR_ELEMENT*>		m_mapElement;
};

#endif // !defined(AFX_USERVARIABLE_H__3D9D1260_795C_4A28_9F6D_F735BEBEA697__INCLUDED_)
