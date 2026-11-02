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
unsigned char big_dfile[1 + (BUFFER_LINES * ROW_BYTES)];

// State buffers for cellular tracking
unsigned char buffer1[WIDTH];
unsigned char buffer2[WIDTH];
unsigned char *src = buffer1;
unsigned char *dst = buffer2;

const unsigned char rule[] = {0, 2, 0, 1, 1, 1, 2}; // Wolfram 1815 rule
const unsigned char char_map[] = {0x00, 0x08, 0x80};

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
    
    for (line = 0; line < BUFFER_LINES; line++) {
        unsigned char *row = &big_dfile[1 + (line * ROW_BYTES)];
        memset(row, 0x00, WIDTH);   // Fill row characters with spaces
        row[WIDTH] = 0x76;          // Terminate line with hardware HALT
    }
}

int main() {
    int x;
    unsigned int sum;
    unsigned char *tmp;
    
    // We track everything relative to our current scrolling viewport index (0 to 23)
    int viewport_line = 0;                 

    setup_big_dfile();
    init_automata();

    // Grab the address of the pointer holding the hardware D_FILE address
    unsigned char **dfile_pointer = (unsigned char **)0x400C;

    while (1) {
        if (in_Inkey() == ' ') {
            break; 
        }

        // Calculate the bottom row relative to our moving viewport (23 rows down)
        int bottom_line_idx = viewport_line + (HEIGHT - 1);
        unsigned char *screen_bottom = &big_dfile[1 + (bottom_line_idx * ROW_BYTES)];

        // 1. Dual-Write Mirroring Optimization: 
        // Write the characters to the current scrolling bottom row.
        // If we are deep in the second half of the buffer, mirror it to the top half too.
        for (x = 0; x < WIDTH; x++) {
            unsigned char glyph = char_map[src[x]];
            screen_bottom[x] = glyph;
            
            if (bottom_line_idx >= HEIGHT) {
                // Subtract 24 lines to mirror the exact same data to the top page variant
                big_dfile[1 + ((bottom_line_idx - HEIGHT) * ROW_BYTES) + x] = glyph;
            }
        }

        // 2. Hardware Viewport Shift: Point the ZX81 display chip to our sliding index
        *dfile_pointer = &big_dfile[viewport_line * ROW_BYTES];

        // 3. Calculate the NEXT generation
        for (x = 0; x < WIDTH; x++) {
            int left  = (x == 0) ? src[WIDTH - 1] : src[x - 1];
            int mid   = src[x];
            int right = (x == WIDTH - 1) ? src[0] : src[x + 1];

            sum = left + mid + right;
            dst[x] = rule[sum];
        }

        // Pointer swap for cellular buffer
        tmp = src; src = dst; dst = tmp;

        // 4. Progress scrolling mechanics
        viewport_line++;

        // When the viewport completes its pass through the first 24 lines, 
        // it instantly jumps back to 0. Because of our dual-writing mirror above,
        // row 0-22 already contain the preceding partial page history perfectly intact!
        if (viewport_line >= HEIGHT) {
            viewport_line = 0;
        }
    }

    // Restore the pointer to the absolute top before exiting to protect OS stability
    *dfile_pointer = big_dfile; 

    return 0;
}
