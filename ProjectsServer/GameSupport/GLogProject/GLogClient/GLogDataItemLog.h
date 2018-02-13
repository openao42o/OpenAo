
//////////////////////////////////
// 2012-10-08 bckim, GLog 3Â÷ Ãß°¡
// GLogDataItemLog
//////////////////////////////////

#pragma once
#include "glogdatabase.h"

class CGLogDataItemLog :	public CGLogDataBase
{
public:
	CGLogDataItemLog(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand);
	~CGLogDataItemLog(void);

	virtual Err_t	GLogProcessing();

private:
	virtual Err_t	GLogDBSelect(int * o_pSelectCnt);
	virtual Err_t	GLogDBInsert(int i_nCnt);
	virtual Err_t	GLogDBDelete(int i_nCnt);

	virtual	void	ResetData();
	virtual void	DBInsertParametersAppend(int i_nCnt);					// OleDB Paramter Append
	virtual void	DBInsertParametersDelete();								// OleDB Paramter Delete

	CItemLog	m_arrItemLog[GLOG_DATA_MAX_CNT];

};
