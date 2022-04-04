#include <stdint.h>
#include <string.h>

// Sponge constants - R is subject to change
#define B 48     // Block length
#define R 16     // Rate: bytes of 'in' read per iteration
#define C (B-R)  // Capacity: part of state that 'in' doesn't touch

// A 48-byte public random permutation. Permutes the 48 bytes beginning at p
static void scramble(uint32_t *a, uint32_t *b, uint32_t *c) {
    uint32_t x = (*a << 24) | (*a >>  8);
    uint32_t y = (*b <<  9) | (*b >> 23);
    uint32_t z = *c;
    *a = z ^ y ^ ((x & y) << 3);
    *b = y ^ x ^ ((x | z) << 1);
    *c = x ^ (z << 1) ^ ((y & z) << 2);
}
static void perm(void *addr) {
    uint32_t t, *p = (uint32_t *)addr;
    for (int round = 0; round < 24; round++) {
        scramble(p+0, p+4, p+8);
        scramble(p+1, p+5, p+9);
        scramble(p+2, p+6, p+10);
        scramble(p+3, p+7, p+11);
        if (round % 4 == 0) { // ie, when round is 0, 4, 8, 12, 16, 20
            t = p[0]; p[0] = p[1]; p[1] = t;
            t = p[2]; p[2] = p[3]; p[3] = t;
            p[0] = p[0] ^ 0x9E377900 ^ round;
        }
        if (round % 4 == 2) { // ie, when round is 2, 6, 10, 14, 18, 22
            t = p[0]; p[0] = p[2]; p[2] = t;
            t = p[1]; p[1] = p[3]; p[3] = t;
        }
    }
}

// blk points at a B+1 byte buffer. The first B bytes are the evolving
//   sponge block. Use of the final byte is optional.

// sponge_reset initializes this blk for new hash
void sponge_reset(void *blk) {
    memset(blk, 0, B+1);
}

// sponge_update updates this blk with buffer pointed at by in
void sponge_update(void *blk, void *in, int inbytes) {
    uint8_t *blk8 = (uint8_t *)blk;
    uint8_t *p8 = (uint8_t *)in;
    
    /* Perm loop */
    while ((inbytes+blk8[B]) >= R) {
        for (int i = 0; (i+blk8[B]) < R; i++)
            blk8[i+blk8[B]] ^= p8[i];
        perm(blk);
        inbytes -= (R - blk8[B]);
        p8 += (R - blk8[B]);
        blk8[B] = 0;
    }
    
    /* Extra bits not permed */
    for (int i = 0; i < inbytes; i++)
        blk8[i+blk8[B]] ^= p8[i];  
    
    /* Buffering */
    if ((inbytes + blk8[B]) < R)    // If not enough bytes for perm
        blk8[B] += inbytes;         // Save numbytes written to last byte of blk
}


// sponge_final updates this blk with buffer pointed at by in,
//   then completes the hash and outputs the result to out.
void sponge_final(void *blk, void *in, int inbytes, void *out, int outbytes) {
    uint8_t *blk8 = (uint8_t *)blk;
    uint8_t *p8 = (uint8_t *)in;
    
    /* Perm loop */
    while ((inbytes+blk8[B]) >= R) {
        for (int i = 0; (i+blk8[B]) < R; i++)
            blk8[i+blk8[B]] ^= p8[i];
        perm(blk);
        inbytes -= (R - blk8[B]);
        p8 += (R - blk8[B]);
        blk8[B] = 0;
    }
    
    /* Extra bits not permed */
    for (int i = 0; i < inbytes; i++)
        blk8[i+blk8[B]] ^= p8[i];
    
    /* Padding */
    blk8[inbytes] ^= 0x80;
    blk8[R-1] ^= 0x01;
    
    /* Output */
    p8 = out;
    perm(blk);
    while (outbytes >= R){
        memcpy(p8, blk8, R);
        perm(blk);
        outbytes -= R;
        p8 += R;
    }
    memcpy(p8, blk8, outbytes);
}