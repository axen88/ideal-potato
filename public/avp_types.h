/*******************************************************************************

            版权所有(C), 2011~2014, 曾华荣 (zeng_hr@163.com)
********************************************************************************
文 件 名: AVP_TYPES.H
作    者: 曾华荣 (zeng_hr@163.com)
版    本: 1.00
日    期: 2011年4月4日
功能描述: 
函数列表: 
    1. ...: 
修改历史: 
    版本：1.00  作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年4月4日
--------------------------------------------------------------------------------
    1. 初始版本
*******************************************************************************/

#ifndef _AVP_TYPES_H_
#define _AVP_TYPES_H_

//==========================================================
// Complier options
//  1. System options
//  2. User defined options
//==========================================================
//#pragma pack(1)  // Aligned by 1 byte
//#pragma pack()  // Use the default alignment

//#define _MCS51_
#define _ENABLE_DEBUG_
#ifdef WIN32
//#define _BIG_ENDIAN_ // It is Big Endian system for 8051
#else
#define _BIG_ENDIAN_ // It is Big Endian system for 8051
#endif
#define _DEBUG_MEMORY_
#define _EN_STANDARD_LIB_
//#define _SUPPORT_64BITS_ // Support 64 bits data type

//==========================================================
// Basic types
//==========================================================
typedef unsigned int         AVP_BOOL; // 1-bit
//typedef void                 AVP_VOID; // AVP_VOID
#define  AVP_VOID void // C++ 要求

typedef unsigned char        AVP_U8;   // unsigned 8-bit
typedef unsigned short       AVP_U16;  // unsigned 16-bit
typedef unsigned int         AVP_U32;  // unsigned 32-bit
typedef unsigned long long   AVP_U64;  // unsigned 64-bit

typedef char                 AVP_S8;   // signed 8-bit
typedef short                AVP_S16;  // signed 16-bit
typedef int                  AVP_S32;  // signed 32-bit
typedef long long            AVP_S64;  // signed 64-bit

#ifdef WIN32
typedef unsigned long long       AVP_ULONG;  // pointer
typedef long long       AVP_LONG;  // pointer
#else
typedef unsigned long            AVP_ULONG;  // pointer
typedef long            AVP_LONG;  // pointer
#endif

#define B_FALSE   0
#define B_TRUE    1

#ifndef SUCCESS
#define SUCCESS 0
#endif

#endif /* End of _AVP_TYPES_H_ */

