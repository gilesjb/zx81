#if 0
zcc +zx81 -create-app -o build/vshift.bin "$0"; exit
#endif

#include <intrinsic.h>
#include <input.h>
#include <string.h>
#include <stdlib.h>

#define WT 0x00
#define WTS 0x76, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT
#define LINES 24

#define ALIEN 1

unsigned char buffer[] = { 
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,

    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,

    WTS, WTS,
    0x76
};

unsigned char starline[] = {
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    // 0x1b, // full stop
    0x08, // grey
    // 0x17, // star
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT
};

int main() {
    unsigned char **dfile = (unsigned char **) 0x400C;
    unsigned char *screen = *dfile; 

    unsigned char *frames = (unsigned char *) 0x4034;
    unsigned char current_frame = *frames;

    unsigned char x = 30;

    unsigned int exit_key = in_LookupKey(' ');
    unsigned int left_key = in_LookupKey('Q');
    unsigned int right_key = in_LookupKey('W');

    unsigned char *alien;

    for (;;) {
        for (int r = LINES; 0 <= r; --r) {
            unsigned char *row = buffer + 33 * r;
            int rnd = rand();

            // create new row
            memcpy(row + 1, starline + (rnd & 31), 32);
            // memcpy(row + 1, starline + 20, 32);

            while (current_frame == *frames) {
                intrinsic_halt();
            }
            *dfile = row;
            // *(row + 1) = 0x1c + current_frame - *frames; // 1 means 50fps
            // *(row + 1) = 0x1c + r;
            current_frame = *frames;

            memcpy(row + 33 * (LINES + 1), row, 33);

            if (r == LINES - ALIEN) {
                alien = row + 10 + 33;
                *(alien + 0) = 0x06;
                *(alien + 1) = 0x8b;
                *(alien + 2) = 0x86;
            } else if (r < LINES - ALIEN) {
                alien -= 33;;
                *(alien + 0) = 0x06;
                *(alien + 1) = 0x8b;
                *(alien + 2) = 0x86;
                *(alien + 33) = *(alien + 33 + LINES * 33);
                *(alien + 34) = *(alien + 34 + LINES * 33);
                *(alien + 35) = *(alien + 35 + LINES * 33);
            }

            unsigned char *p = row + 33 * (LINES - 1) + 1 + (x >> 1);
            if (x & 0x01) {
                *p = 0x87;
                *(p + 1) = 0x86;
            } else {
                *p = 0x06;
                *(p + 1) = 0x04;
            }

            if (in_KeyPressed(exit_key)) {
                *dfile = screen;
                return 69;
            } 
            if (in_KeyPressed(left_key) && x > 0) {
                --x;
            }
            if (in_KeyPressed(right_key) && x < 61) {
                ++x;
            }
        }
    }
}
