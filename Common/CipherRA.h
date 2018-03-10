//Copyright[2002] MasangSoft
#pragma once

//#define CIPHER2015

#pragma warning(push)
#pragma warning(disable : 4307)

namespace Cipher2015
{
    const unsigned int magic = 2654435761;

    inline unsigned genKey4(unsigned pos)
    {
        return ((pos / 4) + 1337) * magic;
    }
    
    inline char genKey1(unsigned pos)
    {
        unsigned key = genKey4(pos);
        return ((char*)&key)[pos % 4];
    }
} // namespace Cipher2015

unsigned SeedFromFilename(char* filename);

void CipherRA(char* buffer, unsigned count, unsigned offset);

inline void CipherRA(char* buffer, unsigned count, int offset)
{
    if (offset >= 0) CipherRA(buffer, count, (unsigned)offset);
}

#pragma warning(pop)
