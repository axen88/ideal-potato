/*
* 使用gcc -Wall -g -o dir_scan dir_scan.c进行编译链接
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int DirScan(const char *v_pPath, int (*v_pFileProcess)(const char *))
{
    struct stat s;
    DIR *pDir;
    struct dirent *pDt;
    int ret = 0;
    char fullName[1024];

    if (NULL == v_pPath)
    {
        printf("Invalid parameter.\n");
        return -1;
    }

    ret = lstat(v_pPath, &s);
    if (ret < 0)
    {
        printf("lstat failed. [name: %s]\n", v_pPath);
        return -2;
    }

    ret = v_pFileProcess(v_pPath);
    if (ret < 0)
    {
        return ret;
    }

    if (!S_ISDIR(s.st_mode))
    {
        return 0;
    }

    pDir = opendir(v_pPath);
    if (NULL == pDir)
    {
        printf("opendir failed. [name: %s]\n", v_pPath);
        return -3;
    }

    while ((pDt = readdir(pDir)) != NULL)
    {
        if (('.' == pDt->d_name[0])
            && ((strcasecmp(pDt->d_name, ".") == 0)
                || (strcasecmp(pDt->d_name, "..") == 0)))
        {
            continue;
        }

        snprintf(fullName, 1024, "%s/%s", v_pPath, pDt->d_name);

        ret = DirScan(fullName, v_pFileProcess);
        if (ret < 0)
        {
            break;
        }
    }

    closedir(pDir);

    return ret;
}

int FileProcess(const char *v_pName)
{
    printf("%s\n", v_pName);
    
    return 0;
}

int main(int argc, char **argv)
{
    int ret;
    
    if (argc < 2)
    {
        printf("Usage: %s dir_name\n", argv[0]);
        return -1;
    }
    
    ret = DirScan(argv[1], FileProcess);
    if (ret < 0)
    {
        printf("DirScan failed. [ret: %d]\n", ret);
    }
    
    return 0;
}


