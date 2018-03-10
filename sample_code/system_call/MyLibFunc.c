/*******************************************************************************
��Ȩ����, 2009~2012, ��Ϊ�������˿Ƽ����޹�˾
�ļ���: MYLIBFUNC.C
����: ������ (90003523)  ����: 2009��7��10��
��������: ͨ�õĹ���������
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (90003523)  ����: 2009��7��10��
  ------------------------------------------------------------------------------
  ��ʼ�汾
*******************************************************************************/
	
#include <stdlib.h>
#include <stdio.h>

#include "MyLibFunc.h"

int MyWriteLog(char *fmt, ...)     
{
	INT8U buf[1024];
	va_list args;
	
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	printf(buf);
	va_end(args); 
}

/*******************************************************************************
  ��������: SwapVar16
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
INT16U SwapVar16(INT16U dat)
{
	UNION16 ret;
	
	ret.b.l8 = ((UNION16 *)&dat)->b.h8;
	ret.b.h8 = ((UNION16 *)&dat)->b.l8;
	
	return ret.u16;
} // End of SwapVar16;
	
/*******************************************************************************
  ��������: SwapVar32
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
INT32U SwapVar32(INT32U dat)
{
	UNION32 ret;
	
	ret.b.l8 = ((UNION32 *)&dat)->b.h8;
	ret.b.ml8 = ((UNION32 *)&dat)->b.mh8;
	ret.b.mh8 = ((UNION32 *)&dat)->b.ml8;
	ret.b.h8 = ((UNION32 *)&dat)->b.l8;
	
	return ret.u32;
} // End of SwapVar32;
	
/*******************************************************************************
  ��������: SwapVar64
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
INT64U SwapVar64(INT64U dat)
{
	UNION64 ret;
	
	ret.b.l8 = ((UNION64 *)&dat)->b.h8;
	ret.b.ml8 = ((UNION64 *)&dat)->b.mh8;
	ret.b.mml8 = ((UNION64 *)&dat)->b.mmh8;
	ret.b.mmml8 = ((UNION64 *)&dat)->b.mmmh8;
	ret.b.mmmh8 = ((UNION64 *)&dat)->b.mmml8;
	ret.b.mmh8 = ((UNION64 *)&dat)->b.mml8;
	ret.b.mh8 = ((UNION64 *)&dat)->b.ml8;
	ret.b.h8 = ((UNION64 *)&dat)->b.l8;
	
	return ret.u64;
} // End of SwapVar64;
	
#ifdef _DEBUG_MEMORY_

static INT32U totalMem;
static INT32U maxMem;

/*******************************************************************************
  ��������: WMemInit
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
void WMemInit(void)
{
	totalMem = 0;
	maxMem = 0;
} // End of WMemInit;

/*******************************************************************************
  ��������: WMemAlloc
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
void *WMemAlloc(INT16U size)
{
	void *mem;
	
	mem = malloc(size);
	if (mem == NULL)
	{
		return NULL;
	}
	
	totalMem += size;
	if (maxMem < totalMem)
	{ // Record the max memrory used
		maxMem = totalMem;
	}

	PrintDebugToScreen("+Memory Used Now: %d Bytes\n", totalMem);
	
	return mem;
} // End of WMemAlloc;

/*******************************************************************************
  ��������: WMemCalloc
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
void *WMemCalloc(INT16U size)
{
	void *mem;
	
	mem = calloc(size, sizeof(char));
	if (mem == NULL)
	{
		return NULL;
	}
	
	totalMem += size;
	if (maxMem < totalMem)
	{ // Record the max memrory used
		maxMem = totalMem;
	}

	PrintDebugToScreen("+Memory Used Now: %d Bytes\n", totalMem);
	
	return mem;
} // End of WMemCalloc;

/*******************************************************************************
  ��������: WMemFree
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
void WMemFree(void *mem, INT16U size)
{
	totalMem -= size;
	free(mem);

	PrintDebugToScreen("-Memory Used Now: %d Bytes\n", totalMem);
} // End of WMemFree;

/*******************************************************************************
  ��������: WMemGetCur
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
INT32U WMemGetCur(void)
{
    return totalMem;
} /* End of WMemGetCur */

/*******************************************************************************
  ��������: WMemGetMax
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
INT32U WMemGetMax(void)
{
    return maxMem;
} /* End of WMemGetMax */

#endif // End of _DEBUG_MEMORY_ 
	
#include <Windows.h>

/*******************************************************************************
  ��������: AnsiToUnicode
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
unsigned char AnsiToUnicode(char *srcStr, unsigned short *desStr)
{
	unsigned char len;

	len = MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (LPCSTR)srcStr,
		-1, (LPWSTR)desStr, 0);
	
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, (LPCSTR)srcStr,
		-1, (LPWSTR)desStr, len);
		
	return len-1; // Exclude the '\0' character
} // End of AnsiToUnicode;

char outPutStr[256 * 2];
	
/*******************************************************************************
  ��������: UnicodeToAnsi
  ��������: 
  ���ú���: 
  ��������: 
  ���ʵı��: 
  ���µı��: 
  �������: 
  �������: 
  ����ֵ: 
  ����: 
*******************************************************************************/
char *UnicodeToAnsi(unsigned short *srcStr, unsigned char len)
{
	unsigned short size;
	
	size = WideCharToMultiByte(CP_OEMCP, WC_NO_BEST_FIT_CHARS, (LPCWSTR)srcStr,
		len, NULL, 0, NULL, FALSE);
	WideCharToMultiByte(CP_OEMCP, WC_NO_BEST_FIT_CHARS, (LPCWSTR)srcStr,
		len, outPutStr, size, NULL, FALSE);

	outPutStr[size] = 0; // The string end
		
	return outPutStr;
} // End of UnicodeToAnsi;


