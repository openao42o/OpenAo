#pragma once

typedef UINT32 Err_t;										// 에러 타입을 위해

#define ERR_NO_ERROR				(Err_t)0x00000000

// DB Error
#define ERR_DB_NO_DATA					(Err_t)0x00000010
//#define ERR_DB_SOURCE_CONNECT_FAIL		(Err_t)0x00001000
//#define ERR_DB_DEST_CONNECT_FAIL		(Err_t)0x00002000	
#define ERR_DB_COM_CONNECT_FAIL			(Err_t)0x80004005
#define ERR_DB_COM_DUPLICATE_INSERT		(Err_t)0x80040e2f
#define ERR_DB_COM_PROCEDURE			(Err_t)0x80040e14

