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
// #define _ X(0x16) // minus
#define L X(0x76) // newline
#define B X(0x80) // black square
#define G X(0x08) // grey square
#define U X(0x0a) // upper half grey
#define D X(0x09) // bottom half grey
#define A X(0x26) // 'A'

#define SCREEN0 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L _ _ D D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ _ U U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U U
// effective row 1/3

#define SCREEN1 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G _ _ _ _ _ G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ D D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ _ _ _ _ _ U U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U U \
    L G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G
// effective row 0/3

#define SCREEN2 \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G _ G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
// effective row -1/3

#define TOP L L L L L L L L L L L L L L L // Initial newline + 14 lines
#define TAIL 0x76

#define X(c) c,
uchar buffer0[] = { TOP SCREEN0 TAIL };
uchar buffer1[] = { TOP SCREEN1 TAIL };
uchar buffer2[] = { TOP SCREEN2 TAIL };

#undef X
#define X(c) c

#define FP 6 // number of fixed point bits

uchar *buffers[] = {buffer0, buffer1, buffer2};

// offsets of screen buffer lines into slope array
uchar offsets[][9] = {
    {13, 13, 13, 10, 7, 7, 4, 1, 1},
    {15, 15, 12,  9, 9, 6, 3, 3, 0},
    {14, 14, 11, 11, 8, 5, 5, 2, 0}
};

// road shape tables
uchar straight_left[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
uchar straight_left1[] = {0, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20, 21, 23, 24, 26};
uchar straight_center[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uchar straight_right1[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8};
uchar straight_right[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uchar curve_left[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 22, 30};
uchar curve_right[] = {0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 7, 7, 6, 6, 6, 5};

uchar *curves[] = {straight_left, straight_left1, straight_center, straight_right1, straight_right, curve_left, curve_right};

// current slopes being used by the screens
uchar *slopes[] = {straight_right, straight_right, straight_right};
uchar carxs[] = {14, 14, 14};

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
    unsigned int left_key = in_LookupKey('O');
    unsigned int right_key = in_LookupKey('P');

    uchar carx = carxs[0];

    uchar *slope = straight_left;

    for (;;) {

        for (uchar i = 0; i < 3; i++) {

            uchar *op = buffers[i] + 15;
            uchar *offset = offsets[i];
            uchar off, *p_, *pl;

            for (int l = 0; l < 7; l++) {
                off = *offset++;
                p_ = op + slopes[i][off];
                pl = op + slope[off];
                *(p_) = _;
                *(pl) = L;
                op += 32;
            }
            off = *offset++;
            p_ = op + slopes[i][off];
            pl = op + slope[off];

            *(p_) = _;
            p_ += carxs[i]; *p_++ = _; *p_++ = _; *p_++ = _;
            *(pl) = L;
            pl += carx; *pl++ = D; *pl++ = A; *pl++ = D;
            op += 32;

            off = *offset++;
            p_ = op + slopes[i][off];
            pl = op + slope[off];

            *(p_) = _;
            p_ += carxs[i]; *p_++ = _; *p_++ = _; *p_++ = _;
            *(pl) = L;
            pl += carx; *pl++ = B; *pl++ = U; *pl++ = B;

            // update the buffer records
            slopes[i] = slope;
            carxs[i] = carx;

            flip(buffers[i], 2);
            
            unsigned int key = in_Inkey();
            if (key == 'O') {
                --carx;
            } else if (key == 'P') {
                ++carx;
            } else if ('1' <= key && key < '1' + sizeof(curves)) {
                slope = curves[key - '1'];
            }
        }
    }
}
