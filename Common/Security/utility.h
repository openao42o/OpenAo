#ifndef GAME_SHIELD_LIB_UTILITY_H
#define GAME_SHIELD_LIB_UTILITY_H

#include <io.h>
#include <direct.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <Wininet.h>
#include <shlwapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

struct update_by_crc_param
{
    update_by_crc_param(const char* rootURL,
                        const char* destDir):
        m_rootURL(new char[MAX_PATH + 1]),
        m_destDir(new char[MAX_PATH + 1])
    {
        strcpy(m_rootURL, rootURL);
        strcpy(m_destDir, destDir);
    }

    ~update_by_crc_param()
    {
        delete[] m_rootURL;
        delete[] m_destDir;
    }

    char*  m_rootURL;
    char*  m_destDir;
};

struct utility
{
    static std::string int2str(uint64 n);

    static uint64 str2int(const std::string& str, bool& err);

    static std::string get_parent_directory(std::string p);

    static bool create_directory(const std::string& d);

    static void create_parent_directories(std::string p);

    static bool download_url_to_directory(const std::string& url,
                                          const std::string& dst_file);

    static bool update_by_crc(update_by_crc_param* up);

    static void run_command(std::string cmd);

    static uint32 get_file_size(const std::string& fname);

    static bool connection_exists(const std::string& srv,
                                  uint16 srvPort,
                                  uint32 timeout);
    enum {
        buff_len = 4096
    };
};

#endif //GAME_SHIELD_LIB_UTILITY_H

