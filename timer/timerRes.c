#include <time.h>
#include <stdio.h>
#include <string.h>

clockid_t clocks[] = {
    CLOCK_REALTIME,
    CLOCK_MONOTONIC,
    CLOCK_PROCESS_CPUTIME_ID,
    CLOCK_THREAD_CPUTIME_ID,
    CLOCK_MONOTONIC_RAW,
    (clockid_t) - 1
};

int main(int argc, char* argv[])
{
    int result = 0;

    if(argc >= 1)
    {
        if(strcmp(argv[0], "help") == 0)
        {
            printf("timeres : my linux timer res view\n");
            result = 1;
        }

        else if(strcmp(argv[0], "timeres") == 0)
        {
            for(int i = 0; clocks[i] != (clockid_t) - 1; ++i)
            {
                struct timespec res;
                int ret;

                ret = clock_getres(clocks[i], &res);

                if(ret)
                {
                    result = 0;
                    break;
                }

                else
                {
                    printf("clock=%d sec=%ld nsec=%ld\n", clocks[i], res.tv_sec, res.tv_nsec);
                }
            }
            result = 1;
        }
    }

    return result;
}
