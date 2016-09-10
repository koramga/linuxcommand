#ifndef __USER_INTERFACE_CORE_H__
#define __USER_INTERFACE_CORE_H__

#define __TEXT_MAX_COL__ 64
#define __TEXT_MAX_ROW__ 256

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct tagUserInput
{
    char*   userInput[__TEXT_MAX_COL__];
    char*   inputBuffer;
    int     col;
}USER_INPUT;

USER_INPUT* __createUserInput__();
void __makeUserInput__(const char* readBuffer, const int len);

bool initInterface();
void __releaseInterface__();
bool readUser();
void delUserInput(USER_INPUT* pUserInput);



#endif
