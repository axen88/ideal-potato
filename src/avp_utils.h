#ifndef __UTILS_H__
#define __UTILS_H__

extern unsigned long long GetCycleCount(void);
extern unsigned long long GetMsCount(void);
extern unsigned long long GetSecCount(void);
extern int StrToLong(const char *v_pStr, long *v_pVal, int v_base);
extern int StrToLongLong(const char *v_pStr, long long *v_pVal, int v_base);


#endif /* End of __UTILS_H__ */
