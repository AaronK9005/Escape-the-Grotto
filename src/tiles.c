#include "tiles.h"

Tile_def_t tile_defs[TILE_COUNT] = {
    //#define T(NAME, walk, opaque, interact) [TILE_##NAME] = {}
    [TILE_FLOOR] =      { '.', TILE_WALKABLE},
    [TILE_WALL] =       { .state = { '|', '-'}, TILE_OPAQUE},
    [TILE_DOOR_OPEN] =  { '/', TILE_WALKABLE | TILE_INTERACTABLE },
    [TILE_DOOR_CLOSED]= { '\\', TILE_OPAQUE | TILE_INTERACTABLE },
    [TILE_RUBBLE]       = { '*', TILE_WALKABLE }
};

// const char tile_walkable[TILE_COUNT] = {
    // #define T(NAME, walk, opaque, interact) [TILE_##NAME] = walk,
    // #include "tiles.def"
    // #undef T
// };
// 
// const char tile_opaque[TILE_COUNT];
// const char tile_interactable[TILE_COUNT];
