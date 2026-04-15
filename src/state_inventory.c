#include "game.h"
#include "input_map.h"
//#include "sys/util/ansi_wrappers.h"

void handle_input_INVENTORY(Game_t* game) {
    switch (game->input_char)
    {
        #define cols game->player.inventory->cols
        #define rows game->player.inventory->rows

    case INPUT_MOVE_UP: {
        if(game->cursor_idx / cols > 0) {
            game->cursor_idx -= cols;
            game->flags |= GF_render;
        }
        break;
    }
    case INPUT_MOVE_DOWN: {
        if(game->cursor_idx / cols < rows-1) {
            game->cursor_idx += cols;
            game->flags |= GF_render;
        }
        break;
    }
    case INPUT_MOVE_LEFT: {
        if(game->cursor_idx % cols != 0) {
            game->cursor_idx--;
            game->flags |= GF_render;
        }
        break;
    }
    case INPUT_MOVE_RIGHT: {
        if(game->cursor_idx % cols != cols-1) {
            game->cursor_idx++;
            game->flags |= GF_render;
        }
        break;
    }

        #undef cols
        #undef rows

    case INPUT_TOGGLE_INVENTORY: {
        if (container_add(game->player.inventory->cont, &game->cursor_slot) != CONTAINER_RET_OK) {
            // drop item
        }
        game->game_state = GAMESTATE_PLAYING;
        game->flags |= GF_render | GF_clear_screen; // to print after exiting inventory
        break;
    }
    case INPUT_RIGHT_CLICK: {
        if (game->cursor_slot.stack.id == ITEM_NONE || PLAYER_INV(game)[game->cursor_idx].stack.id == ITEM_NONE) {
            slot_swap(&game->cursor_slot, &PLAYER_INV(game)[game->cursor_idx]);
            game->flags |= GF_render;
        }
        else {
            if (slot_merge(&game->cursor_slot, &PLAYER_INV(game)[game->cursor_idx]) != SLOT_RET_NOOP) game->flags |= GF_render;
        }
        break;
    }
    case INPUT_LEFT_CLICK: {
        if (game->cursor_slot.stack.id == ITEM_NONE) {
            if (slot_split(&PLAYER_INV(game)[game->cursor_idx], &game->cursor_slot) != SLOT_RET_NOOP) game->flags |= GF_render;
            // fix: prints inventory when cursor is on empty slot
        }
        else if (PLAYER_INV(game)[game->cursor_idx].stack.id == ITEM_NONE ||
                 PLAYER_INV(game)[game->cursor_idx].stack.id == game->cursor_slot.stack.id) {
            game->flags |= GF_render;
            PLAYER_INV(game)[game->cursor_idx] = SLOT_NEW(game->cursor_slot.stack.id, ++PLAYER_INV(game)[game->cursor_idx].stack.count);
            game->cursor_slot.stack.count--;
            if (game->cursor_slot.stack.count <= 0) slot_clear(&game->cursor_slot);
        }
        break;
    }

    default:
        break;
    }
}