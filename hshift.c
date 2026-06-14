#if 0
zcc +zx81 -create-app -o build/hshift.bin "$0"; exit
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
// #define WL W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,L
#define WL G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,G,W,L

unsigned char buffer[] = {
    W,L,
    WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,
    WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,
    WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL,WL
};

void main() {
    unsigned char **dfile = (unsigned char **)0x400C;

    unsigned char *frames = (unsigned char *) 0x4034;
    unsigned char current_frame = *frames;

    for (;;) {
        unsigned char *buf = buffer; // include white char before nl
    
        for (unsigned int o = 0; o < 33 * 23; o++) {
            while (current_frame == *frames) {
                intrinsic_halt();
            }
            current_frame = *frames;

            buf++;
            unsigned char *p = buf;
            for (unsigned char row = 0; row < 49; row++) {
                *p = W;
                *(p + 1) = L;
                p += 33;
            }

            *(buf + 33 + (33 * (rand() & 15))) = B;
            *buf = W;
            *(buf + 1) = L;
            *(buf + 24 * 33) = L;
            *dfile = buf + (1 + 24 * 33);
        }
    }
}
