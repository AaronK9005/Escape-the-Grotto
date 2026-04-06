#pragma once

#define TILE_WALKABLE           (1 << 0)
#define TILE_OPAQUE             (1 << 1)
#define TILE_INTERACTABLE       (1 << 2)

typedef struct Tile_definition_t {
    union
    {
        char glyph;
        char state[4];
    };
    int flags;
} Tile_def_t;

typedef enum tile_identificator_t {
    #define T(NAME,      walk, opaque, interact) TILE_##NAME,
    #include "tiles.def"
    #undef T

    TILE_COUNT
} Tile_id_t;

typedef unsigned char Tile_type_t;

extern /* const */ Tile_def_t tile_defs[TILE_COUNT];

// extern const char tile_walkable[TILE_COUNT];
// extern const char tile_opaque[TILE_COUNT];
// extern const char tile_interactable[TILE_COUNT];