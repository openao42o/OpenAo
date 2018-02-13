//////////////////////////////////////////////////////////////////////////
// 2015-09-23 Future, static Helper Class for Machine Identification Methods
#pragma once
class CMachineID
{
public:
	static void GetPrimaryAdapterMacAddress(OUT BYTE* pAddress, OUT OPTIONAL UINT* pnAddressLen = NULL);
	static void GetPrimaryAdapterMacAddressStr(OUT char* strAddress);
};

