/******************************************************************************
��Ȩ����, 2011~2014, ������ (zeng_hr@163.com)
�ļ���: OS_MUTEX.H
����: ������ (zeng_hr@163.com)  ����: 2011��2��17��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2011��2��17��
  -----------------------------------------------------------------------------
  ��ʼ�汾
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
    OS_MUTEX     mutex; /* ������Ч�Ļ����� */
    
    OS_MUTEX     lock; /* ������������ĳ�Ա */
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

