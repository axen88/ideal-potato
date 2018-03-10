#ifndef __OSP_ASYNC_WORK_QUEUE_H__
#define __OSP_ASYNC_WORK_QUEUE_H__

#include <stdio.h>

class CTAWQTask
{
public:
    CTAWQTask(int v_taskId)
    {
        m_taskId = v_taskId;
        m_taskState = 0;
    };

    virtual ~CTAWQTask()
    {

    };

    virtual int OnRun(void)
    {
        if (++m_taskState < 5)
        {
            printf("Task running. [m_taskId: %d, m_taskState: %d]\n",
                m_taskId, m_taskState);
            return 1;
        }
        
        printf("Task finished. [m_taskId: %d, m_taskState: %d]\n",
            m_taskId, m_taskState);

        return 0;
    };

    int GetTaskId(void)
    {
        return m_taskId;
    };

private:
    CTAWQTask();
    int m_taskState;
    int m_taskId;
}; /* End of CTAWQTask */

class CTAsyncWorkQueue
{
public:
    CTAsyncWorkQueue();
    ~CTAsyncWorkQueue();
    int Start(int v_threadsNum, int v_runQSize, int v_endQSize);
    int TryPutTask(CTAWQTask *v_pTask);
    int TryGetTask(CTAWQTask **v_ppTask);

private:
    void *m_pRunQ;
    void *m_pEndQ;
    void *m_pThreadsGroup;
    int m_stopped;
    
    friend void *ThreadWork(void *v_pObj);
}; /* End of CTAsyncWorkQueue */


#endif /* End of __OSP_ASYNC_WORK_QUEUE_H__ */

