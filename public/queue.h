#ifndef __OSP_QUEUE_H__
#define __OSP_QUEUE_H__


#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagQUEUE_ERROR_CODE_E
{
    ERR_QUEUE_FULL=1,
    ERR_QUEUE_EMPTY,
} QUEUE_ERROR_CODE_E; /* End of tagQUEUE_ERROR_CODE_E */


typedef struct tagQUEUE_S
{
    void **memb;
    int head;
    int tail;
    int size;
} QUEUE_S; /* End of QUEUE_S */

static inline QUEUE_S *queue_create(int size)
{
    QUEUE_S *q = NULL;

    q = (QUEUE_S *)malloc(sizeof(QUEUE_S));
    if (NULL == q)
    {
        return NULL;
    }

    q->memb = (void **)malloc(sizeof(void*) * size);
    if (NULL == q->memb)
    {
        free(q);
        return NULL;
    }

    q->head = 0;
    q->tail = 0;
    q->size = size;

    return q;
}

static inline int queue_push(QUEUE_S *q, void *memb)
{
    ASSERT(q);

    if (((q->tail + 1) % q->size) == q->head)
    {
        return -ERR_QUEUE_FULL;
    }

    q->memb[q->tail++] = memb;
    q->tail %= q->size;

    return 0;
}

static inline int queue_pop(QUEUE_S *q, void **memb)
{
    ASSERT(q);

    if (q->head == q->tail)
    {
        return -ERR_QUEUE_EMPTY;
    }

    *memb = q->memb[q->head++];
    q->head %= q->size;

    return 0;
}

static inline int queue_get_size(QUEUE_S *q)
{
    ASSERT(q);

    return (q->tail - q->head + q->size) % q->size;
}

static inline void queue_destroy(QUEUE_S *q)
{
    ASSERT(q);

    free(q->memb);
    free(q);
}

#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif /* End of __OSP_QUEUE_H__ */
