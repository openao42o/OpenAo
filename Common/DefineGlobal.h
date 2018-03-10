// Copyright[2002] MasangSoft
#pragma once

#include <type_traits>
#include <cmath>

#ifndef _ATUM_DATA_LOADER
#include "Random.h"
#endif

#define _ARRAY(x)
#define ARRAY_(x)
#define STRUCT_(x)

#define LODWORD(ll)            ((DWORD)(ll))
#define HIDWORD(ll)            ((DWORD)(((UINT64)(ll) >> 32) & 0xFFFFFFFF))


namespace util
{
    template<typename min_t, typename val_t, typename max_t>
    bool in_range(min_t min, val_t val, max_t max) { return min <= val && val <= max; }

    template<typename x_t, typename y_t> auto length(x_t x, y_t y) { return sqrt(x*x + y*y); }
    

    template<typename T> void del(T*& ptr) { if (ptr) { delete ptr; ptr = nullptr; } }
    template<typename T> void del_array(T*& ptr) { if (ptr) { delete[] ptr; ptr = nullptr; } }
    template<typename T> void del_destroy(T*& ptr) { if (ptr) { ptr->Destroy();  delete ptr; ptr = nullptr; } }

    template<typename T> void release(T*& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }

    template<typename ptr_t, typename ArraySize_t>
    void zero(ptr_t* ptr, ArraySize_t size) { ::memset(ptr, 0x00, size); }

    template<typename struct_t>
    void zero(struct_t& obj)
    {
        static_assert(!std::is_pointer<struct_t>::value, "You must specify a size to use a pointer.");
        
        ::memset(&obj, 0x00, sizeof(struct_t));
    }

    template<unsigned N>
    void strcpy(char (&dest)[N], const char* src)
    {
        ::strcpy_s(dest, src);
    }

    template<typename MaxSize_t>
    void strncpy(char* szDest, const char* szSource, MaxSize_t MaxSize) { util::zero(szDest, MaxSize); ::strncpy(szDest, szSource, MaxSize - 1); }

    template<size_t MaxSize>
    void strncpy(char(&szDest)[MaxSize], const char* szSource) { util::zero(szDest, MaxSize); ::strncpy(szDest, szSource, MaxSize - 1); }

} // namespace util

constexpr auto MAX_INT64_VALUE = LLONG_MAX;
constexpr auto MAX_UINT64_VALUE = ULLONG_MAX;
constexpr auto MAX_INT_VALUE = INT_MAX;
constexpr auto MAX_UINT_VALUE = UINT_MAX;
constexpr auto MAX_SHORT_VALUE = SHRT_MAX;
constexpr auto MAX_USHORT_VALUE = USHRT_MAX;
constexpr auto MAX_BYTE_VALUE = UCHAR_MAX;

// I actually like this, c++ should support some automation like it for enums
#define GET_CASERETURN_STRING_BY_TYPE(str)    case str: return #str ""
