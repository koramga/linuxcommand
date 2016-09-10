#include "fork_result-core.h"

int __findPIDIndex__(pid_t findPID)
{
    for(int i = 0; i < __MAX_FORK_RESULT_SIZE__; ++i)
    {
        if(__forkResult__[i].pid == findPID)
            return i;
    }

    setError(ERR_INDEX, "fork_result-core : __findPIDIndex__  error\n");

    return -1;      //error -1
}

int __findEmptyIndex__()
{
    int emptyIndex =  __findPIDIndex__(0);     //pid가 0이면 empty를 의미합니다.

    if(emptyIndex == -1)
        setError(ERR_OVERFLOW, "fork_result-core : __findEmptyIndex__ error\n");

    return emptyIndex;
}

void __initForkResultValue__(int index)
{
    memset(&__forkResult__[index], 0, sizeof(FORK_RESULT));
}

bool __delForkResultIndex__(int index)
{
    safe_free_dptr(&__forkResult__[index].command);
}

bool __setTimer__(struct timespec* pTimer)
{
    int ret;

    ret = clock_gettime(CLOCK_MONOTONIC, pTimer);

    if(ret == -1)
    {
        setError(ERR_CREATE, "fork_result-core : __setTimer__ create error"); 
        return false;
    }

    return true;
}

bool initForkResult()
{
    for(int i = 0; i < __MAX_FORK_RESULT_SIZE__; ++i)
        __initForkResultValue__(i);

    atexit(__releaseForkResult__);
}

void __releaseForkResult__()
{
    for(int i = 0; i < __MAX_FORK_RESULT_SIZE__; ++i)
    {
        if(__forkResult__[i].command != NULL)
            __delForkResultIndex__(i);
    }
}

const FORK_RESULT* const getForkResult(pid_t pid)
{
    int findIndex = __findPIDIndex__(pid);

    if(findIndex == -1)
        return NULL;

    return &__forkResult__[findIndex];
}

bool setForkResultPID(pid_t pid)
{
    int findIndex = __findEmptyIndex__();

    if(findIndex == -1)
        return false;

    if(!__setTimer__(&__forkResult__[findIndex].startTimer))
        return false;

    __forkResult__[findIndex].pid = pid;

    return true;
}

bool addForkResultCommand(pid_t pid, const char* command)
{
    int findIndex = 0;

    findIndex = __findPIDIndex__(pid);

    if(findIndex == -1)
        return false;

    __forkResult__[findIndex].command = (char*)calloc(sizeof(char), 1024);

    if(__forkResult__[findIndex].command == NULL)
    {
        setError(ERR_CREATE, "fork_result : addForkResultCommand, malloc error");
        return false;
    }

    strcpy(__forkResult__[findIndex].command, command);

    return true;
}

bool addForkResultStatus(pid_t pid, int status)
{
    int findIndex = __findPIDIndex__(pid);

    if(findIndex == -1)
        return false;

    if(!__setTimer__(&__forkResult__[findIndex].endTimer))
        return false;

    __forkResult__[findIndex].status = status;

    return true;
}

bool pushForkResult(pid_t pid)
{
    queue_push(pid);
}

bool delForkResult(pid_t pid)
{
    int findIndex = __findPIDIndex__(pid);

    if(findIndex == -1)
        return false;

    __delForkResultIndex__(findIndex);
}

/*
int main()
{
    initForkResult();
    
    setForkResultPID(1);
    addForkResultCommand(1, "command");
    addForkResultStatus(1, 1);

    setForkResultPID(2);
    addForkResultCommand(2, "forkfork");
    addForkResultStatus(2, 3);

    const FORK_RESULT* const forkResult = getForkResult(2);

//    if(forkResult == NULL)
//        printf("error");

    printf("pid_t : %d\ncommand : %s\nstatus : %d\ndiffTimerSec : %d\ndiffTimernSec : %d\n", 
            forkResult->pid, forkResult->command, forkResult->status, forkResult->endTimer.tv_sec - forkResult->startTimer.tv_sec, forkResult->endTimer.tv_nsec - forkResult->startTimer.tv_nsec);

//    releaseForkResult();

    return 0;
}
*/
