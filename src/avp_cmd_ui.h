/*******************************************************************************

            ��Ȩ����(C), 2011~2014, ������ (zeng_hr@163.com)
********************************************************************************
�� �� ��: CMDUI.H
��    ��: ������ (zeng_hr@163.com)
��    ��: 1.00
��    ��: 2011��4��4��
��������: 
�����б�: 
    1. ...: 
�޸���ʷ: 
    �汾��1.00  ����: ������ (zeng_hr@163.com)  ����: 2011��4��4��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
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

