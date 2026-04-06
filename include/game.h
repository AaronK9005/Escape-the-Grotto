#pragma once

#include "core.h"
#include "graphics/renderer.h"

typedef enum {
    GAMESTATE_TITLE,
    GAMESTATE_PLAYING,
    GAMESTATE_INVENTORY,
} Game_state_t;

#define FLAG(n) (1 << (n))

typedef enum Game_flags_t {
    GF_shouldClose = FLAG(1),
    GF_render = FLAG(2),

    GF_default = GF_render
} Game_flags_t;

#define NUMBER_OF_FLOORS 5

typedef struct Game_t {
    Game_state_t game_state;
    Game_flags_t flags;
    Slot_t cursor_slot;
    int cursor_idx;
    int input_char;
    Camera_t player_cam;
    Renderer_t renderer;

    // game_data
    Player_t player;
    i8 current_floor;
    Floor_t floors[NUMBER_OF_FLOORS];
} Game_t;
//int main() { sizeof(Game_t); }

#define CURRENT_FLOOR(g) ((g)->floors[(g)->current_floor])
#define PLAYER_INV(g) ((g)->player.inventory->cont->slot)

static inline int should_render(Game_t* game) {
    return game && (game->flags & GF_render) != 0;
}

static inline int should_close(Game_t* game) {
    if (!game) return 1;
    return (game->flags & GF_shouldClose) != 0;
}

void init_game(Game_t* game);
void update_game(Game_t* game);
void render_game(Game_t* game);
void free_game(Game_t* game);

int save_game(Game_t* game);
int load_game(Game_t* game);

/*
extern Game_state_t game_state;
extern int input_char;
extern Game_flags_t flags;
extern Game_data_t game_data;
extern Camera_t player_cam;
extern Renderer_t renderer;

// INVENTORY
extern Slot_t cursor_slot;
extern int cursor_idx;
*/
