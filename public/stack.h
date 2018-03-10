#ifndef __STACK_H__
#define __STACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagSTACK_ERROR_CODE_E
{
    ERR_STACK_FULL = 1,
    ERR_STACK_EMPTY,
} STACK_ERROR_CODE_E; /* End of tagSTACK_ERROR_CODE_E */

typedef struct tagSTACK_S
{
    void **memb;
    int num;
    int max;
} STACK_S; /* End of STACK_S */

static inline STACK_S *StackCreate(int size)
{
    STACK_S *s = NULL;

    s = malloc(sizeof(STACK_S));
    if (NULL == s)
    {
        return NULL;
    }

    s->memb = malloc(sizeof(void*) * size);
    if (NULL == s->memb)
    {
        free(s);
        return NULL;
    }

    s->num = 0;
    s->max = size;

    return s;
}

static inline int StackPush(STACK_S *s, void *memb)
{
    ASSERT(s);
    ASSERT(memb);

    if (s->num >= s->max)
    {
        return -ERR_STACK_FULL;
    }

    s->memb[s->num++] = memb;

    return 0;
}

static inline int StackPop(STACK_S *s, void **memb)
{
    ASSERT(s);
    ASSERT(memb);

    if (s->num <= 0)
    {
        return -ERR_STACK_EMPTY;
    }

    *memb = s->memb[--s->num];

    return 0;
}

static inline int StackGetSize(STACK_S *s)
{
    ASSERT(s);

    return s->num;
}

static inline void StackDestroy(STACK_S *s)
{
    ASSERT(s);

    free(s->memb);
    free(s);
}

#ifdef __cplusplus
}
#endif /* End of __cplusplus */


#endif /* End of __STACK_H__ */
