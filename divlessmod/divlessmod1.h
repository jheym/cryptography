#include <stdint.h>

// Divisionless x mod 2^a - 1. Result may be >= 2^a - 1
uint64_t divlessmod1(uint64_t x, int a) {
    uint64_t y = ((uint64_t)1 << a) - 1;
    return x % y;
}


