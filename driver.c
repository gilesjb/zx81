#if 0
zcc +zx81 -create-app -o build/driver.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
#include <input.h>
#include <intrinsic.h>

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
    L _ _ _ U U U _ _ _ _ _ _ _ _ D A D _ _ _ _ _ _ _ _ _ U U U \
    L G G G _ _ _ _ _ _ _ _ _ _ _ B U B _ _ _ _ _ _ _ _ _ _ _ _ G G G

#define SCREEN2 \
    L \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ _ G _ _ G \
    L _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ _ _ _ _ U \
    L _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ G G G _ _ _ _ _ _ _ _ _ D A D _ _ _ _ _ _ _ _ _ _ G G G \
    L D D _ _ _ _ _ _ _ _ _ _ _ _ B U B _ _ _ _ _ _ _ _ _ _ _ _ _ D D

#define SCREEN3 \
    L \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ D _ _ _ _ D \
    L _ _ _ _ _ _ _ _ _ _ _ _ _ U _ _ _ _ U \
    L _ _ _ _ _ _ _ _ _ _ G G _ _ _ _ _ _ _ _ G G \
    L _ _ _ _ _ _ _ D D _ _ _ _ _ _ _ _ _ _ _ _ _ _ D D \
    L _ _ _ _ _ _ _ U U _ _ _ _ _ _ _ _ _ _ _ _ _ _ U U \
    L _ _ _ _ G G G _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ G G G \
    L _ D D D _ _ _ _ _ _ _ _ _ _ D A D _ _ _ _ _ _ _ _ _ _ _ D D D \
    L _ U U U _ _ _ _ _ _ _ _ _ _ B U B _ _ _ _ _ _ _ _ _ _ _ U U U  

#define TOP L L L L L L L L L L L L L L
#define END L L L L L L L

#define X(c) c,
unsigned char buffer1[] = { TOP SCREEN1 END 0x00 };
unsigned char buffer2[] = { TOP SCREEN2 END 0x00 };
unsigned char buffer3[] = { TOP SCREEN3 END 0x00 };
#undef X

unsigned char *buffers[] = {buffer1, buffer2, buffer3};

unsigned char **dfile = (unsigned char **)0x400C;
unsigned char *frames = (unsigned char *) 0x4034;

void flip(unsigned char *frame) {
    for (unsigned char i = 0; i < 2 ; i++) {
        unsigned char current_frame = *frames;
        do {
            intrinsic_halt();
        } while (current_frame == *frames);
    }
    *dfile = frame;
}

void main() {

    for (;;) {
        for (char i = 0; i < 3; i++) {
            flip(buffers[i]);
        }
    }
}
