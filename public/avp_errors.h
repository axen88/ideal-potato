/*******************************************************************************

            版权所有(C), 2010~2013, 曾华荣 (EMail: zeng_hr@163.com)
********************************************************************************
文 件 名: OSP_ERRORS.H
作    者: 曾华荣 (EMail: zeng_hr@163.com)
版    本: V1.00
日    期: 2010年5月26日
功能描述: 
函数列表: 
    1. ...: 
修改历史: 
    作者: 曾华荣 (EMail: zeng_hr@163.com)    日期: 2010年5月26日
--------------------------------------------------------------------------------
    1. 初始版本
*******************************************************************************/
#ifndef __OSP_ERRORS_H__
#define __OSP_ERRORS_H__

/* 错误代码定义 */
typedef enum tagERROR_CODE_E
{
    ERROR_START = 2000,

} ERROR_CODE_E; /* End of ERROR_CODE_E */

/* 成功代码定义 */
typedef enum tagSUCCESS_CODE_E
{
    SUCCESS_START = 6000,
    
} SUCCESS_CODE_E; /* End of SUCCESS_CODE_E */

#endif /* End of __OSP_ERRORS_H__ */
