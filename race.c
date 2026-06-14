#if 0
zcc +zx81 -create-app -o build/race.bin "$0"; exit
#endif

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
    0x76
};

unsigned char topline[] = {
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,

    0x08, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, 0x08,

    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
};

int main() {
    unsigned char **dfile = (unsigned char **) 0x400C;
    unsigned char *screen = *dfile; 

    unsigned char *frames = (unsigned char *) 0x4034;
    unsigned char current_frame = *frames;

    unsigned char x = 30;
    unsigned int key = 0;

    int shift = 10;
    int direction = 1;

    for (;;) {
        for (int r = LINES; 0 <= r; --r) {
            unsigned char *row = buffer + 33 * r;

            int r = rand();
            if (shift == 0) {
                direction = 1;
            } else if (shift == 20) {
                direction = -1;
            } else if ((r & 0x1f00) == 0x1f00) {
                direction = -direction;
            }

            if ((r & 0x0003) == 0x0003) {
                shift += direction;
            }
            // create new row
            memcpy(row + 1, topline + shift, 32);

            while (current_frame == *frames) {
                intrinsic_halt();
            }
            *dfile = row;
            // *(row + 1) = 0x1c + ((current_frame - *frames) & 0x0f); // 1 means 50fps
            current_frame = *frames;

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
                return 69;
            } else if (key == 'Q' && x > 0) {
                --x;
            } else if (key == 'P' && x < 61) {
                ++x;
            }
        }
    }
}
