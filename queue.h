#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdbool.h>
#define MAX_QUEUE_SIZE 10

typedef int __queue_element__;

bool queue_full();
bool queue_empty();
bool queue_push(__queue_element__ data);
bool queue_pop();
__queue_element__ queue_get();
void queue_debug();

#endif
