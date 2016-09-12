#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <sys/times.h>
#include <wait.h>
#include "queue-core.h"
#include "fork_result-core.h"
#include "user_interface-core.h"
#include "command-core.h"

bool initialize();
bool progress();
bool release();

bool g_exit = false;

//init 함수 모음
bool initSignal();

//progress 함수 모음
bool syncFork(USER_INPUT* command);
bool asyncFork(const char* command);
void childSignal(int sig, siginfo_t* siginfo, void* context);
void forkResultQueueCheck();
void userInterfaceQueueCheck();

int main()
{
    if(initialize())
    {
        while(!g_exit)
            progress();

        release();
    }

    else 
        printf("initialize error\n");

    return 0;
}

bool initialize()
{
    bool ret;

    ret = initInterface();

    if(!ret)
    {
        renderErrorMessage();
        return false;
    }

    ret = initSignal();

    if(!ret)
    {
        renderErrorMessage();
        return false;
    }
    
    ret = initForkResult();

    if(!ret)
    {
        renderErrorMessage();
        return false;
    }

    ret = initExcuteFile();

    if(!ret)
    {
        renderErrorMessage();
        return false;
    }

    return true;
}

bool progress()
{
    const char* readData;
    int i = 0;
    bool returnValue = true;

    readUser();

//    printf("progress\n");

    userInterfaceQueueCheck();
    forkResultQueueCheck();

    return returnValue;
}

bool release()
{
   
}

bool syncFork(USER_INPUT* pUserInput)
{
    pid_t pid;
    int status;
    int ret;
    int result = 0;

    pid = fork();

    if(pid > 0) //parent
    {
        setForkResultPID(pid);
        addForkResultCommand(pid, pUserInput->inputBuffer);  
    }

    else if(pid == 0) //manager child
    {
        for(int i = 0; i < getExcuteFileCount(); ++i)
        {
            pid = fork();

            if(pid > 0)
            {
                waitpid(pid, &status, 0);

                if(WIFEXITED(status))
                {
                    if(WEXITSTATUS(status) == 1)
                        result = 1;
                }
            }

            else if(pid == 0)
            {
                ret = execv(getExcuteFile(i), pUserInput->userInput);

                if(ret == -1)
                    perror("exec error\n");

            }

            else if(pid == -1)
            {
                exit(-1);
            }
        }
        exit(result);
    }

    else if(pid == -1)
        return false;

    return true;
}
/*
bool asyncFork(const char* command)
{
    pid_t pid;
    pid_t* childPID;
    int childPIDIndex = 0;
    int status;
    int ret;
    int result = 0;

    pid = fork();

    if(pid > 0) //parent
    {
        setForkResultPID(pid);
        addForkResultCommand(pid, command);  
    }

    else if(pid == 0) //manager child
    {
        childPID = alloca(sizeof(pid_t) * );
//        childPID = (pid_t*)calloc(sizeof(pid_t), g_excuteFileIndex);

        for(int i = 0; i < g_excuteFileIndex; ++i)
        {
            pid = fork();

            if(pid > 0)
            {
 //               printf("PID : %d, childPIDIndex : %d\n", pid, childPIDIndex);
                childPID[childPIDIndex++] = pid;
            }

            else if(pid == 0)
            {
//                printf("%s %s\n", g_excuteFile[i], command);
                ret = execl(g_excuteFile[i], command, NULL);

                if(ret == -1)
                    perror("exec error\n");

            }

            else if(pid == -1)
            {
                exit(-1);
            }
        }

//        printf("child index : %d\n", childPIDIndex);

        for(int i = 0; i < childPIDIndex; ++i)
        {
//          printf("%d %d\n", i, childPID[i]);
            while(true)
            {
                waitpid(childPID[i], &status, 0);

                if(WIFEXITED(status))
                {
//                  printf("PID : %d, status : %d\n", childPID[i], WEXITSTATUS(status));
                    break;
                }
            }

            if(WEXITSTATUS(status) == 1)
                result = 1;
        }

//        printf("result : %d\n", result);
//        free(childPID);
        exit(result);
    }

    else if(pid == -1)
        return false;

    return true;
}
*/
void childSignal(int sig, siginfo_t* siginfo, void* context)
{
    if(siginfo->si_code == CLD_EXITED)
    {
        if(addForkResultStatus(siginfo->si_pid, siginfo->si_status))
        {
            //printf("add fork result\n");
            pushForkResult(siginfo->si_pid);
        }

    }
}

bool initSignal()
{
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));

    act.sa_sigaction = &childSignal;

    act.sa_flags = SA_SIGINFO;

    if(sigaction(SIGCHLD, &act, NULL) < 0)
    {
        setError(ERR_CREATE, "main-core.c : initSignal Error");
        return false;
    }

    return true;
}

void forkResultQueueCheck()
{
    while (!queue_empty(QUEID_FORK_RESULT))
    {
        int* heap_pid = queue_get(QUEID_FORK_RESULT);
        queue_pop(QUEID_FORK_RESULT);

        int pid = (*heap_pid);
        
        safe_free_dptr(&heap_pid);

        const FORK_RESULT* const forkResult = getForkResult(pid);

        printf("=================================\n");
        //    printf("queue result index : %d\n", index);

        if(forkResult->status == 0)
        {
            printf("%s fail command\n", forkResult->command);
        }

        else printf("%s success command\n", forkResult->command);

        printf("excute time sec : %ld, nsec : %ld\n", forkResult->endTimer.tv_sec - forkResult->startTimer.tv_sec, forkResult->endTimer.tv_nsec - forkResult->startTimer.tv_nsec);

        delForkResult(pid);

        printf("================================\n");
    }
}

void userInterfaceQueueCheck()
{
    while(!queue_empty(QUEID_USER_INTERFACE))
    {
        USER_INPUT* pUserInput = (USER_INPUT*)queue_get(QUEID_USER_INTERFACE);
        queue_pop(QUEID_USER_INTERFACE);

       // printf("input\n");

        if(strcmp(pUserInput->userInput[0], "quit") == 0)
        {
            g_exit = true;
        }

        else
        {
            syncFork(pUserInput);
        }

        delUserInput(pUserInput);
    }
}
