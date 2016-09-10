#include "queue-core.h"

__queue_element__ __queue_elements__[__QUEID_COUNT__][__MAX_QUEUE_SIZE__];
int __queue_front__[__QUEID_COUNT__] = {0, };
int __queue_rear__[__QUEID_COUNT__] = {0, };

bool __queue_existID__(int queID)
{
    if(queID >= __QUEID_COUNT__)
        return false;

    return true;
}

bool queue_full(int queID)
{
    if(__queue_existID__(queID))
    {
        if (((__queue_rear__[queID] + 1) % __MAX_QUEUE_SIZE__) == __queue_front__[queID])     //full
            return true;

        return false;
    }

    return false;
}

bool queue_empty(int queID)
{
    if(__queue_existID__(queID))
    {
        if (__queue_rear__[queID] == __queue_front__[queID])
            return true;

        return false;
    }

    return false;
}

bool queue_push(int queID, __queue_element__ data)
{
    if(__queue_existID__(queID))
    {
        if (!queue_full(queID))
        {
            __queue_rear__[queID] = (__queue_rear__[queID] + 1) % __MAX_QUEUE_SIZE__;
            __queue_elements__[queID][__queue_rear__[queID]] = data;
            return true;
        }

        return false;
    }

    return false;
}

bool queue_pop(int queID)
{
    if(__queue_existID__(queID))
    {
        if (!queue_empty(queID))
        {
            __queue_front__[queID] = (__queue_front__[queID] + 1) % __MAX_QUEUE_SIZE__;
            return true;
        }

        return false;
    }

    return false;
}

__queue_element__ queue_get(int queID)
{
    if(__queue_existID__(queID))
    {
        return __queue_elements__[queID][(__queue_front__[queID] + 1) % __MAX_QUEUE_SIZE__];
    }

    return 0;
}

void queue_debug(int queID)
{
    for (int i = 0; i < __MAX_QUEUE_SIZE__; ++i)
        printf("%d\n", __queue_elements__[queID][i]);
}

