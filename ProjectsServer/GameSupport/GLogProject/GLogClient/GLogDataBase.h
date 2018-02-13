#pragma once


class CGLogDataBase
{
public:
	CGLogDataBase(void);
	virtual ~CGLogDataBase(void);

	void Init(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand);

	virtual Err_t	GLogProcessing()			{return ERR_NO_ERROR;}		// GLog DB 업데이트 진행
	virtual Err_t	GLogDBSelect(int * o_pSelectCnt)				{return 0;}					// GLog DB 값 가져오기
	virtual Err_t	GLogDBInsert()				{return 0;}					// GLog DB 값 넣기
	virtual Err_t	GLogDBDelete()				{return 0;}					// GLog DB 값 지우기

	Err_t			ErrorLog(TCHAR * pTableName, _com_error e);		// DB 실행 에러 로그
	Err_t			ErrorProcessing(Err_t i_Err);					// DB 실행 에러 처리

	virtual	void	ResetData()					{}					// Select DB값 초기화
	virtual void	DBInsertParametersAppend(int i_nCnt) {}			// OleDB Paramter Append
	virtual void	DBInsertParametersDelete()			 {}			// OleDB Paramter Delete

	_RecordsetPtr 	m_pSourceRecordSet;
	_CommandPtr		m_pSourceCommand;
	_RecordsetPtr 	m_pDestRecordSet;
	_CommandPtr		m_pDestCommand;
};
