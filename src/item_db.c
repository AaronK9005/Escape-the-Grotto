#include "item_system/item_db.h"

const char* item_names[ITEM_COUNT] = {
    #define X(IDENTIFIER, string, max_stack) string,
    #include "item_system/item_list.def"
    #undef X
};

const int item_max_stack[ITEM_COUNT] = {
    #define X(IDENTIFIER, string, max_stack) max_stack,
    #include "item_system/item_list.def"
    #undef X
};

ItemInfo_t get_item_info(int index) {
    return (ItemInfo_t){item_names[index], item_max_stack[index]};
}

// (debug) printing {
int printf(const char*, ...);

void print_item_info(ItemInfo_t it) {
    printf("Item_info_t: {name: %s, max_stack: %d}\n", it.name, it.max_stack);
}
// }
