#include <stdint.h>         /* Declarations of uint_32 and the like */
#include <pic32mx.h>        /* Declarations of system-specific addresses etc */
#include "declaration.h"    /* Declarations of project specific functions */
#include <stdbool.h>        /* To be able to use boolean */

struct Entity firstball;

//Changes all valid bits to 0
void InitBall() {
  firstball = ball;
  firstball.speedx = 2;
  firstball.speedy = 4;
}

void renderBall() {
  addToBuffer(firstball);
}

//moves ball
char moveBall() {
  // if the firstball moves out of the screen in x, bounce back
  firstball.x += firstball.speedx;
  if (((firstball.x + firstball.imageData.width) > 32) || firstball.x < 0) {
    firstball.speedx *= -1;
    firstball.x += firstball.speedx*2;
  }

  // if the firstball moves out of the screen in x, bounce back
  firstball.y += firstball.speedy;
  if (collision(firstball)){
    firstball.speedx = rand() % 5;
    firstball.speedy *= -1;
    firstball.y += firstball.speedy*2;
  }
  if (firstball.y > 128 || (firstball.y + firstball.imageData.height) < 0)
  {
    return 1;
  }
  return 0;
}
