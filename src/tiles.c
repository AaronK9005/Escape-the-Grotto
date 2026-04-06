#include "tiles.h"

const char tile_char[TILE_COUNT] = {
    [TILE_FLOOR] =          ' ',
    [TILE_WALL] =           '#',
    [TILE_DOOR_OPEN] =      '/',
    [TILE_DOOR_CLOSED] =    '\\',
    [TILE_RUBBLE] =         '*'
};

const int tile_flags[TILE_COUNT] = {
    [TILE_FLOOR] =           TILE_WALKABLE,
    [TILE_WALL] =            TILE_OPAQUE,
    [TILE_DOOR_OPEN] =       TILE_WALKABLE | TILE_INTERACTABLE,
    [TILE_DOOR_CLOSED] =     TILE_OPAQUE | TILE_INTERACTABLE,
    [TILE_RUBBLE] =          0
};

Tile_def_t get_tile_def(Tile_id_t id) {
    return (Tile_def_t){ tile_char[id], tile_flags[id] };
}