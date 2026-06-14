// zcc +zx81 -create-app -o vertical.bin vertical.c

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
// #include <stdio.h>
#include <input.h>
#include <intrinsic.h>

const unsigned char player = 0x0d;

void main() {
    unsigned char *dfile = *(unsigned char **)0x400C;
    
    char fill = 0;
    char x = 16;
    char y = 23;
    char dy = 1; 
    int key, prev_key;

    for (;;) {
        int i = rand() & 31;
        prev_key = key;
        key = in_Inkey();
        if (key == 'Q') break;
        else if (key == ' ' && prev_key != ' ') {
            i = x;
            dy = -dy;
        }
        else if (key == 'Z' && x > 0) x--;
        else if (key == 'X' && x <31) x++;

        unsigned char *p = 0;
        if (dy > 0 && y == 23) {
            p = dfile + 1 + 22 * 33 + x;
        } else if (dy < 0 && y == 0) {
            p = dfile + 1 + 33 + x;
        }

        if (p) {
            if (*p) {
                if (x > 0 && !*(p - 1)) {
                    x--;
                    p--;
                } else if (x < 31 && !*(p + 1)) {
                    x++;
                    p++;
                } else { // bounce
                    i = x;
                    dy = -dy;
                }
            }
            *p = player;
        } else y += dy;

        unsigned char *c;
        if (dy > 0) {
            memmove(dfile + 34, dfile + 1, 23 * 33);
            c = dfile + 1 + i;
        } else {
            memmove(dfile + 1, dfile + 1 + 33, 23 * 33);
            c = dfile + 1 + 23 * 33 + i;
        }
        *c = *c? 0x00 : (fill++ & 1)? 0x00 : 0x08;
        // *c = *c == 0x00? 0x08 : *c == 0x08? 0x80 : 0x00;
    }
}
