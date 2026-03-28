#pragma once

#define _IS(INPUT) == (INPUT_##input)
#define CASE(INPUT) case INPUT_##input:

typedef enum {
    INPUT_NONE = -1,
    INPUT_MOVE_UP = 'w',
    INPUT_MOVE_LEFT = 'a',
    INPUT_MOVE_DOWN = 's',
    INPUT_MOVE_RIGHT = 'd',
    INPUT_QUIT_PROGRAM = 'q',
    INPUT_TOGGLE_INVENTORY = 'i',
    INPUT_LEFT_CLICK = 'l',
    INPUT_RIGHT_CLICK = 'k'
} Input_t;