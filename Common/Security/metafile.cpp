#include "stdafx.h"
#include "metafile.h"
#include <fstream>

file_descriptor::file_descriptor(const std::string& p): m_path(p) {}

metafile::metafile():
    m_files(),
    m_totalSize(0)
{}

inline int to_lower(char x)
{
    return ((0x40 < x) && (x < 0x5b)) ? (x + 0x20) : x;
}

bool file_descriptor::operator< (const file_descriptor& fd) const
{
    uint32 n = m_path.size() < fd.m_path.size() ? m_path.size() : fd.m_path.size();
    for(uint32 i=0; i<n; ++i) {
        int x = to_lower(m_path[i]) - to_lower(fd.m_path[i]);
        if(x) {
            return (x < 0);
        }
    }
    return m_path.size() < fd.m_path.size();
}

static uint32 get_word(const char* buff,
                       uint32 len,
                       uint32& begin,
                       char* word)
{
    static const char separator = '|';
    static const char eol1 = '\r';
    static const char eol2 = '\n';
    while(begin < len && (separator == buff[begin] || eol1 == buff[begin] || eol2 == buff[begin])) ++begin;
    uint32 end = begin;
    while(end < len && (separator != buff[end] && eol1 != buff[end] && eol2 != buff[end])) {
        word[end - begin] = buff[end];
        ++end;
    }
    uint32 l = end - begin;
    begin = end;
    return l;
}

bool file_descriptor::pars_line(const char* buff, uint32 len)
{
    m_path = "";
    m_crc = "";
    m_size = 0;
    uint32 possibleHeader = 4;
    while(0 < len && 0 < possibleHeader && '|' != *buff) {
        ++buff;
        --len;
        --possibleHeader;
    }
    if(0 < len && '|' != *buff) {
        return false;
    } else if (0 == len) {
        return true;
    }
    uint32 m = 0, i = 0, j = 0;
    static char word[max_one_line_len] = {0};
    while(m < 3) {
        switch(m) {
            case 0:
                if(0 < (i = get_word(buff, len, j, word))) {
                    m_path = std::string(word, word + i);
                    ++m;
                } else {
                    return false;
                }
                break;
            case 1:
                if(0 < (i = get_word(buff, len, j, word))) {
                    m_crc = std::string(word, word + i);
                    ++m;
                } else {
                    return false;
                }
                break;
            case 2:
                if((0 < (i = get_word(buff, len, j, word))) && (i < 11)) {
                    bool err = false;
                    m_size = (uint32)utility::str2int(std::string(word, word + i), err);
                    if(err) return false;
                    ++m;
                } else {
                    return false;
                }
                break;
            default: return false;
        }
    }
    return true;
}

bool metafile::load(const std::string& fname)
{
    std::string pdir = utility::get_parent_directory(fname);
    m_crcPath = fname;
    FILE* f = fopen(m_crcPath.c_str(), "rb");
    if(!f) {
        return false;
    }
    char buff[max_one_line_len] = {0};
    uint32 i = 0;
    bool err = false;
    while(true) {
        if(i < max_one_line_len) {
            if(fread(buff + i, sizeof(char), 1, f) < 1) break;
            if(buff[i] == '\n') {
                file_descriptor fd;
                if(!fd.pars_line(buff, i)) {
                    err = true;
                    break;
                }
                if(!fd.m_path.empty()) {
                    m_files.insert(fd);
                    m_totalSize += fd.m_size;
                }
                i = 0;
                continue;
            }
        } else {
            err = true;
            break;
        }
        ++i;
    }
    fclose(f);
    if(0 < i) {
        file_descriptor fd;
        if(!fd.pars_line(buff, i)) {
            return false;
        }
        if(!fd.m_path.empty()) {
            m_files.insert(fd);
            m_totalSize += fd.m_size;
        }
        i = 0;
    }
    return !err;
}

