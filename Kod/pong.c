
#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */
#include <stdbool.h>        /* To be able to use boolean */


/*
The core of the game, handles different screens and the game logic

*/

void *stdin, *stdout, *stderr;

static unsigned int btns;
static unsigned int sw;

int screen, selection, xdirection, score, y_speed,hiscore,animationtest;
int intro = 0;
int ydirection = 1;
int movey_counter = 0;


struct Entity player1, player2;


struct Entity  getPlayer1(void) {return player1;}
struct Entity  getPlayer2(void) {return player2;}

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
  display_init();
  timer_init();
  display_update();
  srand(TMR2);
  hiscore=0;

}

//Run the game
static void game() {

  //sw 4 controlls player 1
  if (sw & 8) xdirection = -1;
  else xdirection = 1;
  if (((player1.x + player1.speedx * xdirection) > 0) && ((player1.x + player1.speedx * xdirection + player1.imageData.width) < 32)) {
    player1.x += player1.speedx * xdirection;
  }

  //sw 1 controlls player 2
  if (sw & 1) xdirection = -1;
  else xdirection = 1;
  if (((player2.x + player2.speedx * xdirection) > 0) && ((player2.x + player2.speedx * xdirection + player2.imageData.width) < 32)) {
    player2.x += player2.speedx * xdirection;
  }

  //players cant go off screen
  if (player2.y < 88)
  {
    ydirection = -1;
  } else if (player2.y > 123) {
    ydirection = 1;
  }

  //difficulty increase with score
  if (score < 50)
  {
    y_speed = 5 - ((int) score / 10);
  } else {
    y_speed = 0;
  }

  //only move once every "y_speed" updates
  if (movey_counter >= y_speed )
  {
    player1.y += player1.speedy * ydirection;
    player2.y += player2.speedy * ydirection;
    movey_counter = 0;
  } else {
    movey_counter++;
  }

  //add entity to buffer
  addToBuffer(player1);
  addToBuffer(player2);

  //if moveball returns 1 which means it left the screen, we see gameover
  if (moveBall()){screen = 4;}
  score = getCollisions();
  draw_number(score, 16, 60);
  renderBall();


}

//Run the main menu
void main_menu() {
  int z;

  //btn 2 and 3 changes selection
  if ((btns & 4))
    selection = 0;
  else if ((btns & 2))
    selection = 1;

  //btn 4 confirms selection, we pick screen depending on selection
  if (btns & 8) {
    switch (selection) {
    case 0:
      screen = 1;
      for (z = 0; z < 700000; z++);
      break;
    case 1:
      screen = 2;
      for (z = 0; z < 700000; z++);
      break;
    }
  }
  //we add the background to buffer
  addToBufferImage(menuImage);
  switch (selection) {
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
    selection = 0;
    for (z = 0; z < 700000; z++);
  }
  addToBufferImage(hiscoreImage);
  draw_number(hiscore, 21, 70);
}


//Run the choose difficulty menu
void mode_menu() {

  int z;

  if ((btns & 4) && selection > 0)
    selection -= 1;
  else if ((btns & 2) && selection < 1)
    selection += 1;

  if (btns & 8) {
    //single player selection
    if (selection == 0) {
      InitBall();
      animationtest=0;
      player1 = wall;
      player1.x = 0;
      player1.y = 0;

      player2 = player;
      player2.x = 0;
      player2.y = 124;
      player2.speedy = -1;
      score = 0;
      screen = 3;
      selection = 0;
      for (z = 0; z < 700000; z++);
    }
    //multiplayer selection
    else if (selection == 1) {
      InitBall();
      player1 = player;
      player1.x = 0;
      player1.y = 0;
      player1.speedy = 1;

      player2 = player;
      player2.x = 0;
      player2.y = 124;
      player2.speedy = -1;
      score = 0;
      screen = 3;
      selection = 0;
      for (z = 0; z < 700000; z++);
    }
  }
  addToBufferImage(modeImage);
  
  //draw line for each selection
  switch (selection) {
  case 0:
    draw_line(54);
    break;
  case 1:
    draw_line(65);
    break;
  }
}

void gameover() {
  int z,i;
  if (btns & 8) {
    if(score>hiscore) {hiscore=score;}
    screen = 0;
    selection = 0;
    for (z = 0; z < 700000; z++);
  }
  //show score in a vertical line
  for (i = 0; i < 15; ++i)
  {
    draw_number(score, 16, (-2+(i*12))); 
  }
}

void update(void) {

  btns = getbtns();

  sw =  getsw();
  
  //we make sure that each loop is correctly timed
  if (IFS(0) & 0x100) {
    IFS(0) = 0; // Reset timer flag

    //short intro image
    if (intro < 20)
    {
      addToBufferImage(introImage);
      intro++;
    }
    //do different tasks depending on which screen we are on
    else {
      switch (screen) {
      case  0:
        main_menu();
        break;
      case  1:
        mode_menu();
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
      default:
        screen = 0;
        break;

      }
    }

    render();
  }
}
