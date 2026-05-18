
#include <intrinsic.h>
#include <input.h>

#define WT 0x00
#define GR 0x08
#define BK 0x80
#define WTS 0x76, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT
#define GRS 0x76, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR
#define BKS 0x76, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK

unsigned char buffer[] = { 
    WTS, WTS, WTS, WTS, WTS, WTS, GRS, BKS,
    WTS, WTS, WTS, WTS, WTS, WTS, GRS, BKS,
    WTS, WTS, WTS, WTS, WTS, WTS, GRS, BKS,
    WTS, WTS, WTS, WTS, WTS, WTS, GRS, BKS,
    0x76
};

void main() {
    unsigned char **dfile = (unsigned char **)0x400C;
    unsigned char *screen = *dfile; 

    unsigned char *frames = (unsigned char *) 16436;
    unsigned char current_frame = *frames;

    for (;;) {
        for (int i = 0; i < 8; ++i) {
            while (current_frame == *frames) {
                intrinsic_halt();
            }
            current_frame = *frames;
            *dfile = buffer + 33 * i;

            if (in_Inkey() == ' ') {
                *dfile = screen;
                return;
            }
        }
    }
}
