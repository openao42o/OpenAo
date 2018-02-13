#ifndef INNOVA_BILLING_INTEGRATION_API_H
#define INNOVA_BILLING_INTEGRATION_API_H

/*!
 * \file    integration_api.h
 * \brief   This header file contains integration API
 *          for Innova Billing System client applications.
 *
 * Copyright (c) Innova Group: All Rights Reserved.
 *
 * \date    created 25-6-2009.
 */
#if defined(_WIN32) || defined(_WIN64)
    #ifdef integration_exports
    #define integration_api __declspec(dllexport)
    #else //integration_exports
    #define integration_api __declspec(dllimport)
    #endif //integration_exports
    typedef unsigned __int64 uint64;
    typedef __int64 int64;
#else // assuming defined linux
    #define integration_api
    #include <inttypes.h>
    typedef uint64_t uint64;
    typedef int64_t int64;
#endif //_WIN32 || _WIN64

#define __in_param    // for input parameters
#define __out_param   // for output parameters
typedef uint64 uint_64;
typedef int64 int_64;


//! \brief The login_result struct.
//!
//!        The object of this type should be passed as an output
//!        parameter to the login(...) function. The library
//!        initializes it itself.
struct login_result
{
    uint_64 user_id;                //!< The user ID in Billing DB.
    double user_balance;            //!< The user's balance.
    uint_64 time_remaining;         //!< The remaining time in minutes(if time based services).
    int user_status;                //!< 0 - premium account, 1 - non premium account, 2 - user blocked.
    int user_action;                //!< The request action.

};// struct login_result

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
//! \defgroup api_group Public Functions
//! \{

//////////////////////////////////////////////////////////////////////////
//! \defgroup init_group Initialization/Uninitialization
//! \{

    //! \brief  Initializes the library configuration parameters from
    //!         configuration file.
    //!
    //! \param  service_id  [out]   The unique service ID in Billing System.
    //!
    //! \return If no error occurs, returns 0. Otherwise, an error code is returned,
    //!         A specific error description can be retrieved by calling error_description(...).
    //!
    //! \remark This function should be the first function called from
    //!         integration_api library
    integration_api int innbill_initialize(__out_param uint_64* service_id);


    //! \brief  Frees all resources allocated by this library.
    //! \remark This function should be the last function called from
    //!         integration_api library
    integration_api int innbill_uninitialize();


//! \}

//////////////////////////////////////////////////////////////////////////

//! \defgroup login_group Authentication and Authorization
//! \{

    //! \brief Called when a user authorizes.
    //!
    //! \param user_name    [in]     The user name.
    //! \param password     [in]     The user password.
    //! \param exp_time     [in]     The session expiration time in seconds, 0 if no expiration time.
    //! \param ip           [in]     IP address of the client.
    //!
    //! \param result       [out]    Resulting authorization info: \n
    //!                              \li <tt>authorization_result.user_id</tt>         The user ID in Billing System.
    //!                              \li <tt>authorization_result.user_balance</tt>    The user current balance.
    //!                              \li <tt>authorization_result.user_status</tt>     0 - premium account, 1 - non premium account, 2 - user blocked.
    //!                              \li <tt>authorization_result.user_action</tt>     0 - nothing needs to be done;
    //!                                                                                1 - user authorizes the first time;
    //!                                                                                2 - password had been changed;
    //!                              \li <tt>authorization_result.time_remaining</tt>  The remaining time in minutes.
    //!
    //! \return If no error occurs, returns 0. Otherwise, an error code is returned,
    //!         A specific error description can be retrieved by calling error_description(...).
    //! \remarks   function extensions are - {_w, _a, _u}:
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_login_w( __in_param const wchar_t* user_name
                                       , __in_param const wchar_t* password
                                       , __in_param int exp_time
                                       , __in_param uint_64 ip
                                       , __out_param login_result* result);

    //! \copydoc innbill_login_w(const wchar_t*, const wchar_t*, int, uint_64, login_result*)
    integration_api int innbill_login_a( __in_param const char* user_name
                                       , __in_param const char* password
                                       , __in_param int exp_time
                                       , __in_param uint_64 ip
                                       , __out_param login_result* result);

    //! \copydoc innbill_login_w(const wchar_t*, const wchar_t*, int, uint_64, login_result*)
    integration_api int innbill_login_u( __in_param const unsigned char* user_name
                                       , __in_param const unsigned char* password
                                       , __in_param int exp_time
                                       , __in_param uint_64 ip
                                       , __out_param login_result* result);

    //! \brief Should be called when the user logged out.
    //!
    //! \param user_id          [in] The user ID.
    //! \param time_elapsed     [in] The elapsed time(minutes).
    //!
    //! \return If no error occurs, returns 0. Otherwise, an error code is returned,
    //!         A specific error description can be retrieved by calling error_description(...).
    //! \remarks  function extensions are - {_i, _w, _a, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_logout_i( __in_param uint_64 user_id
                                        , __in_param uint_64 time_elapsed);


    //! \brief Should be called when the user logged out.
    //!
    //! \param user_name        [in] The game account(login) in Billing DB.
    //! \param time_elapsed     [in] The elapsed time(minutes).
    //!
    //! \return If no error occurs, returns 0. Otherwise, an error code is returned,
    //!         A specific error description can be retrieved by calling error_description(...).
    integration_api int innbill_logout_w( __in_param const wchar_t* user_name
                                        , __in_param uint_64 time_elapsed);

    //! \copydoc innbill_logout_w(const wchar_t*, uint_64*, int*)
    integration_api int innbill_logout_a( __in_param const char* user_name
                                        , __in_param uint_64 time_elapsed);

    //! \copydoc innbill_logout_w(const wchar_t*, uint_64*, int*)
    integration_api int innbill_logout_u( __in_param const unsigned char* user_name
                                        , __in_param uint_64 time_elapsed);

    //! \}

    //////////////////////////////////////////////////////////////////////////

    //! \defgroup status_group Status Checking
    //! \{

    //! \brief Checks user status.
    //!
    //! \param user_id      [in]  The user ID in Billing DB.
    //! \param user_balance [out] The current amount of money.
    //! \param status       [out] 0 - premium account, 1 - non premium account, 2 - user blocked.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_i, _w, _a, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_check_user_status_i( __in_param uint_64 user_id
                                                   , __out_param double* user_balance
                                                   , __out_param int* status);

    //! \brief Checks user status.
    //!
    //! \param user_name [in]  The game account(login) in Billing DB.
    //! \param user_balance [out] The current amount of money.
    //! \param status       [out] 0 - premium account, 1 - non premium account, 2 - user blocked.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_i, _w, _a, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
   integration_api int innbill_check_user_status_w( __in_param const wchar_t* user_name
                                                   , __out_param double* user_balance
                                                   , __out_param int* status);

    //! \copydoc check_user_status_w(const wchar_t*, uint_64*, int*)
    integration_api int innbill_check_user_status_a( __in_param const char* user_name
                                                   , __out_param double* user_balance
                                                   , __out_param int* status);

    //! \copydoc check_user_status_w(const wchar_t*, uint_64*, int*)
    integration_api int innbill_check_user_status_u( __in_param const unsigned char* user_name
                                                   , __out_param double* user_balance
                                                   , __out_param int* status);

    //! \brief Checks user status (for time based services).
    //!
    //! \param user_id          [in]  The user ID
    //! \param user_balance     [out] The current amount of money.
    //! \param status           [out] 0 - premium account, 1 - non premium account, 2 - user blocked.
    //! \param time_remaining   [out] The remaining time in minutes.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_i, _w, _a, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_check_user_status_ex_i( __in_param uint_64 user_id
                                                      , __out_param double* user_balance
                                                      , __out_param int* status
                                                      , __out_param uint_64* time_remaining);


    //! \brief Checks user status. (for time based services)
    //!
    //! \param user_name        [in]  The game account(login) in Billing DB.
    //! \param user_balance     [out] The current amount of money.
    //! \param status           [out] 0 - premium account, 1 - non premium account, 2 - user blocked.
    //! \param time_remaining   [out] The remaining time in minutes.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_i, _w, _a, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_check_user_status_ex_w( __in_param const wchar_t* user_name
                                                      , __out_param double* user_balance
                                                      , __out_param int* status
                                                      , __out_param uint_64* time_remaining);

    //! \copydoc innbill_check_user_status_ex_w(const char*, uint_64*, int*, uint_64*)
    integration_api int innbill_check_user_status_ex_a( __in_param const char* user_name
                                                      , __out_param double* user_balance
                                                      , __out_param int* status
                                                      , __out_param uint_64* time_remaining);

    //! \copydoc innbill_check_user_status_ex_w(const char*, uint_64*, int*, uint_64*)
    integration_api int innbill_check_user_status_ex_u( __in_param const unsigned char* user_name
                                                      , __out_param double* user_balance
                                                      , __out_param int* status
                                                      , __out_param uint_64* time_remaining);

    //! \}


    //////////////////////////////////////////////////////////////////////////

    //! \defgroup selling_group Item Selling
    //! \{

    //! \brief Called when user buys item in a service(game).
    //!
    //! \param user_id                  [in] The user ID.
    //! \param item_id                  [in] The item ID.
    //! \param item_type                [in] The item type. 0 - uses item ID within service, 1 - within billing
    //! \param item_price               [in] The item price or overall price(depend on item_price_type).
    //! \param item_price_type          [in] The type of previous param (item_price)
    //!                                      0 - item_price is an item price and must be checked within the function.
    //!                                      1 - item_price is an item price and mustn't be checked within the function.
    //!                                      2 - item_price is an overall price and must be checked within the function.
    //!                                      3 - item_price is an overall price and mustn't be checked within the function.
    //! \param item_count               [in] The item count.
    //! \param service_transaction_id   [in] The service transaction ID.
    //! \param server_name              [in] The server name.
    //! \param character_name           [in] The character name.
    //! \param billing_transaction_id   [out] The billing transaction ID.
    //! \param user_balance             [out] The current amount of money.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_iiw, _iia, _iiu, _iww, _iaa, _iuu, _wiw, _aia, _uiu, _www, _aaa, _uuu}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_buy_item_iiw( __in_param uint_64 user_id
                                            , __in_param uint_64 item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const wchar_t* server_name
                                            , __in_param const wchar_t* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);

    //! \copydoc innbill_buy_item_iiw(uint_64, uint_64, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_iia( __in_param uint_64 user_id
                                            , __in_param uint_64 item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const char* server_name
                                            , __in_param const char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \copydoc innbill_buy_item_iiw(uint_64, uint_64, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_iiu( __in_param uint_64 user_id
                                            , __in_param uint_64 item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const unsigned char* server_name
                                            , __in_param const unsigned char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \brief Called when user buys item in a service.
    //!        For those services which use item ID as a string.
    //!
    //! \param user_id                  [in] The user ID.
    //! \param item_id                  [in] The item ID.
    //! \param item_type                [in] The item type. 0 - uses item ID within service, 1 - within billing
    //! \param item_price               [in] The item price or overall price(depend on item_price_type).
    //! \param item_price_type          [in] The type of previous param (item_price)
    //!                                      0 - item_price is an item price and must be checked within the function.
    //!                                      1 - item_price is an item price and mustn't be checked within the function.
    //!                                      2 - item_price is an overall price and must be checked within the function.
    //!                                      3 - item_price is an overall price and mustn't be checked within the function.
    //! \param item_count               [in] The item count.
    //! \param service_transaction_id   [in] The service transaction ID.
    //! \param server_name              [in] The server name.
    //! \param character_name           [in] The character name.
    //! \param billing_transaction_id   [out] The billing transaction ID.
    //! \param user_balance             [out] The current amount of money.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_iiw, _iia, _iiu, _iww, _iaa, _iuu, _wiw, _aia, _uiu, _www, _aaa, _uuu}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_buy_item_iww( __in_param uint_64 user_id
                                            , __in_param const wchar_t* item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const wchar_t* server_name
                                            , __in_param const wchar_t* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);

    //! \copydoc innbill_buy_item_iww(uint_64, const wchar_t*, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_iaa( __in_param uint_64 user_id
                                            , __in_param const char* item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const char* server_name
                                            , __in_param const char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \copydoc innbill_buy_item_iww(uint_64, const wchar_t*, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_iuu( __in_param uint_64 user_id
                                            , __in_param const unsigned char* item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const unsigned char* server_name
                                            , __in_param const unsigned char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \brief Called when user buys item in a service(game).
    //!
    //! \param user_name                [in] The game account(login) in Billing DB.
    //! \param item_id                  [in] The item ID.
    //! \param item_type                [in] The item type. 0 - uses item ID within service, 1 - within billing
    //! \param item_price               [in] The item price or overall price(depend on item_price_type).
    //! \param item_price_type          [in] The type of previous param (item_price)
    //!                                      0 - item_price is an item price and must be checked within the function.
    //!                                      1 - item_price is an item price and mustn't be checked within the function.
    //!                                      2 - item_price is an overall price and must be checked within the function.
    //!                                      3 - item_price is an overall price and mustn't be checked within the function.
    //! \param item_count               [in] The item count.
    //! \param service_transaction_id   [in] The service transaction ID.
    //! \param server_name              [in] The server name.
    //! \param character_name           [in] The character name.
    //! \param billing_transaction_id   [out] The billing transaction ID.
    //! \param user_balance             [out] The current amount of money.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_iiw, _iia, _iiu, _iww, _iaa, _iuu, _wiw, _aia, _uiu, _www, _aaa, _uuu}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_buy_item_wiw( __in_param const wchar_t* user_name
                                            , __in_param uint_64 item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const wchar_t* server_name
                                            , __in_param const wchar_t* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);

    //! \copydoc innbill_buy_item_wiw(const wchar_t*, uint_64, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_aia( __in_param const char* user_name
                                            , __in_param uint_64 item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const char* server_name
                                            , __in_param const char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \copydoc innbill_buy_item_wiw(const wchar_t*, uint_64, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_uiu( __in_param const unsigned char* user_name
                                            , __in_param uint_64 item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const unsigned char* server_name
                                            , __in_param const unsigned char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \brief Called when user buys item in a service.
    //!        For those services which use item ID as a string.
    //!
    //! \param user_name                [in] The game account(login) in Billing DB.
    //! \param item_id                  [in] The item ID.
    //! \param item_type                [in] The item type. 0 - uses item ID within service, 1 - within billing
    //! \param item_price               [in] The item price or overall price(depend on item_price_type).
    //! \param item_price_type          [in] The type of previous param (item_price)
    //!                                      0 - item_price is an item price and must be checked within the function.
    //!                                      1 - item_price is an item price and mustn't be checked within the function.
    //!                                      2 - item_price is an overall price and must be checked within the function.
    //!                                      3 - item_price is an overall price and mustn't be checked within the function.
    //! \param item_count               [in] The item count.
    //! \param service_transaction_id   [in] The service transaction ID.
    //! \param server_name              [in] The server name.
    //! \param character_name           [in] The character name.
    //! \param billing_transaction_id   [out] The billing transaction ID.
    //! \param user_balance             [out] The current amount of money.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_iiw, _iia, _iiu, _iww, _iaa, _iuu, _wiw, _aia, _uiu, _www, _aaa, _uuu}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_buy_item_www( __in_param const wchar_t* user_name
                                            , __in_param const wchar_t* item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const wchar_t* server_name
                                            , __in_param const wchar_t* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \copydoc innbill_buy_item_www(const wchar_t*, const wchar_t*, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_aaa( __in_param const char* user_name
                                            , __in_param const char* item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const char* server_name
                                            , __in_param const char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);


    //! \copydoc innbill_buy_item_www(const wchar_t*, const wchar_t*, int, int, uint_64, const wchar_t*, const wchar_t*, uint_64*, uint_64*)
    integration_api int innbill_buy_item_uuu( __in_param const unsigned char* user_name
                                            , __in_param const unsigned char* item_id
                                            , __in_param int item_type
                                            , __in_param double item_price
                                            , __in_param int item_price_type
                                            , __in_param int item_count
                                            , __in_param uint_64 service_transaction_id
                                            , __in_param const unsigned char* server_name
                                            , __in_param const unsigned char* character_name
                                            , __out_param uint_64* billing_transaction_id
                                            , __out_param double* user_balance);

    //////////////////////////////////////////////////////////////////////////

    //! \brief Cancels transaction.
    //!
    //! \param user_id                  [in] The user ID.
    //! \param service_transaction_id   [in] The service transaction ID.
    //! \param billing_transaction_id   [in] The billing transaction ID.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_i, _a, _w, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_cancel_buy_item_i( __in_param uint_64 user_id
                                                 , __in_param uint_64 service_transaction_id
                                                 , __in_param uint_64 billing_transaction_id);


    //! \brief Cancels transaction.
    //!
    //! \param user_name                [in] The user's game account.
    //! \param service_transaction_id   [in] The service transaction ID.
    //! \param billing_transaction_id   [in] The billing transaction ID.
    //!
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_i, _a, _w, _u}:
    //!            i - means that 64 bit id used as identifier,
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_cancel_buy_item_w( __in_param const wchar_t* user_name
                                                 , __in_param uint_64 service_transaction_id
                                                 , __in_param uint_64 billing_transaction_id);


    //! \copydoc innbill_cancel_buy_item_w(wchar_t*, uint_64, uint_64)
    integration_api int innbill_cancel_buy_item_a( __in_param const char* user_name
                                                 , __in_param uint_64 service_transaction_id
                                                 , __in_param uint_64 billing_transaction_id);


    //! \copydoc innbill_cancel_buy_item_w(wchar_t*, uint_64, uint_64)
    integration_api int innbill_cancel_buy_item_u( __in_param const unsigned char* user_name
                                                 , __in_param uint_64 service_transaction_id
                                                 , __in_param uint_64 billing_transaction_id);

    //////////////////////////////////////////////////////////////////////////

    //! \brief  Retrieves an error description from the error code.
    //!
    //! \param  error_code [in] The error code returned from the other library functions.
    //!
    //! \return error description.
    //! \remarks function extensions are - {_a, _w, _u}:
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api char* innbill_error_description_a(int error_code);

    //! \copydoc innbill_error_description_a(int error_code)
    integration_api wchar_t* innbill_error_description_w(int error_code);

    //! \copydoc innbill_error_description_a(int error_code)
    integration_api unsigned char* innbill_error_description_u(int error_code);


    //! \brief  Frees memory allocated by an error_description(...).
    //!
    //! \param  err_description [in] The pointer to the memory allocated
    //!                              by error_description(...) function.
    //! \return If no error occurs, returns 0.
    //!         Otherwise, an error code is returned, A specific error
    //!         description can be retrieved by calling error_description(...).
    //! \remarks function extensions are - {_a, _w, _u}:
    //!            w - means that string param is in UNICODE encoding,
    //!            a - means that string param is in ascii encoding (by default russian ascii, can be changed from config),
    //!            u - means that string param is in utf8 encoding.
    integration_api int innbill_free_error_description_a(char* err_description);

    //! \copydoc innbill_free_error_description_a(char* err_description)
    integration_api int innbill_free_error_description_u(unsigned char* err_description);

    //! \copydoc innbill_free_error_description_a(char* err_description)
    integration_api int innbill_free_error_description_w(wchar_t* err_description);

    //! \}

    //! \}
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //INNOVA_BILLING_INTEGRATION_API_H

