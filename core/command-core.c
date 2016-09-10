#include "command-core.h"
#include "error-core.h"
#include "util-core.h"

char*       __excuteFiles__[__PROCESS_COUNT__];
int         __excuteFileCount__ = 0;

int getExcuteFileCount()
{
    return __excuteFileCount__;
}

const char* getExcuteFile(int index)
{
    if(index >= __excuteFileCount__)
        return NULL;

    return __excuteFiles__[index];
}

bool __addExcuteFile__(const char* name)
{
    __excuteFiles__[__excuteFileCount__] = (char*)calloc(sizeof(char), __PROCESS_NAME_SIZE__);

    strcpy(__excuteFiles__[__excuteFileCount__], name);

    __excuteFileCount__++;
}

int __isExcuteFile__(const char* name)
{
    struct stat sb;
    int ret;

    ret = stat(name, &sb);

    if(ret == -1)
    {
        setError(ERR_CREATE, "command-core : __isExcuteFile__ create error");
        return -1;
    }

    else
    {
        if((sb.st_mode & S_IFMT) == S_IFREG)
        {
            if((sb.st_mode & S_IXUSR) && (sb.st_mode & S_IXGRP) && (sb.st_mode & S_IXOTH))
                return true;
        }
    }

    return false;
}

int __isDirectory__(const char* name)
{
    struct stat sb;
    int ret;

    ret = stat(name, &sb);

    if(ret == -1)
    {
        setError(ERR_CREATE, "command-core : __isDirectory__ create error");
        return -1;
    }

    if((sb.st_mode & S_IFMT) == S_IFDIR)
        return true;

    return false;
}

void findExcuteFile(const char* path)
{
    int ret;
    struct dirent* entry;
    DIR* dir;
    char* directoryPath = alloca(1024);
    char* realPath = alloca(1024);
    strcpy(directoryPath, path);
    
    int len = strlen(directoryPath);
    directoryPath[len++] = '/';
    directoryPath[len] = NULL;

//    printf("%s\n", directoryPath);

    dir = opendir(directoryPath);

    while((entry = readdir(dir)) != NULL)
    {
        
        if(strcmp(entry->d_name, "..") == 0)
            continue;

        else if(strcmp(entry->d_name, ".") == 0)
            continue;

        else if(strcmp(entry->d_name, "core") == 0)
            continue;

        else if(entry->d_name[0] == '.')
            continue;
        
        
//       printf("%s\n", entry->d_name);

        memset(realPath, 0, 1024);
        strcpy(realPath, directoryPath);
        strcat(realPath, entry->d_name);

        if(__isDirectory__(realPath))
            findExcuteFile(realPath);

        else if(__isExcuteFile__(realPath))
            __addExcuteFile__(realPath);
       
    }

    closedir(dir);
}

void __releaseExcuteFile__()
{
    for(int i = 0; i < __excuteFileCount__; ++i)
        safe_free_dptr(&__excuteFiles__[i]);
}

void initExcuteFile()
{
    findExcuteFile("..");
    atexit(__releaseExcuteFile__);
}

/*
int main()
{
    initExcuteFile();

    for(int i = 0; i < __excuteFileCount__; ++i)
        printf("%d. %s\n", i, __excuteFiles__[i]);
}
i*/
