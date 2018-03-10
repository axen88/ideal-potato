#ifndef __OSP_THREDS_GROUP_H__
#define __OSP_THREDS_GROUP_H__

#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagTHREADS_GROUP_ERROR_CODE_E
{
    ERR_THREADS_GROUP_INVALID_PARA = 300,
} THREADS_ARRAY_ERROR_CODE_E; /* End of tagTHREADS_ARRAY_ERROR_CODE_E */

extern void *ThreadsGroupCreate(int v_num, void *(*v_pFunc)(void *),
    void *v_pArg);
extern int ThreadsGroupGetRealNum(void *v_pThreadGroup);
extern int ThreadsGroupDestroy(void *v_pThreadGroup, int v_force,
    unsigned long long v_msOverTime);


#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* End of __OSP_THREDS_GROUP_H__ */

