#include <stdint.h>
#include <stdio.h>

// Divisionless x mod 2^a - 1. Result must be equal to x mod 2^a - 1
uint64_t divlessmod_final(uint64_t x, int a) {
    int64_t y = ((uint64_t)1 << a) - 1;
    uint64_t xlo = x & y;               // mask grabs low a bits
    uint64_t xhi = x >> a;              // all bits beyond low a
    x = xhi + xlo;                      
    int64_t t = (y - 1) - x;
    t = t >> 63;                        // x >= y -> t = 1 | x < y -> t = 0
    while (t){                          // reduce congruent result by prime until x < y
        x -= y;                         
        t = (y - 1) - x;
        t = t >> 63;
    }
    return x;
}


int main(){
    int check = 500 % 7;
    
    printf("%d\n", (int)divlessmod_final(500, 3));
    printf("%d\n", check);
    return 0;
}
