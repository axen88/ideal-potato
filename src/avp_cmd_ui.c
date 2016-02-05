/*******************************************************************************

            版权所有(C), 2011~2014, 曾华荣 (zeng_hr@163.com)
********************************************************************************
文 件 名: CMDUI.C
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "MyTypes.h"
#include "avp_cmd_ui.h"

void PrintUsage(MY_CMD_LIST *cl, INT8U clLen)
{
	INT8U j;
	
	PrintNormalToScreen("********************************************************************\n");
	PrintNormalToScreen("User defined command list:\n");
	PrintNormalToScreen("--------------------------------------------------------------------\n");
	for (j = 0; j < clLen; j++)
	{
		PrintNormalToScreen(cl[j].cmd);
		PrintNormalToScreen(cl[j].comment);
	}
    
	PrintNormalToScreen("\n");
	PrintNormalToScreen("Default command list:\n");
	PrintNormalToScreen("--------------------------------------------------------------------\n");
	PrintNormalToScreen("help\tPrint this information\n");
	PrintNormalToScreen("quit\tExit the program\n");
	PrintNormalToScreen("********************************************************************\n");
} /* End of PrintUsage */


void MyExeCmd(MY_CMD_LIST *cl, INT8U clLen)
{
	PrintUsage(cl, clLen);
	
	while (1)
	{
		INT8U i;
		char cmd[1024];
		
		PrintNormalToScreen("\nPlease enter your cmd:\n");
		PrintNormalToScreen(">");
		
		scanf("%s", cmd);
		
		for (i = 0; i < clLen; i++)
		{
			if (strncmp(cl[i].cmd, cmd, strlen(cmd)) == 0)
			{ // Get the user command
				cl[i].func();
				break;
			}
			else if (strncmp("help", cmd, strlen(cmd)) == 0)
			{ // Get the default help command
				PrintUsage(cl, clLen);
				break;
			}
			else if (strncmp("quit", cmd, strlen(cmd)) == 0)
			{ // Get the default quit command
				return;
				break;
			}
		}
		
		if (i == clLen)
		{
			PrintNormalToScreen("Command not found\n");
		}
	}
} // End of GetFuncNo;


