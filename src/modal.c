#include "graphics/modal.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "sys/util/ansi_wrappers.h"
#include "sys/util/clamp.h"

int printf(const char*, ...);
int puts(const char*);
int putchar(int);
int getchar();
//int snprintf(void*, size_t, const char*, ...);

Modal_t* modal_create(svec2 pos, svec2 size, const char* msg, unsigned button_count) {
    Modal_t* m = malloc(sizeof(Modal_t) + button_count * sizeof(Text_button_t));
    if (!m) return NULL;

    m->pos = pos;
    m->size = size;
    m->button_count = button_count;
    m->index = 0;
    if (msg) {
        int len = strlen(msg);
        m->msg_len = len;
        int free_x = size.x - len - 2;
        m->msg_start_x = pos.x + 1 + free_x / 2;
    }
    else {
        m->msg_len = 0;
        m->msg_start_x = pos.x + 2;
    }
    m->message = msg;

    return m;
}

Modal_t* modal_auto_create(svec2 terminal_size, const char* msg, int button_count, ...) {
    if (!msg || button_count <= 0) return NULL;
    
    short max_size_x = terminal_size.x > 80 ? 80 : terminal_size.x - 2;
    short max_size_y = terminal_size.y > 30 ? 30 : terminal_size.y - 2;
    if (max_size_x < 30 || max_size_y < 7) return NULL;

    Modal_t* m = malloc(sizeof(Modal_t) + button_count * sizeof(Text_button_t));
    if (!m) return NULL;

    int sumLen = 0;

    { // init buttons via va_args
        va_list args;
        va_start(args, button_count);
        for (int i = 0; i < button_count; i++) {
            const char* label = va_arg(args, const char*);
            size_t len = strlen(label);

            m->buttons[i] = (Text_button_t){
                i, 0, COLOR_WHITE, len, label
            };

            sumLen += len;
        }
        va_end(args);
    }

    { // determine m->size
        const int space_For_box = 2, padding = 2;
        int spaces_between_buttons = button_count + 1;

        int sum_offset = space_For_box + spaces_between_buttons;
        int msg_offset = space_For_box + 2 * padding;
        
        int msg_len = strlen(msg);

        int max_x = imax(sumLen + sum_offset, msg_len + msg_offset);
        if (max_x < max_size_x) {
            m->size = SVEC2_NEW(max_x, 7);
            m->msg_len = msg_len;
        }
        else {
            printf("to large texts (msg or sum of buttons) given");
            getchar();
            free(m);
            return NULL;
        }
    }

    { // determine m->pos
        int pos_x = (max_size_x - m->size.x) / 2;
        int pos_y = (max_size_y - m->size.y) / 2;
        if (pos_x <= 0 || pos_y <= 0) {
            printf("some error while calculating position happend");
            getchar();
            free(m);
            return NULL;
        }

        m->pos = SVEC2_NEW(pos_x, pos_y);
    }

    { // buttons.start_x
        int free_space = (int)m->size.x - (int)sumLen - 4 /* box & spaces */;
        if (free_space <= button_count) {
            printf("error while calculating start_x for buttons");
            getchar();
            free(m);
            return NULL;
        }

        int diff_space = free_space / button_count;

        int pos_x = m->pos.x + diff_space + 1;
        for (int i = 0; i < m->button_count; i++) {
            m->buttons[i].start_x = pos_x;
            pos_x += diff_space + m->buttons[i].len;
        }
    }

    { // center msg
        int free_x = m->size.x - m->msg_len - 2;
        m->msg_start_x =  (u8)(m->pos.x + 1 + free_x / 2);
    }

    m->button_count = button_count;
    m->index = 0;
    m->message = msg;

    return m;
}

int modal_init_buttons(Modal_t* m, ...) {
    if (!m || m->button_count == 0) return 0;

    size_t sumLen = 0;
    int done = 0;

    va_list args;
    va_start(args, m);

    for (int i = 0; i < m->button_count; i++) {
        const char* label = va_arg(args, const char*);
        size_t len = strlen(label);

        m->buttons[i] = (Text_button_t){
            i, 0, COLOR_WHITE, len, label
        };

        sumLen += len; 
        done++;
    }

    va_end(args);

    int free_space = (int)m->size.x - (int)sumLen - 4;
    if (free_space <= m->button_count) {
        return -1;
        /*
        int poped = 0;
        int current_space = free_space;
        while (current_space <= m->button_count) {
            int index = m->button_count - poped++;
            if (poped >= m->button_count) break;
            current_space -= m->buttons[index].len;
        }
        
        if (poped == m->button_count) { // first is too long

        }
        */
    }

    int diff = (free_space) / m->button_count;

    int pos_x = m->pos.x + 1 + diff;
    for (int i = 0; i < m->button_count; i++) {
        m->buttons[i].start_x = pos_x;
        pos_x += diff + m->buttons[i].len;
    }

    return done;
}

void modal_destroy(Modal_t* m) {
    if (m) free(m);
}

static inline void char_line(int n, char c) {
    for (int i = 0; i < n; i++) putchar(c);
}

static inline void surrounded_char_line(int n, char c, char surround) {
    putchar(surround);
    for (int i = 0; i < n - 2; i++) {
        putchar(c);
    }
    putchar(surround);
}

int modal_draw(Modal_t* m) {
    if (!m) return 1;

    {
        int y = m->pos.y;
        for (int i = 0; i < m->size.y; i++) {
            ansi_goto(m->pos.x, y++);
            if (i == 0 || i == m->size.y - 1)
                char_line(m->size.x, BOX_CHAR);
            else 
                surrounded_char_line(m->size.x, ' ', BOX_CHAR);
        }
    }

    ansi_goto(m->msg_start_x, m->pos.y + 2);
    printf("%s", m->message);

    int button_y = m->pos.y + m->size.y - 3;
    if (button_y < 0) button_y = 0;
    ansi_goto(m->buttons[0].start_x, button_y);
    for (int i = 0; i < m->button_count; i++) {
        ansi_goto(m->buttons[i].start_x, button_y);
        printf("\x1b[%dm%s", m->buttons[i].color, m->buttons[i].label);
        ansi_reset();
    }

    return 0;
}

