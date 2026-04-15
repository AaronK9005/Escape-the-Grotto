#pragma once

/**
 * STD_CPP_VECTOR
 * raw memory ptr
 * member_size based
 * 
 * resolve usage func names
 */

 #include <stddef.h>

typedef struct std_cpp_vector {
    void* data;
    size_t member_size;
    size_t size;
    size_t capacity;
} vector_t;

typedef enum vector_return_values {
    VEC_RET_OK,
    VEC_ERR_INVALID_ARGS,
    VEC_ERR_NO_VEC,
    VEC_ERR_NO_DATA_PTR,
    VEC_ERR_OOM, // out of memory
    VEC_WARN_MAX_CAPACITY
} vec_ret;

#define VEC_MIN_SIZE 8
#define VEC_GROW_FACTOR 2

vec_ret vector_init(vector_t* v, size_t membsize, size_t startcap);
vec_ret vector_finish(vector_t* v);

vec_ret vector_clear(vector_t* v);

vec_ret vector_front(vector_t* v, void* data);
vec_ret vector_back(vector_t*v, void* data);
vec_ret vector_add(vector_t* v, void* data);
vec_ret vector_get(vector_t* v, void *dest);