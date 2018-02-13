// DissolutionItemmanager.h: interface for the DissolutionItemmanager class.
//
//////////////////////////////////////////////////////////////////////

#include "AtumParam.h"
#include "AtumFieldDBManager.h"

class CDissolutionItemManager  
{
public: //연관 클래스 정의
	friend CAtumDBHelper;

public:
	CDissolutionItemManager();
	virtual ~CDissolutionItemManager();

	void Initialize ( void );
	void Release ( void );
	// 용해 아이템 정보 로드
	BOOL LoadDBDissolutionItemData ( CODBCStatement *i_odbcstmt );
	
	// 용해 아이템 체크 함수 리턴값: 해당 용해 정보 데이터 리턴(Create 함수에서 사용하기 위해)
	tDissolutionItemInfo CheckDissolutionItem(INT i_itemnum);

	// 아이템 용해 함수 리턴값 : 용해 아이템 결과
	BOOL CreateDissolutionItem(INT i_itemnum, tDissolutionItemInfo i_DissolutionData, DissolutionItems* o_Dissolved_ItemData );

private:

	mtmapDissolutionItemInfo m_mapDissolutionItemData; // 용해 아이템을 저장할 컨테이너

};

