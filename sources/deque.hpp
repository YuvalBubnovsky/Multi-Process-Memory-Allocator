#ifndef DEQUE
#define DEQUE

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "memory.h"

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
    char value[1024];
    struct dequeue* next;
    struct dequeue* prev;
}*pdeq;

typedef struct deqMap
{
    pdeq first;
    pdeq curr;
}*pdm;


/* *** Mutex Related *** */

int _locker;
struct flock _lock;
struct deqMap *_dm;

void lock_mutex()
{
    _lock.l_type = F_WRLCK;

    if (fcntl(_locker, F_SETLKW, &_lock) == -1)
    {
        perror("fcntl_lock");
        exit(1);
    }
}

void unlock_mutex()
{
    _lock.l_type = F_UNLCK;

    if (fcntl(_locker, F_SETLKW, &_lock) == -1)
    {
        perror("fcntl_lock");
        exit(1);
    }
    close(_locker);
}


void _print()
{
    /**
     * @brief prints the object for manual debugging.
     *
     

    pnode node = deq->head;
    int t = deq->size;
    printf("DEBUG: The Current Dequeue is of size %d: ", t);
    while (node != NULL)
    {
        printf("%s, ", node->value);

        node = node->next;
    }
    printf("\n");*/
    // TODO: implement this function
}


void _PUSH(char* str)
{
    lock_mutex();
    if(_dm->curr==NULL){
        _dm->curr = _dm->first;
        strcpy(_dm->curr->value,str); 
        _dm->curr->next = NULL;
    } else {
        pdeq temp = _dm->curr + 1;
        strcpy(temp->value,str); 
        temp->next = _dm->curr;
        _dm->curr = temp;
    }
    unlock_mutex();
}

char* _POP()
{
    lock_mutex();
    char* str;
    if (_dm->curr == NULL) {
        return "\nQueue Is Empty!!\n";

    } else {
        str = _dm->curr->value;
        _dm->curr = _dm->curr->next;
    }
    unlock_mutex();
    return str;
}

char* _TOP()
{
    /**
     * @brief returns a pointer to the head of the deqeue
     *
     */
    lock_mutex();
    if(_dm->curr==NULL){
        return "\nQueue Is Empty!!\n";
    }
    char* str;
    unlock_mutex();
    return strcpy(str,_dm->curr->value);
}

void _ENQUEUE(char* str)
{
    /**
     * @brief inserts the value as the tail
     *
     

    

    lock_mutex();
    node->next = NULL;
    node->prev = NULL;
    /* ********************************* 

    if (deq->size == 0)
    {
        deq->head = node;
        deq->tail = node;
    }
    else
    {
        node->prev = deq->tail;
        deq->tail->next = node;
        deq->tail = node;
    }
    deq->size += 1;
    unlock_mutex();*/
}

char* _DEQUEUE()
{
    // TODO: implement this function
    /**
     * @brief removes tail from the dequeue and returns a pointer to it
     *
     *

    lock_mutex();
    pnode ans = NULL;

    if (deq->size < 2)
    {
        if (deq->size == 1)
        {
            pnode node = deq->tail; // semantic, can take head as well.
            deq->head = NULL;
            deq->tail = NULL;
            ans = node;
        }
    }
    else
    {
        pnode old_tail = deq->tail;
        deq->tail = old_tail->prev;
        deq->tail->next = NULL;
        old_tail->prev = NULL;
        ans = old_tail;
    }

    if (ans != NULL)
    {
        deq->size -= 1;
    }
    unlock_mutex();
    return ans;*/
    return " ";
}


#endif