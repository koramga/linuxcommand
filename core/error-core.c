#include "error-core.h"

char __errMsg__[__ERR_MSG_SIZE__];
int __errNum__ = ERR_INIT;

void setErrorMessage(const char* errorMessage)
{
    memset(__errMsg__, 0, __ERR_MSG_SIZE__);

    strcpy(__errMsg__, errorMessage);
}

void setErrorNumber(const int errNum)
{
    __errNum__ = errNum;
}

void setError(const int errNum, const char* errorMessage)
{
    setErrorNumber(errNum);

    if(errorMessage == NULL)
        setErrorMessage("Success");

    else setErrorMessage(errorMessage);
}

const char* getErrorMessage()
{
    if(__errNum__ == ERR_INIT)
        return NULL;

    return __errMsg__;
}

const int getErrorNumber()
{
    return __errNum__;
}

void renderErrorMessage()
{
    printf("errMsg : %s\n", __errMsg__);
}

/* 
int main()
{
    setError(ERR_OVERFLOW, "error overflow");
    
    if(getErrorNumber() == ERR_OVERFLOW)
        renderErrorMessage();

    else printf("error\n");
}
*/
