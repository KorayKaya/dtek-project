
#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */
#include <stdbool.h>        /* To be able to use boolean */


void *stdin, *stdout, *stderr;

static unsigned int btns;
static unsigned int sw;

int screen, selection, direction, score;
int intro = 0;


struct Entity player1,player2;



//Get button values.
unsigned int getbtns(void) {return PORTD >> 4 & 14 | PORTF >> 1 & 1;}

//Generate number
int rn(int x) {return rand() % x;}

//Setup timer
static void timer_init(void) {
  T2CON = 0x70;
  PR2 = (80000000 / 256) / 15;
  T2CONSET = 0x8000;
}

//Fix button
static void btn_init(void) {
  // Init port D
  // Set bits 11 through 5 to 1 (input)
  TRISDSET = 0b1111111 << 5;
}

//Return switch values
int getsw( void ) {
  return ((PORTD & (0xF << 8)) >> 8);
}

//Initalize
void init(void) {
  srand(13);
  display_init();
  timer_init();
  display_update();

}

//Run the game
static void game() {

  if (sw & 8) direction = -1;
  else direction=1;
  if (((player1.x + player1.speedx*direction)>0) && ((player1.x + player1.speedx*direction + player1.imageData.width)<32)){
    player1.x += player1.speedx*direction;
  }

  if (sw & 1) direction = -1;
  else direction=1;
  if (((player2.x + player2.speedx*direction)>0) && ((player2.x + player2.speedx*direction + player2.imageData.width)<32)){
    player2.x += player2.speedx*direction;
  }

  addToBuffer(player1);
  addToBuffer(player2);

  if (moveBall()) screen=4;
  renderBall();

}


//Run the main menu
void main_menu() {
  int z;

  if ((btns & 4) && selection>0)
    selection -= 1;
  else if ((btns & 2) && selection<1)
    selection += 1;

  if (btns & 8) {
    switch (selection) {
      case 0:
        screen=1;
        for (z = 0; z < 700000; z++);
        break;
      case 1:
        selection=0;
        screen=2;
        for (z = 0; z < 700000; z++);
        break;
    }
  }
  addToBufferImage(menuImage);
  switch(selection) {
    case 0:
      draw_line(77);
      break;
    case 1:
      draw_line(90);
      break;
  }

}


//Run the choose rocket menu
void hiscore_menu() {

  int z;
  if (btns & 8) {
    screen = 0;
    for (z = 0; z < 700000; z++);
  }
  addToBufferImage(hiscoreImage);
}


//Run the choose difficulty menu
void chooseMode() {

  int z;

  if (btns & 8) {
        InitBall();
        player1 = player;
        player1.x = 0;
        player1.y = 0;

        player2 = player;
        player2.x = 0;
        player2.y = 124;       
    screen = 3;
    for (z = 0; z < 700000; z++);
  }
}

//Run gameover screen
void gameover() {
  int z;
  if (btns & 8) {
    screen = 0;
    for (z = 0; z < 700000; z++);
  }
}

void update(void) {

  btns = getbtns();

  sw =  getsw();
  if (IFS(0) & 0x100) {
    IFS(0) = 0; // Reset timer flag

    if (intro<20)
    {
      addToBufferImage(introImage);
      intro++;
    }
    else{
      switch (screen) {
    case  0:
      main_menu();
      break;
    case  1:
      chooseMode();
      break;
    case  2:
      hiscore_menu();
      break;
    case  3:
      game();
      break;
    case 4:
      gameover();
      break;

      }
    }

    render();
  }
}
