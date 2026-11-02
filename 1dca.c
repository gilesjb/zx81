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
#define INTERNAL_WIDTH 34 

unsigned char buffer1[INTERNAL_WIDTH];
unsigned char buffer2[INTERNAL_WIDTH];

unsigned char *src = buffer1;
unsigned char *dst = buffer2;

// The largest possible index is (0x80 + 0x80 + 0x80) >> 3 = 48
unsigned char rule_table[49];

void setup_rule_table() {
    int left, mid, right;
    int state_l, state_m, state_r, state_sum;
    unsigned int char_sum;
    
    // The master Wolfram 1599 array based on the 0, 1, 2 state counts:
    // Index:        0     1     2     3     4     5     6
    // Next State:   0     2     1     0     2     0     2
    // Char Code:  0x00  0x80  0x08  0x00  0x80  0x00  0x80
    const unsigned char state_to_char[] = {0x00, 0x80, 0x08, 0x00, 0x80, 0x00, 0x80};
    const unsigned char choices[] = {0x00, 0x08, 0x80};

    memset(rule_table, 0x00, sizeof(rule_table));

    // Exhaustively iterate through all 27 possible neighborhood combinations
    for (left = 0; left < 3; left++) {
        for (mid = 0; mid < 3; mid++) {
            for (right = 0; right < 3; right++) {
                
                // 1. Calculate what the original state sum (0, 1, 2) would be
                state_l = (choices[left]  == 0x00) ? 0 : ((choices[left]  == 0x08) ? 1 : 2);
                state_m = (choices[mid]   == 0x00) ? 0 : ((choices[mid]   == 0x08) ? 1 : 2);
                state_r = (choices[right] == 0x00) ? 0 : ((choices[right] == 0x08) ? 1 : 2);
                
                state_sum = state_l + state_m + state_r;

                // 2. Sum up the actual raw character codes
                char_sum = (unsigned int)choices[left] + choices[mid] + choices[right];

                // 3. Store the correct target character token in our compressed array hash slot
                rule_table[char_sum >> 3] = state_to_char[state_sum];
            }
        }
    }
}

void init_automata() {
    int i;
    unsigned char choices[] = {0x00, 0x08, 0x80};
    for (i = 1; i <= WIDTH; i++) {
        src[i] = choices[rand() % 3];
    }
}

int main() {
    int x;
    unsigned char *tmp;
    unsigned char *screen_row;
    int current_line = 0;

    zx_slow();
    printf("\014"); 
    
    setup_rule_table();
    init_automata();

    unsigned char *dfile_base = *(unsigned char **)0x400C; 

    while (1) {
        if (in_Inkey() == ' ') {
            break; 
        }

        // Edge wrapping using raw hardware bytes
        src[0] = src[WIDTH];
        src[WIDTH + 1] = src[1];

        // Locate VRAM line target
        screen_row = dfile_base + 1 + (current_line * 33);

        unsigned char *p_left  = &src[0];
        unsigned char *p_mid   = &src[1];
        unsigned char *p_right = &src[2];
        unsigned char *p_dst   = &dst[1];

        for (x = 0; x < WIDTH; x++) {
            // Blast character straight to memory
            *screen_row++ = *p_mid;

            // Mathematical update lookup via direct bit shifted sums
            unsigned int char_sum = (unsigned int)*p_left++ + *p_mid++ + *p_right++;
            *p_dst++ = rule_table[char_sum >> 3];
        }

        // Swap execution generation buffers
        tmp = src; src = dst; dst = tmp;

        current_line++;
        if (current_line >= HEIGHT) {
            current_line = 0; 
        }
    }

    printf("\014");
    return 0;
}
