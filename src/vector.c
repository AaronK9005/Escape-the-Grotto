#include "sys/data_structs/vector.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// unused, but maybe a working start point in the future

static inline void* _vector_index(vector_t* v, size_t index) {
    return (char *)v->data + index * v->member_size;
}

static inline vec_ret _vector_grow(vector_t* v) {
    size_t newcap = v->capacity * VEC_GROW_FACTOR;

    void* newdata = realloc(v->data, newcap * v->member_size);
    if (!newdata) return VEC_ERR_OOM;

    v->data = newdata;
    v->capacity = newcap;

    return VEC_RET_OK;
}

vec_ret vector_init(vector_t* v, size_t membsize, size_t startcap) {
    if (!v) return VEC_ERR_NO_VEC;

    if (startcap < VEC_MIN_SIZE) startcap = VEC_MIN_SIZE;

    void* data = malloc(membsize * startcap);
    if (!data) return VEC_ERR_OOM;

    v->data = data;
    v->member_size = membsize;
    v->size = 0;
    v->capacity = startcap;

    return VEC_RET_OK;
}

vec_ret vector_finish(vector_t* v) {
    if (!v) return VEC_ERR_NO_VEC;

    if (v->data) {
        free(v->data);
        v->data = NULL;
    }

    return VEC_RET_OK;
}

vec_ret vector_clear(vector_t* v) {
    if (!v) return VEC_ERR_NO_VEC;

    v->size = 0;

    return VEC_RET_OK;
}

vec_ret vector_front(vector_t* v, void* data) {
    if (!v) return VEC_ERR_NO_VEC;
    if (!data) return VEC_ERR_NO_DATA_PTR;

    memcpy(data, v->data, v->member_size);

    return VEC_RET_OK;
}

vec_ret vector_back(vector_t* v, void* data) {
    if (!v) return VEC_ERR_NO_VEC;
    if (!data) return VEC_ERR_NO_DATA_PTR;

    memcpy(data, _vector_index(v, v->size), v->member_size);

    return VEC_RET_OK;
}

vec_ret vector_add(vector_t* v, void* data) {
    if (!v) return VEC_ERR_NO_VEC;
    if (!data) return VEC_ERR_NO_DATA_PTR;

    while (v->size >= v->capacity) {
        if (_vector_grow(v) != VEC_RET_OK) return VEC_ERR_OOM;
    }

    memcpy(_vector_index(v, v->size++), data, v->member_size);

    return VEC_RET_OK;
}
vec_ret vector_get(vector_t* v, void *dest) {
    if (!v) VEC_ERR_NO_VEC;
    if (!dest) VEC_ERR_NO_DATA_PTR;


}