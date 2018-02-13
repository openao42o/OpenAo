// AtumDataLoader.h: interface for the CAtumDataLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMDATALOADER_H__86859305_F2F3_4DA7_A7E6_674DDCF802FA__INCLUDED_)
#define AFX_ATUMDATALOADER_H__86859305_F2F3_4DA7_A7E6_674DDCF802FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WINSOCKAPI_ // stop windows.h from loading winsock stuff because it gets loaded again
#include <windows.h>

#include "DefineGlobal.h"
#include "AtumParam.h"

#ifdef _ATUM_SERVER
#include "DbgOut_Field.h"
#define _loader_print DBGOUT
#else // _ATUM_SERVER
#define _loader_print printf
#endif // _ATUM_SERVER

struct INT_RANGE {
	INT		Min;
	INT		Max;

	void Reset() { Min = 0; Max = MAX_INT_VALUE; }
};

struct QUEST_PAY_ITEM {
	INT		ItemNum;
	INT		Count;
	INT		PerItem;		// 이 아이템 1개당 ItemNum을 Count개 지급
	BOOL	ToBeAttached;	// 습득 즉시 장착될 것인가?
	USHORT	ToUnitKind;		// 어떤 기어에게 줄 것인가? UNITKIND_XXX

	void Reset() { ItemNum = 0; Count = 0; PerItem = 0; ToBeAttached = FALSE; ToUnitKind = UNITKIND_ALL_MASK; }
};

#define COUNT_RANDOM_PAY_ITEM	10

struct QUEST_RANDOM_PAY_ITEM {
	ITEM_W_COUNT_PROB	ItemWithCountProb[COUNT_RANDOM_PAY_ITEM];
	INT					NumOfItems;
	INT					PerItemNum;		// 이 아이템 1개당 Item을 지급

	void Reset()
	{
		for (int i = 0; i < COUNT_RANDOM_PAY_ITEM; i++)
		{
			ItemWithCountProb[i].Reset(); 
		}
		NumOfItems = 0;
		PerItemNum = 0;
	}
};

// 2007-03-14 by cmkwon, AtumParam.h로 이동함
//struct MAP_AREA {
//	INT		MapIndex;
//	INT		X;
//	INT		Y;
//	INT		Radius;
//
//	BOOL IsPositionInArea(INT i_nMapIndex, INT i_nX, INT i_nY)
//	{
//		// map 확인
//		if (MapIndex == 0) {							return TRUE; }
//		if (MapIndex != i_nMapIndex) {					return FALSE; }
//
//		// 거리 확인
//		if (X == -1 || Y == -1 || Radius == -1){		return TRUE; }
//		if ((INT)util::length(X-i_nX, Y-i_nY) <= Radius)
//		{
//			return TRUE;
//		}
//		else
//		{
//			return FALSE;
//		}
//
//		return TRUE;
//	}
//
//	void Reset() { MapIndex = 0; X = 0; Y = 0; Radius = 0; }
//};

struct ITEM_W_COUNT_MAP_AREA {
	ITEM_W_COUNT	ItemWithCount;
	MAP_AREA		MapArea;
	INT				MonsterUniqueNumber;

	void Reset() { ItemWithCount.Reset(); MapArea.Reset(); MonsterUniqueNumber = 0; }
};

struct QUEST_NPC_INFO
{
	INT		NPCIndex;								// 해당 NPC 인덱스
	char	NPCName[SIZE_MAX_NPC_NAME];				// 담당 NPC 이름
	char	PreTalk[SIZE_MAX_NPC_TALK_STRING];		// 퀘스트 시작 시 NPC Talk
	char	MidTalk[SIZE_MAX_NPC_TALK_STRING];		// 퀘스트 수행 중 중간 NPC Talk
	char	FailTalk[SIZE_MAX_NPC_TALK_STRING];		// 퀘스트 수행 실패 시 NPC Talk
	char	SuccessTalk[SIZE_MAX_NPC_TALK_STRING];	// 퀘스트 수행 성공 시 NPC Talk

	void Reset()
	{
		NPCIndex = 0;
		memset(NPCName, 0x00, SIZE_MAX_NPC_NAME);
		memset(PreTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
		memset(MidTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
		memset(SuccessTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
		memset(FailTalk, 0x00, SIZE_MAX_NPC_TALK_STRING);
	}
};

#endif // !defined(AFX_ATUMDATALOADER_H__86859305_F2F3_4DA7_A7E6_674DDCF802FA__INCLUDED_)
