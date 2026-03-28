#include "sys/data_structs/monster_vector.h"
#include "monster.h"
#include <stdlib.h>
#include <stdarg.h>

#define MIN_SIZE 8
#define GROW_FACTOR 2
#define SHRINK_FACTOR 2

#define MVEC_GROW(v) mvec_resize_data((v), (v)->capacity * GROW_FACTOR)
#define MVEC_SHRINK(v) mvec_resize_data((v), (v)->capacity / SHRINK_FACTOR)

/**
 * helper, needs non-null v
 */
static int mvec_resize_data(Mvec_t* v, size_t newcap) {
    if (newcap < MIN_SIZE) newcap = MIN_SIZE;

    Monster_t* newdata = realloc(v->data, newcap * sizeof(Monster_t));
    if (!newdata) { return 1; }

    v->data = newdata;
    v->capacity = newcap;
    return 0;
}

int mvec_init(Mvec_t* v, int least_count){
    if (!v || least_count < 0) return 1;

    size_t newcap = MIN_SIZE;
    while (newcap < least_count) newcap *= GROW_FACTOR;

    v->data = NULL;
    v->count = 0;
    if (mvec_resize_data(v, newcap) != 0) {
        return 1;
    }

    return 0;
}

int mvec_finish(Mvec_t* v) {
    if (!v) return 1;
    if (v->data) {
        free(v->data);
        v->data = NULL;
    }
    return 0;
}

int mvec_add(Mvec_t* v, Monster_t* m) {
    if (!v || !m) return 1;

    if (v->count == v->capacity) {
        if (MVEC_GROW(v) != 0) return 1;
    }

    v->data[v->count++] = *m;

    return 0;
}

int mvec_get(Mvec_t* v, Monster_t* m, int index) {
    if (!v || !m || index < 0) return 1;

    if (index >= v->count) {
        return 2;
    }

    *m = v->data[index];

    return 0;
}

int mvec_remove(Mvec_t* v, int index) {
    if (!v || index < 0) return 1;

    if (index >= v->count) {
        return 2;
    }

    v->count--;
    if (index != v->count) {
        v->data[index] = v->data[v->count];
    }

    return 0;
}

// redo with int* indexes instead
/* fix apparently
int mvec_n_remove(Mvec_t* v, int count, ...) {
    if (!v || count <= 0) return 1;
    if (count > 16) return 2;

    int indexes[16];
    int actuall_c = 0;

    { // va_args work
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        int idx = va_arg(args, int);

        if (idx < 0 || idx >= v->count) continue; // skip invalid index

        // ignore duplicates
        int j;
        for (j = 0; j < i; j++) {
            if (idx == indexes[j]) break;
        }
        if (j != actuall_c) continue;

        indexes[actuall_c++] = idx; // everything ok, add index
    }
    va_end(args);
    }

    // sort descending
    for (int i = 0; i < actuall_c - 1; i++) {
        for (int j = i + 1; j < actuall_c; j++) {
            if (indexes[i] < indexes[j]) {
                int tmp = indexes[i];
                indexes[i] = indexes[j];
                indexes[j] = tmp;
            }
        }
    }

    // remove
    for (int i = 0; i < actuall_c; i++) {
        int idx = indexes[i];

        v->count--;
        if (idx != v->count) {
            v->data[idx] = v->data[v->count];
        }
    }

    return 0;
}
    */