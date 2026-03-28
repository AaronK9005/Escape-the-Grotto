#pragma once


#include "core.h"


#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <ctype.h>

    #define setNonBlocking(bool) 

    int get_input() {
        if (_kbhit()) { return tolower(_getch()); }
        else return -1;
    }
#else
    #error Compilation failed: Currently this game only supports Windows (error in get_input).
#endif

int input_char = 0;
Game_state_t game_state = GAMESTATE_PLAYING;
Game_flags_t flags = GF_default;
Game_data_t game_data = { 0 };
Camera_t player_cam = { {1, 1}, DEFAULT_VISIBILITY, DEFAULT_VISIBILITY };
Renderer_t renderer = EMPTY_RENDERER;

// INVENTORY
Slot_t cursor_slot = { 0, 0 };
int cursor_idx = 0;

int moved = true; // temporary
