//////////////////////////////////
// 2011-12-12 by hskim, GLog 2Â÷

#pragma once
#include "glogdatabase.h"

class CGLogDataAceConnectUser : public CGLogDataBase
{
public:
	CGLogDataAceConnectUser(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand);
	~CGLogDataAceConnectUser(void);

	virtual Err_t	GLogProcessing();

private:
	virtual Err_t	GLogDBSelect(int * o_pSelectCnt);
	virtual Err_t	GLogDBInsert(int i_nCnt);
	virtual Err_t	GLogDBDelete(int i_nCnt);

	virtual	void	ResetData();
	virtual void	DBInsertParametersAppend(int i_nCnt);					// OleDB Paramter Append
	virtual void	DBInsertParametersDelete();								// OleDB Paramter Delete

	CAceConnectUserData	m_arrAceConnectUserData[GLOG_DATA_MAX_CNT];
};
