#include "graphics/renderer.h"

#include "sys/util/ansi_wrappers.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_console_size(int *rows, int *cols) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(h, &csbi)) {
        return 1;
    }
    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 0;
}

/**
 * needs valid camera to work
 */
static int _renderer_resize_buffer(Renderer_t* r) {
    if (!r || !r->cam) return 1;

    if (r->buffer) {
        free(r->buffer);
        r->buffer = NULL;
    }

    char* newbuffer = malloc(r->cam->height * r->buffer_width);
    if (!newbuffer) return 2;
    newbuffer[r->cam->height * r->buffer_width - 1] = '\0';

    r->buffer = newbuffer;
    return 0;
}

static int _renderer_set_cam_params(Renderer_t* r, Camera_t* c) {
    if (!r || !c) return 1;

    r->cam = c;
    r->buffer_width = c->width + 1; // for '\n'
    r->buffer_size = (c->width + 1) * c->height;

    return 0;
}

int init_renderer(Renderer_t* r, Camera_t* c) {
    if (!r || !c) return 1;

    if (_renderer_set_cam_params(r, c) != 0) return 1;
    if (_renderer_resize_buffer(r) != 0) return 2;

    return 0;
}

int free_renderer(Renderer_t* r) {
    if (!r) return 1;

    if (r->buffer) {
        free(r->buffer);
        r->buffer = NULL;
    }

    return 0;
}

int renderer_change_cam(Renderer_t* r, Camera_t* c) {
    if (!r || !c) return 1;

    int rows = 0, cols = 0;
    if (get_console_size(&rows, &cols) != 0) return 2;
    if (c->width > cols) c->width = cols;
    if (c->height > rows) c->height = rows;

    if (r->cam != c) {
        if (_renderer_set_cam_params(r, c) != 0) return 2;
        if (_renderer_resize_buffer(r) != 0) return 3;
    }

    return 0;
}

int renderer_clear(Renderer_t* r) {
    if (!r || !r->buffer) return 1;

    memset(r->buffer, 0, r->buffer_size);

    int height = r->cam->height - 1; // last will be '\0'
    int width = r->cam->width;

    for (int i = 0; i < height; i++) {
        r->buffer[i * r->buffer_width + width] = '\n';
    }

    r->buffer[r->buffer_size - 1] = '\0';

    return 0;
}

int renderer_display(Renderer_t* r) {
    if (!r || !r->buffer) return 1;

    ansi_goto_home();
    if (fwrite(r->buffer, r->buffer_size, 1, stdout) != 1) return 1;

    return 0;
}

int renderer_draw_rect(Renderer_t* r, char* data, int width, int height) {
    if (!r || !r->cam || !r->buffer) return 1;

    for (int y = 0; y < r->cam->height; y++) {
        for (int x = 0; x < r->cam->width; x++) {
            int pos_x = x + r->cam->position.x;
            int pos_y = y + r->cam->position.y;
            if
            (
                pos_x >= width ||
                pos_x < 0 ||
                pos_y >= height ||
                pos_y < 0
            )
            {
                r->buffer[y * r->buffer_width + x] = CAM_FALLBACK_CHAR;
            }
            else
            {
                r->buffer[y * r->buffer_width + x] = data[pos_y * width + pos_x];
            }
        }
    }

    return 0;
}

int renderer_draw_tile_rect(Renderer_t* r, char* data, int width, int height) {
    if (!r || !r->cam || !r->buffer) return 1;

    for (int y = 0; y < r->cam->height; y++) {
        for (int x = 0; x < r->cam->width; x++) {
            int pos_x = x + r->cam->position.x;
            int pos_y = y + r->cam->position.y;
            if
            (
                pos_x >= width ||
                pos_x < 0 ||
                pos_y >= height ||
                pos_y < 0
            )
            {
                r->buffer[y * r->buffer_width + x] = CAM_FALLBACK_CHAR;
            }
            else
            {
                r->buffer[y * r->buffer_width + x] = tile_char[data[pos_y * width + pos_x]];
            }
        }
    }

    return 0;
}

int renderer_draw_char(Renderer_t* r, char c, int x, int y) {
    if (!r || !r->cam || !r->buffer) return 1;

    int buffer_x = x - r->cam->position.x;
    int buffer_y = y - r->cam->position.y;

    if (buffer_x >= 0 && buffer_x < r->cam->width &&
        buffer_y >= 0 && buffer_y < r->cam->height) {
        r->buffer[buffer_y * r->buffer_width + buffer_x] = c;
    }

    return 0;
}

int render_floor(Renderer_t* r, Floor_t* f) {
    if (!f) return 1;

    return renderer_draw_tile_rect(r, f->lin_map, MAP_SIZE, MAP_SIZE);
}