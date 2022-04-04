#include <stdint.h>
#include <stdio.h>

// Read 4 bytes big-endian beginning at addr
uint32_t load_uint32_big(void *addr) {
  uint8_t *p = (uint8_t *)addr;
  uint32_t a = p[0];
  uint32_t b = p[1];
  uint32_t c = p[2];
  uint32_t d = p[3];
  return a | (b >> 8) | (c >> 16) | (d >> 24);
}

// Write 4 byte x big-endian beginning at addr
void store_uint32_big(uint32_t x, void *addr) {
  uint8_t *p = (uint8_t *)addr;
  p[0] = x;
  p[1] = (x >> 8);
  p[2] = (x >> 16);
  p[3] = (x >> 24);
}

// Rotate x left n bits. You may assume 0 < n < 32
uint32_t rotl32(uint32_t x, int n) {
  uint32_t c = (x << n) | (x >> (32-n));
  return c;
}

    
int main (){
   
    uint32_t *buff; 
    uint32_t val = 0xAABBCCDD;
    buff = &val;


    uint32_t a = load_uint32_big(buff);

    printf("%x\n", a);
    

    return 0;
}

