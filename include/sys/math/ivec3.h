#pragma once

typedef struct {
    int x, y, z;
} ivec3;

#define IVEC3_NEW(x, y, z) (ivec3){x, y, z}