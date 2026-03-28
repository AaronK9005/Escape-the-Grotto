#ifndef _MY_MC_LIKE_CONTAINERS_V4_ITEM_DATABASE_H
#define _MY_MC_LIKE_CONTAINERS_V4_ITEM_DATABASE_H

typedef enum {
#define X(IDENTIFIER, string, max_stack) ITEM_##IDENTIFIER,
    #include "item_list.def"
#undef X
    ITEM_COUNT
} ItemID_t;

extern const char*  item_names[ITEM_COUNT];
extern const int    item_max_stack[ITEM_COUNT];

typedef struct {
    const char* name;
    int max_stack;
} ItemInfo_t;

ItemInfo_t get_item_info(int index);
void print_item_info(ItemInfo_t);

typedef struct {
    ItemID_t id;
    int count;
} ItemStack_t;

#endif