//////////////////////////////////////////////////////////////////////////
// 2015-09-23 Future, static Helper Class for Machine Identification Methods

#include "StdAfx.h"
#include "MachineID.h"

#include <iptypes.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define SIZE_MAX_ADAPTER_INFO_BUFFER	16	// Maximum Number of Network Adapters to acquire

// 2015-09-23 Future, added transmition of MAC-Address
void CMachineID::GetPrimaryAdapterMacAddress(OUT BYTE* pAddress, OUT OPTIONAL UINT* pnAddressLen /*= NULL*/)
{
	IP_ADAPTER_INFO adapterInfo[SIZE_MAX_ADAPTER_INFO_BUFFER];
	DWORD infoLen = sizeof(adapterInfo);
	memset(adapterInfo, 0x00, infoLen);

	// Acquire Adapters Info
	DWORD res = GetAdaptersInfo(adapterInfo, &infoLen);
	if (res != ERROR_SUCCESS)
		return;

	// Get the primary Adapter
	PIP_ADAPTER_INFO pAdapterInfo, pPrimaryAdapter;
	pAdapterInfo = pPrimaryAdapter = adapterInfo;
	while (pAdapterInfo)
	{
		if (pAdapterInfo->Index < pPrimaryAdapter->Index)
		{
			pPrimaryAdapter = pAdapterInfo;
		}
		pAdapterInfo = pAdapterInfo->Next;
	}

	// Set the adapter Info
	if (pPrimaryAdapter)
		memcpy_s(pAddress
		, sizeof(((MSG_FC_CONNECT_LOGIN*)0)->MACAddress)
		, pPrimaryAdapter->Address
		, sizeof(pPrimaryAdapter->Address));
	if (pnAddressLen)
		*pnAddressLen = pPrimaryAdapter->AddressLength;
}

void CMachineID::GetPrimaryAdapterMacAddressStr(OUT char* strAddress)
{
	if (!strAddress)
		return;

	BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
	UINT AddressLength = 0;

	// Get the primary Mac Address
	GetPrimaryAdapterMacAddress(Address, &AddressLength);

	// Format the MAC Address
	int writePtr = 0;
	for (UINT i = 0; i < AddressLength; i++)
	{
		if (i == (AddressLength - 1))
		{
			sprintf_s(strAddress + writePtr, SIZE_MAX_MAC_ADDRESS - writePtr, "%.2X", (int)Address[i]);
			writePtr += 2;
		}
		else
		{
			sprintf_s(strAddress + writePtr, SIZE_MAX_MAC_ADDRESS - writePtr, "%.2X-", (int)Address[i]);
			writePtr += 3;
		}
	}
}
