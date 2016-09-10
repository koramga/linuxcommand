#ifndef __COMMAND_CORE_H__
#define __COMMAND_CORE_H__

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define __PROCESS_NAME_SIZE__   1024
#define __PROCESS_COUNT__       1024         

bool __addExcuteFile__(const char* name);
int __isExcuteFile__(const char* name);
int __isDirectory__(const char* name);
void findExcuteFile(const char* path);
void initExcuteFile();
void __releaseExcuteFile__();
int getExcuteFileCount();
const char* getExcuteFile(int index);

#endif
