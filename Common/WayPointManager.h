#pragma once

class CAtumDBHelper;
class CODBCStatement;

class CWayPointMove : public SWAY_POINT_MOVE
{
public:
	CWayPointMove();
	CWayPointMove(UINT dwIndex, BYTE byCAction);
	virtual ~CWayPointMove();

	void Clean();

	INT GetSize();
	UINT GetIndex()	const { return dwIndex; }

	BOOL Add(SWAY_POINT *pData);
	
	void Sort();

	SWAY_POINT *GetWayPoint(int iStep);
	INT GetCompletionAction();

private:
	vectorWayPoint m_vectWayPointList;
};

typedef vector<CWayPointMove> vectorWayPointPattern;

class CWayPointManager
{
	friend CAtumDBHelper;
public:
	CWayPointManager();
	virtual ~CWayPointManager();

	void Clean();
	BOOL Create();
	void Release();

	BOOL LoadDBWayPointData(CODBCStatement *i_odbcstmt);

public:
	BOOL AddPattern(CWayPointMove *pData);
	BOOL AddWayPoint(SWAY_POINT *pWayPoint);

	SWAY_POINT *GetWayPointInfo(int iIndex, int iStep);
	CWayPointMove *GetWayPointPattern(int iIndex);

	INT GetCompletionAction(int iIndex);
	INT GetTotalCountPattern();		// 총 패턴 수
	INT GetTotalCountData();		// 모든 패턴들의 총 데이터 수
	
	BOOL IsCollision(int iIndex, int iStep, D3DXVECTOR3 vecPosition);
	BOOL IsLastStep(int iIndex, int iStep);
	BOOL IsFirstStep(int iStep);

	void Sort();

private:
	vectorWayPointPattern m_vectWPPatternList;
};
