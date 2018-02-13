#include "md5.h"
#include <sstream>

const char md5Hash::m_oneAndZeros = 0x80;
const char md5Hash::m_zeros = 0x0;
const unsigned __int32 md5Hash::m_k[64] =
    {
        3614090360, 3905402710, 606105819,  3250441966, 4118548399, 1200080426, 2821735955, 4249261313, 
        1770035416, 2336552879, 4294925233, 2304563134, 1804603682, 4254626195, 2792965006, 1236535329, 
        4129170786, 3225465664, 643717713,  3921069994, 3593408605, 38016083,   3634488961, 3889429448, 
        568446438,  3275163606, 4107603335, 1163531501, 2850285829, 4243563512, 1735328473, 2368359562, 
        4294588738, 2272392833, 1839030562, 4259657740, 2763975236, 1272893353, 4139469664, 3200236656, 
        681279174,  3936430074, 3572445317, 76029189,   3654602809, 3873151461, 530742520,  3299628645, 
        4096336452, 1126891415, 2878612391, 4237533241, 1700485571, 2399980690, 4293915773, 2240044497, 
        1873313359, 4264355552, 2734768916, 1309151649, 4149444226, 3174756917, 718787259,  3951481745
    };

md5Hash::md5Hash()
{
    for (int  i = 0; i < 64; i += 4) {
        if (i < 16) {
            m_r[i + 0] =  7;
            m_r[i + 1] = 12;
            m_r[i + 2] = 17;
            m_r[i + 3] = 22;
        } else if (i < 32) {
            m_r[i + 0] =  5;
            m_r[i + 1] = 9;
            m_r[i + 2] = 14;
            m_r[i + 3] = 20;
        } else if (i < 48) {
            m_r[i + 0] =  4;
            m_r[i + 1] = 11;
            m_r[i + 2] = 16;
            m_r[i + 3] = 23;
        } else {
            m_r[i + 0] =  6;
            m_r[i + 1] = 10;
            m_r[i + 2] = 15;
            m_r[i + 3] = 21;
        }
    }
}

std::string md5Hash::getMD5FromFile(const char* fileName)
{
    m_h[0] = 0x67452301;
    m_h[1] = 0xEFCDAB89;
    m_h[2] = 0x98BADCFE;
    m_h[3] = 0x10325476;
    __int64 bitLen = 0;
    unsigned char res[16] = {0};
    char part[0x40] = {0};
    __int64 i = 0;
    FILE* myFile = fopen(fileName, "rb");
    if(myFile) {
        while (true) {
            size_t gcount = fread(part, sizeof(part[0]), sizeof(part)/sizeof(part[0]), myFile);
            if (gcount == 64) {
                bitLen += gcount * 8;
                processChunk(part);
            } else {
                bitLen += gcount * 8;
                i = gcount;
                part[i] = m_oneAndZeros;
                ++i;
                if (i > 0x38) {
                    for (int j = (int)i; j < 0x40; ++j){
                        part[j] = m_zeros;
                    }
                    i = 0;
                    processChunk(part);
                }
                for (int j = (int)i; j < 0x40; ++j) {
                    if (j < 0x38) {
                        part[j] = m_zeros;
                    } else {
                        part[j] = (char)(bitLen >> (8 * (j - 0x38)));
                    }
                }
                processChunk(part);
                break;
            }
        }
        fclose(myFile);
        for (int ii = 0; ii < 16; ii++) {
            res[ii] = ((char*)&(m_h[ii / 4]))[(ii % 4)];
        }
    }
    return convToString(res);
}

void md5Hash::processChunk(char* input)
{
    unsigned __int32 a = m_h[0];
    unsigned __int32 b = m_h[1];
    unsigned __int32 c = m_h[2];
    unsigned __int32 d = m_h[3];
    unsigned __int32 t = 0;
    unsigned __int32 f = 0;
    unsigned __int32 g = 0;
    unsigned __int32 w0= 0;
    unsigned __int32 w1= 0;
    unsigned __int32 w2= 0;
    unsigned __int32 w3= 0;
    unsigned __int32 w[16] = {0};

    for (int i = 0; i < 64; ++i) {
        if (i < 16) {
            w0 = (int)input[4 * i + 0];
            w1 = (int)input[4 * i + 1];
            w2 = (int)input[4 * i + 2];
            w3 = (int)input[4 * i + 3];
            w[i] = (w0 & 0x000000FF) | ((w1 << 8) & 0x0000FF00) | ((w2 << 16) & 0x00FF0000) | ((w3 << 24) & 0xFF000000);
            f = F(b, c, d);
            g = i;
        } else if (i < 32) {
            f = G(b, c, d);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = H(b, c, d);
            g = (3 * i + 5) % 16;
        } else {
            f = I(b, c, d);
            g = (7 * i) % 16;
        }
        t = d;
        d = c;
        c = b;
        b = b + ROTATE_LEFT((a + f + m_k[i] + w[g]) , m_r[i]);
        a = t;
    }
    m_h[0] += a;
    m_h[1] += b;
    m_h[2] += c;
    m_h[3] += d;
}

std::string md5Hash::convToString(unsigned char* bytes)
{
    std::stringstream os;
    for(int i = 0; i < 16; i++) {
        os.width(2);
        os.fill('0');
        os << std::hex << (0xff & (static_cast<unsigned int>(bytes[i])));
    }
    return os.str();
}

