/******************************************************************************
��Ȩ����, 2011~2014, ������ (zeng_hr@163.com)
�ļ���: OSP_MUTEX.H
����: ������ (zeng_hr@163.com)  ����: 2011��2��17��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2011��2��17��
  -----------------------------------------------------------------------------
  ��ʼ�汾
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

