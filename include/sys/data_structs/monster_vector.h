#pragma once

typedef struct Monster_t Monster_t;

typedef struct Monster_vector_t {
    Monster_t* data;
    int capacity;
    int count;
} Mvec_t;

// int main() {sizeof(Mvec_t)}

int mvec_init(Mvec_t* v, int least_count);
int mvec_finish(Mvec_t* v);

int mvec_add(Mvec_t* v, Monster_t* m);
int mvec_get(Mvec_t* v, Monster_t* m, int index);
int mvec_remove(Mvec_t* v, int index);

// fix
int mvec_n_remove(Mvec_t* v, int count, ...);