#pragma once

static inline int iclamp (int data, int min, int max) {
    if (min > max) {
        int tmp = min;
        min = max;
        max = tmp;
    }
    return (data < min) ? min : (data > max) ? max : data;
}

static inline int imin(int x, int y) {
    return x < y ? x : y;
}

static inline int imax(int x, int y) {
    return x > y ? x : y;
}

