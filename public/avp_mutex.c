/*******************************************************************************

            ��Ȩ����(C), 2011~2014, ������ (zeng_hr@163.com)
********************************************************************************
�� �� ��: OSP_MUTEX.C
��    ��: ������ (zeng_hr@163.com)
��    ��: 1.00
��    ��: 2011��4��4��
��������: 
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2011��4��4��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
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

