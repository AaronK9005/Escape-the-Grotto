#pragma once

#define TILE_WALKABLE           (1 << 0)
#define TILE_OPAQUE             (1 << 1)
#define TILE_INTERACTABLE       (1 << 2)

typedef union {
    char state[4];
    char glyph;
} Tile_states_t;

typedef struct Tile_definition_t {
    Tile_states_t;
    int flags;
} Tile_def_t;

typedef enum tile_identificator_t {
    #define T(NAME,      walk, opaque, interact) TILE_##NAME,
    #include "tiles.def"
    #undef T

    TILE_COUNT
} Tile_id_t;

typedef char Tile_type_t; // for representation in floor

extern const char   tile_char[TILE_COUNT];
extern const int    tile_flags[TILE_COUNT];

Tile_def_t get_tile_def(Tile_id_t id);
