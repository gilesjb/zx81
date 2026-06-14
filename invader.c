#if 0
zcc +zx81 -create-app -o build/invader.bin "$0"
exit
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
#define TOP L,L,L,L,L,L,L,L,L,L,L,L,L,L,L
#define BOTTOM L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L,L
#define LEFT W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,L

unsigned char buffer[] = {
    TOP,
    LEFT,0x00,0x86,0x00,0x87,0x01,0x00, 0x00,0x02,0x04,0x00,0x06,0x00,
    LEFT,0x87,0x07,0x80,0x07,0x82,0x00, 0x00,0x81,0x84,0x80,0x84,0x04,
    LEFT,0x07,0x80,0x80,0x80,0x07,0x05, 0x85,0x84,0x80,0x80,0x80,0x84,
    LEFT,0x01,0x86,0x04,0x83,0x01,0x01, 0x02,0x02,0x83,0x87,0x06,0x02,
    LEFT,W,W,W,W,W,W,W,W,W,W,W,W,
    LEFT,0x00,0x86,0x00,0x87,0x01,0x00, 0x00,0x02,0x04,0x00,0x06,0x00,
    LEFT,0x87,0x07,0x80,0x07,0x82,0x00, 0x00,0x81,0x84,0x80,0x84,0x04,
    LEFT,0x07,0x80,0x80,0x80,0x07,0x05, 0x85,0x84,0x80,0x80,0x80,0x84,
    LEFT,0x01,0x86,0x04,0x83,0x01,0x01, 0x02,0x02,0x83,0x87,0x06,0x02,
    BOTTOM
};

unsigned char *invader = buffer + 15;

// at 0,0 buffer offset for L is 37
#define STEP *pxx = L; *px = W; px += 33; pxx += 33


int main() {
    unsigned char **dfile = (unsigned char **)0x400C;
    unsigned char *bfile = *dfile;

    unsigned char *frames = (unsigned char *) 0x4034;
    unsigned char current_frame = *frames;

    char x = 20, y = 14, dx = -1; // 0,0 is bottom right

    for (;;) {
        while (current_frame == *frames) {
            intrinsic_halt();
        }
        current_frame = *frames;
        *dfile = buffer + y;

        unsigned char *px = invader + x, *pxx = px + dx;
        STEP; STEP; STEP; STEP; STEP; STEP; STEP; STEP; STEP;

        // finished display buffer changes
        x += dx;

        if (x == 0) {
            dx = 1;
            y--;
        } else if (x == 20) {
            dx = -1;
            y--;
        }
        if (y < 0) y = 14;

        if (in_Inkey()) {
            *dfile = bfile;
            return buffer;
        }
    }
}
