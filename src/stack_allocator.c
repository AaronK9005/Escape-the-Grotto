#include "sys/data_structs/stack_allocator.h"

// unused rn

stack_allocator_t* _default_stack_allocator = NULL;

void* stack_alloc(stack_allocator_t* alloc, size_t size) {
    if (size == 0 || size > alloc->avaliable) return NULL;
    if (alloc == NULL) alloc = _default_stack_allocator;

    void* retptr = alloc->unused_data;
    alloc->unused_data += size;
    alloc->avaliable -= size;
    return retptr;
}

void stack_clear(stack_allocator_t* alloc) {
    if (!alloc) {
        if (_default_stack_allocator == NULL) return;
        alloc = _default_stack_allocator;
    }

    alloc->avaliable = alloc->capacity;
    alloc->unused_data = alloc->buffer;
}