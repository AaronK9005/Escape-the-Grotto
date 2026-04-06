#ifndef _MY_MC_LIKE_CONTAINERS_V4_CONTAINER_H
#define _MY_MC_LIKE_CONTAINERS_V4_CONTAINER_H

#include "item_db.h"

typedef struct Slot_t {
    ItemStack_t stack;
} Slot_t;

#define SLOT_EMPTY (Slot_t){ {0,0} }
#define SLOT_NEW(id, count) (Slot_t){ {id, count} }

static inline void slot_clear(Slot_t* slot) {
    slot->stack.id = 0; slot->stack.count = 0;
}

void slot_swap(Slot_t* s1, Slot_t* s2);
int slot_merge(Slot_t* src, Slot_t* dest);
int slot_split(Slot_t* src, Slot_t* dest);

typedef struct Container_t {
    Slot_t* slot;
    int slot_count;
} Container_t;

typedef enum {
    SLOT_RET_NOOP,
    SLOT_RET_FULL,
    SLOT_RET_REMAIN,
    SLOT_RET_SPLIT
} slot_ret_val_t;

typedef enum {
    CONTAINER_RET_OK,
    CONTAINER_RET_NOOP,
    CONTAINER_RET_FAIL
} container_ret_val_t;

int container_init(Container_t* cont, int nslots);
int container_finish(Container_t* cont);

Container_t* container_create(int nslots);
void container_destroy(Container_t* cont);

int container_add(Container_t* cont, Slot_t* slot);

int container_random_fill(Container_t* cont, int w, int h, int min_items, int max_items);

void container_print(Container_t* cont, int cols, int selected_index);
void slot_print(Slot_t* slot);

#endif