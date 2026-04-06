#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include "game.h"
#include "input_map.h"
#include "sys/util/clamp.h"
#include "sys/util/ansi_wrappers.h"

void init_game(Game_t* game) {
    if (!game) return;

    game->game_state = GAMESTATE_PLAYING;
    game->flags = GF_default;
    //game->game->cursor_slot = SLOT_EMPTY;
    //game->game->cursor_idx = 0;
    //game->input_char = 0;
    game->player_cam = (Camera_t){ {1,1}, DEFAULT_VISIBILITY, DEFAULT_VISIBILITY };
    //game->renderer = EMPTY_RENDERER;
    //game->player = (Player_t){ 0 };
    //game->current_floor = 0;
    // clearing floors

    init_renderer(&game->renderer, &game->player_cam);
    init_player(&game->player, "Test player name");

    // test
    floor_init_test_grid(&CURRENT_FLOOR(game));
    inventory_random_fill(game->player.inventory, 3, 7);
}

void game_get_input(Game_t* game) {
    if (!game) return;

    #define ARROW_PREFIX 224
    #define ARROW_UP 72
    #define ARROW_LEFT 75
    #define ARROW_RIGHT 77
    #define ARROW_DOWN 80

    if (_kbhit())
    {
        int input = tolower(_getch());
        if (input == ARROW_PREFIX) {
            switch (_getch())
            {
            case ARROW_UP:
                game->input_char = INPUT_MOVE_UP;
                break;
            case ARROW_LEFT:
                game->input_char = INPUT_MOVE_LEFT;
                break;
            case ARROW_RIGHT:
                game->input_char = INPUT_MOVE_RIGHT;
                break;
            case ARROW_DOWN:
                game->input_char = INPUT_MOVE_DOWN;
                break;
            default:
                break;
            }
        }
        else {
            game->input_char = input;
        }
    }
    else game->input_char = INPUT_NONE;
}

#pragma region separate handle_input_STATE functions
void handle_input_PLAYING(Game_t* game) {
    switch (game->input_char) {
        case INPUT_NONE: break;
        case INPUT_QUIT_PROGRAM: game->flags |= GF_shouldClose; break;
        case INPUT_MOVE_UP:
            if (game->player.pos.y > 0) {
                game->player.pos.y--;
                game->flags |= GF_render;
            }
            break;
        case INPUT_MOVE_DOWN:
            if (game->player.pos.y < MAP_SIZE - 1) {
                game->player.pos.y++;
                game->flags |= GF_render;
            }
            break;
        case INPUT_MOVE_LEFT:
            if (game->player.pos.x > 0) {
                game->player.pos.x--;
                game->flags |= GF_render;
            }
            break;
        case INPUT_MOVE_RIGHT:
            if (game->player.pos.x < MAP_SIZE - 1) {
                game->player.pos.x++;
                game->flags |= GF_render;
            }
            break;
        case INPUT_TOGGLE_INVENTORY:
            game->game_state = GAMESTATE_INVENTORY;
            ansi_clear_screen();
            game->flags |= GF_render;
            break;
    }
}

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
    case INPUT_TOGGLE_INVENTORY: {
        if (container_add(game->player.inventory->cont, &game->cursor_slot) != CONTAINER_RET_OK) {
            // drop item
        }
        game->game_state = GAMESTATE_PLAYING;
        ansi_clear_screen(); // because map rendering does not clear screen
        game->flags |= GF_render; // to print after exiting inventory
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
#pragma endregion

void update_game(Game_t* game) {
    if (!game) return;

    switch (game->game_state)
    {
    case GAMESTATE_TITLE: break;
    case GAMESTATE_PLAYING:
        handle_input_PLAYING(game);
        break;
    case GAMESTATE_INVENTORY:
        handle_input_INVENTORY(game);
        break;
    default:
        printf("Unknown game state: %d\n", game->game_state);
        break;
    }
}

void render_game(Game_t* game) {
    if (!game || !should_render(game)) return;

    game->flags &= ~GF_render;

    switch (game->game_state)
    {
    case GAMESTATE_PLAYING:
        {
        renderer_clear(&game->renderer);

        { // update camera position
            svec2 ncpos = { // new camera position
                game->player.pos.x - game->player_cam.width / 2,
                game->player.pos.y - game->player_cam.height / 2
            };

            game->player_cam.position.x = (short)iclamp(ncpos.x, 0, imax(MAP_SIZE - (game->player_cam.width), 0));
            game->player_cam.position.y = (short)iclamp(ncpos.y, 0, imax(MAP_SIZE - (game->player_cam.height), 0));
        }
        render_floor(&game->renderer, &CURRENT_FLOOR(game));
        renderer_draw_char_pos(&game->renderer, PLAYER_CHAR, game->player.pos);

        renderer_display(&game->renderer);
        printf("\n\nPlayer.pos = (%d, %d)     \n", game->player.pos.x, game->player.pos.y);
        break;
        }
    case GAMESTATE_INVENTORY:
        {
        ansi_goto_home();
        inventory_print(game->player.inventory, game->cursor_idx);
        { // print cursor slot
            putchar('\n'); putchar('\n');
            ansi_red();
            slot_print(&game->cursor_slot);
            ansi_clear_endl();
            ansi_reset();
        }
        break;
        }
    }
}

void free_game(Game_t* game) {
    if (!game) return;

    { // player
        finish_player(&game->player);
        game->player = (Player_t){ 0 };
    }
    { // renderer
        free_renderer(&game->renderer);
        game->renderer = EMPTY_RENDERER;
    }
}


int save_game(Game_t* game);

int load_game(Game_t* game);
