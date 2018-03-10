/*******************************************************************************

            版权所有(C), 2011~2014, 曾华荣 (zeng_hr@163.com)
********************************************************************************
文 件 名: OSP_MUTEX.C
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
#include "avp_mutex.h"

#ifdef _EN_MUEX_

void CSInit(OSP_CS *v_pCs)
{
#ifdef WIN32
    InitializeCriticalSection(v_pCs);
#else
    #ifdef __KERNEL__
    init_MUTEX(v_pCs);
    #else
    pthread_mutexattr_t mutexattr;

    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(v_pCs, &mutexattr);
    #endif
#endif /* End of WIN32 */
}

void CSEnter(OSP_CS *v_pCs)
{
#ifdef WIN32
    EnterCriticalSection(v_pCs);
#else
    #ifdef __KERNEL__
    down(v_pCs);
    #else
    pthread_mutex_lock(v_pCs);
    #endif
#endif /* End of WIN32 */
}

void CSLeave(OSP_CS *v_pCs)
{
#ifdef WIN32
    LeaveCriticalSection(v_pCs);
#else
    #ifdef __KERNEL__
    up(v_pCs);
    #else
    pthread_mutex_unlock(v_pCs);
    #endif
#endif /* End of WIN32 */
}

void CSDestroy(OSP_CS *v_pCs)
{
#ifdef WIN32
    DeleteCriticalSection(v_pCs);
#else
    #ifndef __KERNEL__
    pthread_mutex_destroy(v_pCs);
    #endif
#endif /* End of WIN32 */
}

#endif

