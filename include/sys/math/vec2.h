#pragma once

#include "cvec2.h"
#include "fvec2.h"
#include "ivec2.h"
#include "svec2.h"

#define IS_VEC2_ZERO(vec) ((vec).x == 0 && (vec).y == 0)
#define VEC2_EQUALS(v1, v2) ((v1).x == (v2).x && (v1).y == (v2).y)