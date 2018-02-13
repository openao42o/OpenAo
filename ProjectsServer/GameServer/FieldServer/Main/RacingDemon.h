#pragma once

class CFieldMapChannel;

class CRacingDemon  
{
public:
	CRacingDemon() :
		m_bWorkerEndFlag { false },
		m_threadWorker { },
		m_mtvectorRacingMapChannelList { } { }

	virtual ~CRacingDemon() { CleanRacingDemon(); }

	bool InitRacingDemon();	
	void CleanRacingDemon();

	DWORD Worker();

	bool PutRacingMapChannel(CFieldMapChannel* i_racingMapChannel);
	void PopRacingMapChannel(CFieldMapChannel* i_racingMapChannel);
	CFieldMapChannel *FindRacingMapChannel(MAP_CHANNEL_INDEX i_mapChanIdx);

	bool MakeMessageRacingDemon(MessageType_t i_msgType, void* o_pMsg);

protected:

	bool							m_bWorkerEndFlag;
	thread							m_threadWorker;

	mt_vector<CFieldMapChannel*>	m_mtvectorRacingMapChannelList;
};
