#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct node *pnode;

typedef struct node
{
    char* value;
    pnode prev;
    pnode next;
}*pnode;

typedef struct dequeue
{
    size_t size;
    pnode head;
    pnode tail;
}*pdeq; 

void _print(pdeq deq); // control/debugging
void _clear(pdeq deq); // control/debugging
void _PUSH(pdeq deq, pnode node);
pnode _POP(pdeq deq);
pnode _TOP(pdeq deq);
void _ENQUEUE(pdeq deq, pnode node);
pnode _DEQUEUE (pdeq deq);