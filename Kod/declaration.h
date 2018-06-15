#ifndef DECLARATION_H_URHXV5O2
#define DECLARATION_H_URHXV5O2

#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include <stdbool.h>        /* To be able to use boolean */
#include <stdlib.h>



struct Graphic{
    int width;
    int height;
    uint32_t image[20];
};


struct Entity{
    int x;
    int y;
    int speedx;
    int speedy;
    struct Graphic imageData;
};



//display.c
uint8_t spi_send_recv(uint8_t data);
void display_image(int x, const uint8_t *data);
void display_init(void);
void draw_number(int number,int x, int y);
void draw_line(int y);
void render(void);
void draw_info();
void display_update(void);
void addToBuffer(struct Entity entity);
void addToBufferImage(const uint32_t const image[]);
void draw_dashBoard();
void display_debug( volatile int * const addr );
void impactAnimation(int x, int y, int frame);


extern const uint32_t const menuImage[];
extern const uint32_t const introImage[];
extern const uint32_t const hiscoreImage[];
extern const uint32_t const modeImage[];
extern const uint8_t const font[128*8];
extern const uint32_t const numbers[10][8];
extern char textbuffer[4][16];

//Pong functions
void update();
void init();
int rn(int x);
struct Entity getPlayer1(void);
struct Entity getPlayer2(void);


//data.c
extern struct Entity ball;
extern struct Entity player;
extern struct Entity wall;

//ball.c
void InitBall();
void renderBall();
char moveBall();
int getCollisions(void);

#endif /* end of include guard: DECLARATION_H_URHXV5O2 */
