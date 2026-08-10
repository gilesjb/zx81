#if 0
zcc +zx81 -create-app -o build/flips.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
// #include <stdio.h>
#include <input.h>
#include <intrinsic.h>

#define W 0x00
#define L 0x76
#define G 0x08
#define B 0x80

unsigned char buffer[] = {
    L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,
    B,B,B,B,B,B,W,B,W,W,W,W,B,W,W,B,B,B,B,W,W,W,W,B,W,W,W,W,W,W,W,W,L,
    W,W,W,W,B,W,W,W,B,W,W,B,W,W,B,W,W,W,W,B,W,W,B,B,W,W,W,W,W,W,W,W,L,
    W,W,W,B,W,W,W,W,W,B,B,W,W,W,W,B,B,B,B,W,W,B,W,B,W,W,W,W,W,W,W,W,L,
    W,W,B,W,W,W,W,W,W,B,B,W,W,W,B,W,W,W,W,B,W,W,W,B,W,W,W,W,W,W,W,W,L,
    W,B,W,W,W,W,W,W,B,W,W,B,W,W,B,W,W,W,W,B,W,W,W,B,W,W,W,W,W,W,W,W,L,
    B,B,B,B,B,B,W,B,W,W,W,W,B,W,W,B,B,B,B,W,W,B,B,B,B,B,L,
    L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L
};

unsigned char **dfile = (unsigned char **)0x400C;
unsigned char *frames = (unsigned char *) 0x4034;

void flip(unsigned char *frame) {
    unsigned char current_frame = *frames;
    do {
        intrinsic_halt();
    } while (current_frame == *frames);
    *dfile = frame;
}

void main() {

    for (;;) {
        for (unsigned int y = 1; y < 46; y++) {
            flip(buffer + (y >> 1));
            flip(buffer + 24 - (y >> 1));
        }
    }
}
