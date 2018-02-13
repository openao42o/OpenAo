#ifndef _MGAME_DECRYPTION_H_
#define _MGAME_DECRYPTION_H_

#ifndef MGAME_MAX_PARAM_STRING_SIZE
#define MGAME_MAX_PARAM_STRING_SIZE			50
#endif

extern char g_szMGameExcuteType[MGAME_MAX_PARAM_STRING_SIZE];
extern char g_szMGameSeed[MGAME_MAX_PARAM_STRING_SIZE];
extern char g_szMGameID[MGAME_MAX_PARAM_STRING_SIZE];
extern char g_szMGameEncryptedID[MGAME_MAX_PARAM_STRING_SIZE];
extern char g_szMGameEncryptedPWD[MGAME_MAX_PARAM_STRING_SIZE];
extern char g_szMGameSEX[MGAME_MAX_PARAM_STRING_SIZE];
extern char g_szMGameYear[MGAME_MAX_PARAM_STRING_SIZE];


unsigned char* decrypt(unsigned char* original, int seed);


#endif