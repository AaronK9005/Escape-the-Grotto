#pragma once

typedef struct short_vec2 {
    short x, y;
} svec2;

#define SVEC2_NEW(x, y) (svec2){x, y}