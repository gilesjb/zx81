#if 0
zcc +zx81 -create-app -o build/driver.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
#include <input.h>
#include <intrinsic.h>

#define W 0x00
#define WW W,W
#define WWW W,W,W
#define L 0x76
#define B 0x80
#define TOP L,L,L,L,L,L,L,L,L,L,L,L,L,L,L

#define G 0x08
#define U 0x0a
#define D 0x09

unsigned char buffer1[] = {
    TOP,

    WWW, WWW, WWW, WWW, WWW, D,D,L,
    WWW, WWW, WWW, WWW, WWW, U,U,L,

    WWW, WWW, WWW, WWW, G, WWW, WWW, G,L,

    WWW, WWW, WWW, D,D, WW, WWW, WWW, WW, D,D,L,
    WWW, WWW, WWW, U,U, WW, WWW, WWW, WW, U,U,L,

    WWW, WWW, G,G, WW, WWW, WWW, WWW, WWW, WW, G,G,L,

    WWW, D,D,D, WWW, WWW, WWW, WWW, WWW, WWW, WW, D,D,D,L,
    WWW, U,U,U, WWW, WWW, WWW, WWW, WWW, WWW, WW, U,U,U,L,
    
    G,G,G, WWW,WWW,WWW,WWW,WWW,WWW,WWW,WWW, WW, G,G,G,L,

    L,L,L,L,L,L
};

unsigned char buffer2[] = {
    TOP,

    L,
    WW, WWW, WWW, WWW, WWW, G, WW, G,L,

    WW, WWW, WWW, WWW, D, WWW, WWW, WW, D,L,
    WW, WWW, WWW, WWW, U, WWW, WWW, WW, U,L,
    
    WW, WWW, WWW, G,G, WWW,WWW,WWW,WWW, G,G,L,

    WW, WWW, D,D, WWW, WWW, WWW, WWW, WWW, WWW, D,D,L,
    WW, WWW, U,U, WWW, WWW, WWW, WWW, WWW, WWW, U,U,L,

    WW, G,G,G, WWW, WWW, WWW, WWW, WWW, WWW, WWW, W, G,G,G,L,

    D,D, WWW, WWW, WWW, WWW, WWW, WWW, WWW, WWW, WWW, W, D,D,L,

    L,L,L,L,L,L
};

unsigned char buffer3[] = {
    TOP,

    L,
    WWW, WWW, WWW, WWW, W, D, WW, WW, D,L,
    WWW, WWW, WWW, WWW, W, U, WW, WW, U,L,

    WWW, WWW, WWW, W, G,G, WWW, WWW, WW, G,G,L,

    WWW, WWW, W, D,D, WWW, WWW, WWW, WWW, WW, D,D,L,
    WWW, WWW, W, U,U, WWW, WWW, WWW, WWW, WW, U,U,L,

    WWW, W, G,G,G, WWW,WWW,WWW,WWW,WWW,WWW, G,G,G,L,

    W, D,D,D, WWW,WWW,WWW,WWW,WWW,WWW,WWW,WWW,D,D,D,L,
    W, U,U,U, WWW,WWW,WWW,WWW,WWW,WWW,WWW,WWW,U,U,U,L,

    L,L,L,L,L,L
};

unsigned char **dfile = (unsigned char **)0x400C;
unsigned char *frames = (unsigned char *) 0x4034;

void flip(unsigned char *frame) {
    for (unsigned char i = 0; i < 2 ; i++) {
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
