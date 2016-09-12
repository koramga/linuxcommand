#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const char* command = "";
const char* commandReview = "";

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

}
