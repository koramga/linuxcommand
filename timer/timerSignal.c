#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

const char* command = "timer_sig";
const char* commandReview = "[timer_sig][%%d] [%%d is count]";
int alrm = 0;
int vtalrm = 0;

bool progress(int argc, char* argv[]);
void progressSignal(int sig, siginfo_t* siginfo, void* context);

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

    if(result)
    {
        for(int i = 0; i < atoi(argv[1]); ++i)
            pause();
    }

    if(result)
    {
        while(vtalrm != atoi(argv[1]));
    }

    return result;
}

bool progress(int argc, char* argv[])
{
    int interval = 0;
    struct itimerval delay;
    struct sigaction act;
    int ret = 0;

    if(argc >= 2)
    {
        interval = atoi(argv[1]);
        //interval = 5;

        act.sa_sigaction = &progressSignal;
        act.sa_flags = SA_SIGINFO;
    
        if(sigaction(SIGALRM, &act, NULL) < 0)
        {
            perror("sigalarm progress error\n");
            return false;
        }

        if(sigaction(SIGVTALRM, &act, NULL) < 0)
        {
            perror("sigvtalarm progress error\n");
            return false;
        }

        delay.it_value.tv_sec = interval;
        delay.it_value.tv_usec = 0;
        delay.it_interval.tv_sec = 1;
        delay.it_interval.tv_usec = 0;

        
        ret = setitimer(ITIMER_REAL, &delay, NULL);

        if(ret == -1){
            perror("ITIMER_REAL error");
        }

        ret = setitimer(ITIMER_VIRTUAL, &delay, NULL);

        if(ret == -1){
            perror("ITIMER_VIRTUAL | ITIMER_PROF error");
        }
        
    }

    return true;
}

void lineRender(const char* render)
{
    printf("=============%s================\n", render);
}

void progressSignal(int sig, siginfo_t* siginfo, void* context)
{
    if(sig == SIGALRM)
    {
        alrm++;
        lineRender("SIGALRM");
        printf("alrm : %d\n", alrm);
    }

    if(sig == SIGVTALRM)
    {
        vtalrm++;
        lineRender("SIGVTALRM");
        printf("vtalrm : %d\n", vtalrm);
    }
}
