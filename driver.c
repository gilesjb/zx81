#if 0
zcc +zx81 -create-app -o build/driver.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
#include <input.h>
#include <intrinsic.h>

#define uchar unsigned char

#define _ X(0x00) // space
#define L X(0x76) // newline
#define B X(0x80) // black square
#define G X(0x08) // grey square
#define U X(0x0a) // upper half grey
#define D X(0x09) // bottom half grey
#define A X(0x26) // 'A'

#define SCREEN1 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ _ _ _ _ _ _ G _ _ _ _ _ _ G \
    L _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ U U _ _  _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ D D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ _ _ U U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U U \
    L G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G

#define SCREEN2 \
    L \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ G _ _ G \
    L _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ _ _ _ _ U \
    L _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D

#define SCREEN3 \
    L \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L _ D D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ U U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U U  

#define TOP L L L L L L L L L L L L L L L // Initial newline + 14 lines
#define TAIL L

#define X(c) c,
uchar buffer1[] = { TOP SCREEN1 TAIL 0xff };
uchar buffer2[] = { TOP SCREEN2 TAIL 0xff };
uchar buffer3[] = { TOP SCREEN3 TAIL 0xff };

#undef X
#define X(c) c

uchar *buffers[] = {buffer1, buffer2, buffer3};
uchar *lines[3][24];

void init_offsets() {
    for (uchar buf = 0; buf < 3; buf++) {
        uchar *lchar = buffers[buf];
        for (uchar line = 0; line < 24; line++) {
            while (*lchar != L) lchar++;
            lines[buf][line] = lchar++;
        }
    }
}

uchar **DFILE = (uchar **)0x400C;
uchar *FRAME = (uchar *) 0x4034;

void flip(uchar *frame) {
    for (uchar i = 0; i < 2 ; i++) {
        uchar current_frame = *FRAME;
        do {
            intrinsic_halt();
        } while (current_frame == *FRAME);
    }
    *DFILE = frame;
}

void main() {
    init_offsets();

    unsigned int left_key = in_LookupKey('Q');
    unsigned int right_key = in_LookupKey('W');
    uchar carx = 16;
    char lasti = 2;

    for (;;) {

        for (char i = 0; i < 3; i++) {
            uchar lastx = carx;
            if (in_KeyPressed(left_key) && carx > 8) {
                --carx;
            }
            if (in_KeyPressed(right_key) && carx < 23) {
                ++carx;
            }
    
            uchar *car = lines[i][22] + carx;
            *car++ = D; *car++ = A; *car++ = D;
            car = lines[i][23] + carx;
            *car++ = B; *car++ = U; *car++ = B;

            flip(buffers[i]);

            car = lines[lasti][22] + lastx;
            *car++ = _; *car++ = _; *car++ = _;
            car = lines[lasti][23] + lastx;
            *car++ = _; *car++ = _; *car++ = _;

            lasti = i;
        }
    }
}
