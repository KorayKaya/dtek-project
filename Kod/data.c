

#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */

char textbuffer[4][16];



//image of ball
struct Entity ball = {14,62,0,0,{2,2,{0x00000008,0x00000008}}};

struct Entity player = {0,0,2,0,{16,4,{0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff}}};

struct Entity wall = {0,0,0,0,{32,4,{0xffffffff,0xffffffff,0xffffffff,0xffffffff}}};

const uint8_t const numberFont[70] = {112,32,48,48,16,120,48,120,48,48
,136,96,72,72,80,64,72,8,72,72
,136,32,8,8,80,48,64,8,72,72
,168,32,16,48,120,8,112,16,48,56
,136,32,32,8,16,8,72,16,72,8
,136,32,64,72,16,72,72,32,72,8
,112,112,120,48,16,48,48,32,48,8
};

const uint32_t modeImage[] = {
0xffffffff,
0xffffffe0,
0xffffffea,
0x03ffe195,
0xf8fc082a,
0xff795517,
0xff32aaaf,
0xff85fffd,
0xffcbbb6f,
0xffd7ffff,
0xffcdffff,
0xffdfffff,
0xffbfffff,
0xffafffff,
0xff7fffff,
0xffafffff,
0xff7fffff,
0xffdfffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfff5ffff,
0xfff9ffff,
0xfff4ffff,
0xfffaffff,
0xfffcffff,
0xfffaffff,
0x7fdcffff,
0x7ffaffff,
0x3f74ffff,
0x5fea6fff,
0x2d857bfb,
0x9eaa5f6e,
0xd560affd,
0xeaff956a,
0xf1ffaa95,
0xffffc00a,
0xffffefe1,
0xfffffffc,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfc3fdfff,
0xfdffdfff,
0xfdf0d87f,
0xfc36db7f,
0xffb6db7f,
0xffb6db7f,
0xfc30d87f,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfe3fffff,
0xfedfffff,
0xfedb70ff,
0xfedb76ff,
0xfedb76ff,
0xfedb76ff,
0xfe38b0ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfc3fffff,
0xf3bfffff,
0xe7cfffff,
0xdfefffff,
0x9fe6ffff,
0x3ff47fff,
0xfff9bfff,
0xfffbbfff,
0xfffdbfff,
0xfffb8fff,
0xfffde3ff,
0xfffffbff,
0xfffffdff,
0xfff7fdff,
0x57c9f5ff,
0xaaa2a9ff,
0x054d53ff,
0xf83c07ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffe0,
0xffffffdf,
0xffffffdf,
0xffffffdf,
0xffffffdf,
0xffffffd7,
0x1fffffea,
0xefffffe1,
0xf47ffffc,
0xf1bfffff,
0xff81ffff,
0xfffe7fff,
0xffff0fff,
0xffffe7ff,
0xfffff7ff,
0xfffff3ff,
0xfffffbff,
0x557ffbff,
0xaaad59ff,
0x0052a0ff,
0xff8000ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffe3,
0xffffffd8,
0x0fffffdf,
0xf7ffffde,
0xf9ffff9d,
0xfe7fffaa,
0x551fffd5,
0xaaa0ffc8,
0x00001fe1
};

const uint32_t hiscoreImage[] = {

0x00000000,
0x07800001,
0x8fb000c7,
0xdfbf03ff,
0xffdfc7ff,
0xffdfa7ff,
0xffee7fff,
0xfff1ffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xdaf1fff7,
0xdbf722ab,
0xc2916a63,
0xdafd6aef,
0xdaf122e3,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffe0,
0xffffffef,
0x03ffe19f,
0xf8fc0c3f,
0xff79ffbf,
0xff37ffff,
0xff87ffff,
0xffdfffff,
0xffdfffff,
0xffdfffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfffdffff,
0xfffdffff,
0xfffcffff,
0xfffeffff,
0xfffeffff,
0xfffeffff,
0x7ffeffff,
0x7ffeffff,
0x7ffeffff,
0x7ffe7fff,
0x3f8f7fff,
0x9fae7fff,
0xdf60bfff,
0xeeffbfff,
0xf1ffbfff,
0xffffc00f,
0xffffefe1,
0xfffffffc,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfc3fffff,
0xf3bfffff,
0xe7cfffff,
0xdfefffff,
0x9fe6ffff,
0x3ff47fff,
0xfff9bfff,
0xffffbfff,
0xffffbfff,
0xffff8fff,
0xffffe3ff,
0xfffffbff,
0xfffffdff,
0xfffffdff,
0xfffffdff,
0xffe3fdff,
0x07cdfbff,
0xf83c07ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffe0,
0xffffffdf,
0xffffffdf,
0xffffffdf,
0xffffffdf,
0xffffffdf,
0x1fffffef,
0xefffffe3,
0xf47ffffc,
0xf1bfffff,
0xff81ffff,
0xfffe7fff,
0xffff0fff,
0xffffe7ff,
0xfffff7ff,
0xfffff3ff,
0xfffffbff,
0xfffffbff,
0xfffffdff,
0x007ffeff,
0xff8000ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffe3,
0xffffffd8,
0x0fffffdf,
0xf7ffffdf,
0xf9ffff9f,
0xfe7fffbf,
0xff9fffdf,
0xffe0ffcf,
0xfffe1fe3
};

const uint32_t menuImage[] = {
  0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xf000001f,
0xf7ffffef,
0xf400002f,
0xf400002f,
0xf4ffff2f,
0xf480012f,
0xf4bffd2f,
0xf4bffd2f,
0xf4bffd2f,
0xf480012f,
0xf4ffff2f,
0xf400002f,
0xf400002f,
0xf4ffffef,
0xf480001f,
0xf4bfffff,
0xf4bfffff,
0xf4bfffff,
0xf4bfffff,
0xf4bffffe,
0xf4bffffe,
0xf4bffff1,
0xf7bffff0,
0xf87ffff4,
0xfffffff8,
0xfffffff0,
0xffffffe0,
0xffffffe0,
0xffffffe0,
0xff07fff0,
0xfc21fff8,
0xfc1107fe,
0xf89200fe,
0xe80c001f,
0xf0000007,
0xf8200017,
0xf9c00017,
0xfbc00017,
0xfbc00017,
0xfbc00017,
0xfbc40117,
0xfbc3023f,
0xfbe3fe3f,
0xfbf3fe3f,
0xfde3fe3f,
0x5fe37e3b,
0xb5c37c37,
0x00000000,
0x00000000,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xc0cf033b,
0xcecf3b3b,
0xcecf3b3b,
0xcecf3b3b,
0xc0cf0303,
0xcfcf3bcf,
0xcfcf3bcf,
0xcfcfbbcf,
0xcfc1bbcf,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xdaf1fff7,
0xdbf722ab,
0xc2916a63,
0xdafd6aef,
0xdaf122e3,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

//Font for menu
const uint32_t introImage[] = {
0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xf000001f
,0xf7ffffef
,0xf400002f
,0xf400002f
,0xf4ffff2f
,0xf480012f
,0xf4bffd2f
,0xf4bffd2f
,0xf4bffd2f
,0xf480012f
,0xf4ffff2f
,0xf400002f
,0xf400002f
,0xf4ffffef
,0xf480001f
,0xf4bfffff
,0xf4bfffff
,0xf4bfffff
,0xf4bfffff
,0xf4bffffe
,0xf4bffffe
,0xf4bffff1
,0xf7bffff0
,0xf87ffff4
,0xfffffff8
,0xfffffff0
,0xffffffe0
,0xffffffe0
,0xffffffe0
,0xff07fff0
,0xfc21fff8
,0xfc1107fe
,0xf89200fe
,0xe80c001f
,0xf0000007
,0xf8200017
,0xf9c00017
,0xfbc00017
,0xfbc00017
,0xfbc00017
,0xfbc40117
,0xfbc3023f
,0xfbe3fe3f
,0xfbf3fe3f
,0xfde3fe3f
,0x5fe37e3b
,0xb5c37c37
,0x00000000
,0x00000000
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xf07fffff
,0xf7bffe1f
,0xf4dffdef
,0xf46ffd2f
,0xf437fd2f
,0xf41bfd2f
,0xf40dfd2f
,0xf486fd2f
,0xf4c37d2f
,0xf4e1bd2f
,0xf4b0dd2f
,0xf4986d2f
,0xf4ac352f
,0xf4b6192f
,0xf4bb0d2f
,0xf4bd872f
,0xf4bec32f
,0xf4bf612f
,0xf4bfb02f
,0xf4bfd82f
,0xf4bfec2f
,0xf4bff62f
,0xf7bffb2f
,0xf87ffdef
,0xfffffe1f
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xfe00001f
,0xfdffffef
,0xfb00002f
,0xf600002f
,0xf47fffef
,0xf4c0001f
,0xf4bfffff
,0xf4bfffff
,0xf4b8001f
,0xf4b7ffef
,0xf4b4002f
,0xf4b4002f
,0xf4b7ff2f
,0xf4b8012f
,0xf4bffd2f
,0xf4bffd2f
,0xf4bffd2f
,0xf4bff32f
,0xf4800e2f
,0xf4fff82f
,0xf400002f
,0xf600032f
,0xfbffff2f
,0xfc00012f
,0xfffffd2f
,0xfffffdef
,0xfffffe1f
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff
,0xffffffff


};


const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};
