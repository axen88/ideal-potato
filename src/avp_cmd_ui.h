/*******************************************************************************

            版权所有(C), 2011~2014, 曾华荣 (zeng_hr@163.com)
********************************************************************************
文 件 名: CMDUI.H
作    者: 曾华荣 (zeng_hr@163.com)
版    本: 1.00
日    期: 2011年4月4日
功能描述: 
函数列表: 
    1. ...: 
修改历史: 
    版本：1.00  作者: 曾华荣 (zeng_hr@163.com)  日期: 2011年4月4日
--------------------------------------------------------------------------------
    1. 初始版本
*******************************************************************************/
#ifndef _CMD_UI_H_
#define _CMD_UI_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _MY_CMD_LIST
{
	MY_FUNC_PTR func; // The func
	char *cmd; // The cmd to call this function
	char *comment; // The comment to this command
} MY_CMD_LIST;

extern void MyExeCmd(MY_CMD_LIST *cl, INT8U clLen);

#ifdef  __cplusplus
}
#endif

#endif /* End of _CMD_UI_H_ */

