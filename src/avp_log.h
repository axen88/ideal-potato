#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif /* End of __cplusplus */

#define LOG_TO_NULL        0x00
#define LOG_TO_FILE        0x01
#define LOG_TO_SCREEN      0x02
#define LOG_TO_SCNFILE     (LOG_TO_FILE | LOG_TO_SCREEN)

#ifdef __KERNEL__

#define GetCurrentThreadId() ((long)100)

#define LogOpen(fileName, version, dir, traceLevel, traceMode) NULL
#define LogClose(v_pLog)
#define LogSetTraceLevel(v_pLog, v_level)
#define LogTrace(log, x, ...) printk(__VA_ARGS__)

#else

    #ifdef WIN32
    
#include <windows.h>
    
    #else

#include <pthread.h>
#define GetCurrentThreadId() pthread_self()

    #endif
    
extern void *LogOpen(const char *fileName, const char *version,
    const char *dir, int traceLevel, int traceMode);
extern void LogClose(void *v_pLog);
extern void LogSetTraceLevel(void *v_pLog, int v_level);
extern void LogTrace(void *v_pLog, int level, const char *format, ...);

#endif

    #if (defined(WIN32) && (_MSC_VER <= 1200))  // Windows VC6.0

class MyTracer
{
    public:
        MyTracer(int level, const char *prefix, const char *file, int line)
            :_level(level), _prefix(prefix), _file(file), _line(line)
        {
        
        }
        
        void operator()(void *log, const char *fmt, ...)
        {
            va_list ap;
            char buffer[1024];
            
            va_start(ap, fmt);
            memset(buffer, 0, sizeof(buffer));
            _vsnprintf(buffer, 1023, fmt, ap);
            assert(log);
            LogTrace(log, _level, "[%s][%lu][%s:%d]: %s", _prefix,
                GetCurrentThreadId(), _file, _line, buffer);
            va_end(ap);
        }
    private:
        // copy-ctor and operator=
        MyTracer(const MyTracer&);
        MyTracer& operator=(const MyTracer&);
        
        const char *_file;
        const char *_prefix;
        int _level;
        int _line;
};
    
#define LogTraceDebug    (MyTracer(4, "DEBUG",   __FILE__, __LINE__))
#define LogTraceInfo     (MyTracer(3, "EVENT",   __FILE__, __LINE__))
#define LogTraceWarning  (MyTracer(2, "WARNING", __FILE__, __LINE__))
#define LogTraceError    (MyTracer(1, "ERROR",   __FILE__, __LINE__))
#define LogTraceEmerg    (MyTracer(0, "EMERG",   __FILE__, __LINE__))
    
    #else
    
#define LogTraceDebug(log, fmt, ...) \
{ \
    LogTrace(log, 4, "[DEBUG][%lu][%s:%s:%d]: "fmt, GetCurrentThreadId(), \
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceInfo(log, fmt, ...) \
{ \
    LogTrace(log, 3, "[EVENT][%lu][%s:%s:%d]: "fmt, GetCurrentThreadId(), \
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceWarning(log, fmt, ...) \
{ \
    LogTrace(log, 2, "[WARNING][%lu][%s:%s:%d]: "fmt, GetCurrentThreadId(), \
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceError(log, fmt, ...) \
{ \
    LogTrace(log, 1, "[ERROR][%lu][%s:%s:%d]: "fmt, GetCurrentThreadId(), \
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceEmerg(log, fmt, ...) \
{ \
    LogTrace(log, 0, "[EMERG][%lu][%s:%s:%d]: "fmt, GetCurrentThreadId(), \
        __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
}

    #endif
    
#ifdef __cplusplus
}
#endif /* End of __cplusplus */

#endif
