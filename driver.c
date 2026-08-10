#if 0
zcc +zx81 -create-app -o build/driver.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
// #include <stdio.h>
#include <input.h>
#include <intrinsic.h>

#define W 0x00
#define L 0x76
#define B 0x80

#define G 0x08
#define U 0x0a
#define D 0x09

unsigned char buffer1[] = {
    L,L,L,L,L,L,L,L,L,L,

    D,L,
    U,L,
    G,L,

    D,D,L,
    U,U,L,
    G,G,L,

    D,D,D,L,
    U,U,U,L,
    G,G,G,L,

    L,L,L,L,L,L
};

unsigned char buffer2[] = {
    L,L,L,L,L,L,L,L,L,L,

    U,L,
    G,L,

    D,L,
    U,L,
    
    G,G,L,

    D,D,L,
    U,U,L,

    G,G,G,L,

    D,D,D,L,

    L,L,L,L,L,L
};

unsigned char buffer3[] = {
    L,L,L,L,L,L,L,L,L,L,

    G,L,
    D,L,
    U,L,

    G,G,L,
    D,D,L,
    U,U,L,

    G,G,G,L,
    D,D,D,L,
    U,U,U,L,

    L,L,L,L,L,L
};

unsigned char **dfile = (unsigned char **)0x400C;
unsigned char *frames = (unsigned char *) 0x4034;

void flip(unsigned char *frame) {
    for (unsigned char i = 0; i < 10; i++) {
        unsigned char current_frame = *frames;
        do {
            intrinsic_halt();
        } while (current_frame == *frames);
    }
    *dfile = frame;
}

void main() {

    for (;;) {
        for (unsigned int y = 1; y < 46; y++) {
            flip(buffer1);
            flip(buffer2);
            flip(buffer3);
        }
    }
}
