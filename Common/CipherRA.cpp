#include "stdafx.h"
#include "CipherRA.h"

void CipherRA(char* buffer, unsigned count, unsigned offset)
{
	auto c1 = (4 - offset % 4) % 4;
	auto c2 = (count - c1) / 4;
	auto c3 = (count - c1) % 4;
	
	for (unsigned i = 0; i < c1; i++) buffer[i] ^= Cipher2015::genKey1(offset + i);
	
	buffer += c1;
	
	for (unsigned i = 0; i < c2; i++) ((unsigned*)buffer)[i] ^= Cipher2015::genKey4(offset + c1 + i * 4);

	buffer += c2 * 4;

	for (unsigned i = 0; i < c3; i++) buffer[i] ^= Cipher2015::genKey1(offset + c1 + c2 * 4 + i);
}
