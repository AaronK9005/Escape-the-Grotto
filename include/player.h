#pragma once

#include "sys/math/vec2.h"
#include "item_system/inventory.h"

#define PLAYER_CHAR '@'

typedef struct {
    svec2 pos;
    Inventory_t* inventory;
    const char* name;
} Player_t;

Player_t* new_player(const char* name);
int init_player(Player_t* player, const char* name);

int finish_player(Player_t* p);
void delete_player(Player_t* p);