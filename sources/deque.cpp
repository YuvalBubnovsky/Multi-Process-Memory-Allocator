#include "deque.hpp"
#include "memory.hpp"

void _print(pdeq deq)
{
    /**
     * @brief prints the object for manual debugging.
     *
     */

    pnode node = deq->head;
    int t = deq->size;
    printf("DEBUG: The Current Dequeue is of size %d: ", t);
    while (node != NULL)
    {
        printf("%s, ", node->value);

        node = node->next;
    }
    printf("\n");
}

void _clear(pdeq deq)
{
    /**
     * @brief clears the entire dequeue object + frees memory
     *
     */
    pnode node = deq->head;
    pnode to_free = NULL;
    while (node != NULL)
    {
        to_free = node;
        node = node->next;
        free(to_free->value);
        free(to_free);
    }
    free(deq);
}

void _PUSH(pdeq deq, pnode node)
{
    /**
     * @brief inserts the value as the head
     *
     */

    /* making sure node ends aren't junk */
    node->next = NULL;
    node->prev = NULL;
    /* ********************************* */

    if (deq->size == 0)
    {
        deq->head = node;
        deq->tail = node;
    }
    else
    {
        node->next = deq->head;
        deq->head->prev = node;
        deq->head = node;
    }
    deq->size += 1;
}

pnode _POP(pdeq deq)
{
    /**
     * @brief removes head from the dequeue and returns a pointer to it
     *
     */
    pnode ans = NULL;

    if (deq->size < 2)
    {
        if (deq->size == 1)
        {
            pnode node = deq->head; // semantic, can take tail as well.
            deq->head = NULL;
            deq->tail = NULL;
            ans = node;
        }
    }
    else
    {
        pnode old_head = deq->head;
        deq->head = old_head->next;
        deq->head->prev = NULL;
        old_head->next = NULL;
        ans = old_head;
    }

    if (ans != NULL)
    {
        deq->size -= 1;
    }
    return ans;
}

pnode _TOP(pdeq deq)
{
    /**
     * @brief returns a pointer to the head of the deqeue
     *
     */
    if (deq->size == 0)
    {
        return NULL;
    }

    return deq->head;
}

void _ENQUEUE(pdeq deq, pnode node)
{
    /**
     * @brief inserts the value as the tail
     *
     */

    /* making sure node ends aren't junk */
    node->next = NULL;
    node->prev = NULL;
    /* ********************************* */

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
}

pnode _DEQUEUE(pdeq deq)
{
    /**
     * @brief removes tail from the dequeue and returns a pointer to it
     *
     */

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
    return ans;
}
