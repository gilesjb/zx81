
#include <intrinsic.h>
#include <input.h>
#include <string.h>
#include <stdlib.h>

#define WT 0x00
#define WTS 0x76, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT
#define LINES 24

unsigned char buffer[] = { 
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,
    WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS, WTS,
    WTS, WTS,
    WT
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

void main() {
    unsigned char **dfile = (unsigned char **) 0x400C;
    unsigned char *screen = *dfile; 

    unsigned char *frames = (unsigned char *) 0x4034;
    unsigned char current_frame = *frames;

    unsigned char x = 0;
    unsigned int key = 0;

    for (;;) {
        for (int r = LINES; 0 <= r; --r) {
            unsigned char *row = buffer + 33 * r;

            // create new row
            memcpy(row + 1, starline + (rand() & 31), 32);

            while (current_frame == *frames) {
                intrinsic_halt();
            }
            current_frame = *frames;
            *dfile = row;

            memcpy(row + 33 * (LINES + 1), row, 33);

            unsigned char *p = row + 33 * (LINES - 1) + 1 + (x >> 1);
            if (x & 0x01) {
                *p = 0x87;
                *(p + 1) = 0x86;
            } else {
                *p = 0x06;
                *(p + 1) = 0x04;
            }

            key = in_Inkey();
            if (key == ' ') {
                *dfile = screen;
                return;
            } else if (key == 'Q' && x > 0) {
                --x;
            } else if (key == 'P' && x < 61) {
                ++x;
            }
        }
    }
}
