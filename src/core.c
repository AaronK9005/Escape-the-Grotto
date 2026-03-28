#include "core.h"
#include "input_map.h"
#include "sys/util/ansi_wrappers.h"
#include "sys/util/clamp.h"

int printf(const char*, ...);
int puts(const char*);
int putchar(int);

int rand();

typedef struct _iobuf FILE;
size_t fwrite(const void*, size_t, size_t, FILE*);
FILE* fopen(const char*, const char*);
int fclose(FILE*);

/**
 * IMPLEMENT:
 */

void init_game() {
    init_renderer(&renderer, &player_cam);

    // init game data
    init_player(&game_data.player, "Harold");
    floor_init_test_grid(&GD_CURRENT_FLOOR);
    { // fill inv with random stuff for test
        int r = (rand() % 7) + 3;

        const int w = GD.player.inventory->cols;
        const int h = GD.player.inventory->rows;

        for (int i = 0; i < r; i++) {
            int id = (rand() % (ITEM_COUNT - 1)) + 1;
            int max_stack = item_max_stack[id];
            int count = (rand() % max_stack) + 1;
            
            int x = rand() % w;
            int y = rand() % h;

            if (PLAYER_INV[y * w + x].stack.id == ITEM_NONE) {
                PLAYER_INV[y * w + x] = SLOT_NEW(id, count);
            }
        }
    }
}

void handle_input_PLAYING() {
    switch (input_char) {
        case INPUT_NONE: break;
        case INPUT_QUIT_PROGRAM: flags |= GF_shouldClose; break;
        case INPUT_MOVE_UP:
            if (game_data.player.pos.y > 0) {
                game_data.player.pos.y--;
                flags |= GF_render;
            }
            break;
        case INPUT_MOVE_DOWN:
            if (game_data.player.pos.y < MAP_SIZE - 1) {
                game_data.player.pos.y++;
                flags |= GF_render;
            }
            break;
        case INPUT_MOVE_LEFT:
            if (game_data.player.pos.x > 0) {
                game_data.player.pos.x--;
                flags |= GF_render;
            }
            break;
        case INPUT_MOVE_RIGHT:
            if (game_data.player.pos.x < MAP_SIZE - 1) {
                game_data.player.pos.x++;
                flags |= GF_render;
            }
            break;
        case INPUT_TOGGLE_INVENTORY:
            game_state = GAMESTATE_INVENTORY;
            ansi_clear_screen();
            flags |= GF_render;
            break;
    }
}

void handle_input_INVENTORY() {
    switch (input_char)
    {
        #define cols game_data.player.inventory->cols
        #define rows game_data.player.inventory->rows

    case INPUT_MOVE_UP: {
        if(cursor_idx / cols > 0) {
            cursor_idx -= cols;
            flags |= GF_render;
        }
        break;
    }
    case INPUT_MOVE_DOWN: {
        if(cursor_idx / cols < rows-1) {
            cursor_idx += cols;
            flags |= GF_render;
        }
        break;
    }
    case INPUT_MOVE_LEFT: {
        if(cursor_idx % cols != 0) {
            cursor_idx--;
            flags |= GF_render;
        }
        break;
    }
    case INPUT_MOVE_RIGHT: {
        if(cursor_idx % cols != cols-1) {
            cursor_idx++;
            flags |= GF_render;
        }
        break;
    }
    case INPUT_TOGGLE_INVENTORY: {
        game_state = GAMESTATE_PLAYING;
        ansi_clear_screen();
        flags |= GF_render; // to print after exiting inventory
        break;
    }
    case INPUT_RIGHT_CLICK: {
        if (cursor_slot.stack.id == ITEM_NONE || PLAYER_INV[cursor_idx].stack.id == ITEM_NONE) {
            slot_swap(&cursor_slot, &PLAYER_INV[cursor_idx]);
            flags |= GF_render;
        }
        else {
            if (slot_merge(&cursor_slot, &PLAYER_INV[cursor_idx]) != SLOT_RET_NOOP) flags |= GF_render;
        }
        break;
    }
    case INPUT_LEFT_CLICK: {
        if (cursor_slot.stack.id == ITEM_NONE) {
            if (slot_split(&PLAYER_INV[cursor_idx], &cursor_slot) != SLOT_RET_NOOP) flags |= GF_render;
            // fix: prints inventory when cursor is on empty slot
        }
        break;
    }

    default:
        break;
    }
}

void game_logic() {
    switch (game_state)
    {
    case GAMESTATE_TITLE: break;
    case GAMESTATE_PLAYING:
        handle_input_PLAYING();
        break;
    case GAMESTATE_INVENTORY:
        handle_input_INVENTORY();
        break;
    default:
        printf("Unknown game state: %d\n", game_state);
        break;
    }
}

void render() {
    if (!(flags & GF_render)) return;

    flags &= ~GF_render;

    switch (game_state)
    {
    case GAMESTATE_PLAYING: {
        renderer_clear(&renderer);

        { // update camera position
            svec2 ncpos = { // new camera position
                game_data.player.pos.x - player_cam.width / 2,
                game_data.player.pos.y - player_cam.height / 2
            };

            player_cam.position.x = (short)iclamp(ncpos.x, 0, imax(MAP_SIZE - (player_cam.width + 1) / 2, 0));
            player_cam.position.y = (short)iclamp(ncpos.y, 0, imax(MAP_SIZE - (player_cam.height + 1) / 2, 0));
        }
        render_floor(&renderer, &GD_CURRENT_FLOOR);
        renderer_draw_char_pos(&renderer, PLAYER_CHAR, game_data.player.pos);

        renderer_display(&renderer);
        printf("\n\nPlayer.pos = (%d, %d)     \n", game_data.player.pos.x, game_data.player.pos.y);
        break;
    }
    case GAMESTATE_INVENTORY: {
        ansi_goto_home();
        inventory_print(game_data.player.inventory, cursor_idx);
        { // print cursor slot
            putchar('\n'); putchar('\n');
            ansi_red();
            slot_print(&cursor_slot);
            ansi_clear_endl();
            ansi_reset();
        }
        break;
    }
    }
}

int save_game() {
    FILE* f = fopen("appdata/save_file.bin", "wb");
    if (!f) return 1;

    if (fwrite(&game_data, sizeof(game_data), 1, f) != 1) return 2;

    if (fclose(f) != 0) return 3;

    return 0;
}

void free_game() {
    finish_player(&game_data.player);
}