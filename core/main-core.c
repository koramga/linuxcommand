#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/times.h>
#include "queue-core.h"
#include "fork_result-core.h"

#define PROCESS_NAME_SIZE 1024

bool initialize();
bool progress();
void render();
bool release();

const char* g_excuteFile[PROCESS_NAME_SIZE];
int g_excuteFileIndex = 0;
int g_fd;

//init 함수 모음
const char* makeForkProcessName(const char* name);
void findExcuteFile();
bool isExcuteFile(const char* name);
bool openStdin();
bool initSignal();

//progress 함수 모음
const char* readStdin();
bool syncFork(const char* command);
bool asyncFork(const char* command);
void childSignal(int sig, siginfo_t* siginfo, void* context);
void queueCheck();

int main()
{
    if(initialize())
    {
        while(progress());

        release();
    }

    else perror("init error");

    return 0;
}

bool initialize()
{
    if(openStdin())
    {
        if(initSignal())
        {
            initForkResult();
            findExcuteFile();
            return true;
        }
    }

    return false;
}

bool progress()
{
    const char* readData;
    int i = 0;
    bool returnValue = true;

    readData = readStdin();

    if(readData != NULL)
    {
        if(strcmp(readData, "quit") == 0)
        {
            returnValue = false;
        }

        else
        {
            syncFork(readData);
            /*
            if(strcmp(readData, "help") == 0)
                syncFork(readData);

            else
                asyncFork(readData);
            */
        }

        free((void*)readData);
    }

    queueCheck();

    return returnValue;
}

void render()
{
    int i = 0;

    for(i = 0; i < g_excuteFileIndex; ++i)
        printf("%s\n", g_excuteFile[i]);
}

bool release()
{
    int i = 0;

    for(i = 0; i < g_excuteFileIndex; ++i)
        free((void*)g_excuteFile[i]);

    close(g_fd);
}

bool syncFork(const char* command)
{
    pid_t pid;
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
        close(g_fd);
        for(int i = 0; i < g_excuteFileIndex; ++i)
        {
            pid = fork();

            if(pid > 0)
            {
                waitpid(pid, &status, 0);

                if(WIFEXITED(status))
                {
                    if(WEXITSTATUS(status) == 1)
                        result = 1;
//                    printf("WIFEXITED : %d\n", WEXITSTATUS(status));
                }
            }

            else if(pid == 0)
            {
                ret = execl(g_excuteFile[i], command, NULL);

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
        childPID = alloca(sizeof(pid_t) * g_excuteFileIndex);
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
//            printf("%d %d\n", i, childPID[i]);
            while(true)
            {
                waitpid(childPID[i], &status, 0);

                if(WIFEXITED(status))
                {
//                    printf("PID : %d, status : %d\n", childPID[i], WEXITSTATUS(status));
                    break;
                }
            }
/*
            if(WIFCONTINUED(status))
            {
                printf("==Continued==\n");
            }

            if(WIFSTOPPED(status))
            {
                printf("==Stop==\n");
            }

            if(WIFSIGNALED(status))
            {
                printf("==Signal==\n");
            }

            if(WIFEXITED(status))
            {
                printf("==Exit==\n");
            }
*/
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

bool openStdin()
{
    g_fd = open("/dev/stdin", O_NONBLOCK | O_RDONLY);

    if(g_fd == -1)
    {
        perror("fd error");
        return false;
    }

    return true;
}

const char* readStdin()
{
    char buf[1024];
    int len;
    int err;
    char* returnBuf;
    int returnBufLen = 0;
    int i = 0;

    len = read(g_fd, buf, 1024);

    if(len == -1)
    {
        err = errno;

        switch(err)
        {
            case EINTR :
                break;

            case EAGAIN :
                break;
        }
    }

    else 
    {
        returnBuf = (char*)calloc(sizeof(char), 1024);

        for(i = 0; i < strlen(buf); ++i)
        {
            if(buf[i] == 10)
                break;

            returnBuf[i] = buf[i];
        }

//        strncpy(returnBuf, buf, len);
        
        return returnBuf;
    }

    return NULL;
}

void findExcuteFile()
{
    struct dirent *entry;
    int ret;
    DIR* dir;

    dir = opendir("./");

    errno = 0;

    while((entry = readdir(dir)) != NULL)
    {
        if(isExcuteFile(entry->d_name))
        {
            if(strcmp(entry->d_name,"init") == 0)
                continue;

            printf("%s\n", entry->d_name);

            g_excuteFile[g_excuteFileIndex++] = makeForkProcessName(entry->d_name);
        }
    } 
//    printf("%d\n", g_excuteFileIndex);
    closedir(dir);
}

bool isExcuteFile(const char* fileName)
{
    struct stat sb;
    int ret;

    ret = stat(fileName, &sb);

    if(ret == -1)
    {
        const int err = errno;
        char* buf[1024];

        memset(buf, 0, 1024);

        ret = strerror_r(err, buf, 1024);

        if(ret == -1)
            perror("84 code error\n");

        else
            perror(buf);
    }

    else
    {
        if((sb.st_mode & S_IFMT) == S_IFREG)
        {
            if((sb.st_mode & S_IXUSR) && (sb.st_mode & S_IXGRP) && (sb.st_mode & S_IXOTH))
            {
                return true;
            }
        }
    }

    return false;
}

const char* makeForkProcessName(const char* name)
{
    char* processName;

    processName = (char*)calloc(sizeof(char), PROCESS_NAME_SIZE);
    //name을 생성하는데 있어서 모두 0으로 채워넣어야 하므로
    //malloc보다 더 좋다.


    strcpy(processName, "./");
    strcat(processName, name);

    return processName;
}

void childSignal(int sig, siginfo_t* siginfo, void* context)
{
    if(siginfo->si_code == CLD_EXITED)
    {
        if(addForkResultStatus(siginfo->si_pid, siginfo->si_status))
        {
            printf("add fork result\n");
            pushForkResult(siginfo->si_pid);
//            queue_push(__findPIDIndex__(siginfo->si_pid));
            /*
               int index = __findPIDIndex__(siginfo->si_pid);
               printf("%s\n", g_forkResult[index].command);
               printf("%d, %d, %d\n", siginfo->si_pid, siginfo->si_status, siginfo->si_value); 
            */
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
        perror("sigaction");
        return false;
    }

    return true;
}

void queueCheck()
{
    while (!queue_empty())
    {
        int pid = queue_get();
        queue_pop();

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
