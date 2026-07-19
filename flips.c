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
void main() {
    unsigned char **dfile = (unsigned char **)0x400C;

    unsigned char *frames = (unsigned char *) 0x4034;
    unsigned char current_frame = *frames;

    for (;;) {
        for (unsigned int y = 1; y < 23; y++) {
            while (current_frame == *frames) {
                intrinsic_halt();
            }
            *dfile = buffer + y;
            current_frame = *frames;
    
            while (current_frame == *frames) {
                intrinsic_halt();
            }
            *dfile = buffer + 24 - y;
            current_frame = *frames;
        }
    }
}
