#pragma once

namespace VMemPool
{
	BOOL vmPoolAddObject(DWORD dwObjTypeSize, DWORD dwObjPoolSize = 1024);
	BOOL vmPoolInit();
	void vmPoolClean();
	void vmPoolSaveCurrentStatus();		// 2009-05-04 by cmkwon, 메모리풀 상태정보 로그 남기기 - 

	void* vmObjectNew(DWORD dwObjSize);
	void vmObjectDelete(void *p, DWORD dwObjSize);
};
