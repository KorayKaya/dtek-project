#ifndef DECLARATION_H_URHXV5O2
#define DECLARATION_H_URHXV5O2

#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include <stdbool.h>        /* To be able to use boolean */
#include <stdlib.h>



struct ImageData{
    int width;
    int height;
    uint32_t image[8];
};

struct Entity{
    int x;
    int y;
    int speedx;
    int speedy;
    struct ImageData imageData;
};


//display.c
uint8_t spi_send_recv(uint8_t data);
void display_image(int x, const uint8_t *data);
void display_init(void);
void draw_menu(void);
void draw_gameover(void);
void draw_line(int y);
void draw_rocketMenu();
void render(void);
void draw_info();
void display_update(void);
char collision(struct Entity entity);
void addToBuffer(struct Entity entity);
void addToBufferImage(const uint32_t const image[]);
void draw_dashBoard();
void draw_go(int y);
void display_debug( volatile int * const addr );


extern const uint32_t const menuImage[];
extern const uint32_t const introImage[];
extern const uint32_t const hiscoreImage[];
extern const uint32_t const modeImage[];
extern const uint8_t const font[128*8];
extern char textbuffer[4][16];
const uint8_t const numberFont[70];

//ABHO-12 functions and variables
void chooseMode();
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


#endif /* end of include guard: DECLARATION_H_URHXV5O2 */
