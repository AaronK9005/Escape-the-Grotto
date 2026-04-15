#include "game.h"
#include "input_map.h"
#include "sys/util/ansi_wrappers.h"

static inline int try_player_move(Game_t* game, int dx, int dy) {
    short new_x = PLAYER(game).pos.x + dx;
    short new_y = PLAYER(game).pos.y + dy;

    if (new_x < 0 || new_x >= MAP_SIZE) return 0;
    if (new_y < 0 || new_y >= MAP_SIZE) return 0;
    if (!PLAYER_CAN_WALK(game, dx, dy)) return 0;

    PLAYER(game).pos.x = new_x;
    PLAYER(game).pos.y = new_y;

    return 1;
}

static inline int toggle_debug_cam(Game_t* game) {
    game->flags ^= GF_using_debug_cam;

    if (game->flags & GF_using_debug_cam)
    {
        renderer_change_cam(&game->renderer, &game->debug_cam); // todo: check return
    }
    else
    {
        renderer_change_cam(&game->renderer, &game->player_cam);
    }


}

void handle_input_PLAYING(Game_t* game) {
    switch (game->input_char) {
        case INPUT_NONE: break;
        case INPUT_QUIT_PROGRAM: game->flags |= GF_shouldClose; break;
        case INPUT_MOVE_UP:     if (try_player_move(game,  0, -1)) game->flags |= GF_render; break;
        case INPUT_MOVE_DOWN:   if (try_player_move(game,  0,  1)) game->flags |= GF_render; break;
        case INPUT_MOVE_LEFT:   if (try_player_move(game, -1,  0)) game->flags |= GF_render; break;
        case INPUT_MOVE_RIGHT:  if (try_player_move(game,  1,  0)) game->flags |= GF_render; break;
        case INPUT_TOGGLE_INVENTORY: CHANGE_STATE(game, GAMESTATE_INVENTORY); break;
        case INPUT_TOGGLE_DEBUG_CAM: toggle_debug_cam(game); game->flags |= GF_render | GF_clear_screen; break;
    }
}

