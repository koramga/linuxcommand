#include "util-core.h"

//const char* __errMsg__[__ERR_MSG_SIZE__];
//int __errNum__ = ERR_INIT;

void safe_free_ptr(void* data)
{
    if(data == NULL)
        free((void*)data);
}

void safe_free_dptr(void** data)
{
    if((*data) != NULL)
        free((void*)(*data));

    (*data) = NULL;
}

/*
int main()
{
    //test main
    int* pNumber = (int*)malloc(sizeof(int) * 100);

    for(int i = 0; i < 100; ++i)
    {
        pNumber[i] = i;
    }

    for(int i = 0; i < 100; ++i)
        printf("%d\n", pNumber[i]);

    __safe_free__(&pNumber);
    printf("delete data");
    __safe_free__(&pNumber);
}
*/
