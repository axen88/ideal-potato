#ifndef __OSP_QUEUE_HPP__
#define __OSP_QUEUE_HPP__

#include <queue>
#include "avp_mutex.h"

typedef enum tagQUEUE_ERROR_CODE_E
{
    ERR_QUEUE_MALLOC = 100,
    ERR_QUEUE_INVALID_PARA,
    ERR_QUEUE_FULL,
    ERR_QUEUE_EMPTY,
} QUEUE_ERROR_CODE_E; /* End of tagQUEUE_ERROR_CODE_E */

class CTQueue
{
public:
    CTQueue(unsigned int v_maxsize);
    ~CTQueue();
    int Push(void *v_pMemb);
    int Pop(void **v_ppMemb);
    int PopPush(void *v_pMembPush, void **v_ppMembPop);
    unsigned int GetSize(void);
        
private:
    CTQueue();
    ::std::queue < void * > m_q;
    OSP_CS m_cs;
    unsigned int m_maxsize;
}; /* End of CTQueue */

#endif /* End of __OSP_QUEUE_HPP__ */
