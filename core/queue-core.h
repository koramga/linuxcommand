#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define __MAX_QUEUE_SIZE__  10
#define QUEID_FORK_RESULT       0
#define QUEID_USER_INTERFACE    1
#define __QUEID_COUNT__         2   

typedef void* __queue_element__;

bool __queue_existID__(int queID);

bool queue_full(int queID);
bool queue_empty(int queID);
bool queue_push(int queID, __queue_element__ data);
bool queue_pop(int queID);
__queue_element__ queue_get(int queID);
void queue_debug(int queID);

#endif
