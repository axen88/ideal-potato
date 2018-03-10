/*******************************************************************************

            ��Ȩ����(C), 2011~2014, ������ (zeng_hr@163.com)
********************************************************************************
�� �� ��: OS_MUTEX.C
��    ��: 1.00
��    ��: 2011��4��4��
��������: ͨ��������ʵ��Ƕ����(��������) CSΪCritical Section(�ٽ���)����д
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2011��4��4��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
*******************************************************************************/
#include "os_adapter.h"
#include "os_mutex.h"

#ifdef _EN_MUEX_

#define INVALID_THREAD_ID ((OS_THREAD_ID)-1)

/*******************************************************************************
��������: CSInit
����˵��: ��ʼ����
�������:
    cs: �������Ŀ�������
�������: ��
�� �� ֵ: ��
˵    ��: ��
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
��������: CSEnter
����˵��: �����ٽ�������ʱ��ȡ�������������߳̿����ظ������ٽ�����
    �����߳�����ȴ�
�������:
    cs: �������Ŀ�������
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
void CSEnter(OS_CS *cs, char *f, uint32_t line)
{
    OS_MUTEX_LOCK(&cs->lock);

    if (OS_GET_THREAD_ID() != cs->pid)
    {
        OS_MUTEX_UNLOCK(&cs->lock);
        
        /* ��ȡ������ */
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
��������: CSLeave
����˵��: �뿪�ٽ�����������Ѿ���ȡ�������̣߳����������������������0��
    �����
�������:
    cs: �������Ŀ�������
�������: ��
�� �� ֵ: ��
˵    ��: ��
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

    /* �ͷŻ����� */
    OS_MUTEX_UNLOCK(&cs->mutex);
    
    return;
}

/*******************************************************************************
��������: CSDestroy
����˵��: ���ٿ�������
�������:
    cs: �������Ŀ�������
�������: ��
�� �� ֵ: ��
˵    ��: ��
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

