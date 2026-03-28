#pragma once

#include "floor.h"

#include "sys/math/vec2.h"

#define DEFAULT_VISIBILITY 20

typedef struct Camera_t {
    svec2 position;
    unsigned char height, width;
} Camera_t;

#define CAM_FALLBACK_CHAR ' '

/**
 * Right now just simple buffer based on camera, which can render section of data from test_data buffer
 */

typedef struct Renderer_t {
    char* buffer;
    Camera_t* cam;
    // for convenience
    int buffer_width; // r->cam->width + 1
    int buffer_size;
} Renderer_t;

// int main() {sizeof(Renderer_t);}

#define EMPTY_RENDERER {NULL, NULL, 0}

int init_renderer(Renderer_t* r, Camera_t* c);

int free_renderer(Renderer_t* r);

int renderer_change_cam(Renderer_t* r, Camera_t* c);

int renderer_clear(Renderer_t* r);

int renderer_display(Renderer_t* r);

int renderer_draw_rect(Renderer_t* r, char* data, int width, int height);

int renderer_draw_char(Renderer_t* r, char c, int x, int y);

static inline int renderer_draw_char_pos(Renderer_t* r, char c, svec2 pos) {
    return renderer_draw_char(r, c, pos.x, pos.y);
}

int render_floor(Renderer_t* r, Floor_t* f);

int get_console_size(int *rows, int *cols);