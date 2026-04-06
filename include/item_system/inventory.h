#pragma once

#include "container.h"

typedef struct {
    int cols;
    int rows;
    Container_t* cont;
} Inventory_t;

Inventory_t* new_inventory(int cols, int rows);
void delete_inventory(Inventory_t *i);

int inventory_random_fill(Inventory_t* i, int min, int max);

static inline void inventory_print(Inventory_t* i, int selected_idx)  {
    if (i) container_print(i->cont, i->cols, selected_idx);
}
