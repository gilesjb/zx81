#if 0
zcc +zx81 -create-app -o build/bouncy.bin "$0"
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

// 32 chars in first line for 25 newlines + 5 right shifts
unsigned char buffer[] = {
    L,
    G,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,L,
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

    char x = 0, y = 0, dy = 0;

    *dfile = buffer;

    for (;;) {
        while (current_frame == *frames) {
            intrinsic_halt();
        }
        current_frame = *frames;

        if (y > 0 && dy != 0) buffer[26 - y] = (dy == 1) ? L : G;

        y += dy;

        if (y == 0) {
            dy = 1;
        } else if (y == 25) {
            char xp = x;
            x = rand() & 7;
            for (int b = 33; b < (33 + 6 * 33); b += 33) {
                buffer[b - xp] = W;
                buffer[b - x] = L;
            }
            dy = -1;
        }
    }
}
