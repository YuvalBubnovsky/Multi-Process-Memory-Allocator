#include "memory.hpp"


// Custom memory manager, using greedy method of searching a free space on the heap
// Once an allocation request is submitted.

// Reference - https://nyu-cso.github.io/notes/14-Dynamic_Allocator.pdf

void *global_base = NULL;

chunk *get_chunk_ptr(void *ptr)
{
    return (chunk *)ptr - 1;
}

// Find the first free chunk in the heap
chunk *find_free_block(chunk **last, size_t size)
{
    chunk *current = static_cast<chunk *>(global_base);
    while (current && !(current->free && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
    return current;
}


chunk *request_space(chunk *last, size_t size)
{
    chunk *block;
    block = static_cast<chunk *>(sbrk(0));
    void *request = sbrk(size + CHUNK_SIZE);
    if (request == (void *)-1)
    {
        return NULL; // sbrk failed.
    }
    if (last)
    { // NULL on first request.
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}

void *malloc(size_t _size)
{
    chunk *block;
    size_t size = ALIGN(_size); // Aligning size to 16 bits

    if (_size <= 0)
    {
        return NULL;
    }

    if (!global_base)
    { // First call.
        block = request_space(NULL, size);
        if (!block)
        {
            return NULL;
        }
        global_base = block;
    }
    else
    {
        chunk *last = static_cast<chunk *>(global_base);
        block = find_free_block(&last, size);
        if (!block)
        { // Failed to find free block.
            block = request_space(last, size);
            if (!block)
            {
                return NULL;
            }
        }
        else
        { // Found free block
            // We don't split blocks here, to keep this simple
            block->free = 0;
        }
    }

    return (block + 1); // Since we finished at the last allocated block, increment pointer to point to a free one.
}

void *calloc(size_t no_elements, size_t element_size)
{
    size_t size = no_elements * element_size;
    void *ptr = malloc(size); // neat trick to write less code, allocate the space requested and use a built-in function to zero it
    bzero(ptr, size);
    return ptr;
}

void free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    chunk *chunk_ptr = get_chunk_ptr(ptr);
    chunk_ptr->free = 1; // set the free flag so next requests can use this block and overwrite it.
}