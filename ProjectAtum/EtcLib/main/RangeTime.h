#pragma once

class CRemainTime
{
protected:
	float m_fRemainTime;
	float m_fStartTime;
public:
	CRemainTime() : m_fRemainTime { 1.0f }, m_fStartTime { 1.0f } { }

	// ReSharper disable once CppNonExplicitConvertingConstructor
	CRemainTime(float fRemainTime) { Set(fRemainTime); }

	void Tick(float fElapsedTime)
	{
		if (m_fRemainTime < 0) return;

		m_fRemainTime -= fElapsedTime;
	}

	void Set(float fRemainTime)
	{
		if (fRemainTime <= 0.0f) fRemainTime = 0.0f;

		m_fStartTime = m_fRemainTime = fRemainTime;
	}

	float Get() const { return m_fRemainTime; }

	float GetRemainRate() const { return m_fRemainTime / m_fStartTime; }

	bool IsRemain() const { return m_fRemainTime > 0; }

};

class CRemainLoopTime : public CRemainTime
{
private:
	int m_nLoopCount;
public:
	CRemainLoopTime()
	{
		Set(1.0f, 1);
	}
	CRemainLoopTime(float fRemainTime, int nLoopCount = 1)
	{
		Set(fRemainTime, nLoopCount);
	}

	void Set(float fRemainTime, int nLoopCount = 1)
	{
		CRemainTime::Set(fRemainTime);
		m_nLoopCount = nLoopCount;
	}
	void Tick(float fElapsedTime)
	{
		if (m_fRemainTime > 0)
			m_fRemainTime -= fElapsedTime;
		else
		{
			if (m_nLoopCount > 0)
				m_nLoopCount--;
		}
	}
	BOOL IsNextTime()
	{
		if (m_fRemainTime > 0)
			return FALSE;
		else
		{
			if (m_nLoopCount > 0)
				return TRUE;
			else
				return FALSE;
		}

	}
	BOOL IsRemainLoop()
	{
		if (m_nLoopCount > 0)
			return TRUE;
		else
			return FALSE;
	}
	void StartNext()
	{
		float fRemainTime = m_fStartTime + m_fRemainTime;
		Set(fRemainTime, m_nLoopCount);
	}
};

/////////////////////////////////////////////////////////////////////////////////
///	\class		CRangeTime
/// \brief		Tick 호출시마다 m_fMovePosition은 FixStart부터 FixEnd 의 범위까지 
///				계속 움직인다.
/////////////////////////////////////////////////////////////////////////////////

class CRangeTime
{
private:
	float m_fFixStart;
	float m_fFixEnd;
	float m_fFixMid;
	float m_fTimer;

public:
	CRangeTime()
	{
		Set(0.0f, 1.0f, 0.6f);
	}

	CRangeTime(float fFixStart, float fFixEnd, float fMidRate)
	{
		Set(fFixStart, fFixEnd, fMidRate);
	}

	void Set(float fFixStart, float fFixEnd, float fMidRate)
	{
		m_fFixStart = fFixStart;
		m_fFixEnd = fFixEnd;
		SetMidPosition(fMidRate);
		m_fTimer = m_fFixStart;
	}
	void
		SetMidPosition(float fMidRate) { m_fFixMid = (m_fFixEnd - m_fFixStart) * fMidRate; }

	float Get() const { return m_fTimer; }

	float GetPositionRate() const
	{
		if (IsOverMiddle())
			
			return (m_fTimer - m_fFixMid) / (m_fFixEnd - m_fFixMid);

		return (m_fFixMid - m_fTimer) / (m_fFixMid - m_fFixStart);
	}
	float GetPositionRateInTotal() const { return (m_fTimer - m_fFixStart) / (m_fFixEnd - m_fFixStart); }

	bool IsOverMiddle() const { return m_fTimer > m_fFixMid; }

	bool IsUnderMiddle() const { return m_fTimer <= m_fFixMid; }

	bool IsOverEnd() const { return m_fTimer > m_fFixEnd; }

	void Start() { m_fTimer = m_fFixStart; }

	void Stop() { m_fTimer = m_fFixEnd + 0.001f; }

	void Tick(float fElapedTime)
	{
		if (m_fTimer > m_fFixEnd) return;

		m_fTimer += fElapedTime;
	}
};