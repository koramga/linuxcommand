#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

char* command = "ctime";
char* commandReview = "current time";

bool progress(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int result = 0;

    if(argc >= 1)
    {
        if(strcmp(argv[0], "help") == 0)
        {
            printf("%s : %s\n", command, commandReview);
            result = 1;
            //help message
        }

        else if(strcmp(argv[0], command) == 0)
        {//chage "command" -> ur command
            if(progress(argc, argv))
                result = 1;

            else result = 0;
        }
    }

    return result;
}

bool progress(int argc, char* argv[])
{
    struct tm* tm;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    printf("year : %d\n", tm->tm_year + 1900);
    printf("month : %d\n", tm->tm_mon + 1);
    printf("day : %d\n", tm->tm_mday);
    printf("hour : %d\n", tm->tm_hour);
    printf("minute : %d\n", tm->tm_min);
    printf("second : %d\n", tm->tm_sec);

    return true;
}
