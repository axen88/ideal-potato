#ifndef _TLOG_HPP_
#define _TLOG_HPP_

#include <stdio.h>
#include "avp_mutex.h"

#define MAX_FILE_LINES    200000

#define LOG_TO_NULL        0
#define LOG_TO_FILE        1
#define LOG_TO_SCREEN    2
#define LOG_TO_SCNFILE    3

#ifndef WIN32
#define MAX_PATH            256
#endif
#define MAX_VERSION_LEN        256

typedef int CTbool;
#define CT_boolFALSE 0
#define CT_boolTRUE  1

class TLog
{
public:
    void SetVersion(const char *szVersion);
    TLog(const char *module, const char *version,int bakmode = 0, int multiuser = 1);
    virtual ~TLog();
    //打开日志文件
    FILE *open(int mode = LOG_TO_FILE);
    //关闭日志文件
    void close();
    //设置日志的输出方式：到nul,FILE（缺省）,SCREEN or both

    void setMode(int mode);
    //设置是否逐行刷新日志标志,0=不逐行刷新，1=逐行刷新（缺省）

    void setFlushFlag(int flag){ m_nFlushFlag = flag;}

    //设置日志文件最大行数
    void setMaxLines(int nMaxLines);

    //得到日志的输出方式
    int getMode() const { return m_nTraceMode; }

    //普通日志，开头打印时间
    void print(const char *format,...);
    void trace(int level, const char *format,...);

    //普通日志，开头打印时间和ID
    void print(int ID, const char *format,...);

    //普通日志，开头不打印时间
    void printnt(const char *format,...);
    void tracent(int level, const char *format,...);

    //二进制日志，开头打印时间
    void printb(const char *title, const unsigned char *buf,int len);
    void traceb(int level, const char *title, const unsigned char *buf,int len);

    //二进制日志，开头打印时间
    void printb(unsigned int ID, const char *title, const unsigned char *buf,int len);

    //二进制日志，开头不打印时间
    void printnb(const unsigned char *buf,int len);
    void tracenb(int level, const unsigned char *buf,int len);

    //打印错误日志，开头打印时间
    void perror(const char *format,...);

    //可以传递变长参数指针进行打印,开头打印时间和ID
    void vprint(int ID, const char *format, va_list argptr);

    //刷新日志
    void flush();

    void SetTraceLevelThreshold(int level);    //用于动态调整日志级别

private:
    //防止错误用法：printnt(0, "%d", 1234);
    void printnt(int , const char *format,...);
    void perror(int , const char *format,...);

    //备份
    void Backup();
    //打印版本信息
    void PrintVersion(void);
    CTbool CanTrace(int level);            //用于控制是否打印日志

    inline void LOCK()  { if(m_nMultiUser)  CSEnter(&m_Lock); }
    inline void UNLOCK()  { if(m_nMultiUser)  CSLeave(&m_Lock); }

private:
    FILE *m_FP;                            //文件描述符
    OSP_CS m_Lock;
    char m_acFileName[MAX_PATH];        //日志文件名
    char m_acBakFileName[MAX_PATH];        //备份日志命LOG_Bak
    char m_acErrorFileName[MAX_PATH];    //错误日志文件名
    int m_nTotalCount;                //总行数计数
    int m_nMaxLines;                    //日志文件的最大行数
    char m_acModuleName[MAX_PATH];        //模块名
    int m_nTraceMode;                    //日志输出位置：nul,文件,屏幕,both
    int m_nFlushFlag;                    //是否逐行刷新日志
    int m_nMultiUser;                    //是否多用户。多用户使用时需要加锁访问，单用户使用时不需要加锁
    char m_acVersion[MAX_VERSION_LEN];    //版本信息
    int m_iTraceLevelThreshold;
    int m_iBakMode;                        //备份方式 0 - 两文件交互备份 ，1- 永久备份 , > 2 循环备份
    int    m_iIdx;

}; 




#endif // _TLOG_HPP_
