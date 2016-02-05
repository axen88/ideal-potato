#ifndef __OSP_STACK_H__
#define __OSP_STACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

typedef enum tagSTACK_ERROR_CODE_E
{
    ERR_STACK_MALLOC = 200,
    ERR_STACK_INVALID_PARA,
    ERR_STACK_FULL,
    ERR_STACK_EMPTY,
} STACK_ERROR_CODE_E; /* End of tagSTACK_ERROR_CODE_E */

extern void *StackCreate(int v_size);
extern int StackPush(void *v_pS, void *v_pMemb);
extern int StackPop(void *v_pS, void **v_ppMemb);
extern int StackPopPush(void *v_pS, void *v_pMembPush, void **v_ppMembPop);
extern int StackGetSize(void *v_pS);
extern int StackDestroy(void *v_pS);

#ifdef __cplusplus
}
#endif /* End of __cplusplus */


#endif /* End of __OSP_STACK_H__ */
