#if 0
zcc +zx81 -create-app -o build/1dca.bin "$0"; exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zx81.h>
#include <input.h>

#define WIDTH 32
#define HEIGHT 24

// State buffers for cellular tracking
unsigned char buffer1[WIDTH];
unsigned char buffer2[WIDTH];

unsigned char *src = buffer1;
unsigned char *dst = buffer2;

// Rule mapping: Neighborhood sums range from 0 to 6
const unsigned char rule[] = 
    // {0, 2, 1, 0, 2, 0, 2}; // Wolfram 1599 rule
    {0, 2, 0, 1, 1, 1, 2}; // Wolfram 1815 rule
    // {0, 0, 1, 2, 0, 2, 2}; // Wolfram 2007 rule

// Map cell states directly to raw native ZX81 hardware character matrix
const unsigned char char_map[] = {0x00, 0x08, 0x80};

void init_automata() {
    int i;
    for (i = 0; i < WIDTH; i++) {
        src[i] = rand() % 3;
    }
}

int main() {
    int x;
    unsigned int sum;
    unsigned char *tmp;
    unsigned char *screen_row;
    int current_line = 0;

    init_automata();

    // Fetch the hardware D_FILE pointer location from z88dk system variables
    // d_file points to the initial HALT instruction (0x76) at the start of VRAM.
    unsigned char *dfile_base = *(unsigned char **)0x400C; 

    // Infinite loop for continuous rendering
    while (1) {
        if (in_Inkey() == ' ') {
            break; 
        }
        // Calculate the absolute memory address of the target row
        // Skip the initial 0x76 byte (+1). Each row has 32 chars + 1 newline byte (33 bytes total).
        screen_row = dfile_base + 1 + (current_line * 33);

        // 1. Blast raw hardware character bytes directly into screen memory row
        for (x = 0; x < WIDTH; x++) {
            screen_row[x] = char_map[src[x]];
        }

        // 2. Loop calculations across the 32 column neighborhood layout
        for (x = 0; x < WIDTH; x++) {
            int left  = (x == 0) ? src[WIDTH - 1] : src[x - 1];
            int mid   = src[x];
            int right = (x == WIDTH - 1) ? src[0] : src[x + 1];

            sum = left + mid + right;
            dst[x] = rule[sum];
        }

        // 3. Low-overhead pointer swap to progress to the next iteration safely
        tmp = src;
        src = dst;
        dst = tmp;

        // 4. Handle vertical screen wrapping tracking
        current_line++;
        if (current_line >= HEIGHT) {
            current_line = 0; 
        }
        
        // Optional frame brake pacing mechanism: 
        // If it renders too fast, uncomment the next line to yield slightly to the ULA rasterizer:
        // delay(10); 
    }

    return 0;
}
