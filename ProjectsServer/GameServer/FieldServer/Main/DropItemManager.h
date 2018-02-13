#ifndef _ATUM_DROP_ITEM_MANAGER_H_
#define _ATUM_DROP_ITEM_MANAGER_H_

class CFieldIOCP;
class CFieldIOCPSocket;
class CAtumFieldDBManager;
class CFieldMapChannel;

/*

// defined in "MapBlock.h"

struct DROPITEM
{
	ITEM_GENERAL		*pItem;						// new로 생성되어 할당됨!
	PRIORITY_INFO		priorityInfoArray[SIZE_MAX_DROP_ITEM_PRIORITY_INFO];
	int					priorityInfoCount;
	DWORD				dwStartTick;				// miliseconds
	D3DXVECTOR3			Position;
	BOOL				bProcessing;

public:
	DROPITEM()
	{
		pItem				= NULL;
		util::zero(priorityInfoArray, SIZE_MAX_DROP_ITEM_PRIORITY_INFO*sizeof(PRIORITY_INFO));
		priorityInfoCount	= 0;
		dwStartTick			= 0;
		Position.x			= 0;
		Position.y			= 0;
		Position.z			= 0;
		bProcessing			= FALSE;
	}
};
*/

typedef UINT ItemFieldIndex_t;

class CDropItemManager  
{
public:
	CDropItemManager();

	void Reset();
	BOOL InitDropItemManager(CFieldIOCP *i_pFieldIOCP, CAtumFieldDBManager	*i_pFieldDBManager);
	void CleanDropItemManager();

	BOOL DropItem(CFieldMapChannel *i_pFieldMapChannel, D3DXVECTOR3 *i_pDropPosition, INT i_nItemNum, INT i_nCountableItemCount = 1, BOOL i_bAdminCommand = FALSE);		// 2013-04-10 by hskim, 어드민 명령에 의한 드랍 아이템은 대기 시간 2초 적용
	BOOL DropItemInRandomRadius(CFieldMapChannel *i_pFieldMapChannel, D3DXVECTOR3 *i_pDropPosition, INT i_nRadius, INT i_nItemNum, INT i_nCountableItemCount = 1, BOOL i_bAdminCommand = FALSE);		// 2013-04-10 by hskim, 어드민 명령에 의한 드랍 아이템은 대기 시간 2초 적용

private:
	CFieldIOCP			*m_pFieldIOCP9;
	CAtumFieldDBManager	*m_pFieldDBManager;
};

#endif // _ATUM_DROP_ITEM_MANAGER_H_
