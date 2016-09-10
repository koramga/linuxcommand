#include "queue-core.h"

__queue_element__ __queue_elements__[MAX_QUEUE_SIZE];
__queue_element__ __queue_front__ = 0;
__queue_element__ __queue_rear__ = 0;

bool queue_full()
{
        if (((__queue_rear__ + 1) % MAX_QUEUE_SIZE) == __queue_front__)     //full
                    return true;

            return false;
}

bool queue_empty()
{
        if (__queue_rear__ == __queue_front__)
                    return true;

            return false;
}

bool queue_push(__queue_element__ data)
{
        if (!queue_full())
                {
                            __queue_rear__ = (__queue_rear__ + 1) % MAX_QUEUE_SIZE;
                                    __queue_elements__[__queue_rear__] = data;
                                            return true;
                                                }

            return false;
}

bool queue_pop()
{
        if (!queue_empty())
                {
                            __queue_front__ = (__queue_front__ + 1) % MAX_QUEUE_SIZE;
                                    return true;
                                        }

            return false;
}

__queue_element__ queue_get()
{
        return __queue_elements__[(__queue_front__ + 1) % MAX_QUEUE_SIZE];
}

void queue_debug()
{
        for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
                    printf("%d\n", __queue_elements__[i]);
}

