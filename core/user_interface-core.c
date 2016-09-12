#include "user_interface-core.h"
#include "error-core.h"
#include "queue-core.h"
#include "util-core.h"
#include <stdlib.h>

int __std_in_fd__ = 0;

bool initInterface()
{
    __std_in_fd__ = open("/dev/stdin", O_NONBLOCK | O_RDONLY);

    if(__std_in_fd__ == -1)
    {
        setError(ERR_CREATE, "user_interface-core : initInterface fd error");
        return false;
    }
    
    atexit(__releaseInterface__);

    return true;
}

void __releaseInterface__()
{
    close(__std_in_fd__);
}

bool readUser()
{
    char buf[1024];
    int len;
    int err;

    len = read(__std_in_fd__, buf, 1024);

    if(len == -1)
        return false;

    else 
    {
        for(int i = 0; i < strlen(buf); ++i)
        {
            if(buf[i] == 10)
            {
                len = i;
                break;
            }
        }

        /*
        printf("========================\n");
        for(int i = 0; i < len; ++i)
            printf("%c", buf[i]);
        printf("========================\n");
        */
        __makeUserInput__(buf, len);

        return true;
    }

    return false;
}

void delUserInput(USER_INPUT* pUserInput)
{
    for(int i = 0; i < pUserInput->col - 1; ++i)
    {
        safe_free_dptr(&pUserInput->userInput[i]);
        safe_free_dptr(&pUserInput->inputBuffer);
    }

    safe_free_dptr(&pUserInput);
}

void __makeUserInput__(const char* readBuffer, const int len)
{
    USER_INPUT* pUserInput = __createUserInput__();
    int bufIndex = 0;
    int row = 0;

    for(int i = 0; i < len; ++i)
        pUserInput->inputBuffer[i] = readBuffer[i];

    while(pUserInput->col < __TEXT_MAX_COL__)
    {
        row = 0;
        pUserInput->userInput[pUserInput->col] = (char*)calloc(sizeof(char), __TEXT_MAX_ROW__);

//        memset(pUserInput->userInput[pUserInput->col], 0, __TEXT_MAX_ROW__);
//
        while(bufIndex < len)
        {
            if(readBuffer[bufIndex] == ' ')
            {
                bufIndex++;
                break;
            }

            pUserInput->userInput[pUserInput->col][row++] = readBuffer[bufIndex++];
        }

        pUserInput->col++;

        if(bufIndex >= len)
            break;
    }

    pUserInput->userInput[pUserInput->col++] = NULL;

    queue_push(QUEID_USER_INTERFACE, (void*)pUserInput);
}

USER_INPUT* __createUserInput__()
{
    USER_INPUT* pUserInput = (USER_INPUT*)malloc(sizeof(USER_INPUT));
    pUserInput->inputBuffer = (char*)calloc(sizeof(char), 1024);
    pUserInput->col = 0;

    return pUserInput;
}

/*
int main()
{
    if(initInterface())
    {
        while(!readUser());

        USER_INPUT* pUserInput = (USER_INPUT*)queue_get(QUEID_USER_INTERFACE);
        queue_pop(QUEID_USER_INTERFACE);

        for(int i = 0; i < pUserInput->col; ++i)
        {
            if(pUserInput->userInput[i] == NULL)
                printf("%d. NULL\n", i);
            else
                printf("%d. %s\n", i, pUserInput->userInput[i]);
        }

        delUserInput(pUserInput);
    }

    return 0;
}
*/
