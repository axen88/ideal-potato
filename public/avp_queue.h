#ifndef __OSP_QUEUE_H__
#define __OSP_QUEUE_H__


#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagQUEUE_ERROR_CODE_E
{
    ERR_QUEUE_MALLOC = 100,
    ERR_QUEUE_INVALID_PARA,
    ERR_QUEUE_FULL,
    ERR_QUEUE_EMPTY,
} QUEUE_ERROR_CODE_E; /* End of tagQUEUE_ERROR_CODE_E */

extern void *QueueCreate(int v_size);
extern int QueuePush(void *v_pQ, void *v_pMemb);
extern int QueuePop(void *v_pQ, void **v_ppMemb);
extern int QueuePopPush(void *v_pQ, void *v_pMembPush, void **v_ppMembPop);
extern int QueueGetSize(void *v_pQ);
extern int QueueDestroy(void *v_pQ);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* End of __OSP_QUEUE_H__ */
