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