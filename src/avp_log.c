/*
* 使用gcc -o avp_log avp_log.c avp_mutex.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>

#ifdef WIN32
#include <direct.h>
#define snprintf _snprintf
#define unlink   _unlink
#define mkdir    _mkdir
#define vsnprintf  _vsnprintf
#define sleep(x) Sleep((x) * 1000)
#define MkDir(name)  mkdir(name)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MkDir(name)  mkdir(name, 0777)
#endif

#include "avp_mutex.h"
#include "avp_log.h"

#ifdef _EN_DEBUG_
#define MAX_FILE_LINES    3
#else
#define MAX_FILE_LINES    200000
#endif

#define FILE_NAME_LEN     256
#define MAX_VERSION_LEN   256

#define DATA_TIME_STR_LEN 20
#define BUF_LEN           1024

#define FlushLog(v_f)     fflush(v_f)  // 逐行刷新日志

typedef struct tagLOG_S
{
    char dir[FILE_NAME_LEN];              // 日志文件所在的目录
    char fileName[FILE_NAME_LEN];         // 日志文件名
    char version[MAX_VERSION_LEN];        // 版本信息
    int  totalLines;                      // 总行数计数
    int  traceMode;                       // 日志输出位置：nul,文件,屏幕,both
    int  traceLevel;                      // 日志级别
    FILE *pf;                             // 日志文件句柄
    OSP_CS cs;                            // 日志锁
} LOG_S; 

static int GetDateTimeStr(char *v_pStr, int v_strSize)
{
    time_t curr_time = 0;
    struct tm *pt = NULL;

    if (NULL == v_pStr)
    {
        return -1;
    }
    
    v_pStr[0] = 0;
    
    curr_time = time(NULL);
    pt = localtime(&curr_time);
    if (NULL == pt)
    {
        return -2;
    }

    snprintf(v_pStr, v_strSize, "%04d-%02d-%02d %02d:%02d:%02d", 
        pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday, 
        pt->tm_hour, pt->tm_min, pt->tm_sec);
    v_pStr[v_strSize - 1] = 0;
    
    return 0;
}

// 动态调整日志级别
void LogSetTraceLevel(void *v_pLog, int v_level)
{
    if (NULL == v_pLog)
    {
        return;
    }
    
    ((LOG_S *)v_pLog)->traceLevel = v_level;
}

static FILE *OpenLog(void *v_pLog)
{
    char fileName[FILE_NAME_LEN];
    LOG_S *pLog = (LOG_S *)v_pLog;

    assert(NULL != pLog);
    
    snprintf(fileName, FILE_NAME_LEN, "%s/%s.log", pLog->dir, pLog->fileName);
    fileName[FILE_NAME_LEN - 1] = 0;
    
    pLog->pf = fopen(fileName, "wt");
    if (NULL != pLog->pf)
    {
        fprintf(pLog->pf, "%s %s\n", pLog->fileName, pLog->version);
        fflush(pLog->pf);
    }

    return pLog->pf;
}

static int BackupLog(void *v_pLog)
{
    LOG_S *pLog = (LOG_S *)v_pLog;
    int ret = 0;
    char bakName[FILE_NAME_LEN];
    char fileName[FILE_NAME_LEN];
    time_t t = 0;    
    struct tm *ts = NULL;

    if (0 == (pLog->traceMode & LOG_TO_FILE))
    {
        return 0;
    }
    
    MkDir(pLog->dir);
    
    CSEnter(&pLog->cs);
    
    t = time(NULL);
    ts = localtime(&t);
    if (NULL == ts)
    {
        snprintf(bakName, FILE_NAME_LEN, "%s/%s.log.bak",
            pLog->dir, pLog->fileName);
    }
    else
    {
        snprintf(bakName, FILE_NAME_LEN, "%s/%s_%04d%02d%02d_%02d%02d%02d.log", 
            pLog->dir, pLog->fileName, ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, 
            ts->tm_hour, ts->tm_min, ts->tm_sec);
    }

    bakName[FILE_NAME_LEN - 1] = 0;
    
    snprintf(fileName, FILE_NAME_LEN, "%s/%s.log", pLog->dir, pLog->fileName);
    fileName[FILE_NAME_LEN - 1] = 0;

    /* 关闭之前已经打开的文件 */
    if (NULL != pLog->pf)
    {
        fclose(pLog->pf);
        pLog->pf = NULL;
    }
    
    unlink(bakName);
    rename(fileName, bakName);

    if (NULL == OpenLog(pLog))
    {
        ret = -1;
    }

    pLog->totalLines = 0;

    CSLeave(&pLog->cs);
    
    return ret;
}

void *LogOpen(const char *fileName, const char *version,
    const char *dir, int traceLevel, int traceMode)
{
    LOG_S *pLog = NULL;

    if ((NULL == fileName) || (NULL == version) || (NULL == dir))
    {
        return NULL;
    }
    
    pLog = (LOG_S *)malloc(sizeof(LOG_S));
    if (NULL == pLog)
    {
        return NULL;
    }

    memset(pLog, 0, sizeof(LOG_S));
    
    CSInit(&pLog->cs);

    strncpy(pLog->dir, dir, FILE_NAME_LEN);
    pLog->dir[FILE_NAME_LEN - 1] = 0;
    strncpy(pLog->fileName, fileName, FILE_NAME_LEN);
    pLog->fileName[FILE_NAME_LEN - 1] = 0;
    strncpy(pLog->version, version, FILE_NAME_LEN);
    pLog->version[FILE_NAME_LEN - 1] = 0;

    pLog->traceMode = traceMode;
    LogSetTraceLevel(pLog, traceLevel);
    
    if (0 > BackupLog(pLog))
    {
        free(pLog);
        return NULL;
    }
    
    return pLog;
} /* End of LogOpen */

/*******************************************************************************
函数名称: LogClose
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void LogClose(void *v_pLog)
{
    LOG_S *pLog = (LOG_S *)v_pLog;
    
    if (NULL == pLog)
    {
        return;
    }

    if (NULL != pLog->pf)
    {
        char dt[DATA_TIME_STR_LEN];
        
        GetDateTimeStr(dt, DATA_TIME_STR_LEN);
        fprintf(pLog->pf, "%s %s\n", dt, "NOTE: LOG FILE CLOSE!!!");
        fclose(pLog->pf);
        pLog->pf = NULL;
    }

    CSDestroy(&pLog->cs);
    free(pLog);
} /* End of LogClose */

void LogTrace(void *v_pLog, int level, const char *format, ...)
{
    LOG_S *pLog = (LOG_S *)v_pLog;
    char dt[DATA_TIME_STR_LEN];
    char buffer[BUF_LEN];
    va_list ap;

    if ((NULL == pLog) || (level > pLog->traceLevel)
        || (0 == (pLog->traceMode & LOG_TO_SCNFILE))
        || ((0 == (pLog->traceMode & LOG_TO_SCREEN)) && (NULL == pLog->pf)))
    {
        return;
    }

    CSEnter(&pLog->cs);
    
    GetDateTimeStr(dt, DATA_TIME_STR_LEN);
    va_start(ap,format);

    vsnprintf(buffer, BUF_LEN, format, ap);
    buffer[BUF_LEN - 1] = 0;

    if (0 != (pLog->traceMode & LOG_TO_SCREEN))
    {
        printf("%s %s", dt, buffer);
    }
    
    if (NULL != pLog->pf)
    {
        fprintf(pLog->pf, "%s %s", dt, buffer);
        FlushLog(pLog->pf);
        pLog->totalLines++;
    }
    
    va_end(ap);
    
    CSLeave(&pLog->cs);
    
    if (pLog->totalLines > MAX_FILE_LINES)
    {
        BackupLog(pLog);
    }
}

#ifdef _EN_DEBUG_

int main(int argc, char *argv[])
{
    int i;
    void *pLog = LogOpen("test", "V100R001C01", "./log", 5, LOG_TO_SCNFILE);
    //void *pLog = LogOpen("test", "V100R001C01", "./log", 5, LOG_TO_FILE);
    //void *pLog = LogOpen("test", "V100R001C01", "./log", 5, LOG_TO_SCREEN);
    if (NULL == pLog)
    {
        printf("Open log failed.\n");
        return -1;
    }

    sleep(1);
    
    i = 0;
    LogTraceEmerg(pLog, "Test i. [%d]\n", i++);
    LogTraceError(pLog, "Test i. [%d]\n", i++);
    LogTraceWarning(pLog, "Test i. [%d]\n", i++);
    LogTraceInfo(pLog, "Test i. [%d]\n", i++);
    LogTraceDebug(pLog, "Test i. [%d]\n", i++);
    
    LogClose(pLog);

    system("pause");

    return 0;
} /* End of DebugLog */

#endif

