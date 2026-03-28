#pragma once

#include "ivec3.h"

#define IS_VEC3_ZERO(vec) ((vec).x == 0 && (vec).y == 0 && (vec).z == 0)
#define VEC3_EQUALS(v1, v2) ((v1).x == (v2).x && (v1).y == (v2).y && (v1).z == (v2).z)