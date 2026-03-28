#pragma once

typedef struct {
    float x, y;
} fvec2;

#define FVEC_NEW(x, y) (fvec2){x, y}