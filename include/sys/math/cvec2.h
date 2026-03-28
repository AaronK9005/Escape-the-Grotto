#pragma once

typedef struct char_vec2 {
    char x, y;
} cvec2;

#define CVEC2_NEW(x, y) (cvec2){x, y}