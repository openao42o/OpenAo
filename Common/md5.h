#ifndef MD5_H
#define MD5_H

#include <string>

/*
 * ROTATE_LEFT rotates x left n bits. 
 * cast to unsigned int to guarantee support for 64Bit System
 */
#define ROTATE_LEFT(x, c) ((x << c) | (x >> (32-c)))

/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) ((x & y) | ((~x) & z))
#define G(x, y, z) ((z & x) | ((~z) & y))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | (~z)))

class md5Hash
{
public:
    md5Hash();

public:
    std::string getMD5FromFile(const char* fileName);
private:
    void processChunk(char*);
    std::string convToString(unsigned char* bytes);

private:
    unsigned __int32 m_r[64];
    unsigned __int32 m_h[4];

    static const unsigned __int32 m_k[64];
    static const char m_oneAndZeros;
    static const char m_zeros;
};

#endif //MD5_H

