#pragma once

/**
 * @todo implement support with gamestate
 * @todo change *.start_x to be relative
 */

#include "sys/math/vec2.h"
#include "sys/util/my_stdint.h"

typedef enum ANSI_color_codes {
    COLOR_BLACK = 30,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE = 37,
    COLOR_DEFAULT = 39
} Color_code_t;

typedef struct Text_button_t {
    i16 id, start_x;
    i16 color, len;
    const char* label;
} Text_button_t;

#define BOX_CHAR '*'

typedef struct Pop_up_window_t {
    svec2 pos, size;

    u8 button_count, index;
    i16 msg_len;
    u8 msg_start_x;
    // +3 B

    const char* message;
    Text_button_t buttons[];
} Modal_t;

 // int main() { sizeof(Modal_t); }

Modal_t* modal_create(svec2 pos, svec2 size, const char* msg, unsigned button_count);

Modal_t* modal_auto_create(svec2 terminal_size, const char* msg, int button_count, ...);

int modal_init_buttons(Modal_t* m, ...);

void modal_destroy(Modal_t* m);

int modal_draw(Modal_t* m);

//static inline void modal_set_box_char(Modal_t* m, char c) { if(m) m->box_char = c;}
