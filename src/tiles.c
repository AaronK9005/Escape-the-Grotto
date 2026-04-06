#include "tiles.h"

Tile_def_t tile_defs[TILE_COUNT] = {
    [TILE_FLOOR] =      { ' ', TILE_WALKABLE},
    [TILE_WALL] =       { .state = { '|', '-'}, TILE_OPAQUE},
    [TILE_DOOR_OPEN] =  { '/', TILE_WALKABLE | TILE_INTERACTABLE },
    [TILE_DOOR_CLOSED]= { '\\', TILE_OPAQUE | TILE_INTERACTABLE }
};
