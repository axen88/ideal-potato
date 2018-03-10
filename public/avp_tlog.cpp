#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <time.h>

#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif

#include "tlog.h"

inline int PrintDateTimeSignature(FILE *fp)
{
    time_t curr_time = time(NULL);
    struct tm *pt = NULL;
    
    #ifdef WIN32
        pt = localtime(&curr_time);
    #else
        struct tm t1;
        pt = localtime_r(&curr_time, &t1);
    #endif
    
    if (pt)
    {
        fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d ", 
                pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday, 
                pt->tm_hour, pt->tm_min, pt->tm_sec);
        return 0;
    }
    return -1;
}

TLog::TLog(const char *module,const char *version,int bakmode, int multiuser)
{
    m_nMultiUser = multiuser;
    m_iTraceLevelThreshold = 0;
    m_iBakMode = bakmode;
    m_iIdx = 0;

    CSInit(&m_Lock);

    LOCK();

    m_nTotalCount = 0;

    m_nMaxLines = MAX_FILE_LINES;
    m_nTraceMode = LOG_TO_NULL;
    m_nFlushFlag = 1;
    m_FP = NULL;

    assert(module);
    assert(version);

    strcpy(m_acModuleName,module);
    strcpy(m_acVersion,version);

#ifdef WIN32
    mkdir("./log");
#else
    mkdir("./log", 0777);
#endif
    
    sprintf(m_acFileName, "./log/%s.log", m_acModuleName);
    sprintf(m_acErrorFileName, "./log/%s.err", m_acModuleName);

    UNLOCK();
}

TLog::~TLog()
{
    LOCK();
    if(m_FP)    
    {
        fclose(m_FP);
        m_FP = NULL;
    }
    UNLOCK();
}

FILE * TLog::open(int mode)
{
    m_nTraceMode = mode;
    if(mode != LOG_TO_NULL && mode != LOG_TO_FILE 
       && mode != LOG_TO_SCREEN &&    mode != LOG_TO_SCNFILE)
    {
        m_nTraceMode = LOG_TO_FILE;
    }

    if( mode == LOG_TO_NULL || mode == LOG_TO_SCREEN )
        return NULL ;

    LOCK();

    if( m_FP )
        fclose(m_FP);

    char fn_bak[MAX_PATH];
    char fp_bak[MAX_PATH];

    time_t now = time(0);    
#ifdef WIN32
    struct tm *ts = localtime(&now);
#else
    struct tm t1;
    struct tm *ts = localtime_r(&now,&t1);
#endif
    
    if(access("./log", 0) == 0)
        strcpy(fp_bak, "./log/");
    else
        strcpy(fp_bak, "./");

    if (ts)
           sprintf( fn_bak, "%s%04d%02d%02d%02d%02d%02d_%s.log", 
                fp_bak, ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, 
                ts->tm_hour, ts->tm_min, ts->tm_sec, m_acModuleName );
    else
        sprintf( fn_bak, "%s%s.log", fp_bak, m_acModuleName ) ;

    unlink( fn_bak ) ;
    rename( m_acFileName, fn_bak);
    
    //backup log_bak file by datatime
    if (ts)
           sprintf( fn_bak, "%s%04d%02d%02d%02d%02d%02d_%s.log_bak", 
                fp_bak, ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, 
                ts->tm_hour, ts->tm_min, ts->tm_sec, m_acModuleName );
    else
        sprintf( fn_bak, "%s%s.log_bak", fp_bak, m_acModuleName ) ;

    unlink( fn_bak ) ;
    rename( m_acBakFileName, fn_bak);
    //

    m_FP = fopen(m_acFileName, "wt");
    PrintVersion();

    UNLOCK();
    return m_FP;
}

void TLog::close()
{
    LOCK();

    if(m_FP)
    {
        PrintDateTimeSignature(m_FP);
        fprintf(m_FP, "%s\n", "NOTE: LOG FILE CLOSE!!!");
        fclose(m_FP);
        m_FP = NULL;
    }

    UNLOCK();
}

void TLog::setMode(int mode)
{
    LOCK();

    if((m_nTraceMode == LOG_TO_FILE || m_nTraceMode == LOG_TO_SCNFILE) && m_FP)    
        fprintf( m_FP, "SET TRACE MODE(%d)\n", mode ) ;

    m_nTraceMode = mode;    
    if( mode == LOG_TO_NULL || mode == LOG_TO_SCREEN ) 
    {
        if( m_FP )
        {
            fprintf( m_FP, "TRACE OFF\n" ) ;
            fclose(m_FP);
            m_FP = NULL;
        }
    }
    else if( mode == LOG_TO_FILE || mode == LOG_TO_SCNFILE )
    {
        if( !m_FP ) 
            m_FP = fopen(m_acFileName,"at+");    

        if( m_FP )
        {
            fprintf( m_FP, "TRACE ON\n" ) ;
            fflush(m_FP);
        }
    }

    UNLOCK();
}

void TLog::setMaxLines(int nMaxLines)
{
    m_nMaxLines = nMaxLines > 100 ? nMaxLines : 100;
}

void TLog::trace(int level, const char *format,...)
{
    if(CT_boolFALSE == CanTrace(level))
        return;

    LOCK();
    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    
    if (!PrintDateTimeSignature(m_FP))
    {
        va_list ap;
        va_start(ap,format);
        char buffer[1024] = {0};
#ifndef WIN32
        vsnprintf(buffer, 1023, format, ap);
#else
        _vsnprintf(buffer, 1023, format, ap);
#endif
        fprintf(m_FP, "%s", buffer);
        va_end(ap);
        if(m_nFlushFlag)
            fflush(m_FP);
        m_nTotalCount ++;
    }
    UNLOCK();
}

CTbool TLog::CanTrace(int level)            //用于控制是否打印日志
{
    if(level <= m_iTraceLevelThreshold)
        return CT_boolTRUE;
    else
        return CT_boolFALSE;
}

void TLog::SetTraceLevelThreshold(int level)    //用于动态调整日志级别
{
    m_iTraceLevelThreshold = level;
}

void TLog::print(const char *format,...)
{
    if(m_nTraceMode == LOG_TO_NULL)
        return;

    LOCK();
    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if (!PrintDateTimeSignature(m_FP))
    {
        va_list ap;
        va_start(ap,format);
        char buffer[1024] = {0};
#ifndef WIN32
        vsnprintf(buffer, 1023, format, ap);
#else
        _vsnprintf(buffer, 1023, format, ap);
#endif
        fprintf(m_FP, "%s", buffer);
        va_end(ap);
        if(m_nFlushFlag)
            fflush(m_FP);
         m_nTotalCount ++;
    }
    UNLOCK();
}

void TLog::print(int ID, const char *format,...)
{
    if(m_nTraceMode == LOG_TO_NULL)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    
    if (!PrintDateTimeSignature(m_FP))
    {
        va_list ap;
        va_start(ap,format);
        char buffer[1024] = {0};
#ifndef WIN32
        vsnprintf(buffer, 1023, format, ap);
#else
        _vsnprintf(buffer, 1023, format, ap);
#endif
        fprintf(m_FP, "%s", buffer);
        va_end(ap);
        if(m_nFlushFlag)
            fflush(m_FP);

        m_nTotalCount ++;
    }
    UNLOCK();
}

void TLog::vprint(int ID,const char *format,va_list argptr )
{
    if(m_nTraceMode == LOG_TO_NULL)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if (!PrintDateTimeSignature(m_FP))
    {
        fprintf(m_FP,"[%08X]", ID);
        vfprintf(m_FP, format, argptr );
        if(m_nFlushFlag)
            fflush(m_FP);
        m_nTotalCount ++;
    }
    UNLOCK();
}


void TLog::printnt(const char *format,...)
{
    if( m_nTraceMode == LOG_TO_NULL)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    va_list ap;
    va_start(ap,format);
    char buffer[1024] = {0};

#ifndef WIN32
    vsnprintf(buffer, 1023, format, ap);
#else
    _vsnprintf(buffer, 1023, format, ap);
#endif
    fprintf(m_FP, "%s", buffer);
    va_end(ap);

    if(m_nFlushFlag)
        fflush(m_FP);
    m_nTotalCount ++;
    
    UNLOCK();
}

void TLog::tracent(int level, const char *format,...)
{
    if(CT_boolFALSE == CanTrace(level))
        return;

    if( m_nTraceMode == LOG_TO_NULL)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    va_list ap;
    va_start(ap,format);
    char buffer[1024] = {0};

#ifndef WIN32
    vsnprintf(buffer, 1023, format, ap);
#else
    _vsnprintf(buffer, 1023, format, ap);
#endif
    fprintf(m_FP, "%s", buffer);
    va_end(ap);

    if(m_nFlushFlag)
        fflush(m_FP);
    m_nTotalCount ++;

    UNLOCK();
}

void TLog::printb(const char *title, const unsigned char *buf,int len)
{
    if(m_nTraceMode == LOG_TO_NULL || len <= 0)
        return;

    if(!title || !buf)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if (!PrintDateTimeSignature(m_FP))
    {
        UNLOCK();
        return;
    }

    char msg[512]="", tmp[10];
    int i;
    for( i=0; i<len && i<8096; i++ ) 
    {
        sprintf(tmp, "%02x ", buf[i]);
        strcat( msg, tmp);
        if( ((i+1) % 32 ) == 0  )
        {
            strcat( msg, "\n"  );
            fprintf( m_FP, "%s", msg);
            m_nTotalCount ++;
            strcpy(msg,"");
        }
    }
    if( i % 32    )
    {
        strcat( msg, "\n"  );
        fprintf( m_FP, "%s", msg);
    }

    if(len == 0)
        fprintf(m_FP, "\n");

    if(m_nFlushFlag)      
        fflush(m_FP);
    m_nTotalCount ++;
    UNLOCK();
}

void TLog::traceb(int level, const char *title, const unsigned char *buf,int len)
{
    if(CT_boolFALSE == CanTrace(level))
        return;

    if(m_nTraceMode == LOG_TO_NULL || len <= 0)
        return;

    if(!title || !buf)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if (!PrintDateTimeSignature(m_FP))
    {
        UNLOCK();
        return;
    }

    char msg[512]="", tmp[10];
    int i;
    for( i=0; i<len && i<8096; i++ ) 
    {
        sprintf(tmp, "%02x ", buf[i]);
        strcat( msg, tmp);
        if( ((i+1) % 32 ) == 0  )
        {
            strcat( msg, "\n"  );
            fprintf( m_FP, "%s", msg);
            m_nTotalCount ++;
            strcpy(msg,"");
        }
    }
    if( i % 32    )
    {
        strcat( msg, "\n"  );
        fprintf( m_FP, "%s", msg);
    }

    if(len == 0)
        fprintf(m_FP, "\n");

    if(m_nFlushFlag)      
        fflush(m_FP);
    m_nTotalCount ++;
    UNLOCK();
}

void TLog::printb(unsigned int ID, const char *title, const unsigned char *buf,int len)
{
    if(m_nTraceMode == LOG_TO_NULL || len <= 0)
        return;

    if(!title || !buf)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if (!PrintDateTimeSignature(m_FP))
    {
        UNLOCK();
        return;
    }

    char msg[512]="", tmp[10];
    int i;
    for( i=0; i<len && i<8096; i++ ) 
    {
        sprintf(tmp, "%02x ", buf[i]);
        strcat( msg, tmp);
        if( ((i+1) % 32 ) == 0  )
        {
            strcat( msg, "\n"  );
            m_nTotalCount ++;
            fprintf( m_FP, "%s", msg);
            strcpy(msg,"");
        }
    }

    if( i % 32    )
    {
        strcat( msg, "\n"  );
        fprintf( m_FP, "%s", msg);
    }

    if(len == 0)
        fprintf(m_FP, "\n");
    m_nTotalCount ++;

    if(m_nFlushFlag)        
        fflush(m_FP);
    UNLOCK();
}

void TLog::printnb(const unsigned char *buf,int len)
{
    if(m_nTraceMode == LOG_TO_NULL || len <= 0)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    char msg[512]="", tmp[10];
    int i = 0;
    for(i=0; i<len && i<8096; i++ ) 
    {
        sprintf(tmp, "%02x ", buf[i]);
        strcat( msg, tmp);
        if( ((i+1) % 32 ) == 0  )
        {
            strcat( msg, "\n"  );
            fprintf( m_FP, "%s", msg);
            m_nTotalCount ++;
            strcpy(msg,"");
        }
    }
    if( i % 32   )
    {
        strcat( msg, "\n"  );
        fprintf( m_FP, "%s", msg);
    }

    if(len == 0)
        fprintf(m_FP, "\n");

    if(m_nFlushFlag)       
        fflush(m_FP);

    m_nTotalCount++;
    
    UNLOCK();
}

void TLog::tracenb(int level, const unsigned char *buf,int len)
{
    if(CT_boolFALSE == CanTrace(level))
        return;

    if(m_nTraceMode == LOG_TO_NULL || len <= 0)
        return;

    LOCK();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    char msg[512]="", tmp[10];
    int i = 0;
    for(i=0; i<len && i<8096; i++ ) 
    {
        sprintf(tmp, "%02x ", buf[i]);
        strcat( msg, tmp);
        if( ((i+1) % 32 ) == 0  )
        {
            strcat( msg, "\n"  );
            fprintf( m_FP, "%s", msg);
            m_nTotalCount ++;
            strcpy(msg,"");
        }
    }
    if( i % 32   )
    {
        strcat( msg, "\n"  );
        fprintf( m_FP, "%s", msg);
    }

    if(len == 0)
        fprintf(m_FP, "\n");

    if(m_nFlushFlag)       
        fflush(m_FP);

    m_nTotalCount++;

    UNLOCK();
}

void TLog::perror(const char *format,...)
{
    LOCK();

    FILE *fp = fopen(m_acErrorFileName, "at+");
    if( fp ) 
    {          
        PrintDateTimeSignature(fp);
        va_list ap;
        va_start(ap, format);
        char buffer[1024] = {0};
#ifndef WIN32
        vsnprintf(buffer, 1023, format, ap);
#else
        _vsnprintf(buffer, 1023, format, ap);
#endif
        fprintf(fp, "%s", buffer);
        va_end(ap);

        fclose(fp);
    }

    if(!m_FP)
    {
        UNLOCK();
        return;
    }

    if(m_nTotalCount > m_nMaxLines)
        Backup();

    if(!m_FP)
    {
        UNLOCK();
        return;
    }
    
    va_list ap;
    va_start(ap,format);
    PrintDateTimeSignature(m_FP);
    char buffer[1024] = {0};
#ifndef WIN32
    vsnprintf(buffer, 1023, format, ap);
#else
    _vsnprintf(buffer, 1023, format, ap);
#endif
    fprintf(m_FP, "%s", buffer);
    va_end(ap);
    
    m_nTotalCount ++;

    if(m_nFlushFlag)
        fflush(m_FP);

    UNLOCK();
}

void TLog::flush()
{
    LOCK();

    if( m_FP )
        fflush( m_FP );

    UNLOCK();
}

/////////////////////////////////////////privates//////////////////////////////

void TLog::Backup()
{
    if(m_FP)
    {
        fclose(m_FP);
        char bak_name[MAX_PATH];
        
        if(m_iBakMode==0)
        {
            strcpy(bak_name,m_acFileName);
            strcat(bak_name,"_bak");
        }
        else if(m_iBakMode==1)
        {
            char fp_bak[MAX_PATH];
            time_t now = time(0);    
#ifdef WIN32
            struct tm *ts = localtime(&now);
#else
            struct tm t1;
            struct tm *ts = localtime_r(&now,&t1);
#endif
    
            if(access("./log", 0) == 0)
                strcpy(fp_bak, "./log/");
            else
                strcpy(fp_bak, "./");

            if (ts)
                   sprintf( bak_name, "%s%04d%02d%02d%02d%02d%02d_%s.log", 
                    fp_bak, ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, 
                    ts->tm_hour, ts->tm_min, ts->tm_sec, m_acModuleName );
            else
                sprintf( bak_name, "%s%s.log", fp_bak, m_acModuleName ) ;

        }
        else
        {
            sprintf(bak_name,"%s_%04d",m_acFileName,m_iIdx);
            m_iIdx++;
            if(m_iIdx>=m_iBakMode) m_iIdx = 0;

        }
        m_nTotalCount = 0;
        unlink(bak_name);
        rename(m_acFileName,bak_name);
    }
    m_FP = fopen(m_acFileName,"wt");
    PrintVersion();
}

void TLog::PrintVersion(void)
{
    if( m_FP )
    {
        fprintf(m_FP, "%s %s\n", m_acModuleName, m_acVersion);
        fflush(m_FP);
    }
}

void TLog::SetVersion(const char *szVersion)
{
    strcpy(m_acVersion, szVersion);
    return;
}
