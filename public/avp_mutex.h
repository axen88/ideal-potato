/******************************************************************************
版权所有, 2011~2014, 曾华荣 (zeng_hr@163.com)
文件名: OSP_MUTEX.H
作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年2月17日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年2月17日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/

#ifndef __OSP_MUTEX_H__
#define __OSP_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

#define _EN_MUEX_
    
#ifdef _EN_MUEX_
    
    #ifdef WIN32
    
#include <windows.h>
typedef CRITICAL_SECTION OSP_CS;

    #else

    #ifdef __KERNEL__
#include <linux/kthread.h>
typedef struct semaphore OSP_CS;
    #else
#include <pthread.h>
typedef pthread_mutex_t OSP_CS;
    #endif

    #endif /* End of WIN32 */

extern void CSInit(OSP_CS *v_pCs);
extern void CSEnter(OSP_CS *v_pCs);
extern void CSLeave(OSP_CS *v_pCs);
extern void CSDestroy(OSP_CS *v_pCs);

#else

typedef int OSP_CS;

#define CSInit(v_pCs)
#define CSEnter(v_pCs)
#define CSLeave(v_pCs)
#define CSDestroy(v_pCs)

#endif

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* End of __OSP_MUTEX_H__ */

