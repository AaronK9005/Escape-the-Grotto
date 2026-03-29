#include "floor.h"
#include "monster.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int snprintf(char*, size_t, const char*, ...);

 int floor_init_test_grid(Floor_t* f) {
    if (!f) return 1;

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (i == MAP_SIZE - 1 || j == MAP_SIZE - 1 || (i % 8 == 0) || (j % 8 == 0) ) {
                f->map[i][j] = '*';
            }
            else f->map[i][j] = '.';
        }
    }

    return 0;
 }

 int floor_printable_garbage_data(Floor_t* f) {
    if (!f) return 1;

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (!isprint((unsigned char)f->map[i][j])) f->map[i][j] = '~';
        }
    }
 }

 int save_floor(Floor_t* floor, int index) {
    if (!floor) return 1;

    char filename[64];
    snprintf(filename, sizeof(filename), "%s%d", SAVEFILE, index);

    FILE* f = fopen(filename, "wb");
    if (!f) return 2;

    Floor_counts_t counts = {floor->monsters.count};

    // static
    if (fwrite(floor, FLOOR_STATIC_DATA_SIZE + sizeof(counts), 1, f) != 1) {
        fclose(f);
        return 3;
    }

    // dynamic
    if (fwrite(&floor->monsters, sizeof(Monster_t), counts.monsters, f) != counts.monsters) {
        fclose(f);
        return 4;
    }

    return 0;
 }

 int load_floor(Floor_t* floor, int index);

 