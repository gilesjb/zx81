#if 0
zcc +zx81 -create-app -o build/driver.bin "$0"; exit
#endif

#include <zx81.h>
#include <string.h>
#include <stdlib.h>
#include <input.h>
#include <intrinsic.h>

#define W 0x00
#define WW W,W
#define WWW W,W,W
#define L 0x76
#define B 0x80
#define TOP L,L,L,L,L,L,L,L,L,L,L,L,L,L,L

#define G 0x08
#define U 0x0a
#define D 0x09

#define CART D,0x26,D // top of car
#define CARB B,U,B    // bottom of car

unsigned char buffer1[] = {
    TOP,

    // prev line should be just long enough we can 'borrow' up to 15 spaces from it 
    WWW, WWW, WWW, WWW, WWW, D,D,L,  //15
    WWW, WWW, WWW, WWW, WWW, U,U,L,  //15

    WWW, WWW, WWW, WWW, G, WWW, WWW, G,L, //12

    WWW, WWW, WWW, D,D, WW, WWW, WWW, WW, D,D,L, //9
    WWW, WWW, WWW, U,U, WW, WWW, WWW, WW, U,U,L, //9

    WWW, WWW, G,G, WW, WWW, WWW, WWW, WWW, WW, G,G,L, //6

    WWW, D,D,D, WWW, WWW, WWW, WWW, WWW, WWW, WW, D,D,D,L, //3
    WWW, U,U,U, WWW, WWW, WWW, CART, WWW, WWW, WW, U,U,U,L, //3
    
    G,G,G, WWW,WWW,WWW,WWW,CARB,WWW,WWW,WWW, WW, G,G,G,L, //0

    L,L,L,L,L,L
};

unsigned char buffer2[] = {
    TOP,

    L,
    WW, WWW, WWW, WWW, WWW, G, WW, G,L, //14

    WW, WWW, WWW, WWW, D, WWW, WWW, WW, D,L, //11
    WW, WWW, WWW, WWW, U, WWW, WWW, WW, U,L, //11
    
    WW, WWW, WWW, G,G, WWW,WWW,WWW,WWW, G,G,L, //8

    WW, WWW, D,D, WWW, WWW, WWW, WWW, WWW, WWW, D,D,L, //5
    WW, WWW, U,U, WWW, WWW, WWW, WWW, WWW, WWW, U,U,L, //5

    WW, G,G,G, W, WWW, WWW, WWW, CART, WWW, WWW, WWW, G,G,G,L, //2

    D,D, W, WWW, WWW, WWW, WWW, CARB, WWW, WWW, WWW, WWW, D,D,L, //-1

    L,L,L,L,L,L
};

unsigned char buffer3[] = {
    TOP,

    L,
    WWW, WWW, WWW, WWW, W, D, WW, WW, D,L, //13
    WWW, WWW, WWW, WWW, W, U, WW, WW, U,L, //13

    WWW, WWW, WWW, W, G,G, WWW, WWW, WW, G,G,L, //10

    WWW, WWW, W, D,D, WWW, WWW, WWW, WWW, WW, D,D,L, //7
    WWW, WWW, W, U,U, WWW, WWW, WWW, WWW, WW, U,U,L, //7

    WWW, W, G,G,G, WWW,WWW,WWW,WWW,WWW,WWW, G,G,G,L, //4

    W, D,D,D, WWW,WWW,WWW,WW,CART,W,WWW,WWW,WWW,D,D,D,L, //1
    W, U,U,U, WWW,WWW,WWW,WW,CARB,W,WWW,WWW,WWW,U,U,U,L, //1

    L,L,L,L,L,L
};

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
        for (unsigned int y = 1; y < 46; y++) {
            flip(buffer1);
            flip(buffer2);
            flip(buffer3);
        }
    }
}
