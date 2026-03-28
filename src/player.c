#include "player.h"
#include <stdlib.h>

int printf(const char*, ...);

 Player_t* new_player(const char* name) {
    Player_t* p = malloc(sizeof(Player_t));

    if (init_player(p, name) != 0) {
        free(p);
        return NULL;
    }

    return p;
}

int init_player(Player_t* player, const char* name) {
    if (!player) return 1;

    Inventory_t* i = new_inventory(9, 3);
    if (!i) {
        printf("Inventory allocation failed\n");
        return 2;
    }

    player->pos = SVEC2_NEW(1, 1);
    player->inventory = i;
    player->name = name;

    return 0;
}

int finish_player(Player_t* p) {
    if (!p) return 1;

    if (p->inventory) {
        delete_inventory(p->inventory);
        p->inventory = NULL;
    }

    return 0;
}

void delete_player(Player_t* p) {
    if (p) {
        finish_player(p);
        free(p);
    }
}