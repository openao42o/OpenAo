//Copyright[2002] MasangSoft
///////////////////////////////////////////////////////////////////////////////
//  GlobalFunc.cpp : 
//
//  Date    : 2008-06-26 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#include "GlobalFunc.h"

///////////////////////////////////////////////////////////////////////////////
// 2008-06-26 by cmkwon, float -> int 형변환 함수 추가 - 
//! INTORFLOAT union for easy access to bits of a float.
typedef union
{
    int     i;          // as integer
    float   f;          // as float
    struct              // as bit fields
    {
        unsigned int    sign:1;
        unsigned int    biasedexponent:8;
        unsigned int    significand;
    } bits;
} INTORFLOAT;

INTORFLOAT  FTOIBIAS = {((23 - 0 + 127) << 23) + (1 << 22)};


///////////////////////////////////////////////////////////////////////////////
/// \fn            int f2i(float f) 
/// \brief        // 2008-06-26 by cmkwon, float -> int 형변환 함수 추가 - 
///                    방법 : Game Programming Gems2 chapter 2.1에 Yossarian King씨가 작성한 방법입니다
///                    장점 : FPU모드에 따라 일반적으로 일반 int형변환과 정확히 일치합니다.
///                    단점 : float의 값이 2의 23승(8388608)보다 작아야 합니다. 또한, FPU모드가 Chopping이 아닌 경우. 예를 들면 위의 fastf2i_round나 fastf2i와 같이 사용하면 안됩니다. fastf2i_round나 fastf2i는 FPU모드를 반올림모드로 사용하는데, f2i함수는 FPU모드를 따로 설정하지 않기 때문에 현재 FPU모드 그대로 변환을 시도합니다. (FPU모드는 위의 참고를 보시기 바랍니다)
/// \author        cmkwon
/// \date        2008-06-26 ~ 2008-06-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
inline int f2i(float f) 
{
    INTORFLOAT ftmp;
    ftmp.f = f;
    ftmp.f += FTOIBIAS.f;
    ftmp.i -= FTOIBIAS.i;
    return ftmp.i;    
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            int fastf2i_round(float f)
/// \brief        // 2008-06-26 by cmkwon, float -> int 형변환 함수 추가 - 
///                방법 : FPU를 이용한 인라인 어셈코드입니다. 
///                장점 : 가장 빠릅니다.
///                단점 : 반올림이 됩니다. 따라서 int a = int(1234.56f) 와 같은 값이 안 나온다는 것입니다. 반올림되어서 1235가 나옵니다.     
/// \author        cmkwon
/// \date        2008-06-26 ~ 2008-06-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
inline int fastf2i_round(float f)
{
#ifdef _WIN32
    int i;
    __asm
    {
        fld        f
        fistp    i
    }
    return i;
#else
    return (int)(f + 0.5f);
#endif    
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            int fastf2i(float fValue)
/// \brief        // 2008-06-26 by cmkwon, float -> int 형변환 함수 추가 - 
///                    방법 : 역시 FPU를 이용한 인라인 어셈코드이지만 반올림을 안하도록 약간의 조정을 합니다.
///                    장점 : 역시 방법1과 흡사하게 빠릅니다.
///                    단점 : 방법1에서 반올림이 안되도록 반올림값을 빼주고 있습니다만 0.9999999f식의 값이 들어오면 역시 반올림됩니다. 물론 round_to_nearest 를 약간 상황에 맞게 수정하면 되는 것입니다만 약간 찝찝한 것 사실입니다.
/// \author        cmkwon
/// \date        2008-06-26 ~ 2008-06-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
inline int fastf2i(float fValue)
{
#ifdef _WIN32
    static const float round_to_nearest = -0.499999f;

    int iValue;
    __asm 
    {
        fld        fValue
        fadd    round_to_nearest
        fistp    iValue
    }
    return iValue;
#else
    return (int)fValue;
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            int Float2Int( float a )
/// \brief        // 2008-06-26 by cmkwon, float -> int 형변환 함수 추가 - 
///                    방법 : int변환을 그대로 인라인 어셈화 하였습니다.
///                    장점 : 일반 int형변환과 정확히 일치합니다.
///                    단점 : 그다지 많이 빠르지 않네요.
/// \author        cmkwon
/// \date        2008-06-26 ~ 2008-06-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
inline int Float2Int( float a )
{
#ifdef _WIN32
    int CtrlwdHolder;
    int CtrlwdSetter;
    int RetVal;
    __asm 
    {
        fld    a                    // push 'a' onto the FP stack
        fnstcw CtrlwdHolder            // store FPU control word
        movzx  eax, CtrlwdHolder    // move and zero extend word into eax
        and    eax, 0xFFFFF3FF        // set all bits except rounding bits to 1
        or     eax, 0x00000C00        // set rounding mode bits to round down
        mov    CtrlwdSetter, eax    // Prepare to set the rounding mode -- prepare to enter plaid!
        fldcw  CtrlwdSetter            // Entering plaid!
        fistp  RetVal                // Store and converted (to int) result
        fldcw  CtrlwdHolder            // Restore control word
    }
    return RetVal;
#else
    return (int)a;
#endif    
}
