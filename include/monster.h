#pragma once

// unused & never properly defined yet

#include "sys/util/my_stdint.h"

typedef enum {
    MONSTER_NONE = -1,

    #define M(NAME, string, SIZE, hp, defence, magic_defence) MONSTER_##NAME,
    #include "monster_list.def"
    #undef M

    MONSTER_COUNT,
} Monster_type_t;

typedef struct Monster_t {
    Monster_type_t  type;
    i16             hp,         maxhp;
    i8              base_def,   def;
    i8              base_mdef,  mdef;    
} Monster_t;

#define MONSTER_EMPTY (Monster_t){ -1, \
                                    0, 0, \
                                    0, 0, \
                                    0, 0 }

// int main() {sizeof(Monster_t);}
