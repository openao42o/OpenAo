class MD5
{
	unsigned long int state[4];
	unsigned long int count[2];
	unsigned char buffer[64];
	unsigned char padding[64];

protected:
	void MD5Update(unsigned char*, unsigned int);
	void MD5Final(unsigned char[16]);
	static void MD5Transform(unsigned long int[4], unsigned char[64]);
	static void Encode(unsigned char*, unsigned long int*, unsigned int);
	static void Decode(unsigned long int*, unsigned char*, unsigned int);
	static void MD5_memcpy(unsigned char*, unsigned char*, unsigned int);
	static void MD5_memset(unsigned char*, int, unsigned int);

public:
	MD5();
	virtual ~MD5() = default;
	void MD5Encode(char*, unsigned char*);
	static void MD5Binary2String(unsigned char[16], char *EncodedString);
	static void MD5String2Binary(char *EncodedString, unsigned char[16]);
};
