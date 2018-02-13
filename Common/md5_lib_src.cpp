#include "stdafx.h"
#include <stdlib.h>
#include "md5_lib_src.h"

constexpr auto TEST_BLOCK_LEN = 1000;
constexpr auto TEST_BLOCK_COUNT = 1000;

constexpr auto S11 = 7;
constexpr auto S12 = 12;
constexpr auto S13 = 17;
constexpr auto S14 = 22;
constexpr auto S21 = 5;
constexpr auto S22 = 9;
constexpr auto S23 = 14;
constexpr auto S24 = 20;
constexpr auto S31 = 4;
constexpr auto S32 = 11;
constexpr auto S33 = 16;
constexpr auto S34 = 23;
constexpr auto S41 = 6;
constexpr auto S42 = 10;
constexpr auto S43 = 15;
constexpr auto S44 = 21;

template<class X, class N> auto rotateleft(X x, N n) { return (x << n) | (x >> (32 - n)); }

template<class A, class B, class C, class D, class X, class S, class AC>
void FF(A& a, B b, C c, D d, X x, S s, AC ac)
{
	a += ((b & c) | (~b & d)) + x + (unsigned long int)ac;
	a = rotateleft(a, s);
	a += b;
}
template<class A, class B, class C, class D, class X, class S, class AC>
void GG(A& a, B b, C c, D d, X x, S s, AC ac)
{
	a += ((b & d) | (c & ~d)) + x + (unsigned long int)ac;
	a = rotateleft(a, s);
	a += b;
}
template<class A, class B, class C, class D, class X, class S, class AC>
void HH(A& a, B b, C c, D d, X x, S s, AC ac)
{
	a += (b ^ c ^ d) + x + (unsigned long int)ac;
	a = rotateleft(a, s);
	a += b;
}
template<class A, class B, class C, class D, class X, class S, class AC>
void II(A& a, B b, C c, D d, X x, S s, AC ac)
{
	a += (c ^ (b | ~d)) + x + (unsigned long int)ac;
	a = rotateleft(a, s);
	a += b;
}

MD5::MD5()
{
	memset(padding, 0x00, sizeof(padding));

	padding[0] = 0x80;

	count[0] = 0;
	count[1] = 0;

	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;

	memset(buffer, 0x00, sizeof(buffer));
}

void MD5::MD5Encode(char* string, unsigned char* buffer)
{
	unsigned char digest[16];
	unsigned int len = strlen(string);

	MD5Update((unsigned char*)string, len);
	MD5Final(digest);

	memcpy(buffer, digest, sizeof(digest));
}


void MD5::MD5Binary2String(unsigned char digest[16], char* EncodedString)
{
	sprintf(EncodedString, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
		digest[0], digest[1], digest[2], digest[3],
		digest[4], digest[5], digest[6], digest[7],
		digest[8], digest[9], digest[10], digest[11],
		digest[12], digest[13], digest[14], digest[15]);
}

void MD5::MD5String2Binary(char* EncodedString, unsigned char byDigest[16])
{
	char szTemp[128];
	for (auto i = 0; i < 16; i++)
	{
		memset(szTemp, 0x00, 128);
		memcpy(szTemp, &EncodedString[i * 2], 2);
		byDigest[i] = (unsigned char)strtol(szTemp, nullptr, 16);
	}
}

void MD5::MD5Update(unsigned char* input, unsigned int inputLen)
{
	unsigned int i, index, partLen;

	index = (unsigned int)((count[0] >> 3) & 0x3F);

	if ((count[0] += ((unsigned long int)inputLen << 3)) < ((unsigned long int)inputLen << 3)) count[1]++;

	count[1] += ((unsigned long int)inputLen >> 29);

	partLen = 64 - index;

	if (inputLen >= partLen)
	{
		MD5_memcpy((unsigned char *)&buffer[index], (unsigned char*)input, partLen);
		MD5Transform(state, buffer);

		for (i = partLen; i + 63 < inputLen; i += 64) MD5Transform(state, &input[i]);

		index = 0;
	}
	else i = 0;

	MD5_memcpy((unsigned char *)&buffer[index], (unsigned char*)&input[i], inputLen - i);
}

void MD5::MD5Final(unsigned char digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;

	Encode(bits, count, 8);

	index = (unsigned int)((count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update(padding, padLen);

	MD5Update(bits, 8);

	Encode(digest, state, 16);

	MD5_memset((unsigned char*)this, 0, sizeof(*this));
}

void MD5::MD5Transform(unsigned long int state[4], unsigned char block[64])
{
	auto a = state[0];
	auto b = state[1];
	auto c = state[2];
	auto d = state[3];
	
	unsigned long int x[16];

	Decode(x, block, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
	FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */

	GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
	HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
	II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	MD5_memset((unsigned char *)x, 0, sizeof(x));
}

void MD5::Encode(unsigned char *output, unsigned long int *input, unsigned int len)
{
	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

void MD5::Decode(unsigned long int *output, unsigned char *input, unsigned int len)
{
	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j + 1]) << 8) |
		(((unsigned long int)input[j + 2]) << 16) | (((unsigned long int)input[j + 3]) << 24);
}

void MD5::MD5_memcpy(unsigned char* output, unsigned char * input, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++) output[i] = input[i];
}

void MD5::MD5_memset(unsigned char* output, int value, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++) ((char *)output)[i] = (char)value;
}

