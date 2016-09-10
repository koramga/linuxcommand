#ifndef __FORK_RESULT_CORE_H__
#define __FORK_RESULT_CORE_H__

/*
 * 
 *
 *
*/

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <string.h>
#include "queue-core.h"
#include "util-core.h"
#include "error-core.h"

#define __MAX_FORK_RESULT_SIZE__ 10

typedef struct tagForkResult
{
    pid_t   pid;
    char*   command;
    int     status;
    struct timespec startTimer;
    struct timespec endTimer;
}FORK_RESULT;

//private
FORK_RESULT __forkResult__[__MAX_FORK_RESULT_SIZE__];
int __findPIDIndex__(pid_t findPID);
int __findEmptyIndex__();
void __initForkResultValue__(int index);
bool __delForkResultIndex__(int index);
bool __setTimer__(struct timespec* pTimer);

//have-to use function
bool initForkResult();                  
void __releaseForkResult__();

//use function
const FORK_RESULT* const getForkResult(pid_t pid);
bool setForkResultPID(pid_t pid);                              
bool addForkResultCommand(pid_t pid, const char* command);
bool addForkResultStatus(pid_t pid, int status);
bool pushForkResult(pid_t pid);
bool delForkResult(pid_t pid);

#endif 
