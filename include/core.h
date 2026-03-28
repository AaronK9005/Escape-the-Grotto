#pragma once

#include <stdbool.h>
#include "sys/util/my_stdint.h"
#include "player.h"
#include "floor.h"
#include "graphics/renderer.h"

typedef enum {
    GAMESTATE_TITLE,
    GAMESTATE_PLAYING,
    GAMESTATE_INVENTORY,
} Game_state_t;

#define NUMBER_OF_FLOORS 5

typedef struct {
    Floor_t floors[NUMBER_OF_FLOORS];
    i8 current_floor;
    Player_t player;
} Game_data_t;

#define GD game_data
#define PL player
#define INV inventory

#define GD_CURRENT_FLOOR game_data.floors[game_data.current_floor]
#define PLAYER_INV game_data.player.inventory->cont->slot

// int main() { sizeof(Game_data_t); }

#define GET(flag) (flags & (flag))
#define SET(flag) flags |= (flag)
#define UNSET(flag) flags &= ~ (flag)
#define FLAG(n) (1 << (n))

typedef enum {
    GF_shouldClose = FLAG(1),
    GF_render = FLAG(2),

    GF_default = GF_render
} Game_flags_t;

extern Game_state_t game_state;
extern int input_char;
extern Game_flags_t flags;
extern Game_data_t game_data;
extern Camera_t player_cam;
extern Renderer_t renderer;

// INVENTORY
extern Slot_t cursor_slot;
extern int cursor_idx;

void init_game();
void game_logic();
void render();

int save_game();
void free_game();