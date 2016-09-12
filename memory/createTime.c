#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void getTimeSpec(struct timespec* ts)
{
    int ret;

    while(1)
    {
        ret = clock_gettime(CLOCK_MONOTONIC, ts);

        if(ret != -1)
            break;
    }
}

bool help()
{
    printf("[sch] [%%d] : stack compare heap count\n");

    return true;
}

bool compareMemory(int count)
{
    int* pNumber;
    struct timespec ts_stack_start, ts_stack_end;
    struct timespec ts_heap_start, ts_heap_end;


    getTimeSpec(&ts_stack_start);
    
    printf("stack create\n");

    for(int i = 0; i < count; ++i)
    {
        pNumber = (int*)alloca(sizeof(int) * 1024);
    }

    getTimeSpec(&ts_stack_end);

    printf("stack end\n");

    getTimeSpec(&ts_heap_start);

    printf("heap create\n");

    for(int i = 0; i < count; ++i)
    {
        pNumber = (int*)malloc(sizeof(int) * 1024);

        for(int j = 0; j < count; ++j)
            pNumber[j] = j;

        free((void*)pNumber);
    }

    getTimeSpec(&ts_heap_end);

    printf("heap end\n");

    printf("=========================================================\n");

    printf("heap time sec : %d, nsec : %ld\n"
            ,ts_heap_end.tv_sec - ts_heap_start.tv_sec
            ,ts_heap_end.tv_nsec - ts_heap_start.tv_nsec);

    printf("stack time sec : %d, nsec : %ld\n"
            ,ts_stack_end.tv_sec - ts_stack_start.tv_sec
            ,ts_stack_end.tv_nsec - ts_stack_start.tv_nsec);
    
    printf("diff stack and heap time sec : %d, nsec : %ld\n"
            ,(ts_heap_end.tv_sec - ts_heap_start.tv_sec) - (ts_stack_end.tv_sec - ts_stack_start.tv_sec)
            , (ts_heap_end.tv_nsec - ts_heap_start.tv_nsec) - (ts_stack_end.tv_nsec - ts_stack_start.tv_nsec));

    printf("=========================================================\n");

    return true;
}

bool progress(int argc, char* argv[])
{
    if(strcmp(argv[0], "sch") == 0)
    {
        if(argc <= 1)
        {
            printf("we need count\n");
            return false;
        }

        compareMemory(atoi(argv[1]));       
    }
}

int main(int argc, char* argv[])
{
    
    bool ret = false;

    if(argc >= 1)
    {
        if(strcmp(argv[0], "help") == 0)
        {
            ret = help();
        }

        else
        {
           ret = progress(argc, argv);
        }
    }

    if(ret)
        return 1;

    return 0;
    
//    compareMemory(1000);
}
