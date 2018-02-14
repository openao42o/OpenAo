//Copyright[2002] MasangSoft
#pragma once

#ifdef ARBILLING_DLL
#    define ARBILLING_DLLDECLARE    __declspec(dllexport) 
#else
#    define ARBILLING_DLLDECLARE    __declspec(dllimport) 
#endif //ARBILLING_DLLDECLARE

#if _MSC_VER >= 1310 // if .net2003 or more higher version
#include <atlstr.h>
#endif

#include "AuthFunctor.h"

class ArParam : public Param
{
public:
    ARTP_HEADER* m_pInfo;
    ArParam( ARTP_HEADER* pInfo ) : m_pInfo(pInfo)
    {
        
    }

private:
    ArParam() :    m_pInfo(0)
    {
        
 }
};

extern "C" { 
 // for billing
    //!====================================================================================================================
    //! @brief    try getting riocash
    //!            result can know by return value.
    //!
    ARBILLING_DLLDECLARE int  __cdecl GetRioCash( ARTP_RCS_GETRIOCASH& _in, ARTP_RET_RCS_GETRIOCASH& _out );

    //!====================================================================================================================
    //! @brief    try using riocash    
    //!
    ARBILLING_DLLDECLARE int __cdecl UseRioCash( ARTP_RCS_USERIOCASH& _in, ARTP_RET_RCS_USERIOCASH& _out );

    //!====================================================================================================================
    //! @brief    try rollback
    //!
    ARBILLING_DLLDECLARE int __cdecl Rollback( ARTP_RCS_ROLLBACK& _in, ARTP_RET_RCS_ROLLBACK& _out );

    //!====================================================================================================================
    //! @brief    try getting riocash by async
    //!            result can know by return value.
    //!
    ARBILLING_DLLDECLARE void  __cdecl GetRioCashAsync( ARTP_RCS_GETRIOCASH& _in );

    //!====================================================================================================================
    //! @brief    try using riocash by async
    //!
    ARBILLING_DLLDECLARE void __cdecl UseRioCashAsync( ARTP_RCS_USERIOCASH& _in );

    //!====================================================================================================================
    //! @brief    try rollback by async
    //!
    ARBILLING_DLLDECLARE void __cdecl RollbackAsync( ARTP_RCS_ROLLBACK& _in );

    //!====================================================================================================================
    //! @brief    set callback function of process.
    //!
    ARBILLING_DLLDECLARE void __cdecl SetResultCallbackFunction( IFunctor* pCallBackFunc_ );    

    //!====================================================================================================================
    //! @brief    set ini path    
    //!
    ARBILLING_DLLDECLARE void __cdecl SetBillingInfoPath( LPCWSTR _path );
}
