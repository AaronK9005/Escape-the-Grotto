#include "floor.h"
#include "monster.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int snprintf(char*, size_t, const char*, ...);
int rand();

 int floor_init_test_grid(Floor_t* f) {
    if (!f) return 1;

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (i == MAP_SIZE - 1 || j == MAP_SIZE - 1 || (i % 8 == 0) || (j % 8 == 0) ) {
                f->map[i][j] = TILE_RUBBLE;
            }
            else f->map[i][j] = TILE_FLOOR;
        }
    }

    return 0;
 }

int floor_init_random_grid(Floor_t* f) {
    if (!f) return 1;

    memset(f->lin_map, TILE_WALL, MAP_SIZE * MAP_SIZE);

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (rand() % 100 < 70) f->map[y][x] = TILE_FLOOR;
            /*
            int chance = (rand() % 100);

            if (chance < 70)
            {
                f->map[y][x] = TILE_FLOOR;
            }
            else if (chance < 80)
            {
                f->map[y][x] = TILE_WALL;
            }
            else if (chance < 90)
            {
                f->map[y][x] = TILE_RUBBLE;
            }
            else if (chance < 95)
            {
                f->map[y][x] = TILE_DOOR_CLOSED;
            }
            else
            {
                f->map[y][x] = TILE_DOOR_OPEN;
            }
            */
        }
    }

    return 0;
}

int floor_generate_grid_v1(Floor_t* f) {
    if (!f) return 1;

    memset(f->map, TILE_WALL, MAP_SIZE * MAP_SIZE);
    
    const int min_rooms = 30;
    const int max_rooms = 50;

    const int min_spread = 10;
    const int max_spread = 50;

    int num_rooms = (rand() % (max_rooms - min_rooms + 1)) + min_rooms;

    // few random walkers rooms
    while (num_rooms --> 0) 
    {
        int room_x = rand() % MAP_SIZE;
        int room_y = rand() % MAP_SIZE;
        int room_spread = (rand() % (max_spread - min_spread + 1)) + min_spread;

        int x = room_x;
        int y = room_y;

        while (room_spread > 0)
        {
            int dir = rand() % 4;
            
            switch (dir)
            {
            case 0: // up
                y--;
                break;
            case 1: // down
                y++;
                break;
            case 2: // left
                x--;
                break;
            case 3: // right
                x++;
                break;
            }

            if (x < 0) x = 0;
            else if (x >= MAP_SIZE) x = MAP_SIZE - 1;

            if (y < 0) y = 0;
            else if (y >= MAP_SIZE) y = MAP_SIZE - 1;

            if (f->map[y][x] == TILE_WALL) {
                f->map[y][x] = TILE_FLOOR;
                room_spread--;
            }
        }
        
    }

    int x = 1;
    int y = 1;
    int base_spread = max_spread;

    // big room by random walker at { 1, 1 }
    while (base_spread > 0)
    {
        int dir = rand() % 4;
                    
        switch (dir)
        {
        case 0: // up
            y--;
            break;
        case 1: // down
            y++;
            break;
        case 2: // left
            x--;
            break;
        case 3: // right
            x++;
            break;
        }

        if (x < 0) x = 0;
        else if (x >= MAP_SIZE) x = MAP_SIZE - 1;

        if (y < 0) y = 0;
        else if (y >= MAP_SIZE) y = MAP_SIZE - 1;

        if (f->map[y][x] == TILE_WALL) {
            f->map[y][x] = TILE_FLOOR;
            base_spread--;
        }
    }
    
    x = 1;
    y = 1;

    // squiggly line from "spawn" through the whole map
    const int end_x = 60;
    const int end_y = 60;

    while (x != end_x || y != end_y)
    {
        switch (rand() % 4)
        {
        case 0: // favor x++
            if (x - y >= -10) y++;
            else x++;
            break;
        case 1: // favor y++
            if (x - y <= 10) x++;
            else y++;
            break;
        case 2: // thicken the line
            f->map[y - 1][x - 1] = TILE_FLOOR;
            f->map[y + 1][x + 1] = TILE_FLOOR;
            continue;
        case 3: // branch
            int branch_chance = rand() % 100;
            const int step = 30;
            int iter = 0;
            if (branch_chance < step * ++iter) y--;
            else if (branch_chance < step * ++iter) x++;
            else {
                const int min_len = 5;
                const int max_len = 20;
                int branch_len = (rand() % (max_len - min_len + 1)) + min_len;
                int branch_dir = rand() % 6;
                int branch_x = x;
                int branch_y = y;

                while (branch_len-- > 0)
                {
                    int chance = rand() % 100;
                    switch (branch_dir)
                    {
                    case 0: // up
                        if (chance < 10) branch_x++;
                        else if (chance < 20) branch_x--;
                        else branch_y++;
                        break;
                    case 1: // y =.= x, 1st quadrant
                        if (branch_x - branch_y < 2) branch_x++;
                        else branch_y++;
                        break;
                    case 2: // y =.= konst, x++
                        if (chance < 10) branch_y++;
                        else if (chance < 20) branch_y--;
                        else branch_x++;
                        break;
                    case 3: // y =.= konst, x--;
                        if (chance < 10) branch_y++;
                        else if (chance < 20) branch_y--;
                        else branch_x--;
                        break;
                    case 4: // y =.= x, 3rd quadrant
                        if (branch_x - branch_y < 2) branch_x--;
                        else branch_y--;
                        break;
                    case 5: // down
                        if (chance < 10) branch_x++;
                        else if (chance < 20) branch_x--;
                        else branch_y--;
                        break;
                    }

                    if (branch_x < 0) branch_x = 0;
                    else if (branch_x >= MAP_SIZE) branch_x = MAP_SIZE - 1;

                    if (branch_y < 0) branch_y = 0;
                    else if (branch_y >= MAP_SIZE) branch_y = MAP_SIZE - 1;

                    f->map[branch_y][branch_x] = TILE_FLOOR;
                }
                
            }
        }

        if (x < 1) x = 1;
        else if (x > end_x) x = end_x;

        if (y < 1) y = 1;
        else if (y > end_y) y = end_y;

        if (f->map[y][x] == TILE_WALL) {
            f->map[y][x] = TILE_FLOOR;
        }
    }
    
    return 0;
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

 