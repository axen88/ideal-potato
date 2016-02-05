#ifndef __OSP_STACK_HPP__
#define __OSP_STACK_HPP__

#include <stack>
#include "avp_mutex.h"

typedef enum tagSTACK_ERROR_CODE_E
{
    ERR_STACK_MALLOC = 100,
    ERR_STACK_INVALID_PARA,
    ERR_STACK_FULL,
    ERR_STACK_EMPTY,
} STACK_ERROR_CODE_E; /* End of tagSTACK_ERROR_CODE_E */

class CTStack
{
public:
    CTStack(unsigned int v_maxsize);
    ~CTStack();
    int Push(void *v_pMemb);
    int Pop(void **v_ppMemb);
    int PopPush(void *v_pMembPush, void **v_ppMembPop);
    unsigned int GetSize(void);
        
private:
    CTStack();
    ::std::stack < void * > m_s;
    OSP_CS m_cs;
    unsigned int m_maxsize;
}; /* End of CTStack */

#endif /* End of __OSP_STACK_HPP__ */
