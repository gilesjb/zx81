#if 0
zcc +zx81 -create-app -o build/driver.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
#include <input.h>
#include <intrinsic.h>

#define uchar unsigned char

// #define _ X(0x00) // space
#define _ X(0x16) // minus
#define L X(0x76) // newline
#define B X(0x80) // black square
#define G X(0x08) // grey square
#define U X(0x0a) // upper half grey
#define D X(0x09) // bottom half grey
#define A X(0x26) // 'A'

#define SCREEN0 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L _ _ D D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ _ U U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U U
// effective row 1/3

#define SCREEN1 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G _ _ _ _ _ _ G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ D D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ _ _ _ _ _ U U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U U \
    L G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G
// effective row 0/3

#define SCREEN2 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G _ _ G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ _ _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// effective row -1/3

#define TOP L L L L L L L L L L L L L L L // Initial newline + 14 lines
#define TAIL 0x76

#define X(c) c,
uchar buffer0[] = { TOP SCREEN0 TAIL };
uchar buffer1[] = { TOP SCREEN1 TAIL };
uchar buffer2[] = { TOP SCREEN2 TAIL };

#undef X
#define X(c) c

uchar *buffers[] = {buffer0, buffer1, buffer2};
uchar *lines[3][24];

// void init_offsets() {
//     for (uchar buf = 0; buf < 3; buf++) {
//         uchar *lchar = buffers[buf];
//         for (uchar line = 0; line < 24; line++) {
//             while (*lchar != L) lchar++;
//             lines[buf][line] = lchar++;
//         }
//     }
// }

#define LAST_LINE(buf) (buf + 15 + 8 * 33)

uchar **DFILE = (uchar **)0x400C;
uchar *FRAME = (uchar *) 0x4034;

void flip(uchar *frame, uchar skip) {
    uchar next = *FRAME - skip;
    do {
        intrinsic_halt();
    } while (*FRAME != next);
    *DFILE = frame;
}

void main() {
    // init_offsets();

    unsigned int left_key = in_LookupKey('Q');
    unsigned int right_key = in_LookupKey('W');
    uchar carx = 16;
    // char lasti = 2;

    char slope = 5; // 3 times the slope between side markers

    for (;;) {

        for (char i = 0; i < 3; i++) {
            uchar *op = LAST_LINE(buffers[i]);
            char off = slope * (1 - i);

            for (int y = 0; y < 9; y++) {
                if (off > 0) {
                    *op = _; //0x1c + off;
                    *(op + off / 3) = L;
                }
                uchar z = *(op - 1);
                if (z == U || z == G) {
                    off += slope * 3;
                }
                op -= 33;
            }

            // uchar lastx = carx;
            // if (in_KeyPressed(left_key) && carx > 8) {
            //     --carx;
            // }
            // if (in_KeyPressed(right_key) && carx < 23) {
            //     ++carx;
            // }
    
            // uchar *car = lines[i][22] + carx;
            // *car++ = D; *car++ = A; *car++ = D;
            // car = lines[i][23] + carx;
            // *car++ = B; *car++ = U; *car++ = B;

            flip(buffers[i], 2);

            // car = lines[lasti][22] + lastx;
            // *car++ = _; *car++ = _; *car++ = _;
            // car = lines[lasti][23] + lastx;
            // *car++ = _; *car++ = _; *car++ = _;

            // lasti = i;
        }
    }
}
