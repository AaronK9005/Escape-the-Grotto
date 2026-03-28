#pragma once

// unused rn

#include <stddef.h>

typedef struct {
    char* unused_data;
    size_t capacity;
    size_t avaliable;
    char buffer[];
} stack_allocator_t;

extern stack_allocator_t* _default_stack_allocator;

#define new_stack_allocator_t(name, _capacity) \
    char _data_of_##name[sizeof(stack_allocator_t) + _capacity]; \
    stack_allocator_t* name = (stack_allocator_t*)_data_of_##name; \
    name->unused_data = name->buffer; \
    name->capacity = _capacity; \
    name->avaliable = _capacity

void* stack_alloc(stack_allocator_t* alloc, size_t size);
void stack_clear(stack_allocator_t* alloc);

static inline void* salloc(size_t size) {
    return stack_alloc(_default_stack_allocator, size);
}

static inline void sclear() {
    stack_clear(_default_stack_allocator);
}

static inline void sinit(stack_allocator_t* alloc) {
    _default_stack_allocator = alloc;
}

