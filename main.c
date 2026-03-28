#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sys/util/ansi_wrappers.h"

#include "main_cleanup.inl"

#include "graphics/pop_up_win.h"

int main()
{
    printf("loading game ...\n");

#if RUN_MODAL_DEMO
    Modal_t* mod = modal_create( SVEC2_NEW(10,10), SVEC2_NEW(40, 10), "please work", 4);
    if (!mod) return 1;
    modal_init_buttons(mod, "ok", "sure", "omg", "really?");
    mod->buttons[1].color = COLOR_BLUE;
    mod->buttons[2].color = COLOR_GREEN;

    ansi_clear_screen();
    modal_draw(mod);
    getchar();
    ansi_clear_screen();

    modal_destroy(mod);
    mod = NULL;
    int cols, rows;
    if (get_console_size(&rows, &cols) != 0) {
        printf("failed to get console size");
        return 0;
    }
    mod = modal_auto_create(SVEC2_NEW(cols, rows), "this was created with auto create", 3, "1", "2", "3");
    if (!mod) return 1;
    mod->buttons[1].color = COLOR_BLUE;
    mod->buttons[2].color = COLOR_GREEN;
    modal_draw(mod);
    getchar();
    ansi_clear_screen();
    modal_destroy(mod);

    return 0;
#endif

    srand(time(NULL));
    init_game();

    ansi_clear_screen();
    ansi_hide_cursor();
    
    while (!GET(GF_shouldClose)) {
        input_char = get_input();
        game_logic();
        render();
    }

    free_game();
    free_renderer(&renderer);
    
    ansi_show_cursor();

    return 0;
}