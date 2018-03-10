/******************************************************************************
版权所有, 2011~2014, 曾华荣 (zeng_hr@163.com)
文件名: OS_MUTEX.H
作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年2月17日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年2月17日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/

#ifndef __OS_MUTEX_H__
#define __OS_MUTEX_H__

#include "os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define _EN_MUEX_

#ifdef _EN_MUEX_

typedef struct tagOS_CS
{
    OS_MUTEX     mutex; /* 对外生效的互斥锁 */
    
    OS_MUTEX     lock; /* 仅用来锁下面的成员 */
    uint32_t       cnt;
    OS_THREAD_ID pid;
} OS_CS;


extern void CSInit(OS_CS *cs, char *file, uint32_t line);
extern void CSEnter(OS_CS *cs, char *file, uint32_t line);
extern void CSLeave(OS_CS *cs, char *file, uint32_t line);
extern void CSDestroy(OS_CS *cs, char *file, uint32_t line);

#define OS_RWLOCK_INIT(cs)    CSInit(cs, __FILE__, __LINE__)
#define OS_RWLOCK_WRLOCK(cs)   CSEnter(cs, __FILE__, __LINE__)
#define OS_RWLOCK_WRUNLOCK(cs)   CSLeave(cs, __FILE__, __LINE__)
#define OS_RWLOCK_DESTROY(cs) CSDestroy(cs, __FILE__, __LINE__)

#endif

#ifdef __cplusplus
}
#endif

#endif

