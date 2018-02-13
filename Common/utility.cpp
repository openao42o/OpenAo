#include "utility.h"
#include "metafile.h"
#include "md5.h"
#include <shlwapi.h>
#include <direct.h>
#include <errno.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "shlwapi.lib")

std::string utility::int2str(uint64 n)
{
    if(!n) {
        return "0";
    }
    std::string str;
    while(n) {
        str = char('0' + (n % 10)) + str;
        n /= 10;
    }
    return str;
}

uint64 utility::str2int(const std::string& str, bool& err)
{
    uint64 n = 0;
    err = false;
    if(str.empty() || str.size() > 20) {
        err = true;
        return n;
    }
    for(size_t i=0; i<str.size(); ++i) {
        if(!isdigit(str[i])) {
            err = true;
            return n;
        }
        n *= 10;
        n += (str[i] - '0');
    }
    return n;
}

std::string utility::get_parent_directory(std::string p)
{
    char sep[] = {'/', '\\', 0};
    std::string::size_type k = p.size();
    for(; 1 <= k && ((sep[0] == p[k - 1]) || (sep[1] == p[k - 1])); --k);
    if(!p.empty()) {
        p = p.substr(0, k);
        p = p.substr(0, p.find_last_of(sep) + 1);
    }
    return p;
}

bool utility::create_directory(const std::string& d)
{
    if(PathFileExistsA(d.c_str())) {
        DeleteFileA(d.c_str());
    }
    int r = _mkdir(d.c_str());
    if(0 != r && EEXIST != errno) {
        return false;
    }
    return true;
}

void utility::create_parent_directories(std::string p)
{
    char sep[] = {'/', '\\', 0};
    std::string::size_type k = p.size();
    for(; 1 <= k && ((sep[0] == p[k - 1]) || (sep[1] == p[k - 1])); --k);
    if(!p.empty()) {
        p = p.substr(0, k);
        p = p.substr(0, p.find_last_of(sep) + 1);
    }
    if(!p.empty()) {
        for(std::string::size_type i = 0; i < p.size();){
            std::string::size_type j = p.find_first_of(sep, i);
            i = (std::string::npos != i) ? (j + 1) : j;
            std::string tmpS = p.substr(0, i);
            if(!tmpS.empty()){
                std::string::size_type x = tmpS.size();
                while((1 < x) && ((sep[0] == tmpS[x-1]) || (sep[1] == tmpS[x-1]))) --x;
                if(x < tmpS.size()) {
                    tmpS = tmpS.substr(0, x);
                }
                create_directory(tmpS);
            }
        }
    }
}

uint32 utility::get_file_size(const std::string& fname)
{
    struct _stat buf;
    if(!_stat(fname.c_str(), &buf )) {
        return buf.st_size;
    }
    return 0;
}

HINTERNET get_hSession()
{
    static HINTERNET hSession =
        InternetOpenA("frost_static_updateder", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
    return hSession;
}

bool utility::
download_url_to_directory(const std::string& url,
                          const std::string& dst_file)
{
    HINTERNET hURL = InternetOpenUrlA(get_hSession(), url.c_str(), 0, 0,
        INTERNET_FLAG_DONT_CACHE, 0) ;
    if (!hURL){
        return false;
    }
    if(PathIsDirectoryA(dst_file.c_str())) {
        utility::run_command(std::string("rd \"") + dst_file + "\" /S/Q");
    }
    utility::create_parent_directories(dst_file);
    if(PathFileExistsA(dst_file.c_str())) {
        SetFileAttributesA(dst_file.c_str(), FILE_ATTRIBUTE_NORMAL);
    }
    FILE* f = fopen(dst_file.c_str(), "wb");
    if(!f) {
        InternetCloseHandle(hURL);
        return false;
    }
    const uint32 bufflen = 1024;
    char buff[bufflen] = {0};
    DWORD len = 0;
    while(InternetReadFile(hURL, buff, bufflen, &len) && len) {
        fwrite(buff, sizeof(char), len, f);
    }
    fclose(f);
    InternetCloseHandle(hURL);
    return true;
}

void utility::run_command(std::string cmd)
{
    char* cs = getenv("ComSpec");
    if(cs) {
        cmd = std::string(cs) + " /C " + cmd;
    } else {
        cmd = std::string("cmd /C ") + cmd;
    }
    system(cmd.c_str());
}

static bool get_host_ip(const char* host, char* ip)
{
    in_addr inaddr;
    inaddr.S_un.S_addr = inet_addr(host);
    if( inaddr.S_un.S_addr == INADDR_NONE) {
        hostent* phostent = gethostbyname( host);
        if( phostent == 0) {
            return false;
        }
        if( sizeof(inaddr) != phostent->h_length) {
            return false;
        }
        struct in_addr addr;
        addr.S_un.S_addr = *((u_long*)phostent->h_addr_list[0]);
        strcpy(ip, inet_ntoa(addr));
        return true;
    } else {
        strcpy(ip, host);
        return true;
    }
    return false;
}

static bool tryConnect(uint32 ip,
                       uint16 port,
                       uint32 timeout)
{
    SOCKET sc = INVALID_SOCKET;
    sockaddr_in clnt;
    clnt.sin_family = AF_INET;
    clnt.sin_addr.s_addr = ip;
    clnt.sin_port = htons(port);
    sc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == sc) {
        return false;
    }
    int iMode = 1;
    ioctlsocket(sc, FIONBIO, (u_long FAR*) &iMode);
    int err = connect(sc, (SOCKADDR*)&clnt, sizeof(clnt));
    if((0 != err) && (WSAEWOULDBLOCK == WSAGetLastError())) {
        fd_set socks;
        FD_ZERO(&socks);
        FD_SET(sc, &socks);
        timeval tm;
        tm.tv_sec = timeout / 1000;
        tm.tv_usec = 1000 * (timeout % 1000);
        int r = ::select(0, 0, &socks, 0, &tm);
        if((1 == r) && FD_ISSET(sc, &socks)) {
            err = 0;
        }
    }
    closesocket(sc);
    return (0 == err);
}

bool utility::connection_exists(const std::string& srvName,
                                uint16 srvPort,
                                uint32 timeout)
{
    char ip_str[16] = {0};
    if(!get_host_ip(srvName.c_str(), ip_str)) {
        return false;
    }
    return tryConnect(::inet_addr(ip_str), srvPort, timeout);
}

inline int to_lower(char x)
{
    return ((0x40 < x) && (x < 0x5b)) ? (x + 0x20) : x;
}

static bool cmpStr(const std::string& s1, 
                   const std::string& s2)
{
    if(s1.size() != s2.size()){
        return false;
    }
    for(size_t i=0; i<s1.size(); ++i) {
        if(to_lower(s1[i]) != to_lower(s2[i])) {
            return false;
        }
    }
    return true;
}

static bool update_file(metafile& mtf,
                        update_by_crc_param* up,
                        const file_descriptor& f)
{
    static md5Hash md5;
    char crc32[33] = {0};
    std::string fn = std::string(up->m_destDir) + "\\" + f.m_path;
    if(PathFileExistsA(fn.c_str())) {
        std::string ncrc(md5.getMD5FromFile(fn.c_str()));
        if(cmpStr(ncrc, f.m_crc)) {
            return true;
        }
    }

    if(!utility::download_url_to_directory(
        std::string(up->m_rootURL) + "/" + f.m_path,
        fn)) {
        return false;
    }
    if(f.m_size != utility::get_file_size(fn)) {
        DeleteFileA(fn.c_str());
        return false;
    }
    return true;
}

static bool download_files(metafile& mtf,
                           update_by_crc_param* up)
{
    std::set<file_descriptor>::const_iterator
        i = mtf.m_files.begin(), e = mtf.m_files.end();
    for(; i != e; ++i) {
        if(!update_file(mtf, up, *i)) {
            return false;
        }
    }
    return true;
}

static void remove_local_crc(const std::string& dir)
{
    if(PathFileExistsA((dir + "\\update.crc").c_str())) {
        DeleteFileA((dir + "\\update.crc").c_str());
    }
}

bool utility::update_by_crc(update_by_crc_param* up)
{
    if(!up) {
        return false;
    }
    metafile mtf;
    if(!utility::download_url_to_directory(std::string(up->m_rootURL) + "/update.crc",
        std::string(up->m_destDir) + "\\update.crc")){
        remove_local_crc(up->m_destDir);
        return false;
    }
    if(!mtf.load(std::string(up->m_destDir) + "\\update.crc")){
        remove_local_crc(up->m_destDir);
        return false;
    }
    remove_local_crc(up->m_destDir);
    bool r = download_files(mtf, up);
    InternetCloseHandle(get_hSession());
    return r;
}

