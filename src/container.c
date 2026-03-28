#include "item_system/container.h"
#include <stdlib.h>
#include <string.h>

void slot_swap(Slot_t* s1, Slot_t* s2) {
    ItemStack_t temp = s1->stack;
    s1->stack = s2->stack;
    s2->stack = temp;
}
int slot_merge(Slot_t* src, Slot_t* dest) {
    if (src->stack.id == dest->stack.id) {
        int max_stack = item_max_stack[src->stack.id];
        dest->stack.count += src->stack.count;
        if (dest->stack.count > max_stack) {
            src->stack.count = dest->stack.count - max_stack;
            dest->stack.count -= src->stack.count;
            return SLOT_RET_REMAIN;
        }
        else {
            slot_clear(src);
            return SLOT_RET_FULL;
        }
    }
    return SLOT_RET_NOOP;
}
int slot_split(Slot_t* src, Slot_t* dest) {
    if (
        src->stack.count < 2 ||
        dest->stack.id != 0 ||
        dest->stack.count != 0
        )
        return SLOT_RET_NOOP;
    
    dest->stack.id = src->stack.id;
    dest->stack.count = src->stack.count / 2;
    src->stack.count -= dest->stack.count;
    return SLOT_RET_SPLIT;
}

#pragma region container ctor + dtor
int container_init(Container_t* cont, int nslots) {
    if (!cont || nslots <= 0) return CONTAINER_RET_FAIL;
    
    Slot_t* newslots = malloc(nslots * sizeof(Slot_t));
    if (!newslots) return CONTAINER_RET_FAIL;
    memset(newslots, 0, nslots * sizeof(Slot_t));
    
    cont->slot = newslots;
    cont->slot_count = nslots;
    return CONTAINER_RET_OK;
}

int container_finish(Container_t* cont) {
    if (!cont) return CONTAINER_RET_FAIL;
    
    if (cont->slot) {
        free(cont->slot);
        cont->slot = NULL;
    }
    return CONTAINER_RET_OK;
}

Container_t* container_create(int nslots) {
    Container_t* cont = malloc(sizeof(Container_t));

    if (container_init(cont, nslots) != CONTAINER_RET_OK) {
        free(cont);
        return NULL;
    }
    
    return cont;
}

void container_destroy(Container_t* cont) {
    if (cont) {
        if (cont->slot) {
            free(cont->slot);
            cont->slot = NULL;
        }
        free(cont);
    }
}
#pragma endregion

int container_add(Container_t* cont, Slot_t* slot) {
    if (!cont || !slot) return CONTAINER_RET_FAIL;
    if (slot->stack.id == 0 || slot->stack.count == 0) return CONTAINER_RET_NOOP;
    
    for (int i = 0; i < cont->slot_count; i++) {
        if (slot_merge(slot, &cont->slot[i]) == SLOT_RET_FULL) return CONTAINER_RET_OK;
    }
    
    for (int i = 0; i < cont->slot_count; i++) {
        if (cont->slot[i].stack.count == 0) {
            slot_swap(slot, &cont->slot[i]);
            return CONTAINER_RET_OK;
        }
    }
    
    return CONTAINER_RET_OK;
}

#pragma region (debug) printing
int printf(const char*, ...);
int putchar(int);

void container_print(Container_t* cont, int cols, int selected_index) {
    if (!cont) { 
        printf("cont == NULL\n");
        return;
    }
    for (int i = 0; i < cont->slot_count; i++) {
        if (i == selected_index) printf("\033[94m");
        printf("(%s: %d) ", item_names[cont->slot[i].stack.id], cont->slot[i].stack.count);
        if (i == selected_index) printf("\033[0m");
        if ((i + 1) % cols == 0) {
            printf("\x1b[0K"); // clear to endl
            putchar('\n');
        }
    }
}

void slot_print(Slot_t* slot) {
    if (!slot) {
        printf("slot == NULL\n");
        return;
    }
    printf("\033[31m");
    printf("{%s: %d}", item_names[slot->stack.id], slot->stack.count);
    printf("\033[0m");
}
#pragma endregion