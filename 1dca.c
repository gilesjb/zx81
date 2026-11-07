#if 0
zcc +zx81 -create-app -o build/1dca.bin "$0"; exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zx81.h>
#include <input.h>
#include <intrinsic.h>

#define WIDTH 32
#define HEIGHT 24
#define ROW_BYTES 33

// Double-sized hardware display matrix (48 lines total)
// Structure: 1 initial HALT + (48 lines * 33 bytes per line)
#define BUFFER_LINES 48
uchar big_dfile[1 + ((BUFFER_LINES + 2) * ROW_BYTES)];

#define CHAR0 0x00
#define CHAR1 0x08
#define CHAR2 0x80

// State buffers for cellular tracking
uchar buffer1[WIDTH + 2];
uchar buffer2[WIDTH + 2];
uchar *src = buffer1;
uchar *dst = buffer2;

const uchar rule[] = 
    // {0, 2, 0, 2, 1, 0, 2}; // Wolfram 1599 rule
    // {0, 2, 0, 1, 1, 1, 2}; // Wolfram 1815 rule
    {0, 0, 1, 2, 0, 2, 2}; // Wolfram 2007 rule

const uchar char_map[] = 
    {CHAR0, CHAR1, CHAR2};
    // {0x00, 0x34, 0x1c};

void init_automata() {
    int i;
    for (i = 0; i < WIDTH; i++) {
        src[i] = rand() % 3;
    }
}

// Format our massive custom matrix to look like native ZX81 VRAM structures
void setup_big_dfile() {
    int line;
    big_dfile[0] = 0x76; // Initial hardware HALT
    
    for (line = 0; line < BUFFER_LINES + 2; line++) {
        uchar *row = &big_dfile[1 + (line * ROW_BYTES)];
        // memset(row, 0x00, WIDTH);   // Fill row characters with spaces
        row[WIDTH] = 0x76;          // Terminate line with hardware HALT
    }
}

uchar **DFILE = (uchar **)0x400C;

int main() {
    int x;
    unsigned int sum;
    uchar *tmp;
    uchar *original_dfile = *DFILE;
    
    // We track everything relative to our current scrolling viewport index (0 to 23)
    int viewport_line = 1;                 

    setup_big_dfile();
    init_automata();

    while (1) {
        if (in_Inkey() == ' ') {
            break; 
        }

        uchar *screen_top = &big_dfile[viewport_line * ROW_BYTES];

        uchar *pr1 = screen_top + 1 + ROW_BYTES * (HEIGHT - 1), 
            *pr2 = screen_top + 1 - ROW_BYTES * 2,
            *pg = src + 1;

        for (uchar x = 0; x < WIDTH; x++) {
            uchar code = *pg++;
            uchar glyph = char_map[code];                
            *pr1++ = glyph;
            *pr2++ = glyph;
        }

        *DFILE = screen_top;

        uchar *ps = src, *pd = dst + 1;
        uchar sum = *ps + *(ps + 1) + *(ps + 2);
        // 3. Calculate the NEXT generation
        for (x = 0; x < WIDTH; x++) {
            *pd++ = rule[sum];
            sum -= *ps;
            sum += *(ps + 3);
            ++ps;
        }
        dst[0] = dst[WIDTH];
        dst[WIDTH + 1] = dst[1];

        // Pointer swap for cellular buffer
        tmp = src; src = dst; dst = tmp;

        // When the viewport completes its pass through the first 24 lines, 
        // it instantly jumps back to 0. Because of our dual-writing mirror above,
        // row 0-22 already contain the preceding partial page history perfectly intact!
        if (++viewport_line > HEIGHT + 1) {
            viewport_line = 1;
        }
    }

    memmove(original_dfile, *DFILE, HEIGHT * ROW_BYTES);
    *DFILE = original_dfile; 

    return 0;
}
