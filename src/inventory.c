#include "item_system/inventory.h"
#include <stdlib.h>

Inventory_t* new_inventory(int cols, int rows) {
    Inventory_t* inv = malloc(sizeof(Inventory_t));
    if (!inv) return NULL;

    Container_t* cont = container_create(cols * rows);
    if (!cont) {
        free(inv);
        return NULL;
    }

    *inv = (Inventory_t){cols, rows, cont};

    return inv;
}

void delete_inventory(Inventory_t *i) {
    if (i) {
        if (i->cont) {
            container_destroy(i->cont);
            i->cont = NULL;
        }
        free(i);
    }
}

int inventory_random_fill(Inventory_t* i, int min, int max) {
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }

    if (min < 0) min = 0;
    if (max < 0) max = 1;
    if (max > i->cont->slot_count) max = i->cont->slot_count;

    return container_random_fill(i->cont, i->cols, i->rows, min, max);
}