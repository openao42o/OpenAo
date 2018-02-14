// ChatBlockManager.h: interface for the CChatBlockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATBLOCKMANAGER_H__0D41A9DF_9A0A_4E5F_B5E7_1D53A60C594C__INCLUDED_)
#define AFX_CHATBLOCKMANAGER_H__0D41A9DF_9A0A_4E5F_B5E7_1D53A60C594C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SChatBlockInfo
{
	char		szBlockedCharacterName[SIZE_MAX_CHARACTER_NAME];
	DWORD		dwChatBlockedTick;				
	DWORD		dwExpireTick;					// timeGetTime()함수 사용
};

typedef mt_map<string, SChatBlockInfo*>			mtmapChatBlockInfoPtr;
typedef vector<SChatBlockInfo*>					vectChatBlockInfoPtr;		// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 

class CChatBlockManager  
{
public:
	CChatBlockManager();
	~CChatBlockManager();

	void InsertChatBlockList(char *i_szCharacterName, int i_nBlockTermMinutes);
	void DeleteChatBlockList(char *i_szCharacterName);
	BOOL IsExistChatBlockList(int *o_pRemainMinute, char *i_szCharacterName);
	
	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - CChatBlockManager::CheckChatBlockList()에 인자 추가(vectChatBlockInfoPtr *o_pvectReleasedBlockInfoPtr=NULL)
	void CheckChatBlockList(vectChatBlockInfoPtr *o_pvectReleasedBlockInfoPtr=NULL);

protected:
	mtmapChatBlockInfoPtr		m_mtmapChatBlockInfoPtr;
};

#endif // !defined(AFX_CHATBLOCKMANAGER_H__0D41A9DF_9A0A_4E5F_B5E7_1D53A60C594C__INCLUDED_)
