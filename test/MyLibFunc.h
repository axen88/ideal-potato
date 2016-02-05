/*******************************************************************************
版权所有, 2009~2012, 华为赛门铁克科技有限公司
文件名: MYLIBFUNC.H
作者: 曾华荣 (90003523)  日期: 2009年7月10日
功能描述: 通用的公共函数库
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (90003523)  日期: 2009年7月10日
  ------------------------------------------------------------------------------
  初始版本
*******************************************************************************/

#ifndef _MY_LIB_FUNC_H_
#define _MY_LIB_FUNC_H_

#include "MyTypes.h"

	#ifdef _DEBUG_MEMORY_

extern void WMemInit(void);
extern void *WMemAlloc(INT16U size);
extern void *WMemCalloc(INT16U size);
extern void WMemFree(void *mem, INT16U size);
extern INT32U WMemGetCur(void);
extern INT32U WMemGetMax(void);

	 #else
	 
#include <stdlib.h>

#define WMemInit()
#define WMemAlloc(size)     malloc(size)
#define WMemCalloc(size)    calloc(size)
//#define WMemCalloc(size) calloc(size, sizeof(INT8U))
#define WMemFree(mem, size) free(mem)
#define WMemGetMax() 0
#define WMemGetCur() 0

	#endif // End of _DEBUG_MEMORY_ 


extern INT16U SwapVar16(INT16U dat);
extern INT32U SwapVar32(INT32U dat);
extern INT64U SwapVar64(INT64U dat);

extern unsigned char AnsiToUnicode(char *srcStr, unsigned short *desStr);
extern char *UnicodeToAnsi(unsigned short *srcStr, unsigned char len);
	
#endif /* End of _MY_LIB_FUNC_H_ */

