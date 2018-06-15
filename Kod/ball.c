#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */
#include <stdbool.h>        /* To be able to use boolean */

struct Entity firstball;
int collisions;

//initialize ball with random speed, set "score" 0
void InitBall() {
  firstball = ball;
  firstball.speedx = (rand() % 5) - 2;
  if (((rand() % 10) - 5) < 0)
  {
    firstball.speedy = 4;
  } else {
    firstball.speedy = -4;
  }
  collisions = 0;
}

void renderBall() {
  addToBuffer(firstball);
}


int getCollisions(void) {
  return collisions;
}

char ballCollision() {
  struct Entity player1 = getPlayer1();
  struct Entity player2 = getPlayer2();

  if ((firstball.x + firstball.imageData.width) > player2.x && firstball.x < (player2.x + player2.imageData.width) && (firstball.y + firstball.imageData.height) > player2.y && firstball.y < (player2.y + player2.imageData.height))
  {
    return 1;
  }
  if ((firstball.x + firstball.imageData.width) > player1.x && firstball.x < (player1.x + player1.imageData.width) && (firstball.y + firstball.imageData.height) > player1.y && firstball.y < (player1.y + player1.imageData.height))
  {
    return 1;
  }
  return 0;
}

//moves ball
char moveBall() {
  int i;
  
  // if the firstball moves out of the screen in y, bounce back
  firstball.y += firstball.speedy;
  if (ballCollision()) {
    firstball.speedx = rand() % 5;
    firstball.speedy *= -1;
    firstball.y += firstball.speedy * 2;
    collisions++;


    
  }

  // if the firstball moves out of the screen in x, bounce back
  firstball.x += firstball.speedx;
  if (((firstball.x + firstball.imageData.width) > 31) || firstball.x < 1 || ballCollision()) {
    firstball.speedx *= -1;
    firstball.x += firstball.speedx * 2;
  }

  if (firstball.y > 128 || (firstball.y + firstball.imageData.height) < 0)
  {
    return 1;
  }
  return 0;
}
