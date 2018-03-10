	
#ifndef _SLIST_H_
#define _SLIST_H_

typedef struct tagSLIST_HEAD_S
{
	struct tagSLIST_HEAD_S *pNext;
} SLIST_HEAD_S;

/**
* SListInit - Initialize list head
*/
extern
void
SListInit(
	SLIST_HEAD_S *v_pEntry
	);

/**
* SListAdd - add a new entry
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
extern
void
SListAddHead(
	SLIST_HEAD_S *v_pHead,  // SList head to add it after
	SLIST_HEAD_S *v_pEntry // Entry to be added
	);

/**
* SListAddTail - add a new entry
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
extern
void
SListAddTail(
	SLIST_HEAD_S *v_pHead,  // SList head to add it before
	SLIST_HEAD_S *v_pEntry // Entry to be added
	);

extern
SLIST_HEAD_S *
SListGetEntry(
    SLIST_HEAD_S *v_pHead, // The list to test
    int position
    );
    
extern int SListRemoveTargetEntry(SLIST_HEAD_S *v_pHead, int v_cnt);

extern
int
SListWalkAll(
    SLIST_HEAD_S *v_pHead, // The list to test
    int (v_pFunc)(void *)
    );

/**
* SListEmpty - tests whether a list is empty
*/
extern
int
SListIsEmpty(
	SLIST_HEAD_S *v_pHead // The list to test
	);

extern
int
SListCount(
	SLIST_HEAD_S *v_pHead // The list to test
	);

#endif // End of _SLIST_H_ 


