#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
timer_t timerID;
int tick_count = 0;
const char* command = "new_timer";
const char* commandReview = "[new_timer] [%%d] : %%d is wait time";

bool progress(int argc, char* argv[]);

void timer_handler(int sig, siginfo_t* si, void* uc)
{
    timer_t* tidp;
    tidp = si->si_value.sival_ptr;

//    printf("timer_handler\n");

    if((*tidp) == timerID)
    {
        tick_count++;
        printf("=============ALRM=============\n");
        printf("%d\n", tick_count);
    }
}

bool createSignal()
{
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    
    if(sigaction(SIGRTMIN, &sa, NULL) == -1)
    {
        perror("sigaction error\n");
        return false;
    }

    return true;
}

bool createTime()
{
   int ret;
   struct sigevent evp;

   evp.sigev_value.sival_ptr = &timerID;
   evp.sigev_notify = SIGEV_SIGNAL;
   evp.sigev_signo = SIGRTMIN;

   ret = timer_create(CLOCK_REALTIME, &evp, &timerID);

   if(ret == -1)
   {
       perror("timer_create error\n");
       return false;
   }

   return true;
}

bool setTime(int sec)
{
    struct itimerspec ts;
    int ret;

    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = sec;
    ts.it_value.tv_nsec = 0;

    ret = timer_settime(timerID, 0, &ts, NULL);

    if(ret == -1)
    {
        perror("timer_settime");
        return false;
    }

    return true;
}

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
    int sec;
    int ret;
    int over;
    struct itimerspec ts;

    if(argc >= 2)
    {
        sec = atoi(argv[1]);
        createSignal();
        createTime();
        setTime(sec);

        printf("ready! go!\n");

        for(int i = 0; i < sec; ++i)
            pause();

        ret = timer_gettime(timerID, &ts);

        if(ret == -1)
        {
            perror("timer_gettime error");
            return false;
        }

        else 
        {        
            printf("current sec = %ld nsec = %ld\n", ts.it_value.tv_sec, ts.it_value.tv_nsec);
            printf("next sec = %ld nsec = %ld\n", ts.it_interval.tv_sec, ts.it_interval.tv_nsec);
        }

        over = timer_getoverrun(timerID);

        if(over == -1)
        {
            perror("timer_getoverrun error");
            return false;
        }

        else 
        {
            printf("%d overrun(s)\n", over);
        }

        timer_delete(timerID);

        return true;
    }

    return false;
}
