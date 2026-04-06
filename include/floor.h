#pragma once

/**
 * WORK IN PROGRESS
 * 
 *      needs total overhaul
 *      - remove bounded map
 *      - chunk based data
 *      - absolute position as svec2
 *      - make vector<monster> for monster storage
 *          > minimal ~ 16 monster, double on grow, half on shrink
 *          > in bin file store it as [monster_count][<monster_count>*monster]
 */

#include "sys/data_structs/monster_vector.h"

#define MAP_SIZE 64

typedef struct Floor_t {
   // static data
   union {
       char lin_map[MAP_SIZE * MAP_SIZE];
       char map[MAP_SIZE][MAP_SIZE];
   };

   // dynamic data
   Mvec_t monsters;
   
} Floor_t;

#define FLOOR_STATIC_DATA_SIZE (sizeof(((Floor_t *)0)->map))

typedef union Sizes_of_floor_dynamic_data {
    struct {
        int monsters;
    };
    int fields[1];
} Floor_counts_t;

// int main() {sizeof(Floor_t)}

int floor_init_test_grid(Floor_t* f);
int floor_printable_garbage_data(Floor_t* f);

#define SAVEFILE "appdata/savefile_floor"
int save_floor(Floor_t* floor, int index);
int load_floor(Floor_t* floor, int index);

#pragma region visibility type

enum {
    VISIBILITY_TOTAL_DARKNESS,
    VISIBILITY_DEFAULT = VISIBILITY_TOTAL_DARKNESS,
    VISIBILITY_VISIBLE,
    VISIBILITY_SEEN
};

typedef char Visibility_type_t;

#pragma endregion