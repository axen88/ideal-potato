/*******************************************************************************

            版权所有(C), 2011~2014, 曾华荣 (zeng_hr@163.com)
********************************************************************************
文 件 名: OS_MUTEX.C
版    本: 1.00
日    期: 2011年4月4日
功能描述: 通过互斥锁实现嵌套锁(可重入锁) CS为Critical Section(临界区)的缩写
函数列表: 
    1. ...: 
修改历史: 
    版本：1.00  作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年4月4日
--------------------------------------------------------------------------------
    1. 初始版本
*******************************************************************************/
#include "os_adapter.h"
#include "os_mutex.h"

#ifdef _EN_MUEX_

#define INVALID_THREAD_ID ((OS_THREAD_ID)-1)

/*******************************************************************************
函数名称: CSInit
功能说明: 初始化锁
输入参数:
    cs: 待操作的可重入锁
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void CSInit(OS_CS *cs, char *f, uint32_t line)
{
    cs->pid = INVALID_THREAD_ID;
    cs->cnt = 0;
    OS_MUTEX_INIT(&cs->mutex);
    OS_MUTEX_INIT(&cs->lock);

    return;
}

/*******************************************************************************
函数名称: CSEnter
功能说明: 进入临界区，此时获取到互斥锁锁的线程可以重复进入临界区，
    其他线程则需等待
输入参数:
    cs: 待操作的可重入锁
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void CSEnter(OS_CS *cs, char *f, uint32_t line)
{
    OS_MUTEX_LOCK(&cs->lock);

    if (OS_GET_THREAD_ID() != cs->pid)
    {
        OS_MUTEX_UNLOCK(&cs->lock);
        
        /* 获取互斥锁 */
        OS_MUTEX_LOCK(&cs->mutex);
        
        OS_MUTEX_LOCK(&cs->lock);
        cs->pid = OS_GET_THREAD_ID();
        cs->cnt = 0;
    }

    ++cs->cnt;
    
    OS_MUTEX_UNLOCK(&cs->lock);
    
    return;
}

/*******************************************************************************
函数名称: CSLeave
功能说明: 离开临界区，如果是已经获取到锁的线程，则会检查计数，如果计数减到0，
    则解锁
输入参数:
    cs: 待操作的可重入锁
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void CSLeave(OS_CS *cs, char *f, uint32_t line)
{
    OS_MUTEX_LOCK(&cs->lock);
    
    if (OS_GET_THREAD_ID() != cs->pid)
    {
        LOG_EMERG("Try leave in another thread. cs(%p) f(%s) line(%d) cnt(%d) pid(%u)\n",
            cs, f, line, cs->cnt, cs->pid);
        OS_MUTEX_UNLOCK(&cs->lock);
        return;
    }
    
    if (0 == cs->cnt)
    {
        LOG_EMERG("Leave too many times."
            " [cs: %p, f: %s, line: %d, cnt: %d]\n",
            cs, f, line, cs->cnt);
        OS_MUTEX_UNLOCK(&cs->lock);
        return;
    }
    
    if (0 != --cs->cnt)
    {
        OS_MUTEX_UNLOCK(&cs->lock);
        return;
    }

    cs->pid = INVALID_THREAD_ID;
    OS_MUTEX_UNLOCK(&cs->lock);

    /* 释放互斥锁 */
    OS_MUTEX_UNLOCK(&cs->mutex);
    
    return;
}

/*******************************************************************************
函数名称: CSDestroy
功能说明: 销毁可重入锁
输入参数:
    cs: 待操作的可重入锁
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void CSDestroy(OS_CS *cs, char *f, uint32_t line)
{
    OS_MUTEX_DESTROY(&cs->mutex);
    OS_MUTEX_DESTROY(&cs->lock);

    return;
}

EXPORT_SYMBOL(CSEnter);
EXPORT_SYMBOL(CSLeave);
EXPORT_SYMBOL(CSInit);
EXPORT_SYMBOL(CSDestroy);

#endif

