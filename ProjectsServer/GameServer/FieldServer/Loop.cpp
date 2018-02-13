#include "StdAfx.h"
#include "FieldIOCPSocket.h"

 CFieldIOCPSocket::syncdata::~syncdata()
 {
	 if (running())
	 {
		 data->pleaseDel = true;
		 data->pleaseStop = true;
	 }
	 else delete data;
 }

void DPLoop(CFieldIOCPSocket* socket, CFieldIOCPSocket::loopdata* data)
{
	data->running = true;
	data->pleaseStop = false;
	data->pleaseDel = false;

	while (!data->pleaseStop)
	{
		auto updated = false;

		if (socket->m_character.CurrentHP < socket->m_character.HP)
		{
			socket->m_character.CurrentHP += data->hpamount;
		
			socket->m_character.CurrentHP = min(socket->m_character.CurrentHP, socket->m_character.HP);

			updated = true;
		}

		if (socket->m_character.CurrentDP < socket->m_character.DP)
		{
			socket->m_character.CurrentDP += data->dpamount;
		
			socket->m_character.CurrentDP = min(socket->m_character.CurrentDP, socket->m_character.DP);

			updated = true;
		}

		if (!updated) break;

		socket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);

		this_thread::sleep_for(chrono::milliseconds(data->interval));
	}

	data->running = false;

	if (data->pleaseDel) delete data;
}

bool CFieldIOCPSocket::CheckDPLoop()
{
	return false;

	if (dploopdata.running()) return false;

	if (!dploopdata.data) dploopdata.data = new loopdata { 200, 135, 65 };

	thread t { DPLoop, this, dploopdata.data };

	t.detach();

	return true;
}