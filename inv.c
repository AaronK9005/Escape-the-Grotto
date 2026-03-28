#include <stdio.h>
#include <ctype.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>

    #define setNonBlocking(bool) 

    int get_input() {
        if (_kbhit()) { return tolower(_getch()); }
        else return -1;
    }
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>

    void setNonBlocking(int enable) {
        struct termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        if (enable) {
            ttystate.c_lflag &= ~ICANON; // znak po znaku
            ttystate.c_lflag &= ~ECHO;   // nevypisovat
        } else {
            ttystate.c_lflag |= ICANON;
            ttystate.c_lflag |= ECHO;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        if (enable)
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        else
            fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    }

    int get_input() {
        return towlower(getchar());
    }
#endif

#include <stdlib.h>
#include <time.h>

#include "container.h"

#define endl putchar('\n')

#define inv_print container_print(inventory, cols, cursor_idx); endl; slot_print(&cursor)

#define inv_putstack(stack) container_add(inventory, &(Slot_t){ stack })
#define inv_putfields(id, count) container_add(inventory, &(Slot_t){ {id, count} })

#define inv_reprint printf("\033[H\033[2J"); inv_print

const int cols = 9, rows = 3;
int cursor_idx = 0;

void print_selection(Container_t* cont);

int main()
{
    srand(time(NULL));

    Container_t* inventory = container_create(cols * rows);
    Slot_t cursor = SLOT_EMPTY;
    
    // random fill inventory {
    int r = (rand() % 7) + 3;
    for (int i = 0; i < r; i++) {
        int id = (rand() % (ITEM_COUNT - 1)) + 1;
        int max_stack = item_max_stack[id];
        int count = (rand() % max_stack) + 1;
        
        inv_putfields(id, count);
    }
    // }
    
    int c;
    setNonBlocking(1);
    printf("\x1B[?25l"); // hide cursor
    inv_reprint;

    while (1) {
        c = get_input();
        if (c == -1) continue;

        if (c == 'q') break;
        switch(c) {
            case 'a': if(cursor_idx % cols != 0) { cursor_idx--; inv_reprint; } break;
            case 'd': if(cursor_idx % cols != cols-1) { cursor_idx++; inv_reprint; } break;
            case 'w': if(cursor_idx / cols > 0) { cursor_idx -= cols; inv_reprint; } break;
            case 's': if(cursor_idx / cols < rows-1) { cursor_idx += cols; inv_reprint; } break;
            case 'k': // right click
                if (cursor.stack.id == ITEM_NONE || inventory->slot[cursor_idx].stack.id == ITEM_NONE) {
                    slot_swap(&cursor, &inventory->slot[cursor_idx]);
                    inv_reprint;
                }
                else {
                    if (slot_merge(&cursor, &inventory->slot[cursor_idx]) != SLOT_RET_NOOP) inv_reprint;
                }
                break;
            case 'l': // left click
                if (cursor.stack.id == ITEM_NONE) {
                    if (slot_split(&inventory->slot[cursor_idx], &cursor) != SLOT_RET_NOOP) inv_reprint;
                    // fix: prints inventory when cursor is on empty slot
                }
                break;
        }
    }
    
    setNonBlocking(0);
    
    return 0;
}